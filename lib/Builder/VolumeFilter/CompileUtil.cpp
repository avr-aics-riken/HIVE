//
// Based on shader DLL framework from SURFACE.
//
#ifdef HIVE_ENABLE_MPI
#include <mpi.h>
#endif

#if defined(_WIN32)
#include <io.h> // _mktemp
#endif

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "CompileUtil.h"

namespace {

/// Generate unique filename. User must free memory of returned pointer when it
/// become unused.
char *GenerateUniqueFilename() {

#if defined(_WIN32)
  char basename[] = "hiveutil_shader_XXXXXX";
#else

#ifdef HIVE_ENABLE_MPI
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  char basename[1024];
  sprintf(basename, "hiveutil_shader_%08d_XXXXXX", rank);
#else
  char basename[] = "hiveutil_shader_XXXXXX";
#endif
#endif

#if defined(_WIN32)

  // NOTE: Windows can't exec DLL in Temp file.
  // char *name = strdup("hiveutil_shader.dll"); //_tempnam(NULL, prefix);

  assert(strlen(basename) < 1023);

  char buf[1024];
  strncpy(buf, basename, strlen(basename));
  buf[strlen(basename)] = '\0';

  size_t len = strlen(buf) + 1;
  int err =
      _mktemp_s(buf, len); // prefix will be overwritten with actual filename
  if (err != 0) {
    fprintf(stderr, "[CompileUtil] Failed to create unique filename.\n");
    return NULL;
  }

  printf("DBG: Unique name: %s", buf);

  char *name = strdup(buf);

#else
  // The string returned by tempnam() is allocated using malloc(3) and hence
  // should be freed by free(3).
  // char *name = tempnam(NULL, prefix);
  // if (!name) {
  //  if (errno == EINVAL) {
  //    fprintf(stderr, "Bad template parameter.\n");
  //    return NULL;
  //  } else if (errno == EEXIST) {
  //    fprintf(stderr, "Out of unique filename.\n");
  //    return NULL;
  //  } else {
  //    fprintf(stderr, "Failed to create unique name.\n");
  //    return NULL;
  //  }
  //}

  int fd = mkstemp(basename); // prefix will be overwritten with actual filename
  if (fd == -1) {
    fprintf(stderr, "[CompileUtil] Failed to create unique filename.\n");
    return NULL;
  }
  close(fd);
  int ret = unlink(basename);
  if (ret == -1) {
    fprintf(stderr, "[CompileUtil] Failed to delete file: %s\n", basename);
  }

  char *name = strdup(basename);

#endif

  return name;
}

bool LoadDLL(void** method, void** handle, const std::string &filename, const std::string& methodName) {

  (*handle) = NULL;

#ifdef _WIN32
  HMODULE module = LoadLibrary(filename.c_str());
  if (module == NULL) {
    fprintf(stderr, "[CompileUtil] Cannot find/open file: %s\n",
            filename.c_str());
    fprintf(stderr, "[CompileUtil] Err = %d\n", GetLastError());
    return false;
  }

  // Find function
  (*method) = GetProcAddress(module, methodName.c_str());
  if ((*method) == NULL) {
    fprintf(stderr, "[CompileUtil] Cannot find function %s from: %s\n",
            methodName.c_str(), filename.c_str());
    FreeLibrary(module);
    return false;
  }

#else
  std::string filepath = filename;
  (*handle) = dlopen(filepath.c_str(), RTLD_NOW);

  if ((*handle) != NULL) {
    // Will be safe to delete .so file after dlopen().
    unlink(filename.c_str());
  } else {
    if ((filename.size() > 1) &&
        ((filename[0] != '/') || (filename[0] != '.'))) {
      // try to load from current path(this might have security risk?).
      filepath = std::string("./") + filename;
      (*handle) = dlopen(filepath.c_str(), RTLD_NOW);

      // Will be safe to delete .so file after dlopen().
      unlink(filename.c_str());

      if ((*handle) == NULL) {
        fprintf(stderr, "[CompileUtil] Cannot find/open file: %s\n",
                filepath.c_str());
      }
      return false;
    } else {
      fprintf(stderr, "[CompileUtil] Cannot find/open file: %s\n",
              filename.c_str());
      return false;
    }
  }

  // Find  function
  (*method) = dlsym((*handle), methodName.c_str());
  if ((*method) == NULL) {
    fprintf(stderr, "[CompileUtil] Cannot find function %s from: %s\n",
            methodName.c_str(), filename.c_str());
    dlclose((*handle));
    return false;
  }

#endif

  return true;
}

} // namespace

namespace hiveutil {

CompileUtil::CompileUtil() : compiled_(false), handle_(NULL), method_(NULL) {

}

CompileUtil::~CompileUtil() {
    Release();
}

bool CompileUtil::SetCompiler(const std::string& compilerName, const std::string& compilerFlags) {
    compilerName_ = compilerName;
    compilerFlags_ = compilerFlags;

#ifdef __APPLE__
    compilerFlags_ += " -flat_namespace -bundle -undefined suppress ";
#elif __linux__
    compilerFlags_ += " -shared -g -v ";
#elif _WIN32
    // Assume MinGW compiler
    compilerFlags_ += " -shared -g ";
#endif

    return true;
}

bool CompileUtil::SetSource(const std::string& source) {
    source_ = source;
    return true;
}

bool CompileUtil::Compile(const std::string& methodName)
{
  // @fixme { Assume this function is not re-entrant. }
  static int sCounter = 0;

    if (source_.empty()) {
        return false;
    }

    if (compilerName_.empty()) {
        return false;
    }


  const char *src = source_.c_str();

  //
  // 1. Dump shader code to a file.
  //
  char *tempFilenameStr = GenerateUniqueFilename();
  assert(tempFilenameStr);

  std::string tempFilename = std::string(tempFilenameStr);
  tempFilename += std::string(".c"); // add suffix

  //int status = unlink(tempFilenameStr);
  //if (status == -1) {
  //  perror("unlink");
  //}
  free(tempFilenameStr);

  FILE *fp = fopen(tempFilename.c_str(), "w");
  if (!fp) {
    fprintf(stderr, "Can't open file to write.");
    return false;
  }

  // printf("Write to file: %s\n", tempFilename.c_str());

  size_t n = fwrite(source_.c_str(), 1, source_.size(), fp);
  assert(n == source_.size());
  fclose(fp);

  //
  // 2. Invoke compiler
  //

  // Generate unique filename.
  char outputFilename[2048];
#ifdef HIVE_ENABLE_MPI
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#ifdef __linux__
	// It looks on linux system it failed to compile .so into current directory through popen from dynamic module?
	// Force write .so to /tmp
  sprintf(outputFilename, "/tmp/hivefilter_%04d_%08d.so", sCounter, rank);
#else
  sprintf(outputFilename, "hivefilter_%04d_%08d.so", sCounter, rank);
#endif
#else
#ifdef __linux__
	// It looks on linux system it failed to compile .so into current directory through popen from dynamic module?
	// Force write .so to /tmp
  sprintf(outputFilename, "/tmp/hivefilter_%04d.so", sCounter);
#else
  sprintf(outputFilename, "hivefilter_%04d.so", sCounter);
#endif
#endif

  sCounter++;

  // Assume gcc like compiler
  std::stringstream ss;
  ss << compilerName_ << " " << compilerFlags_ << " " << "-o " << outputFilename << " " << tempFilename;
  std::string cmd = ss.str();

  printf("[CompileUtil] cmd: %s\n", cmd.c_str());

  // See popen(3) manual for details why calling fflush(NULL) here
  fflush(NULL);

#if defined(_WIN32)
  FILE *pfp = _popen(cmd.c_str(), "r");
#else
  FILE *pfp = popen(cmd.c_str(), "r");
#endif

  if (!pfp) {
    fprintf(stderr, "[CompileUtil] Failed to open pipe.\n");
    perror("popen");

    int status = unlink(tempFilename.c_str());
    if (status == -1) {
      perror("unlink");
    }

    return false;
  }

  char buf[4096];
  while (fgets(buf, 4095, pfp) != NULL) {
    printf("[compiler] %s", buf);
  }

  int status = unlink(tempFilename.c_str());
  if (status == -1) {
    perror("unlink");
  }

#if defined(_WIN32)
  status = _pclose(pfp);
  if (status == -1) {
    fprintf(stderr, "[CompileUtil] Failed to close pipe.\n");
    return false;
  }
#else
  status = pclose(pfp);
	printf("pclose = %d\n", status);
  if (status == -1) {
    fprintf(stderr, "[CompileUtil] Failed to close pipe.\n");
    return false;
  }
#endif

  //
  // 3. Check if compiled shader exists.
  //
  std::string binaryFilename(outputFilename);
  std::ifstream ifile(binaryFilename.c_str());
  if (!ifile) {
    fprintf(stderr, "[CompileUtil] Failed to compile code.\n");
    return false;
  }

  //
  // 4. load compiled binary(of file)
  //
  bool ret = LoadDLL(&method_, &handle_, binaryFilename, methodName);

  compiled_ = ret;
  return ret;
}

typedef void (*FuncPtr)(void *args);

bool CompileUtil::Eval(void *args)
{
    if (!compiled_ || !method_) {
        return false;
    }

    FuncPtr fn = reinterpret_cast<FuncPtr>(method_);
    fn(args);

    return true; // May OK
}

bool CompileUtil::Release() {
    if (compiled_) {

#if defined(_WIN32)
        if (handle_) {
          BOOL ret = FreeLibrary(reinterpret_cast<HMODULE>(handle_));
          assert(ret == TRUE);
          return (ret == TRUE) ? true : false;
        }

        if (!filename_.empty()) {
          int ret = unlink(filename_.c_str());
          if (ret == -1) {
            fprintf(stderr, "[CompileUtil] Failed to delete file: %s\n",
                    filename_.c_str());
          }

          filename_ = std::string("");
        }
#else
        if (handle_) {
          int ret = dlclose(handle_);
          assert(ret == 0);
          return (ret == 0) ? true : false;
        }

        if (!filename_.empty()) {
          int ret = unlink(filename_.c_str());
          if (ret == -1) {
            fprintf(stderr, "[CompileUtil] Failed to delete file: %s\n",
                    filename_.c_str());
          }

          filename_ = std::string("");
        }
#endif

        compiled_ = false;
    }

    return true;
}

} // namespace
