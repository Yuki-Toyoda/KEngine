#include "CPUParticleEmitterManager.h"

void CPUParticleEmitterManager::Init()
{
	// エミッタ配列をクリア
	emitters_.clear();
}

void CPUParticleEmitterManager::Update()
{
	// 粒子が終了状態の時リストから除外
	emitters_.remove_if([](std::unique_ptr<ICPUParticleEmitter>& emitter) {
		if (emitter->GetIsEnd()) {
			// 粒子用モデルを削除
			emitter->model_->isDestroy_ = true;
			return true;
		}

		return false;
	});

	// 生成されている粒子の更新
	for (std::unique_ptr<ICPUParticleEmitter>& emitter : emitters_) {
		emitter->PreUpdate();	// 共通更新
		emitter->Update();	    // 更新
		emitter->PostUpdate();  // 更新後処理
	}
}

void CPUParticleEmitterManager::DisplayImGui()
{
#ifdef _DEBUG // デバッグ時のみImGuiを表示

	// ImGuiの開始
	ImGui::Begin("Particles");
	// 全パーティクルのImGuiを描画
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(350, 500), ImGuiWindowFlags_NoTitleBar);
	// パーティクルが1つでもあった場合
	if (emitters_.size() > 0) {
		for (std::unique_ptr<ICPUParticleEmitter>& p : emitters_) {
			p->DisplayImGui();
		}
	}
	else // 1つもパーティクルがない場合テキストで表示
		ImGui::Text("No Particle!");
	ImGui::EndChild();


	ImGui::End();

#endif // _DEBUG
}
