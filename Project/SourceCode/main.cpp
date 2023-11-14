#include <iostream>
#include <cstdlib>
#include <cassert>
#include <memory>

#include "TextureConverter.h"
#include "AppSystem.h"
#include "Random.h"

// コマントライン引数
enum class Argument
{
	ApplicationPath,	// アプリケーションのパス
	FilePath,			// 渡されたファイルのパス

	Size,
};

int main(int argc, char* argv[])
{
	Random::Init();

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

		if (appSystem->GetisEnd() == true)
		{
			break;
		}
	}

	// COMライブライの初期化終了
	CoUninitialize();

	return 0;
}