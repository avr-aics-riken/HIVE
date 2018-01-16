/*
    RenderPluginSURFACE/main.cpp
*/

#include "Commands.h"

namespace {

    void SetParallelRendering(bool enableParallel)
    {
        SetScreenParallel_SGL(enableParallel, false);
    }

    void SampleCoverage(float fsaa)
    {
        SampleCoverage_SGL(fsaa, 0);
    }

    void PixelStep(int pixelstep)
    {
        PixelStep_SGL(pixelstep);
    }

}

// LUA export

//#include <lua.h>
// TODO!!

/*int RenderPluginSURFACE(lua_XXX) {
   
    //TODO
 
}*/
