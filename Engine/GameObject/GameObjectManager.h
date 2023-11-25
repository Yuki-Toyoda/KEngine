#pragma once
#include <list>
#include "BaseObject.h"

/// <summary>
/// ゲームオブジェクトマネージャー
/// </summary>
class GameObjectManager
{
private: // コンストラクタ、デストラクタ等

	// シングルトンパターンの設定
	GameObjectManager() = default;
	~GameObjectManager() = default;
	GameObjectManager(const GameObjectManager&) = delete;
	const GameObjectManager& operator=(const GameObjectManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
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
	/// ゲームオブジェクトインスタンス生成関数
	/// </summary>
	/// <typeparam name="SelectObject">生成オブジェクト</typeparam>
	/// <param name="name">オブジェクト名前</param>
	/// <param name="tag">タグ名</param>
	/// <returns>生成したオブジェクトのインスタンス</returns>
	template<IsBaseObject SelectObject>
	inline SelectObject* CreateInstance(std::string name, BaseObject::Tag tag) {
		// オブジェクトを生成
		SelectObject* newObject = new SelectObject(); // インスタンスを生成
		newObject->PreInitialize(name, tag);		  // 初期化

		// 生成したオブジェクト名をかぶらないように設定
		// 同名オブジェクト個数計算用
		int sameNameCount = 0;

		// 全オブジェクトから同名のオブジェクトがないか探す
		for (BaseObject* object : objects_) {
			// オブジェクト名を取得
			std::string objectName = object->GetObjectName();
			// オブジェクト名の末尾に数字が含まれている場合は末尾の文字を削除
			while (isdigit(objectName.at(objectName.size() - 1)))
			{
				// 末尾の文字を削除
				objectName.pop_back();
			}

			// 同名オブジェクトがあった場合は加算
			if (objectName == newObject->GetObjectName()) {
				sameNameCount++;
			}
		}

		// 同名オブジェクトが一個でも存在する場合末尾に番号をつける
		if (sameNameCount > 0) {
			// 数字を文字列に変換
			std::string count = std::to_string((sameNameCount + 1));
			newObject->SetObjectName(newObject->GetObjectName() + count);
		}

		// 生成したオブジェクトを追加
		objects_.push_back(newObject);

		return newObject;
	}

	/// <summary>
	/// 指定した名前のゲームオブジェクトを取得する関数
	/// </summary>
	/// <typeparam name="SelectObject">取得するオブジェクトの型</typeparam>
	/// <param name="name">取得するオブジェクトの名前</param>
	/// <returns>オブジェクト</returns>
	template<IsBaseObject SelectObject>
	inline SelectObject* GetGameObject(std::string name) {
		// 返還用
		SelectObject* result = new SelectObject();

		// 全オブジェクトから同名のオブジェクトがないか探す
		for (BaseObject* object : objects_) {
			// 同名オブジェクトを見つけた場合
			if (object->GetObjectName() == name) {
				// そのオブジェクトを選択型に返還
				result = dynamic_cast<SelectObject*>(object);
				// 結果がnullptr以外だったらそれを返す
				if (result != nullptr)
					return result;
			}
		}

		// 返還するオブジェクトがなかった場合nullptrを返す
		return nullptr;
	}

	/// <summary>
	/// 指定した型のゲームオブジェクトを一括取得する関数
	/// </summary>
	/// <typeparam name="SelectObject">取得するオブジェクトの型</typeparam>
	/// <returns>オブジェクト達(vector)</returns>
	template<IsBaseObject SelectObject>
	inline std::vector<SelectObject*> GetGameObject() {
		// 返還用
		std::vector<SelectObject*> result;

		// 全オブジェクトから同名のオブジェクトがないか探す
		for (BaseObject* object : objects_) {
			// そのオブジェクトを選択型に返還
			SelectObject* o = dynamic_cast<SelectObject*>(object);
			// 結果がnullptr以外だったらそれを配列に追加
			if (o != nullptr)
				result.push_back(o);
		}

		// 返還するオブジェクトがなかった場合nullptrを返す
		return result;
	}

	/// <summary>
	/// 指定した型のゲームオブジェクトを一括取得する関数
	/// </summary>
	/// <typeparam name="SelectObject">取得するオブジェクトの型</typeparam>
	/// <returns>オブジェクト達(vector)</returns>
	template<IsBaseObject SelectObject>
	inline std::list<SelectObject*> GetGameObjectList() {
		// 返還用
		std::list<SelectObject*> result;

		// 全オブジェクトから同名のオブジェクトがないか探す
		for (BaseObject* object : objects_) {
			// そのオブジェクトを選択型に返還
			SelectObject* o = dynamic_cast<SelectObject*>(object);
			// 結果がnullptr以外だったらそれを配列に追加
			if (o != nullptr)
				result.push_back(o);
		}

		// 返還するオブジェクトがなかった場合nullptrを返す
		return result;
	}

	/// <summary>
	/// 指定したインデックスをBaseObject型で受け取る関数
	/// </summary>
	/// <param name="index">取得する番号</param>
	/// <returns>インデックスのゲームオブジェクト</returns>
	BaseObject* GetGameObject(int index);

private: // メンバ変数

	// オブジェクトリスト
	std::list<BaseObject*> objects_;

#ifdef _DEBUG // ImGui用デバッグ変数

	// 選択中オブジェクト
	int imGuiSelectObjects_;

#endif // _DEBUG // ImGui用デバッグ変数


};

