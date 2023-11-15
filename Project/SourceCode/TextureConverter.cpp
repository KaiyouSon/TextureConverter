#include "TextureConverter.h"
#include "Util.h"
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
	const NoiceData& data,
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

	switch (data.type)
	{
	case NoiceTextureType::Mosaic:
		CreateMosaicNoice(scratchImage, data);
		break;

	case NoiceTextureType::Block:
		CreateBlockNoice(scratchImage, data);
		break;

	case NoiceTextureType::Value:
		CreateValueNoice(scratchImage, data);
		break;

	case NoiceTextureType::Perlin:
		CreatePerlinNoice(scratchImage, data);
		break;
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

// --- �m�C�Y�e�N�X�`���֘A ----------------------------------------------------------------------------- //

// ���U�C�N�m�C�Y
void TextureConverter::CreateMosaicNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data)
{
	uint8_t* imageData = scratchImage.GetPixels();

	uint32_t rowPitch = data.width * sizeof(uint8_t);
	for (uint32_t y = 0; y < data.height; y++)
	{
		for (uint32_t x = 0; x < data.width; x++)
		{
			imageData[x] = static_cast<uint8_t>(Random::Range(0, 255));
		}
		imageData += rowPitch;
	}
}

// �u���b�N�m�C�Y
void TextureConverter::CreateBlockNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data)
{
	uint8_t* imageData = scratchImage.GetPixels();

	// �e�u���b�N�ɓ��������_���Ȓl��ݒ�
	for (uint32_t blockY = 0; blockY < data.height; blockY += data.blockSize)
	{
		for (uint32_t blockX = 0; blockX < data.width; blockX += data.blockSize)
		{
			uint8_t blockValue = static_cast<uint8_t>(Random::Range(0, 255));

			// �e�u���b�N���œ����l��ݒ�
			for (uint32_t y = blockY; y < blockY + data.blockSize && y < data.height; y++)
			{
				for (uint32_t x = blockX; x < blockX + data.blockSize && x < data.width; x++)
				{
					imageData[y * (uint32_t)data.width + x] = blockValue;
				}
			}
		}
	}
}

// �o�����[�m�C�Y
void TextureConverter::CreateValueNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data)
{
	// ���u���b�N�m�C�Y�ɂ���
	CreateBlockNoice(scratchImage, data);

	// �u���b�N�m�C�Y�̌��ʂ��g���ăo�����[�m�C�Y�ɂ���
	uint8_t* imageData = scratchImage.GetPixels();

	for (uint32_t blockY = 0; blockY < data.height; blockY += data.blockSize)
	{
		for (uint32_t blockX = 0; blockX < data.width; blockX += data.blockSize)
		{
			// �e�u���b�N�̐F�擾
			uint8_t v00 = imageData[(blockY) * (uint32_t)data.width + blockX];
			uint8_t v01 = imageData[(blockY + data.blockSize) * (uint32_t)data.width + blockX];
			uint8_t v10 = imageData[(blockY) * (uint32_t)data.width + blockX + data.blockSize];
			uint8_t v11 = imageData[(blockY + data.blockSize) * (uint32_t)data.width + blockX + data.blockSize];

			// �e�u���b�N���œ����l��ݒ�
			for (uint32_t y = blockY; y < blockY + data.blockSize && y < data.height; y++)
			{
				for (uint32_t x = blockX; x < blockX + data.blockSize && x < data.width; x++)
				{
					float px = static_cast<float>(x - blockX) / static_cast<float>(data.blockSize);
					float py = static_cast<float>(y - blockY) / static_cast<float>(data.blockSize);

					float vx = px * px * (3 - 2 * px);
					float vy = py * py * (3 - 2 * py);

					float v0010 = Lerp(v00, v10, vx);
					float v0111 = Lerp(v01, v11, vx);

					uint8_t col = Lerp(static_cast<uint8_t>(v0010), static_cast<uint8_t>(v0111), vy);
					imageData[y * (uint32_t)data.width + x] = col;
				}
			}
		}
	}
}

// �p�[�����m�C�Y
void TextureConverter::CreatePerlinNoice(DirectX::ScratchImage& scratchImage, const NoiceData& data)
{
	// ���o�����[�m�C�Y�ɂ���
	//CreateValueNoice(scratchImage, data);

	// �u���b�N�m�C�Y�̌��ʂ��g���ăo�����[�m�C�Y�ɂ���
	uint8_t* imageData = scratchImage.GetPixels();

	// �u���b�N���ƂɃ����_���x�N�g���𐶐�
	uint32_t widthBlockCount = data.width / data.blockSize;
	uint32_t heightBlockCount = data.height / data.blockSize;

	// �x�N�g���i�[
	std::vector<std::vector<Vec2>> randVecs(heightBlockCount + 1, std::vector<Vec2>(widthBlockCount + 1));
	for (uint32_t y = 0; y < heightBlockCount + 1; y++)
	{
		for (uint32_t x = 0; x < widthBlockCount + 1; x++)
		{
			randVecs[y][x] = Vec2(Random::RangeF(-1, 1), Random::RangeF(-1, 1));
		}
	}

	for (uint32_t y = 0; y < data.height; y++)
	{
		for (uint32_t x = 0; x < data.width; x++)
		{
			uint32_t indexX = x / data.blockSize;
			uint32_t indexY = y / data.blockSize;

			Vec2 v00 = randVecs[indexY + 0][indexX + 0];
			Vec2 v01 = randVecs[indexY + 1][indexX + 0];
			Vec2 v10 = randVecs[indexY + 0][indexX + 1];
			Vec2 v11 = randVecs[indexY + 1][indexX + 1];

			Vec2 uv = Vec2(x / data.width, y / data.height);

			float c00 = Vec2::Dot(v00, uv - Vec2(0, 0));
			float c01 = Vec2::Dot(v01, uv - Vec2(0, 1));
			float c10 = Vec2::Dot(v10, uv - Vec2(1, 0));
			float c11 = Vec2::Dot(v11, uv - Vec2(1, 1));

			Vec2 u =
			{
				uv.x * uv.x * (3 - 2 * uv.x),
				uv.y * uv.y * (3 - 2 * uv.y),
			};

			float v0010 = Lerp(c00, c10, u.x);
			float v0111 = Lerp(c01, c11, u.x);

			float col = Lerp(v0010, v0111, u.y) * 0.5f + 0.5f;
			imageData[y * (uint32_t)data.width + x] = col * 255;
		}
	}

	for (uint32_t blockY = 0; blockY < data.height; blockY += data.blockSize)
	{
		for (uint32_t blockX = 0; blockX < data.width; blockX += data.blockSize)
		{
			// �e�u���b�N�̐F�擾
			uint32_t index00 = blockY * data.width + blockX;
			uint32_t index01 = (std::min)(blockY + data.blockSize, (uint32_t)data.height - 1) * data.width + blockX;
			uint32_t index10 = blockY * data.width + (std::min)(blockX + data.blockSize, (uint32_t)data.width - 1);
			uint32_t index11 = (std::min)(blockY + data.blockSize, (uint32_t)data.height - 1) * data.width + (std::min)(blockX + data.blockSize, (uint32_t)data.width - 1);

			uint8_t v00 = imageData[index00];
			uint8_t v01 = imageData[index01];
			uint8_t v10 = imageData[index10];
			uint8_t v11 = imageData[index11];

			// �e�u���b�N���œ����l��ݒ�
			for (uint32_t y = blockY; y < blockY + data.blockSize && y < data.height; y++)
			{
				for (uint32_t x = blockX; x < blockX + data.blockSize && x < data.width; x++)
				{
					float px = static_cast<float>(x - blockX) / static_cast<float>(data.blockSize);
					float py = static_cast<float>(y - blockY) / static_cast<float>(data.blockSize);

					float vx = px * px * (3 - 2 * px);
					float vy = py * py * (3 - 2 * py);

					float v0010 = Lerp(v00, v10, vx);
					float v0111 = Lerp(v01, v11, vx);

					uint8_t col = Lerp(static_cast<uint8_t>(v0010), static_cast<uint8_t>(v0111), vy);
					imageData[y * (uint32_t)data.width + x] = col;
				}
			}
		}
	}

	int a = 0;

	//for (uint32_t blockY = 0; blockY < data.height; blockY += data.blockSize)
	//{
	//	for (uint32_t blockX = 0; blockX < data.width; blockX += data.blockSize)
	//	{
	//		Vec2 v00 = Vec2(Random::RangeF(-1, 1), Random::RangeF(-1, 1));
	//		Vec2 v01 = Vec2(Random::RangeF(-1, 1), Random::RangeF(-1, 1));
	//		Vec2 v10 = Vec2(Random::RangeF(-1, 1), Random::RangeF(-1, 1));
	//		Vec2 v11 = Vec2(Random::RangeF(-1, 1), Random::RangeF(-1, 1));

	//		// �e�u���b�N���œ����l��ݒ�
	//		for (uint32_t y = blockY; y < blockY + data.blockSize && y < data.height; y++)
	//		{
	//			for (uint32_t x = blockX; x < blockX + data.blockSize && x < data.width; x++)
	//			{
	//				Vec2 uv = Vec2(x / data.width, y / data.height);

	//				Vec2 uvFmod =
	//				{
	//					fmod(uv.x, 1.0f),
	//					fmod(uv.y, 1.0f),
	//				};

	//				float c00 = Vec2::Dot(v00, uv - Vec2(0, 0));
	//				float c01 = Vec2::Dot(v01, uv - Vec2(0, 1));
	//				float c10 = Vec2::Dot(v10, uv - Vec2(1, 0));
	//				float c11 = Vec2::Dot(v11, uv - Vec2(1, 1));

	//				Vec2 u =
	//				{
	//					uv.x * uv.x * (3 - 2 * uv.x),
	//					uv.y * uv.y * (3 - 2 * uv.y),
	//				};

	//				float v0010 = Lerp(c00, c10, u.x);
	//				float v0111 = Lerp(c01, c11, u.x);

	//				float col = Lerp(v0010, v0111, u.y) * 0.5f + 0.5f;
	//				imageData[y * (uint32_t)data.width + x] = col * 255;
	//			}
	//		}
	//	}
	//}
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