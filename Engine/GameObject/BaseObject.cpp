#include "BaseObject.h"

void BaseObject::PreInitialize()
{
	// 非表示
	isActive_ = false;
	
	// ワールドトランスフォームの初期化
	transform_.Initialize();

	// メッシュリストクリア
	meshes_.clear();

	// タグの初期化
	tag_ = TagNone;
	// 名前の初期化
	name_ = "object";

	// デストロイトリガーFalse
	isDestroy_ = false;
}

void BaseObject::PreUpdate()
{
	// 回転角のリセット
	if (transform_.rotate_.x > (float)std::numbers::pi * 2.0f)
		transform_.rotate_.x = transform_.rotate_.x - (float)std::numbers::pi * 2.0f;
	else if(transform_.rotate_.x < -(float)std::numbers::pi * 2.0f)
		transform_.rotate_.x = (float)std::numbers::pi * 2.0f + (transform_.rotate_.x - (float)std::numbers::pi * 2.0f); // x

	if (transform_.rotate_.y > (float)std::numbers::pi * 2.0f)
		transform_.rotate_.y = transform_.rotate_.y - (float)std::numbers::pi * 2.0f;
	else if (transform_.rotate_.y < -(float)std::numbers::pi * 2.0f)
		transform_.rotate_.y = (float)std::numbers::pi * 2.0f + (transform_.rotate_.y - (float)std::numbers::pi * 2.0f); // y

	if (transform_.rotate_.z > (float)std::numbers::pi * 2.0f)
		transform_.rotate_.z = transform_.rotate_.z - (float)std::numbers::pi * 2.0f;
	else if (transform_.rotate_.z < -(float)std::numbers::pi * 2.0f)
		transform_.rotate_.z = (float)std::numbers::pi * 2.0f + (transform_.rotate_.z - (float)std::numbers::pi * 2.0f); // z
}

void BaseObject::DisplayImGui()
{
	// 基底クラスでは特に記述なし
}

void BaseObject::AddMesh(WorldTransform* wt, const Vector4& color, const std::string& path, const std::string& fileName, bool enableLighting)
{
	
}

