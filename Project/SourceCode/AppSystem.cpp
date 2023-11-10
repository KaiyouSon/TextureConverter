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

// �e�X�e�[�g�̏���
void AppSystem::ChooseTypeUpdate()
{
	std::cout << "////////////////////////" << std::endl;
	std::cout << "/// --- �I����� --- ///" << std::endl;
	std::cout << "////////////////////////" << std::endl;
	std::cout << "1 : " << "[.png]��[.dds]�ɕϊ�" << std::endl;
	std::cout << "2 : " << "3D Texture�̍쐬" << std::endl;
	std::cout << "0 : " << "�I��" << std::endl;

	std::cout << std::endl;

	// ���͂ɂ�镪��
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
	std::cout << "/// --- �e�N�X�`���ϊ� --- ///" << std::endl;
	std::cout << "//////////////////////////////" << std::endl;

	std::cout << "1 : �t�H���_���J��" << std::endl;
	std::cout << "2 : " << "�O�ɖ߂�" << std::endl;
	std::cout << "0 : " << "�I��" << std::endl;

	std::cout << std::endl;

	// ���͂ɂ�镪��
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// �t�H���_�[���ƕϊ�����ꍇ
	if (input == 1)
	{
		// �ϊ�
		ConvertToDDSTexture();
		std::cout << "�ϊ����������܂����B\n" << std::endl;
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
	std::cout << "/// --- 3D�e�N�X�`�����쐬 --- ///" << std::endl;
	std::cout << "//////////////////////////////////" << std::endl;

	std::cout << "1 : �t�H���_���J��" << std::endl;
	std::cout << "2 : �O�ɖ߂�" << std::endl;
	std::cout << "0 : �I��" << std::endl;

	std::cout << std::endl;

	// ���͂ɂ�镪��
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// �t�H���_�[���ƕϊ�����ꍇ
	if (input == 1)
	{
		ConvertTo3DTexture();
		std::cout << "�쐬���������܂����B\n" << std::endl;
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

// �ϊ��֐�
void AppSystem::ConvertToDDSTexture()
{
	// �t�@�C���_�C�A���O����I������
	std::string folderpath = OpenFileDialog();
	if (folderpath.empty() == true)
	{
		return;
	}

	// �t�H���_�쐬
	std::string createPath = relative(folderpath, mResourcesDirectroy).string();
	RDToCreateFolder(mOutputFolderDirectroy + "/" + createPath);

	// �ċA����
	RDToConvert(folderpath);

}
void AppSystem::ConvertTo3DTexture()
{
	// �t�@�C���_�C�A���O����I������
	std::string folderpath = OpenFileDialog();
	if (folderpath.empty() == true)
	{
		return;
	}

	// �t�H���_�쐬
	std::string createPath = relative(folderpath, mResourcesDirectroy).string();
	RDToCreateFolder(mOutputFolderDirectroy + "/" + createPath);

	// �t�H���_���̂��ׂĂ�png�t�@�C���̖��O���܂Ƃ߂�
	std::vector<std::string> filenames;

	// ���i�[������
	for (const auto& entry : directory_iterator(folderpath))
	{
		const auto& path = entry.path();

		// png�t�@�C���Ȃ�
		if (path.extension().string() == ".png")
		{
			filenames.push_back(path.filename().string());
		}
	}

	// �o�̓t�H���_���쐬
	std::string outputFolderPath = mOutputFolderDirectroy + "/" + relative(folderpath, mResourcesDirectroy).string();
	CreateFolder(outputFolderPath);

	// 3D�e�N�X�`�����쐬
	mConverter->ConvertTo3DTexture(filenames, folderpath + "\\", outputFolderPath + "/");
}

// �t�H���_�����ċA����
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

			// �t�H���_�쐬
			CreateFolder(folderPath);

			// �ċA
			RDToConvert(path);
		}

		// �ϊ�
		if (path.extension().string() == ".png")
		{
			std::filesystem::path outputPath = mOutputFolderDirectroy;
			outputPath /= relative(path, mResourcesDirectroy).parent_path();

			// outputPath�𕶎���ɕϊ����ďo�͂���ꍇ
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

// �p�X�̐e�܂ōċA���쐬����
void AppSystem::RDToCreateFolder(const std::filesystem::path& directory)
{
	// �t�H���_�����݂��Ă����
	if (!directory.parent_path().empty())
	{
		RDToCreateFolder(directory.parent_path());
	}

	// ���ɑ��݂��Ă邩���`�F�b�N
	if (!exists(directory))
	{
		// �o�̓t�H���_���쐬����
		create_directory(directory);
	}
}

// ���̑�
void AppSystem::CreateFolder(const std::string path)
{
	// ���ɑ��݂��Ă邩���`�F�b�N
	if (!exists(path))
	{
		// �o�̓t�H���_���쐬����
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

			// �w�肵���t�@�C���p�X�������I����Ԃɐݒ�
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

// �Q�b�^�[
bool AppSystem::GetisEnd()
{
	return mIsEnd;
}

// �Z�b�^�[
void AppSystem::SetConverter(TextureConverter* converter)
{
	mConverter = converter;
}