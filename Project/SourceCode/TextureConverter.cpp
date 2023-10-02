#include "TextureConverter.h"
#include "Util.h"
#include <iostream>
using namespace DirectX;

// �e�N�X�`����WIC����DDC�ɕϊ�����
void TextureConverter::ConvertTextureWICToDDC(const std::string& filrPath, const uint32_t numOptions, char* options[])
{
	// �e�N�X�`���[���[�h
	LoadWICTextureFromFile(filrPath);

	// DDS�e�N�X�`���o��
	SaveDDSTextureToFile(numOptions, options);
}

// �g�p���@���o�́i�\���j����
void TextureConverter::OutputUsage()
{
	std::cout << "�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂��B" << std::endl;
	std::cout << std::endl;

	std::cout << "TextureConverter [�h���C�u:][�p�X][�t�@�C����] [-ml level]" << std::endl;
	std::cout << std::endl;

	std::cout << "[�h���C�u:][�p�X][�t�@�C����]: ��ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂�" << std::endl;
	std::cout << std::endl;

	std::cout << "[-ml level]:�~�b�v���x�����w�肵�܂��B0���w�肷���1x1�܂ł̃t���~�b�v�}�b�v�`�F�[���𐶐����܂�" << std::endl;
	std::cout << std::endl;
}

// �e�N�X�`���t�@�C���̓ǂݍ���
void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	HRESULT result;

	// �t�@�C���p�X�����C���h������ɕω�
	std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());

	// WIC�e�N�X�`�����[�h
	result = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &mTextureData.metadata, mTextureData.scratchImage);
	assert(SUCCEEDED(result));

	// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	mTextureData.wFilePathInfo = SeparateWFilePath(wFilePath);
}

// DDS�e�N�X�`���Ƃ��ăt�@�C���������o��
void TextureConverter::SaveDDSTextureToFile(const uint32_t numOptions, char* options[])
{
	HRESULT result;

	// �~�b�v�}�b�v���x���w�������
	uint32_t mipLevel = 0;
	for (uint32_t i = 0; i < numOptions; i++)
	{
		if (std::string(options[i]) == "-ml")
		{
			// �~�b�v���x���w��
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}

	// �~�b�v�}�b�v����
	ScratchImage mipChain;
	result = GenerateMipMaps(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT,
		mipLevel,
		mipChain);

	if (SUCCEEDED(result))
	{
		// �C���[�W�ƃ��^�f�[�^���~�b�v�}�b�v�łŒu��������
		mTextureData.scratchImage = std::move(mipChain);	// �R�s�[�֎~�Ȃ̂�move����
		mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	}

	// ���k�`���ɕϊ�
	TEX_COMPRESS_FLAGS flags =
		TEX_COMPRESS_BC7_QUICK |
		TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL;

	ScratchImage converted;
	result = Compress(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.scratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,
		flags,
		1.0f,
		converted);

	if (SUCCEEDED(result))
	{
		mTextureData.scratchImage = std::move(converted);	// �R�s�[�֎~�Ȃ̂�move����
		mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	}

	// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	mTextureData.metadata.format = MakeSRGB(mTextureData.metadata.format);

	// �o�̓t�@�C������ݒ肷��
	std::wstring wFilePath = mTextureData.wFilePathInfo.CompositeFilePath(L".dds");

	// DDS�t�@�C�������o��
	result = SaveToDDSFile(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.metadata,
		DDS_FLAGS_NONE,
		wFilePath.c_str());
	assert(SUCCEEDED(result));
}


