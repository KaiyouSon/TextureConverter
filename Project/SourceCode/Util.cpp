#include "Util.h"

WFilepathInfo SeparateWFilePath(const std::wstring& filePath)
{
	WFilepathInfo result;

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

// 数字と文字列に分解する関数
std::pair<std::string, int32_t> SplitStringAndNumber(const std::string& input)
{
	std::string letters;
	int32_t number;

	// 数字が現れるまでの文字列を取得
	auto it = std::find_if(input.begin(), input.end(), ::isdigit);
	letters = std::string(input.begin(), it);

	// 数字を取得
	number = std::stoi(std::string(it, input.end()));

	return std::make_pair(letters, number);
}

void SortPNGFilenameToAscendingName(std::vector<std::string>& filenames)
{
	// 数値と文字列に分解して比較する関数
	auto compareFunction = [](const std::string& a, const std::string& b)
	{
		auto [lettersA, numberA] = SplitStringAndNumber(a);
		auto [lettersB, numberB] = SplitStringAndNumber(b);

		// 文字列部分を比較
		if (lettersA != lettersB)
		{
			return lettersA < lettersB;
		}

		// 数値部分を比較
		return numberA < numberB;
	};


	// ファイル名を比較
	std::sort(filenames.begin(), filenames.end(), compareFunction);
}

// Debugビルド時にのみ実行される処理
void ProcessAtDebugBuild(std::function<void()> lambdaFunc)
{
#ifdef _DEBUG
	lambdaFunc();
#endif
}
