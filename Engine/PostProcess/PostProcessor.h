#pragma once
#include "../Base/RootSignature/IRootSignature.h"
#include "../Base/PSO/PSO.h"

#include "IPostProcess.h"
#include "GrayScale/GrayScale.h"

#include<fstream>

/// <summary>
/// ポストプロセス管理クラス
/// </summary>
class PostProcessor final
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// シェーダーファイル生成関数
	/// </summary>
	void CreateShaderFile();

	/// <summary>
	/// 事前処理関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="target">ポストプロセス対象リソース</param>
	void PreCommands(ID3D12GraphicsCommandList6* cmdList, RenderResource* target);

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	///	コマンドリストにアドレスをセットする
	/// </summary>
	/// <param name="list">セットする対象のコマンドリスト</param>
	void SetToCommandList(ID3D12GraphicsCommandList6* list);

	/// <summary>
	/// 全ポストプロセスのゲッター
	/// </summary>
	/// <returns>全ポストプロセスを格納下配列</returns>
	std::vector<IPostProcess*> GetAllProcess();

public: // パブリックメンバ変数

	// ポストプロセス変数群
	GrayScale grayScale_; // グレースケール

private: // メンバ変数

	// ポストプロセス用ルートシグネチャ
	IRootSignature root_{};

	// ポストプロセス用PSO
	PSO pso_{};

	// 適用するポストプロセス格納配列
	std::vector<IPostProcess*> processes_;

};

