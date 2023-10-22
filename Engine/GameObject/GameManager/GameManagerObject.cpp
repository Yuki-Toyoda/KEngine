#include "GameManagerObject.h"
#include "../../Scene/SceneManager.h"

void GameManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// ギアの座標を初期化
	for (int i = 0; i < 3; i++) {
		gearTransforms_[i].Initialize();
		gearTransforms_[i].scale_ = mainGearTransform_->scale_; // スケールをメインのギアと合わせる
	}

	gearTransforms_[0].translate_ = { -27.0f, -12.0f, 0.0f };
	gearTransforms_[1].translate_ = { -24.0f, 12.5f, 0.0f };
	gearTransforms_[2].translate_ = { 25.0f, -16.0f, 0.0f };

	// クリアゲージの初期化
	clearGageTransform_.Initialize();
	clearGageTransform_.scale_ = {14.5f, 14.5f, 1.0f};
	// クリア時のチェックマークワールド座標
	clearCheckTransform_.Initialize();
	clearCheckTransform_.translate_ = { 0.0f, -100.0f, 0.0f };
	clearCheckTransform_.scale_ = { 0.0f, 0.0f, 1.0f };

	// モデル読み込み
	AddOBJ(&gearTransforms_[0], color_, "./Resources/Gear", "Gear_UI.obj", true);
	AddOBJ(&gearTransforms_[1], color_, "./Resources/Gear", "Gear_UI_S.obj", true);
	AddOBJ(&gearTransforms_[2], color_, "./Resources/Gear", "Gear_M.obj", true);
	AddOBJ(&clearGageTransform_, color_, "./Resources/ClearGage", "ClearGage.obj", false);
	AddOBJ(&clearCheckTransform_, color_, "./Resources/Plane", "Plane.obj", false);

	// 始端、終端座標設定
	cameraStartTranslate_ = camera_->transform_.translate_;
	cameraEndTranslate_ = { -10.0f, 0.0f, -80.0f };
	// 演出中間地点初期化
	cameraStagingWayPoint_ = WayPoint1;
	// 演出用t初期化
	cameraStagingT_ = 0.0f;
	// 演出時間初期化
	cameraStagingTime_ = 1.25f;

	// ステージ内のアイテム個数取得 （仮置き）
	stageItemCount_ = 0;
	stageNowItemCount_ = 0;

	// ステージクリア進捗(仮置き)
	stageClearPercent_ = 0;

	// ステージセレクトシーンへのトリガー
	isGoStageSelectScene_ = false;

	/// テクスチャ読み込み
	textureHandleNumberSheets_ = TextureManager::Load("./Resources/Image", "MyNumberSheets.png"); // 番号シート
	textureHandleTextLeftItem_ = TextureManager::Load("./Resources/Image/Game", "LeftItem.png"); // 残りアイテムテキスト
	textureHandleSlash_ = TextureManager::Load("./Resources/Image/Game", "Slash.png"); // /テクスチャ
	textureHandleTextClearPercent_ = TextureManager::Load("./Resources/Image/Game", "ClearPercent.png"); // クリア進捗テキスト

	// UI全体の色リセット
	spriteUIColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };

	/// スプライト生成
	// 残りアイテム数テキストUI
	leftItemTextPosition_ = { 307.5f, 520.0f }; // 座標
	leftItemTextSize_ = { 284.0f, 56.0f }; // 大きさ
	leftItemTextSprite_.reset(Sprite::Create(
			textureHandleTextLeftItem_, 
			&leftItemTextPosition_, 
			&leftItemTextSize_, 
			&spriteUIColor_, 
			{ 0.5f, 0.5f })); // 生成
	// 残りアイテム数テキストUI
	slashPosition_ = { 310.0f, 620.0f }; // 座標
	slashSize_ = { 64.0f, 64.0f }; // 大きさ
	slashSprite_.reset(Sprite::Create(
		textureHandleSlash_,
			&slashPosition_,
			&slashSize_,
			&spriteUIColor_, 
			{ 0.5f, 0.5f })); // 生成
	// クリア進捗テキストUI
	clearPercentTextPosition_ = { 365.0f, 82.5f }; // 座標
	clearPercentTextSize_ = { 284.0f, 56.0f }; // 大きさ
	clearPercentTextSprite_.reset(Sprite::Create(
		textureHandleTextClearPercent_,
			&clearPercentTextPosition_,
			&clearPercentTextSize_,
			&spriteUIColor_, 
			{ 0.5f, 0.5f })); // 生成

	// ステージ内の全アイテム個数カウンター
	stageItemCounter_.reset(new Counter());
	stageItemCounter_->Initialize(textureHandleNumberSheets_, {512.0f, 512.0f}, &stageItemCount_, { 245.0f, 620.0f }, { 64.0f, 64.0f }, -8.0f);
	stageItemCounter_->SetIsCentered(true);
	// ステージ内の現在のアイテム個数カウンター
	stageNowItemCounter_.reset(new Counter());
	stageNowItemCounter_->Initialize(textureHandleNumberSheets_, { 512.0f, 512.0f }, &stageNowItemCount_, { 425.0f, 620.0f }, { 64.0f, 64.0f }, -8.0f);
	stageNowItemCounter_->SetIsCentered(true);
	// ステージ内の現在のアイテム個数カウンター
	stageClearCounter_.reset(new Counter());
	stageClearCounter_->Initialize(textureHandleNumberSheets_, { 512.0f, 512.0f }, &stageClearPercent_, { 385.0f, 140.0f }, { 48.0f, 48.0f }, -8.0f);
	stageClearCounter_->SetIsCentered(true);
	stageClearCounter_->SetIsDispayPercent(true);

}

void GameManagerObject::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// カメラ演出を行う
	CameraStaging();

	WorldTransform mainGearTransform = *mainGearTransform_;
	// ギアの回転を大元ギアと合わせる
	gearTransforms_[0].rotate_.z = -(mainGearTransform.rotate_.z * (12.0f / 8.0f)) + 0.25f;
	gearTransforms_[1].rotate_.z = -(mainGearTransform.rotate_.z * (12.0f / 6.0f)) + 0.15f;
	gearTransforms_[2].rotate_.z = -(mainGearTransform.rotate_.z * (12.0f / 8.0f)) + 0.125f;

	// UIの更新
	stageItemCounter_->Update();
	stageItemCounter_->color_ = spriteUIColor_;
	stageNowItemCounter_->Update();
	stageNowItemCounter_->color_ = spriteUIColor_;
	stageClearCounter_->Update();
	stageClearCounter_->color_ = spriteUIColor_;

	// クリアゲージの更新
	ClearGageAnimation();

#ifdef _DEBUG

	// Imgui
	ImGui::Begin(objectName_.c_str());
	for (int i = 0; i < 3; i++) {
		std::string name = "previewTransform" + std::to_string(i);
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat3("scale", &gearTransforms_[i].scale_.x, 0.5f);
			ImGui::DragFloat3("rotatate", &gearTransforms_[i].rotate_.x, 0.05f);
			ImGui::DragFloat3("translate", &gearTransforms_[i].translate_.x, 0.5f);
			ImGui::TreePop();
		}
	}

	if (ImGui::TreeNode("ClearGage")) {
		ImGui::DragFloat3("scale", &clearGageTransform_.scale_.x, 0.05f);
		ImGui::DragFloat3("rotatate", &clearGageTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &clearGageTransform_.translate_.x, 0.05f);
		ImGui::DragFloat3("UVscale", &objects_[3]->uvTransform_.scale_.x, 0.05f);
		ImGui::DragFloat3("UVrotatate", &objects_[3]->uvTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("UVtranslate", &objects_[3]->uvTransform_.translate_.x, 0.05f);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("UI")) {
		ImGui::DragFloat2("leftItemTextPosition", &leftItemTextPosition_.x, 0.5f);
		ImGui::DragFloat2("leftItemTextSize", &leftItemTextSize_.x, 0.5f);
		ImGui::DragFloat2("slashPosition_", &slashPosition_.x, 0.5f);
		ImGui::DragFloat2("slashSize_", &slashSize_.x, 0.5f);
		ImGui::DragFloat2("clearPercentTextPosition_", &clearPercentTextPosition_.x, 0.5f);
		ImGui::DragFloat2("clearPercentTextSize_", &clearPercentTextSize_.x, 0.5f);
		ImGui::DragFloat2("stageItemCounterPosition", &stageItemCounter_->position_.x, 0.5f);
		ImGui::DragFloat2("stageItemCounterSize", &stageItemCounter_->size_.x, 0.5f);
		ImGui::DragInt("stageItemCount_", &stageItemCount_, 0.5f);
		ImGui::DragFloat2("stageNowItemCounterPosition", &stageNowItemCounter_->position_.x, 0.5f);
		ImGui::DragFloat2("stageNowItemCounterSize", &stageNowItemCounter_->size_.x, 0.5f);
		ImGui::DragInt("stageNowItemCount_", &stageNowItemCount_, 0.5f);
		ImGui::DragFloat2("stageClearCounterPosition", &stageClearCounter_->position_.x, 0.5f);
		ImGui::DragFloat2("stageClearCounterSize", &stageClearCounter_->size_.x, 0.5f);
		ImGui::SliderInt("stageClearPercent_", &stageClearPercent_, 0, 100);

		ImGui::TreePop();
	}

	ImGui::End();

#endif // _DEBUG

}

void GameManagerObject::Draw()
{
	// 全てのobj描画
	DrawAllOBJ();
}

void GameManagerObject::SpriteDraw()
{
	// UI描画
	leftItemTextSprite_->Draw(); // 残りアイテムテキスト
	clearPercentTextSprite_->Draw(); // クリア進捗テキスト
	slashSprite_->Draw(); // / スプライト
	stageItemCounter_->Draw(); // ステージ内の全アイテム個数カウンター
	stageNowItemCounter_->Draw(); // ステージ内の現在アイテム個数カウンター
	stageClearCounter_->Draw(); // ステージのクリア進捗カウンター
}

void GameManagerObject::AddGlobalVariables()
{

}

void GameManagerObject::ApplyGlobalVariables()
{
}

void GameManagerObject::CameraStaging()
{
	switch (cameraStagingWayPoint_)
	{
	case GameManagerObject::WayPoint1: // フェードイン指示

		// フェードイン
		SceneManager::GetInstance()->StartFadeEffect(cameraStagingTime_, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
		// 次の演出へ
		cameraStagingWayPoint_++;
		break;
	case GameManagerObject::WayPoint2: // カメラ移動
		if (cameraStagingT_ <= cameraStagingTime_) {
			// カメラ座標設定
			camera_->transform_.translate_ = Math::EaseOut(cameraStagingT_, cameraStartTranslate_, cameraEndTranslate_, cameraStagingTime_);

			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;
			// 演出時間設定
			cameraStagingTime_ = 1.0f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint3: // UI透明度設定
		if (cameraStagingT_ <= cameraStagingTime_) {
			// スプライト透明度設定
			spriteUIColor_.w = Math::EaseInOut(cameraStagingT_, 0.0f, 1.0f, cameraStagingTime_);

			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint4:
		if (stageClearPercent_ >= 100) {
			// カメラの始端座標と終端座標
			cameraStartTranslate_ = camera_->transform_.translate_;
			cameraEndTranslate_ = { 0.0f, 0.0f, -90.0f };
			// カメラ演出時間設定
			cameraStagingTime_ = 1.5f;
			// 次の演出へ
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint5:
		if (cameraStagingT_ <= cameraStagingTime_) {
			// スプライト透明度設定
			if (cameraStagingT_ <= 0.5f)
				spriteUIColor_.w = Math::EaseOut(cameraStagingT_, 1.0f, 0.0f, 0.5f);
			else
				spriteUIColor_.w = 0.0f;

			// カメラ座標設定
			camera_->transform_.translate_ = Math::EaseOut(cameraStagingT_, cameraStartTranslate_, cameraEndTranslate_, cameraStagingTime_);
			// FOV設定
			camera_->fov_ = Math::EaseInOut(cameraStagingT_, 0.45f, 0.35f, cameraStagingTime_);

			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;

			// カメラの終端座標
			camera_->transform_.translate_ = cameraEndTranslate_;

			// カメラの演出時間設定
			cameraStagingTime_ = 1.15f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint6:
		if (cameraStagingT_ <= cameraStagingTime_) {

			clearCheckTransform_.translate_ = Math::EaseInOut(cameraStagingT_, { 0.0f, -50.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, cameraStagingTime_);
			clearCheckTransform_.scale_ = Math::EaseIn(cameraStagingT_, { 0.0f, 0.0f, 1.0f }, clearGageTransform_.scale_, cameraStagingTime_);

			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;

			clearCheckTransform_.translate_ = { 0.0f, 0.0f, 0.0f };
			clearCheckTransform_.scale_ = clearGageTransform_.scale_;

			// カメラの演出時間設定
			cameraStagingTime_ = 1.0f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint7:
		
		if (cameraStagingT_ <= cameraStagingTime_) {
			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;

			// カメラの演出時間設定
			cameraStagingTime_ = 1.0f;

			// フェードイン
			SceneManager::GetInstance()->StartFadeEffect(cameraStagingTime_, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });

			cameraStartTranslate_ = camera_->transform_.translate_;
			cameraEndTranslate_ = cameraEndTranslate_ = { 0.0f, 0.0f, -30.0f };

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint8:
		if (cameraStagingT_ <= cameraStagingTime_) {
			// カメラ座標設定
			camera_->transform_.translate_ = Math::EaseInOut(cameraStagingT_, cameraStartTranslate_, cameraEndTranslate_, cameraStagingTime_);
			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;
			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint9:
		// ステージセレクトシーンへ遷移
		isGoStageSelectScene_ = true;
		break;

	}
}

void GameManagerObject::ClearGageAnimation()
{
	// ゲージを動かす
	objects_[3]->uvTransform_.rotate_.x = Math::EaseIn((float)stageClearPercent_, -1.05f, -0.3f, 100);
}
