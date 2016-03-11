#include "Perf.h"

#include <cstdio>

#ifdef HIVE_WITH_PMLIB
#include "PerfMonitor.h" // PMlib

class PMon::Impl
{
public:
    Impl() : m_initialized(false) {};
    ~Impl() {};

    void Init() {
        m_pm.initialize();
        m_initialized = true;
    }

    bool Register(const char* label, PMon::PMonType type, bool exclusive) {
        if (!m_initialized) {
            fprintf(stderr, "Register should be called after Init() call\n");
            return false;
        }
        pm_lib::PerfMonitor::Type ty = pm_lib::PerfMonitor::COMM;
        if (type == PMon::PMON_COMM) {
            ty = pm_lib::PerfMonitor::COMM;
        } else if (type == PMon::PMON_CALC) {
            ty = pm_lib::PerfMonitor::CALC;
        }
        //printf("setprop: %s\n", label);
        m_pm.setProperties(label, ty, exclusive);
        return true;
    }

    bool Start(const char* label) {
        if (!m_initialized) {
            fprintf(stderr, "[PMon] WARN: Not initialized.\n");
            return false;
        }
        //printf("start: %s\n", label);
        m_pm.start(label);
        return true;
    }

    bool Stop(const char* label) {
        if (!m_initialized) {
            fprintf(stderr, "[PMon] WARN: Not initialized.\n");
            return false;
        }
        //printf("stop: %s\n", label);
        m_pm.stop(label);
        return true;
    }

    void Report() {
        if (!m_initialized) {
            fprintf(stderr, "[PMon] WARN: Not initialized.\n");
            return;
        }
        // @todo { Select output channel. }
        m_pm.gather();
        m_pm.print(stdout, "", "HIVE");
        m_pm.printDetail(stdout);
    }

private:
    pm_lib::PerfMonitor m_pm;
    bool m_initialized;
};

#else

// Create dummy impl class.
class PMon::Impl
{
public:
    Impl() : m_initialized(false) {};
    ~Impl() {};

    void Init() {
        m_initialized = true;
    }

    bool Register(const char* label, PMonType type, bool exclusive) {
        return false;
    }

    bool Start(const char* label) {
        return false;
    }

    bool Stop(const char* label) {
        return false;
    }

    void Report() {
        printf("PMlib is not avaiable in this build, thus no profing report.\n");
    }

private:
    bool m_initialized;
};

#endif

void PMon::Init()
{
    getImpl().Init();
}

bool PMon::Register(const char* label, PMonType type, bool exclusive)
{
    return getImpl().Register(label, type, exclusive);
}

bool PMon::Start(const char* label)
{
    return getImpl().Start(label);
}

bool PMon::Stop(const char* label)
{
    return getImpl().Stop(label);
}

void PMon::Report()
{
    getImpl().Report();
}

PMon::Impl& PMon::getImpl()
{
    static PMon::Impl s_imp;
    return s_imp;
}

