#include "StageSelectManagerObject.h"
#include "../../Input/Input.h"

void StageSelectManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// 入力取得
	input_ = Input::GetInstance();

	// 初期座標を設定
	transform_.translate_ = { 0.0f, -7.25f, 0.0f };
	mainGearTransform_.Initialize();
	mainGearTransform_.SetParent(&transform_, 0b101);

	stageCount_ = 10;

	// 全てのステージプレビュー座標を初期化
	for (int i = 0; i < 4; i++)
		previewStageTransforms_[i].Initialize();

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

	// モデル読み込み
	AddOBJ(&mainGearTransform_, color_, "./Resources/Gear", "Gear_XL.obj", false);
	AddOBJ(&mainGearTransform_, color_, "./Resources/GearBase", "GearBoard.obj", false);
	AddOBJ(&transform_, color_, "./Resources/GearBase", "GearBase.obj", false);
	for(int i = 0; i < 4; i++)
		AddOBJ(&previewStageTransforms_[i], color_, "./Resources/Gear", "Gear_L.obj", false);
}

void StageSelectManagerObject::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// ギアの回転を大元ギアと合わせる
	previewStageTransforms_[0].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) + 0.125f;
	previewStageTransforms_[1].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) - 0.125f;
	previewStageTransforms_[2].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) + 0.125f;
	previewStageTransforms_[3].rotate_.z = (transform_.rotate_.z * (18.0f / 12.0f)) - 0.125f;

	// キーを押すとステージを選択
	if (input_->TriggerKey(DIK_D) || input_->TriggerKey(DIK_RIGHTARROW))
		pressCount_++;
	else if (input_->TriggerKey(DIK_A) || input_->TriggerKey(DIK_LEFTARROW))
		pressCount_--;

	if (selectedStageNumber_ < 10 || selectedStageNumber_ > 0) {
		if (pressCount_ > 0) {
			// 押下回数デクリメント
			pressCount_--;
		}
		else if (pressCount_ < 0) {
			// 押下回数インクリメント
			pressCount_++;
		}
	}

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
	}
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

}

void StageSelectManagerObject::AddGlobalVariables()
{

}

void StageSelectManagerObject::ApplyGlobalVariables()
{

}

void StageSelectManagerObject::RotateStagePreview()
{

}
