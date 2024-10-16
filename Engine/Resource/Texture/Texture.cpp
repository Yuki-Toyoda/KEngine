#include "Texture.h"
#include "../../Base/Resource/Rendering/TextureResource.h"
#include "../../Base/Resource/Rendering/RenderResource.h"

Texture::Texture(const TextureResource& resource)
{
	// テクスチャリソースをそのまま代入
	*this = resource;
}

Texture::Texture(const RenderResource& resource)
{
	// レンダリングリソースをそのまま代入
	*this = resource;
}

Texture& Texture::operator=(const TextureResource& resource)
{
	// リソースから各種情報を取得
	view_	 = resource.GetSRVIndex();	     // GPU上のアドレスインデックス
	size_	 = resource.GetTextureSize();	 // テクスチャ解像度
	address_ = resource.GetTextureAddress(); // GPU上のアドレス 
	
	// テクスチャを返す
	return *this;
}

Texture& Texture::operator=(const RenderResource& resource)
{
	// リソースから各種情報を取得
	view_	 = resource.GetSRVIndex();		 // GPU上のアドレスインデックス
	size_	 = resource.GetTextureSize();	 // テクスチャ解像度
	address_ = resource.GetTextureAddress(); // GPU上のアドレス 

	// テクスチャを返す
	return *this;
}

