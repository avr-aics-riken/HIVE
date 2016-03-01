#include "Perf.h"

#ifdef HIVE_WITH_PMLIB
// @todo { Don't create global object. }
pm_lib::PerfMonitor HIVEPM;

pm_lib::PerfMonitor& GetPM()
{
    return HIVEPM;
}

void InitPM()
{
    HIVEPM.initialize();

    for (int i = 0; i < sizeof(HIVEPerfLabels) / sizeof(HIVEPerfLabels[0]); i++) {
        HIVEPM.setProperties(HIVEPerfLabels[i].label, HIVEPerfLabels[i].type, HIVEPerfLabels[i].exclusive);
    }
}

void FinalizePM()
{
    HIVEPM.gather();
    HIVEPM.print(stdout, "", "HIVE");
    HIVEPM.printDetail(stdout);
}
#endif
