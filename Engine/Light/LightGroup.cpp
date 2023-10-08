#include "LightGroup.h"
#include "../Base/DirectXCommon.h"
#include <cassert>

using namespace Microsoft::WRL;

LightGroup* LightGroup::Create()
{
    // 3Dオブジェクトのインスタンス生成
    LightGroup* instance = new LightGroup();
    // 初期化
    instance->Initialize();

    // インスタンスを返す
    return instance;
}

Microsoft::WRL::ComPtr<ID3D12Resource> LightGroup::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 頂点リソース用のヒープを設定する
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース テクスチャの場合はまた別の設定を行う
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes; // リソースのサイズ　今回はVector4を3頂点分(怪しい)

	// バッファの場合これらを1にする
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;

	// バッファの場合これにする
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際に頂点リソースを作成
	ComPtr<ID3D12Resource>vertexResource = nullptr;
	result = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertexResource));

	assert(SUCCEEDED(result));

	return vertexResource;
}

void LightGroup::Initialize()
{
    // 標準ライト設定を行う
    DefaultLightSetting();

	// 定数バッファを生成する
	constBuff_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(ConstBufferData));

	// 結果確認用
	HRESULT	result = S_FALSE;
	// 定数バッファとデータをリンクさせる
	result = constBuff_->Map(0, nullptr, (void**)&constMap_);
	// リンクできたか確認
	assert(SUCCEEDED(result));

	// 定数バッファへデータを転送する
	TransferConstBuffer();

}

void LightGroup::Update()
{

	// 内容が変更された際にのみ更新する
	if (dirty_) {
		// 定数バッファへデータを転送する
		TransferConstBuffer();
		// ダーティーフラグをfalseに
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex)
{
	// 定数バッファビューをセット
	commandList->SetGraphicsRootConstantBufferView(
		rootParameterIndex, constBuff_->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	// 平行光源
	for (int i = 0; i < kDirectionalLight; i++) {
		// ライトが有効なら設定を転送する
		if (directionalLights_[i].GetActive()) {
			constMap_->directionalLights[i].active = true;
			constMap_->directionalLights[i].direction.x = directionalLights_[i].GetLightDirection().x;
			constMap_->directionalLights[i].direction.y = directionalLights_[i].GetLightDirection().y;
			constMap_->directionalLights[i].direction.z = directionalLights_[i].GetLightDirection().z;
			constMap_->directionalLights[i].color = directionalLights_[i].GetColor();
			constMap_->directionalLights[i].intensity = directionalLights_[i].GetIntensity();
		}
		// ライト無効時はライト無効
		else {
			constMap_->directionalLights[i].active = false;
		}
	}
}

void LightGroup::DefaultLightSetting()
{
	directionalLights_[0].SetActive(true);
	directionalLights_[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	directionalLights_[0].SetLightDirection({ 0.0f, -1.0f, 0.0f});
}

void LightGroup::SetDirectionalLightActive(int index, bool active)
{
	// 配列番号の範囲外を選択した場合エラー
	assert(0 <= index && index < kDirectionalLight);
	// 有効状態をセット
	directionalLights_[index].SetActive(active);
}

void LightGroup::SetDirectionalLightDirection(int index, Vector3 direction)
{
	// 配列番号の範囲外を選択した場合エラー
	assert(0 <= index && index < kDirectionalLight);
	// 角度をセット
	directionalLights_[index].SetLightDirection(direction);
	// 値が変更されたことを通知
	dirty_ = true;
}

void LightGroup::SetDirectionalLightColor(int index, Vector4 color)
{
	// 配列番号の範囲外を選択した場合エラー
	assert(0 <= index && index < kDirectionalLight);
	// 色をセット
	directionalLights_[index].SetLightColor(color);
	// 値が変更されたことを通知
	dirty_ = true;
}

void LightGroup::SetDirectionalLightIntensity(int index, float intensity)
{
	// 配列番号の範囲外を選択した場合エラー
	assert(0 <= index && index < kDirectionalLight);
	// 輝度をセット
	directionalLights_[index].SetIntensity(intensity);
	// 値が変更されたことを通知
	dirty_ = true;
}
