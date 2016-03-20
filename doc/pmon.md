# Performance monitoring

HIVE supports performance profiling using PMlib
To use this feature, HIVE must be built with PMlib support(`HIVE_BUILD_WITH_PMLIB`)

## Adding profiling point

### C/C++ 

`PMon` singleton class is provided.
Firstly the application must initlialize `PMon` using `PMon::Init()` between MPI initialization and SceneScript evaluation.

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    PMon::Init()

    SceneScript script;
    script.ExecuteFile(...)


Then call `PMon::Report()` to report profiling result(into stdout). This function must be called before `MPI_Finalize()`.
    
    PMon::Report()

    MPI_Finalize();


Before running SceneScript or calling render method, the application need to register profiling point using `PMon::Register`.
When the function has nested profiling point, `exclusive` must be true in `PMon::Register`.
`PMon::Start` and `PMon::Stop` is used for specifing the beginning of the measurement and the end of the measurement.

### Lua

A macro to register and start/stop profiling point is provided for functions in Lua bindig.

     LUA_SCRIPTCLASS_METHOD_MEASURE_CALC_ARG1("OBJLoader",bool,Load,const char*)

In this example, a profiling label `OBJLoader::Load` is created and registered when loading module, and the macro also inserts `PMon::Start()` and `PMon::Stop()` when calling `Load` function in SceneScript.

Use `LUA_SCRIPTCLASS_METHOD_MEASURE_COMM_ARGx` to measure the amount of comminication for the function.

