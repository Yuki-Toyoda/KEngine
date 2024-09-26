#include "Sprite.h"
#include "../Model/ModelManager.h"

Sprite::~Sprite()
{
	// メッシュを破棄
	model_->isDestroy_ = true;
}

void Sprite::Init(const std::string& name, ModelData* modelData)
{
	// 名称取得
	name_ = name;

	// トランスフォーム初期化
	transform_.Init();

	// スプライト用モデルを生成
	model_ = ModelManager::GetInstance()->CreateSpriteModel(modelData);

	// サイズをテクスチャ自体のサイズに設定
	scale_ = model_->material_.tex_.GetTextureSize();
	// 表示範囲設定
	texSize_ = model_->material_.tex_.GetTextureSize();

	// モデルのトランスフォームに当該トランスフォームを親として設定する
	model_->transform_.SetParent(&transform_);
}

void Sprite::Init(const std::string& name, ModelData* modelData, const Vector2 position, const Vector2& size, Texture texture)
{
	// 名称取得
	name_ = name;

	// トランスフォーム初期化
	transform_.Init();

	// スプライト用モデルを生成
	model_ = ModelManager::GetInstance()->CreateSpriteModel(modelData);

	// 引数を元に設定を行う
	translate_ = position;			// 初期座標
	scale_ = size;					// 大きさ
	model_->material_.tex_ = texture; // テクスチャ
	texSize_ = model_->material_.tex_.GetTextureSize();

	// モデルのトランスフォームに当該トランスフォームを親として設定する
	model_->transform_.SetParent(&transform_);
}

void Sprite::Update()
{
	// 頂点座標を求める
	float left = (0.0f - anchorPoint_.x) * scale_.x;   // 左
	float right = (1.0f - anchorPoint_.x) * scale_.x;  // 右
	float top = (0.0f - anchorPoint_.y) * scale_.y;    // 上
	float bottom = (1.0f - anchorPoint_.y) * scale_.y; // 下

	// 頂点をずらす
	model_->vertices_[0].position_ = { left, top, 0.0f, 1.0f };  // 左下
	model_->vertices_[1].position_ = { right, bottom, 0.0f, 1.0f };     // 左上
	model_->vertices_[2].position_ = { right, top, 0.0f, 1.0f }; // 右下
	model_->vertices_[3].position_ = { left, top, 0.0f, 1.0f };    // 右上
	model_->vertices_[4].position_ = { left, bottom, 0.0f, 1.0f };    // 右上
	model_->vertices_[5].position_ = { right, bottom, 0.0f, 1.0f };    // 右上

	float tex_left = texBase_.x / model_->material_.tex_.GetTextureSize().x;
	float tex_right = (texBase_.x + texSize_.x) / model_->material_.tex_.GetTextureSize().x;
	float tex_top = texBase_.y / model_->material_.tex_.GetTextureSize().y;
	float tex_bottom = (texBase_.y + texSize_.y) / model_->material_.tex_.GetTextureSize().y;

	model_->vertices_[0].texCoord_ = { tex_left, tex_top };     // 左下
	model_->vertices_[1].texCoord_ = { tex_right, tex_bottom };    // 左上
	model_->vertices_[2].texCoord_ = { tex_right, tex_top }; // 右下
	model_->vertices_[3].texCoord_ = { tex_left, tex_top };  // 右上
	model_->vertices_[4].texCoord_ = { tex_left, tex_bottom };  // 右上
	model_->vertices_[5].texCoord_ = { tex_right, tex_bottom };  // 右上

	for (int i = 0; i < 6; i++) {
		model_->vertices_[i].normal_ = { 0.0f, 0.0f, -1.0f };
	}

	// 座標設定
	transform_.translate_ = { translate_.x, translate_.y, 0.0f };
	// 回転設定
	transform_.rotate_ = { 0.0f, 0.0f, rotate_ };

	// マテリアル色設定
	model_->material_.color_ = color_;

	// 表示トリガーを元にスプライトモデルの表示を設定
	model_->isActive_ = isActive_;
}

void Sprite::DisplayImGui()
{
	if (ImGui::TreeNode(name_.c_str())) {
		ImGui::Checkbox("isActive", &isActive_);
		ImGui::DragFloat2("Scale", &scale_.x, 0.1f);
		ImGui::DragFloat("rotate", &rotate_, 0.01f);
		ImGui::DragFloat2("Position", &translate_.x, 1.0f);
		ImGui::DragFloat2("AnchorPoint", &anchorPoint_.x, 0.01f);
		ImGui::DragFloat2("texBase", &texBase_.x, 1.0f);
		ImGui::DragFloat2("texSize", &texSize_.x, 1.0f);
		ImGui::ColorPicker4("Color", &color_.x);

		if (ImGui::TreeNode("UVTransform")) {
			model_->material_.DisplayImGui();
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
