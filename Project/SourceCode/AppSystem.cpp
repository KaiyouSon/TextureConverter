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

	std::cout << "1 : Resources�t�H���_����ϊ�����" << std::endl;
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
		// �o�̓t�H���_���쐬
		CreateFolder(mOutputFolderDirectroy);

		// �ċA����
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
	std::cout << "/// --- 3D�e�N�X�`�����쐬 --- ///" << std::endl;
	std::cout << "//////////////////////////////////" << std::endl;

	std::cout << "1 : �t�H���_�̈ꗗ��\������" << std::endl;
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
		// �t�H���_�\��
		std::cout << "[�t�H���_�ꗗ]" << std::endl;
		ShowFile(mResourcesDirectroy, Folder);

		std::cout << "�쐬����t�H���_�[��I�����Ă�������" << std::endl;

		std::cout << std::endl;

		std::string folderpath;
		while (true)
		{
			std::string filename;
			std::cin >> filename;
			// �I�������t�@�C����T���ɍs��
			folderpath = SearchFile(mResourcesDirectroy, filename);

			if (folderpath != "NotFind")
			{
				break;
			}
			else
			{
				std::cout << "���͂����t�H���_���͌�����܂���ł���" << std::endl;
			}
		}

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

// �\���֘A
void AppSystem::ShowFile(const std::filesystem::path& directory, uint32_t flag, int32_t depth)
{
	for (const auto& entry : directory_iterator(directory))
	{
		const auto& path = entry.path();
		auto relativePath = relative(path, directory);
		std::string filenameString = relativePath.filename().string();

		// �t�H���_�̏ꍇ
		if (flag & Folder)
		{
			if (entry.is_directory())
			{
				for (int i = 0; i < depth; i++)
				{
					std::cout << "-";
				}

				std::cout << filenameString << std::endl;

				// �S�\���܂ōċA
				ShowFile(path, flag, depth + 1);
			}
		}

		// png�t�@�C��
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

// Resource�t�H���_���ċA����
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
			// �t�H���_�쐬
			CreateFolder(folderPath);

			// �ċA
			RDToConvert(path, depth + 1);
		}

		// �ϊ�
		auto t = path.extension().string();
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

void AppSystem::RDToCreate3DTexture(const std::filesystem::path& directory, int32_t depth)
{
}

void AppSystem::CreateFolder(const std::string path)
{
	// ���ɑ��݂��Ă邩���`�F�b�N
	if (!exists(path))
	{
		// �o�̓t�H���_���쐬����
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

		// �t�H���_�̏ꍇ
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

// �Q�b�^�[
bool AppSystem::GetisEnd()
{
	return mIsEnd;
}

void AppSystem::SetConverter(TextureConverter* converter)
{
	mConverter = converter;
}
