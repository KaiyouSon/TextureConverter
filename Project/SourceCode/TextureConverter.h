#pragma once
#include <string>
#include <vector>
#include "TextureData.h"

class TextureConverter
{
private:
	std::vector<TextureData> mTextureDatas;

private:
	// �e�N�X�`���t�@�C���̓ǂݍ���
	void LoadWICTextureFromFile(const std::string& filePath);

	// DDS�e�N�X�`���Ƃ��ăt�@�C���������o��
	void SaveDDSTextureToFile(DirectX::ScratchImage& scratchImage, const std::string& outputPath);

private: // ���k�֘A
	void CompressToBC7(DirectX::ScratchImage& scratchImage, bool useMipMap = true);

private: // ���̑�

	// ScrateImage����������
	void CombineImagesTo3DTexture(DirectX::ScratchImage& combinedImage);


public: // �@�\

	// �e�N�X�`����WIC����DDC�ɕϊ�����
	void ConvertTextureWICToDDC(
		const std::string& filePath,
		const std::string& outputPath);

	// �������̃e�N�X�`����3D�e�N�X�`���ɂ���
	void ConvertTo3DTexture(
		const std::vector<std::string>& filenames,
		const std::string& deirectroyPath,
		const std::string& outputPath);

	// �m�C�Y�e�N�X�`���𐶐�����
	void CreateNoiceTexture();

public:
	// ���ƃR�[�h
	void ConvertTextureWICToDDC(const std::string& filePath, const uint32_t numOptions = 0, char* options[] = nullptr);
	void SaveDDSTextureToFile(const uint32_t numOptions, char* options[]);

public:
	// �g�p���@���o�́i�\���j����
	static void OutputUsage();

};

