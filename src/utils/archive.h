#pragma once

#include <string>

namespace XEmuRun {

bool extractArchive(const std::string& archivePath, const std::string& outputDir);
bool createArchive(const std::string& directoryPath, const std::string& outputArchive);

} // namespace XEmuRun
