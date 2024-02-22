#pragma once
#include <variant>
#include "AnimationKeys.h"
#include <wrl/client.h>

/// <summary>
/// アニメーションクラス
/// </summary>
class Animation
{

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param keyName="keyName">アニメーション名</param>
	void Init(const std::string& name);
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">アニメーション名</param>
	/// <param name="parameterName">読み込むパラメータ名</param>
	void Init(const std::string& name, const std::string& parameterName);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 再生関数
	/// </summary>
	void Play();

	/// <summary>
	/// 再生関数
	/// </summary>
	/// <param name="trantionTime">遷移秒数</param>
	void Play(const float& trantionTime);

	/// <summary>
	/// 停止関数
	/// </summary>
	void Stop();

public: // アクセッサ等

	/// <summary>
	/// アニメーションキー配列を追加する関数
	/// </summary>
	/// <typeparam keyName="T">追加するキーの型名</typeparam>
	/// <param keyName="keyName">追加するキー配列の名前</param>
	/// <param keyName="value">アニメーションさせる値のポインタ</param>
	template<typename T>
	void AddAnimationKeys(const std::string keyName, T* value);

	/// <summary>
	/// (ユーザー呼び出し禁止)アニメーションキー配列を追加する関数
	/// </summary>
	/// <typeparam keyName="T">追加するキーの型名</typeparam>
	/// <param keyName="keyName">追加するキー配列の名前</param>
	template<typename T>
	void AddAnimationKeys(const std::string keyName);

	/// <summary>
	/// アニメーションの再生状態ゲッター
	/// </summary>
	/// <returns>再生中か</returns>
	bool GetIsPlay();

	/// <summary>
	/// <para>アニメーション再生時に読み込むパラメータを変更する関数</para>
	/// <para>変更したパラメータが同名のキー情報を持っている必要アリ</para>
	/// </summary>
	/// <param name="name">読み込むパラメータ名</param>
	/// <param name="isChange">実行時にそのパラメータへの強制的な遷移を行うか</param>
	void ChangeParameter(const std::string name, bool isChange = false);

	/// <summary>
	/// <para>アニメーション再生時に読み込むパラメータを変更する関数</para>
	/// <para>変更したパラメータが同名のキー情報を持っている必要アリ</para>
	/// </summary>
	/// <param name="name">読み込むパラメータ名</param>
	/// <param name="transitionTime">アニメーション補間にかかる秒数</param>
	/// <param name="isChange">実行時にそのパラメータへの強制的な遷移を行うか</param>
	void ChangeParameter(const std::string name, const float& transitionTime, bool isChange = false);

	/// <summary>
	/// 読み込み中パラメータゲッター
	/// </summary>
	const std::string GetReadingParameterName() { return parameterName_; }

	/// <summary>
	/// アニメーション全体の進捗ゲッター
	/// </summary>
	/// <returns>アニメーション全体の進捗</returns>
	float GetAnimationProgress();

	/// <summary>
	/// アニメーション再生速度セッター
	/// </summary>
	/// <param name="animSpeed"></param>
	void SetAnimationSpeed(const float& animSpeed);

public: // その他関数群

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// (ユーザー呼び出し禁止)パラメータ用ImGui表示関数
	/// </summary>
	void DisplayParameterImGui();

	/// <summary>
	/// 呼び出した際、アニメーションを保存する
	/// </summary>
	void SaveAnimation();

public: // メンバ変数

	// アニメーション名
	std::string name_;

	// 読み込むパラメータ名
	std::string parameterName_;

	// アニメーションキー配列をまとめた配列
	std::vector<std::variant<
		AnimationKeys<int>,
		AnimationKeys<int32_t>,
		AnimationKeys<float>,
		AnimationKeys<Vector2>,
		AnimationKeys<Vector3>,
		AnimationKeys<WorldTransform>
		>> animationKeys_;

	// ImGui用変数
	int imGuiSelectKey_; // 選択中キー
	float imGuiAnimSpeed_ = 1.0f; // アニメーション再生速度

	// ループトリガー
	bool isLoop_ = false;

	// 全てのキーの終了状態トリガー
	bool isAllKeyEnd_ = false;
	// アニメーション終了トリガー
	bool isEnd_ = false;

	// アニメーションの破棄トリガー
	bool isDestruction_ = false;

};

template<typename T>
inline void Animation::AddAnimationKeys(const std::string keyName, T* value)
{
	// アニメーションキー配列を新しく追加する処理
	for (auto& keys : animationKeys_) {
		// 格納しているキー名と同一のキーがあった場合にはこのキーの追加は行わない
		std::visit([keyName](auto& key) {
			// 同一キー名を発見した場合
			if (key.keysName_ == keyName) {
				// 追加を行わず処理を抜ける
				return;
			}
		}, keys);
	}

	// 同名キー配列が見つからなかった場合、キーの追加処理を行う
	AnimationKeys newKeys = AnimationKeys<T>(parameterName_, keyName, value); // 新しいキーの生成
	animationKeys_.push_back(newKeys);								 // 配列に追加
}

template<typename T>
inline void Animation::AddAnimationKeys(const std::string keyName)
{
	// アニメーションキー配列を新しく追加する処理
	for (auto& keys : animationKeys_) {
		// 格納しているキー名と同一のキーがあった場合にはこのキーの追加は行わない
		std::visit([keyName](auto& key) {
			// 同一キー名を発見した場合
			if (key.keysName_ == keyName) {
				// 追加を行わず処理を抜ける
				return;
			}
			}, keys);
	}

	// 同名キー配列が見つからなかった場合、キーの追加処理を行う
	AnimationKeys newKeys = AnimationKeys<T>(name_, keyName); // 新しいキーの生成
	animationKeys_.push_back(newKeys);						  // 配列に追加
}
