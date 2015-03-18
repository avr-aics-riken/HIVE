/**
 * @file Thread.h
 * スレッド
 */
#ifndef Include_VX_Thread_h
#define Include_VX_Thread_h

#if _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

#include "CriticalSection.h"

namespace VX
{
	class Thread
	{
	public:
    	Thread() {
       		m_handle = 0;
    	}
    	virtual ~Thread() {
        	terminateThread();
    	}
    
    	// if return true, thread will end.
   		virtual bool process(void) = 0;

		void join() {
        	if (!m_handle)
            	return;
#ifdef _WIN32
			WaitForSingleObject(m_handle, INFINITE);
#else
			pthread_join(m_handle, NULL);
#endif
		}
    
	protected:
    	void go() {
        	m_handle = createThread();
    	}

	private:
    #ifdef _WIN32
    	typedef HANDLE		ThreadHandle;
	#else
    	typedef pthread_t	ThreadHandle;
	#endif
    
    	ThreadHandle m_handle;
    
    #ifdef _WIN32
    	static DWORD WINAPI threadFunc(LPVOID thisptr) {
	#else
    	static void* threadFunc(void* thisptr) {
	#endif
        	Thread* p = reinterpret_cast<Thread*>(thisptr);
        	while (!p->process());        
        	return 0;
    	}
    
    	ThreadHandle createThread() {
        	ThreadHandle hThread;
#ifdef _WIN32
        	unsigned long dwThreadId;
        	hThread = CreateThread(NULL, 0, threadFunc, this, 0, &dwThreadId);
#else
        	pthread_create(&hThread, NULL, &threadFunc, this);
#endif
        	return hThread;
    	}
    
    	void terminateThread() {
        	if (!m_handle)
            	return;
#ifdef _WIN32
        	CloseHandle(m_handle);
#else
        	pthread_cancel(m_handle);
#endif
		}
	};
}

#endif // Include_VX_Thread_h
