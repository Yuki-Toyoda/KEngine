#include "Mesh.h"
#include "../../Resource/Texture/TextureManager.h"
#include "MeshManager.h"
#include "../../Base/Command/CommandManager.h"
#include "../../Base/DescriptorHeaps/SRV.h"

#include <map>
#include <fstream>
#include <sstream>

void Mesh::LoadFile(const std::string& filePath, const std::string& fileName)
{
	// 頂点情報とインデックス情報をクリア
	vertices_.clear(); // 頂点情報
	indexes_.clear();  // インデックス情報

	// objを読み込む
	LoadObj(filePath, fileName);
}

void Mesh::LoadObj(const std::string& filePath, const std::string& fileName)
{
	// フルパスのフルパスの合成
	std::string fullPath = filePath + "/" + fileName;
	
	// バッファリソースの生成
	meshletBuffer_ = std::make_unique<MeshletBuffer>();				  // メッシュレットバッファ
	vertexBuffer_ = std::make_unique<VertexBuffer>();				  // 頂点バッファ
	uniqueVertexBuffer_ = std::make_unique<UniqueVertexBuffer>();	  // 固有頂点バッファ
	primitiveIndexBuffer_ = std::make_unique<PrimitiveIndexBuffer>(); // プリミティブ頂点バッファ

	// インデックス情報を登録するための3次元配列
	std::map<int, std::map<int, std::map<int, int>>> key;

	// 頂点情報の格納ための変数生成
	std::vector<DirectX::XMFLOAT3> positions; // 位置
	std::vector<DirectX::XMFLOAT2> texcoords; // テクスチャ座標
	std::vector<DirectX::XMFLOAT3> normals;	  // 法線

	// キャッシュ保存用
	VertexCache vertexCache;

	// 頂点位置のインデックス情報格納用配列
	std::vector<uint32_t> verticesIndices;

	// ファイルを開く
	std::ifstream file(fullPath); // ファイルを開く
	// 開けなかった場合を止める
	assert(file.is_open());

	// ファイルから読んだ１行を格納するもの
	std::string line;

	// ファイルから1行ずつ読み込む
	while (std::getline(file, line)) {
		// 先端の識別子の値を格納する変数
		std::string identifier;
		// ファイルの１行を取得
		std::istringstream s(line);
		// 先頭の識別子を読む
		s >> identifier;

		// 頂点データ登録
		if (identifier == "v") {
			DirectX::XMFLOAT3 position;					 // 読み込み情報格納用
			s >> position.x >> position.y >> position.z; // その行から情報を読み込み
			positions.push_back(position);				 // 読み込んだ情報を配列を格納
		}
		// テクスチャ座標登録
		else if (identifier == "vt") {
			DirectX::XMFLOAT2 texcoord;	   // 読み込み情報格納用
			s >> texcoord.x >> texcoord.y; // その行から情報を読み込み
			texcoords.push_back(texcoord); // 読み込んだ情報を配列を格納
		}
		// 法線登録
		else if (identifier == "vn") {
			DirectX::XMFLOAT3 normal;			   // 読み込み情報格納用
			s >> normal.x >> normal.y >> normal.z; // その行から情報を読み込み
			normals.push_back(normal);			   // 読み込んだ情報を配列を格納
		}
		// 頂点とインデックス情報を登録
		else if (identifier == "f") {
			// 面のインデックス情報格納用配列
			uint32_t faceIndex[3];

			// 面は三角形限定であり、その他は対応していない
			for (int32_t faceVertex = 0; faceVertex < 3; faceVertex++) {
				std::string vertexDefinition; // 頂点インデックス情報取得用
				s >> vertexDefinition; // 現在の行の文字列を取得

				// 頂点要素へのインデックスは 位置 / UV / 法線 の順で登録されている
				// そのため、分解してIndexを取得する
				std::istringstream v(vertexDefinition); // 1頂点分のデータを取得
				uint32_t elements[3]; // 0 : 位置, 1 : UV, 2 : 法線
				for (int32_t i = 0; i < 3; i++) {
					std::string index;
					std::getline(v, index, '/'); // それぞれの要素を取得
					elements[i] = std::stoi(index);
				}

				// 代入用の変数定義
				VertexData vertex;

				/// インデックス情報を取得する
				uint32_t vertexIndex = 0;
				// インデックス値が0の場合エラー
				if (!elements[0]) {
					Debug::Log("index = 0");
					// 処理を停止させる
					assert(false);
				}
				else if (elements[0] < 0) {
					// インデックス値が0以下の際、相対的なインデックス値を求める
					vertexIndex = uint32_t(ptrdiff_t(positions.size()) + elements[0]);
				}
				else {
					// OBJのフォーマットの都合上、インデックス値は1から始まるので-1する
					vertexIndex = uint32_t(elements[0] - 1);
				}

				// インデックスが頂点数を超えていた時点でエラー
				if (vertexIndex >= positions.size()) {
					assert(false);
				}

				// 頂点位置を取得
				vertex.position = { positions[vertexIndex].x, positions[vertexIndex].y, positions[vertexIndex].z, 1.0f };

				/// テクスチャ座標のインデックス情報を取得する
				uint32_t coordIndex = 0;
				// インデックス値が0の場合エラー
				if (!elements[1]) {
					Debug::Log("index = 0");
					// 処理を停止させる
					assert(false);
				}
				else if (elements[1] < 0) {
					// インデックス値が0以下の際、相対的なインデックス値を求める
					coordIndex = uint32_t(ptrdiff_t(texcoords.size()) + elements[1]);
				}
				else {
					// OBJのフォーマットの都合上、インデックス値は1から始まるので-1する
					coordIndex = uint32_t(elements[1] - 1);
				}

				// インデックスがテクスチャ座標数を超えていた時点でエラー
				if (coordIndex >= texcoords.size()) {
					assert(false);
				}

				// テクスチャ座標を取得
				vertex.texCoord = texcoords[coordIndex];

				/// 法線のインデックス情報を取得する
				uint32_t normIndex = 0;
				// インデックス値が0の場合エラー
				if (!elements[2]) {
					Debug::Log("index = 0");
					// 処理を停止させる
					assert(false);
				}
				else if (elements[2] < 0) {
					// インデックス値が0以下の際、相対的なインデックス値を求める
					normIndex = uint32_t(ptrdiff_t(normals.size()) + elements[2]);
				}
				else {
					// OBJのフォーマットの都合上、インデックス値は1から始まるので-1する
					normIndex = uint32_t(elements[2] - 1);
				}

				// インデックスが法線数を超えていた時点でエラー
				if (normIndex >= normals.size()) {
					assert(false);
				}

				// テクスチャ座標を取得
				vertex.normal = normals[normIndex];

				// ハッシュ値と頂点のインデックス情報の関連付け用マップ
				using VertexCache = std::unordered_multimap<uint32_t, uint32_t>;
				
				// インデックス情報の取得
				const uint32_t vertIndex = AddVertex(vertexIndex, &vertex, vertexCache);
				// インデックス情報を追加出来なかった場合エラー
				if (vertIndex == uint32_t(-1)) {
					Debug::Log("AddVertex Failed");
					assert(false);
				}

				// インデックスの最大値求める
				constexpr uint32_t maxIndex = (sizeof(uint32_t) == 2) ? UINT16_MAX : UINT32_MAX;
				// 求めたインデックスが最大値を超過していた場合エラー
				if (vertIndex >= maxIndex)
				{
					Debug::Log("index >= maxIndex");
					assert(false);
				}

				// 面のインデックス取得
				faceIndex[faceVertex] = vertIndex;

				// 読み込んでいる面が3以下の場合はループを一度抜ける
				if (faceVertex != 2) {
					continue;
				}

				// Convert polygons to triangles
				uint32_t i0 = faceIndex[0];
				uint32_t i1 = faceIndex[1];
				uint32_t i2 = faceIndex[2];

				indexes_.push_back(static_cast<uint32_t>(i0));
				indexes_.push_back(static_cast<uint32_t>(i2));
				indexes_.push_back(static_cast<uint32_t>(i1));
			}
		}
		// マテリアル情報読み込み
		else if (identifier == "mtllib") {
			// マテリアル名を取得
			std::string materialFileName; // マテリアル名格納用
			s >> materialFileName;		  // マテリアル名取得
			// マテリアル読み込み
			LoadMaterial(filePath, materialFileName);
		}
	}

	// 頂点座標格納用配列の作成
	auto vertPos = std::make_unique<DirectX::XMFLOAT3[]>(vertices_.size());
	// 頂点座標を取得
	for (size_t j = 0; j < vertices_.size(); j++) {
		vertPos[j] = { vertices_[j].position.x, vertices_[j].position.y, vertices_[j].position.z };
	}

	// メッシュレットの変換成否確認用
	HRESULT result = S_FALSE;
	// 読み込んだモデルデータをメッシュレットに変換する
	result = DirectX::ComputeMeshlets(
		indexes_.data(), indexes_.size() / 3,
		vertPos.get(), vertices_.size(),
		nullptr,
		meshlets_,
		uniqueVertices_,
		primitiveIndices_
	);
	// 変換成否確認
	assert(SUCCEEDED(result));

	// 共通のSRV用Desc
	D3D12_SHADER_RESOURCE_VIEW_DESC commonDesc{};								   // 汎用設定用
	commonDesc.Format = DXGI_FORMAT_UNKNOWN;									   // フォーマット形式は不明
	commonDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // シェーダーからテクスチャにアクセスする際の値指定 
	commonDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;						   // SRVのバッファであることを指定
	commonDesc.Buffer.FirstElement = 0;											   // 最初の番号を指定
	commonDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;						   // フラッグ設定

	// メッシュレットバッファ
	meshletBuffer_->Resource = std::move(CreateBuffer(((sizeof(DirectX::Meshlet) + 0xff) & ~0xff) * meshlets_.size()));
	result = meshletBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&meshletBuffer_->meshlet));
	meshletBuffer_->index = cmdManager_->GetSRV()->UseEmpty();
	D3D12_SHADER_RESOURCE_VIEW_DESC meshletDesc = { commonDesc };
	meshletDesc.Buffer.NumElements = static_cast<UINT>(meshlets_.size());
	meshletDesc.Buffer.StructureByteStride = sizeof(DirectX::Meshlet);
	meshletBuffer_->View = cmdManager_->GetSRV()->GetGPUHandle(meshletBuffer_->index);
	cmdManager_->GetDevice()->CreateShaderResourceView(meshletBuffer_->Resource.Get(), &meshletDesc, cmdManager_->GetSRV()->GetCPUHandle(meshletBuffer_->index));
	// マッピングに失敗した場合
	if (FAILED(result)) {
		assert(false);
	}

	// 頂点バッファ
	vertexBuffer_->Resource = std::move(CreateBuffer(((sizeof(VertexData) + 0xff) & ~0xff) * vertices_.size()));
	result = vertexBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&vertexBuffer_->vertex));
	vertexBuffer_->index = cmdManager_->GetSRV()->UseEmpty();
	D3D12_SHADER_RESOURCE_VIEW_DESC vertexDesc = { commonDesc };
	vertexDesc.Buffer.NumElements = static_cast<UINT>(vertices_.size());
	vertexDesc.Buffer.StructureByteStride = sizeof(VertexData);
	vertexBuffer_->View = cmdManager_->GetSRV()->GetGPUHandle(vertexBuffer_->index);
	cmdManager_->GetDevice()->CreateShaderResourceView(vertexBuffer_->Resource.Get(), &vertexDesc, cmdManager_->GetSRV()->GetCPUHandle(vertexBuffer_->index));
	// マッピングに失敗した場合
	if (FAILED(result)) {
		assert(false);
	}

	// 固有頂点バッファ
	uniqueVertexBuffer_->Resource = std::move(CreateBuffer(((sizeof(uint32_t) + 0xff) & ~0xff) * indexes_.size()));
	result = uniqueVertexBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&uniqueVertexBuffer_->uniqueVertex));
	uniqueVertexBuffer_->index = cmdManager_->GetSRV()->UseEmpty();
	D3D12_SHADER_RESOURCE_VIEW_DESC uniqueVertexDesc = { commonDesc };
	uniqueVertexDesc.Buffer.NumElements = static_cast<UINT>(indexes_.size());
	uniqueVertexDesc.Buffer.StructureByteStride = sizeof(uint32_t);
	uniqueVertexBuffer_->View = cmdManager_->GetSRV()->GetGPUHandle(uniqueVertexBuffer_->index);
	cmdManager_->GetDevice()->CreateShaderResourceView(uniqueVertexBuffer_->Resource.Get(), &uniqueVertexDesc, cmdManager_->GetSRV()->GetCPUHandle(uniqueVertexBuffer_->index));
	// マッピングに失敗した場合
	if (FAILED(result)) {
		assert(false);
	}

	// プリミティブインデックスバッファ
	primitiveIndexBuffer_->Resource = std::move(CreateBuffer(((sizeof(uint32_t) + 0xff) & ~0xff) * primitiveIndices_.size()));
	result = primitiveIndexBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&primitiveIndexBuffer_->primitve));
	primitiveIndexBuffer_->index = cmdManager_->GetSRV()->UseEmpty();
	D3D12_SHADER_RESOURCE_VIEW_DESC primitiveIndexDesc = { commonDesc };
	primitiveIndexDesc.Buffer.NumElements = static_cast<UINT>(primitiveIndices_.size());
	primitiveIndexDesc.Buffer.StructureByteStride = sizeof(uint32_t);
	primitiveIndexBuffer_->View = cmdManager_->GetSRV()->GetGPUHandle(primitiveIndexBuffer_->index);
	cmdManager_->GetDevice()->CreateShaderResourceView(primitiveIndexBuffer_->Resource.Get(), &primitiveIndexDesc, cmdManager_->GetSRV()->GetCPUHandle(primitiveIndexBuffer_->index));
	// マッピングに失敗した場合
	if (FAILED(result)) {
		assert(false);
	}

	// メッシュレットバッファへのデータコピー
	std::memcpy(meshletBuffer_->meshlet, meshlets_.data(), sizeof(DirectX::Meshlet)* meshlets_.size());

	// 頂点バッファへのデータコピー
	std::memcpy(vertexBuffer_->vertex, vertices_.data(), sizeof(VertexData)* vertices_.size());

	// 固有頂点バッファへのデータコピー
	std::memcpy(uniqueVertexBuffer_->uniqueVertex, indexes_.data(), sizeof(uint8_t)* indexes_.size());

	// プリミティブインデックスバッファへのデータコピー
	std::memcpy(primitiveIndexBuffer_->primitve, primitiveIndices_.data(), sizeof(DirectX::MeshletTriangle)* primitiveIndices_.size());
}

void Mesh::LoadMaterial(const std::string& filePath, const std::string& fileName)
{
	// フルパスの合成
	std::string fullPath = filePath + "/" + fileName;
	// ファイルを開く
	std::ifstream file(fullPath); // ファイルを開く
	// 開けなかった場合を止める
	assert(file.is_open());

	// ファイルから読んだ１行を格納するもの
	std::string line;

	// ファイルから1行ずつ読み込む
	while (std::getline(file, line)) {
		// 先端の識別子の値を格納する変数
		std::string identifier;
		// ファイルの１行を取得
		std::istringstream s(line);
		// 先頭の識別子を読む
		s >> identifier;

		// テクスチャの読み込み
		if (identifier == "map_Kd") {
			// テクスチャ名取得
			std::string textureName; // テクスチャ情報格納用
			s >> textureName;		 // テクスチャ名取得
			// テクスチャ読み込み
			//texture_ = TextureManager::Load(filePath, textureName);
		}
	}
}
