#include "Util.h"

WFilepathInfo SeparateWFilePath(const std::wstring& filePath)
{
	WFilepathInfo result;

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

// �����ƕ�����ɕ�������֐�
std::pair<std::string, int32_t> SplitStringAndNumber(const std::string& input)
{
	std::string letters;
	int32_t number;

	// �����������܂ł̕�������擾
	auto it = std::find_if(input.begin(), input.end(), ::isdigit);
	letters = std::string(input.begin(), it);

	// �������擾
	number = std::stoi(std::string(it, input.end()));

	return std::make_pair(letters, number);
}

void SortPNGFilenameToAscendingName(std::vector<std::string>& filenames)
{
	// ���l�ƕ�����ɕ������Ĕ�r����֐�
	auto compareFunction = [](const std::string& a, const std::string& b)
	{
		auto [lettersA, numberA] = SplitStringAndNumber(a);
		auto [lettersB, numberB] = SplitStringAndNumber(b);

		// �����񕔕����r
		if (lettersA != lettersB)
		{
			return lettersA < lettersB;
		}

		// ���l�������r
		return numberA < numberB;
	};


	// �t�@�C�������r
	std::sort(filenames.begin(), filenames.end(), compareFunction);
}

// Debug�r���h���ɂ̂ݎ��s����鏈��
void ProcessAtDebugBuild(std::function<void()> lambdaFunc)
{
#ifdef _DEBUG
	lambdaFunc();
#endif
}
