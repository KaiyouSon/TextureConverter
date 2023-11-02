#include <iostream>
#include <cstdlib>
#include <cassert>
#include <memory>

#include "TextureConverter.h"
#include "AppSystem.h"

// �R�}���g���C������
enum class Argument
{
	ApplicationPath,	// �A�v���P�[�V�����̃p�X
	FilePath,			// �n���ꂽ�t�@�C���̃p�X

	Size,
};

int main(int argc, char* argv[])
{
	//assert(argc >= (uint32_t)Argument::Size);

	// �R�}���h���C�������w��Ȃ�
	if (argc < (uint32_t)Argument::Size)
	{
		TextureConverter::OutputUsage();
		return 0;
	}

	// COM���C�u�����̏�����
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	// �e�N�X�`���R���o�[�^
	std::unique_ptr<TextureConverter> converter = std::make_unique<TextureConverter>();
	std::unique_ptr<AppSystem> appSystem = std::make_unique<AppSystem>();

	appSystem->SetConverter(converter.get());

	while (true)
	{
		appSystem->Update();
	}




	uint32_t numOptions = argc - (uint32_t)Argument::Size;
	char** options = argv + (uint32_t)Argument::Size;

	// �e�N�X�`���ϊ�
	//converter.ConvertTextureWICToDDC(argv[(uint32_t)Argument::FilePath], numOptions, options);

	// COM���C�u���C�̏������I��
	CoUninitialize();

	return 0;
}