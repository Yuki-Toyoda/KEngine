#include "PlayerAnimManager.h"
#include "Player.h"

void PlayerAnimManager::Init()
{
	// 各トランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_);
	arrowTransform_.Init();
	arrowTransform_.scale_ = { 7.5f, 7.5f, 7.5f };
	arrowTransform_.rotate_.x = (float)std::numbers::pi / 2.0f;

	// メッシュ追加
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Player.obj");

	// アニメーションマネージャ取得
	animManager_ = AnimationManager::GetInstance();

	/// パラメータ作成
	// タイトル上の待機状態
	CreateParameter("Player_Idle");

	// ダメージアニメーション
	CreateParameter("Player_Damage");
	// ダメージ中アニメーション
	CreateParameter("Player_Damaging");
	// ダメージからの復帰アニメーション
	CreateParameter("Player_DamageRecovery");

	// スタンアニメーション
	CreateParameter("Player_Stun");
	// スタン中アニメーション
	CreateParameter("Player_Stuning");
	// スタン復帰アニメーション
	CreateParameter("Player_StunRecovery");

	// ブリンクタイマー開始
	arrowBlinkTimer_.Start(kMinArrowBlinkTime_);
}

void PlayerAnimManager::Update()
{
	// 矢印メッシュが存在するかつ、プレイヤーが存在場合
	if (arrow_ != nullptr && player_ != nullptr) {
		// 入力があった場合矢印を表示する
		if (InputManager::AtackCharge() && player_->GetAbsorptionCount() >= 1 && player_->GetCanDash() && anim_->GetReadingParameterName() != "Player_Damage" && anim_->GetReadingParameterName() != "Player_DamageRecovery" && anim_->GetReadingParameterName() != "Player_Damaging") {
			// 矢印が非表示だったら
			if (!arrow_->isActive_) {
				// 表示
				arrow_->isActive_ = true;
				// ブリンクタイマーリスタート
				arrowBlinkTimer_.Start(kMinArrowBlinkTime_);
			}
		}
		else {
			// 入力がない場合は非表示
			arrow_->isActive_ = false;
		}
		
		// オフセットの取得
		Vector3 offset = arrowOffset_;

		/// 矢印が表示されている間ブリンク処理を挟む
		if (arrow_->isActive_) {
			// ブリンクタイマー終了時
			if (arrowBlinkTimer_.GetIsFinish()) {
				if (isBlinkReturn_) {
					isBlinkReturn_ = false;
				}
				else {
					isBlinkReturn_ = true;
				}

				// 
				if (player_->GetAtackCount() > player_->GetmaxAtackCount() && player_->GetISTutrialDash()) {
					// ブリンクタイマー開始
					arrowBlinkTimer_.Start(kMaxArrowBlinkTime_);
				}
				else {
					// ブリンクタイマー開始
					arrowBlinkTimer_.Start(kMinArrowBlinkTime_);
				}
			}
			else {
				if (isBlinkReturn_) {
					// オフセットを線形補間で動かす
					arrowOffset_.z = KLib::Lerp<float>(blinkStrength_, -7.0f, KLib::EaseInQuad(arrowBlinkTimer_.GetProgress()));
				}
				else {
					// オフセットを線形補間で動かす
					arrowOffset_.z = KLib::Lerp<float>(-7.0f, blinkStrength_, KLib::EaseOutQuad(arrowBlinkTimer_.GetProgress()));
				}

				// 矢印のブリンクタイマー更新
				arrowBlinkTimer_.Update();
			}
		}
		
		// プレイヤーの回転角から回転行列を生成
		Matrix4x4 rotateMat = Math::MakeIdentity4x4();

		// プレイヤーの攻撃チャージが終了している場合、矢印をボスのいる方に向ける
		if (player_->GetAtackCount() > player_->GetmaxAtackCount() && player_->GetISTutrialDash()) {

			// プレイヤーからボスへの方向ベクトルを求める
			Vector3 dir = player_->GetBossDirection();

			// 角度をラジアンで求める
			float angleY = atan2(dir.x, dir.z);

			// 回転行列を生成
			rotateMat = Math::MakeRotateYMatrix(angleY);
			// 矢印の向きを固定
			arrowTransform_.rotate_.y = angleY;

			// 矢印色を変える
			arrowColor_ = {1.0f, 0.85f, 0.0f, 1.0f};
			// ブリンクの強さをチャージ時のモノに
			blinkStrength_ = kMaxBlinkStrength_;
		}
		else {
			// 回転行列を生成
			rotateMat = Math::MakeRotateYMatrix(player_->transform_.rotate_.y);
			arrowTransform_.rotate_.y = player_->transform_.rotate_.y;

			// 矢印色を変える
			arrowColor_ = { 0.0f, 1.0f, 0.1f, 1.0f };
			// ブリンクの強さを通常時のモノに
			blinkStrength_ = kMaxBlinkStrength_;
		}

		// オフセットと回転行列を元に再度オフセットを計算
		offset = Math::Transform(offset, rotateMat);

		// 矢印座標をオフセットを元に求める
		arrowTransform_.translate_ = player_->transform_.translate_ + offset;
	}

	// アニメーションがnullptr以外だったら
	if (anim_ != nullptr) {
		// ダメージの開始アニメーション中
		if (anim_->GetReadingParameterName() == "Player_Damage") {
			// アニメーション終了してたら
			if (anim_->isEnd_) {
				// ダメージ中アニメーションに遷移
				anim_->ChangeParameter("Player_Damaging", true);
				// ループ有効
				anim_->isLoop_ = true;
			}
		}
		// ダメージ中のアニメーション中
		if (anim_->GetReadingParameterName() == "Player_Damaging") {
			// タイマー終了してたら
			if (stanTimer_.GetIsFinish()) {
				// ダメージ後の復帰のアニメーションに遷移
				anim_->ChangeParameter("Player_DamageRecovery", true);
				// ループ無効
				anim_->isLoop_ = false;
			}

			// スタン秒数タイマー更新
			stanTimer_.Update();

		}
		// ダメージからの復帰アニメーション中
		if (anim_->GetReadingParameterName() == "Player_DamageRecovery") {
			// アニメーション終了してたら
			if (anim_->isEnd_) {
				// ダメージ後の復帰のアニメーションに遷移
				anim_->ChangeParameter("Player_Idle", true);
				// ループ有効
				anim_->isLoop_ = true;

				// ダメージを喰らっている
				if (player_ != nullptr) {
					player_->SetIsDamaged(false);
				}
			}
		}
	}
}

void PlayerAnimManager::DisplayImGui()
{
	// 表示状態の切り替え
	ImGui::Checkbox("isActive", &isActive_);

	transform_.DisplayImGuiWithTreeNode("PamTranslate");
	// 各種パーツの情報表示
	bodyTransform_.DisplayImGuiWithTreeNode("body"); // 身体
	arrowTransform_.DisplayImGuiWithTreeNode("Arrow"); // 矢印
	ImGui::DragFloat3("ArrowOffset", &arrowOffset_.x); // 矢印オフセット

	// アニメーションのImGui
	if (anim_ != nullptr) {
		anim_->DisplayImGui();
		// アニメーションの読み込みパラメータ変更
		if (ImGui::TreeNode("Player_ChangeReadParameter")) {
			if (ImGui::Button("Idle")) {
				anim_->ChangeParameter("Player_Idle", true);
			}
			if (ImGui::Button("Damage")) {
				anim_->ChangeParameter("Player_Damage", true);
			}
			if (ImGui::Button("Damaging")) {
				anim_->ChangeParameter("Player_Damaging", true);
			}
			if (ImGui::Button("DamageRecovery")) {
				anim_->ChangeParameter("Player_DamageRecovery", true);
			}
			if (ImGui::Button("Stun")) {
				anim_->ChangeParameter("Player_Stun", true);
			}
			if (ImGui::Button("Stuning")) {
				anim_->ChangeParameter("Player_Stuning", true);
			}
			if (ImGui::Button("StunRecovery")) {
				anim_->ChangeParameter("Player_StunRecovery", true);
			}

			ImGui::TreePop();
		}

		stanTimer_.DisplayImGui("StanTimer");

		// ダメージ処理
		ImGui::DragFloat("AnimationTime", &imGuiAnimationTime_, 0.01f, 0.01f, 5.0f);
		if (ImGui::Button("Damage")) {
			// ダメージ
			Damage(imGuiAnimationTime_);
		}
	}
}

void PlayerAnimManager::SetPlayer(Player* player)
{
	// プレイヤーをセット
	player_ = player;
	// プレイヤーの座標に追従させる
	transform_.SetParent(&player_->transform_);
}

void PlayerAnimManager::CreateParameter(const std::string& name)
{
	// パラメータ生成
	animManager_->CreateAnimationParameter(name);
	// キー生成
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Translate");
}

void PlayerAnimManager::CreateAnimation()
{
	// アニメーション生成
	anim_ = animManager_->CreateAnimation("PlayerAnim", "Player_Idle");
	anim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	anim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);

	// アニメーションループtrue
	anim_->isLoop_ = true;
	// アニメーション再生
	anim_->Play();

	// アニメーション生成を行ったときのみ矢印を生成する
	arrow_ = AddPlane(&arrowTransform_, arrowColor_, TextureManager::Load("dashArrow.png"));

}

void PlayerAnimManager::ChangeParamameter(const std::string& name, const bool& isChanged)
{
	// パラメータ変更
	anim_->ChangeParameter(name, isChanged);
}

void PlayerAnimManager::Damage(const float& stanTime)
{
	// ダメージを喰らっている
	if (player_ != nullptr) {
		player_->SetIsDamaged(true);
	}

	// スタンタイマー開始
	stanTimer_.Start(stanTime);
	// 読み込みパラメータ変更
	anim_->ChangeParameter("Player_Damage", true);
	// ループ無効
	anim_->isLoop_ = false;
}
