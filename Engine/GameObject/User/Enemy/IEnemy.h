#pragma once
#include "../../BaseObject.h"
#include "../Player/Weapon.h"
#include "State/EnemyStateList.h"
#include "../Editor/GameDataManager.h"
#include"../Player/Player.h"
#include"Atack/PushUp.h"
/// <summary>
/// 敵の基底クラス
/// </summary>
class IEnemy : public BaseObject
{
public: // メンバ関数
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;
	/// <summary>
	/// 固有初期化関数
	/// </summary>
	virtual void SuccessorInit() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;
	/// <summary>
	/// 固有更新関数
	/// </summary>
	virtual void SuccessorUpdate() = 0;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// リセット関数
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// 引数で指定した行動状態に変更する関数
	/// </summary>
	/// <param name="newState">新しい行動状態</param>
	void ChangeState(std::unique_ptr<IEnemyState> newState);
	
	//State名から行動状態を変更する関数

	void ChangeState(std::string name);
	/// <summary>
	/// パラメーターを調整項目クラスに追加する関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void AddParameter(const std::string& levelName, const std::string& enemyName);
	/// <summary>
	/// パラメータのセーブ関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void SetParameter(const std::string& levelName, const std::string& enemyName);
	/// <summary>
	/// パラメータのロード関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void ApplyParameter(const std::string& levelName, const std::string& enemyName);

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override = 0;

public: // アクセッサ等

	/// <summary>
	/// 武器のセッター
	/// </summary>
	/// <param name="target"></param>
	void SetWepon(Weapon* weapon) { weapon_ = weapon; }

	/// <summary>
	/// 移動ベクトルセッター
	/// </summary>
	/// <param name="velocity">移動ベクトル</param>
	void Setveclocity(Vector3 velocity) { hitvelocity_ = velocity; }

	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }
	//攻撃待機時間のセッター
	void SetWaitTime(float time) { waitTime_ = time; }
	//攻撃待機時間のゲッター
	float GetWaitTime() { return waitTime_; }
public: // パブリックなメンバ変数

	// 外部出力マネージャ
	GameDataManager* gameDataManager_ = nullptr;
   //下からの攻撃
	std::vector<PushUp*> pushUp_;

	//行動状態のパターン
	std::vector<std::vector<std::unique_ptr<IEnemyState>>>stateList_;
	//行動状態のパターンのナンバー
	int patternNumber_ = 0;
	//行動状態のパターン内のナンバー　　（stateList.at(patternNumer).at(stateNumber)）
	int StateNumber_ = 0;

protected: // 継承先メンバ変数

	// 音再生用クラス
	Audio* audio_;

	// 武器
	Weapon* weapon_ = nullptr;
	// 武器とペアレントをしているか
	bool isParent_;

	// 当たり判定用ワールド座標
	Vector3 worldPos_;

	//速度方向ベクトル
	Vector3 velocity_;
	Vector3 hitvelocity_;

	// 行動状態クラス
	std::unique_ptr<IEnemyState> state_;
	
	Player* player_;
	//攻撃後の待機時間
	float waitTime_=2.0f;
};

/// <summary>
/// IEnemyを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectEnemy>
concept IsIEnemy = std::is_base_of<IEnemy, SelectEnemy>::value;

