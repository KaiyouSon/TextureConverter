#pragma once
#include <string>

struct WFilePathInfo
{
	// �f�B���N�g���p�X
	std::wstring directoryPath;

	// �t�@�C����
	std::wstring fileName;

	// �t�@�C���g���q
	std::wstring fileExt;

	// �t�@�C�������������Ԃ�
	std::wstring CompositeFilePath(std::wstring extension = L"");
	std::wstring CompositeFilePath(std::wstring outpuitDirectroyPath, std::wstring extension = L"");
};