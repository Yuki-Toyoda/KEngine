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
	for (std::map<std::string, SkiningModel*>::const_iterator it = skiningModels_.cbegin(); it != skiningModels_.cend(); ++it) {
		it->second->isDestroy_ = true;
	}

	// モデルカウントリセット
	modelCount_ = 0;

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

	// モデル名の生成
	std::string modelName = "StageModel : " + std::to_string(modelCount_);
	// メッシュ追加
	AddNormalModel(modelName,transforms_.back().get(), filePath, fileName);
	// 追加次第カウント増加
	modelCount_++;
}
