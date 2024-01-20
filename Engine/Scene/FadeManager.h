#pragma once
#include "../Sprite/Sprite.h"
#include "../Utility/Animation/AnimationManager.h"

/// <summary>
/// フェード演出マネージャ
/// </summary>
class FadeManager
{
private: // コンストラクタ、デストラクタ等

	// シングルトンパターンの設定
	FadeManager() = default;
	~FadeManager() = default;
	FadeManager(const FadeManager&) = delete;
	const FadeManager& operator=(const FadeManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static FadeManager* GetInstance() {
		static FadeManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGuiの表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">パラメータ名</param>
	void CreateParameter(const std::string& name);

	/// <summary>
	/// 強制フェードアウト関数
	/// </summary>
	void CompulsionFadeOut();

	/// <summary>
	/// 強制フェードイン関数
	/// </summary>
	void CompulsionFadeIn();


public: // アクセッサ等

	/// <summary>
	/// 再生関数
	/// </summary>
	void Play();

	/// <summary>
	/// 読み込みパラメータ変更関数
	/// </summary>
	/// <param name="name">変更するパラメータ</param>
	/// <param name="isChange">実行時にそのパラメータへの強制的な遷移を行うか</param>
	void ChangeParameter(const std::string& name, bool isChange = false);

	/// <summary>
	/// フェード演出中かどうかのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsStaging() { anim_->GetIsPlay(); }

private: // メンバ変数

	// フェード演出用スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// アニメーションマネージャ
	AnimationManager* animManager_ = nullptr;
	// アニメーション
	Animation* anim_ = nullptr;

};

