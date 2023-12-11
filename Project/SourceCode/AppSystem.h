#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>

// 前方宣言
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

private: // 再帰関連
	// Resourceフォルダ内再帰してddsに変換する
	void RDToConvert(const std::filesystem::path& directory);
	void RDToCreateHeightTexture(const std::filesystem::path& directory);
	void RDToCreateFolder(const std::filesystem::path& directory);

private:
	// 変換
	void ConvertToDDSTexture();
	void ConvertTo3DTexture();
	void CreateHeightTexture();

private:
	// その他
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

