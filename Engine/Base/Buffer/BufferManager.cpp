#include "BufferManager.h"

void BufferManager::Init(DirectXDevice* device, SRV* srv)
{
	/// 共通データバッファの生成
	// バッファ生成
	commonBuffer_ = std::make_unique<ConstantBuffer<CommonData>>();
	// 生成したバッファの初期化
	commonBuffer_->Init(device);
	// 二次元用ViewProjection行列の設定
	commonBuffer_->data_->ViewProjection2D = Matrix4x4() * Matrix4x4::MakeOrthGraphic(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 100.0f);

	// デバイスを保持
	device_ = device;
	// SRVを保持
	srv_ = srv;
}

void BufferManager::Reset()
{
	// 全バッファの情報をクリアする
	meshletsBuffers_.clear();			 // メッシュレット情報
	verticesBuffers_.clear();			 // 頂点情報
	uniqueVertexIndicesBuffers_.clear(); // 固有頂点インデックス
	primitiveIndicesBuffers_.clear();	 // プリミティブインデックス
}

uint32_t BufferManager::GetMeshletCounts(const uint32_t& meshletNum)
{
	// 指定されたインデックスのメッシュレット数を返す
	return meshletsBuffers_[meshletNum].get()->GetMaxSize();
}

void BufferManager::SetCommonDataView(int num, ID3D12GraphicsCommandList6* list)
{
	// コマンドリストにバッファのアドレスをセットする
	list->SetGraphicsRootConstantBufferView(num, commonBuffer_->GetGPUView());
}

void BufferManager::SetMeshletDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list)
{
	// コマンドリストにバッファのアドレスをセットする
	list->SetGraphicsRootDescriptorTable(num, meshletsBuffers_[meshletNum]->GetGPUView());
}

void BufferManager::SetVertexDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list)
{
	// コマンドリストにバッファのアドレスをセットする
	list->SetGraphicsRootDescriptorTable(num, verticesBuffers_[meshletNum]->GetGPUView());
}

void BufferManager::SetUniqueVertexIndicesDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list)
{
	// コマンドリストにバッファのアドレスをセットする
	list->SetGraphicsRootDescriptorTable(num, uniqueVertexIndicesBuffers_[meshletNum]->GetGPUView());
}

void BufferManager::SetPrimitiveIndicesDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list)
{
	// コマンドリストにバッファのアドレスをセットする
	list->SetGraphicsRootDescriptorTable(num, primitiveIndicesBuffers_[meshletNum]->GetGPUView());
}

void BufferManager::AddMeshletsData(const std::vector<DirectX::Meshlet>& meshlets)
{
	// 新しいデータを元に構造体バッファを生成する
	std::unique_ptr<StructuredBuffer<DirectX::Meshlet>> newBuffer = std::make_unique<StructuredBuffer<DirectX::Meshlet>>(meshlets.size());
	// 生成されたバッファを初期化する
	newBuffer->Init(device_, srv_);

	// 全メッシュレットのデータを構造体バッファにコピーする
	std::memcpy(newBuffer->data_, meshlets.data(), sizeof(DirectX::Meshlet) * meshlets.size());

	// 生成したバッファを配列に生成する
	meshletsBuffers_.push_back(newBuffer);
}

void BufferManager::AddVertexData(const std::vector<VertexData>& vertices)
{
	// 新しいデータを元に構造体バッファを生成する
	std::unique_ptr<StructuredBuffer<VertexData>> newBuffer = std::make_unique<StructuredBuffer<VertexData>>(vertices.size());
	// 生成されたバッファを初期化する
	newBuffer->Init(device_, srv_);

	// 全頂点のデータを構造体バッファにコピーする
	std::memcpy(newBuffer->data_, vertices.data(), sizeof(VertexData) * vertices.size());

	// 生成したバッファを配列に生成する
	verticesBuffers_.push_back(newBuffer);
}

void BufferManager::AddUniqueVertexIndicesData(const std::vector<uint8_t>& uniqueVertexIndices)
{
	// 新しいデータを元に構造体バッファを生成する
	std::unique_ptr<StructuredBuffer<uint32_t>> newBuffer = std::make_unique<StructuredBuffer<uint32_t>>(uniqueVertexIndices.size());
	// 生成されたバッファを初期化する
	newBuffer->Init(device_, srv_);

	// 全固有頂点インデックスのデータを構造体バッファにコピーする
	std::memcpy(newBuffer->data_, uniqueVertexIndices.data(), sizeof(uint8_t) * uniqueVertexIndices.size());

	// 生成したバッファを配列に生成する
	uniqueVertexIndicesBuffers_.push_back(newBuffer);
}

void BufferManager::AddPrimitiveIndicesData(const std::vector<DirectX::MeshletTriangle>& primitiveIndices)
{
	// 新しいデータを元に構造体バッファを生成する
	std::unique_ptr<StructuredBuffer<uint32_t>> newBuffer = std::make_unique<StructuredBuffer<uint32_t>>(primitiveIndices.size());
	// 生成されたバッファを初期化する
	newBuffer->Init(device_, srv_);

	// 全プリミティブインデックスのデータを構造体バッファに追加する
	std::memcpy(newBuffer->data_, primitiveIndices.data(), sizeof(DirectX::MeshletTriangle) * primitiveIndices.size());

	// 生成したバッファを配列に生成する
	primitiveIndicesBuffers_.push_back(newBuffer);
}
