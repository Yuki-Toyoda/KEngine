#include "SampleBox.h"
#include "../../../Resource/Texture/TextureManager.h"

void SampleBox::Init() {

	/*=======================================================
		初期化です。メッシュ読み込み、当たり判定の初期化等はここで。
	=========================================================*/

	/// ~メッシュの追加~
	// オブジェクトにメッシュを追加したい場合、下記のAddMesh関数を呼び出してください
	// 引数については関数の定義から参照してください
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	/// ~当たり判定の設定~
	// 当たり判定の設定については以下の手順で行ってください
	// 1. aabb、もしくは sphere のインスタンスを生成
	// 2. 初期化関数を呼び出して初期化(引数は関数の定義を参照)
	// 3. メンバ変数のコライダーに生成したaabb または sphereをセット
	// 4. コライダーに自分自身を登録
	AABB* aabb = new AABB(); // 1
	aabb->Init(transform_.GetWorldPos(), transform_.scale_); // 2
	collider_->SetColliderShape(aabb); // 3
	collider_->SetGameObject(this); // 4
}

void SampleBox::Update() {

	/*=======================================================
		更新処理です。毎フレーム行いたい処理はここに記述してください。
	=========================================================*/

	// 回転させる
	transform_.rotate_.y += 0.01f;

	/// 以下の処理は必ず更新処理の最後に呼び出してください。
	// コライダーの更新
	collider_->Update(transform_.GetWorldPos(), transform_.scale_);
	// リストに自分自身を登録
	collisionManager_->RegisterCollider(collider_.get());

}

void SampleBox::DisplayImGui() {

	/*========================================================
		ImGuiを表示させたい場合、この関数内に処理を記述してください。
		Begin, Endは不要です。下記のように記述すれば基本動きます
	==========================================================*/

	// ワールド座標の表示
	transform_.DisplayImGui();
	// テキスト表示
	ImGui::Text("Color");
	//　色の変更
	ImGui::ColorPicker4("Color", &color_.x);

	// メッシュに関する設定
	meshes_[0]->DisplayImGui(); // メッシュの基本情報
	ImGui::SliderInt("LayerNo", &meshes_[0]->layerNo_, 0, 10); // レイヤー番号
	ImGui::Checkbox("isUI", &meshes_[0]->isUI_); // UIとして描画するか
	ImGui::Checkbox("isActive", &meshes_[0]->isActive_); // 表示と非表示

	ImGui::SliderInt("BlendMode", &imGuiSelectBlendMode_, 0, 4); // ブレンドモードの設定
	meshes_[0]->blendMode_ = (BasePrimitive::kBlendMode)imGuiSelectBlendMode_; // ブレンドモードの適用

	ImGui::Checkbox("isBillBoard", &meshes_[0]->isBillboard_); // ビルボード設定
}

void SampleBox::OnCollisionEnter(BaseObject* object)
{
	/*=================================
		衝突した瞬間に呼び出される関数です。
	===================================*/

	object;
	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void SampleBox::OnCollisionExit(BaseObject* object)
{
	/*================================
		離れた瞬間に呼び出される関数です。
	==================================*/

	object;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
}
