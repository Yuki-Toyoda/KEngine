#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"
#include "State/StateList.h"

// クラスの前方宣言
class FollowCamera;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

private: // その他関数

	/// <summary>
	/// 行動状態変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IState> newState);

public: // パブリックなメンバ変数

	// 入力検知用
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// 追従カメラ格納用
	FollowCamera* followCamera_ = nullptr;

private: // メンバ変数

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;
	// 頭のトランスフォーム
	WorldTransform headTransform_;
	// 右腕のトランスフォーム
	WorldTransform armTransform_R_;
	// 左腕のトランスフォーム
	WorldTransform armTransform_L_;

	// 行動状態格納変数
	std::unique_ptr<IState> state_;

};

