#pragma once
#include <list>
#include "AnimationKey.h"
#include "Engine/Utility/KLib.h"

// テンプレートの定義
template<typename T>

/// <summary>
/// アニメーションキー配列クラス
/// </summary>
class AnimationKeys
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="keyName">キー名</param>
	AnimationKeys(const std::string& animationName, const std::string& keyName);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="keyName">キー名</param>
	/// <param name="value">アニメーションで動かす値</param>
	AnimationKeys(const std::string& animationName, const std::string& keyName, T* value);

public: // メンバ関数

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 再生関数
	/// </summary>
	/// <param name="keyIndex">再生するキー番号</param>
	void Play(const int32_t& keyIndex);

	/// <summary>
	///	アニメーション再生関数
	/// </summary>
	/// <param name="transitionTime">前のアニメーションからの補間秒数</param>
	void Play(const float& transitionTime);

	/// <summary>
	/// ImGuiの表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// キー配列のソートを行う関数
	/// </summary>
	void SortKey();

public: // アクセッサ等

	/// <summary>
	/// アニメーションフレーム数の取得
	/// </summary>
	/// <returns>アニメーション全体のフレーム数</returns>
	int32_t GetAnimationFrameCount() {
		if (keys_.size() != 0) {
			return keys_.back().playFrame_;
		}

		return 0;
	}

	/// <summary>
	/// キー配列ごとのアニメーション進捗ゲッター
	/// </summary>
	/// <returns>アニメーション進捗</returns>
	float GetKeysProgress() { return animTimer_.GetProgress(); }

	/// <summary>
	/// アニメーション再生速度セッター
	/// </summary>
	/// <param name="animSpeed">再生速度</param>
	void SetAnimSpeed(const float& animSpeed);

private: // プライベートなメンバ関数

	/// <summary>
	/// アニメーションキーの追加関数
	/// </summary>
	/// <param name="keyIndex">追加するインデックス位置</param>
	void AddKey(int32_t keyIndex);

	/// <summary>
	/// 次のキーへ遷移させる関数
	/// </summary>
	void NextKey();

	/// <summary>
	/// 調整項目クラスに値を追加する関数
	/// </summary>
	void AddItem();

	/// <summary>
	/// 調整項目クラスに値をセットする関数
	/// </summary>
	void SetItem();

	/// <summary>
	/// 調整項目クラスから値を取得する関数
	/// </summary>
	void ApplyItem();

public: // メンバ変数

	// アニメーション名
	std::string name_;
	// キー配列自体の名前
	std::string keysName_;

	// アニメーションキー配列
	std::list<AnimationKey<T>> keys_;

	// アニメーションさせる実体
	T* animateObject_ = nullptr;

	// キーの総数
	int32_t keyCount_ = 0;
	// 再生中キー
	int32_t playingKey_ = 0;

	// アニメーション変更時のパラメータ保持用
	T prevParameter_;
	// アニメーション間の補間にかかる秒数
	float transitionTime_ = 0.0f;
	// アニメーション間の遷移を行っているか
	bool isTransitionning_ = false;

	// 線形補間を行うキーの取得
	AnimationKey<T> prevKey_; // 前フレーム用
	AnimationKey<T> nextKey_; // 次フレーム用

	// 線形補間用タイマー
	KLib::DeltaTimer lerpTimer_;
	// アニメーション全体のタイマー
	KLib::DeltaTimer animTimer_;

	// 再生トリガー
	bool isPlay_ = false;

	// 終了トリガー
	bool isEnd_ = false;
};

template<typename T>
inline AnimationKeys<T>::AnimationKeys(const std::string& animationName, const std::string& keyName)
{
	// 名称設定
	name_ = animationName;
	// キー名称設定
	keysName_ = keyName;

	// 再生トリガーは一度false
	isPlay_ = false;

	// キーの値を設定
	prevKey_ = AnimationKey<T>(name_, keysName_, 0);
	nextKey_ = AnimationKey<T>(name_, keysName_, 0);

	// キー達を配列に追加
	AddItem();
	// キー配列を取得
	ApplyItem();
}

template<typename T>
inline AnimationKeys<T>::AnimationKeys(const std::string& animationName, const std::string& keyName, T* value)
{
	// 名称設定
	name_ = animationName;
	// キー名称設定
	keysName_ = keyName;

	// アニメーションさせるオブジェクトの取得
	animateObject_ = value;

	// 再生トリガーは一度false
	isPlay_ = false;

	// キーの値を設定
	prevKey_ = AnimationKey<T>(name_, keysName_, 0);
	nextKey_ = AnimationKey<T>(name_, keysName_, 0);

	// キー達を配列に追加
	AddItem();
	// キー配列を取得
	ApplyItem();
}

template<typename T>
inline void AnimationKeys<T>::Update()
{
	// キーの総数を取得
	keyCount_ = (int)keys_.size();

	// 再生中か、キーのサイズが2以上の時
	if (isPlay_ && keys_.size() >= 2 && animateObject_ != nullptr) {

		if (!isTransitionning_) {
			// 再生中フレームで遷移させる
			*animateObject_ = KLib::Lerp<T>(prevKey_.value_, nextKey_.value_, nextKey_.ease_(lerpTimer_.GetProgress()));
		}
		else {
			// 再生中フレームで遷移させる
			*animateObject_ = KLib::Lerp<T>(prevKey_.value_, nextKey_.value_, KLib::EaseInOutQuad(lerpTimer_.GetProgress()));
		}


		// 各種タイマー更新
		lerpTimer_.Update(); // 線形補間
		animTimer_.Update(); // アニメーション全体時間

		// 線形補間タイマーが終了している時
		if (lerpTimer_.GetIsFinish()) {
			// アニメーションの終端地点だった場合
			if (playingKey_ < keys_.size() - 1) {
				// 次のキーへ
				NextKey();
			}
			else {
				// 終了トリガーtrue
				isEnd_ = true;
				// 再生中でない
				isPlay_ = false;
			}
		}
	}
}

template<typename T>
inline void AnimationKeys<T>::Play(const int32_t& keyIndex) {

	// キーの情報を読み取る
	if (animateObject_ != nullptr) {
		ApplyItem();
	}

	// そもそもアニメーションのキー数が2以下だった場合再生すらしない
	if (keys_.size() < 2) {
		isEnd_ = true;
		isPlay_ = false;
		return;
	}

	// この関数を使用した場合補間を行わない
	isTransitionning_ = false;

	// 再生中キーを指定したキーに変更
	playingKey_ = keyIndex;

	// アニメーションの最終フレームを超過していた場合1フレーム前までさかのぼる
	if (playingKey_ >= keys_.size()) {
		// 最終フレームの１つ前のフレームに設定
		playingKey_ = (int)keys_.size() - 2;
	}

	// リストからイテレータで値を取得
	auto pKeyIt = std::next(keys_.begin(), playingKey_);
	auto nKeyIt = std::next(keys_.begin(), playingKey_ + 1);

	// 現在のキーで値を設定
	prevKey_ = *pKeyIt;
	nextKey_ = *nKeyIt;

	// 次のフレームとの差分を求める
	int difFrame = nextKey_.playFrame_ - prevKey_.playFrame_;

	// 遷移時間タイマーを (1フレームの時間 * 差分フレーム) で開始
	lerpTimer_.Start((1.0f / 60.0f) * difFrame);
	// アニメーション時間タイマーを(1フレームの時間 * 最終フレーム) で開始
	animTimer_.Start((1.0f / 60.0f) * keys_.back().playFrame_);

	// 再生開始
	isPlay_ = true;
	// 再生中
	isEnd_ = false;

}

template<typename T>
inline void AnimationKeys<T>::Play(const float& transitionTime)
{
	// キーの情報を読み取る
	if (animateObject_ != nullptr) {
		ApplyItem();
	}

	// そもそもアニメーションのキー数が2以下だった場合再生すらしない
	if (keys_.size() < 2) {
		isEnd_ = true;
		isPlay_ = false;
		return;
	}

	// 再生中キーを指定したキーに変更
	playingKey_ = 0;

	// アニメーションの最終フレームを超過していた場合1フレーム前までさかのぼる
	if (playingKey_ >= keys_.size()) {
		// 最終フレームの１つ前のフレームに設定
		playingKey_ = (int)keys_.size() - 2;
	}

	// アニメーション中のパラメータの現在の状態を取得
	if (animateObject_ != nullptr) {
		prevParameter_ = *animateObject_;
	}
	// 補間秒数の取得
	transitionTime_ = transitionTime;
	// 補間を開始する
	isTransitionning_ = true;

	// リストからイテレータで値を取得
	auto nKeyIt = std::next(keys_.begin(), playingKey_);

	// 現在のキーで値を設定
	prevKey_.value_ = prevParameter_;
	nextKey_ = *nKeyIt;

	// 遷移時間タイマーを指定された遷移秒数で開始
	lerpTimer_.Start(transitionTime_);

	// 再生開始
	isPlay_ = true;
	// 再生中
	isEnd_ = false;

}

template<typename T>
inline void AnimationKeys<T>::DisplayImGui()
{
	// アニメーション名
	std::string name = "Now SelectKeys : " + keysName_;
	ImGui::Text(name.c_str());

	// 破棄するキーがある場合削除する
	keys_.remove_if([](AnimationKey<T>& key) {
		if (key.isDelete_) {
			return true;
		}
		return false;
		});

	// ボタンを押すとキーと追加
	if (ImGui::Button("AddKey")) {
		AddKey((int32_t)(keys_.size()));
	}

	// 全てのキーのImGuiを表示
	for (AnimationKey<T>& key : keys_) {
		key.DisplayImGui();
	}

	// パラメーター全てをセット
	SetItem();
}

template<typename T>
inline void AnimationKeys<T>::SetAnimSpeed(const float& animSpeed)
{
	// 各種タイマーの再生速度を設定
	lerpTimer_.SetTimerSpeed(animSpeed);
	animTimer_.SetTimerSpeed(animSpeed);
}

template<typename T>
inline void AnimationKeys<T>::AddKey(int32_t keyIndex)
{
	// 新しいキーを生成
	AnimationKey newKey = AnimationKey<T>(name_, keysName_, keyIndex);
	// 生成したキーを管理配列に追加
	keys_.push_back(newKey);

	// 配列情報を追加
	AddItem();
}

template<typename T>
inline void AnimationKeys<T>::NextKey()
{
	// 遷移が行われていないなら
	if (!isTransitionning_) {
		// 次のキーへ
		playingKey_++;
	}
	else {
		// 遷移を無効にする
		isTransitionning_ = false;

		// アニメーション時間タイマーを(1フレームの時間 * 最終フレーム) で開始
		animTimer_.Start((1.0f / 60.0f) * keys_.back().playFrame_);
	}

	// リストからイテレータで値を取得
	auto pKeyIt = std::next(keys_.begin(), playingKey_);
	// 現在のキーで値を設定
	prevKey_ = *pKeyIt;

	if (keyCount_ - 1 != playingKey_) {
		auto nKeyIt = std::next(keys_.begin(), playingKey_ + 1);
		nextKey_ = *nKeyIt;

		// 次のフレームとの差分を求める
		int difFrame = nextKey_.playFrame_ - prevKey_.playFrame_;

		// 遷移時間タイマーを (1フレームの時間 * 差分フレーム) で開始
		lerpTimer_.Start((1.0f / 60.0f) * difFrame);
	}
}

template<typename T>
inline void AnimationKeys<T>::SortKey()
{
	// キー配列を再生フレームが早い順にソート
	keys_.sort([](AnimationKey<T>key1, AnimationKey<T>key2) {return key1.playFrame_ < key2.playFrame_; });

	// インデックス計算用
	size_t i = 0;
	// キーカウントも昇順に調整
	for (AnimationKey<T>& k : keys_) {
		k.playKeyCount_ = (int32_t)i;
		i++;
	}
}

template<typename T>
inline void AnimationKeys<T>::AddItem()
{
	// キーの総数を取得
	keyCount_ = (int32_t)keys_.size();
	// キー総数情報追加
	std::string keyName = keysName_ + " : KeyCount";
	GlobalVariables::GetInstance()->AddItem(name_, keyName, keyCount_);

	// 全てのキーの情報追加を行う
	for (AnimationKey a : keys_) {
		a.AddParam();
	}
}

template<typename T>
inline void AnimationKeys<T>::SetItem()
{
	// キーの総数を取得
	keyCount_ = (int32_t)keys_.size();

	// キー総数情報追加
	std::string keyName = keysName_ + " : KeyCount";
	GlobalVariables::GetInstance()->SetValue(name_, keyName, keyCount_);

	// キーが１つもなければ情報追加を行わない
	if (keys_.size() != 0) {
		// 全てのキーの情報セットを行う
		for (AnimationKey a : keys_) {
			a.SetParam();
		}
	}
}

template<typename T>
inline void AnimationKeys<T>::ApplyItem()
{
	// キー総数の取得
	std::string keyName = keysName_ + " : KeyCount";
	keyCount_ = GlobalVariables::GetInstance()->GetIntValue(name_, keyName);

	// キー配列を一度クリアする
	keys_.clear();

	// 取得したキー総数を元にその数分キーを生成する
	for (int i = 0; i < keyCount_; i++) {
		// 新しいキーを生成
		AnimationKey newKey = AnimationKey<T>(name_, keysName_, i);
		// 生成したキーを管理配列に追加
		keys_.push_back(newKey);
	}

}