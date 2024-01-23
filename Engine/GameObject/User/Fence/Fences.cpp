#include "Fences.h"
#include "../../../Resource/Texture/TextureManager.h"

void Fences::Init()
{
	
}

void Fences::Update()
{
	// 配列番号記録用
	int c = 0;
	// 全てのメッシュのuvトランスフォームをスケールに合わせて変更する
	for (BasePrimitive* p : meshes_) {
		// uvトランスフォームを変更
		p->material_.uvTransform_.scale_ = { (fenceTransforms_[c]->scale_.x / 2.0f) / fenceTransforms_[c]->scale_.y,  1.0f, 1.0f};
		// 配列番号インクリメント
		c++;
	}
}

void Fences::DisplayImGui()
{
	// 位置座標のImGuiの表示
	transform_.DisplayImGui();

	// 配列番号記録用
	int c = 0;
	// uvトランスフォームの表示
	for (BasePrimitive* p : meshes_) {
		// 柵 + 配列番号で名前を求める
		std::string fenceName = "Fence" + std::to_string(c);
		// ツリーノード開始
		if (ImGui::TreeNode(fenceName.c_str())) {
			// トランスフォームの表示
			p->transform_->DisplayImGuiWithTreeNode("Fence");

			// uvTransformの表示
			p->material_.uvTransform_.DisplayImGuiWithTreeNode("uvTransform");

			// ツリーノード終了
			ImGui::TreePop();
		}
		// 配列番号インクリメント
		c++;
	}

	if (ImGui::Button("Add Fence")) {
		// 柵の追加
		Add(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f));
	}

}

void Fences::Add(const Vector3& translate, const Vector3& rotate, const Vector2& scale)
{
	// 新しいトランスフォームの生成
	std::unique_ptr<WorldTransform> newTransform = std::make_unique<WorldTransform>();
	// 生成したトランスフォームの初期化
	newTransform->Init();

	// 引数の値を生成したトランスフォームに渡す
	newTransform->translate_ = translate; // 座標
	newTransform->rotate_ = rotate; // 回転角
	newTransform->scale_ = Vector3(scale.x, scale.y, 1.0f); // 大きさ

	// 両平面を追加
	BasePrimitive* m = AddMesh(newTransform.get(), color_, "./Engine/Resource/Samples/BothPlane", "BothPlane.obj");
	// テクスチャ変更
	m->texture_ = TextureManager::Load("./Resources", "FenceTex.png");

	// 設定したトランスフォームを配列に追加
	fenceTransforms_.push_back(std::move(newTransform));
}

