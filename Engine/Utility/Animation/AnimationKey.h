#pragma once
#include <string>
#include "../../GameObject/WorldTransform.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../GlobalVariables/GlobalVariables.h"

// テンプレートを定義
template <typename T>

/// <summary>
/// アニメーション１フレームごとの情報
/// </summary>
class AnimationKey
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="keyName">アニメーション名</param>
	/// <param name="playFrame">キーの位置</param>
	AnimationKey(const std::string& keyName, int32_t playFrame);

public: // メンバ関数

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// パラメーターを調整項目クラスに追加する関数
	/// </summary>
	void AddParam();
	/// <summary>
	/// パラメーターの値を調整項目クラスにセットする関数
	/// </summary>
	void SetParam();
	/// <summary>
	/// パラメーターの値を調整項目クラスから取得する関数
	/// </summary>
	void ApplyParam();

public: // アクセッサ等

	/// <summary>
	/// 任意型のパラメーターゲッター
	/// </summary>
	/// <returns>生成時の任意型ゲッター</returns>
	T GetValue() { return value_; }

	/// <summary>
	/// 再生位置の取得
	/// </summary>
	/// <returns>再生位置</returns>
	int32_t GetPlayFrame() { return playFrame_; }

public: // pa-buメンバ変数

	// アニメーションさせるパラメーター
	// 一応任意型。現在はint, int32_t, float, Vector2, Vector3, WorldTransformに対応予定
	T value_;

	// 終端フレーム
	int32_t playFrame_;

	// キー名
	std::string keyName_;

	// イージングパラメーター
	KLib::EasingType eParam_ = KLib::ParamEaseLinear;
	// イージングの関数ポインタ
	float(*ease_)(float) = nullptr;

};

template<typename T>
inline AnimationKey<T>::AnimationKey(const std::string& keyName, int32_t playFrame)
{
	if constexpr (std::is_same_v<T, WorldTransform>) {
		// worldTransformを初期化
		value_.Init();
	}
	else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, int>) {
		// int型の値で初期化を行う
		value_ = 0;
	}
	else {
		// float型の値で全ての要素を初期化
		value_ = { 0.0f };
	}

	// キー名の取得
	keyName_ = keyName;
	// 再生位置の取得
	playFrame_ = playFrame;

	// イージングパラメーター初期設定
	eParam_ = KLib::ParamEaseLinear;
	// イージング関数の初期設定代入
	ease_ = KLib::EaseLinear;

	// パラメーター追加
	AddParam();
	// パラメーター読み込み
	ApplyParam();
}

template<typename T>
inline void AnimationKey<T>::DisplayImGui()
{
	// キー名の取得
	std::string keyName = "key : " + std::to_string(playFrame_);

	// キー名をテキストで表示
	ImGui::Text(keyName.c_str());
	// スライダーの表示
	if constexpr (std::is_same_v<T, WorldTransform>) {
		value_.DisplayImGui(keyName);
	}else if constexpr (std::is_same_v<T, int32_t>) {
		ImGui::Text(keyName.c_str());
		ImGui::DragInt(keyName.c_str(), &value_, 1.0f, 0);
	}else if constexpr (std::is_same_v<T, int>) {
		ImGui::Text(keyName.c_str());
		ImGui::DragInt(keyName.c_str(), &value_, 1.0f);
	}else if constexpr (std::is_same_v<T, float>) {
		ImGui::Text(keyName.c_str());
		ImGui::DragFloat(keyName.c_str(), &value_, 0.01f);
	}else if constexpr (std::is_same_v<T, Vector2>) {
		ImGui::Text(keyName.c_str());
		ImGui::DragFloat2(keyName.c_str(), &value_.x, 0.01f);
	}else if constexpr (std::is_same_v<T, Vector3>) {
		ImGui::Text(keyName.c_str());
		ImGui::DragFloat3(keyName.c_str(), &value_.x, 0.01f);
	}
	else {
		// エラー回避のため記述なし
	}

	// イージングパラメーターを設定
	ImGui::SliderInt(keyName.c_str() + " : EasingParameter", &eParam_, KLib::ParamEaseLinear, KLib::ParamEaseInOutBounce);
}

template<typename T>
inline void AnimationKey<T>::AddParam()
{
	// キー名の取得
	std::string keyName = keyName_ + " key : " + std::to_string(playFrame_);
	if constexpr (std::is_same_v<T, WorldTransform>) {
		// ワールドトランスフォーム内の情報を渡す
		GlobalVariables::GetInstance()->AddItem(keyName_, keyName + " : Scale", value_.scale_);
		GlobalVariables::GetInstance()->AddItem(keyName_, keyName + " : Rotate", value_.rotate_);
		GlobalVariables::GetInstance()->AddItem(keyName_, keyName + " : Translate", value_.translate_);
	}
	else {
		// パラメーター内の情報を渡す
		GlobalVariables::GetInstance()->AddItem(keyName_, keyName, value_);
	}

	// イージング情報を渡す
	GlobalVariables::GetInstance()->AddItem(keyName_, keyName, eParam_);
}

template<typename T>
inline void AnimationKey<T>::SetParam()
{
	// キー名の取得
	std::string keyName = keyName_ + " key : " + std::to_string(playFrame_);
	if constexpr (std::is_same_v<T, WorldTransform>) {
		// ワールドトランスフォーム内の情報を渡す
		GlobalVariables::GetInstance()->SetValue(keyName_, keyName + " : Scale", value_.scale_);
		GlobalVariables::GetInstance()->SetValue(keyName_, keyName + " : Rotate", value_.rotate_);
		GlobalVariables::GetInstance()->SetValue(keyName_, keyName + " : Translate", value_.translate_);
	}
	else {
		// パラメーター内の情報を渡す
		GlobalVariables::GetInstance()->SetValue(keyName_, keyName, value_);
	}

	keyName = keyName + "EasingParameter";
	// イージング情報を渡す
	GlobalVariables::GetInstance()->SetValue(keyName_, keyName, eParam_);
}

template<typename T>
inline void AnimationKey<T>::ApplyParam()
{
	// キー名の取得
	std::string keyName = keyName_ + " key : " + std::to_string(playFrame_);
	if constexpr (std::is_same_v<T, WorldTransform>) {
		value_.scale_ = GlobalVariables::GetInstance()->GetVector3Value(keyName_, keyName + " : Scale");
		value_.rotate_ = GlobalVariables::GetInstance()->GetVector3Value(keyName_, keyName + " : Rotate");
		value_.translate_ = GlobalVariables::GetInstance()->GetVector3Value(keyName_, keyName + " : Translate");
	}
	else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, int>) {
		value_ = GlobalVariables::GetInstance()->GetIntValue(keyName_, keyName);
	}
	else if constexpr (std::is_same_v<T, float>) {
		value_ = GlobalVariables::GetInstance()->GetFloatValue(keyName_, keyName);
	}
	else if constexpr (std::is_same_v<T, Vector2>) {
		value_ = GlobalVariables::GetInstance()->GetVector2Value(keyName_, keyName);
	}
	else if constexpr (std::is_same_v<T, Vector3>) {
		value_ = GlobalVariables::GetInstance()->GetVector3Value(keyName_, keyName);
	}

	keyName = keyName + "EasingParameter";
	// イージング情報を渡す
	value_ = GlobalVariables::GetInstance()->GetIntValue(keyName_, keyName);

	// 読み取ったイージングタイプを元に設定
	switch (KLib::EasingType)
	{
	case ParamEaseLinear:
		ease_ = KLib::EaseLinear;
		break;
	case ParamEaseInQuad:
		ease_ = KLib::EaseInQuad;
		break;
	case ParamEaseOutQuad:
		ease_ = KLib::EaseOutQuad;
		break;
	case ParamEaseInOutQuad:
		ease_ = KLib::EaseInOutQuad;
		break;
	case ParamEaseInSine:
		ease_ = KLib::EaseInSine;
		break;
	case ParamEaseOutSine:
		ease_ = KLib::EaseOutSine;
		break;
	case ParamEaseInOutSine:
		ease_ = KLib::EaseInOutSine;
		break;
	case ParamEaseInBack:
		ease_ = KLib::EaseInBack();
		break;
	case ParamEaseOutBack:
		ease_ = KLib::EaseOutBack;
		break
	case ParamEaseInOutBack:
		ease_ = KLib::EaseInOutBack;
		break;
	case ParamEaseInBounce:
		ease_ = KLib::EaseInBounce;
		break;
	case ParamEaseOutBounce:
		ease_ = KLib::EaseOutBounce;
		break;
	case ParamEaseInOutBounce:
		ease_ = KLib::EaseInOutBounce;
		break;
	}
}
