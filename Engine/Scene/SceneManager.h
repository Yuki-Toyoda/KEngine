#pragma once
#include "BaseScene.h"
#include "../Audio/Audio.h"

/// <summary>
/// 全てのシーンの管理マネージャ
/// </summary>
class SceneManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

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
	/// 次のシーンのセッター
	/// </summary>
	/// <param name="nextScene">次のシーンの実体</param>
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }

	/// <summary>
	/// 選択中のステージ番号を取得するゲッター
	/// </summary>
	/// <returns>選択中のステージ番号</returns>
	int GetSelectedStageNumber() { return selectedStageNumber_; }
	/// <summary>
	/// 選択中のステージ番号のセッター
	/// </summary>
	/// <param name="selectedNumber">選択するステージ番号</param>
	void SetSelectedStageNumber(int selectedNumber) { selectedStageNumber_ = selectedNumber; }

public: // その他関数

	/// <summary>
	/// フェードエフェクト関数
	/// </summary>
	/// <param name="fadeTime">フェードイン演出時間(秒)</param>
	/// <param name="startcolor">開始時</param>
	/// <param name="endColor">終了時</param>
	/// <param name="playSound">音を鳴らすか</param>
	void StartFadeEffect(float fadeTime, Vector4 startcolor, Vector4 endColor, bool playSound = true);

	/// <summary>
	/// フェードアウトスプライトの色を強制的に変更させる関数
	/// </summary>
	/// <param name="color">変化させる色</param>
	void SetFadeColor(Vector4 color);

private: // プライベートなメンバ関数

	/// <summary>
	/// フェード演出関数
	/// </summary>
	void Fade();

public: // パブリックなメンバ変数

	// 音量
	float bgmVolume_; // BGM音量
	float seVolume_; // SE音量

private: // メンバ変数

	// 現在のシーン
	BaseScene* currentScene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// フェード演出スプライト用座標
	Vector2 fadeSpritePosition_;
	// フェード演出スプライト用大きさ
	Vector2 fadeSpriteSize_;
	// フェード演出スプライト用色
	Vector4 fadeSpriteColor_;
	// フェード演出用スプライト
	std::unique_ptr<Sprite> fadeSprite_;

	// フェード演出用色
	Vector4 startFadeColor_; // 始端
	Vector4 endFadeColor_; // 終端

	// フェード演出用t
	float fadeT_;
	// フェード演出時間(秒)
	float fadeTime_;
	// フェード演出中か
	bool isFading_;
	// フェード演出が終了しているか
	bool isEndFade_;

	// フェード効果音
	uint32_t soundHandleFade_ = 0u;

	// 選択中のステージ番号
	int selectedStageNumber_;

};
