#pragma once
#include "FilePathInfo.h"
#include "Random.h"
#include "Vec2.h"
#include <functional>
#include <algorithm>

WFilepathInfo SeparateWFilePath(const std::wstring& filePath);

std::pair<std::string, int32_t> SplitStringAndNumber(const std::string& input);

void SortPNGFilenameToAscendingName(std::vector<std::string>& filenames);

// Debugビルド時にのみ実行される処理
void ProcessAtDebugBuild(std::function<void()> lambdaFunc);

// 線形補間関数
template<typename T>
T Lerp(const T& a, const T& b, float t)
{
	return a + t * (b - a);
}