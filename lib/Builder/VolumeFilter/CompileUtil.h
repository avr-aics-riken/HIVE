/**
* @file CompileUtil.h
* Portable and simple runtime C compile/eval utilty.
* This utility class calls external C compiler(such like gcc) and create dll/so, then evaluate C code by calling dll method. 
*/
#ifndef __COMPILE_UTIL_H__
#define __COMPILE_UTIL_H__

#include <string>

namespace hiveutil
{

class CompileUtil
{
  public:
    CompileUtil();
    ~CompileUtil();

    bool SetCompiler(const std::string& compilerName, const std::string& compilerFlags);
    bool SetSource(const std::string& source); 

    /// Compile source code. Must be called after `SetCompiler` and `SetSource`
    bool Compile(const std::string& methodName);

    /// Evaluate code. Must be called after `Compile`
    bool Eval(void *args);

    /// Release dll resources.
    bool Release();

  private:
    std::string source_;
    std::string compilerName_;
    std::string compilerFlags_;
    bool compiled_;

    void* method_;
    void* handle_;
    std::string filename_;
};

} // namespace

#endif
