#include "TitleManagerObject.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"

void TitleManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// 見た目を合わるために初期座標を設定
	transform_.translate_ = { 1.25f, 0.0f, 0.0f };

	// 入力取得
	input_ = Input::GetInstance();
	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();

	// 音量取得
	bgmVolume_ = &SceneManager::GetInstance()->bgmVolume_;
	seVolume_ = &SceneManager::GetInstance()->seVolume_;

	// 音をロード
	ambientHandle_ = audio_->LoadWave("/Audio/Ambient/GearAmbient.wav");
	bgmHandle_ = audio_->LoadWave("/Audio/BGM/TitleBGM.wav");

	// カメラがセットされていれば
	if (camera_ != nullptr) {
		camera_->transform_.translate_ = { 0.0f, 10.0f, -50.0f };
		camera_->transform_.rotate_ = { (float)std::numbers::pi / 16.0f, 0.0f , 0.0f };
	}

	// オブジェクト座標初期化
	ｍainGearTransform_.Initialize();
	ｍainGearTransform_.SetParent(&transform_);
	mGearTransform_.Initialize();
	mGearTransform_.translate_ = { -5.0f, 0.0f };
	mGearTransform_.SetParent(&transform_, 0b001);
	mGearTransform2_.Initialize();
	mGearTransform2_.translate_ = { 4.05f, -2.95f };
	mGearTransform2_.SetParent(&transform_, 0b001);
	sGearTransform_.Initialize();
	sGearTransform_.translate_ = { -7.2f, -2.8f };
	sGearTransform_.SetParent(&transform_, 0b001);
	titleGearTransform_.Initialize();
	titleGearTransform_.translate_ = { 4.1f, 1.7f, 0.0f};
	titleGearTransform_.SetParent(&transform_, 0b001);

	// モデル読み込み
	AddOBJ(&ｍainGearTransform_, color_, "./Resources/Gear", "Gear_L.obj", false);
	AddOBJ(&mGearTransform_, color_, "./Resources/Gear", "Gear_M.obj", false);
	AddOBJ(&mGearTransform2_, color_, "./Resources/Gear", "Gear_M.obj", false);
	AddOBJ(&sGearTransform_, color_, "./Resources/Gear", "Gear_S.obj", false);
	AddOBJ(&titleGearTransform_, color_, "./Resources/TitleGear", "TitleGear.obj", false);

	// ロゴ用変数初期化
	logoPosition_ = { 640.0f, 290.0f };
	logoSize_ = { 1280.0f, 170.0f };
	logoColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	logoAnchorPoint_ = { 0.5f, 0.5f };

	// テクスチャ読み込み
	textureHandleTitleLogo_ = TextureManager::Load("./Resources/Image/Title", "titleLogo.png");
	// スプライト読み込み
	titleLogo_.reset(Sprite::Create(textureHandleTitleLogo_, &logoPosition_, &logoSize_, &logoColor_, logoAnchorPoint_));

	// ボタン用変数初期化
	buttonIsActive_ = false;
	buttonPosition_ = { 640.0f, 600.0f };
	buttonSize_ = { 640.0f, 85.0f };
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	buttonAnchorPoint_ = { 0.5f, 0.5f };

	// テクスチャ読み込み
	textureHandleTitleButton_ = TextureManager::Load("./Resources/Image/Title", "titleButton.png");
	// スプライト読み込み
	titleButton_.reset(Sprite::Create(textureHandleTitleButton_, &buttonPosition_, &buttonSize_, &buttonColor_, buttonAnchorPoint_));

	// 演出中間地点リセット
	cameraStagingWayPoint_ = 0;
	// 演出t
	rotateStagingT_ = 0.0f;

	// イージング座標リセット
	cameraStartTranslate_ = { 0.0f, 10.0f, -50.0f };
	cameraEndTranslate_ = { 0.0f, 0.25f, -16.0f };
	// カメラ手振れ演出無効
	enableCameraShake_ = false;
	cameraStagingT_ = (3.0f / 2.0f) / 2.0f;
	cameraStagingT2_ = (3.0f / 2.0f);
	// カメラ演出用tのループトリガー
	cameraStagingTReturn_ = false;
	// カメラ演出用tのループトリガー
	cameraStagingT2Return_ = false;
	// 演出時間リセット
	rotateStagingTime_ = 2.5f;

	// タイトル演出スキップトリガーリセット
	skipTitleStaging_ = false;

	// ゲームシーンへのトリガーリセット
	isGoStageSelectScene_ = false;

	// フェードイン
	SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });

	AddGlobalVariables();

}

void TitleManagerObject::Update()
{

	// 基底クラス更新
	BaseObject::Update();

	// 再生されていなければ再生する
	if (!audio_->IsPlaying(voiceHandleAmbient_) || voiceHandleAmbient_ == -1) {
		voiceHandleAmbient_ = audio_->PlayWave(ambientHandle_, false, *bgmVolume_);
	}
	audio_->SetVolume(voiceHandleAmbient_, *bgmVolume_ * 0.05f);
	// 再生されていなければ再生する
	if (!audio_->IsPlaying(voiceHandleBGM_) || voiceHandleBGM_ == -1) {
		voiceHandleBGM_ = audio_->PlayWave(bgmHandle_, false, *bgmVolume_);
	}
	audio_->SetVolume(voiceHandleBGM_, *bgmVolume_);

	// ギアの回転
	ｍainGearTransform_.rotate_.z += (0.02f * (6.0f / 8.0f)) * (8.0f / 12.0f);
	mGearTransform_.rotate_.z -= 0.02f * (6.0f / 8.0f);
	mGearTransform2_.rotate_.z -= 0.02f * (6.0f / 8.0f);
	sGearTransform_.rotate_.z += 0.02f;
	titleGearTransform_.rotate_.z -= 0.02f;

	// カメラ演出
	switch (cameraStagingWayPoint_)
	{
	case TitleManagerObject::WayPoint1: // カメラをタイトルロゴの位置まで移動させる
		if (rotateStagingT_ <= rotateStagingTime_) { 
			// カメラをイージングで移動させる
			camera_->transform_.translate_ = Math::EaseOut(rotateStagingT_, cameraStartTranslate_, cameraEndTranslate_, rotateStagingTime_);
			camera_->transform_.rotate_ = Math::EaseInOut(rotateStagingT_, { (float)std::numbers::pi / 16.0f, 0.0f , 0.0f }, { 0.0f, 0.0f, 0.0f }, rotateStagingTime_);

			// 演出用tを加算
			rotateStagingT_ += 1.0f / 60.0f;
		}
		else {

			// カメラを終端地点まで強制的に移動
			camera_->transform_.translate_ = cameraEndTranslate_;
			camera_->transform_.rotate_ = { 0.0f, 0.0f, 0.0f };

			cameraStartTranslate_ = cameraEndTranslate_;
			cameraEndTranslate_ = { 0.0f, 0.25f, -14.0f };

			// カメラ手振れ演出有効
			enableCameraShake_ = true;

			// 演出用tをリセット
			rotateStagingT_ = 0.0f;
			// 演出時間設定
			rotateStagingTime_ = 1.5f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case TitleManagerObject::WayPoint2: // タイトルロゴを徐々に表示させる
		if (rotateStagingT_ <= rotateStagingTime_) {
			// タイトルロゴを表示
			logoColor_.w = Math::EaseIn(rotateStagingT_, 0.0f, 1.0f, rotateStagingTime_);

			// 演出用tを加算
			rotateStagingT_ += 1.0f / 60.0f;
		}
		else {

			// タイトルロゴができったため、スキップトリガーをtrueに
			skipTitleStaging_ = true;

			// タイトルボタン表示
			buttonIsActive_ = true;

			// a値を強制的に変更
			logoColor_.w = 1.0f;

			// 演出時間設定
			rotateStagingTime_ = 1.0f;
			// 演出用tをリセット
			rotateStagingT_ = 0.0f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case TitleManagerObject::WayPoint3: // スペースを押すとシーン遷移開始
		if (input_->TriggerKey(DIK_SPACE)) {
			// カメラシェイク無効
			enableCameraShake_ = false;
			// カメラの始端座標を現在のカメラ座標に変更
			cameraStartTranslate_ = camera_->transform_.translate_;
			// カメラの終端座標を設定
			cameraEndTranslate_ = { 1.25f, 0.25f, -18.0f };
			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case TitleManagerObject::WayPoint4:
		if (rotateStagingT_ <= rotateStagingTime_) {
			// カメラをイージングで移動させる
			camera_->transform_.translate_ = Math::EaseOut(rotateStagingT_, cameraStartTranslate_, cameraEndTranslate_, rotateStagingTime_);
			// タイトルロゴを表示
			logoColor_.w = Math::EaseOut(rotateStagingT_, 1.0f, 0.0f, rotateStagingTime_);
			buttonColor_.w = Math::EaseOut(rotateStagingT_, 1.0f, 0.0f, rotateStagingTime_);
			// 演出用tを加算
			rotateStagingT_ += 1.0f / 60.0f;
		}
		else {

			// 終端座標に補正
			camera_->transform_.translate_ = cameraEndTranslate_;

			// 始端座標設定
			cameraStartTranslate_ = cameraEndTranslate_;
			// 終端座標設定
			cameraEndTranslate_ = { 1.25f, 0.25f, -1.0f };

			// 演出時間設定
			rotateStagingTime_ = 1.5f;
			// 演出用tをリセット
			rotateStagingT_ = 0.0f;

			// フェードアウト
			SceneManager::GetInstance()->StartFadeEffect(rotateStagingTime_, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case TitleManagerObject::WayPoint5:
		if (rotateStagingT_ <= rotateStagingTime_) {
			// カメラをイージングで移動させる
			camera_->transform_.translate_ = Math::EaseInOut(rotateStagingT_, cameraStartTranslate_, cameraEndTranslate_, rotateStagingTime_);
			// 演出用tを加算
			rotateStagingT_ += 1.0f / 60.0f;
		}
		else {

			// 終端座標に補正
			camera_->transform_.translate_ = cameraEndTranslate_;

			// 始端座標設定
			cameraStartTranslate_ = cameraEndTranslate_;
			// 終端座標設定
			cameraEndTranslate_ = { 0.0f, 0.25f, -1.0f };

			// 演出時間設定
			rotateStagingTime_ = 1.5f;
			// 演出用tをリセット
			rotateStagingT_ = 0.0f;

			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case TitleManagerObject::WayPoint6:

		// 環境音を止める
		audio_->StopWave(voiceHandleAmbient_);
		// bgm停止
		audio_->StopWave(voiceHandleBGM_);

		// ステージセレクトシーンへ
		isGoStageSelectScene_ = true;
		break;
	}

	// カメラ手振れ演出処理
	if (enableCameraShake_) {
		if (cameraStagingT_ <= 3.0f / 2.0f) {
			// カメラを横に少し動かす
			if (!cameraStagingTReturn_) {
				if(buttonIsActive_)
					buttonColor_.w = Math::EaseOut(cameraStagingT_, 0.01f, 1.0f, 3.0f / 2.0f);
				camera_->transform_.translate_.y = Math::EaseInOut(cameraStagingT_, 0.25f - 0.05f, 0.25f + 0.05f, 3.0f / 2.0f);
			}
			else {
				if(buttonIsActive_)
					buttonColor_.w = Math::EaseOut(cameraStagingT_, 1.0f, 0.01f, 3.0f / 2.0f);
				camera_->transform_.translate_.y = Math::EaseInOut(cameraStagingT_, 0.25f + 0.05f, 0.25f - 0.05f, 3.0f / 2.0f);
			}

			// 演出用tを加算
			cameraStagingT_ += 1.0f / 60.0f;
		}
		else {
			cameraStagingT_ = 0.0f;
			if (!cameraStagingTReturn_)
				cameraStagingTReturn_ = true;
			else
				cameraStagingTReturn_ = false;

		}

		if (cameraStagingT2_ <= 3.0f) {
			// カメラを横に少し動かす
			if (!cameraStagingT2Return_) {
				camera_->transform_.translate_.x = Math::EaseInOut(cameraStagingT2_, -0.05f, 0.05f, 3.0f);
			}
			else {
				camera_->transform_.translate_.x = Math::EaseInOut(cameraStagingT2_, 0.05f, -0.05f, 3.0f);
			}
				

			// 演出用tを加算
			cameraStagingT2_ += 1.0f / 60.0f;
		}
		else {
			cameraStagingT2_ = 0.0f;
			if (!cameraStagingT2Return_)
				cameraStagingT2Return_ = true;
			else
				cameraStagingT2Return_ = false;

		}
	}

	// タイトルロゴ演出スキップ
	if (input_->TriggerKey(DIK_SPACE)) {
		if (!skipTitleStaging_) {
			// カメラを終端地点まで強制的に移動
			camera_->transform_.translate_ = { 0.0f, 0.25f, -16.0f };
			camera_->transform_.rotate_ = { 0.0f, 0.0f, 0.0f };
			// a値を強制的に変更
			logoColor_.w = 1.0f;
			cameraStagingWayPoint_ = WayPoint2;
			rotateStagingTime_ = 0.0f;
			enableCameraShake_ = true;
			skipTitleStaging_ = true;
		}
	}

#ifdef _DEBUG

	// Imgui
	ImGui::Begin(objectName_.c_str());

	if (ImGui::TreeNode("TitleLogo")) {
		ImGui::DragFloat2("SpritePos", &logoPosition_.x, 1.0f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("mainGear")) {
		ImGui::DragFloat3("scale", &ｍainGearTransform_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &ｍainGearTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &ｍainGearTransform_.translate_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("mGear")) {
		ImGui::DragFloat3("scale", &mGearTransform_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &mGearTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &mGearTransform_.translate_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("mGear2")) {
		ImGui::DragFloat3("scale", &mGearTransform2_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &mGearTransform2_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &mGearTransform2_.translate_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("sGear")) {
		ImGui::DragFloat3("scale", &sGearTransform_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &sGearTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &sGearTransform_.translate_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("titleGear")) {
		ImGui::DragFloat3("scale", &titleGearTransform_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &titleGearTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &titleGearTransform_.translate_.x, 0.5f);
		ImGui::TreePop();
	}

	ImGui::End();

#endif // DEBUG

}

void TitleManagerObject::Draw()
{
	// 全てのobjを描画
	DrawAllOBJ();
}

void TitleManagerObject::SpriteDraw()
{
	titleButton_->Draw();
	titleLogo_->Draw();
}

void TitleManagerObject::AddGlobalVariables()
{
}

void TitleManagerObject::ApplyGlobalVariables()
{
}
