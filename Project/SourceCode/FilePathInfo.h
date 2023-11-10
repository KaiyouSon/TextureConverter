#pragma once
#include <string>

struct WFilepathInfo;

struct FilepathInfo
{
	// �f�B���N�g���p�X
	std::string directoryPath;

	// �t�@�C����
	std::string fileName;

	// �t�@�C���g���q
	std::string fileExt;

	// �t�@�C�������������Ԃ�
	std::string CompositeFilePath(std::string extension = "");
	std::string CompositeFilePath(std::string outpuitDirectroyPath, std::string extension = "");

	FilepathInfo& operator=(WFilepathInfo wfilePathInfo);
};

struct WFilepathInfo
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


