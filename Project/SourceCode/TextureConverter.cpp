#include "TextureConverter.h"
#include "Util.h"
#include "Random.h"
#include <iostream>
#include <filesystem>

using namespace DirectX;
using namespace std::filesystem;

// �e�N�X�`����WIC����DDC�ɕϊ�����
void TextureConverter::ConvertTextureWICToDDC(
	const std::string& filePath,
	const std::string& outputPath)
{
	// �e�N�X�`���[���[�h
	mTextureDatas.emplace_back();

	// ���[�h
	LoadWICTextureFromFile(filePath);

	// �o�̓t�@�C������ݒ肷��
	std::string outputFilepath = mTextureDatas.back().filepathInfo.CompositeFilePath(outputPath, ".dds");

	// DDS�e�N�X�`���o��
	SaveDDSTextureToFile(mTextureDatas.back().scratchImage, outputFilepath);

	mTextureDatas.clear();
}

// �t�H���_����3D�e�N�X�`�����쐬����
void TextureConverter::ConvertTo3DTexture(
	const std::vector<std::string>& filenames,
	const std::string& deirectroyPath,
	const std::string& outputPath)
{
	std::vector<std::string> localFilenames = filenames;

	// ���O���Ƀ\�[�h����
	SortPNGFilenameToAscendingName(localFilenames);

	// ���[�h
	for (const auto& filename : localFilenames)
	{
		mTextureDatas.emplace_back();
		LoadWICTextureFromFile(deirectroyPath + filename);
	}

	ScratchImage scratchImage;

	// ����
	CombineImagesTo3DTexture(scratchImage);

	// ���k
	CompressToBC7(scratchImage);

	// �o�̓t�@�C������ݒ肷��
	std::string outputFilepath = mTextureDatas.front().filepathInfo.CompositeFilePath(outputPath, ".dds");

	// �o��
	SaveDDSTextureToFile(scratchImage, outputFilepath);

	mTextureDatas.clear();
}

// �m�C�Y�e�N�X�`���𐶐�����
void TextureConverter::CreateNoiceTexture(
	const NoiceTextureData& data,
	const std::string& filename,
	const std::string& outputPath)
{
	// �����Ƀ��U�C�N����

	DirectX::TexMetadata metadata;
	metadata.width = static_cast<uint32_t>(data.width);
	metadata.height = static_cast<uint32_t>(data.height);
	metadata.depth = 1;
	metadata.format = DXGI_FORMAT_R8_UNORM;
	metadata.mipLevels = 1;
	metadata.arraySize = 1;
	metadata.dimension = TEX_DIMENSION_TEXTURE2D;
	metadata.miscFlags = 0;
	metadata.miscFlags2 = 0;

	ScratchImage scratchImage;
	scratchImage.Initialize(metadata);

	uint8_t* imageData = scratchImage.GetPixels();

	uint32_t rowPitch = metadata.width * sizeof(uint8_t);
	for (uint32_t y = 0; y < metadata.height; y++)
	{
		for (uint32_t x = 0; x < metadata.width; x++)
		{
			imageData[x] = static_cast<uint8_t>(Random::Range(0, 255));
		}
		imageData += rowPitch;
	}

	std::string output = outputPath + filename + ".dds";
	SaveDDSTextureToFile(scratchImage, output);
}

// �e�N�X�`���t�@�C���̓ǂݍ���
void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	if (mTextureDatas.empty() == true)
	{
		return;
	}

	// �t�@�C���p�X�����C���h������ɕω�
	std::wstring wFilePath = std::wstring(filePath.begin(), filePath.end());

	// WIC�e�N�X�`�����[�h
	HRESULT result = LoadFromWICFile(
		wFilePath.c_str(),
		WIC_FLAGS_NONE,
		&mTextureDatas.back().metadata,
		mTextureDatas.back().scratchImage);
	assert(SUCCEEDED(result));

	// �t�H���_�p�X�ƃt�@�C�����𕪗�����
	mTextureDatas.back().filepathInfo = SeparateWFilePath(wFilePath);
}

// DDS�e�N�X�`���Ƃ��ăt�@�C���������o��
void TextureConverter::SaveDDSTextureToFile(DirectX::ScratchImage& scratchImage, const std::string& outputPath)
{
	// �o�̓t�@�C������ݒ肷��
	std::wstring wOutputPath = std::wstring(outputPath.begin(), outputPath.end());

	// DDS�t�@�C�������o��
	HRESULT result = SaveToDDSFile(
		scratchImage.GetImages(),
		scratchImage.GetImageCount(),
		scratchImage.GetMetadata(),
		DDS_FLAGS_NONE,
		wOutputPath.c_str());
	assert(SUCCEEDED(result));
}

// ScrateImage����������
void TextureConverter::CombineImagesTo3DTexture(ScratchImage& combinedImage)
{
	if (mTextureDatas.empty())
	{
		return;
	}

	DXGI_FORMAT format = mTextureDatas[0].scratchImage.GetMetadata().format;
	uint32_t width = mTextureDatas[0].scratchImage.GetMetadata().width;
	uint32_t height = mTextureDatas[0].scratchImage.GetMetadata().height;

	uint32_t totalDepth = mTextureDatas.size(); // 3D�e�N�X�`���̐[���i�X���C�X���j

	DirectX::TexMetadata metadata;
	metadata.width = static_cast<uint32_t>(width);
	metadata.height = static_cast<uint32_t>(height);
	metadata.depth = static_cast<uint32_t>(totalDepth);
	metadata.format = format;
	metadata.mipLevels = 1;
	metadata.arraySize = 1;
	metadata.dimension = TEX_DIMENSION_TEXTURE3D;
	metadata.miscFlags = 0;
	metadata.miscFlags2 = 0;

	// �������鏈��
	HRESULT hr = combinedImage.Initialize(metadata);
	for (size_t slice = 0; slice < totalDepth; ++slice)
	{
		const Image* srcImage = mTextureDatas[slice].scratchImage.GetImage(0, 0, 0);
		const Image* destImage = combinedImage.GetImage(0, 0, slice);
		uint32_t pixelSize = srcImage->rowPitch * srcImage->height;
		memcpy(destImage->pixels, srcImage->pixels, pixelSize);
	}
}

// --- ���k�֘A ----------------------------------------------------------------------------------------- //
void TextureConverter::CompressToBC7(DirectX::ScratchImage& scratchImage, bool useMipMap)
{
	HRESULT result;

	// �~�b�v�}�b�v����
	if (useMipMap == true)
	{
		ScratchImage mipChain;
		result = GenerateMipMaps(
			scratchImage.GetImages(),
			scratchImage.GetImageCount(),
			scratchImage.GetMetadata(),
			TEX_FILTER_DEFAULT,
			0,
			mipChain);

		if (SUCCEEDED(result))
		{
			// �C���[�W�ƃ��^�f�[�^���~�b�v�}�b�v�łŒu��������
			scratchImage = std::move(mipChain);	// �R�s�[�֎~�Ȃ̂�move����
		}
	}

	// ���k�`���ɕϊ�
	TEX_COMPRESS_FLAGS flags =
		TEX_COMPRESS_BC7_QUICK |
		TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL;

	ScratchImage converted;
	result = Compress(
		scratchImage.GetImages(),
		scratchImage.GetImageCount(),
		scratchImage.GetMetadata(),
		DXGI_FORMAT_BC7_UNORM_SRGB,
		flags,
		1.0f,
		converted);

	auto meradata = scratchImage.GetMetadata();
	meradata.format = MakeSRGB(meradata.format);

	if (SUCCEEDED(result))
	{
		scratchImage = std::move(converted);	// �R�s�[�֎~�Ȃ̂�move����
	}
}

// ���Ɖۑ�
void TextureConverter::ConvertTextureWICToDDC(const std::string& filePath, const uint32_t numOptions, char* options[])
{
	// �e�N�X�`���[���[�h
	LoadWICTextureFromFile(filePath);

	// DDS�e�N�X�`���o��
	SaveDDSTextureToFile(numOptions, options);
}
void TextureConverter::SaveDDSTextureToFile(const uint32_t numOptions, char* options[])
{
	//HRESULT result;

	//// �~�b�v�}�b�v���x���w�������
	//uint32_t mipLevel = 0;
	//for (uint32_t i = 0; i < numOptions; i++)
	//{
	//	if (std::string(options[i]) == "-ml")
	//	{
	//		// �~�b�v���x���w��
	//		mipLevel = std::stoi(options[i + 1]);
	//		break;
	//	}
	//}

	//// �~�b�v�}�b�v����
	//ScratchImage mipChain;
	//result = GenerateMipMaps(
	//	mTextureData.scratchImage.GetImages(),
	//	mTextureData.scratchImage.GetImageCount(),
	//	mTextureData.scratchImage.GetMetadata(),
	//	TEX_FILTER_DEFAULT,
	//	mipLevel,
	//	mipChain);

	//if (SUCCEEDED(result))
	//{
	//	// �C���[�W�ƃ��^�f�[�^���~�b�v�}�b�v�łŒu��������
	//	mTextureData.scratchImage = std::move(mipChain);	// �R�s�[�֎~�Ȃ̂�move����
	//	mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	//}

	//// �ǂݍ��񂾃e�N�X�`����SRGB�Ƃ��Ĉ���
	//mTextureData.metadata.format = MakeSRGB(mTextureData.metadata.format);

	//// ���k�`���ɕϊ�
	//TEX_COMPRESS_FLAGS flags =
	//	TEX_COMPRESS_BC7_QUICK |
	//	TEX_COMPRESS_SRGB_OUT |
	//	TEX_COMPRESS_PARALLEL;

	//ScratchImage converted;
	//result = Compress(
	//	mTextureData.scratchImage.GetImages(),
	//	mTextureData.scratchImage.GetImageCount(),
	//	mTextureData.scratchImage.GetMetadata(),
	//	DXGI_FORMAT_BC7_UNORM_SRGB,
	//	flags,
	//	1.0f,
	//	converted);

	//if (SUCCEEDED(result))
	//{
	//	mTextureData.scratchImage = std::move(converted);	// �R�s�[�֎~�Ȃ̂�move����
	//	mTextureData.metadata = mTextureData.scratchImage.GetMetadata();
	//}

	// �o�̓t�@�C������ݒ肷��
	//std::wstring wFilePath = mTextureData.wFilePathInfo.CompositeFilePath(L".dds");

	//// DDS�t�@�C�������o��
	//result = SaveToDDSFile(
	//	mTextureData.scratchImage.GetImages(),
	//	mTextureData.scratchImage.GetImageCount(),
	//	mTextureData.metadata,
	//	DDS_FLAGS_NONE,
	//	wFilePath.c_str());
	//assert(SUCCEEDED(result));
}
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