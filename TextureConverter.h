#pragma once
#include <string>

class TextureConverter
{
private:
	// �e�N�X�`���t�@�C���̓ǂݍ���
	void LoadWICTextureFromFile(const std::string& filePath);

public:
	// �e�N�X�`����WIC����DDC�ɕϊ�����
	void ConvertTextureWICToDDC(const std::string& filrPath);

};

