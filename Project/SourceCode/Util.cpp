#include "Util.h"

WFilePathInfo SeparateWFilePath(const std::wstring& filePath)
{
	WFilePathInfo result;

	// ��؂蕶�����Ȃ��̂Ńt�@�C�����݂̂Ƃ��Ĉ���
	result.directoryPath = L"";
	result.fileExt = L"";
	result.fileName = filePath;

	uint32_t pos1;
	std::wstring exceptExt = filePath;

	// ��؂蕶�� ',' ���o�Ă����ԍŌ�̕���������
	pos1 = filePath.rfind('.');
	// �������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		// ��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		result.fileExt = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// ��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filePath.substr(0, pos1);
	}

	// ��؂蕶���@'\\' ���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('\\');
	// �������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		// ��؂蕶���̑O�܂Ńf�B���N�g���p�X�Ƃ��ĕۑ�
		result.directoryPath = exceptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		result.fileName = exceptExt.substr(pos1 + 1, filePath.size() - pos1 - 1);

		return result;
	}

	// ��؂蕶���@'/' ���o�Ă����ԍŌ�̕���������
	pos1 = exceptExt.rfind('/');
	// �������q�b�g������
	if (pos1 != std::wstring::npos)
	{
		// ��؂蕶���̑O�܂Ńf�B���N�g���p�X�Ƃ��ĕۑ�
		result.directoryPath = exceptExt.substr(0, pos1 + 1);
		// ��؂蕶���̌����t�@�C�����Ƃ��ĕۑ�
		result.fileName = exceptExt.substr(pos1 + 1, filePath.size() - pos1 - 1);

		return result;
	}

	return result;
}
