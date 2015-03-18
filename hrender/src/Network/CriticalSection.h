/**
 * @file CriticalSection.h
 * クリティカルセクション
 */

#ifndef Include_VX_CriticalSection_h
#define Include_VX_CriticalSection_h

#if _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif


namespace VX
{
    class Sleep {
	public:
		/**
		 * @param t sleep time [ms].
		 */
		Sleep(int t) {
#ifdef _WIN32
			::Sleep(t);
#else
			usleep(t*1000);
			//pthread_yield_np();
#endif
        }
    };
    
	class CriticalSection
	{
		public:
			CriticalSection(void) {
#ifdef _WIN32
				if (InitializeCriticalSectionAndSpinCount(&this->m_cSection, 0) == 0)
					throw("Could not create a CriticalSection");
#else
				if (pthread_mutex_init(&this->m_cSection, NULL) != 0)
					throw("Could not create a CriticalSection");
#endif
			}

    		~CriticalSection(void) {
				this->WaitForFinish(); // acquire ownership
#ifdef _WIN32
				DeleteCriticalSection(&this->m_cSection);
#else
				pthread_mutex_destroy(&this->m_cSection);
#endif
			}
    
			void WaitForFinish(void) {
				while (!this->TryEnter()) {
            		Sleep(1);// wait 1ms
				}
			}
    
			void Enter(void) {
				this->WaitForFinish(); // acquire ownership
			}
	
			void Leave(void) {
#ifdef _WIN32
				LeaveCriticalSection(&this->m_cSection);
#else
				pthread_mutex_unlock(&this->m_cSection);
#endif
			}
    
			bool TryEnter(void) {
#ifdef _WIN32
				return (TryEnterCriticalSection(&this->m_cSection) == TRUE);
#else
				return (pthread_mutex_trylock(&this->m_cSection) == 0);
#endif
			}
    
		private:
    
#ifdef _WIN32
			CRITICAL_SECTION m_cSection; //!< internal system critical section object (windows)
#else
			pthread_mutex_t m_cSection; //!< internal system critical section object (*nix)
#endif
	}; 

	class CriticalSectionLock 
	{
	public:
		CriticalSectionLock(CriticalSection* pcs)
		{
			m_pcs = pcs;
			if (m_pcs) {
				m_pcs->Enter();
			}
		}

		~CriticalSectionLock()
		{
			if (m_pcs) {
				m_pcs->Leave();
			}
		}

	private:
		CriticalSection*	m_pcs;
	};

	class Lock {
		public:
    		Lock()  { }
    		~Lock() { }
	    	void Enter() { m_cs.Enter();	}
    		void Leave() { m_cs.Leave();	}
		private:
    		CriticalSection m_cs;
	};
}

#endif // Include_VX_CriticalSection_h
