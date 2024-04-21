#pragma once
#include <stdint.h>

/// <summary>
/// KEngine用設定ヘッダー
/// </summary>
namespace KEngine::Config {

	/// <summary>
	/// レンダリング関係パラメーター
	/// </summary>
	namespace Rendering {
		// 構造体バッファ最大数
		static const uint32_t kMaxBuffer = 744;

		// テクスチャ読み込み最大数
		static const uint32_t kMaxTexture = 256;
	}

}