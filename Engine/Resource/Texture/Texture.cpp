#include "Texture.h"
#include "../../Base/Resource/Rendering/TextureResource.h"

Texture::Texture(const TextureResource& resource)
{
	// テクスチャリソースをそのまま代入
	*this = resource;
}

Texture& Texture::operator=(const TextureResource& resource)
{
	// リソースから各種情報を取得
	view_ = resource.GetTextureAddress(); // GPU上のアドレス
	size_ = resource.GetTextureSize();    // テクスチャ解像度
	
	// テクスチャを返す
	return *this;
}

