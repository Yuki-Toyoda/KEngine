#include "IPrimitive.h"
#include "../Base/Command/CommandManager.h"
#include "../GameObject/WorldTransform.h"

IPrimitive::IPrimitive(CommandManager* manager)
{
	// 基底クラスでは使用しない
	manager;
}

void IPrimitive::ResizeVertices()
{
	// 頂点配列を一度クリア
	vertices_.clear();
	// 頂点配列サイズをリサイズ
	vertices_.resize(GetVertexCount());
}

void IPrimitive::ResizeIndexes()
{
	// インデックス情報配列を一度クリア
	indexes_.clear();
	// インデックス情報配列をリサイズ
	indexes_.resize(GetIndexCount());

	// インデックス情報配列に頂点の数分の情報を追加
	for (int i = 0; i < GetVertexCount() - 2; i++) {
		indexes_[i * 3] = 0;	 // 1番目の頂点
		indexes_[i * 3 + 1] = i + 1; // 2番目の頂点
		indexes_[i * 3 + 2] = i + 2; // 3番目の頂点
	}
}

void IPrimitive::Draw()
{
	// 表示状態でなければ表示しない
	if (!isActive_)
		return;

	// メッシュレットバッファへのデータコピー
	std::memcpy(meshletBuffer_->meshlet, meshlets_.data(), sizeof(DirectX::Meshlet) * meshlets_.size());

	// 頂点バッファへのデータコピー
	std::memcpy(vertexBuffer_->vertex, vertices_.data(), sizeof(VertexData) * vertices_.size());

	// 固有頂点バッファへのデータコピー
	std::memcpy(uniqueVertexBuffer_->uniqueVertex, uniqueVertices_.data(), sizeof(uint8_t) * uniqueVertices_.size());

	// プリミティブインデックスバッファへのデータコピー
	std::memcpy(primitiveIndexBuffer_->primitve, primitiveIndices_.data(), sizeof(DirectX::MeshletTriangle) * primitiveIndices_.size());

	// コマンドリストの取得
	ID3D12GraphicsCommandList6* cmdList = cmdManager_->GetRenderCommandList();

	// コマンドリストにテーブルをセット
	cmdList->SetGraphicsRootDescriptorTable(1, meshletBuffer_->View);
	cmdList->SetGraphicsRootDescriptorTable(2, vertexBuffer_->View);
	cmdList->SetGraphicsRootDescriptorTable(3, uniqueVertexBuffer_->View);
	cmdList->SetGraphicsRootDescriptorTable(4, primitiveIndexBuffer_->View);

	// メッシュシェーダーを実行
	cmdList->DispatchMesh(GetMeshletCount(), 1, 1);
}

void IPrimitive::DisplayImGui()
{
	// メッシュ名を設定
	std::string meshName = name_ + " - Mesh";
	if (ImGui::TreeNode(meshName.c_str())) {
		// 頂点情報の表示
		if (ImGui::TreeNode("vertex")) {
			// 頂点をリストで表示する
			ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(0, 100), ImGuiWindowFlags_NoTitleBar);
			for (UINT i = 0; i < vertices_.size(); i++) {
				std::string name = "vertex" + std::to_string(i);
				ImGui::DragFloat3(name.c_str(), &vertices_[i].position.x, 0.05f);
			}
			ImGui::EndChild();
			ImGui::TreePop();
		}
		
		// 表示状態の設定
		ImGui::Checkbox("isActive", &isActive_);
		// マテリアルのuvトランスフォームを表示
		material_.uvTransform_.DisplayImGui("uvTransform");
		ImGui::Checkbox("isUI", &isUI_);
		if (commonColor != nullptr) {
			ImGui::ColorPicker4("Color", &commonColor->x);
		}

		ImGui::TreePop();
	}
}

ID3D12Resource* IPrimitive::CreateBuffer(size_t size)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 返すリソース
	ID3D12Resource* resource = nullptr;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = size; // リソースのサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	result = cmdManager_->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	// 生成したバッファリソースを返す
	return resource;
}

int IPrimitive::GetVertexCount() const
{
	return 3;
}

int IPrimitive::GetIndexCount() const
{
	return (GetVertexCount() - 2) * 3; // インデックス情報の数を求めて返す
}

int IPrimitive::GetMeshletCount() const
{
	return static_cast<int>(meshlets_.size());
}
