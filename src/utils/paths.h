#ifndef UTILS_PATHS_H
#define UTILS_PATHS_H

#include <filesystem>

namespace utils::path {

  auto exe() -> std::filesystem::path;
  auto exeDir() -> std::filesystem::path;

} // namespace utils::path

#endif // UTILS_PATHS_H
