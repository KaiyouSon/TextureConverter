#include "AppSystem.h"
#include "TextureConverter.h"
#include <iostream>
using namespace std::filesystem;

AppSystem::AppSystem() :
	mState(ChooseType), mIsEnd(false),
	mOutputFolderPath("dds_output"),
	mProjectDirectroy("Resources"),
	mCurrentDirectroy("Resources")
{
}

void AppSystem::Update()
{
	switch (mState)
	{
	case ChooseType:
		ChooseTypeUpdate();
		break;

	case ConverteTexture:
		ConverteTextureUpdate();
	}
}

// 各ステートの処理
void AppSystem::ChooseTypeUpdate()
{
	std::cout << "////////////////////////" << std::endl;
	std::cout << "/// --- 選択画面 --- ///" << std::endl;
	std::cout << "////////////////////////" << std::endl;
	std::cout << "1 : " << "[.png]を[.dds]に変換" << std::endl;
	std::cout << "2 : " << "3D Textureの作成" << std::endl;
	std::cout << "0 : " << "終了" << std::endl;

	// 入力による分岐
	int32_t input = 0;
	std::cin >> input;

	if (input == 1)
	{
		mState = ConverteTexture;
	}
	else if (input == 2)
	{
		mState = Create3DTexture;
	}
	else if (input == 0)
	{
		mIsEnd = true;
	}

	std::cout << std::endl;
}

void AppSystem::ConverteTextureUpdate()
{
	std::cout << "//////////////////////////////" << std::endl;
	std::cout << "/// --- テクスチャ変換 --- ///" << std::endl;
	std::cout << "//////////////////////////////" << std::endl;

	std::cout << "1 : Resourcesフォルダ内を変換する" << std::endl;
	std::cout << "2 : " << "前に戻る" << std::endl;
	std::cout << "0 : " << "終了" << std::endl;

	// 入力による分岐
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// フォルダーごと変換する場合
	if (input == 1)
	{
		// 出力フォルダを作成
		CreateFolder(mOutputFolderPath);

		// 再帰する
		RecursiveDirectory(mCurrentDirectroy);
	}

	std::cout << std::endl;
}

// Resourceフォルダ内再帰する
void AppSystem::RecursiveDirectory(const std::filesystem::path& directory, int32_t depth)
{
	for (const auto& entry : directory_iterator(directory))
	{
		const auto& path = entry.path();
		auto relativePath = relative(path, directory);
		std::string filenameString = relativePath.filename().string();

		for (int i = 0; i < depth; i++)
		{
			std::cout << "-";
		}
		std::cout << filenameString << std::endl;
		if (entry.is_directory())
		{
			auto folderPath = mOutputFolderPath + "/" + relative(path, mProjectDirectroy).string();
			// フォルダ作成
			CreateFolder(folderPath);

			// 再帰
			RecursiveDirectory(path, depth + 1);
		}

		// 変換
		auto t = path.extension().string();
		if (path.extension().string() == ".png")
		{
			std::filesystem::path outputPath = mOutputFolderPath;
			outputPath /= relative(path, mProjectDirectroy).parent_path();

			// outputPathを文字列に変換して出力する場合
			std::string outputPathString = outputPath.string();
			if (outputPathString.back() != '/')
			{
				outputPathString += "//";
			}

			mConverter->ConvertTextureWICToDDC(path.string(), outputPathString);
		}
	}
}

void AppSystem::CreateFolder(const std::string path)
{
	// 既に存在してるかをチェック
	if (!exists(path))
	{
		// 出力フォルダを作成する
		create_directory(path);
	}
}

// ゲッター
bool AppSystem::GetisEnd()
{
	return mIsEnd;
}

void AppSystem::SetConverter(TextureConverter* converter)
{
	mConverter = converter;
}
