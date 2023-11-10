#include "AppSystem.h"
#include "TextureConverter.h"
#include "Util.h"
#include <iostream>
using namespace std::filesystem;

AppSystem::AppSystem() :
	mState(ChooseType), mIsEnd(false),
	mOutputFolderDirectroy("dds_output"),
	mResourcesDirectroy("Resources"),
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
		break;

	case Create3DTexture:
		Create3DTextureUpdate();
		break;
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

	std::cout << std::endl;

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
	if (mState != ConverteTexture)
	{
		return;
	}

	std::cout << "//////////////////////////////" << std::endl;
	std::cout << "/// --- テクスチャ変換 --- ///" << std::endl;
	std::cout << "//////////////////////////////" << std::endl;

	std::cout << "1 : Resourcesフォルダ内を変換する" << std::endl;
	std::cout << "2 : " << "前に戻る" << std::endl;
	std::cout << "0 : " << "終了" << std::endl;

	std::cout << std::endl;

	// 入力による分岐
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// フォルダーごと変換する場合
	if (input == 1)
	{
		// 出力フォルダを作成
		CreateFolder(mOutputFolderDirectroy);

		// 再帰する
		RDToConvert(mCurrentDirectroy);
	}
	else if (input == 2)
	{
		mState = ChooseType;
	}
	else if (input == 0)
	{
		mIsEnd = true;
	}

	std::cout << std::endl;
}

void AppSystem::Create3DTextureUpdate()
{
	if (mState != Create3DTexture)
	{
		return;
	}

	std::cout << "//////////////////////////////////" << std::endl;
	std::cout << "/// --- 3Dテクスチャを作成 --- ///" << std::endl;
	std::cout << "//////////////////////////////////" << std::endl;

	std::cout << "1 : フォルダの一覧を表示する" << std::endl;
	std::cout << "2 : " << "前に戻る" << std::endl;
	std::cout << "0 : " << "終了" << std::endl;

	std::cout << std::endl;

	// 入力による分岐
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// フォルダーごと変換する場合
	if (input == 1)
	{
		// フォルダ表示
		std::cout << "[フォルダ一覧]" << std::endl;
		ShowFile(mResourcesDirectroy, Folder);

		std::cout << "作成するフォルダーを選択してください" << std::endl;

		std::cout << std::endl;

		std::string folderpath;
		while (true)
		{
			std::string filename;
			std::cin >> filename;
			// 選択したファイルを探しに行く
			folderpath = SearchFile(mResourcesDirectroy, filename);

			if (folderpath != "NotFind")
			{
				break;
			}
			else
			{
				std::cout << "入力したフォルダ名は見つかりませんでした" << std::endl;
			}
		}

		// フォルダ内のすべてのpngファイルの名前をまとめる
		std::vector<std::string> filenames;

		// 一回格納させる
		for (const auto& entry : directory_iterator(folderpath))
		{
			const auto& path = entry.path();

			// pngファイルなら
			if (path.extension().string() == ".png")
			{
				filenames.push_back(path.filename().string());
			}
		}

		// 出力フォルダを作成
		std::string outputFolderPath = mOutputFolderDirectroy + "/" + relative(folderpath, mResourcesDirectroy).string();
		CreateFolder(outputFolderPath);

		// 3Dテクスチャを作成
		mConverter->ConvertTo3DTexture(filenames, folderpath + "\\", outputFolderPath + "/");
	}
	else if (input == 2)
	{
		mState = ChooseType;
	}
	else if (input == 0)
	{
		mIsEnd = true;
	}

	std::cout << std::endl;
}

// 表示関連
void AppSystem::ShowFile(const std::filesystem::path& directory, uint32_t flag, int32_t depth)
{
	for (const auto& entry : directory_iterator(directory))
	{
		const auto& path = entry.path();
		auto relativePath = relative(path, directory);
		std::string filenameString = relativePath.filename().string();

		// フォルダの場合
		if (flag & Folder)
		{
			if (entry.is_directory())
			{
				for (int i = 0; i < depth; i++)
				{
					std::cout << "-";
				}

				std::cout << filenameString << std::endl;

				// 全表示まで再帰
				ShowFile(path, flag, depth + 1);
			}
		}

		// pngファイル
		if (flag & Png)
		{
			if (path.extension().string() == ".png")
			{
				for (int i = 0; i < depth; i++)
				{
					std::cout << "-";
				}

				std::cout << filenameString << std::endl;
			}
		}
	}
}

// Resourceフォルダ内再帰する
void AppSystem::RDToConvert(const std::filesystem::path& directory, int32_t depth)
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
			auto folderPath = mOutputFolderDirectroy + "/" + relative(path, mResourcesDirectroy).string();
			// フォルダ作成
			CreateFolder(folderPath);

			// 再帰
			RDToConvert(path, depth + 1);
		}

		// 変換
		auto t = path.extension().string();
		if (path.extension().string() == ".png")
		{
			std::filesystem::path outputPath = mOutputFolderDirectroy;
			outputPath /= relative(path, mResourcesDirectroy).parent_path();

			// outputPathを文字列に変換して出力する場合
			std::string outputPathString = outputPath.string();

			if (outputPathString.back() != '/')
			{
				outputPathString += "\\";
			}
			else if (outputPathString.substr(outputPathString.length() - 2) != "\\")
			{
				outputPathString += "\\";
			}
			else if (outputPathString.substr(outputPathString.length() - 2) != "//")
			{
				outputPathString += "\\";
			}

			mConverter->ConvertTextureWICToDDC(path.string(), outputPathString);
		}
	}
}

void AppSystem::RDToCreate3DTexture(const std::filesystem::path& directory, int32_t depth)
{
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

std::string AppSystem::SearchFile(const std::filesystem::path& directory, const std::string filename)
{
	for (const auto& entry : directory_iterator(directory))
	{
		const auto& path = entry.path();
		auto relativePath = relative(path, directory);
		std::string filenameString = relativePath.filename().string();

		if (filenameString == filename)
		{
			return path.string();
		}

		// フォルダの場合
		if (entry.is_directory())
		{
			std::string result = SearchFile(path, filename);
			if (result != "NotFind")
			{
				return result;
			}
		}
	}

	return "NotFind";
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
