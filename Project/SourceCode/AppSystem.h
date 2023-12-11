#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>

// �O���錾
class TextureConverter;

class AppSystem
{
private:
	enum class State
	{
		ChooseType,
		ConverteTexture,
		Create3DTexture,
		CreateHeightTexture,
		CreateNoiceTexture,
	};

private:
	TextureConverter* mConverter;
	State mState;
	bool mIsEnd;

private:
	std::string mOutputFolderDirectroy;
	std::string mNoiceTextureOutputFolderDirectroy;
	std::string mResourcesDirectroy;
	std::filesystem::path mCurrentDirectroy;

private: // �ċA�֘A
	// Resource�t�H���_���ċA����dds�ɕϊ�����
	void RDToConvert(const std::filesystem::path& directory);
	void RDToCreateHeightTexture(const std::filesystem::path& directory);
	void RDToCreateFolder(const std::filesystem::path& directory);

private:
	// �ϊ�
	void ConvertToDDSTexture();
	void ConvertTo3DTexture();
	void CreateHeightTexture();

private:
	// ���̑�
	void CreateFolder(const std::string path);
	std::string OpenFileDialog();

private:
	void ChooseTypeUpdate();
	void ConverteTextureUpdate();
	void Create3DTextureUpdate();
	void CreateHeightTextureUpdate();
	void CreateNoiceTextureUpdate();

public:
	AppSystem();
	void Update();

public:
	bool GetisEnd();

public:
	void SetConverter(TextureConverter* converter);
};

