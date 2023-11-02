#include <iostream>
#include <cstdlib>
#include <cassert>
#include <memory>

#include "TextureConverter.h"
#include "AppSystem.h"

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
	std::unique_ptr<TextureConverter> converter = std::make_unique<TextureConverter>();
	std::unique_ptr<AppSystem> appSystem = std::make_unique<AppSystem>();

	appSystem->SetConverter(converter.get());

	while (true)
	{
		appSystem->Update();
	}




	uint32_t numOptions = argc - (uint32_t)Argument::Size;
	char** options = argv + (uint32_t)Argument::Size;

	// テクスチャ変換
	//converter.ConvertTextureWICToDDC(argv[(uint32_t)Argument::FilePath], numOptions, options);

	// COMライブライの初期化終了
	CoUninitialize();

	return 0;
}