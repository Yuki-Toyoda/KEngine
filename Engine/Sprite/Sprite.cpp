#include "Sprite.h"
#include "../Primitive/PrimitiveManager.h"

void Sprite::Initialize(const std::string& name)
{
	// トランスフォーム初期化
	transform_.Initialize();

	// 形状生成
	primitive_ = PrimitiveManager::GetInstance()->CreateInstance<Plane>();
	// 形状
	primitive_->isUI_ = true;
	// 名称設定
	primitive_->name_ = name;
	primitive_->commonColor = &color_;
}

void Sprite::Initialize(const std::string& name, const Vector2 position, const Vector2& size, Texture* texture)
{
	// トランスフォーム初期化
	transform_.Initialize();

	// 形状生成
	primitive_ = PrimitiveManager::GetInstance()->CreateInstance<Plane>();
	// 形状
	primitive_->isUI_ = true;
	// メンバ変数を元に設定
	primitive_->name_ = name;	    // 名称設定
	translate_ = position;			// 初期座標
	scale_ = size;					// 大きさ
	primitive_->texture_ = texture; // テクスチャ
	texSize_ = primitive_->texture_->GetTextureSize();
	primitive_->commonColor = &color_;

	// 形状にワールドトランスフォームを渡す
	primitive_->transform_ = &transform_;
}

void Sprite::Update()
{
	// 頂点座標を求める
	float left = (0.0f - anchorPoint_.x) * scale_.x;   // 左
	float right = (1.0f - anchorPoint_.x) * scale_.x;  // 右
	float top = (0.0f - anchorPoint_.y) * scale_.y;    // 上
	float bottom = (1.0f - anchorPoint_.y) * scale_.y; // 下

	// 頂点をずらす
	primitive_->vertices_[0].position = { left, top, 0.0f };  // 左下
	primitive_->vertices_[1].position = { right, top, 0.0f };     // 左上
	primitive_->vertices_[2].position = { right, bottom, 0.0f }; // 右下
	primitive_->vertices_[3].position = { left, bottom, 0.0f };    // 右上

	float tex_left = texBase_.x / primitive_->texture_->GetTextureSize().x;
	float tex_right = (texBase_.x + texSize_.x) / primitive_->texture_->GetTextureSize().x;
	float tex_top = texBase_.y / primitive_->texture_->GetTextureSize().y;
	float tex_bottom = (texBase_.y + texSize_.y) / primitive_->texture_->GetTextureSize().y;

	primitive_->vertices_[0].texCoord = { tex_left, tex_top };  // 左下
	primitive_->vertices_[1].texCoord = { tex_right, tex_top };     // 左上
	primitive_->vertices_[2].texCoord = { tex_right, tex_bottom }; // 右下
	primitive_->vertices_[3].texCoord = { tex_left, tex_bottom };    // 右上

	// 座標設定
	primitive_->transform_->translate_ = { translate_.x, translate_.y, 0.0f };
	// 回転設定
	primitive_->transform_->rotate_ = { 0.0f, 0.0f, rotate_ };
}

void Sprite::DisplayImGui()
{
	std::string name = "Sprite - " + primitive_->name_;
	if (ImGui::TreeNode(name.c_str())) {
		ImGui::DragFloat2("Scale", &scale_.x, 0.1f);
		ImGui::DragFloat("rotate", &rotate_, 0.01f);
		ImGui::DragFloat2("Position", &translate_.x, 1.0f);
		ImGui::DragFloat2("AnchorPoint", &anchorPoint_.x, 0.01f);
		ImGui::DragFloat2("texBase", &texBase_.x, 1.0f);
		ImGui::DragFloat2("texSize", &texSize_.x, 1.0f);
		ImGui::ColorPicker4("Color", &color_.x);

		ImGui::SliderInt("BlendMode", &imGuiSelectBlendMode_, 0, 4);
		primitive_->blendMode_ = (BasePrimitive::kBlendMode)imGuiSelectBlendMode_;

		if (ImGui::TreeNode("UVTransform")) {
			primitive_->material_.uvTransform_.DisplayImGui("uvTransform");
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
