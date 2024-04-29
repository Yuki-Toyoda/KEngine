#include "TextureManager.h"
#include "../../Base/DirectXCommon.h"

TextureManager* TextureManager::GetInstance()
{
	// クラスのインスタンスを取得
	static TextureManager instance;
	// インスタンスを返す
	return &instance;
}

Texture TextureManager::Load(std::string fileName)
{
	return TextureManager::GetInstance()->LoadTexture(fileName);
}

Texture TextureManager::Load(std::string directoryPath, std::string fileName)
{
	return TextureManager::GetInstance()->LoadTexture(directoryPath, fileName);
}

void TextureManager::Init()
{
	// DirectX12汎用クラスのインスタンス取得
	dxCommon_ = DirectXCommon::GetInstance();
}

Texture TextureManager::LoadTexture(std::string fileName)
{
	// ロード
	return Load("./Resources", fileName);
}

Texture TextureManager::LoadTexture(std::string directoryPath, std::string fileName)
{
	// フルパスを生成
	std::string fullPath = directoryPath + "/" + fileName;

	// テクスチャコンテナ内に同名テクスチャが見つからなかった場合読み込む
	if (!textures_.count(fullPath)) {
		// 新しいテクスチャを生成
		textures_[fullPath] = TextureStruct();
		// テクスチャ読み込み
		textures_[fullPath].texResource_.Init(dxCommon_->GetDirectXDevice(), dxCommon_->GetHeaps(), fullPath);
		// GPU上のアドレスを取得
		textures_[fullPath].tex = textures_[fullPath].texResource_;
	}
	// テクスチャを返す
	return textures_[fullPath].tex;
}


