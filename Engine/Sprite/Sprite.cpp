#include "Sprite.h"
#include "../Primitive/PrimitiveManager.h"

Sprite::~Sprite()
{
	// メッシュを破壊
	plane_->isDestroy_ = true;
}

void Sprite::Init(const std::string& name)
{
	// トランスフォーム初期化
	transform_.Init();

	// 形状生成
	plane_ = PrimitiveManager::GetInstance()->CreateInstance<Plane>();
	// 形状
	plane_->isUI_ = true;
	// 名称設定
	plane_->name_ = name;
	plane_->commonColor = &color_;
}

void Sprite::Init(const std::string& name, const Vector2 position, const Vector2& size, Texture* texture)
{
	// トランスフォーム初期化
	transform_.Init();

	// 形状生成
	plane_ = PrimitiveManager::GetInstance()->CreateInstance<Plane>();
	// 形状
	plane_->isUI_ = true;
	// メンバ変数を元に設定
	plane_->name_ = name;	    // 名称設定
	translate_ = position;			// 初期座標
	scale_ = size;					// 大きさ
	plane_->texture_ = texture; // テクスチャ
	texSize_ = plane_->texture_->GetTextureSize();
	plane_->commonColor = &color_;
	plane_->primitiveType_ = IPrimitive::kModelSprite;

	// 形状にワールドトランスフォームを渡す
	plane_->transform_ = &transform_;
}

void Sprite::Update()
{
	// 頂点座標を求める
	float left = (0.0f - anchorPoint_.x) * scale_.x;   // 左
	float right = (1.0f - anchorPoint_.x) * scale_.x;  // 右
	float top = (0.0f - anchorPoint_.y) * scale_.y;    // 上
	float bottom = (1.0f - anchorPoint_.y) * scale_.y; // 下

	// 頂点をずらす
	plane_->v_[0].position = { left, top, 0.0f };  // 左下
	plane_->v_[1].position = { right, top, 0.0f };     // 左上
	plane_->v_[2].position = { right, bottom, 0.0f }; // 右下
	plane_->v_[3].position = { left, bottom, 0.0f };    // 右上

	float tex_left = texBase_.x / plane_->texture_->GetTextureSize().x;
	float tex_right = (texBase_.x + texSize_.x) / plane_->texture_->GetTextureSize().x;
	float tex_top = texBase_.y / plane_->texture_->GetTextureSize().y;
	float tex_bottom = (texBase_.y + texSize_.y) / plane_->texture_->GetTextureSize().y;

	plane_->v_[0].texCoord = { tex_left, tex_top };  // 左下
	plane_->v_[1].texCoord = { tex_right, tex_top };     // 左上
	plane_->v_[2].texCoord = { tex_right, tex_bottom }; // 右下
	plane_->v_[3].texCoord = { tex_left, tex_bottom };    // 右上

	// 座標設定
	plane_->transform_->translate_ = { translate_.x, translate_.y, 0.0f };
	// 回転設定
	plane_->transform_->rotate_ = { 0.0f, 0.0f, rotate_ };
}

void Sprite::DisplayImGui()
{
	std::string name = "Sprite - " + plane_->name_;
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::Checkbox("isActive", &plane_->isActive_);
		ImGui::DragFloat2("Scale", &scale_.x, 0.1f);
		ImGui::DragFloat("rotate", &rotate_, 0.01f);
		ImGui::DragFloat2("Position", &translate_.x, 1.0f);
		ImGui::DragFloat2("AnchorPoint", &anchorPoint_.x, 0.01f);
		ImGui::DragFloat2("texBase", &texBase_.x, 1.0f);
		ImGui::DragFloat2("texSize", &texSize_.x, 1.0f);
		ImGui::ColorPicker4("Color", &color_.x);

		if (ImGui::TreeNode("UVTransform")) {
			plane_->material_.uvTransform_.DisplayImGui("uvTransform");
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
