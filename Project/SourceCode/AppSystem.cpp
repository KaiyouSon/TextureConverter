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

// �e�X�e�[�g�̏���
void AppSystem::ChooseTypeUpdate()
{
	std::cout << "////////////////////////" << std::endl;
	std::cout << "/// --- �I����� --- ///" << std::endl;
	std::cout << "////////////////////////" << std::endl;
	std::cout << "1 : " << "[.png]��[.dds]�ɕϊ�" << std::endl;
	std::cout << "2 : " << "3D Texture�̍쐬" << std::endl;
	std::cout << "0 : " << "�I��" << std::endl;

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
	std::cout << "//////////////////////////////" << std::endl;
	std::cout << "/// --- �e�N�X�`���ϊ� --- ///" << std::endl;
	std::cout << "//////////////////////////////" << std::endl;

	std::cout << "1 : Resources�t�H���_����ϊ�����" << std::endl;
	std::cout << "2 : " << "�O�ɖ߂�" << std::endl;
	std::cout << "0 : " << "�I��" << std::endl;

	// ���͂ɂ�镪��
	int32_t input = 0;
	std::cin >> input;
	std::cout << std::endl;

	// �t�H���_�[���ƕϊ�����ꍇ
	if (input == 1)
	{
		// �o�̓t�H���_���쐬
		CreateFolder(mOutputFolderPath);

		// �ċA����
		RecursiveDirectory(mCurrentDirectroy);
	}

	std::cout << std::endl;
}

// Resource�t�H���_���ċA����
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
			// �t�H���_�쐬
			CreateFolder(folderPath);

			// �ċA
			RecursiveDirectory(path, depth + 1);
		}

		// �ϊ�
		auto t = path.extension().string();
		if (path.extension().string() == ".png")
		{
			std::filesystem::path outputPath = mOutputFolderPath;
			outputPath /= relative(path, mProjectDirectroy).parent_path();

			// outputPath�𕶎���ɕϊ����ďo�͂���ꍇ
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
	// ���ɑ��݂��Ă邩���`�F�b�N
	if (!exists(path))
	{
		// �o�̓t�H���_���쐬����
		create_directory(path);
	}
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
