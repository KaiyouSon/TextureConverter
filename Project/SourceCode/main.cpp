#include <iostream>
#include <cstdlib>
#include <cassert>
#include <Windows.h>

#include "TextureConverter.h"

// コマントライン引数
enum class Argument
{
	ApplicationPath,	// アプリケーションのパス
	FilePath,			// 渡されたファイルのパス

	Size,
};

int main(int argc, char* argv[])
{
	//assert(argc >= (uint32_t)Argument::Size);

	// コマンドライン引数指定なし
	if (argc < (uint32_t)Argument::Size)
	{
		TextureConverter::OutputUsage();
		return 0;
	}

	// COMライブラリの初期化
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	// テクスチャコンバータ
	TextureConverter converter;

	uint32_t numOptions = argc - (uint32_t)Argument::Size;
	char** options = argv + (uint32_t)Argument::Size;

	// テクスチャ変換
	converter.ConvertTextureWICToDDC(argv[(uint32_t)Argument::FilePath], numOptions, options);

	// COMライブライの初期化終了
	CoUninitialize();

	return 0;
}