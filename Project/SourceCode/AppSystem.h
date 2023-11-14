#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>

// �O���錾
class TextureConverter;

class AppSystem
{
private:
	enum State
	{
		ChooseType,
		ConverteTexture,
		Create3DTexture,
		CreateNoiceTexture,
	};

private:
	TextureConverter* mConverter;
	int32_t mState;
	bool mIsEnd;

private:
	std::string mOutputFolderDirectroy;
	std::string mNoiceTextureOutputFolderDirectroy;
	std::string mResourcesDirectroy;
	std::filesystem::path mCurrentDirectroy;

private: // �ċA�֘A
	// Resource�t�H���_���ċA����dds�ɕϊ�����
	void RDToConvert(const std::filesystem::path& directory);
	void RDToCreateFolder(const std::filesystem::path& directory);

private:
	// �ϊ�
	void ConvertToDDSTexture();
	void ConvertTo3DTexture();

private:
	// ���̑�
	void CreateFolder(const std::string path);
	std::string OpenFileDialog();

private:
	void ChooseTypeUpdate();
	void ConverteTextureUpdate();
	void Create3DTextureUpdate();
	void CreateNoiceTextureUpdate();

public:
	AppSystem();
	void Update();

public:
	bool GetisEnd();

public:
	void SetConverter(TextureConverter* converter);
};

