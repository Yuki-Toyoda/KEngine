#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#include <XInput.h>
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>

#include "Engine/Math/Vector3.h"

/// <summary>
/// 入力状態用
/// </summary>
enum InputState {
	TRIGGER, // トリガー
	PRESS,	 // 押下
	RELEASE	 // リリース
};

/// <summary>
/// 入力検知クラス
/// </summary>
class Input {
private: // 内部でしか参照しないクラス

	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;
	};

public: // サブクラス
	enum class PadType {
		DirectInput,
		XInput,
	};

	// variantがC++17から
	union State {
		XINPUT_STATE xInput_;
		DIJOYSTATE2 directInput_;
	};

	struct Joystick {
		Microsoft::WRL::ComPtr<IDirectInputDevice8> device_;
		int32_t deadZoneL_;
		int32_t deadZoneR_;
		PadType type_;
		State state_;
		State statePre_;
	};

private: // コンストラクタ等

	Input() = default;
	~Input();
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public: // 機能関数群

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber) const;

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber) const;

	/// <summary>
	/// 全キー情報取得
	/// </summary>
	/// <param name="keyStateBuf">全キー情報</param>
	const std::array<BYTE, 256>& GetAllKey() const { return key_; }

	/// <summary>
	/// 全マウス情報取得
	/// </summary>
	/// <returns>マウス情報</returns>
	const DIMOUSESTATE2& GetAllMouse() const;

	/// <summary>
	/// マウスの押下をチェック
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>押されているか</returns>
	bool IsPressMouse(int32_t mouseNumber) const;

	/// <summary>
	/// マウスのトリガーをチェック。押した瞬間だけtrueになる
	/// </summary>
	/// <param name="buttonNumber">マウスボタン番号(0:左,1:右,2:中,3~7:拡張マウスボタン)</param>
	/// <returns>トリガーか</returns>
	bool IsTriggerMouse(int32_t buttonNumber) const;

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();

	/// <summary>
	/// ホイールスクロール量を取得する
	/// </summary>
	/// <returns>ホイールスクロール量。奥側に回したら+。Windowsの設定で逆にしてたら逆</returns>
	int32_t GetWheel() const;

	/// <summary>
	/// マウスの位置を取得する（ウィンドウ座標系）
	/// </summary>
	/// <returns>マウスの位置</returns>
	const DirectX::XMFLOAT2& GetMousePosition() const;

	/// <summary>
	/// 指定されたコントローラーボタンが指定した状態かを検証する
	/// </summary>
	/// <param name="button">検知するボタン</param>
	/// <param name="state">検証する状態</param>
	/// <param name="controllerNo">(任意)取得するコントローラー番号</param>
	/// <returns>指定された状態か</returns>
	bool InspectButton(int button, int state, int32_t controllerNo = 0);

	/// <summary>
	/// 左トリガーが指定した状態かを検証する
	/// </summary>
	/// <param name="state">検証する状態</param>
	/// <param name="leftOrRight">左右どちらか (0 : 左, 1 : 右)</param>
	/// <param name="controllerNo">(任意)取得するコントローラー番号</param>
	/// <returns>指定された状態か</returns>
	bool InspectTrigger(int state, int leftOrRight, int32_t controllerNo = 0);

	/// <summary>
	/// コントローラーのスティックの入力ベクトルの取得
	/// </summary>
	/// <param name="leftOrRight">左右どちらか (0 : 左, 1 : 右)</param>
	/// <param name="controllerNo">(任意)取得するコントローラー番号</param>
	/// <returns>スティックの入力ベクトル</returns>
	Vector3 GetJoyStickInput(int leftOrRight, int32_t controllerNo = 0);

	/// <summary>
	/// コントローラーのスティックが入力されているかを検証する
	/// </summary>
	/// <param name="leftOrRight">左右どちらか (0 : 左, 1 : 右)</param>
	/// <param name="controllerNo">(任意)取得するコントローラー番号</param>
	/// <returns>スティックが入力されているか</returns>
	bool InspectJoyStickInput(int leftOrRight, int32_t controllerNo = 0);

private: // メンバ関数

	/// <summary>
	/// 現在のコントローラーの入力状態を取得する
	/// </summary>
	/// <param name="stickNo">取得するコントローラー番号</param>
	/// <param name="out">現在のコントローラーの入力状態</param>
	/// <returns>取得できたか</returns>
	bool GetControllerState(int32_t stickNo, DIJOYSTATE2& out) const;

	/// <summary>
	/// 前フレームのコントローラーの入力状態を取得する
	/// </summary>
	/// <param name="stickNo">取得するコントローラー番号</param>
	/// <param name="out">前フレームのコントローラーの入力状態</param>
	/// <returns>取得できたか</returns>
	bool GetControllerStatePrevious(int32_t stickNo, DIJOYSTATE2& out) const;

	/// <summary>
	/// 現在のコントローラー状態を取得する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="out">現在のジョイスティック状態</param>
	/// <returns>正しく取得できたか</returns>
	bool GetControllerState(int32_t stickNo, XINPUT_STATE& out) const;

	/// <summary>
	/// 前回のジョイスティック状態を取得する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="out">前回のジョイスティック状態</param>
	/// <returns>正しく取得できたか</returns>
	bool GetControllerStatePrevious(int32_t stickNo, XINPUT_STATE& out) const;

	/// <summary>
	/// デッドゾーンを設定する
	/// </summary>
	/// <param name="stickNo">ジョイスティック番号</param>
	/// <param name="deadZoneL">デッドゾーン左スティック 0~32768</param>
	/// <param name="deadZoneR">デッドゾーン右スティック 0~32768</param>
	/// <returns>正しく設定できたか</returns>
	void SetJoystickDeadZone(int32_t stickNo, int32_t deadZoneL, int32_t deadZoneR);

	/// <summary>
	/// ジョイスティックのコールバック取得関数
	/// </summary>
	/// <param name="pdidInstance">インスタンス</param>
	/// <param name="pContext">コンテキスト</param>
	/// <returns>コールバックジョイスティック</returns>
	static BOOL CALLBACK
		EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext) noexcept;
	
	/// <summary>
	/// コントローラーの初期設定関数
	/// </summary>
	void SetupControllers();

	/// <summary>
	/// 接続されているコントローラー数を取得する
	/// </summary>
	/// <returns>接続されているコントローラー数</returns>
	size_t GetNumberOfControllers();

private: // メンバ変数
	  
	// ウィンドウハンドル
	HWND hwnd_{};

	// DirectInputのインスタンス
	Microsoft::WRL::ComPtr<IDirectInput8> dInput_;
	// キー入力取得デバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devKeyboard_;
	// マウス入力取得デバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> devMouse_;
	// ジョイスティック状態取得
	std::vector<Joystick> devJoysticks_{};
	// キー入力状態
	std::array<BYTE, 256> key_{};
	// 前フレームキー入力状態
	std::array<BYTE, 256> keyPre_{};
	// マウス入力状態
	DIMOUSESTATE2 mouse_{};
	// 前フレームマウス入力状態
	DIMOUSESTATE2 mousePre_{};
	// マウス座標
	DirectX::XMFLOAT2 mousePosition_{};

	// トリガーの入力デッドゾーン
	int32_t triggerDeadZone_ = 35;

};