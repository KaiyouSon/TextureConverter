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

	// COM���C�u�����̏�����
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	// �e�N�X�`���R���o�[�^
	TextureConverter converter;

	// �e�N�X�`���ϊ�
	converter.ConvertTextureWICToDDC(argv[(uint32_t)Argument::FilePath]);

	// COM���C�u���C�̏������I��
	CoUninitialize();

	system("pause");
	return 0;
}