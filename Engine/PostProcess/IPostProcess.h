#pragma once
#include "../Base/RootSignature/IRootSignature.h"
#include "../Base/Command.h"
#include "../Base/Resource/Data/ConstantBuffer.h"
#include "../Base/Resource/Rendering/RenderResource.h"
#include "../../Externals/imgui/imgui.h"

#include <string>
#include <fstream>

/// <summary>
/// ポストプロセス基底クラス
/// </summary>
class IPostProcess
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IPostProcess() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IPostProcess() = default;

public: // 純粋仮想関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// バインド書き込み関数
	/// </summary>
	/// <param name="stream">シェーダーファイル本体</param>
	/// <param name="root">ルートシグネチャ</param>
	/// <param name="i">バインド番号</param>
	virtual void WriteBinding(std::ofstream* stream, IRootSignature* root, int* i) { stream; root; i; };

	/// <summary>
	/// main関数内の処理を記述する関数
	/// </summary>
	/// <param name="stream">シェーダーファイル本体</param>
	virtual void WriteProcess(std::ofstream* stream) = 0;

	/// <summary>
	/// 事前処理関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	/// <param name="target">ポストプロセス対象のリソース</param>
	virtual void PreCommand(ID3D12GraphicsCommandList6* list, RenderResource* target) { list, target; };

	/// <summary>
	/// コマンドリストに生成したリソースをセットする関数
	/// </summary>
	/// <param name="cmdList">セットするコマンドリスト</param>
	/// <param name="count">セットするバインド番号</param>
	virtual void SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset) { cmdList, offset; };

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui() = 0;

public: // パブリックメンバ変数

	// ポストプロセス使用フラグ
	bool use_ = false;

	// ルートシグネチャのバインド番号
	int bindIndex_ = -1;
};
