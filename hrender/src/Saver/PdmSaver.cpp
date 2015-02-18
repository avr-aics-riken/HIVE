#include "PdmSaver.h"

#ifndef HIVE_WITH_PDMLIB
#error "HIVE_WITH_PDMLIB must be defined when you compile CDMSaver module"
#endif

#include <cstring>

#include "PDMlib.h"

PDMSaver::PDMSaver(){}
PDMSaver::~PDMSaver(){};

bool PDMSaver::Save(const char* filename){
	// @todo { error check. }

    // Initialize PMDlib
    {
        const std::string dfiName = std::string(filename) + ".dfi";
        PDMlib::PDMlib::GetInstance().Init(0, NULL, dfiName.c_str());
        PDMlib::PDMlib::GetInstance().SetBaseFileName(filename); // @fixme

        PDMlib::ContainerInfo CoordinateInfo = {"Coordinate", "N/A", "none", PDMlib::FLOAT, "geo", 3, PDMlib::IJKN};
        //PDMlib::ContainerInfo IDInfo         = {"ParticleID", "ID_number", "zip", PDMlib::INT32, "id", 1};
        // PDMlib::ContainerInfo Velocity    = {"Velocity", "N/A", "none", PDMlib::DOUBLE, "vel", 3, PDMlib::IJKN};
        // PDMlib::ContainerInfo Temparature = {"Temparature", "N/A", "none", PDMlib::DOUBLE, "temp", 1};

        //PDMlib::PDMlib::GetInstance().AddContainer(IDInfo);
        PDMlib::PDMlib::GetInstance().AddContainer(CoordinateInfo);
        //PDMlib::PDMlib::GetInstance().AddContainer(Velocity);
        //PDMlib::PDMlib::GetInstance().AddContainer(Temparature);
        //double bbox[6] = {0, 0, 0, 3*NumData, 3*NumData, 3*NumData};
        //PDMlib::PDMlib::GetInstance().SetBoundingBox(bbox);
    }

    float* coordPtr = m_pointData->Position()->GetBuffer();
    PDMlib::PDMlib::GetInstance().Write("Coordinate", m_numPoints, coordPtr, (float*)NULL, 3, /* timestep */0, /* time */0); // @todo { Write timestep and time }.

	return false;
}
