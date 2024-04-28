#pragma once
#include <stdint.h>

/// <summary>
/// KEngine用設定ヘッダー
/// </summary>
namespace KEngine::Config {

	/// <summary>
	/// ウィンドウ
	/// </summary>
	namespace Window {
		// ウィンドウ横幅
		static const UINT KWindowWidth  = 1280;
		// ウィンドウ縦幅
		static const UINT KWindowHeight = 720;
	}

	/// <summary>
	/// レンダリング関係パラメーター
	/// </summary>
	namespace Rendering {
		// 構造体バッファ最大数
		static const uint32_t kMaxBuffer  = 10000;

		// テクスチャ読み込み最大数
		static const uint32_t kMaxTexture = 256;
	}

}