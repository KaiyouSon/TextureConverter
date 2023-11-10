#include "FilePathInfo.h"

std::string FilepathInfo::CompositeFilePath(std::string extension)
{
	std::string result = directoryPath + fileName + extension;

	// ˆø”‚ª‚È‚¢‚Æ‚«
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}

std::string FilepathInfo::CompositeFilePath(std::string outpuitDirectroyPath, std::string extension)
{
	std::string result = outpuitDirectroyPath + fileName + extension;

	// ˆø”‚ª‚È‚¢‚Æ‚«
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------------------------------------------------- //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::wstring WFilepathInfo::CompositeFilePath(std::wstring extension)
{
	std::wstring result = directoryPath + fileName + extension;

	// ˆø”‚ª‚È‚¢‚Æ‚«
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}

std::wstring WFilepathInfo::CompositeFilePath(std::wstring outpuitDirectroyPath, std::wstring extension)
{
	std::wstring result = outpuitDirectroyPath + fileName + extension;

	// ˆø”‚ª‚È‚¢‚Æ‚«
	if (extension.empty() == true)
	{
		result = directoryPath + fileName + fileExt;
	}

	return result;
}

FilepathInfo& FilepathInfo::operator=(WFilepathInfo wfilePathInfo)
{
	directoryPath = std::string(wfilePathInfo.directoryPath.begin(), wfilePathInfo.directoryPath.end());
	fileName = std::string(wfilePathInfo.fileName.begin(), wfilePathInfo.fileName.end());
	fileExt = std::string(wfilePathInfo.fileExt.begin(), wfilePathInfo.fileExt.end());

	return *this;
}
