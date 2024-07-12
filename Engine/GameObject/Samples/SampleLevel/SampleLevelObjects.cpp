#include "SampleLevelObjects.h"

void SampleLevelObjects::Init()
{

}

void SampleLevelObjects::Update()
{

}

void SampleLevelObjects::DisplayImGui()
{
	// 全トランスフォームのImGuiを表示
	for (int count = 0; count < transforms_.size(); count++) {
		std::string name = "Object : " + std::to_string(count);

		// ツリーノード開始
		if (ImGui::TreeNode(name.c_str())) {
			transforms_[count]->DisplayImGui("Transform");

			ImGui::TreePop();
		}
	}
	
	// ファイルパスとファイル名の取得
	ImGui::InputText("FilePath", imGuiFilePath_, sizeof(imGuiFilePath_));
	ImGui::InputText("FileName", imGuiFileName_, sizeof(imGuiFileName_));

	// ボタンを押すとレベルをリロード
	if (ImGui::Button("ReloadLevel")) {
		// レベルのロードを行う
		LoadLevel(imGuiFilePath_, imGuiFileName_);
	}
}

void SampleLevelObjects::LoadLevel(const std::string& filePath, const std::string& fileName)
{
	// 全通常モデルを削除
	for (NormalModel* normalModel : normalModels_) {
		normalModel->isDestroy_ = true;
	}

	// 読み込み済みモデルのクリア
	normalModels_.clear();

	// 配列のクリア
	transforms_.clear();

	// レベルデータのロードを行う
	loader_.Load(this, filePath, fileName);
}

void SampleLevelObjects::AddObjects(const std::string filePath, const std::string fileName, WorldTransform t)
{
	std::unique_ptr<WorldTransform> newT = std::make_unique<WorldTransform>();
	newT->Init();
	*newT = t;

	// 配列に追加
	transforms_.push_back(std::move(newT));

	// メッシュ追加
	AddNormalModel(transforms_.back().get(), filePath, fileName);
}
