#pragma once
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Model/Component/Skelton.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

// 構造体の前方宣言
struct aiAnimation;

/// <summary>
/// キーフレーム構造体
/// </summary>
/// <typeparam name="tValue">キーフレームの型</typeparam>
template<typename tValue>
struct keyframe {
	float  time;  // キーフレーム秒数
	tValue value; // キーフレームの値
};

/// <summary>
/// アニメーションカーブ構造体
/// </summary>
/// <typeparam name="tValue">アニメーションカーブの型</typeparam>
template<typename tValue>
struct AnimationCurve {
	// キーフレーム配列
	std::vector<keyframe<tValue>> keyframes;
};

/// <summary>
/// ノードアニメーション構造体
/// </summary>
struct NodeAnimation {
	AnimationCurve<Vector3>    translate; // 位置
	AnimationCurve<Quaternion> rotate;	  // 回転
	AnimationCurve<Vector3>	   scale;	  // 拡縮
};

/// <summary>
/// 補完用アニメーション構造体
/// </summary>
struct TransitionAnimation {
	Vector3		translate;	// 位置
	Quaternion	rotate;		// 回転
	Vector3		scale;		// 拡縮
};


/// <summary>
/// スキンアニメーションクラス
/// </summary>
class SkinAnimation final
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkinAnimation() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkinAnimation() = default;

public: // メンバ関数

	/// <summary>
	/// (ユーザー呼び出し禁止)初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// (ユーザー呼び出し禁止)更新関数
	/// </summary>
	/// <param name="skelton">アニメーション対象のスケルトン</param>
	void Update(Skelton* skelton);

	/// <summary>
	/// アニメーション開始関数
	/// </summary>
	void Start();
	/// <summary>
	/// 秒数指定アニメーション開始関数
	/// </summary>
	/// <param name="startSec">開始秒数</param>
	void Start(float startSec);
	/// <summary>
	/// アニメーション開始関数(補完アリ)
	/// </summary>
	/// <param name="transitionTime">補完秒数</param>
	/// <param name="prevAnim">補完元アニメーション</param>
	/// <param name="playingIndex">補完開始時インデックス数</param>
	void Start(float transitionTime, Skelton* skelton);

	/// <summary>
	/// アニメーション停止関数
	/// </summary>
	void Stop();

	/// <summary>
	/// アニメーション読み込み関数
	/// </summary>
	/// <param name="anim">読み込み中アニメーション</param>
	/// <param name="name">アニメーション名</param>
	void LoadAnimation(const aiAnimation* anim, std::string& name);

public: // アクセッサ等

	/// <summary>
	/// アニメーション終了トリガーゲッター
	/// </summary>
	/// <returns>アニメーション終了フラグ</returns>
	bool GetIsPlay() { return isPlay_; };

	/// <summary>
	/// 補完中トリガーゲッター
	/// </summary>
	/// <returns>補完中トリガー状態</returns>
	bool GetIsTransitioning() { return isTransitioning_; }

	/// <summary>
	///	アニメーション進捗度ゲッター
	/// </summary>
	/// <returns>進捗度(0.0 ~ 1.0)</returns>
	float GetProgress() const;

	/// <summary>
	/// アニメーションの再生速度ゲッター
	/// </summary>
	/// <returns>アニメーション再生速度</returns>
	float GetAnimationSpeed() { return animationSpeed_; }
	/// <summary>
	/// アニメーション再生速度セッター
	/// </summary>
	/// <param name="speed">アニメーション再生速度</param>
	void SetAnimationSpeed(const float speed) { animationSpeed_ = speed; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 引数で指定された秒数の3次元ベクトルをアニメーションから計算する関数
	/// </summary>
	/// <param name="keyframes">対象のキーフレーム配列</param>
	/// <param name="time">アニメーション秒数</param>
	/// <returns>3次元ベクトル</returns>
	Vector3 CalculateValue(const std::vector<keyframe<Vector3>>& keyframes, float time);
	/// <summary>
	/// 引数で指定された秒数のクォータニオンをアニメーションから計算する関数
	/// </summary>
	/// <param name="keyframes">対象のキーフレーム配列</param>
	/// <param name="time">アニメーション秒数</param>
	/// <returns>クォータニオン</returns>
	Quaternion CalculateValue(const std::vector<keyframe<Quaternion>>& keyframes, float time);

	/// <summary>
	/// 引数で指定された秒数の3次元ベクトルをアニメーションから計算する関数
	/// </summary>
	/// <param name="prevKeyframes">補完元キーフレーム配列</param>
	/// <param name="keyframes">対象のキーフレーム配列</param>
	/// <param name="time">アニメーション秒数</param>
	/// <param name="prevAnimTime">補完元アニメーション秒数</param>
	/// <returns>3次元ベクトル</returns>
	Vector3 CalculateValue(const Vector3& prevKeyframes, const std::vector<keyframe<Vector3>>& keyframes, float time);
	/// <summary>
	/// 引数で指定された秒数のクォータニオンをアニメーションから計算する関数
	/// </summary>
	/// <param name="prevKeyframes">補完元キーフレーム配列</param>
	/// <param name="keyframes">対象のキーフレーム配列</param>
	/// <param name="time">アニメーション秒数</param>
	/// <param name="prevAnimTime">補完元アニメーション秒数</param>
	/// <returns>クォータニオン</returns>
	Quaternion CalculateValue(const Quaternion& prevKeyframes, const std::vector<keyframe<Quaternion>>& keyframes, float time);

public: // パブリックメンバ変数

	// アニメーション名
	std::string animName_ = "";

	// 再生中アニメーションの秒数
	float animationTime_ = 0.0f;

	// 再生中トリガー
	bool isPlay_ = false;
	// アニメーションのループトリガー
	bool isLoop_ = false;

	// ノードアニメーション配列
	std::map<std::string, NodeAnimation> nodeAnimations_;

private: // メンバ変数

	// アニメーション全体の尺
	float duration_ = 0.0f;

	// 補完用スケルトン
	Skelton prevSkelton_;
	// 補間中トリガー
	bool isTransitioning_ = false;
	// 補間秒数
	float currentTransitionTime_	= 0.0f; // 現在
	float transitionDuration_		= 0.0f; // 秒数

	// アニメーション再生速度
	float animationSpeed_ = 1.0f;
};

