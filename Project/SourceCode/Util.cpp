#include "Util.h"

WFilePathInfo SeparateWFilePath(const std::wstring& filePath)
{
	WFilePathInfo result;

	// 区切り文字がないのでファイル名のみとして扱う
	result.directoryPath = L"";
	result.fileExt = L"";
	result.fileName = filePath;

	uint32_t pos1;
	std::wstring exceptExt = filePath;

	// 区切り文字 ',' が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		// 区切り文字の後ろをファイル拡張子として保存
		result.fileExt = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}

	// 区切り文字　'\\' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		// 区切り文字の前までディレクトリパスとして保存
		result.directoryPath = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		result.fileName = exceptExt.substr(pos1 + 1, filePath.size() - pos1 - 1);

		return result;
	}

	// 区切り文字　'/' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos)
	{
		// 区切り文字の前までディレクトリパスとして保存
		result.directoryPath = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		result.fileName = exceptExt.substr(pos1 + 1, filePath.size() - pos1 - 1);

		return result;
	}

	return result;
}
