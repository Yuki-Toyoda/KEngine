#pragma once
#include <string>
#include "../BaseObject.h"

// クラスの前方宣言
class Input;

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
	/// ステージプレビュー回転開始
	/// </summary>
	/// <param name="isRight">右方向に回転するか</param>
	void RotateStart(bool isRight);

	/// <summary>
	/// ステージプレビュー回転関数
	/// </summary>
	void RotateStagePreview();

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
	float stagingT_;
	// 演出時点での回転開始角度
	float startAngle_;
	float endAngle_;
};

