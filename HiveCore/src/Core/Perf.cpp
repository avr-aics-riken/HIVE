#include "Perf.h"

#if !defined (__ICC) || !defined (__INTEL_COMPILER)
#include <cstdio>
#endif

#ifdef HIVE_WITH_PMLIB
#include "PerfMonitor.h" // PMlib

#if defined (__ICC) || defined (__INTEL_COMPILER)
#include <cstdio>
#endif

#include <map>

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
            fprintf(stderr, "[PMon] WARN: Register should be called after Init() call\n");
            return false;
        }

        if (label == NULL) {
            fprintf(stderr, "[PMon] WARN: Null label\n");
            return false;
        }

        // Don't register same label
        if (m_properties.find(label) != m_properties.end()) {
            fprintf(stderr, "[PMon] WARN: Trying to re-register label %s\n", label);
            return false;
        } else {
            m_properties[label]++;
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
    std::map<std::string, int> m_properties;
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
        printf("[PMon] PMlib is not avaiable in this build, thus no profing report.\n");
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

