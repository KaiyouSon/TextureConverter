#include "AppSystem.h"
#include "TextureConverter.h"
#include "Util.h"
#include <iostream>
#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
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

	std::cout << "1 : フォルダを開く" << std::endl;
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
		// 変換
		ConvertToDDSTexture();
		std::cout << "変換が完了しました。\n" << std::endl;
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

	std::cout << "1 : フォルダを開く" << std::endl;
	std::cout << "2 : 前に戻る" << std::endl;
	std::cout << "0 : 終了" << std::endl;

	std::cout << std::endl;

	// 入力による分岐
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// フォルダーごと変換する場合
	if (input == 1)
	{
		ConvertTo3DTexture();
		std::cout << "作成が完了しました。\n" << std::endl;
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

// 変換関数
void AppSystem::ConvertToDDSTexture()
{
	// ファイルダイアログから選択する
	std::string folderpath = OpenFileDialog();
	if (folderpath.empty() == true)
	{
		return;
	}

	// フォルダ作成
	std::string createPath = relative(folderpath, mResourcesDirectroy).string();
	RDToCreateFolder(mOutputFolderDirectroy + "/" + createPath);

	// 再帰する
	RDToConvert(folderpath);

}
void AppSystem::ConvertTo3DTexture()
{
	// ファイルダイアログから選択する
	std::string folderpath = OpenFileDialog();
	if (folderpath.empty() == true)
	{
		return;
	}

	// フォルダ作成
	std::string createPath = relative(folderpath, mResourcesDirectroy).string();
	RDToCreateFolder(mOutputFolderDirectroy + "/" + createPath);

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

// フォルダ内を再帰する
void AppSystem::RDToConvert(const std::filesystem::path& directory)
{
	for (const auto& entry : directory_iterator(directory))
	{
		const auto& path = entry.path();
		auto relativePath = relative(path, directory);
		std::string filenameString = relativePath.filename().string();

		if (entry.is_directory())
		{
			auto folderPath = mOutputFolderDirectroy + "/" + relative(path, mResourcesDirectroy).string();

			// フォルダ作成
			CreateFolder(folderPath);

			// 再帰
			RDToConvert(path);
		}

		// 変換
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

// パスの親まで再帰し作成する
void AppSystem::RDToCreateFolder(const std::filesystem::path& directory)
{
	// フォルダが存在していれば
	if (!directory.parent_path().empty())
	{
		RDToCreateFolder(directory.parent_path());
	}

	// 既に存在してるかをチェック
	if (!exists(directory))
	{
		// 出力フォルダを作成する
		create_directory(directory);
	}
}

// その他
void AppSystem::CreateFolder(const std::string path)
{
	// 既に存在してるかをチェック
	if (!exists(path))
	{
		// 出力フォルダを作成する
		create_directory(path);
	}
}
std::string AppSystem::OpenFileDialog()
{
	std::string path = std::filesystem::current_path().string() + "\\" + mResourcesDirectroy;
	std::wstring wOpenDirectroy = std::wstring(path.begin(), path.end());

	std::string resultPath;

	IFileOpenDialog* pFileOpenDialog;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileOpenDialog))))
	{
		DWORD dwOptions;
		if (SUCCEEDED(pFileOpenDialog->GetOptions(&dwOptions)))
		{
			pFileOpenDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

			// 指定したファイルパスを初期選択状態に設定
			IShellItem* pInitFolder = NULL;
			if (SUCCEEDED(SHCreateItemFromParsingName(wOpenDirectroy.c_str(), NULL, IID_PPV_ARGS(&pInitFolder))))
			{
				pFileOpenDialog->SetFolder(pInitFolder);
				pInitFolder->Release();
			}

			if (SUCCEEDED(pFileOpenDialog->Show(NULL)))
			{
				IShellItem* pItem;
				if (SUCCEEDED(pFileOpenDialog->GetResult(&pItem)))
				{
					PWSTR pszFolderPath;
					if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath)))
					{
						std::wstring wResultPath = pszFolderPath;
						resultPath = std::string(wResultPath.begin(), wResultPath.end());
						CoTaskMemFree(pszFolderPath);
					}
					pItem->Release();
				}
			}
		}

		pFileOpenDialog->Release();
	}

	return resultPath;
}

// ゲッター
bool AppSystem::GetisEnd()
{
	return mIsEnd;
}

// セッター
void AppSystem::SetConverter(TextureConverter* converter)
{
	mConverter = converter;
}
