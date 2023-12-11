#include "SampleBox.h"
#include "../../../Resource/Texture/TextureManager.h"

void SampleBox::Initialize() {

	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	AddSprite("Test", { 640.0f, 460.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// 当たり判定の設定
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), transform_.scale_);
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

}

void SampleBox::Update() {

	//transform_.rotate_.y += 0.01f;

	// タイマーの更新処理
	timer_.Update();

	// コライダーの更新
	collider_->Update(transform_.GetWorldPos(), transform_.scale_);
	// リストに自分自身を登録
	collisionManager_->RegisterCollider(collider_.get());

}

void SampleBox::DisplayImGui() {
	// ワールド座標の表示
	transform_.DisplayImGui();
	// テキスト表示
	ImGui::Text("Color");
	//　色の変更
	ImGui::ColorPicker4("Color", &color_.x);

	timer_.DisplayImGui("timer");

	ImGui::DragFloat("StartValue", &imGuiStartValue_, 0.05f);
	ImGui::DragFloat("EndValue", &imGuiEndValue_, 0.05f);
	float nowValue = KLib::Lerp<float>(imGuiStartValue_, imGuiEndValue_, timer_.GetProgress());
	ImGui::Text("nowValue : %4.2f", nowValue);

	sprites_[0]->DisplayImGui();

	for (Mesh* mesh : meshes_) {
		mesh->DisplayImGui();
		ImGui::SliderInt("LayerNo", &mesh->layerNo_, 0, 10);
		ImGui::Checkbox("isUI", &mesh->isUI_);
		ImGui::Checkbox("isActive", &mesh->isActive_);

		ImGui::SliderInt("BlendMode", &imGuiSelectBlendMode_, 0, 4);
		mesh->blendMode_ = (BasePrimitive::kBlendMode)imGuiSelectBlendMode_;

		ImGui::Checkbox("isBillBoard", &mesh->isBillboard_);
	}
}

void SampleBox::OnCollisionEnter(BaseObject* object)
{
	object;
	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void SampleBox::OnCollisionExit(BaseObject* object)
{
	object;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
}
