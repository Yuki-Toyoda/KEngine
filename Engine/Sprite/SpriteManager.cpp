#include "SpriteManager.h"

SpriteManager* SpriteManager::GetInstance()
{
	static SpriteManager instance;
	return &instance;
}

void SpriteManager::Initialize()
{
	// 全スプライトを破壊
	for (std::unique_ptr<Sprite>& sprite : sprites_)
		sprite->Destroy();
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
	newSprite->Initialize(name);

	// 返還用インスタンスの生成
	Sprite* returnSprite = newSprite.get();

	// スプライトリストに追加
	sprites_.push_back(std::move(newSprite));

	// 生成したインスタンスを返す
	return returnSprite;
}

Sprite* SpriteManager::Create(const std::string& name, const Vector2 position, const Vector2& size, Texture* texture)
{
	// インスタンス生成
	std::unique_ptr<Sprite> newSprite = std::make_unique<Sprite>();
	// 初期化
	newSprite->Initialize(name, position, size, texture);

	// 返還用インスタンスの生成
	Sprite* returnSprite = newSprite.get();

	// スプライトリストに追加
	sprites_.push_back(std::move(newSprite));

	// 生成したインスタンスを返す
	return returnSprite;
}
