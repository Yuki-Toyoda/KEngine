#include "GameManagerObject.h"
#include "../../Scene/SceneManager.h"
#include "../../Stage/StageManager.h"

void GameManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// ステージマネージャのインスタンス取得
	stageManager_ = StageManager::GetInstance();

	// 入力インスタンス取得
	input_ = Input::GetInstance();
	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();

	// 音量取得
	bgmVolume_ = &SceneManager::GetInstance()->bgmVolume_;
	seVolume_ = &SceneManager::GetInstance()->seVolume_;

	// 音をロード
	bgmHandle_ = audio_->LoadWave("/Audio/BGM/GameBGM.wav");
	soundHandleBack_ = audio_->LoadWave("/Audio/SE/BackSound.wav");
	soundHandleRetry_ = audio_->LoadWave("/Audio/SE/RetrySound.wav"); // リトライ時の効果音
	soundHandleClear_ = audio_->LoadWave("/Audio/SE/Clear.wav");
	soundHandlePerfectClear_ = audio_->LoadWave("/Audio/SE/PerfectClear.wav");

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
	clearGageTransform_.translate_ = { 0.0f, 0.0f, 1.0f };
	clearGageTransform_.scale_ = {14.5f, 14.5f, 1.0f};
	// クリア時のチェックマークワールド座標
	clearCheckTransform_.Initialize();
	clearCheckTransform_.translate_ = { 0.0f, -25.0f, 0.0f };
	clearCheckTransform_.scale_ = clearGageTransform_.scale_;

	// チュートリアル平面のワールド座標
	tutorialTransform_.Initialize();
	tutorialTransform_.translate_ = { 0.0f, 2.5f, 0.5f };
	tutorialTransform_.scale_ = { 3.5f, 3.5f, 1.0f };

	// チュートリアル切り替え
	if (SceneManager::GetInstance()->GetSelectedStageNumber() != 0)
		isTutorial_ = false;
	else
		isTutorial_ = true;
	// t初期化
	tutorialStagingT_ = 0.0f;
	// 描画時間設定
	tutorialStagingDrawTime_ = 1.0f;
	// 前フレーム初期化
	preDrawFrame_ = 0;

	// モデル読み込み
	AddOBJ(&gearTransforms_[0], color_, "./Resources/Gear", "Gear_UI.obj", true);
	AddOBJ(&gearTransforms_[1], color_, "./Resources/Gear", "Gear_UI_S.obj", true);
	AddOBJ(&gearTransforms_[2], color_, "./Resources/Gear", "Gear_M.obj", true);
	AddOBJ(&clearGageTransform_, color_, "./Resources/ClearGage", "ClearGage.obj", false);
	AddOBJ(&clearCheckTransform_, color_, "./Resources/Plane", "Plane.obj", false);
	// チュートリアルが有効なら
	if (isTutorial_) {
		AddOBJ(&tutorialTransform_, color_, "./Resources/TutorialPlane", "TutorialPlane.obj", false);
		AddOBJ(&tutorialTransform_, color_, "./Resources/TutorialPlane", "TutorialFrame.obj", false);
		objects_[6]->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	}
		
	objects_[3]->uvTransform_.rotate_.x = -1.05f;

	objects_[4]->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });

	// 演出中
	cameraIsStaging_ = true;
	// 始端、終端座標設定
	cameraStartTranslate_ = camera_->transform_.translate_;
	cameraEndTranslate_ = { -11.0f, 0.0f, -66.0f };
	// 演出中間地点初期化
	cameraStagingWayPoint_ = WayPoint1;
	// 演出用t初期化
	cameraStagingT_ = 0.0f;
	// 演出時間初期化
	cameraStagingTime_ = 1.25f;

	// ステージセレクトへ戻る演出中間地点
	backStageSelectStagingWayPoint_ = WayPoint1;
	backStageSelectStaging_ = false;
	isRetry_ = false;

	// ステージ内のアイテム個数取得
	stageItemCount_ = stageManager_->GetAllItem(); // アイテム総数取得
	stageNowItemCount_ = stageManager_->GetAllItem(); // 現在のアイテム個数

	// ステージクリア進捗(仮置き)
	stageClearPercent_ = 0;

	// ステージセレクトシーンへのトリガー
	isGoStageSelectScene_ = false;
	// リトライトリガー
	isRetryThisScene_ = false;

	// 完全クリアトリガーリセット
	isPerfectClear_ = false;

	/// テクスチャ読み込み
	textureHandleNumberSheets_ = TextureManager::Load("./Resources/Image", "MyNumberSheets.png"); // 番号シート
	textureHandleTextLeftItem_ = TextureManager::Load("./Resources/Image/Game", "LeftItem.png"); // 残りアイテムテキスト
	textureHandleSlash_ = TextureManager::Load("./Resources/Image/Game", "Slash.png"); // /テクスチャ
	textureHandleTextClearPercent_ = TextureManager::Load("./Resources/Image/Game", "ClearPercent.png"); // クリア進捗テキスト
	textureHandleEsc_N_ = TextureManager::Load("./Resources/Image/Game", "Esc_N.png"); // /テクスチャ
	textureHandleR_N_ = TextureManager::Load("./Resources/Image/Game", "R_N.png"); // /テクスチャ
	textureHandleStageSelect_ = TextureManager::Load("./Resources/Image/Game", "StageSelect.png"); // rUI_押してない時
	textureHandleRestart_ = TextureManager::Load("./Resources/Image/Game", "Restart.png"); // rUI_押してない時

	// UI全体の色リセット
	spriteUIColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	spriteClearUIColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };

	/// スプライト生成
	// 残りアイテム数テキストUI
	leftItemTextPosition_ = { 255.0f, 550.0f }; // 座標
	leftItemTextSize_ = { 320.0f, 64.0f }; // 大きさ
	leftItemTextSprite_.reset(Sprite::Create(
			textureHandleTextLeftItem_, 
			&leftItemTextPosition_, 
			&leftItemTextSize_, 
			&spriteClearUIColor_,
			{ 0.5f, 0.5f })); // 生成
	// 残りアイテム数テキストUI
	slashPosition_ = { 255.0f, 652.5f }; // 座標
	slashSize_ = { 64.0f, 64.0f }; // 大きさ
	slashSprite_.reset(Sprite::Create(
		textureHandleSlash_,
			&slashPosition_,
			&slashSize_,
			&spriteClearUIColor_,
			{ 0.5f, 0.5f })); // 生成
	// クリア進捗テキストUI
	clearPercentTextPosition_ = { 330.0f, 65.0f }; // 座標
	clearPercentTextSize_ = { 284.0f, 56.0f }; // 大きさ
	clearPercentTextSprite_.reset(Sprite::Create(
		textureHandleTextClearPercent_,
			&clearPercentTextPosition_,
			&clearPercentTextSize_,
			&spriteUIColor_, 
			{ 0.5f, 0.5f })); // 生成

	// ステージ内の全アイテム個数カウンター
	stageItemCounter_.reset(new Counter());
	stageItemCounter_->Initialize(textureHandleNumberSheets_, {512.0f, 512.0f}, &stageItemCount_, { 188.0f, 650.0f }, { 64.0f, 64.0f }, -8.0f);
	stageItemCounter_->SetIsCentered(true);
	// ステージ内の現在のアイテム個数カウンター
	stageNowItemCounter_.reset(new Counter());
	stageNowItemCounter_->Initialize(textureHandleNumberSheets_, { 512.0f, 512.0f }, &stageNowItemCount_, { 383.0f, 650.0f }, { 64.0f, 64.0f }, -8.0f);
	stageNowItemCounter_->SetIsCentered(true);
	// ステージ内の現在のアイテム個数カウンター
	stageClearCounter_.reset(new Counter());
	stageClearCounter_->Initialize(textureHandleNumberSheets_, { 512.0f, 512.0f }, &stageClearPercent_, { 350.0f, 130.0f }, { 48.0f, 48.0f }, -8.0f);
	stageClearCounter_->SetIsCentered(true);
	stageClearCounter_->SetIsDispayPercent(true);

	// EscUI
	escUIPosition_ = { 48.0f, 64.0f }; // 座標
	escUISize_ = { 64.0f, 64.0f }; // 大きさ
	escUISprite_.reset(Sprite::Create(
		textureHandleEsc_N_,
		&escUIPosition_,
		&escUISize_,
		&spriteUIColor_,
		{ 0.5f, 0.5f })); // 生成
	stageSelectUIPosition_ = { 144.0f, 64.0f };
	stageSelectUISprite_.reset(Sprite::Create(
		textureHandleStageSelect_,
		&stageSelectUIPosition_,
		&escUISize_,
		&spriteUIColor_,
		{ 0.5f, 0.5f })); // 生成

	// RUI
	rUIPosition_ = { 48.0f, 160.0f }; // 座標
	rUISize_ = { 64.0f, 64.0f }; // 大きさ
	rUISprite_.reset(Sprite::Create(
		textureHandleR_N_,
		&rUIPosition_,
		&rUISize_,
		&spriteUIColor_,
		{ 0.5f, 0.5f })); // 生成
	restartUIPosition_ = { 144.0f, 160.0f }; // 座標
	restartUISprite_.reset(Sprite::Create(
		textureHandleRestart_,
		&restartUIPosition_,
		&rUISize_,
		&spriteUIColor_,
		{ 0.5f, 0.5f })); // 生成

}

void GameManagerObject::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 再生されていなければ再生する
	if (!audio_->IsPlaying(voiceHandleBGM_) || voiceHandleBGM_ == -1) {
		voiceHandleBGM_ = audio_->PlayWave(bgmHandle_, false, *bgmVolume_ * 0.2f);
	}
	audio_->SetVolume(voiceHandleBGM_, *bgmVolume_ * 0.2f);

	// カメラ演出を行う
	if(!backStageSelectStaging_)
		CameraStaging();

	WorldTransform mainGearTransform = *mainGearTransform_;
	// ギアの回転を大元ギアと合わせる
	gearTransforms_[0].rotate_.z = -(mainGearTransform.rotate_.z * (12.0f / 8.0f)) + 0.25f;
	gearTransforms_[1].rotate_.z = -(mainGearTransform.rotate_.z * (12.0f / 6.0f)) + 0.15f;
	gearTransforms_[2].rotate_.z = -(mainGearTransform.rotate_.z * (12.0f / 8.0f)) + 0.125f;

	// UIの更新
	stageItemCounter_->Update();
	stageItemCounter_->color_ = spriteClearUIColor_;
	stageNowItemCount_ = (int)stageManager_->GetAllItem() - (int)stageManager_->GetUsedItem(); // 現在のアイテム個数
	stageNowItemCounter_->Update();
	stageNowItemCounter_->color_.w = spriteClearUIColor_.w;
	stageClearCounter_->Update();
	stageClearCounter_->color_ = spriteUIColor_;

	// クリアゲージの更新
	if (!cameraIsStaging_) {
		ClearGageAnimation();

		if (!backStageSelectStaging_ && !isRetry_) {
			if (input_->TriggerKey(DIK_ESCAPE)) {
				audio_->PlayWave(soundHandleBack_, false, *seVolume_ * 0.3f);
				backStageSelectStagingWayPoint_++;
				backStageSelectStaging_ = true;
				cameraStagingTime_ = 1.0f;
				cameraStagingT_ = 0.0f;
				// フェードアウト
				SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 0.0f });
			}

			if (input_->TriggerKey(DIK_R)) {
				audio_->PlayWave(soundHandleRetry_, false, *seVolume_ * 0.3f);
				backStageSelectStagingWayPoint_++;
				backStageSelectStaging_ = true;
				cameraStagingTime_ = 1.0f;
				cameraStagingT_ = 0.0f;
				isRetry_ = true;
				// フェードアウト
				SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 0.0f });
			}
		}

		switch (backStageSelectStagingWayPoint_)
		{
		case GameManagerObject::WayPoint1:

			break;
		case GameManagerObject::WayPoint2:
			// フェードアウト
			SceneManager::GetInstance()->StartFadeEffect(cameraStagingTime_, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
			backStageSelectStagingWayPoint_++;
			break;
		case GameManagerObject::WayPoint3:
			if (cameraStagingT_ <= cameraStagingTime_)
				cameraStagingT_ += 1.0f / 60.0f;
			else {
				if (!isRetry_)
					isGoStageSelectScene_ = true;
				else
					isRetryThisScene_ = true;
				// BGMを止める
				audio_->StopWave(voiceHandleBGM_);
			}
			break;
		}
	}

	// チュートリアル演出
	if(isTutorial_)
		TutorialStaging();

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
	escUISprite_->Draw();
	stageSelectUISprite_->Draw();
	rUISprite_->Draw();
	restartUISprite_->Draw();
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
		// ステージマネージャーに演出中ということを伝える
		stageManager_->SetIsStaging(true);
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

			// カメラ演出中に
			cameraIsStaging_ = false;

			// ステージマネージャーに演出終了を伝える
			stageManager_->SetIsStaging(false);

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint3: // UI透明度設定
		if (cameraStagingT_ <= cameraStagingTime_) {
			// スプライト透明度設定
			spriteUIColor_.w = Math::EaseInOut(cameraStagingT_, 0.0f, 1.0f, cameraStagingTime_);
			spriteClearUIColor_.w = Math::EaseInOut(cameraStagingT_, 0.0f, 1.0f, cameraStagingTime_);

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

			// ステージマネージャーに演出中だと伝える
			stageManager_->SetIsStaging(true);

			// ステージ上の全アイテムを取得したら完全クリア
			if (stageNowItemCount_ <= 0) {
				objects_[4]->SetColor({ 0.0f, 0.35f, 0.05f, 0.0f });
				isPerfectClear_ = true;
			}

			// カメラ演出中に
			cameraIsStaging_ = true;

			// 次の演出へ
			cameraStagingWayPoint_++;
		}
		break;
	case GameManagerObject::WayPoint5:
		if (cameraStagingT_ <= cameraStagingTime_) {
			// スプライト透明度設定
			if (cameraStagingT_ <= 0.5f) {
				spriteUIColor_.w = Math::EaseOut(cameraStagingT_, 1.0f, 0.0f, 0.5f);
				spriteClearUIColor_.w = Math::EaseOut(cameraStagingT_, 1.0f, 0.0f, 0.5f);
			}
			else {
				spriteUIColor_.w = 0.0f;
				spriteClearUIColor_.w = 0.0f;
			}

			if (isTutorial_) {
				objects_[5]->SetColor({ objects_[5]->GetColor().x,objects_[5]->GetColor().y,objects_[5]->GetColor().z, Math::EaseInOut(cameraStagingT_, 1.0f, 0.0f, cameraStagingTime_) });
				objects_[6]->SetColor({ objects_[6]->GetColor().x,objects_[6]->GetColor().y,objects_[6]->GetColor().z, Math::EaseInOut(cameraStagingT_, 1.0f, 0.0f, cameraStagingTime_) });
			}
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

			// ステージ上の全アイテムを取得したら完全クリア
			if (isPerfectClear_) {
				audio_->PlayWave(soundHandlePerfectClear_, false, *seVolume_ * 0.3f);
			}
			else {
				audio_->PlayWave(soundHandleClear_, false, *seVolume_ * 0.3f);
				objects_[4]->SetTextureHandle(TextureManager::Load("./Resources/Plane", "ClearGage_A.png"));
				leftItemTextSprite_->SetTextureHandle(TextureManager::Load("./Resources/Image/Game", "LeftClearItem.png"));
				leftItemTextPosition_ = { 640.0f, 280.0f }; // 座標
				leftItemTextSize_ = { 426.0f,85.0f };
				slashPosition_ = { 640.0f, 402.5f }; // 座標
				slashSize_ = { 85.0f,85.0f };
				stageItemCounter_->position_ = { 583.5f, 402.5f };
				stageItemCounter_->size_ = { 85.0f, 85.0f };
				stageNowItemCounter_->position_ = { 778.5f, 402.5f };
				stageNowItemCounter_->size_ = { 85.0f, 85.0f };
				stageNowItemCounter_->color_ = { 1.0f, 0.1f, 0.0f, 0.0f };
			}
				
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
			clearCheckTransform_.translate_ = Math::EaseInOut(cameraStagingT_, { 0.0f, -5.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, cameraStagingTime_);
			objects_[4]->SetColor({
				objects_[4]->GetColor().x,
				objects_[4]->GetColor().y,
				objects_[4]->GetColor().z,
				Math::EaseInOut(cameraStagingT_, 0.0f, 1.0f, cameraStagingTime_) });

			if(!isPerfectClear_) {
				objects_[3]->SetColor({
				Math::EaseInOut(cameraStagingT_, 1.0f, 0.65f, cameraStagingTime_),
				Math::EaseInOut(cameraStagingT_, 1.0f, 0.65f, cameraStagingTime_),
				Math::EaseInOut(cameraStagingT_, 1.0f, 0.65f, cameraStagingTime_),
				objects_[3]->GetColor().w });
				leftItemTextPosition_.y = Math::EaseInOut(cameraStagingT_, 500.0f, 280.0f, cameraStagingTime_);
				slashPosition_.y = Math::EaseInOut(cameraStagingT_, 622.5f, 402.5f, cameraStagingTime_);
				stageItemCounter_->position_.y = Math::EaseInOut(cameraStagingT_, 622.5f, 402.5f, cameraStagingTime_);
				stageNowItemCounter_->position_.y = Math::EaseInOut(cameraStagingT_, 622.5f, 402.5f, cameraStagingTime_);
				spriteClearUIColor_.w = Math::EaseInOut(cameraStagingT_, 0.0f, 1.0f, cameraStagingTime_);
			}

			// tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			// tリセット
			cameraStagingT_ = 0.0f;

			clearCheckTransform_.translate_ = { 0.0f, 0.0f, 0.0f };

			// カメラの演出時間設定
			if(isPerfectClear_)
				cameraStagingTime_ = 1.0f;
			else
				cameraStagingTime_ = 2.0f;

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
			if (!isPerfectClear_) {
				if (cameraStagingT_ <= 0.35f)
					spriteClearUIColor_.w = Math::EaseInOut(cameraStagingT_, 1.0f, 0.0f, 0.35f);
				else
					spriteClearUIColor_.w = 0.0f;

			}
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

		// ステージマネージャーに演出終了を伝える
		stageManager_->SetIsStaging(false);

		// BGMを止める
		audio_->StopWave(voiceHandleBGM_);
		// ステージセレクトシーンへ遷移
		if(isPerfectClear_)
			isGoStageSelectScene_ = true;
		else
			isRetryThisScene_ = true;
		break;

	}
}

void GameManagerObject::ClearGageAnimation()
{
	if (stageManager_->GetGearCondition() <= stageManager_->GetClearCondition())
		stageClearPercent_ = Math::Linear(stageManager_->GetGearCondition(), 0, 100, stageManager_->GetClearCondition());
	else
		stageClearPercent_ = 100;

	// ゲージを動かす
	objects_[3]->uvTransform_.rotate_.x = Math::EaseIn((float)stageClearPercent_, -1.05f, -0.3f, 100);
}

void GameManagerObject::TutorialStaging()
{
	// チュートリアル画像を切り替える
	if (tutorialStagingT_ <= tutorialStagingDrawTime_ * 5.0f) {
		// 描画フレームを求める
		int drawFrame = Math::Linear(tutorialStagingT_, 0, 5, tutorialStagingDrawTime_ * 5.0f);
		
		if (drawFrame != preDrawFrame_) {
			// テクスチャ名を求める
			std::string textureName = "Tutorial_0" + std::to_string(drawFrame) + ".png";
			// テクスチャ読み込み、変更
			objects_[5]->SetTextureHandle(TextureManager::Load("./Resources/TutorialPlane", textureName));
		}

		// t加算
		tutorialStagingT_ += 1.0f / 60.0f;
	}
	else
		tutorialStagingT_ = 0.0f;
}
