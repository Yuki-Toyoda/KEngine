#pragma once
#include <functional>
#include <chrono>

#include "../../../Externals/imgui/imgui.h"

/// <summary>
/// KLib
/// </summary>
namespace KLib {
	/// <summary>
/// 経過秒数タイマー
/// </summary>
	class DeltaTimer
	{
	public: // コンストラクタ等

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="finish">終了までの秒数</param>
		DeltaTimer(float finish = 0.0f) : finishFlame_(finish), nowFlame_(0.0f), isFinish_(true), isActive_(false) {}

	public: // メンバ関数

		/// <summary>
		/// タイマーの開始関数
		/// </summary>
		void Start();

		/// <summary>
		/// 開始秒数を指定してタイマーを開始させる関数
		/// </summary>
		/// <param name="time">タイマー秒数</param>
		void Start(float time);

		/// <summary>
		/// 更新関数
		/// </summary>
		/// <returns>実行中か</returns>
		bool Update();

		/// <summary>
		/// タイマーの強制停止関数
		/// </summary>
		void Finish();

	public: // アクセッサ等

		/// <summary>
		/// タイマーの進行度ゲッター
		/// </summary>
		/// <returns>タイマーの進行度(0.0 ~ 1.0)</returns>
		float GetProgress() const;

		/// <summary>
		/// 終了状態ゲッター
		/// </summary>
		/// <returns>タイマーが終了しているか</returns>
		bool GetIsFinish() { return isFinish_; }

		/// <summary>
		/// 有効状態ゲッター
		/// </summary>
		/// <returns>タイマーが有効かどうか</returns>
		bool GetIsActive() { return isActive_; }

	private: // プライベートなメンバ関数

		/// <summary>
		/// 経過秒数ゲッター
		/// </summary>
		/// <returns>現在の経過秒数</returns>
		float GetDeltaTime() const { return std::clamp(ImGui::GetIO().DeltaTime, 0.f, 0.1f); }

	private: // メンバ変数

		// 終了秒数
		float finishFlame_;

		// 現在の経過秒数
		float nowFlame_;

		// 終了トリガー
		bool isFinish_;
		// タイマーの有効トリガー
		bool isActive_;
	};
}
