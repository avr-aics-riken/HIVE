#include "PolygonAnalyzer_Lua.h"

double PolygonAnalyzer_Lua::MinX() {
    return PolygonAnalyzer::MinX();
}

double PolygonAnalyzer_Lua::MinY() {
    return PolygonAnalyzer::MinY();
}

double PolygonAnalyzer_Lua::MinZ() {
    return PolygonAnalyzer::MinZ();
}

double PolygonAnalyzer_Lua::MaxX() {
    return PolygonAnalyzer::MaxX();
}

double PolygonAnalyzer_Lua::MaxY() {
    return PolygonAnalyzer::MaxY();
}

double PolygonAnalyzer_Lua::MaxZ() {
    return PolygonAnalyzer::MaxZ();
}

bool PolygonAnalyzer_Lua::Execute(PolygonModel_Lua *model)
{
    PolygonAnalyzer::Execute(model);
    return true;
}


