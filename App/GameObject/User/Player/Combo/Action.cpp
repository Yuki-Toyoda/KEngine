#include "Action.h"
#include "App/GameObject/User/Player/Player.h"

Action::Action(const std::string& comboName, const std::string& actionName)
{
	// コンボ名の取得
	comboName_ = comboName;
	// アクション名の取得
	name_ = actionName;

	// パラメータ追加
	AddParam();
	// パラメータ読み込み
	ApplyParam();
}

void Action::Init()
{
	// プレイヤーがセットされていなければ早期リターン
	if (player_ == nullptr) { return; }

	// 各タイマーの開始
	stunTimer_.Start(stunTime_);		// 硬直時間
	acceptTimer_.Start(acceptTime_);	// 受付時間

	// 指定されたアニメーションの再生
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation(animName_, 0.1f);
	// プレイヤーの攻撃判定の設定
	player_->SetIsAttacking(isAttack_);
	// プレイヤーの攻撃判定の長さを設定
	player_->GetSwordLine()->length_ = attackLength_;

	// ToDo : 特殊条件の取得処理を書く
	if (specialConditionName_ != "") {

	}
}

void Action::Update()
{
	// プレイヤーがセットされていなければ早期リターン
	if (player_ == nullptr) { return; }

	// アニメーション終了時に
	if (isStandByfromAnimEnd_) {
		// アニメーションが再生されていない場合
		if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation(animName_)) {
			// 遷移準備完了
			isTransitionReady_ = true;
		}
	}

	// 硬直時間が終了し次第遷移可能に
	if (stunTimer_.GetIsFinish()) { isTransitionReady_ = true; }

	// 遷移時間タイマー終了時
	if (isTransitionReady_) {
		// 遷移条件のチェック
		CheckCondition();
	}

	// 受付時間タイマー終了時
	if (acceptTimer_.GetIsFinish()) {
		// アクション終了
		isEndAction_ = true;
	}

	// 各タイマーの更新
	stunTimer_.Update();	// 硬直時間
	acceptTimer_.Update();	// コンボ受付時間
}

void Action::DisplayImGui()
{
	if (ImGui::TreeNode(name_.c_str())) {
		// アクション名の設定
		ImGui::InputText("ActionName", name_.data(), sizeof(name_.size()));

		// 再生されるアニメーション名
		ImGui::InputText("AnimName", animName_.data(), sizeof(animName_.size()));

		// 硬直時間の設定
		ImGui::DragFloat("StunTime", &stunTime_, 0.01f, 0.001f);

		// 受付時間の設定
		ImGui::DragFloat("AcceptTime", &acceptTime_, 0.01f, 0.001f);

		// 攻撃判定があるかどうか
		if (ImGui::Checkbox("IsAttack", &isAttack_)) { // 攻撃判定がある場合
			// ダメージ量の設定
			ImGui::InputInt("Damage", &damage_);
			// 攻撃長さの設定
			ImGui::DragFloat("AttackLength", &acceptTime_, 0.01f, 0.5f);
		}

		// 入力条件の設定
		ImGui::RadioButton("TRIGGER", &inputCondition_, TRIGGER);
		ImGui::RadioButton("PRESS",   &inputCondition_, PRESS);
		ImGui::RadioButton("TRIGGER", &inputCondition_, RELEASE);

		// 特殊条件の設定
		ImGui::InputText("SpecialConditionName", specialConditionName_.data(), sizeof(specialConditionName_.size()));

		ImGui::TreePop();
	}
}

void Action::AddParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// 各パラメータをグローバル変数クラスに追加
	gv->AddItem(comboName_, name_, animName_);
	gv->AddItem(comboName_, name_, isStandByfromAnimEnd_);
	gv->AddItem(comboName_, name_, stunTime_);
	gv->AddItem(comboName_, name_, acceptTime_);
	gv->AddItem(comboName_, name_, isAttack_);
	gv->AddItem(comboName_, name_, damage_);
	gv->AddItem(comboName_, name_, attackLength_);
	gv->AddItem(comboName_, name_, inputCondition_);
	gv->AddItem(comboName_, name_, specialConditionName_);
}

void Action::ApplyParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// 各パラメータをグローバル変数クラスから取得
	animName_				= gv->GetStringValue(comboName_, name_);
	isStandByfromAnimEnd_	= gv->GetIntValue(comboName_, name_);
	stunTime_				= gv->GetFloatValue(comboName_, name_);
	acceptTime_				= gv->GetFloatValue(comboName_, name_);
	isAttack_				= gv->GetIntValue(comboName_, name_);
	damage_					= gv->GetIntValue(comboName_, name_);
	attackLength_			= gv->GetFloatValue(comboName_, name_);
	inputCondition_			= gv->GetIntValue(comboName_, name_);
	specialConditionName_	= gv->GetStringValue(comboName_, name_);

}

void Action::CheckCondition()
{
	// コンボ変更を行う条件を満たしている場合
	if (specialCondition_) {
		// コンボ変更フラグをtrueに
		isChangeCombo_ = true;
	}

	// 次のコンボに移る条件を達成している場合
	if (Input::GetInstance()->InspectButton(XINPUT_GAMEPAD_A, inputCondition_)) {
		// 次のコンボに移る
		isGoNextAction_ = true;
		// 行動を終了する
		isEndAction_ = true;
	}
}
