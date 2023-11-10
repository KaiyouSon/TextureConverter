#pragma once
#include <stdint.h>
#include <string>
#include <filesystem>

// 前方宣言
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
	enum FileType
	{
		Folder = 1,
		Png = 2,
	};

private:
	TextureConverter* mConverter;
	int32_t mState;
	bool mIsEnd;

private:
	std::string mOutputFolderDirectroy;
	std::string mResourcesDirectroy;
	std::filesystem::path mCurrentDirectroy;

private:
	// 表示関連
	void ShowFile(const std::filesystem::path& directory, uint32_t flag, int32_t depth = 0);

private: // 再帰関連
	// Resourceフォルダ内再帰してddsに変換する
	void RDToConvert(const std::filesystem::path& directory, int32_t depth = 0);
	void RDToCreate3DTexture(const std::filesystem::path& directory, int32_t depth = 0);

private:
	// その他
	void CreateFolder(const std::string path);
	std::string SearchFile(const std::filesystem::path& directory, const std::string filename);

private:
	void ChooseTypeUpdate();
	void ConverteTextureUpdate();
	void Create3DTextureUpdate();

public:
	AppSystem();
	void Update();

public:
	bool GetisEnd();

public:
	void SetConverter(TextureConverter* converter);
};

