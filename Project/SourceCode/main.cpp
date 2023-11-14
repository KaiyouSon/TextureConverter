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
	// COM���C�u�����̏�����
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	// �e�N�X�`���R���o�[�^
	std::unique_ptr<TextureConverter> converter = std::make_unique<TextureConverter>();
	std::unique_ptr<AppSystem> appSystem = std::make_unique<AppSystem>();

	appSystem->SetConverter(converter.get());
	converter->CreateNoiceTexture();

	while (true)
	{
		appSystem->Update();

		if (appSystem->GetisEnd() == true)
		{
			break;
		}
	}

	// COM���C�u���C�̏������I��
	CoUninitialize();

	return 0;
}