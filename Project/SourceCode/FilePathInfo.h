#pragma once
#include <string>

struct WFilepathInfo;

struct FilepathInfo
{
	// ディレクトリパス
	std::string directoryPath;

	// ファイル名
	std::string fileName;

	// ファイル拡張子
	std::string fileExt;

	// ファイル名を合成し返す
	std::string CompositeFilePath(std::string extension = "");
	std::string CompositeFilePath(std::string outpuitDirectroyPath, std::string extension = "");

	FilepathInfo& operator=(WFilepathInfo wfilePathInfo);
};

struct WFilepathInfo
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


