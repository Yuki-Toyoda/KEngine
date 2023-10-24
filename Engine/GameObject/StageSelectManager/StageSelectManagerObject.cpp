#include "StageSelectManagerObject.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"
#include "../../Stage/StageManager.h"
#include "../Item/Item.h"
#include "../Catapult/Catapult.h"

void StageSelectManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// 入力取得
	input_ = Input::GetInstance();

	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();

	// 音量取得
	bgmVolume_ = &SceneManager::GetInstance()->bgmVolume_;
	seVolume_ = &SceneManager::GetInstance()->seVolume_;

	// 音をロード
	ambientHandle_ = audio_->LoadWave("/Audio/Ambient/GearAmbient.wav");
	bgmHandle_ = audio_->LoadWave("/Audio/BGM/StageSelectBGM.wav");

	soundHandleRotateGear_ = audio_->LoadWave("/Audio/SE/RotatePreviewGear.wav"); // プレビュー歯車回転音

	// ステージマネージャ取得
	stageManager_ = StageManager::GetInstance();
	// ステージ情報読み込み
	stageManager_->LoadStages();

	// 初期座標を設定
	transform_.translate_ = { 0.0f, -7.25f, 0.0f };
	mainGearTransform_.Initialize();
	mainGearTransform_.SetParent(&transform_, 0b101);

	boardTransform_.Initialize();
	boardTransform_.translate_ = { 0.0f, 0.0f, 5.0f };
	boardTransform_.SetParent(&transform_, 0b101);

	// 全てのステージプレビュー座標を初期化
	for (int i = 0; i < 4; i++) {
		previewStageTransforms_[i].Initialize();
		ornamentGearTransforms_[i].Initialize();
	}

	// ステージプレビュー座標を個々で設定
	previewStageTransforms_[0].translate_ = { 0.0f, 7.25f, 0.0f };
	previewStageTransforms_[0].rotate_ = { 0.0f, 0.0f, 0.125f };
	previewStageTransforms_[0].SetParent(&transform_, 0b011);
	previewStageTransforms_[1].translate_ = { 7.25f, 0.0f, 0.0f };
	previewStageTransforms_[1].rotate_ = { 0.0f, 0.0f, -0.125f };
	previewStageTransforms_[1].SetParent(&transform_, 0b011);
	previewStageTransforms_[2].translate_ = { 0.0f, -7.25f, 0.0f };
	previewStageTransforms_[2].rotate_ = { 0.0f, 0.0f, 0.125f };
	previewStageTransforms_[2].SetParent(&transform_, 0b011);
	previewStageTransforms_[3].translate_ = { -7.25f, -0.0f, 0.0f };
	previewStageTransforms_[3].rotate_ = { 0.0f, -0.0f, -0.125f };
	previewStageTransforms_[3].SetParent(&transform_, 0b011);

	// 装飾ギアの座標を初期化
	ornamentGearTransforms_[0].translate_ = { 5.0f, 4.75f, 1.5f };
	ornamentGearTransforms_[0].scale_ = { 1.35f, 1.35f, 1.0f };
	ornamentGearTransforms_[1].translate_ = { 2.05f, -2.85f, 0.0f };
	ornamentGearTransforms_[1].SetParent(&ornamentGearTransforms_[0], 0b101);
	ornamentGearTransforms_[2].translate_ = { -5.0f, 4.75f, 1.5f };
	ornamentGearTransforms_[2].scale_ = { 1.35f, 1.35f, 1.0f };
	ornamentGearTransforms_[3].translate_ = { -2.0f, -2.85f, 0.0f };
	ornamentGearTransforms_[3].SetParent(&ornamentGearTransforms_[2], 0b101);

	// モデル読み込み
	AddOBJ(&mainGearTransform_, color_, "./Resources/Gear", "Gear_XL.obj", false);
	AddOBJ(&boardTransform_, color_, "./Resources/GearBase", "GearBoard.obj", false);
	AddOBJ(&transform_, color_, "./Resources/GearBase", "GearBase.obj", false);
	for(int i = 0; i < 4; i++)
		AddOBJ(&previewStageTransforms_[i], color_, "./Resources/Gear", "Gear_L.obj", false);
	AddOBJ(&ornamentGearTransforms_[0], color_, "./Resources/Gear", "Gear_M.obj", false);
	AddOBJ(&ornamentGearTransforms_[1], color_, "./Resources/Gear", "Gear_S.obj", false);
	AddOBJ(&ornamentGearTransforms_[2], color_, "./Resources/Gear", "Gear_M.obj", false);
	AddOBJ(&ornamentGearTransforms_[3], color_, "./Resources/Gear", "Gear_S.obj", false);

	// 全ステージ数取得
	stageCount_ = (int)stageManager_->GetStageMaxIndex() - 1;

	// 選択中のステージ番号
	selectedStageNumber_ = SceneManager::GetInstance()->GetSelectedStageNumber();
	// 切り替えボタン押下回数
	pressCount_ = 0;

	// 演出用tリセット
	rotateStagingT_ = 0.0f;
	// 演出時間
	rotateStagingTime_ = 0.5f;
	// 演出トリガーリセット
	isRotateStaging_ = false;

	// カメラ手振れ演出無効
	enableCameraShake_ = true;
	cameraStagingT_ = (3.0f / 2.0f) / 2.0f;
	cameraStagingT2_ = (3.0f / 2.0f);
	// カメラ演出用tのループトリガー
	cameraStagingTReturn_ = false;
	// カメラ演出用tのループトリガー
	cameraStagingT2Return_ = false;

	// 演出中間地点
	cameraStagingWayPoint_ = WayPoint1;

	// 遷移演出中である
	isTransitionStaging_ = true;
	// 遷移演出用t
	transitionStagingT_ = 0.0f;
	// 遷移演出時間
	transitionStagingTime_ = 2.5f;

	// カメラ演出始端座標設定
	cameraStartTranslate_ = camera_->transform_.translate_;
	// カメラ演出終端座標設定
	cameraEndTranslate_ = { 0.0f, 0.0f, -13.5f };

	// ゲームシーンへは遷移させない
	isGoGameScene_ = false;

	for (int i = 0; i < 20; i++) {
		Item* item = new Item(); // インスタンス生成
		item->Initialize("previewItem", BaseObject::tagOther); // 初期化
		item->transform_.translate_ = { 10000.0f, 10000.0f, 0.0f };
		item->transform_.scale_ = { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f };
		item->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // アイテムを非表示
		item->SetIsActive(true);
		previewItems_.push_back(item); // リストに追加
		GameObjectManager::GetInstance()->AddGameObject(item);
	}

	for (int i = 0; i < 5; i++) {
		Catapult* catapult = new Catapult(); // インスタンス生成
		catapult->Initialize("previewCatapult", BaseObject::tagOther); // 初期化
		catapult->SetLength(10000.0f);
		catapult->transform_.scale_ = { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f };
		catapult->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // アイテムを非表示
		catapult->SetIsActive(true);
		previewCatapults_.push_back(catapult); // リストに追加
		GameObjectManager::GetInstance()->AddGameObject(catapult);
	}

	// プレビューアイテム表示演出中か
	previewItemStaging_ = false;
	// プレビューアイテム用の演出t
	previewItemStagingT_ = 0.0f;

	// プレビューアイテムの座標設定
	SetPreviewItems();

	// UI全体の色
	spriteUIColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };

	// UIテクスチャ群
	textureHandleSelectArrow_ = TextureManager::Load("./Resources/Image/StageSelect", "SelectArrow.png"); // 選択矢印
	textureHandleNumberSheets_ = TextureManager::Load("./Resources/Image", "MyNumberSheets.png"); // 番号シート
	textureHandleStageNumberBackGround_ = TextureManager::Load("./Resources/Image/StageSelect", "stageSelectCounterBackGround.png"); // ステージ番号背景

	// ステージ選択矢印UI
	selectArrowPosition_[0] = {340.0f, 600.0f}; // 選択矢印UI座標 左
	selectArrowPosition_[1] = {940.0f, 600.0f }; // 選択矢印UI座標 右
	selectArrowSize_ = {128.0f, 128.0f}; // 選択矢印サイズ
	for (int i = 0; i < 2; i++) {
		selectArrowSprites_[i].reset(Sprite::Create(
			textureHandleSelectArrow_,
			&selectArrowPosition_[i],
			&selectArrowSize_,
			&spriteUIColor_,
			{ 0.5f, 0.5f })); // 生成
		playingSelectArrowAnim_[i] = false; // 再生トリガーリセット
		selectArrowAnimT_[i] = 0.0f; // tリセット
		selectArrowAnimTime_[i] = 0.15f; // 演出時間設定 
		startSelectArrowPosition_[i] = selectArrowPosition_[i]; // 始端座標設定
	}
	// それぞれのスプライトの角度を設定
	selectArrowSprites_[0]->rotation_ = -(float)std::numbers::pi / 2.0f;
	selectArrowSprites_[1]->rotation_ = (float)std::numbers::pi / 2.0f;

	// 終端座標設定
	endSelectArrowPosition_[0] = { selectArrowPosition_[0].x - 25.0f, selectArrowPosition_[0].y};
	endSelectArrowPosition_[1] = { selectArrowPosition_[1].x + 25.0f, selectArrowPosition_[1].y };

	// ステージ番号UI
	stageSelectedNumber_ = 1;
	stageNumber_.reset(new Counter());
	stageNumber_->Initialize(textureHandleNumberSheets_, {512.0f, 512.0f}, &stageSelectedNumber_, {700.0f, 600.0f}, {128.0f, 128.0f}, -16.0f);
	// ステージ番号のカウンターの色を更新
	stageNumber_->color_ = spriteUIColor_;
	stageNumber_->SetIsCentered(true); // 中央ぞろえに
	// ステージ番号カウンタ背景
	stageNumberBackGround_Position_ = stageNumber_->position_;
	stageNumberBackGroundSize_ = { 192.0f, 192.0f };
	stageNumberBackGround_.reset(Sprite::Create(
		textureHandleStageNumberBackGround_,
		&stageNumberBackGround_Position_,
		&stageNumberBackGroundSize_,
		&spriteUIColor_,
		{ 0.775f, 0.5f }
	));

	// フェードイン
	SceneManager::GetInstance()->StartFadeEffect(transitionStagingTime_, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
}

void StageSelectManagerObject::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 再生されていなければ再生する
	if (!audio_->IsPlaying(voiceHandleAmbient_) || voiceHandleAmbient_ == -1) {
		voiceHandleAmbient_ = audio_->PlayWave(ambientHandle_, false, *bgmVolume_);
	}
	audio_->SetVolume(voiceHandleAmbient_, *bgmVolume_ * 0.1f);
	// 再生されていなければ再生する
	if (!audio_->IsPlaying(voiceHandleBGM_) || voiceHandleBGM_ == -1) {
		voiceHandleBGM_ = audio_->PlayWave(bgmHandle_, false, *bgmVolume_ * 0.2f);
	}
	audio_->SetVolume(voiceHandleBGM_, *bgmVolume_ * 0.2f);

	// ステージプレビュー回転
	if (isRotateStaging_)
		RotateStagePreview();

	if (previewItemStaging_)
		PrevItemStaging();

	// ギアの回転を大元ギアと合わせる
	previewStageTransforms_[0].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) + 0.125f;
	previewStageTransforms_[1].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) - 0.125f;
	previewStageTransforms_[2].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) + 0.125f;
	previewStageTransforms_[3].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) - 0.125f;

	// 遷移中でなければ
	if (!isTransitionStaging_) {
		// キーを押すとステージを選択
		if (input_->TriggerKey(DIK_D) || input_->TriggerKey(DIK_RIGHTARROW)) {

			playingSelectArrowAnim_[1] = true;
			selectArrowAnimT_[1] = 0.0f;

			if (pressCount_ < 0)
				pressCount_ = 0;

			if (pressCount_ < 2)
				pressCount_++;
		}
		else if (input_->TriggerKey(DIK_A) || input_->TriggerKey(DIK_LEFTARROW)) {

			playingSelectArrowAnim_[0] = true;
			selectArrowAnimT_[0] = 0.0f;

			if (pressCount_ > 0)
				pressCount_ = 0;

			if (pressCount_ > -2)
				pressCount_--;
		}
		// 回転演出中でなければ
		if (!isRotateStaging_) {

			if (pressCount_ > 0) {
				// 押下回数デクリメント
				pressCount_--;
				RotateStart(true);
				
			}
			else if (pressCount_ < 0) {
				// 押下回数インクリメント
				pressCount_++;
				RotateStart(false);
			}
		}
	}

	// 選択中ステージ番号を検索
	stageSelectedNumber_ = selectedStageNumber_ + 1;

	// 遷移演出
	TransitionStaging();

	// カメラ手振れ演出
	CameraShake();

	// ギアアニメーション
	OrnamentAnimation();

	// UIアニメーション
	if (!isTransitionStaging_)
		UIAnimation();
	stageNumber_->Update();
	// ステージ番号のカウンターの色を更新
	stageNumber_->color_ = spriteUIColor_;

#ifdef _DEBUG

	// Imgui
	ImGui::Begin(objectName_.c_str());
	for (int i = 0; i < 4; i++) {
		std::string name = "previewTransform" + std::to_string(i);	
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat3("scale", &previewStageTransforms_[i].scale_.x, 0.5f);
			ImGui::DragFloat3("rotatate", &previewStageTransforms_[i].rotate_.x, 0.05f);
			ImGui::DragFloat3("translate", &previewStageTransforms_[i].translate_.x, 0.5f);
			ImGui::TreePop();
		}	
		name = "ornamentTransform" + std::to_string(i);
		if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat3("scale", &ornamentGearTransforms_[i].scale_.x, 0.5f);
			ImGui::DragFloat3("rotatate", &ornamentGearTransforms_[i].rotate_.x, 0.05f);
			ImGui::DragFloat3("translate", &ornamentGearTransforms_[i].translate_.x, 0.5f);
			ImGui::TreePop();
		}
	}

	if (ImGui::TreeNode("boardTransform")) {
		ImGui::DragFloat3("scale", &boardTransform_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &boardTransform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &boardTransform_.translate_.x, 0.5f);
		ImGui::TreePop();
	}

	ImGui::DragInt("pressCount", &pressCount_, 1.0f);
	ImGui::DragInt("selectedStage", &selectedStageNumber_, 1.0f);
	ImGui::DragFloat2("stageNumber", &stageNumber_->position_.x, 0.5f);
	ImGui::End();

#endif // _DEBUG
}

void StageSelectManagerObject::Draw()
{
	// 全てのobjを描画
	DrawAllOBJ();
}

void StageSelectManagerObject::SpriteDraw()
{
	// 選択矢印描画
	for (int i = 0; i < 2; i++)
		selectArrowSprites_[i]->Draw();
	// ステージ番号背景描画
	stageNumberBackGround_->Draw();
	// ステージ番号の描画
	stageNumber_->Draw();
}

void StageSelectManagerObject::AddGlobalVariables()
{

}

void StageSelectManagerObject::ApplyGlobalVariables()
{

}

void StageSelectManagerObject::TransitionStaging()
{
	switch (cameraStagingWayPoint_)
	{
	case StageSelectManagerObject::WayPoint1: // カメラ移動
		if (transitionStagingT_ <= transitionStagingTime_) {
			// カメラ座標を動かす
			camera_->transform_.translate_.z = Math::EaseOut(transitionStagingT_, cameraStartTranslate_.z, cameraEndTranslate_.z, transitionStagingTime_);
			// 視野角を広げる
			camera_->fov_ = Math::EaseOut(transitionStagingT_, 0.4f, 0.55f, transitionStagingTime_);
			// UIの色を変更
			spriteUIColor_.w = Math::EaseOut(transitionStagingT_, 0.0f, 1.0f, transitionStagingTime_);
			// tを加算
			transitionStagingT_ += 1.0f / 60.0f;

			// 遷移中である
			isTransitionStaging_ = true;
		}
		else {
			// tをリセット
			transitionStagingT_ = 0.0f;
			// 秒数設定
			transitionStagingTime_ = 1.0f;

			// 遷移中ではない
			isTransitionStaging_ = false;

			// 次の演出へ
			cameraStagingWayPoint_++;
		}

		if (input_->TriggerKey(DIK_SPACE)) {

			// 無理やりフェードイン
			SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 0.0f });

			// UI色リセット
			spriteUIColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

			// カメラ座標を動かす
			camera_->transform_.translate_.z = cameraEndTranslate_.z;
			// 視野角を広げる
			camera_->fov_ = 0.55f;

			// tをリセット
			transitionStagingT_ = 0.0f;
			// 秒数設定
			transitionStagingTime_ = 1.0f;

			// 遷移中ではない
			isTransitionStaging_ = false;

			// 次の演出へ
			cameraStagingWayPoint_++;
		}
		break;
	case StageSelectManagerObject::WayPoint2: // スペースを押したらシーン遷移開始
		if (input_->TriggerKey(DIK_SPACE)) {
			// カメラシェイク無効
			enableCameraShake_ = false;
			// カメラの始端座標を現在のカメラ座標に変更
			cameraStartTranslate_ = camera_->transform_.translate_;
			// カメラの終端座標を設定
			cameraEndTranslate_ = { 0.0f, 0.0f, -17.0f };
			// 次の演出に
			cameraStagingWayPoint_++;
		}
		break;
	case StageSelectManagerObject::WayPoint3: // 一度カメラを引く
		if (transitionStagingT_ <= transitionStagingTime_) {
			// カメラ座標を動かす
			camera_->transform_.translate_ = Math::EaseInOut(transitionStagingT_, cameraStartTranslate_, cameraEndTranslate_, transitionStagingTime_);
			// UIの色を変更
			spriteUIColor_.w = Math::EaseOut(transitionStagingT_, 1.0f, 0.0f, transitionStagingTime_);
			// tを加算
			transitionStagingT_ += 1.0f / 60.0f;

			// 遷移中である
			isTransitionStaging_ = true;
		}
		else {
			// カメラの始端座標を現在のカメラ座標に変更
			cameraStartTranslate_ = cameraEndTranslate_;
			// カメラの終端座標を設定
			cameraEndTranslate_ = { 0.0f, 0.0f, -3.0f };

			// tをリセット
			transitionStagingT_ = 0.0f;
			// 秒数設定
			transitionStagingTime_ = 1.0f;

			// フェードアウト
			SceneManager::GetInstance()->StartFadeEffect(transitionStagingTime_, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });

			// 次の演出へ
			cameraStagingWayPoint_++;
		}
		break;
	case StageSelectManagerObject::WayPoint4: // カメラを画面奥に動かしながらシーン遷移
		if (transitionStagingT_ <= transitionStagingTime_) {
			// カメラ座標を動かす
			camera_->transform_.translate_ = Math::EaseInOut(transitionStagingT_, cameraStartTranslate_, cameraEndTranslate_, transitionStagingTime_);
			// 視野角を広げる
			camera_->fov_ = Math::EaseOut(transitionStagingT_, 0.55f, 0.35f, transitionStagingTime_);
			// tを加算
			transitionStagingT_ += 1.0f / 60.0f;

			// 遷移中である
			isTransitionStaging_ = true;
		}
		else {

			// tをリセット
			transitionStagingT_ = 0.0f;
			// 秒数設定
			transitionStagingTime_ = 1.0f;

			// 次の演出へ
			cameraStagingWayPoint_++;
		}
		break;
	case StageSelectManagerObject::WayPoint5:

		// 環境音を止める
		audio_->StopWave(voiceHandleAmbient_);
		audio_->StopWave(voiceHandleBGM_);

		// ゲームシーンへ遷移させる
		isGoGameScene_ = true;
		break;
	}
}

void StageSelectManagerObject::CameraShake()
{
	// カメラ手振れ演出有効なら
	if (enableCameraShake_) {
		if (cameraStagingT_ <= 3.0f / 2.0f) {
			// カメラを横に少し動かす
			if (!cameraStagingTReturn_) {
				camera_->transform_.translate_.y = Math::EaseInOut(cameraStagingT_, -0.05f, 0.05f, 3.0f / 2.0f);
			}
			else {
				camera_->transform_.translate_.y = Math::EaseInOut(cameraStagingT_, 0.05f, -0.05f, 3.0f / 2.0f);
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
}

void StageSelectManagerObject::RotateStart(bool isRight)
{
	// 始端角度設定
	startAngle_ = transform_.rotate_.z;
	// 終端角度設定
	if (isRight) {
		endAngle_ = transform_.rotate_.z - ((float)std::numbers::pi / 2.0f);
		if (selectedStageNumber_ < stageCount_)
			selectedStageNumber_++;
		else
			selectedStageNumber_ = 0;
	}
	else {
		endAngle_ = transform_.rotate_.z + ((float)std::numbers::pi / 2.0f);
		if (selectedStageNumber_ > 0)
			selectedStageNumber_--;
		else
			selectedStageNumber_ = stageCount_;
	}

	// 歯車回転音を鳴らす
	audio_->PlayWave(soundHandleRotateGear_, false, *seVolume_);

	// 演出中に
	isRotateStaging_ = true;
	previewItemStaging_ = false;
}

void StageSelectManagerObject::RotateStagePreview()
{
	if (rotateStagingT_ <= rotateStagingTime_ && isRotateStaging_) {
		transform_.rotate_.z = Math::EaseOut(rotateStagingT_, startAngle_, endAngle_, rotateStagingTime_);
		if (rotateStagingT_ <= 0.15f) {
			for (int i = 0; i < 20; i++)
				previewItems_[i]->transform_.scale_ = Math::EaseOut(rotateStagingT_, { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }, { 0.0f, 0.0f, 0.0f }, 0.15f);
			for (int i = 0; i < 5; i++)
				previewCatapults_[i]->transform_.scale_ = Math::EaseOut(rotateStagingT_, { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }, { 0.0f, 0.0f, 0.0f }, 0.15f);
		}
		else {
			for (int i = 0; i < 20; i++)
				previewItems_[i]->transform_.scale_ = { 0.0f, 0.0f, 0.0f };
			for (int i = 0; i < 5; i++)
				previewCatapults_[i]->transform_.scale_ = { 0.0f, 0.0f, 0.0f };
		}
		// t加算
		rotateStagingT_ += 1.0f / 60.0f;
	}
	else {

		// プレビュー表示の切り替え
		SetPreviewItems();

		// tリセット
		rotateStagingT_ = 0.0f;
		// 演出終了
		isRotateStaging_ = false;
	}
}

void StageSelectManagerObject::SetPreviewItems()
{
	// 全てのアイテムの情報
	for (int i = 0; i < (int)stageManager_->GetStageInfo(selectedStageNumber_).itemInfo_.size(); i++) {
		previewItems_[i]->transform_.translate_ = stageManager_->GetStageInfo(selectedStageNumber_).itemInfo_[i].position_;
		previewItems_[i]->transform_.translate_.x = previewItems_[i]->transform_.translate_.x / 6.0f;
		previewItems_[i]->transform_.translate_.y = previewItems_[i]->transform_.translate_.y / 6.0f;
		previewItems_[i]->transform_.scale_ = {0.0f, 0.0f, 0.0f};
		previewItems_[i]->SetIsRePop(stageManager_->GetStageInfo(selectedStageNumber_).itemInfo_[i].isRePop_);
	}
	for(int i = (int)stageManager_->GetStageInfo(selectedStageNumber_).itemInfo_.size(); i < 20; i++)
		previewItems_[i]->transform_.translate_ = { 10000.0f, 10000.0f, 0.0f };

	// 全てのアイテムの情報
	for (int i = 0; i < (int)stageManager_->GetStageInfo(selectedStageNumber_).catapultInfo_.size(); i++) {
		previewCatapults_[i]->SetTheta(stageManager_->GetStageInfo(selectedStageNumber_).catapultInfo_[i].theta_);
		previewCatapults_[i]->transform_.scale_ = { 0.0f, 0.0f, 0.0f };
		previewCatapults_[i]->SetLength(stageManager_->GetStageInfo(selectedStageNumber_).catapultInfo_[i].length_ / 5.25f);
	}
	for (int i = (int)stageManager_->GetStageInfo(selectedStageNumber_).catapultInfo_.size(); i < 5; i++) {
		previewCatapults_[i]->SetLength(10000.0f);
		previewCatapults_[i]->SetTheta(10000.0f);
	}

	// プレビューアイテム表示演出開始
	previewItemStaging_ = true;
	// 表示演出tをリセット
	previewItemStagingT_ = 0.0f;
}

void StageSelectManagerObject::PrevItemStaging()
{
	if (previewItemStagingT_ <= 0.25f) {
		for (int i = 0; i < (int)stageManager_->GetStageInfo(selectedStageNumber_).itemInfo_.size(); i++)
			previewItems_[i]->transform_.scale_ = Math::EaseOut(previewItemStagingT_, {0.0f, 0.0f, 0.0f}, { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }, 0.25f);
		for (int i = 0; i < (int)stageManager_->GetStageInfo(selectedStageNumber_).catapultInfo_.size(); i++)
			previewCatapults_[i]->transform_.scale_ = Math::EaseOut(previewItemStagingT_, { 0.0f, 0.0f, 0.0f }, { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }, 0.25f);

		// t加算
		previewItemStagingT_ += 1.0f / 60.0f;
	}
	else {

		for (int i = 0; i < (int)stageManager_->GetStageInfo(selectedStageNumber_).itemInfo_.size(); i++)
			previewItems_[i]->transform_.scale_ = { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f };
		for (int i = 0; i < (int)stageManager_->GetStageInfo(selectedStageNumber_).catapultInfo_.size(); i++)
			previewCatapults_[i]->transform_.scale_ = { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f };
		
		// 表示演出を止める
		previewItemStaging_ = false;
		// t加算
		previewItemStagingT_ = 0.0f;
	}
}

void StageSelectManagerObject::UIAnimation()
{
	for (int i = 0; i < 2; i++) {
		if (playingSelectArrowAnim_[i]) {
			if (selectArrowAnimT_[i] <= selectArrowAnimTime_[i]) {
				// 座標を動かす
				selectArrowPosition_[i].x = Math::Linear(selectArrowAnimT_[i], startSelectArrowPosition_[i].x, endSelectArrowPosition_[i].x, selectArrowAnimTime_[i]);
				// tを加算
				selectArrowAnimT_[i] += 1.0f / 60.0f;
			}
			else {
				// 選択アニメーション再生しない
				playingSelectArrowAnim_[i] = false;
				// 座標をリセット
				selectArrowPosition_[i].x = startSelectArrowPosition_[i].x;
				// tを加算
				selectArrowAnimT_[i] = 0.0f;
			}
		}
	}

	if (stageSelectedNumber_ >= 10){
		stageNumberBackGroundSize_.x = 192.0f * 1.5f;
		stageNumberBackGround_Position_.x = stageNumber_->position_.x + 30.0f;

	}
	else {
		stageNumberBackGroundSize_.x = 192.0f;
		stageNumberBackGround_Position_ = stageNumber_->position_;
	}

	stageNumber_->Update();

}

void StageSelectManagerObject::OrnamentAnimation()
{
	ornamentGearTransforms_[0].rotate_.z -= 0.01f;
	ornamentGearTransforms_[2].rotate_.z += 0.01f;
	// ギアの回転を大元ギアと合わせる
	ornamentGearTransforms_[1].rotate_.z = -(ornamentGearTransforms_[0].rotate_.z * (8.0f / 6.0f)) - 0.125f;
	ornamentGearTransforms_[3].rotate_.z = -(ornamentGearTransforms_[2].rotate_.z * (8.0f / 6.0f)) + 0.125f;
}
