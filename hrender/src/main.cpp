/**
 * @file main.cpp
 * hrender main エントリポイント
 */
//
//  hrender
//

#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

#include "SceneScript/SceneScript.h"

#include "Core/Path.h"

#include "../../lib/Network/Connection.h"
#include "rapidjson/document.h"

#ifdef _WIN32
	#define strncasecmp(x,y,z) _strnicmp(x,y,z)

	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
	#endif

    #include <windows.h>

    namespace {
        void h_sleep (int milliseconds) {
            Sleep (milliseconds);
        }
    }
#else

    #include <time.h>

    namespace {
        void h_sleep (int milliseconds)
        {
            struct timespec ts;
            ts.tv_sec = milliseconds / 1000;
            ts.tv_nsec = milliseconds % 1000 * 1000000;
            nanosleep (&ts, NULL);
        }
    }

#endif

/**
 * sceneレンダリングコア関数
 */
void renderScene(const char* scenefile, const std::vector<std::string>& sceneargs)
{
    printf("RENDER!! > %s\n", scenefile);
    
    std::string scenepath(scenefile);
    std::string scenefullfile = convertFullpath(scenepath);
    changeFileDir(scenefullfile);
    
    SceneScript script;
    if (!script.ExecuteFile(scenefullfile.c_str(), sceneargs)) {
        fprintf(stderr, "[Error] scene file!! > %s\n", scenefullfile.c_str());
    }
}

std::string makeErrorResponse(const std::string& err, const std::string& id)
{
    std::stringstream ss;
    ss << "{\"jsonrpc\":\"2.0\", \"error\":\"" << err << "\", \"id\":\"" << id << "\"}";
    return ss.str();
}

std::string makeResultResponse(const std::string& result, const std::string& id)
{
    std::stringstream ss;
    ss << "{\"jsonrpc\":\"2.0\", \"result\":\"" << result << "\", \"id\":\"" << id << "\"}";
    return ss.str();
}


/**
 * server mode関数
 */
void serverMode(const std::string& url, const std::vector<std::string>& sceneargs)
{
    fprintf(stdout, "start up server mode!! connect to > %s\n", url.c_str());

    Connection con;
    
    std::stringstream ss;
    bool r = con.Connect(url.c_str());
    if (!r) {
        int retryCnt = 5;
        while (!r && retryCnt > 0) {
            h_sleep(1000);
            r = con.Connect(url.c_str());
            retryCnt--;
        }
        if (!r) {
            fprintf(stderr, "[Error] Connection failed.\n");
            return;
        }
    }
    
    std::string scenepath("./dummy.scn");
    std::string scenefullfile = convertFullpath(scenepath);
    changeFileDir(scenefullfile);
    SceneScript script;
    
    script.Begin(sceneargs);
    while (1) {
        std::string scenecmd = "";
        std::string json = con.Recv();
        if (json != "") {

            rapidjson::Document doc;
            if (doc.Parse<0>(json.c_str()).HasParseError()) {
                fprintf(stderr, "[Error] json parse error.\n");
            }
            
            std::string jsonrpc = doc["jsonrpc"].GetString();
            std::string id      = doc["id"].GetString();
            std::string method  = doc["method"].GetString();
            if (jsonrpc != "2.0") {
                const std::string err = "Invalid jsonrpc json.";
                con.SendText(makeErrorResponse(err, id));
            } else {
                if (method == "Command") {
                    rapidjson::Value& v = doc["param"];
                    scenecmd = v.GetString();
                    if (!script.Command(scenecmd.c_str())) {
                        const std::string err = "Failed scene command.";
                        fprintf(stderr, "[Error] %s > %s\n", err.c_str(), scenecmd.c_str());
                        con.SendText(makeErrorResponse(err, id));
                    } else {
                        con.SendText(makeResultResponse("done.", id));
                    }
                } else {
                    const std::string err = "Unkown method.";
                    con.SendText(makeErrorResponse(err, id));
                }
            }
        } else {
            h_sleep(5);
            const std::string status = con.GetState();
            
            // reconnect
            if (status == "CLOSED" || status == "NOT_CONNECTED") {
                while (!r) {
                    h_sleep(1000);
                    r = con.Connect(url.c_str());
                }
                if (!r) {
                    fprintf(stderr, "[Error] Reconnection failed.\n");
                    return;
                }
            }
        }
    }
    script.End();
}



/**
 * hrender main エントリポイント
 * @param argc コマンドライン引数数
 * @param argv コマンドライン引数文字列
 * @return 常に成功を返却する
 */

int main(int argc, char* argv[])
{
#ifdef _WIN32
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
		_CrtSetBreakAlloc(1037);
	#endif
#endif

    int rank = 0;
#ifdef HIVE_ENABLE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //printf("[MPI] rank = %d\n", rank);
#endif
    
    char* scenefile = 0;
    std::vector<std::string> sceneargs;
    std::string serverUrl = "";
    
    // Parse args
    for (int i = 1; i < argc; ++i) {
        //const char* arg = &argv[i][0];
        size_t na = strlen(argv[i]);
        if (i == 1
        && na > 4
        && (strncasecmp(&argv[i][na-4],".scn",4) == 0 || strncasecmp(&argv[i][na-4],".lua",4) == 0))
        {
            scenefile = argv[i];
        }
        if (strncasecmp(&argv[i][0],"--server:",9) == 0) {
            serverUrl = std::string(argv[i]);
            serverUrl = serverUrl.substr(9);
        }
        if (i > 1 && na < 2048) {
            sceneargs.push_back(argv[i]);
        }
    }
    
    if (argc <= 1 || (scenefile == 0 && serverUrl == "")) {
		if (rank == 0) {
			printf("\n  Usage: hrender scenefile.scn\n\n");
		}

#ifdef HIVE_ENABLE_MPI
        MPI_Finalize();
#endif
        return 0;
    }
    
    if (serverUrl != "") {
        serverMode(serverUrl, sceneargs);
    } else {
        renderScene(scenefile, sceneargs);
    }
    
    printf("Exit hrender.\n");
    
#ifdef HIVE_ENABLE_MPI
    printf("[MPI] finalize at rank: %d\n", rank);
    MPI_Finalize();
#endif
    return 0;
}

