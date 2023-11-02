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
	};

private:
	TextureConverter* mConverter;
	int32_t mState;
	bool mIsEnd;

private:
	std::string mOutputFolderPath;
	std::string mProjectDirectroy;
	std::filesystem::path mCurrentDirectroy;

private:
	// Resource�t�H���_���ċA����
	void RecursiveDirectory(const std::filesystem::path& directory, int32_t depth = 0);
	void CreateFolder(const std::string path);

private:
	void ChooseTypeUpdate();
	void ConverteTextureUpdate();

public:
	AppSystem();
	void Update();

public:
	bool GetisEnd();

public:
	void SetConverter(TextureConverter* converter);
};

