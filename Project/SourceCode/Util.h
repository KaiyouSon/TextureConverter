#pragma once
#include "FilePathInfo.h"
#include "Random.h"
#include "Vec2.h"
#include <functional>
#include <algorithm>

WFilepathInfo SeparateWFilePath(const std::wstring& filePath);

std::pair<std::string, int32_t> SplitStringAndNumber(const std::string& input);

void SortPNGFilenameToAscendingName(std::vector<std::string>& filenames);

// Debug�r���h���ɂ̂ݎ��s����鏈��
void ProcessAtDebugBuild(std::function<void()> lambdaFunc);

// ���`��Ԋ֐�
template<typename T>
T Lerp(const T& a, const T& b, float t)
{
	return a + t * (b - a);
}