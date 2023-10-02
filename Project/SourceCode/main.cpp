#include <iostream>
#include <cstdlib>
#include <cassert>
#include <Windows.h>

#include "TextureConverter.h"

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
	TextureConverter converter;

	uint32_t numOptions = argc - (uint32_t)Argument::Size;
	char** options = argv + (uint32_t)Argument::Size;

	// �e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDC(argv[(uint32_t)Argument::FilePath], numOptions, options);

	// COM���C�u���C�̏������I��
	CoUninitialize();

	return 0;
}