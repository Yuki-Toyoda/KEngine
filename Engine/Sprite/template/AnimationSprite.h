#pragma once
#include "../Sprite.h"

/// <summary>
/// アニメーションするスプライトテンプレート
/// </summary>
class AnimationSprite
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="animationSheet">アニメーションシート</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="anchorPoint">中心点</param>
	/// <param name="color">色</param>
	/// <param name="animationFrame">アニメーション全体のフレーム数</param>
	/// <param name="textureHandleSize">１フレームごとのテクスチャサイズ</param>
	/// <param name="isLoop">ループするか</param>
	/// <param name="drawTime">(任意)1フレームの描画秒数(初期値 : 1.0f / 60.0f)</param>
	void Initialize(uint32_t animationSheet, Vector2 position, Vector2 size, Vector2 anchorPoint, Vector4 color, int animationFrame, Vector2 textureHandleSize, bool isLoop, float drawTime = 1.0f/ 60.0f);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // アクセッサ等

	/// <summary>
	/// アニメーションテクスチャセッター
	/// </summary>
	/// <param name="animationSheet">変更するテクスチャ</param>
	/// <param name="textureSize">変更するテクスチャサイズ</param>
	/// <param name="isReplay">最初から再生するか</param>
	void ChangeAnimationSheets(uint32_t animationSheet, Vector2 textureSize, int animationFrame, int x, int y);

	/// <summary>
	/// １フレームの描画秒数セッター
	/// </summary>
	/// <param name="drawTime">1フレーム事の描画秒数</param>
	void SetDrawTime(float drawTime) { drawTime_ = drawTime; }

	
	/// <summary>
	/// 開始フレームセッター
	/// </summary>
	/// <param name="x">読み込んでいるテクスチャの横何列目か</param>
	/// <param name="y">読み込んでいるテクスチャの縦何列目か</param>
	void SetBeginFrame(int x, int y);

	/// <summary>
	/// 描画フレームゲッター
	/// </summary>
	/// <returns>今の描画フレーム</returns>
	int GetDrawFrame() { return drawFrame_; }

	/// <summary>
	/// ループセッター
	/// </summary>
	/// <param name="isLoop">ループするかしないか</param>
	void SetLoop(bool isLoop) { isLoop_ = isLoop; }

	/// <summary>
	/// 再生トリガー
	/// </summary>
	/// <param name="isPlay">再生か停止か</param>
	void SetIsPlay(bool isPlay) { isPlay_ = isPlay; }

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	bool GetIsEnd() { return isEnd_; }

public: // その他関数

	/// <summary>
	/// 最初から再生する関数
	/// </summary>
	void Replay();

	/// <summary>
	/// 選択したフレームに移動する関数
	/// </summary>
	/// <param name="x">読み込んでいるテクスチャの横何列目か</param>
	/// <param name="y">読み込んでいるテクスチャの縦何列目か</param>
	void ChangeSelectedFrame(int x, int y);

public: // パブリックなメンバ変数

	// 座標
	Vector2 position_;

	// 大きさ
	Vector2 size_;

	// 色
	Vector4 color_;

	// 表示トリガー
	bool isActive_;

private: // メンバ変数

	// アンカーポイント
	Vector2 anchorPoint_;

	// スプライトアニメーションに使用するシート
	uint32_t animationSheet_ = 0u;

	// スプライト
	std::unique_ptr<Sprite> sprite_;

	// １フレームごとの画像サイズ
	Vector2 textureHandleSize_;

	// フレーム列サイズ
	Vector2 playFrameColumn_;
	// 再生開始列
	Vector2 beginPlayFrameColumn_;
	// 再生中フレーム列
	Vector2 nowPlayFrameColumn_;

	// アニメーション用t
	float animT_;
	// １フレームを何秒描画するか
	float drawTime_;
	// 描画開始フレーム
	int beginDrawFrame_;
	// 現在描画フレーム
	int drawFrame_;
	// 前フレーム描画フレーム数
	int prevDrawFrame_;
	// 全体のアニメーションフレーム数
	int animationFrame_;

	// 再生トリガー
	bool isPlay_;
	// 再生中か
	bool isPlaying_;
	// ループするか
	bool isLoop_;
	// 終了トリガー
	bool isEnd_;

	// リセットトリガー
	bool isReset_;

};

