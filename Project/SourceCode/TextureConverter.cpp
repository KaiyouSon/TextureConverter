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


