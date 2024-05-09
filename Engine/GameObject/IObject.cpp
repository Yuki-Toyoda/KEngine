#include "IObject.h"

IObject::~IObject()
{
	// 全形状を削除
	for (IPrimitive* primitive : meshes_)
		primitive->isDestroy_ = true;
	// 全スプライトを削除
	for (Sprite* sprite : sprites_)
		sprite->Destroy();
}

void IObject::PreInitialize(std::string name, Tag tag)
{
	// インスタンス取得
	collisionManager_ = CollisionManager::GetInstance(); // 衝突判定マネージャ

	// 非表示
	isActive_ = true;

	// ワールドトランスフォームの初期化
	transform_.Init();

	// メッシュリストクリア
	meshes_.clear();

	// タグの初期化
	tag_ = tag;
	// 名前の初期化
	name_ = name;

	// デストロイトリガーFalse
	isDestroy_ = false;

	// 色を設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 初期化を呼び出す
	Init();
}

void IObject::PreUpdate()
{
	// 回転角のリセット
	if (transform_.rotate_.x >= (float)std::numbers::pi * 2.0f) {
		transform_.rotate_.x -= (float)std::numbers::pi * 2.0f;
	}
	if (transform_.rotate_.x <= -(float)std::numbers::pi * 2.0f) {
		transform_.rotate_.x += (float)std::numbers::pi * 2.0f;
	}

	if (transform_.rotate_.y >= (float)std::numbers::pi * 2.0f) {
		transform_.rotate_.y -= (float)std::numbers::pi * 2.0f;
	}
	if (transform_.rotate_.y <= -(float)std::numbers::pi * 2.0f) {
		transform_.rotate_.y += (float)std::numbers::pi * 2.0f;
	}

	if (transform_.rotate_.z >= (float)std::numbers::pi * 2.0f) {
		transform_.rotate_.z -= (float)std::numbers::pi * 2.0f;
	}
	if (transform_.rotate_.z <= -(float)std::numbers::pi * 2.0f) {
		transform_.rotate_.z += (float)std::numbers::pi * 2.0f;
	}

	if (!isActive_) {
		for (IPrimitive* m : meshes_) {
			m->isActive_ = false;
		}
	}
	else {
		for (IPrimitive* m : meshes_) {
			m->isActive_ = true;
		}
	}
}

void IObject::PostUpdate()
{
	// 有効化されているコライダーを登録
	for (std::unique_ptr<Collider>& collider : colliders_) {
		if (collider->GetIsActive()) {
			collisionManager_->RegisterCollider(collider.get());
		}
	}
}

void IObject::AnimUpdate()
{
	// 全アニメーション分ループ
	for (int i = 0; i < transform_.animations_.size(); i++) {
		// 再生中のアニメーションがある場合
		if (transform_.animations_[i].isPlay) {
			// 60FPS固定でアニメーションの秒数加算
			transform_.animations_[i].animationTime += 1.0f / 60.0f;

			// ループ再生を行う場合
			if (transform_.animations_[i].isLoop) {
				// 最終秒数に到達していた場合、最初の秒数に戻す
				transform_.animations_[i].animationTime = std::fmod(transform_.animations_[i].animationTime, transform_.animations_[i].duration);
			}

			// rootNodeのアニメーションを取得
			NodeAnimation& rootNodeAnimation = transform_.animations_[i].nodeAnimations[transform_.rootNode_.name];
			// 各要素の値を取得
			Vector3    translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, transform_.animations_[i].animationTime); // 位置座標
			Quaternion rotate = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, transform_.animations_[i].animationTime);	  // 回転
			Vector3    scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, transform_.animations_[i].animationTime);	  // 拡縮

			// ローカル行列を求める
			localMat_ = Quaternion::MakeAffineMatrix(scale, rotate, translate);

			// ローカル行列をセット
			transform_.SetLocalMat(localMat_);
		}
	}
}

void IObject::DisplayImGui()
{
	// 表示状態の切り替え
	ImGui::Checkbox("isActive", &isActive_);

	// トランスフォーム内の情報を表示
	transform_.DisplayImGui();

}

void IObject::AddMesh(WorldTransform* wt, Vector4& color, const std::string& path, const std::string& fileName, bool enableLighting)
{
	// 形状マネージャのインスタンスが取得されていない場合ここで取得
	if (primitiveManager_ == nullptr)
		primitiveManager_ = PrimitiveManager::GetInstance();

	// メッシュのインスタンスを生成
	Mesh* newMesh = primitiveManager_->CreateInstance<Mesh>(); // インスタンス生成
	newMesh->name_ = fileName;								   // メッシュ名をファイル名に
	newMesh->transform_ = wt;								   // ワールドトランスフォームを与える
	newMesh->LoadModelFile(path, fileName);					   // モデルを読み込み
	newMesh->commonColor = &color;							   // 色を設定
	newMesh->material_->enableLighting_ = enableLighting;	   // ライティングの有効設定
	newMesh->layerNo_ = 1;
	// メッシュリストに生成メッシュを追加
	meshes_.push_back(newMesh);
}

void IObject::AddSprite(const std::string& name, const Vector2& position, const Vector2& size, Texture* texture)
{
	// 新しいインスタンスの追加
	Sprite* newSprite = SpriteManager::GetInstance()->Create(name, position, size, texture);
	// リストに追加
	sprites_.push_back(newSprite);
}

void IObject::AddColliderSphere(const std::string& name, Vector3* center, float* radius, bool enable)
{
	// 新しいコライダーを生成する
	std::unique_ptr<Collider> newCollider = std::make_unique<Collider>();
	// 形状生成
	std::unique_ptr<Sphere> shape = std::make_unique<Sphere>();
	shape->Init(center, radius);
	// コライダー初期化
	newCollider->Init(name, std::move(shape));
	// コライダー有効状態設定
	newCollider->SetIsActive(enable);
	// 自身を設定
	newCollider->SetGameObject(this);
	// 生成したコライダーをリストに追加
	colliders_.push_back(std::move(newCollider));
}

void IObject::AddColliderAABB(const std::string& name, Vector3* center, Vector3* size, bool enable)
{
	// 新しいコライダーを生成する
	std::unique_ptr<Collider> newCollider = std::make_unique<Collider>();
	// 形状生成
	std::unique_ptr<AABB> shape = std::make_unique<AABB>();
	shape->Init(center, size);
	// コライダー初期化
	newCollider->Init(name, std::move(shape));
	// コライダー有効状態設定
	newCollider->SetIsActive(enable);
	// 自身を設定
	newCollider->SetGameObject(this);
	// 生成したコライダーをリストに追加
	colliders_.push_back(std::move(newCollider));
}

void IObject::AddColliderOBB(const std::string& name, Vector3* scale, Vector3* rotate, Vector3* translate, bool enable)
{
	// 新しいコライダーを生成する
	std::unique_ptr<Collider> newCollider = std::make_unique<Collider>();
	// 形状生成
	std::unique_ptr<OBB> shape = std::make_unique<OBB>();
	shape->Init(scale, rotate, translate);
	// コライダー初期化
	newCollider->Init(name, std::move(shape));
	// コライダー有効状態設定
	newCollider->SetIsActive(enable);
	// 自身を設定
	newCollider->SetGameObject(this);
	// 生成したコライダーをリストに追加
	colliders_.push_back(std::move(newCollider));
}

void IObject::DeleteCollider(const std::string& name)
{
	// 全てのコライダーから一致するコライダーを検索
	colliders_.remove_if([name](std::unique_ptr<Collider>& collider) {
		// 一致する場合は削除する
		if (collider->GetColliderName() == name) {
			return true;
		}
		return false;
		});
}
