#pragma once
#include <string>
#include "Engine/GameObject/WorldTransform.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Utility/KLib.h"

// テンプレートを定義
template <typename T>

/// <summary>
/// アニメーション１フレームごとの情報
/// </summary>
class AnimationKey
{
public: // コンストラクタ等

	AnimationKey() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="keyName">キー名</param>
	/// <param name="keyCount">配列上のインデックス</param>
	AnimationKey(const std::string& animationName, const std::string& keyName, int32_t keyCount);

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

public: // メンバ変数

	// アニメーションさせるパラメーター
	// 一応任意型。現在はint, int32_t, float, Vector2, Vector3, WorldTransformに対応予定
	T value_;

	// 再生されるキーのカウント
	int32_t playKeyCount_ = 0;

	// 終端フレーム
	int32_t playFrame_ = 0;

	// 大元のアニメーション名
	std::string animationName_;
	// キー名
	std::string keyName_;

	// イージングパラメーター
	KLib::EasingType eParam_ = KLib::ParamEaseLinear;
	// イージングの関数ポインタ
	float(*ease_)(float) = nullptr;

	// 編集用削除トリガー
	bool isDelete_ = false;
};

template<typename T>
inline AnimationKey<T>::AnimationKey(const std::string& animationName, const std::string& keyName, int32_t keyCount)
{
	if constexpr (std::is_same_v<T, WorldTransform>) {
		// worldTransformを初期化
		value_.Init();
	}
	else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, int>) {
		// int型の値で初期化を行う
		value_ = 0;
	}
	else if constexpr (std::is_same_v<T, Vector2>) {
		// Vector2型の値で初期化を行う
		value_ = Vector2();
	}
	else if constexpr (std::is_same_v<T, Vector3>) {
		// Vector3型の値で初期化を行う
		value_ = Vector3();
	}
	else {
		// float型の値で全ての要素を初期化
		value_ = { 0.0f };
	}

	// アニメーション名の取得
	animationName_ = animationName;
	// キー名の取得
	keyName_ = keyName;

	// 再生キーの位置を取得
	playKeyCount_ = keyCount;

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
	std::string keyName = "key : " + std::to_string(playKeyCount_);

	// キーの削除を行う
	std::string deleteButtonName = "delete : " + keyName;

	ImGui::Text(keyName.c_str());

	ImGui::SameLine();
	// ボタンを押したら削除する
	if (ImGui::Button(deleteButtonName.c_str())) {
		isDelete_ = true;
	}

	// スライダーの表示
	if constexpr (std::is_same_v<T, WorldTransform>) {
		value_.DisplayImGui(keyName);
	}
	else if constexpr (std::is_same_v<T, int32_t>) {

		ImGui::DragInt(keyName.c_str(), &value_, 1.0f, 0);
	}
	else if constexpr (std::is_same_v<T, int>) {
		ImGui::DragInt(keyName.c_str(), &value_, 1.0f);
	}
	else if constexpr (std::is_same_v<T, float>) {
		ImGui::DragFloat(keyName.c_str(), &value_, 0.01f);
	}
	else if constexpr (std::is_same_v<T, Vector2>) {
		ImGui::DragFloat2(keyName.c_str(), &value_.x, 0.01f);
	}
	else if constexpr (std::is_same_v<T, Vector3>) {
		ImGui::DragFloat3(keyName.c_str(), &value_.x, 0.01f);
	}
	else {
		// エラー回避のため記述なし
	}

	// 再生フレームの設定
	std::string playFrameName = keyName + " : PlayFrame";
	ImGui::DragInt(playFrameName.c_str(), &playFrame_, 1.0f, 0);

	// イージングパラメーターを設定
	std::string eName = keyName + " : EasingParameter";
	int eParam = (int)eParam_;
	ImGui::SliderInt(eName.c_str(), &eParam, (int)KLib::ParamEaseLinear, (int)KLib::ParamEaseInOutBounce);
	eParam_ = (KLib::EasingType)eParam;
}

template<typename T>
inline void AnimationKey<T>::AddParam()
{
	// キー名の取得
	std::string keyName = keyName_ + " key : " + std::to_string(playKeyCount_);
	if constexpr (std::is_same_v<T, WorldTransform>) {
		// ワールドトランスフォーム内の情報を渡す
		GlobalVariables::GetInstance()->AddItem(animationName_, keyName + " : Scale", value_.scale_);
		GlobalVariables::GetInstance()->AddItem(animationName_, keyName + " : Rotate", value_.rotate_);
		GlobalVariables::GetInstance()->AddItem(animationName_, keyName + " : Translate", value_.translate_);
	}
	else {
		// パラメーター内の情報を渡す
		GlobalVariables::GetInstance()->AddItem(animationName_, keyName, value_);
	}

	// 再生フレーム情報を渡す
	std::string playFrameName = keyName + " : PlayFrame";
	GlobalVariables::GetInstance()->AddItem(animationName_, playFrameName, playFrame_);

	// イージング情報を渡す
	std::string eName = keyName + " : EasingParameter";
	GlobalVariables::GetInstance()->AddItem(animationName_, eName, eParam_);
}

template<typename T>
inline void AnimationKey<T>::SetParam()
{
	// キー名の取得
	std::string keyName = keyName_ + " key : " + std::to_string(playKeyCount_);
	if constexpr (std::is_same_v<T, WorldTransform>) {
		// ワールドトランスフォーム内の情報を渡す
		GlobalVariables::GetInstance()->SetValue(animationName_, keyName + " : Scale", value_.scale_);
		GlobalVariables::GetInstance()->SetValue(animationName_, keyName + " : Rotate", value_.rotate_);
		GlobalVariables::GetInstance()->SetValue(animationName_, keyName + " : Translate", value_.translate_);
	}
	else {
		// パラメーター内の情報を渡す
		GlobalVariables::GetInstance()->SetValue(animationName_, keyName, value_);
	}

	// 再生フレーム情報を渡す
	std::string playFrameName = keyName + " : PlayFrame";
	GlobalVariables::GetInstance()->SetValue(animationName_, playFrameName, playFrame_);

	// イージング情報を渡す
	std::string eName = keyName + " : EasingParameter";
	GlobalVariables::GetInstance()->SetValue(animationName_, eName, eParam_);
}

template<typename T>
inline void AnimationKey<T>::ApplyParam()
{
	// キー名の取得
	std::string keyName = keyName_ + " key : " + std::to_string(playKeyCount_);
	if constexpr (std::is_same_v<T, WorldTransform>) {
		value_.scale_ = GlobalVariables::GetInstance()->GetVector3Value(animationName_, keyName + " : Scale");
		value_.rotate_ = GlobalVariables::GetInstance()->GetVector3Value(animationName_, keyName + " : Rotate");
		value_.translate_ = GlobalVariables::GetInstance()->GetVector3Value(animationName_, keyName + " : Translate");
	}
	else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, int>) {
		value_ = GlobalVariables::GetInstance()->GetIntValue(animationName_, keyName);
	}
	else if constexpr (std::is_same_v<T, float>) {
		value_ = GlobalVariables::GetInstance()->GetFloatValue(animationName_, keyName);
	}
	else if constexpr (std::is_same_v<T, Vector2>) {
		value_ = GlobalVariables::GetInstance()->GetVector2Value(animationName_, keyName);
	}
	else if constexpr (std::is_same_v<T, Vector3>) {
		value_ = GlobalVariables::GetInstance()->GetVector3Value(animationName_, keyName);
	}

	// 再生フレーム情報を渡す
	std::string playFrameName = keyName + " : PlayFrame";
	playFrame_ = GlobalVariables::GetInstance()->GetIntValue(animationName_, playFrameName);

	// イージング情報を渡す
	std::string eName = keyName + " : EasingParameter";
	eParam_ = (KLib::EasingType)GlobalVariables::GetInstance()->GetIntValue(animationName_, eName);

	// 読み取ったイージングタイプを元に設定
	switch (eParam_)
	{
	case KLib::ParamEaseLinear:
		ease_ = KLib::EaseLinear;
		break;
	case KLib::ParamEaseInQuad:
		ease_ = KLib::EaseInQuad;
		break;
	case KLib::ParamEaseOutQuad:
		ease_ = KLib::EaseOutQuad;
		break;
	case KLib::ParamEaseInOutQuad:
		ease_ = KLib::EaseInOutQuad;
		break;
	case KLib::ParamEaseInSine:
		ease_ = KLib::EaseInSine;
		break;
	case KLib::ParamEaseOutSine:
		ease_ = KLib::EaseOutSine;
		break;
	case KLib::ParamEaseInOutSine:
		ease_ = KLib::EaseInOutSine;
		break;
	case KLib::ParamEaseInBack:
		ease_ = KLib::EaseInBack;
		break;
	case KLib::ParamEaseOutBack:
		ease_ = KLib::EaseOutBack;
		break;
	case KLib::ParamEaseInOutBack:
		ease_ = KLib::EaseInOutBack;
		break;
	case KLib::ParamEaseInBounce:
		ease_ = KLib::EaseInBounce;
		break;
	case KLib::ParamEaseOutBounce:
		ease_ = KLib::EaseOutBounce;
		break;
	case KLib::ParamEaseInOutBounce:
		ease_ = KLib::EaseInOutBounce;
		break;
	}
}