#include "ICPUParticle.h"

void ICPUParticle::PreInit(float aliveTime, const Vector3& position, const Vector3& scale, const Vector3& velocity, const Material& material, const Vector4& color)
{
	// 生存時間設定
	aliveTimer_.Start(aliveTime);

	// トランスフォーム内の情報を表示
	transform_.Init();

	// 座標設定
	transform_.translate_ = position;
	// サイズ設定
	transform_.scale_ = scale;

	// 速度設定
	velocity_ = velocity;

	// マテリアル取得
	material_ = material;
	// 色設定
	material_.color_ = color;

	// 固有初期化を呼び出す
	Init();

	// 一度更新後処理を呼び出す
	PostUpdate();
}

void ICPUParticle::Init()
{
	// 基底クラスでは記述なし
}

void ICPUParticle::Update()
{
	transform_.translate_ = transform_.translate_ + velocity_;

	material_.color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));
}

void ICPUParticle::PostUpdate()
{
	//// 頂点座標を求める
	//float left = (0.0f - anchorPoint_.x) * scale_.x;   // 左
	//float right = (1.0f - anchorPoint_.x) * scale_.x;  // 右
	//float top = (0.0f - anchorPoint_.y) * scale_.y;    // 上
	//float bottom = (1.0f - anchorPoint_.y) * scale_.y; // 下

	//// 頂点をずらす
	//plane_->vertices_[0].position = { right, top, 0.0f };  // 左下
	//plane_->vertices_[1].position = { left, top, 0.0f };     // 左上
	//plane_->vertices_[2].position = { left, bottom, 0.0f }; // 右下
	//plane_->vertices_[3].position = { right, bottom, 0.0f };    // 右上

	//// テクスチャの描画範囲設定
	//float tex_left = texBase_.x / plane_->texture_->GetTextureSize().x;
	//float tex_right = (texBase_.x + texSize_.x) / plane_->texture_->GetTextureSize().x;
	//float tex_top = texBase_.y / plane_->texture_->GetTextureSize().y;
	//float tex_bottom = (texBase_.y + texSize_.y) / plane_->texture_->GetTextureSize().y;

	//// テクスチャの描画座標をずらす
	//plane_->vertices_[0].texCoord = { tex_left, tex_top };  // 左下
	//plane_->vertices_[1].texCoord = { tex_right, tex_top };     // 左上
	//plane_->vertices_[2].texCoord = { tex_right, tex_bottom }; // 右下
	//plane_->vertices_[3].texCoord = { tex_left, tex_bottom };    // 右上

	//// 回転設定
	//plane_->transform_->rotate_ = { 0.0f, 0.0f, rotate_ };

	//// ビルボード行列更新
	//plane_->UpdateBillboardMat();

	// タイマー更新
	aliveTimer_.Update();

	// タイマーが終了していたら
	if (aliveTimer_.GetIsFinish()) {
		isEnd_ = true;
	}
}
