#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseObject
{
public: // メンバ関数

	Player();
	~Player() override;

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollisionEnter(BaseObject* object)override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// 非衝突時コールバック関数
	/// </summary>
	/// <param name="object">前フレーム衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object) override;

private: // メンバ関数

	// 作成した関数等の初期化
	void InitializeVariables();

	// 操作すべてを取る
	void GetOperation();
	// プレイヤー自身の更新
	void UpdatePlayer();
	// 歯車自体の更新
	void UpdateGear();

	// 回転中のプレイヤーの位置の補正
	void UpdatePlayerRotate();

	// 回転速度を回転角に変換する関数
	float ConvertSpeedToRadian(float rotateSpeed);

	// 角度からどちらに回転するかを取得する
	void CheckDirectionRotate();
	// 歯車のどの位置に着くかを決める
	void CheckGearCollision();

	// 空中でジャンプする関数
	void AirJump();

	// デバッグ時にのみ動くGUI
	void DebugGui();

private: // メンバ変数

	// 入力状態検知
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	/*--------------------------//
	//	プレイヤー自体の変数		//
	//--------------------------*/

	// プレイヤーの最大移動速度
	float kMaxPlayerVelocity_ = 0;

	// プレイヤーにかかる重力
	float kGravity_ = 0;
	
	// プレイヤーの落下方向
	Vector3 kFallDirection_ = { 0.0f,-1.0f,0.0f };

	float kPlayerJumpPower_ = 0;

	float radius_ = 1.0f;

	// プレイヤーの移動速度
	Vector3 playerVelocity_ = { 0,0,0 };
	Vector3 playerAcceleration_ = { 0,0,0 };

	// プレイヤーのいる角度
	float playerTheta_ = 0;

	/*------------------//
	//	歯車自体の変数		//
	//------------------*/

	// 歯車の回転の最高速度
	float kMaxGearRotateSpeed_ = 0;

	// 歯車が回転するときの最低値
	float kMinGearRollSpeed_ = 0;

	// 歯車の抵抗力
	//float kGearFriction_ = 0;

	// 歯車の振り子する幅
	float kGearAmplitude = 0;

	// 歯車の減少率
	float kGearDecreaseRate_ = 0.90f;

	// 歯車の内円の半径
	float kGearInnerRadius_;

	// 振り子範囲に入った時の速度最低値
	float kMinGearPendulumSpeed_ = 0.0f;

	// 歯車用のトランスフォーム
	WorldTransform gearTransform_;

	// 歯車の回転角取得用
	float& gearTheta_;

	// 歯車の色
	Vector4 gearColor_;

	// 歯車の回転速度
	float gearRotateSpeed_;

	// 歯車の加算の割合
	float gearRollRatio_ = 1.0f;

	/*--------------------------------------//
	//	フラグによって操作も管理するために、		//
	//	一度クッションをはさむ					//
	//--------------------------------------*/
	
	// ジャンプするかどうかのフラグ
	bool isJumpTrigger_ = false;

	// 歯車と設置しているかどうか
	bool isLanding_ = false;

	// 歯車の回転方向
	// 右回転 : true, 左回転 : false
	bool isRotateRight_ = true;	

	// 振り子する範囲に入っていたかどうか
	bool isPendulum_ = false;
	// 振り子するためのバッファ
	bool wasRotateRight_ = true;

	/*------------------//
	//	デバッグ用の変数	//
	//------------------*/

#ifdef _DEBUG

	bool isUpdateRotate = false;

#endif // _DEBUG


};

/*	作る処理
* ・回転方法を角度そのままではなく、回転力から回転する角度へと変換してから回転させる
* ・歯車についていない時の挙動が可笑しいので直す
* 
*/