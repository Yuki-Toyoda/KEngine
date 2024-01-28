#pragma once
#include "../../BaseObject.h"
#include "../../../Audio/Audio.h"
#include "State/EnemyStateList.h"
#include "../Editor/GameDataManager.h"
#include"../Player/Player.h"
#include"Atack/PushUp.h"

// クラスの前方宣言
class BossAnimManager;

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
	//待機時間のゲッター
	float GetWaitSingle() { return waitForSingle_; }
	float GetWaitRoller() { return waitForRoller_; }
	float GetWaitMulti() { return waitForMulti_; }
	float GetWaitPushUp() { return waitForPushUp_; }
	void SetgameManager(GameManager* gamemanager) { gameManager_ = gamemanager; }

	/// <summary>
	/// 落下攻撃準備時間ゲッター
	/// </summary>
	/// <returns>落下攻撃準備時間</returns>
	float GetFallAttackReadyTime() { return fallAttackReadyTime_; }
	
	/// <summary>
	/// 突き上げ攻撃準備時間ゲッター
	/// </summary>
	/// <returns>突き上げ攻撃準備時間</returns>
	float GetPushUpReadyTime() { return pushUpReadyTime_; }

	
	float GetRollerAttackReadyTime() { return rollerAttackReadyTime_; }

	/// <summary>
	/// ボスのアニメーションマネージャーセッター
	/// </summary>
	/// <param name="bam">ボスのアニメーションマネージャー本体</param>
	void SetBossAnimManager(BossAnimManager* bam) { bam_ = bam; }
	/// <summary>
	/// ボスのアニメーションマネージャーゲッター
	/// </summary>
	/// <returns>ボスのアニメーションマネージャー</returns>
	BossAnimManager* GetBossAnimManager() { return bam_; }

public: // パブリックなメンバ変数
	struct MyState
	{
		//行動状態のパターン　第一引数がパターンの行う順番で第二引数が行う行動
		std::vector<std::vector<std::unique_ptr<IEnemyState>>>state_;
		//行動状態の呼び出す番号　　第一引数がパターンの行う順番で第二引数が行う行動の設置場所
		std::vector<std::vector<int>>stateNumber_;
	};
	// 外部出力マネージャ
	GameDataManager* gameDataManager_ = nullptr;
   //下からの攻撃
	std::vector<PushUp*> pushUp_;

	MyState stateList_;
	//行動状態のパターンのナンバー
	int patternNumber_ = 0;
	//行動状態のパターン内のナンバー　　（stateList.at(patternNumer).at(stateNumber)）
	int StateNumber_ = 0;
	GameManager* gameManager_;

protected: // 継承先メンバ変数

	// 音再生用クラス
	Audio* audio_;

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
	//singleAtack後の待機
	float waitForSingle_=2.0f;
	//MultiAtack後の待機
	float waitForMulti_=2.0f;
	//RollerAtack後の待機
	float waitForRoller_=2.0f;
	//PushUpAtack後の待機
	float waitForPushUp_=2.0f;

	// ボスのアニメーションマネージャー
	BossAnimManager* bam_ = nullptr;

	/// ボスの攻撃時の待機アニメーション秒数
	// ボスの落下攻撃時の待機時間
	float fallAttackReadyTime_ = 0.25f;
	// ボスの突き上げ攻撃時の待機時間
	float pushUpReadyTime_ = 0.25f;
	// ボスのローラー攻撃時の待機時間
	float rollerAttackReadyTime_ = 0.25f;
};

/// <summary>
/// IEnemyを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectEnemy>
concept IsIEnemy = std::is_base_of<IEnemy, SelectEnemy>::value;

