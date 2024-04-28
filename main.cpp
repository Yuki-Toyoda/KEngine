#include <Windows.h>
#include "Engine/KEngine.h"
#include "Game/Game.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ゲームクラスの生成
	KEngineFrameWork* game = new Game();

	// エンジン実行
	game->Run();

	// 終了
	return 0;
}