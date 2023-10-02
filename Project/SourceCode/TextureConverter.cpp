#include "TextureConverter.h"
#include "Util.h"
using namespace DirectX;

// �e�N�X�`����WIC����DDC�ɕϊ�����
void TextureConverter::ConvertTextureWICToDDC(const std::string& filrPath)
{
	// �e�N�X�`���[���[�h
	LoadWICTextureFromFile(filrPath);

	// DDS�e�N�X�`���o��
	SaveDDSTextureToFile();
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
void TextureConverter::SaveDDSTextureToFile()
{
	HRESULT result;

	// �~�b�v�}�b�v����
	ScratchImage mipChain;
	result = GenerateMipMaps(
		mTextureData.scratchImage.GetImages(),
		mTextureData.scratchImage.GetImageCount(),
		mTextureData.scratchImage.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
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


