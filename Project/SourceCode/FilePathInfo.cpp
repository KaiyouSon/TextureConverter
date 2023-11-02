#include "FilePathInfo.h"

std::wstring WFilePathInfo::CompositeFilePath(std::wstring extension)
{
	std::wstring result = directoryPath + fileName + extension;

	// 引数がないとき
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}

std::wstring WFilePathInfo::CompositeFilePath(std::wstring outpuitDirectroyPath, std::wstring extension)
{
	std::wstring result = outpuitDirectroyPath + fileName + extension;

	// 引数がないとき
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}
