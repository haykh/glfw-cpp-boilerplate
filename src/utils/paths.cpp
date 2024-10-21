#include "paths.h"

#include <filesystem>
#include <string>

#if defined(_MSC_VER)
  #include <libloaderapi.h>
#else
  #include <unistd.h>
#endif

namespace utils {

  auto exe() -> std::filesystem::path {
#if defined(_MSC_VER)
    wchar_t path[FILENAME_MAX] = { 0 };
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    return std::filesystem::path(path);
#else
    char        path[FILENAME_MAX];
    std::size_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    return std::filesystem::path(std::string(path, (count > 0) ? count : 0));
#endif
  }

  auto exeDir() -> std::filesystem::path {
#if defined(_MSC_VER)
    wchar_t path[FILENAME_MAX] = { 0 };
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    return std::filesystem::path(path).parent_path().string();
#else
    char        path[FILENAME_MAX];
    std::size_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    return std::filesystem::path(std::string(path, (count > 0) ? count : 0))
      .parent_path()
      .string();
#endif
  }

} // namespace utils
