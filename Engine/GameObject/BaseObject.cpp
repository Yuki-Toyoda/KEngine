#include "BaseObject.h"

BaseObject::~BaseObject() {
}

void BaseObject::Initialize(std::string name, Tag tag)
{
	globalVariables_ = GlobalVariables::GetInstance(); // 調整項目クラス取得
	collisionManager_ = CollisionManager::GetInstance(); // 衝突マネージャー取得
	collider_.reset(new Collider); // コライダーのインスタンス生成
	isDestroy_ = false;
	objectName_ = name;
	objectTag_ = tag;
	transform_.Initialize();
}

void BaseObject::Update()
{
	// グローバル変数の値を適用させる
	ApplyGlobalVariables();

	// 回転角のリセット
	if (transform_.rotate_.x >= (float)std::numbers::pi * 2.0f || (float)-std::numbers::pi * 2.0f >= transform_.rotate_.x)
		transform_.rotate_.x = 0.0f;
	if (transform_.rotate_.y >= (float)std::numbers::pi * 2.0f || (float)-std::numbers::pi * 2.0f >= transform_.rotate_.y)
		transform_.rotate_.y = 0.0f;
	if (transform_.rotate_.z >= (float)std::numbers::pi * 2.0f || (float)-std::numbers::pi * 2.0f >= transform_.rotate_.z)
		transform_.rotate_.z = 0.0f;

#ifdef _DEBUG
	// Imgui
	ImGui::Begin(objectName_.c_str());
	ImGui::Checkbox("Active", &isActive_);
	if (ImGui::TreeNode("Transform")) {
		ImGui::DragFloat3("scale", &transform_.scale_.x, 0.5f);
		ImGui::DragFloat3("rotatate", &transform_.rotate_.x, 0.05f);
		ImGui::DragFloat3("translate", &transform_.translate_.x, 0.5f);
		ImGui::TreePop();
	}
	if (ImGui::Button("Destroy")) {
		Destroy();
	}
	ImGui::Checkbox("debug", &isDebug_);
	ImGui::End();
#endif // _DEBUG
}

void BaseObject::AddOBJ(WorldTransform* wt, const Vector4& color, const std::string& directoryPath, const std::string& fileName, bool enableLighting)
{
	// モデル読み込み
	objects_.emplace_back(OBJ::Create(wt, color, directoryPath, fileName));
	objects_.back()->SetEnableLighting(enableLighting);
}

void BaseObject::DrawAllOBJ()
{
	// オブジェクトの描画
	for (auto& obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}
