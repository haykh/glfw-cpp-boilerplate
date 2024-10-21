#ifndef UTILS_PATHS_H
#define UTILS_PATHS_H

#include <filesystem>

namespace utils {

  auto exe() -> std::filesystem::path;
  auto exeDir() -> std::filesystem::path;

} // namespace utils

#endif // UTILS_PATHS_H
