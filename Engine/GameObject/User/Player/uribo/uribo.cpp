#include "uribo.h"
#include "../../../../Resource/Texture/TextureManager.h"
#include "../../../../Scene/FadeManager.h"
#include "../../../GameObjectManager.h"
#include "../../../Core/Camera.h"

void Uribo::Init()
{
	// 各種トランスフォームの初期化
	bodyTransform_.Init();
	feedAreaTransform_.Init();
	feedAreaTransform_.rotate_.x = (float)std::numbers::pi / 2.0f;
	// 親子付け
	bodyTransform_.SetParent(&transform_);
	feedAreaTransform_.SetParent(&transform_);

	// メッシュ追加
	AddMesh(&bodyTransform_, color_, "./Resources/Uribo", "Uribo.obj");
	feedAreaMesh_ = AddMesh(&feedAreaTransform_, areaColor_, "./Engine/Resource/Samples/Plane", "Plane.obj");
	feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Enable.png");
	feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Disable.png");

	// スフィアコライダーを追加
	AddColliderSphere("Roller", &transform_.translate_, &collisionScale_.x);
	collisionScale_ = { 10.0f,10.0f,10.0f };
	transform_.translate_.y = 3.0f;
	transform_.translate_.x = 30.0f;
	transform_.scale_ = { 1.75f,1.75f, 1.75f };

	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);
	variables->AddItem(name_, "DefaultHp", defaultHP_);
	variables->AddItem(name_, "collisionScale", collisionScale_);
	variables->AddItem(name_, "scale", transform_.scale_);
	variables->AddItem(name_, "translate", transform_.translate_);
	variables->AddItem(name_, "decrementHP", decrementHP);
	SetGlobalVariables();
	hitPoint_ = defaultHP_;

	// エリアをコライダーの大きさによって広げる
	feedAreaTransform_.scale_ = {collisionScale_.x / 2.0f, collisionScale_.z / 2.0f, 1.0f };
	feedAreaTransform_.translate_.y = -(transform_.scale_.y / 2.0f) + 0.1f;

	// 演出用カメラの生成
	stagingCamera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("stagingCamera_Uribo", BaseObject::TagCamera);

	// アニメーションマネージャーのインスタンス取得
	animManager_ = AnimationManager::GetInstance();

	/// パラメータ生成
	// 待機状態
	CreateParameter("Uribo_Idle");
	// HP低下時の待機状態
	CreateParameter("Uribo_LowIdle");
	// 野菜取得待機状態
	CreateParameter("Uribo_Vegetables");
	// 死亡アニメーション
	CreateParameter("Uribo_Dead");

	/// アニメーションの生成
	anim_ = animManager_->CreateAnimation("UriboAnim", "Uribo_Idle");
	anim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	anim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);
	anim_->AddAnimationKeys<Vector3>("Camera_Rotate", &stagingCamera_->transform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Camera_Translate", &cameraOffset_);

	// ループ有効
	anim_->isLoop_ = true;
	// アニメーション再生
	anim_->Play();
	isTutrial_ = false;
	tutrialStart_ = true;
}

void Uribo::Update()
{
	timer_.Update();
	// カメラ使用中のみに更新処理を呼び出す
	if (stagingCamera_->GetIsUsedCamera()) {
		CameraUpdate();
	}

	// ボスが死亡していない場合
	if (!isBossDead_&&hitPoint_>=0&&timer_.GetIsFinish()&&tutrialStart_) {
		hitPoint_ -= decrementHP;
	}
	if (isTutrial_) {
		if (hitPoint_ <= 50) {
			hitPoint_ = 50;
		}
	}
	// HPが0以下になったら死亡アニメーション再生
	if (hitPoint_ <= 0) {
		if (!isTutrial_) {
			// アニメーションの読み込みパラメータ名が同一でない場合
			if (anim_->GetReadingParameterName() != "Uribo_Dead") {
				// 演出用カメラの有効化
				stagingCamera_->UseThisCamera();
				// アニメーションのループを切る
				anim_->isLoop_ = false;
				// アニメーション状態を変更する
				anim_->ChangeParameter("Uribo_Dead", true);

				// 死亡時のテクスチャに変更
				meshes_[0]->texture_ = TextureManager::Load("./Resources/Player", "TexGameOver.png");
			}
			else {
				// アニメーション終了時
				if (anim_->isEnd_) {
					// 死亡トリガーTrue
					isDead_ = true;
				}

				// アニメーション進捗が一定値を超えたらフェードアウト開始
				if (anim_->GetAnimationProgress() >= 0.8f) {
					if (!isFade_) {
						// フェードアウト
						FadeManager::GetInstance()->ChangeParameter("FadeOut", true);
						FadeManager::GetInstance()->Play();

						// フェード演出を行っていたらtrue
						isFade_ = true;
					}
				}
			}
		}
	}
	else {

		//// ボスが死亡していない場合
		//if (!isBossDead_) {
		//	hitPoint_ -= decrementHP;
		//}

		// テクスチャが同一でない場合テクスチャを変更
		if (feedAreaMesh_->texture_ != TextureManager::Load("FeedAreaTex_Disable.png")) {
			feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Disable.png");
		}

		// ウリボのHP割合を取得
		float uriboHPRatio = (float)hitPoint_ / (float)defaultHP_;
		// HP割合が3 / 1 以下になったとき
		if (uriboHPRatio <= 0.33f && !canFeed_) {
			// アニメーションの読み込みパラメータ名が同一でない場合
			if (anim_->GetReadingParameterName() != "Uribo_LowIdle") {
				// アニメーション状態を変更する
				anim_->ChangeParameter("Uribo_LowIdle", true);
			}
		}
		else if (!canFeed_) {
			// アニメーションの読み込みパラメータ名が同一でない場合
			if (anim_->GetReadingParameterName() != "Uribo_Idle") {
				// アニメーション状態を変更する
				anim_->ChangeParameter("Uribo_Idle", false);
			}
		}

		// 餌を与えられる状態の場合アニメーションを強制変更
		if (canFeed_) {
			// アニメーションの読み込みパラメータ名が同一でない場合
			if (anim_->GetReadingParameterName() != "Uribo_Vegetables") {
				// アニメーション状態を変更する
				anim_->ChangeParameter("Uribo_Vegetables", true);
			}
		}

		// 餌を与えられる状態トリガーリセット
		canFeed_ = false;
	}
}

void Uribo::DisplayImGui()
{
	transform_.DisplayImGui();
	
	if (ImGui::TreeNode("StagingCamera")) {
		// 演出用カメラのオフセット
		ImGui::DragFloat3("CameraOffset", &cameraOffset_.x);
		ImGui::DragFloat3("CameraRotate", &stagingCamera_->transform_.rotate_.x, 0.01f);

		ImGui::TreePop();
	}

	anim_->DisplayImGui();
	// アニメーションの読み込みパラメータ変更
	if (ImGui::TreeNode("Uribo_ChangeReadParameter")) {
		if (ImGui::Button("Idle")) {
			anim_->ChangeParameter("Uribo_Idle", true);
		}
		if (ImGui::Button("LowIdle")) {
			anim_->ChangeParameter("Uribo_LowIdle", true);
		}
		if (ImGui::Button("Vegetables")) {
			anim_->ChangeParameter("Uribo_Vegetables", true);
		}
		if (ImGui::Button("Dead")) {
			anim_->ChangeParameter("Uribo_Dead", true);
		}

		ImGui::TreePop();
	}

	feedAreaTransform_.DisplayImGuiWithTreeNode("FeedArea");
	ImGui::ColorPicker4("areaColor", &areaColor_.x);

	ImGui::DragFloat3("collisionScale", &collisionScale_.x);
	ImGui::DragInt("defaultHP", &defaultHP_);
	ImGui::DragInt("HP", &hitPoint_);
	if (ImGui::Button("Save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();
		variables->SetValue(name_, "DefaultHp", defaultHP_);
		variables->SetValue(name_, "collisionScale", collisionScale_);
		variables->SetValue(name_, "scale", transform_.scale_);
		variables->SetValue(name_, "translate", transform_.translate_);
		variables->SetValue(name_, "decrementHP", decrementHP);
		variables->SaveFile(name_);
	}
}

void Uribo::OnCollision(Collider* collider)
{
	// エリア内にプレイヤーがいたらテクスチャを変更する
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Enable.png");

		// 餌を与えられる状態であることを示す
		canFeed_ = true;
	}
}

void Uribo::CameraUpdate()
{
	// オフセットを取得
	Vector3 offset = cameraOffset_;

	// 回転角から行列を生成
	Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(stagingCamera_->transform_.rotate_);
	// オフセットをカメラの回転に合わせて回転させる
	offset = Math::Transform(offset, rotateMat);

	// オフセットをもとにカメラ座標を計算する
	stagingCamera_->transform_.translate_ = transform_.translate_ + offset;

	// カメラの更新
	stagingCamera_->Update();
}

void Uribo::Heal(int healPower)
{
	if (timer_.GetIsFinish()) {
		
	}
	else {
		IsCanHeal = false;
		return;
	}
	// 回復させる
	if (hitPoint_ > 0) {
		hitPoint_ += healPower;
		IsCanHeal = true;
	}
	if (hitPoint_ >= defaultHP_) {
		hitPoint_ = defaultHP_;
		timer_.Start(3.0f);
		IsCanHeal = true;
	}
	
}

void Uribo::Tutrial()
{
	//チュートリアル用に体力を1/3に減らす
	hitPoint_ = defaultHP_/3;
}

bool Uribo::TutrialEnd()
{
	// 
	if (hitPoint_ > defaultHP_ / 2) {
		return true;
	}

	// HPが最大値以上になったときは最大値で打ち止め
	if (hitPoint_ > defaultHP_) {
		hitPoint_ = defaultHP_;
	}

	return false;
}

void Uribo::SetGlobalVariables()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();
	defaultHP_=variables->GetIntValue(name_, "DefaultHp");
	collisionScale_=variables->GetVector3Value(name_, "collisionScale");
	transform_.scale_=variables->GetVector3Value(name_, "scale");
	transform_.translate_=variables->GetVector3Value(name_, "translate");
	decrementHP=variables->GetIntValue(name_, "decrementHP");
}

void Uribo::CreateParameter(const std::string& name)
{
	// パラメータ名生成
	animManager_->CreateAnimationParameter(name);
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Translate");
}
