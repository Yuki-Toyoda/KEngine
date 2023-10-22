#pragma once
#include <string>
#include "../BaseObject.h"

// クラスの前方宣言
class Input;
class Camera;
class Item;
class StageManager;

/// <summary>
/// ステージセレクト画面のマネージャー
/// </summary>
class StageSelectManagerObject : public BaseObject {
private: // サブクラス

	/// <summary>
	/// 演出中間地点列挙子
	/// </summary>
	enum StagingWayPoint {
		WayPoint1,
		WayPoint2,
		WayPoint3,
		WayPoint4,
		WayPoint5,
		WayPoint6,
	};

public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;
	// スプライト描画
	void SpriteDraw() override;

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
	/// カメラセッター
	/// </summary>
	/// <param name="camera"></param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// 遷移演出関数
	/// </summary>
	void TransitionStaging();

	/// <summary>
	/// カメラ手振れ演出関数
	/// </summary>
	void CameraShake();

	/// <summary>
	/// ステージプレビュー回転開始
	/// </summary>
	/// <param name="isRight">右方向に回転するか</param>
	void RotateStart(bool isRight);

	/// <summary>
	/// ステージプレビュー回転関数
	/// </summary>
	void RotateStagePreview();

	/// <summary>
	/// プレビューアイテムの座標設定
	/// </summary>
	void SetPreviewItems();

	/// <summary>
	/// プレビューアイテム演出関数
	/// </summary>
	void PrevItemStaging();

	/// <summary>
	/// ゲームシーンへの遷移トリガーゲッター
	/// </summary>
	/// <returns>ゲームシーンへ遷移するか</returns>
	bool IsGoGameScene() { return isGoGameScene_; }

private: // メンバ変数

	// 入力
	Input* input_ = nullptr;

	// 全体のステージ数
	int stageCount_;

	// ステージのプレビューモデルを表示させる座標配列
	WorldTransform mainGearTransform_;
	WorldTransform previewStageTransforms_[4];

	// 選択中のステージ番号
	int selectedStageNumber_;
	// 切り替えボタン押下回数デクリメント
	int pressCount_;

	// 演出用t
	float rotateStagingT_;
	// 演出時間
	float rotateStagingTime_;
	// 演出中トリガー
	bool isRotateStaging_;

	// 角度
	float startAngle_;
	float endAngle_;

	// カメラ
	Camera* camera_ = nullptr;
	// カメラ手振れ演出トリガー
	bool enableCameraShake_;
	// カメラ演出用t
	float cameraStagingT_;
	// カメラ演出用tのループトリガー
	bool cameraStagingTReturn_;
	// カメラ演出用t2
	float cameraStagingT2_;
	// カメラ演出用t2のループトリガー
	bool cameraStagingT2Return_;

	// 演出中間地点
	int cameraStagingWayPoint_;

	// 遷移演出トリガー
	bool isTransitionStaging_;
	// 遷移演出用t
	float transitionStagingT_;
	// 遷移演出時間
	float transitionStagingTime_;

	// カメラ演出始端座標
	Vector3 cameraStartTranslate_;
	// カメラ演出終端座標
	Vector3 cameraEndTranslate_;

	// ゲームシーンへの遷移トリガー
	bool isGoGameScene_;

	StageManager* stageManager_ = nullptr;
	// プレビュー上のアイテムオブジェクト
	std::vector<Item*>previewItems_;

	// プレビューアイテム表示演出中か
	bool previewItemStaging_;
	// プレビューアイテム用の演出t
	float previewItemStagingT_;
};

