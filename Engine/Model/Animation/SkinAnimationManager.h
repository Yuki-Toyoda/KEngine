#pragma once
#include "SkinAnimation.h"

// クラスの前方宣言
class SkiningModel;

// 構造体の前方宣言
struct aiScene;

/// <summary>
/// スキンアニメーションの管理用クラス
/// </summary>
class SkinAnimationManager
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkinAnimationManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkinAnimationManager() = default;

public: // メンバ関数

	/// <summary>
	/// (ユーザー呼び出し禁止)更新関数
	/// </summary>
	/// <param name="skelton">アニメーション対象スケルトン</param>
	void Update(Skelton* skelton);

	/// <summary>
	/// (ユーザー呼び出し禁止)アニメーションロード関数
	/// </summary>
	/// <param name="scene">読み込み中シーン</param>
	void LoadAnimations(const aiScene* scene);
	
	/// <summary>
	/// アニメーション再生関数
	/// </summary>
	/// <param name="animName">再生するアニメーション名</param>
	/// <param name="transitionTime">(任意)アニメーション間の補完秒数</param>
	/// <param name="isLoop">(任意)ループさせるか</param>
	/// <param name="startTime">(任意)アニメーションの開始秒数</param>
	void PlayAnimation(const std::string& animName, float transitionTime = 0.0f, bool isLoop = false, float startTime = 0.0f);

	/// <summary>
	/// 再生中アニメーション停止関数
	/// </summary>
	void StopPlayingAnimation();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	/// スケルトンセッター
	/// </summary>
	/// <param name="skelton">アニメーション対象スケルトン</param>
	void SetSkelton(Skelton* skelton) { skelton_ = skelton; };

	/// <summary>
	/// 再生中アニメーションが再生されているかのゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsPlayingAnimation();

private: // プライベートなメンバ関数

	/// <summary>
	/// アニメーション停止関数
	/// </summary>
	/// <param name="animName">停止させるアニメーション</param>
	void StopAnimation(const std::string& animName);

private: // メンバ変数

	// スケルトン
	Skelton* skelton_ = nullptr;

	// アニメーション配列
	std::map<std::string, SkinAnimation> animations_;

	// 再生中アニメーション名
	std::string playingAnimName_ = "";

};

