#include "FilePathInfo.h"

std::wstring WFilePathInfo::CompositeFilePath(std::wstring extension)
{
	std::wstring result = directoryPath + fileName + extension;

	// �������Ȃ��Ƃ�
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}