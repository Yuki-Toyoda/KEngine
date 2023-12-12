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

	// コライダーの生成
	collider_ = std::make_unique<Collider>();

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

void BaseObject::DisplayImGui()
{
	// 基底クラスでは特に記述なし
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

void BaseObject::AddSprite(const std::string& name, const Vector2 position, const Vector2& size, Texture* texture)
{
	// 新しいインスタンスの追加
	Sprite* newSprite = SpriteManager::GetInstance()->Create(name, position, size, texture);
	// リストに追加
	sprites_.push_back(newSprite);
}

