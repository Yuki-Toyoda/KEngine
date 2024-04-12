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
			// 面は三角形限定。その他は未対応
			// 3頂点ループ
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);	// 1頂点分のデータ
				std::string elements[3];	// 0 → 位置 : 1 → UV : 2 → 法線
				for (int32_t i = 0; i < 3; ++i) {
					std::getline(v, elements[i], '/');
				}
				// 次の処理に移る前に、これが新しいパターンの頂点かチェック
				int value = key[std::stoi(elements[0])][std::stoi(elements[1])][std::stoi(elements[2])];
				// 既存のパターンだった場合 -> 既存の頂点のインデックスを求めてからインデックスに追加
				if (value > 0 && value < vertices_.size()) {
					indexes_.push_back(static_cast<uint32_t>(value - 1)); // パターンが見つかった場合のインデックス
				}
				// 新しいパターンの場合 -> 新しく頂点を追加し、インデックスに追加
				else {
					// 新しい頂点を生成
					VertexData newVertex;
					newVertex.position = { positions[std::stoi(elements[0]) - 1].x, positions[std::stoi(elements[0]) - 1].y, positions[std::stoi(elements[0]) - 1].z, 1.0f };
					newVertex.texCoord = texcoords[std::stoi(elements[1]) - 1];
					newVertex.normal = normals[std::stoi(elements[2]) - 1];
					// 左手座標系なので座標を変換
					newVertex.position.x *= -1.0f;
					newVertex.texCoord.y = 1.0f - newVertex.texCoord.y;
					newVertex.normal.x *= -1.0f;
					// 頂点追加
					vertices_.push_back(newVertex);
					// インデックスを登録
					indexes_.push_back(static_cast<uint32_t>(vertices_.size() - 1));

					// 既存のパターンであることを指すために追加
					key[std::stoi(elements[0])][std::stoi(elements[1])][std::stoi(elements[2])] = static_cast<int>(vertices_.size());
				}
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
