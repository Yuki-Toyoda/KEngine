#include "HeapManager.h"

HeapManager::HeapManager(DirectXDevice* device)
{
	/// 各種ヒープの生成を行う
	// RTVの生成
	rtv_ = std::make_unique<RTV>(device->GetDevice()); // 生成
	rtv_->Init();									   // 初期化
	// SRVの生成
	srv_ = std::make_unique<SRV>(device->GetDevice()); // 生成
	srv_->Init();									   // 初期化
	// DSVの生成
	dsv_ = std::make_unique<DSV>(device->GetDevice()); // 生成
	dsv_->Init();									   // 初期化
}
