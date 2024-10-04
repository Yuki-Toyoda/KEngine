#include "Line.h"

Line::~Line()
{
	
}

void Line::Init(const std::string& name, const Vector3& position, const Vector2& thickness, const float& length, Texture texture)
{
	// 座標とかを初期化
	name_ = name;
	position_ = position;
	thickness_ = thickness;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	length_ = length;

	// ワールド座標の初期化
	transform_.Init();
	transform_.translate_ = position_;
	transform_.scale_ = Vector3{ thickness_.x, thickness_.y, length_ };

	// マテリアル初期化
	trailMaterial_.enableLighting_ = false; // ライティング無効化
	// テクスチャ取得
	trailMaterial_.tex_ = texture;

	// 軌跡用配列を要素数で初期化
	trailBuffers_ = std::vector<TrailBuffer>(trailBufferCount_);

	// 軌跡用バッファの生成
	vertexBuffer_	= std::make_unique<StructuredBuffer<TrailVertex>>(2 * trailBufferCount_);			// 頂点
	indexBuffer_	= std::make_unique <StructuredBuffer<uint32_t>>(((2 * trailBufferCount_) - 2) * 3);	// インデックス
}

void Line::Update()
{
	// 回転角のリセット
	if (rotate_.x >= (float)std::numbers::pi * 2.0f) {
		rotate_.x -= (float)std::numbers::pi * 2.0f;
	}
	if (rotate_.x <= -(float)std::numbers::pi * 2.0f) {
		rotate_.x += (float)std::numbers::pi * 2.0f;
	}

	if (rotate_.y >= (float)std::numbers::pi * 2.0f) {
		rotate_.y -= (float)std::numbers::pi * 2.0f;
	}
	if (rotate_.y <= -(float)std::numbers::pi * 2.0f) {
		rotate_.y += (float)std::numbers::pi * 2.0f;
	}

	if (rotate_.z >= (float)std::numbers::pi * 2.0f) {
		rotate_.z -= (float)std::numbers::pi * 2.0f;
	}
	if (rotate_.z <= -(float)std::numbers::pi * 2.0f) {
		rotate_.z += (float)std::numbers::pi * 2.0f;
	}
	
	transform_.translate_ = position_;
	transform_.rotate_ = rotate_;

	transform_.scale_ = Vector3(thickness_.x, length_, thickness_.y);

	// オフセットを求めるよ
	Vector3 offset = { 0.0f, length_, 0.0f };
	// 回転角を元に回転行列を求める
	Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

	// オフセットを元に移動
	transform_.translate_ = position_ + (offset * rotateMat);
	
	// 親がいるのであれば
	if (transform_.GetParent() != nullptr) {
		worldPos_ = transform_.GetWorldPos();
	}

	// 軌跡描画を行う場合軌跡の更新を行う
	if (isDisplayTrail_) {
		// 軌跡更新
		TrailUpdate();
	}
}

void Line::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 軌跡描画を行うかつバッファサイズが2以上のとき
	if (isDisplayTrail_ && trailBuffers_.size() > 2) {
		cmdList;
	}
}

void Line::DisplayImGui()
{
	if (ImGui::TreeNode("Line")) {
		ImGui::Checkbox("IsActive", &isActive_);
		ImGui::DragFloat3("Position", &position_.x, 0.05f);
		ImGui::DragFloat2("Thickness", &thickness_.x, 0.05f, 0.05f);
		ImGui::DragFloat("Length", &length_, 0.05f, 0.05f);
		ImGui::DragFloat3("Rotate", &rotate_.x, 0.005f);

		if (ImGui::TreeNode("Trail")) {
			// 軌跡の表示トリガー
			ImGui::Checkbox("IsDisplay", &isDisplayTrail_);

			// バッファの状態を描画
			if (ImGui::TreeNode("Buffers")) {
				// 配列内の要素を表示する
				for (int i = 0; i < trailBuffers_.size(); i++) {
					// 名前設定
					std::string sName = "Trail - Start : " + std::to_string(i);
					std::string eName = "Trail - End" + std::to_string(i);
					// 始点
					ImGui::DragFloat3(sName.c_str(), &trailBuffers_[i].start.x);
					// 終点
					ImGui::DragFloat3(eName.c_str(), &trailBuffers_[i].end.x);
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void Line::TrailUpdate()
{
	// 線の始点と終点を取得
	tempTrail_.start = position_;
	// オフセットを求める
	Vector3 offset = { 0.0f, length_, 0.0f };
	// 回転角を元に回転行列を求める
	Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);
	// 終点の座標を取得する
	tempTrail_.end = position_ + (offset * rotateMat);
	// 使用済み状態に
	tempTrail_.isUsed = true;

	// 軌跡座標の更新
	for (int i = static_cast<int>(trailBuffers_.size() - 1); i > 0; --i) {
		// 座標情報をずらす
		trailBuffers_[i] = trailBuffers_[i - 1];
	}
	// 最終要素の現在の座標を代入する
	trailBuffers_.front() = tempTrail_;
	
	// 使用済み要素を抽出
	std::vector<TrailBuffer> usedTrailBuffer = GetUsedTrailBuffer();
	// 要素が2つ以上ない場合平面を生成出来ないので早期リターン
	if (usedTrailBuffer.size() <= 2) {
		return;
	}

	// UV座標のオフセット用
	float amount = 1.0f / (usedTrailBuffer.size() - 1);
	float uvOffset = 0;

	// 頂点配列のクリア
	vertices_.clear();
	// 頂点配列のリサイズ
	vertices_.resize(usedTrailBuffer.size() * 4);

	// 頂点座標とUV座標の調整
	for (int i = 0, j = 0; i < vertices_.size() && j < usedTrailBuffer.size(); i += 2, j++) {
		vertices_[i].pos		= { usedTrailBuffer[j].start.x, usedTrailBuffer[j].start.y, usedTrailBuffer[j].start.z, 1.0f };
		vertices_[i].uv			= { 1.0f, uvOffset };
		vertices_[i + 1].pos	= { usedTrailBuffer[j].end.x, usedTrailBuffer[j].end.y, usedTrailBuffer[j].end.z, 1.0f };
		vertices_[i + 1].uv		= { 0.0f, uvOffset };

		// UV座標をずらす
		uvOffset += amount;
	}

	// インデックス配列のクリア
	indices_.clear();
	// インデックス配列のリサイズ
	indices_.resize(6 * trailBufferCount_);

	for (int i = 0; i < indices_.size(); i += 3) {
		indices_[i]		= 2 + i;
		indices_[i + 1] = 1 + i;
		indices_[i + 2] = 0 + i;
	}
}

void Line::AddCollider(const std::string& name, IObject* object)
{
	if (transform_.GetParent() != nullptr) {
		object->AddColliderOBB(name, &transform_.scale_, &transform_.rotate_, &worldPos_);
	}
	else {
		object->AddColliderOBB(name, &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	}
}

void Line::SetParent(WorldTransform* parent, uint8_t parentType)
{
	// 親子関係をセット
	transform_.SetParent(parent, parentType);
}

std::vector<Line::TrailBuffer> Line::GetUsedTrailBuffer()
{
	// 返還用配列
	std::vector<TrailBuffer> tv;

	// 配列分ループ
	for (int i = 0; i < trailBuffers_.size(); i++) {
		// 使用済みなら配列に追加する
		if (trailBuffers_[i].isUsed) {
			tv.push_back(trailBuffers_[i]);
		}
	}

	// 取得した配列を返す
	return tv;
}
