#include "BaseObject.h"

BaseObject::~BaseObject()
{
	// 全形状を削除
	for (BasePrimitive* primitive : meshes_)
		primitive->isDestroy_ = true;
	// 全スプライトを削除
	for (Sprite* sprite : sprites_)
		sprite->Destroy();
}

void BaseObject::PreInitialize(std::string name, Tag tag)
{

	// インスタンス取得
	collisionManager_ = CollisionManager::GetInstance(); // 衝突判定マネージャ

	// 非表示
	isActive_ = false;

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

void BaseObject::PreUpdate()
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

void BaseObject::PostUpdate()
{
	// 有効かされているコライダーを登録
	for (std::unique_ptr<Collider>& collider : colliders_) {
		if (collider->GetIsActive()) {
			collisionManager_->RegisterCollider(collider.get());
		}
	}
}

void BaseObject::DisplayImGui()
{
	// トランスフォーム内の情報を表示
	transform_.DisplayImGui();

	// メッシュ情報があれば
	if (meshes_.size() > 0) {
		ImGui::Text("Meshes");
		ImGui::BeginChild("Meshes", ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
		for (BasePrimitive* p : meshes_) {
			p->DisplayImGui();
		}
		ImGui::EndChild();	
	}

	// スプライト情報があれば
	if (sprites_.size() > 0) {
		ImGui::Text("Sprites");
		ImGui::BeginChild("Sprites", ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
		for (Sprite* s : sprites_) {
			s->DisplayImGui();
		}
		ImGui::EndChild();
	}

	// スプライト情報があれば
	if (colliders_.size() > 0) {
		ImGui::Text("Colliders");
		ImGui::BeginChild("Colliders", ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
		for (std::unique_ptr<Collider>& c : colliders_) {
			c->DisplayImGui();
		}
		ImGui::EndChild();
	}
}

void BaseObject::AddMesh(WorldTransform* wt, Vector4& color, const std::string& path, const std::string& fileName, bool enableLighting)
{
	// 形状マネージャのインスタンスが取得されていない場合ここで取得
	if (primitiveManager_ == nullptr)
		primitiveManager_ = PrimitiveManager::GetInstance();

	// メッシュのインスタンスを生成
	Mesh* newMesh = primitiveManager_->CreateInstance<Mesh>(); // インスタンス生成
	newMesh->name_ = fileName;								   // 1メッシュ名をファイル名に
	newMesh->LoadFile(path, fileName);						   // モデルを読み込み
	newMesh->transform_ = wt;								   // ワールドトランスフォームを与える
	newMesh->commonColor = &color;							   // 色を設定
	newMesh->material_.enableLighting_ = enableLighting;	   // ライティングの有効設定
	newMesh->layerNo_ = 1;

	// メッシュリストに生成メッシュを追加
	meshes_.push_back(newMesh);
}

void BaseObject::AddSprite(const std::string& name, const Vector2& position, const Vector2& size, Texture* texture)
{
	// 新しいインスタンスの追加
	Sprite* newSprite = SpriteManager::GetInstance()->Create(name, position, size, texture);
	// リストに追加
	sprites_.push_back(newSprite);
}

void BaseObject::AddColliderSphere(const std::string& name, Vector3* center, float* radius, bool enable)
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

void BaseObject::AddColliderAABB(const std::string& name, Vector3* center, Vector3* size, bool enable)
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

void BaseObject::DeleteCollider(const std::string& name)
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

