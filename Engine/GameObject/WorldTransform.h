#pragma once
#include <string>
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../Externals/imgui/ImGui.h"
#include "../math/Math.h"

/// <summary>
/// キーフレーム構造体
/// </summary>
/// <typeparam name="tValue">キーフレームの値の型</typeparam>
template<typename tValue>
struct KeyFrame {
	float  time;  // キーフレームの秒数
	tValue value; // キーフレームの値
};

// 3次元ベクトルキーフレーム
using keyFrameVector3 = KeyFrame<Vector3>;
// クォータニオンキーフレーム
using KeyFrameQuaternion = KeyFrame<Quaternion>;

/// <summary>
/// アニメーションカーブ構造体
/// </summary>
/// <typeparam name="tValue">カーブの型</typeparam>
template<typename tValue>
struct AnimationCurve {
	// キーフレーム配列
	std::vector<KeyFrame<tValue>> keyFrames;
};

/// <summary>
/// ノードアニメーション構造体
/// </summary>
struct NodeAnimation {
	AnimationCurve<Vector3>    translate; // 位置座標
	AnimationCurve<Quaternion> rotate;	  // 回転
	AnimationCurve<Vector3>    scale;	  // 拡縮
};

/// <summary>
/// アニメーション構造体
/// </summary>
struct Animation {
	float duration;										 // アニメーション全体の尺
	float animationTime = 0.0f;							 // 再生中アニメーションの秒数
	bool isPlay;										 // アニメーション再生トリガー
	bool isLoop;										 // アニメーションのループトリガー
	std::map<std::string, NodeAnimation> nodeAnimations; // ノードアニメーション配列

	// 3次元ベクトル計算関数
	static Vector3 CalculateValue(const std::vector<keyFrameVector3>& keyFrames, float time) {
		// キーがない場合返す値がないので停止させる
		assert(!keyFrames.empty());

		// キーが１つ、または秒数がキーフレーム前なら最初の値とする
		if (keyFrames.size() == 1 || time <= keyFrames[0].time) {
			// 最初の値を返す
			return keyFrames[0].value;
		}

		// キーフレーム数分ループ
		for (size_t index = 0; index < keyFrames.size() - 1; index++) {
			// 次のインデックスを求める
			size_t nextIndex = index + 1;

			// 現在のインデックスと次のインデックスの2つのキーフレームを取得し、範囲内に秒数があるか判定
			if (keyFrames[index].time <= time && time <= keyFrames[nextIndex].time) {
				// 範囲内を補完する
				float t = (time - keyFrames[index].time) / (keyFrames[nextIndex].time - keyFrames[index].time);
				// 線形補間を行う
				return Math::Linear(t, keyFrames[index].value, keyFrames[nextIndex].value);
			}
		}

		// ここまで来た場合は最後の値を返す
		return (*keyFrames.rbegin()).value;
	}

	// クォータニオン計算関数
	static Quaternion CalculateValue(const std::vector<KeyFrameQuaternion>& keyFrames, float time) {
		// キーがない場合返す値がないので停止させる
		assert(!keyFrames.empty());

		// キーが１つ、または秒数がキーフレーム前なら最初の値とする
		if (keyFrames.size() == 1 || time <= keyFrames[0].time) {
			// 最初の値を返す
			return keyFrames[0].value;
		}

		// キーフレーム数分ループ
		for (size_t index = 0; index < keyFrames.size() - 1; index++) {
			// 次のインデックスを求める
			size_t nextIndex = index + 1;

			// 現在のインデックスと次のインデックスの2つのキーフレームを取得し、範囲内に秒数があるか判定
			if (keyFrames[index].time <= time && time <= keyFrames[nextIndex].time) {
				// 範囲内を補完する
				float t = (time - keyFrames[index].time) / (keyFrames[nextIndex].time - keyFrames[index].time);
				// 線形補間を行う
				return Math::Slerp(t, keyFrames[index].value, keyFrames[nextIndex].value);
			}
		}

		// ここまで来た場合は最後の値を返す
		return (*keyFrames.rbegin()).value;
	}

};

/// <summary>
/// ワールド座標計算クラス
/// </summary>
class WorldTransform {
public: // サブクラス

	/// <summary>
	/// ノード構造体
	/// </summary>
	struct Node {
		Matrix4x4 localMatrix;		// NodeのLocalMatrix
		std::string name;			// ノード名
		std::vector<Node> children; // 子供のノード
		bool hasNode = false;		// ノードを所持しているか
	};

public: // パブリックメンバ変数
	// 拡縮
	Vector3 scale_;
	// 回転
	Vector3 rotate_;
	// 位置
	Vector3 translate_;

public: // パブリックメンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WorldTransform();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">位置</param>
	WorldTransform(Vector3 scale, Vector3 rotate, Vector3 translate);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="rotate">回転</param>
	/// <param name="translate">位置</param>
	WorldTransform(Vector3 rotate, Vector3 translate);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="translate">位置</param>
	WorldTransform(Vector3 translate);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	/// <param name="id">ウィンドウ表示名</param>
	void DisplayImGui(const std::string& id);
	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	/// <param name="id">ツリーノード名</param>
	void DisplayImGuiWithTreeNode(const std::string& id);

public: // パブリックなメンバ変数

	// 親ノード
	Node rootNode_;

	// アニメーション配列
	std::vector<Animation> animations_;

private: // メンバ変数

	// 親子関係
	const WorldTransform* parent_;

	// 親子関係タイプ
	// 0x111 ... scale, rotate, translate 全て
	// それぞれ scale rotate translate
	uint8_t parentType_;

	// 直接ワールド行列を代入したい場合はこの変数に代入
	const Matrix4x4* worldMat_ = nullptr;

	// 主にアニメーション再生に用いるローカル行列
	const Matrix4x4* localMat_ = nullptr;

public: // アクセッサ等

	/// <summary>
	/// 親子関係のセッター
	/// </summary>
	/// <param name="parent">セットするワールド座標</param>
	/// <param name="parentType">親子関係フラグ</param>
	void SetParent(WorldTransform* parent, uint8_t parentType = 0b111);
	/// <summary>
	/// 親のゲッター
	/// </summary>
	/// <returns>親</returns>
	const WorldTransform* GetParent();

	/// <summary>
	/// ワールド行列のセッター
	/// </summary>
	/// <param name="mat">ワールド行列</param>
	void SetWorldMat(const Matrix4x4& mat) { worldMat_ = &mat; };
	/// <summary>
	/// ワールド行列削除関数
	/// </summary>
	void DeleteWorldMat() { worldMat_ = nullptr; }

	/// <summary>
	/// ローカル行列のセッター
	/// </summary>
	/// <param name="mat">ローカル行列</param>
	void SetLocalMat(const Matrix4x4& mat) { localMat_ = &mat; }

	/// <summary>
	/// 現在のワールド行列のゲッター
	/// </summary>
	/// <returns>ワールド行列</returns>
	Matrix4x4 GetMatWorld() const;

	/// <summary>
	/// 現在のワールド座標のゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPos() const;

public: // 演算子オーバーロード

	// 演算子オーバーロード(+)
	WorldTransform operator+(const WorldTransform wt) const;

	// 演算子オーバーロード(-)
	WorldTransform operator-(const WorldTransform wt) const;

	// 演算子オーバーロード(*)
	WorldTransform operator*(const WorldTransform wt) const;
	// 演算子オーバーロード(*)(float)
	WorldTransform operator*(const float f) const;

	// 演算子オーバーロード(/)
	WorldTransform operator/(const WorldTransform wt) const;

};
