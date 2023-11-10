#pragma once
#include "FilePathInfo.h"
#include <functional>
#include <algorithm>

WFilepathInfo SeparateWFilePath(const std::wstring& filePath);

std::pair<std::string, int32_t> SplitStringAndNumber(const std::string& input);

void SortPNGFilenameToAscendingName(std::vector<std::string>& filenames);

// Debugƒrƒ‹ƒh‚É‚Ì‚İÀs‚³‚ê‚éˆ—
void ProcessAtDebugBuild(std::function<void()> lambdaFunc);