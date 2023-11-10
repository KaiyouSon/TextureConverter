#pragma once
#include "FilePathInfo.h"
#include <functional>
#include <algorithm>

WFilepathInfo SeparateWFilePath(const std::wstring& filePath);

std::pair<std::string, int32_t> SplitStringAndNumber(const std::string& input);

void SortPNGFilenameToAscendingName(std::vector<std::string>& filenames);

// Debugビルド時にのみ実行される処理
void ProcessAtDebugBuild(std::function<void()> lambdaFunc);