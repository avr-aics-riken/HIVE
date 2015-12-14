#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdint.h>

#include "happyhttp.h"
#include "easywsclient.hpp"

#include "nn.hpp"
#include <nanomsg/pair.h>

#include "Connection.h"
#include <queue>

#ifndef SINGLE_THREAD_RECV
#include "Thread.h"
#include "CriticalSection.h"
#endif

namespace {
    
#ifdef SINGLE_THREAD_RECV
    // Assume single instance for websocket.
    std::string gRecvBuffer;

    void RecvCallback(const std::string& message) {
        //printf("msg: %s\n", message.c_str());
        gRecvBuffer = message;
    }
    
#else
    static VX::CriticalSection g_wsclientCS;
    
    class WsRecvThread : public VX::Thread
    {
    public:
        WsRecvThread(easywsclient::WebSocket::pointer ws) : VX::Thread(), m_ws(ws) { go(); }
        ~WsRecvThread() {}
        
        bool process() {
            
            //g_wsclientCS.Enter();
            const easywsclient::WebSocket::readyStateValues rs = m_ws->getReadyState();
            //g_wsclientCS.Leave();
            if (rs == easywsclient::WebSocket::CLOSED) {
                return true; // exit
            }
            
            // Assume single thread, single instance execution.
            g_wsclientCS.Enter();
            m_ws->poll();
            m_ws->dispatch(recvCallback);
            g_wsclientCS.Leave();
            
            return false; // retry
        }
        std::string Pop() {
            m_recvCS.Enter();
            size_t num = s_recvBuffer.size();
            m_recvCS.Leave();
            if (num == 0) {
                return "";
            }
            m_recvCS.Enter();
            std::string msg = s_recvBuffer.front();
            s_recvBuffer.pop();
            m_recvCS.Leave();
            return msg;
        }
        
    private:
        easywsclient::WebSocket::pointer m_ws;
        static VX::CriticalSection m_recvCS;
        static std::queue<std::string> s_recvBuffer;
        static void recvCallback(const std::string& message) {
            //printf("WEBSOCKET RECV: %s\n", message.c_str());
            m_recvCS.Enter();
            s_recvBuffer.push(message);
            m_recvCS.Leave();
        }
    };
    VX::CriticalSection WsRecvThread::m_recvCS;
    std::queue<std::string> WsRecvThread::s_recvBuffer;

    class WsSendThread : public VX::Thread
    {
    public:
        WsSendThread(easywsclient::WebSocket::pointer ws) : VX::Thread(), m_ws(ws) { go(); }
        ~WsSendThread() {}
        
        bool process() {
            
            //g_wsclientCS.Enter(); // NOT NEED?
            const easywsclient::WebSocket::readyStateValues rs = m_ws->getReadyState();
            //g_wsclientCS.Leave();
            if (rs == easywsclient::WebSocket::CLOSED) {
                return true; // exit
            }
            
            // Assume single thread, single instance execution.
            
            m_sendCS.Enter();
            const size_t num = s_sendBuffer.size();
            std::string msg;
            if (num != 0) {
                msg = s_sendBuffer.front();
                s_sendBuffer.pop();
            }
            m_sendCS.Leave();
            
            if (msg != std::string("")) {
                g_wsclientCS.Enter();
                m_ws->send(msg);
                m_ws->poll();
                g_wsclientCS.Leave();
            }
            
            return false; // retry
        }
        void Send(const std::string& msg) {
            m_sendCS.Enter();
            s_sendBuffer.push(msg);
            m_sendCS.Leave();
        }
        
    private:
        easywsclient::WebSocket::pointer m_ws;
        static VX::CriticalSection m_sendCS;
        static std::queue<std::string> s_sendBuffer;
    };
    VX::CriticalSection WsSendThread::m_sendCS;
    std::queue<std::string> WsSendThread::s_sendBuffer;
    
#endif
    
    // callback  for http
    void onBegin( const happyhttp::Response* r, void* userdata )
    {
    }
    // callback for http
    void onData( const happyhttp::Response* r, void* userdata, const unsigned char* data, int n )
    {
    }
    // callback for http
    void onComplete( const happyhttp::Response* r, void* userdata )
    {
    }
    int base64encode(char* dst, const char* src, unsigned int in_size)
    {
        const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        if (in_size == 0 || src == dst) return 0;
        
        int count = 0;
        for (int i = 0; i < (in_size - 1) / 3 + 1; i++) {
            const unsigned char* c = reinterpret_cast<const unsigned char*>(&src[i*3]);
            for (int j = 0; j < 4; j++)
                if (in_size >= i * 3 + j) {
                    dst[count++] = b64[(c[j-1]<<(6-j*2) | c[j]>>(j*2+2)) & 0x3f];
                }
                else dst[count++] = '=';
        }
        dst[count] = '\0';
        return count;
    }
    
} // anonymouse namespace

/**
 * default implementaion
 */
class Impl : public ConnectionIF
{
public:
    Impl()
    {
        m_connection = NULL;
        m_ws = NULL;
        m_nn = NULL;
        m_timeout = 1000;
#ifndef SINGLE_THREAD_RECV
        m_wsrecvthread = NULL;
        m_wssendthread = NULL;
#endif
    }
    
    ~Impl()
    {
#ifndef SINGLE_THREAD_RECV
        delete m_wsrecvthread;
        delete m_wssendthread;
#endif
        Close();
        delete m_connection;
        m_connection = NULL;
        delete m_ws;
        m_ws = NULL;
        delete m_nn;
        m_nn = NULL;
    }

    bool Connect(const std::string& url)
    {
        if (m_connection) {
            if (!Close()) {
                return false;
            }
        }
        printf("connect > %s  \n", url.c_str());
        if (url.find("http://") != std::string::npos) {
            return connectHTTP(url);
        } else if (url.find("ws://") != std::string::npos) {
            return connectWS(url);
        } else {
            return connectNN(url);
        }
        return false;
    }
    
    bool SendText(const std::string& text)
    {
        if (m_connection) {
            const unsigned char* body = reinterpret_cast<const unsigned char*>(text.c_str());
            const int size = static_cast<int>(text.size());
            const char* header[] = {
                "Content-type", "text/plain", 0
            };
            happyhttp::Error error = m_connection->request("POST", "/postparam", header, body, size);
            if (error) {
                fprintf(stderr, "Connection Error: %s¥n", error->c_str());
                return false;
            }
            return true;
        } else if (m_ws) {
#ifdef SINGLE_THREAD_SEND
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Enter();
#endif
            m_ws->send(text);
            m_ws->poll();
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Leave();
#endif
#else
            m_wssendthread->Send(text);
#endif
            
            return true;
        } else if (m_nn) {
            const unsigned char* body = reinterpret_cast<const unsigned char*>(text.c_str());
            const int size = static_cast<int>(text.size());
            m_nn->send(body, size, 0);
            return true;
        }
        return false;
    }
    
    bool SendJSON(const std::string& json)
    {
        if (m_connection) {
            const unsigned char* body = reinterpret_cast<const unsigned char*>(json.c_str());
            const int size = static_cast<int>(json.size());
            const char* header[] = {
                "Content-type", "application/json", 0
            };
            happyhttp::Error error = m_connection->request("POST", "/postparam", header, body, size);
            if (error) {
                fprintf(stderr, "Connection Error: %s\n", error->c_str());
                return false;
            }
            return true;
        } else if (m_ws) {
#ifdef SINGLE_THREAD_SEND
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Enter();
#endif
            m_ws->send(json);
            m_ws->poll();
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Leave();
#endif
#else
            m_wssendthread->Send(json);
#endif

            return true;
        } else if (m_nn) {
            const unsigned char* body = reinterpret_cast<const unsigned char*>(json.c_str());
            const int size = static_cast<int>(json.size());
            m_nn->send(body, size, 0);
            return true;
        }
        return false;
    }
    
    bool SendBinary(const Binary binary, int size)
    {
        if (m_connection) {
            const unsigned char* body = reinterpret_cast<const unsigned char*>(binary);
            const char* header[] = {
                "Content-type", "application/octet-stream", 0
            };
            happyhttp::Error error = m_connection->request("POST", "/postparam", header, body, size);
            if (error) {
                fprintf(stderr, "Connection Error: %s\n", error->c_str());
                return false;
            }
            return true;
        } else if (m_ws) {
            std::string body(reinterpret_cast<const char*>(binary), size);
#ifdef SINGLE_THREAD_SEND
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Enter();
#endif
            m_ws->sendBinary(body);
            m_ws->poll();
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Leave();
#endif
#else
            m_wssendthread->Send(body);
#endif

            return true;
        } else if (m_nn) {
            m_nn->send(binary, size, 0);///NN_DONTWAIT);
            return true;
        }
        return false;
    }
    
    bool SendImage(const std::string& filepath)
    {
        printf("send image : %s\n", filepath.c_str());
        
        // load image
        std::ifstream ifs(filepath.c_str(), std::ios::in | std::ios::binary);
        if (!ifs.good()) {
            fprintf(stderr, "File Open Error");
            return false;
        }
        ifs.seekg(0, std::ifstream::end);
        const unsigned int fsize = ifs.tellg();
        ifs.seekg(0, std::ifstream::beg);
        char* buffer = new char[fsize];
        ifs.read(buffer, fsize);
        
        // HTTP
        if (m_connection) {
            // base64 encoding
            char* base64buffer = new char[fsize * 2];
            const int base64size = base64encode(base64buffer, buffer, fsize);
            
            // send data
            const char* header[] = {
                "Content-type", "image", 0
            };
            unsigned char* body = reinterpret_cast<unsigned char*>(base64buffer);
            happyhttp::Error error = m_connection->request("POST", "/postparam", header, body, base64size);
            delete [] buffer;
            delete [] base64buffer;
            if (error) {
                fprintf(stderr, "Connection Error: %s\n", error->c_str());
                return false;
            }
            return true;
        }
        // Websocket
        else if (m_ws) {
            std::string binary(buffer, fsize);
#ifdef SINGLE_THREAD_SEND
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Enter();
#endif
            m_ws->sendBinary(binary);
            m_ws->poll();
#ifndef SINGLE_THREAD_RECV
            g_wsclientCS.Leave();
#endif
#else
            m_wssendthread->Send(binary);
#endif
            delete [] buffer;
            return true;
        } else if (m_nn) {
            m_nn->send(buffer, fsize, 0);
            delete [] buffer;
            return true;
        }
        return false;
    }
    
    
    std::string GetState() {
        if (m_connection) {
            
            // TODO:
            fprintf(stderr, "Not implemented yet");
            
            return std::string("NOT_IMPLEMETED_YET");
            
        } else if (m_ws) {
#ifndef SINGLE_THREAD_RECV
            //g_wsclientCS.Enter(); // Now ignore, because too slow!! : TODO-> cache state!
#endif
            easywsclient::WebSocket::readyStateValues state = m_ws->getReadyState();
#ifndef SINGLE_THREAD_RECV
            //g_wsclientCS.Leave();
#endif
            if (state == easywsclient::WebSocket::CLOSED) {
                return std::string("CLOSED");
            } else if (state == easywsclient::WebSocket::CLOSING) {
                return std::string("CLOSING");
            } else if (state == easywsclient::WebSocket::CONNECTING) {
                return std::string("CONNECTING");
            } else if (state == easywsclient::WebSocket::OPEN) {
                return std::string("OPEN");
            } else {
                return std::string("UNKNOWN");
            }
        } else if (m_nn) {
            return std::string("OPEN");
        } else {
            return std::string("NOT_CONNECTED");
        }
    }
    
    std::string Recv() {
        if (m_connection) {
            fprintf(stderr, "No connection Error");
            return "NOT_IMPLEMETED_YET";
        } else if (m_ws) {
#ifdef SINGLE_THREAD_RECV
            easywsclient::WebSocket::readyStateValues state = m_ws->getReadyState();
            if (state == easywsclient::WebSocket::CLOSED) {
                return "";
            }
            
            // Assume single thread, single instance execution.
            gRecvBuffer.clear();

            m_ws->poll(m_timeout);
            m_ws->dispatch(RecvCallback);

            return gRecvBuffer;
#else
            std::string msg = m_wsrecvthread->Pop();
            return msg;
#endif
        } else if (m_nn) {
            static const int size = 16*1024;
            static char cbuf[size];
            m_nn->recv(cbuf, size, 0);
            return cbuf;
        }
        return "";
    }
    
    bool SetTimeout(int timeout) {
        if (timeout < 0) {
            return false;
        }
        m_timeout = timeout;
        return true;
    }
    
    bool Close()
    {
        if (m_connection) {
            m_connection->close();
            delete m_connection;
            m_connection = NULL;
        }
        if (m_ws) {
#ifndef SINGLE_THREAD_RECV
            delete m_wsrecvthread;
            m_wsrecvthread = NULL;
            delete m_wssendthread;
            m_wssendthread = NULL;
#endif
            m_ws->close();
            /*
             don't delete m_ws for reconnection.
             */
        }
        return true;
    }
    
private:
    typedef std::vector<std::string> SURL;
    void separateURL(SURL& dst, int& dstPort, const std::string& url) {
        int pos = static_cast<int>(url.find("http://"));
        int startHost = pos + 7;
        if (pos != std::string::npos) {
            dst.push_back(url.substr(pos, startHost));
            std::string second = url.substr(startHost, url.size() - startHost);
            if (second.find(':') != std::string::npos) {
                // find port
                int colon = second.find(':');
                int endHost = second.find_first_of("/");
                if (endHost == std::string::npos) {
                    endHost = second.size();
                }
                std::string port = second.substr(colon+1, endHost-(colon+1));
                std::stringstream ss;
                ss << port;
                ss >> dstPort;
                // make url without port
                std::string host = second.substr(0, colon);
                std::string other;
                if (endHost < second.size()) {
                    other = second.substr(endHost+1, second.size() - (endHost+1));
                }
                dst.push_back(host + other);
            }
            else
            {
                // use default port
                dst.push_back(second);
            }
        }
    }
    
    bool connectHTTP(const std::string& url) {
        SURL surl;
        int port = 8080;
        separateURL(surl, port, url);
        m_connection = new happyhttp::Connection(surl[1].c_str(), port);
        m_connection->setcallbacks( onBegin, onData, onComplete, 0 );
        happyhttp::Error error = m_connection->connect();
        if (error) {
            fprintf(stderr, "Connection Error: %s\n", error->c_str());
            return false;
        }
        while (m_connection->outstanding()) {
            happyhttp::Error err = m_connection->pump();
            if (err) {
                fprintf(stderr, "Error: %s\n", err->c_str());
                return false;
            }
        }
        return true;
    }
    
    bool connectWS(const std::string& url) {
        m_ws = easywsclient::WebSocket::from_url(url);
        if (!m_ws)
            return false;
#ifndef SINGLE_THREAD_RECV
        m_wsrecvthread = new WsRecvThread(m_ws);
        m_wssendthread = new WsSendThread(m_ws);
#endif
        return true;
    }
    
    bool connectNN(const std::string& url) {
        m_nn = new nn::socket(AF_SP, NN_PAIR);
        if (m_nn->connect(url.c_str()) < 0)
            return false;
        return true;
    }

    easywsclient::WebSocket::pointer m_ws;
    happyhttp::Connection* m_connection;
    nn::socket* m_nn;
    std::string m_recvBuf;
    int m_timeout;
#ifndef SINGLE_THREAD_RECV
    WsRecvThread* m_wsrecvthread;
    WsSendThread* m_wssendthread;
#endif
};

/**
 * MPI implementaion
 */
#ifdef HIVE_ENABLE_MPI
class MPIImpl : public ConnectionIF
{
public:
    MPIImpl() {}
    ~MPIImpl() {}
    
    bool isValid() const {
        int rank = 0;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        return (rank == 0);
    }
    
    bool Connect(const std::string& url)
    {
        if (!isValid()) { return false; }
        return m_imp.Connect(url);
    }
    
    bool SendText(const std::string& text)
    {
        if (!isValid()) { return false; }
        return m_imp.SendText(text);
    }
    
    bool SendJSON(const std::string& text)
    {
        if (!isValid()) { return false; }
        return m_imp.SendText(text);
    }
    
    bool SendBinary(const Binary binary, int size)
    {
        if (!isValid()) { return false; }
        return m_imp.SendBinary(binary, size);
    }
    
    bool SendImage(const std::string& filepath)
    {
        if (!isValid()) { return false; }
        return m_imp.SendImage(filepath);
    }
    
    std::string Recv() {
        int rank = 0;
        int nnodes = 0;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nnodes);
        
        std::string buf;
        int len = 0;
        if (rank == 0) {
            buf = m_imp.Recv();
            len = buf.size();
        }

		if (len == 0) {
			return "";
		}

		if (nnodes > 1) {
        
			MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
			if (len > 0) {
				if (rank != 0) {
					buf.resize(len);
				}
				MPI_Bcast(&buf.at(0), len, MPI_BYTE, 0, MPI_COMM_WORLD);
			}
			
			if (len > 0) {
				std::string msg = buf;
				return msg;
			} else {
				return "";
			}
		} else {
			std::string msg = buf;
			return msg;
		}
    }
    
    std::string GetState() {
        return m_imp.GetState();
    }
    
    bool SetTimeout(int timeout) {
        return m_imp.SetTimeout(timeout);
    }
    
    //int RecvSize() const {
    //    return static_cast<int>(m_recvBuf.size());
    //}
    
    bool Close() {
        return m_imp.Close();
    }
    
private:
    Impl m_imp;
};
#endif // HIVE_ENABLE_MPI

/// constructor.
/// switch mpi or default impl
Connection::Connection()
{
#ifdef HIVE_ENABLE_MPI
    m_imp = new MPIImpl();
#else
    m_imp = new Impl();
#endif
}

/// destructor
Connection::~Connection()
{
    delete m_imp;
}

bool Connection::Connect(const std::string& url)
{
    return m_imp->Connect(url);
}

bool Connection::SendText(const std::string& text)
{
    return m_imp->SendText(text);
}

bool Connection::SendJSON(const std::string& text)
{
    return m_imp->SendJSON(text);
}

bool Connection::SendBinary(const Binary binary, int size)
{
    return m_imp->SendBinary(binary, size);
}

bool Connection::SendImage(const std::string& filepath)
{
    return m_imp->SendImage(filepath);
}

std::string Connection::Recv()
{
    return m_imp->Recv();
}

std::string Connection::GetState()
{
    return m_imp->GetState();
}

bool Connection::Close()
{
    return m_imp->Close();
}

bool Connection::SetTimeout(int timeout)
{
    return m_imp->SetTimeout(timeout);
}

