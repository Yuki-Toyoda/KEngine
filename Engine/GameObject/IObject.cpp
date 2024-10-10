#include "IObject.h"

void IObject::PreInitialize(std::string name, Tag tag)
{
	// インスタンス取得
	modelDataManager_ = ModelDataManager::GetInstance(); // モデルデータマネージャー
	modelManager_	  = ModelManager::GetInstance();	 // モデルマネージャー
	collisionManager_ = CollisionManager::GetInstance(); // 衝突判定マネージャ

	// 非表示
	isActive_ = true;

	// ワールドトランスフォームの初期化
	transform_.Init();

	// モデルリストクリア
	normalModels_.clear();
	skiningModels_.clear();

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

void IObject::DisplayImGui()
{
	// 表示状態の切り替え
	ImGui::Checkbox("isActive", &isActive_);

	// トランスフォーム内の情報を表示
	transform_.DisplayImGui();

	// 全通常モデルのImGuiを描画
	for (std::map<std::string, NormalModel*>::const_iterator it = normalModels_.cbegin(); it != normalModels_.cend(); ++it) {
		it->second->DisplayImGui();
	}

	// 全スキニングモデルのImGuiを描画
	for (std::map<std::string, SkiningModel*>::const_iterator it = skiningModels_.cbegin(); it != skiningModels_.cend(); ++it) {
		it->second->DisplayImGui();
	}
}

void IObject::AddNormalModel(const std::string& name, WorldTransform* wt, const std::string& path, const std::string& fileName, bool enableLighting)
{
	// モデルマネージャのインスタンスが取得されていない場合ここで取得
	if (modelManager_ == nullptr) {
		modelManager_ = ModelManager::GetInstance();
	}

	// 新規モデルの生成
	normalModels_[name] = modelManager_->CreateNormalModel(path, fileName);
	normalModels_[name]->transform_.SetParent(wt);
	if (!enableLighting) {
		// 全マテリアルのライティングを設定
		for (Material& m : normalModels_[name]->materials_) {
			m.enableLighting_ = false;
		}
	}
}

void IObject::AddSkiningModel(const std::string& name, WorldTransform* wt, const std::string& path, const std::string& fileName, bool enableLighting)
{
	// モデルマネージャのインスタンスが取得されていない場合ここで取得
	if (modelManager_ == nullptr) {
		modelManager_ = ModelManager::GetInstance();
	}

	// 新規モデルの生成
	skiningModels_[name] = modelManager_->CreateSkiningModel(path, fileName);
	skiningModels_[name]->transform_.SetParent(wt);
	if (!enableLighting) {
		// 全マテリアルのライティングを設定
		for (Material& m : skiningModels_[name]->materials_) {
			m.enableLighting_ = false;
		}
	}

}

void IObject::AddSprite(const std::string& name, const Vector2& position, const Vector2& size, Texture texture)
{
	// 新しいインスタンスの追加
	sprites_[name] = SpriteManager::GetInstance()->Create(name, position, size, texture);
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

