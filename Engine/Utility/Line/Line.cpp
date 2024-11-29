#include "Line.h"
#include "Engine/Base/DirectXCommon.h"

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
	transform_.scale_ = Vector3{ thickness_.x, length_, thickness_.y };

	// マテリアル初期化
	trailMaterial_.enableLighting_ = false; // ライティング無効化
	// テクスチャ取得
	trailMaterial_.tex_ = texture;

	// 軌跡用配列を要素数で初期化
	trailBuffers_ = std::vector<TrailBuffer>(trailBufferCount_);

	// バッファ初期化のためにデバイスとSRVの取得
	DirectXDevice* d = DirectXCommon::GetInstance()->GetDirectXDevice(); // デバイス
	SRV* s = DirectXCommon::GetInstance()->GetSRV();			// SRV

	// 軌跡用バッファの生成
	transformBuffer_ = std::make_unique<ConstantBuffer<Matrix4x4>>();													// トランスフォーム
	materialBuffer_ = std::make_unique<ConstantBuffer<MaterialData>>();													// マテリアル
	vertexBuffer_ = std::make_unique<StructuredBuffer<TrailVertex>>((2 * trailBufferCount_) * (trailInterpCount_));		// 頂点
	indexBuffer_ = std::make_unique <StructuredBuffer<uint32_t>>((6 * (trailBufferCount_ - 1)) * trailInterpCount_);	// インデックス
	// バッファ初期化
	transformBuffer_->Init(d);
	materialBuffer_->Init(d);
	vertexBuffer_->Init(d, s);
	indexBuffer_->Init(d, s);

	timer_.Start(0.0f);
	timer_.Update();
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
		// 軌跡描画
		TrailDraw();
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

			std::vector<TrailBuffer> b = GetUsedTrailBuffer();

			// バッファの状態を描画
			if (ImGui::TreeNode("Buffers")) {
				// 配列内の要素を表示する
				for (int i = 0; i < b.size(); i++) {
					// 名前設定
					std::string sName = "Trail - Start : " + std::to_string(i);
					std::string eName = "Trail - End" + std::to_string(i);
					// 始点
					ImGui::DragFloat3(sName.c_str(), &b[i].start.x);
					// 終点
					ImGui::DragFloat3(eName.c_str(), &b[i].end.x);
				}
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void Line::AddTrail()
{
	// 軌跡座標を更新しない場合早期リターン
	if (!isTrailUpdate_) { return; }

	// 親がいる場合
	if (transform_.GetParent() != nullptr) {

		// オフセットを求める
		Vector3 offset = { 0.0f, length_, 0.0f };
		// 回転角を元に回転行列を求める
		Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

		Matrix4x4 t = transform_.GetMatWorld();

		// 始点設定
		tempTrail_.start = position_ * transform_.GetMatWorld();
		// 終点の座標を取得する
		tempTrail_.end = (position_ + (offset * rotateMat)) * transform_.GetMatWorld();
	}
	else {
		// オフセットを求める
		Vector3 offset = { 0.0f, length_ * 2.0f, 0.0f };
		// 回転角を元に回転行列を求める
		Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

		// 始点設定
		tempTrail_.start = position_;
		// 終点の座標を取得する
		tempTrail_.end = position_ + (offset * rotateMat);
	}

	// 使用済み状態に
	tempTrail_.isUsed = true;

	// 軌跡座標の更新
	for (int i = static_cast<int>(trailBuffers_.size() - 1); i > 0; --i) {
		// 座標情報をずらす
		trailBuffers_[i] = trailBuffers_[i - 1];
	}
	// 最終要素の現在の座標を代入する
	trailBuffers_.front() = tempTrail_;
}

void Line::TrailUpdate()
{
	// 軌跡用座標追加
	AddTrail();

	// 使用済み要素を抽出
	std::vector<TrailBuffer> usedTrailBuffer = GetUsedTrailBuffer();
	// 要素が2つ以上ない場合平面を生成出来ないので早期リターン
	if (usedTrailBuffer.size() <= 2) {
		return;
	}

	// 曲線補間を行う
	MakeCurveVertices(usedTrailBuffer);

	// UV座標のオフセット用
	float amount = 1.0f / (usedTrailBuffer.size() - 1);
	float uvOffset = 0;

	// 頂点配列のクリア
	vertices_.clear();
	// 頂点配列のリサイズ
	vertices_.resize(usedTrailBuffer.size() * 2);

	// 頂点座標とUV座標の調整
	for (int i = 0, j = 0; i < vertices_.size() && j < usedTrailBuffer.size(); i += 2, j++) {
		vertices_[i].pos = { usedTrailBuffer[j].start.x, usedTrailBuffer[j].start.y, usedTrailBuffer[j].start.z, 1.0f };
		vertices_[i].uv = { uvOffset, 0.0f };
		vertices_[i + 1].pos = { usedTrailBuffer[j].end.x, usedTrailBuffer[j].end.y, usedTrailBuffer[j].end.z, 1.0f };
		vertices_[i + 1].uv = { uvOffset, 1.0f };

		// UV座標をずらす
		uvOffset += amount;
	}

	// インデックス配列のクリア
	indices_.clear();
	// インデックス配列の生成
	for (int i = 0; i < vertices_.size() / 2 - 1; i++) {
		// 頂点のインデックス
		uint32_t v0_start = static_cast<uint32_t>(i * 2);
		uint32_t v0_end = static_cast<uint32_t>(i * 2 + 1);
		uint32_t v1_start = static_cast<uint32_t>((i + 1) * 2);
		uint32_t v1_end = static_cast<uint32_t>((i + 1) * 2 + 1);
		
		// 1つ目の三角形 (v0_start, v0_end, v1_start)
		indices_.push_back(v0_start);
		indices_.push_back(v0_end);
		indices_.push_back(v1_start);

		// 2つ目の三角形 (v1_start, v0_end, v1_end)
		indices_.push_back(v1_start);
		indices_.push_back(v0_end);
		indices_.push_back(v1_end);
	}
}

void Line::TrailDraw()
{
	// 軌跡描画を行うかつバッファサイズが2以上のとき
	if (isDisplayTrail_ && trailBuffers_.size() > 2) {
		// 描画するマテリアルデータを取得
		MaterialData md = trailMaterial_;

		// バッファにデータを代入する
		*transformBuffer_->data_ = transform_.GetMatWorld();
		*materialBuffer_->data_ = md;
		std::memcpy(vertexBuffer_->data_, vertices_.data(), sizeof(TrailVertex) * vertices_.size());
		std::memcpy(indexBuffer_->data_, indices_.data(), sizeof(uint32_t) * indices_.size());

		// 軌跡描画レンダラーの取得
		TrailRenderer* r = DirectXCommon::GetInstance()->GetRendererManager()->GetTrailRenderer();

		r->AddDrawTask([this](ID3D12GraphicsCommandList6* cmdList) {
			// コマンドリストに情報を渡す
			cmdList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUView());	// トランスフォーム
			cmdList->SetGraphicsRootConstantBufferView(2, materialBuffer_->GetGPUView());	// マテリアル
			cmdList->SetGraphicsRootDescriptorTable(3, vertexBuffer_->GetGPUView());		// 頂点
			cmdList->SetGraphicsRootDescriptorTable(4, indexBuffer_->GetGPUView());			// インデックス

			// 描画を行う
			cmdList->DrawInstanced(UINT(6 * GetUsedTrailBuffer().size()), 1, 0, 0);
			});
	}
}

void Line::MakeCurveVertices(std::vector<TrailBuffer>& usedTrailBuffers)
{
	// 頂点座標が3未満、または曲線補間を行う設定でない場合早期リターン
	if (usedTrailBuffers.size() < 4 || trailInterpCount_ < 1) { return; }

	// 新規配列の生成
	std::vector<TrailBuffer> InterpTrailBuffers;

	// 最初の要素を配列に組み込む
	InterpTrailBuffers.push_back(usedTrailBuffers.front());
	for (int i = 1; i < usedTrailBuffers.size() - 2; i++) {
		// 制御点取得
		TrailBuffer& p0 = usedTrailBuffers[i - 1];
		TrailBuffer& p1 = usedTrailBuffers[i];
		TrailBuffer& p2 = usedTrailBuffers[i + 1];
		TrailBuffer& p3 = usedTrailBuffers[i + 2];

		// 分割数分スプライン曲線で補完する
		for (int j = 0; j <= trailInterpCount_; j++) {
			// 補間用tを求める
			float t = static_cast<float>(j) / static_cast<float>(trailInterpCount_);

			// スプライン曲線関数で補間
			TrailBuffer newTrailBuffer{};
			newTrailBuffer.start = CatmullRom(p0.start, p1.start, p2.start, p3.start, t);
			newTrailBuffer.end = CatmullRom(p0.end, p1.end, p2.end, p3.end, t);

			// 補間した頂点情報を追加
			InterpTrailBuffers.push_back(newTrailBuffer);
		}
	}

	// 配列の情報を代入する
	usedTrailBuffers = InterpTrailBuffers;
}

Vector3 Line::CatmullRom(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t)
{
	const float s = 0.5f;
	float t2 = t * t;
	float t3 = t2 * t;

	Vector3 e3 = (p0 * -1.0f) + (p1 * 3.0f) - (p2 * 3.0f) + p3;
	Vector3 e2 = (p0 * 2.0f) - (p1 * 5.0f) + (p2 * 4.0f) - p3;
	Vector3 e1 = (p0 * -1.0f) + p2;
	Vector3 e0 = p1 * 2.0f;
	return (e3 * t3 + e2 * t2 + e1 * t + e0) * s;
}

void Line::AddCollider(const std::string& name, IObject* object)
{
	if (transform_.GetParent() != nullptr) {
		object->AddColliderOBB(name, &transform_.scale_, &transform_.rotate_, &worldPos_);
	}
	else {
		object->AddColliderOBB(name, &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	}

	// 追加したコライダーを取得しておく
	collider_ = object->GetCollider(name);
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
