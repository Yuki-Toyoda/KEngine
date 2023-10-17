#include "TitleManagerObject.h"
#include "../../Resource/Texture/TextureManager.h"

void TitleManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	transform_.translate_ = { 1.25f, 0.0f, 0.0f };

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
	logoColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	logoAnchorPoint_ = { 0.5f, 0.5f };

	// テクスチャ読み込み
	textureHandleTitleLogo_ = TextureManager::Load("./Resources/Image/Title", "titleLogo.png");
	// スプライト読み込み
	titleLogo_.reset(Sprite::Create(textureHandleTitleLogo_, &logoPosition_, &logoSize_, &logoColor_, logoAnchorPoint_));

}

void TitleManagerObject::Update()
{

	BaseObject::Update();

	ｍainGearTransform_.rotate_.z += (0.025f * (6.0f / 8.0f)) * (8.0f / 12.0f);
	mGearTransform_.rotate_.z -= 0.025f * (6.0f / 8.0f);
	mGearTransform2_.rotate_.z -= 0.025f * (6.0f / 8.0f);
	sGearTransform_.rotate_.z += 0.025f * (6.0f / 8.0f);
	titleGearTransform_.rotate_.z -= 0.025f;

#ifdef _DEBUG

	// Imgui
	ImGui::Begin(objectName_.c_str());

	ImGui::DragFloat2("SpritePos", &logoPosition_.x, 1.0f);
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
	titleLogo_->Draw();
}

void TitleManagerObject::AddGlobalVariables()
{
}

void TitleManagerObject::ApplyGlobalVariables()
{
}
