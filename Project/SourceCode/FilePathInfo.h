#pragma once
#include <string>

struct WFilePathInfo
{
	// ディレクトリパス
	std::wstring directoryPath;

	// ファイル名
	std::wstring fileName;

	// ファイル拡張子
	std::wstring fileExt;

	// ファイル名を合成し返す
	std::wstring CompositeFilePath(std::wstring extension = L"");
	std::wstring CompositeFilePath(std::wstring outpuitDirectroyPath, std::wstring extension = L"");
};