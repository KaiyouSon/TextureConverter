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

	// COMライブラリの初期化
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	// テクスチャコンバータ
	TextureConverter converter;

	// テクスチャ変換
	converter.ConvertTextureWICToDDC(argv[(uint32_t)Argument::FilePath]);

	// COMライブライの初期化終了
	CoUninitialize();

	system("pause");
	return 0;
}