#include <Windows.h>
#include "Engine/KEngine.h"
#include "App/Game.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ゲームクラスの生成
	KEngineFrameWork* game = new Game();

	// エンジン実行
	game->Run();

	// インスタンスのdelete
	delete game;

	// 終了
	return 0;
}