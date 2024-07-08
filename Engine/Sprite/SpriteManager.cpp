#include "SpriteManager.h"
#include "../Model/Data/ModelDataManager.h"
#include "../Resource/Texture/TextureManager.h"

SpriteManager* SpriteManager::GetInstance()
{
	static SpriteManager instance;
	return &instance;
}

void SpriteManager::Init()
{
	// スプライト用モデルが読み込まれていなければ
	if (modelData_ == nullptr) {
		// モデルデータの生成を行う
		modelData_ = std::make_unique<ModelData>();
		modelData_->Load("./Engine/Resource/Samples/Plane", "Plane.obj");

		if (modelData_->materials_[0].tex_.GetView() == -1) {
			modelData_->materials_[0].tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "uvChecker.png");
			modelData_->materials_[0].color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}

	// 全スプライトを破壊
	for (std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Destroy();
	}
	// 破壊フラグの立ったスプライトを削除
	sprites_.remove_if([](std::unique_ptr<Sprite>& sprite) {
		if (sprite->GetIsDestroy()) {
			return true;
		}
		return false;
	});
}

void SpriteManager::Update()
{
	// 破壊フラグの立ったスプライトを削除
	sprites_.remove_if([](std::unique_ptr<Sprite>& Sprite) {
		if (Sprite->GetIsDestroy()) {
			return true;
		}
		return false;
		});

	// 全スプライトを更新
	for (std::unique_ptr<Sprite>& Sprite : sprites_) {
		Sprite->Update();	 // 更新
	}
}

Sprite* SpriteManager::Create(const std::string& name)
{
	// インスタンス生成
	std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
	// 初期化
	newSprite->Init(name, modelData_.get());

	// 返還用インスタンスの生成
	Sprite* returnSprite = newSprite.get();

	// スプライトリストに追加
	sprites_.push_back(std::move(newSprite));

	// 生成したインスタンスを返す
	return returnSprite;
}

Sprite* SpriteManager::Create(const std::string& name, const Vector2 position, const Vector2& size, Texture texture)
{
	// インスタンス生成
	std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
	// 初期化
	newSprite->Init(name, modelData_.get(), position, size, texture);

	// 返還用インスタンスの生成
	Sprite* returnSprite = newSprite.get();

	// スプライトリストに追加
	sprites_.push_back(std::move(newSprite));

	// 生成したインスタンスを返す
	return returnSprite;
}
