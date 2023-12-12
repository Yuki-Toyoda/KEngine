#include "TextureManager.h"
#include "../../Base/DirectXCommon.h"

TextureManager* TextureManager::GetInstance()
{
	// クラスのインスタンスを取得
	static TextureManager instance;
	// インスタンスを返す
	return &instance;
}

Texture* TextureManager::Load(std::string fileName)
{
	return TextureManager::GetInstance()->LoadTexture(fileName);
}

Texture* TextureManager::Load(std::string directoryPath, std::string fileName)
{
	return TextureManager::GetInstance()->LoadTexture(directoryPath, fileName);
}

void TextureManager::Init()
{
	// DirectX12汎用クラスのインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();
}

Texture* TextureManager::LoadTexture(std::string fileName)
{
	// ロード
	return Load("./Resources", fileName);
}

Texture* TextureManager::LoadTexture(std::string directoryPath, std::string fileName)
{
	// フルパスを生成
	std::string fullPath = directoryPath + "/" + fileName;

	// テクスチャコンテナ内に同名テクスチャが見つからなかった場合生成
	if (!textures_.count(fullPath))
		textures_[fullPath] = std::make_unique<Texture>(dxCommon_->GetCommandManager(), fullPath);
	return textures_[fullPath].get();
}


