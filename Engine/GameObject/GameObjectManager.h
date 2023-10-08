#pragma once
#include <list>
#include "../Base/DirectXCommon.h"
#include "BaseObject.h"

/// <summary>
/// 全てのゲームオブジェクトの管理マネージャ
/// </summary>
class GameObjectManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	GameObjectManager() = default;
	~GameObjectManager() = default;
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static GameObjectManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// 生成したゲームオブジェクトをリストに追加する関数
	/// </summary>
	/// <param name="gameObject">リストに追加するオブジェクト</param>
	void AddGameObject(BaseObject* gameObject);

	/// <summary>
	/// 引数で指定した名前のオブジェクトを取得する関数
	/// </summary>
	/// <param name="objectName">取得するオブジェクト名</param>
	/// <returns>取得したいゲームオブジェクト</returns>
	BaseObject* GetGameObject(std::string objectName);
	/// <summary>
	/// 引数で指定したタグのオブジェクト全てを取得する関数
	/// </summary>
	/// <returns>取得したいゲームオブジェクトリスト</returns>
	std::vector<BaseObject*> GetGameObject(BaseObject::Tag tag);

	/// <summary>
	/// 引数で指定した名前のオブジェクトを削除する関数
	/// </summary>
	/// <param name="objectName">削除したいゲームオブジェクト名</param>
	void DeleteGameObject(std::string objectName);
	/// <summary>
	/// 引数で指定したタグのオブジェクト全てを削除する関数
	/// </summary>
	/// <param name="tag">削除したいゲームオブジェクトのタグ</param>
	void DeleteGameObject(BaseObject::Tag tag);

private: // メンバ関数

	// ゲームオブジェクトのリスト
	std::list<BaseObject*> objects_;

};

