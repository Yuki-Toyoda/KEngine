#include "Action.h"
#include "App/GameObject/User/Player/Player.h"

Action::Action(const std::string& comboName, const std::string& actionName, const int32_t count)
{
	// コンボ名の取得
	comboName_ = comboName;
	// アクション名の取得
	name_ = actionName;

	// 何コンボ目のアクションかを取得
	comboCount_ = count;

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
	stunTimer_.Start(stunTime_);			// 硬直時間
	acceptTimer_.Start(acceptTime_);		// 受付時間
	attackEndTimer_.Start(attackEndTime_);	// 攻撃終了時間

	// 攻撃開始秒数が0.0以外だった場合
	if (attackStartTime_ != 0.0f) {
		// 攻撃開始秒数タイマー開始
		attackStartTimer_.Start(attackStartTime_);
	}

	// 攻撃していない状態に
	player_->SetIsAttacking(false);
	// 指定されたアニメーションの再生
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation(animName_, 0.015f);
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
		AnimationCheck();
	}

	// 攻撃判定有効時
	if (isAttack_) {
		// 攻撃判定の更新
		AttackJudgeUpdate();
	}

	// 硬直時間が終了し次第、かつアニメーションが終了するまで待たない場合次のアクションへ遷移できる状態に
	if (stunTimer_.GetIsFinish() && !isStandByfromAnimEnd_) { 
		isTransitionReady_ = true; 
	}

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
	stunTimer_.Update();		// 硬直時間
	acceptTimer_.Update();		// コンボ受付時間
	attackStartTimer_.Update(); // 攻撃開始時間
	attackEndTimer_.Update();	// 攻撃終了時間
}

void Action::DisplayImGui()
{
	// アクション名の設定
	ImGui::InputText("ActionName", imGuiActionName_, sizeof(imGuiActionName_));
	name_ = imGuiActionName_;

	// 再生されるアニメーション名
	ImGui::InputText("AnimName", imGuiAnimName_, sizeof(imGuiAnimName_));
	animName_ = imGuiAnimName_;
	// アニメーション終了した段階で遷移可能にするか
	ImGui::Checkbox("IsStandByfromAnimEnd", &isStandByfromAnimEnd_);

	// 硬直時間の設定
	ImGui::DragFloat("StunTime", &stunTime_, 0.01f, 0.001f);

	// 受付時間の設定
	ImGui::DragFloat("AcceptTime", &acceptTime_, 0.01f, 0.001f);

	// 攻撃判定があるかどうか
	ImGui::Checkbox("IsAttack", &isAttack_);
	if (isAttack_) { // 攻撃判定がある場合
		// ダメージ量の設定
		ImGui::InputInt("Damage", &damage_);
		// 攻撃長さの設定
		ImGui::DragFloat("AttackLength", &attackLength_, 0.01f, 0.5f);

		// 攻撃開始秒数の設定
		ImGui::DragFloat("StartTime", &attackStartTime_, 0.01f, 0.0f);
		// 攻撃終了秒数の設定
		ImGui::DragFloat("EndTime", &attackEndTime_, 0.01f, 0.5f);
	}

	// 移動した際のアクション終了設定
	ImGui::Checkbox("IsActionEndFromMove", &isActionEndFromMove_);

	// 入力条件の設定
	ImGui::Text("Input Condition");
	ImGui::RadioButton("TRIGGER", &inputCondition_, TRIGGER);
	ImGui::RadioButton("PRESS", &inputCondition_, PRESS);
	ImGui::RadioButton("RELEASE", &inputCondition_, RELEASE);

	// 特殊条件の設定
	ImGui::InputText("SpecialConditionName", imGuiSpecialConditionName_, sizeof(imGuiSpecialConditionName_));
	specialConditionName_ = imGuiSpecialConditionName_;
	// 特殊条件が設定されている場合
	if (specialConditionName_ != "") {
		// コンボ名の設定
		ImGui::InputText("SpecialConditionName", imGuiSpecialConditionName_, sizeof(imGuiSpecialConditionName_));
		specialConditionName_ = imGuiSpecialConditionName_;
	}
}

void Action::AddParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();
	// 被り防止用に名称設定
	std::string actionName = name_ + " : " + std::to_string(comboCount_);

	// 各パラメータをグローバル変数クラスに追加
	gv->AddItem(comboName_, actionName + " - AnimName", animName_);
	gv->AddItem(comboName_, actionName + " - IsStandByfromAnimEnd", isStandByfromAnimEnd_);
	gv->AddItem(comboName_, actionName + " - StunTime", stunTime_);
	gv->AddItem(comboName_, actionName + " - AcceptTime", acceptTime_);
	gv->AddItem(comboName_, actionName + " - IsAttack", isAttack_);
	gv->AddItem(comboName_, actionName + " - Damage", damage_);
	gv->AddItem(comboName_, actionName + " - AttackLength", attackLength_);
	gv->AddItem(comboName_, actionName + " - AttackStartTime", attackStartTime_);
	gv->AddItem(comboName_, actionName + " - AttackEndTime", attackEndTime_);
	gv->AddItem(comboName_, actionName + " - IsActionEndFromMove", isActionEndFromMove_);
	gv->AddItem(comboName_, actionName + " - InputCondition", inputCondition_);
	gv->AddItem(comboName_, actionName + " - SpecialConditionName", specialConditionName_);
	gv->AddItem(comboName_, actionName + " - NextComboName", nextComboName_);
}

void Action::SetValue()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();
	// 被り防止用に名称設定
	std::string actionName = name_ + " : " + std::to_string(comboCount_);

	// 各パラメータをグローバル変数クラスに追加
	gv->SetValue(comboName_, actionName + " - AnimName", animName_);
	gv->SetValue(comboName_, actionName + " - IsStandByfromAnimEnd", isStandByfromAnimEnd_);
	gv->SetValue(comboName_, actionName + " - StunTime", stunTime_);
	gv->SetValue(comboName_, actionName + " - AcceptTime", acceptTime_);
	gv->SetValue(comboName_, actionName + " - IsAttack", isAttack_);
	gv->SetValue(comboName_, actionName + " - Damage", damage_);
	gv->SetValue(comboName_, actionName + " - AttackLength", attackLength_);
	gv->SetValue(comboName_, actionName + " - AttackStartTime", attackStartTime_);
	gv->SetValue(comboName_, actionName + " - AttackEndTime", attackEndTime_);
	gv->SetValue(comboName_, actionName + " - IsActionEndFromMove", isActionEndFromMove_);
	gv->SetValue(comboName_, actionName + " - InputCondition", inputCondition_);
	gv->SetValue(comboName_, actionName + " - SpecialConditionName", specialConditionName_);
	gv->SetValue(comboName_, actionName + " - NextComboName", nextComboName_);
}

void Action::ApplyParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();
	// 被り防止用に名称設定
	std::string actionName = name_ + " : " + std::to_string(comboCount_);

	// 各パラメータをグローバル変数クラスから取得
	animName_				= gv->GetStringValue(comboName_, actionName + " - AnimName");
	isStandByfromAnimEnd_	= gv->GetIntValue(comboName_, actionName + " - IsStandByfromAnimEnd");
	stunTime_				= gv->GetFloatValue(comboName_, actionName + " - StunTime");
	acceptTime_				= gv->GetFloatValue(comboName_, actionName + " - AcceptTime");
	isAttack_				= gv->GetIntValue(comboName_, actionName + " - IsAttack");
	damage_					= gv->GetIntValue(comboName_, actionName + " - Damage");
	attackLength_			= gv->GetFloatValue(comboName_, actionName + " - AttackLength");
	attackStartTime_		= gv->GetFloatValue(comboName_, actionName + " - AttackStartTime");
	attackEndTime_			= gv->GetFloatValue(comboName_, actionName + " - AttackEndTime");
	isActionEndFromMove_	= gv->GetIntValue(comboName_, actionName + " - IsActionEndFromMove");
	inputCondition_			= gv->GetIntValue(comboName_, actionName + " - InputCondition");
	specialConditionName_	= gv->GetStringValue(comboName_, actionName + " - SpecialConditionName");
	nextComboName_			= gv->GetStringValue(comboName_, actionName + " - NextComboName");

	// ImGui用変数の名称を変更しておく
	strncpy_s(imGuiActionName_, sizeof(imGuiActionName_), name_.c_str(), _TRUNCATE);
	strncpy_s(imGuiAnimName_, sizeof(imGuiAnimName_), animName_.c_str(), _TRUNCATE);
	strncpy_s(imGuiSpecialConditionName_, sizeof(imGuiSpecialConditionName_), specialConditionName_.c_str(), _TRUNCATE);
	strncpy_s(imGuiNextComboName_, sizeof(imGuiNextComboName_), nextComboName_.c_str(), _TRUNCATE);
}

void Action::AnimationCheck()
{
	// アニメーションが再生されていない場合
	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation(animName_)) {
		// 遷移準備完了
		isTransitionReady_ = true;
	}
}

void Action::AttackJudgeUpdate()
{
	// 攻撃開始タイマーが終了している場合
	if (attackStartTimer_.GetIsFinish()) {
		// 攻撃中に
		player_->SetIsAttacking(true);

		// 攻撃終了タイマーが終了している場合
		if (attackEndTimer_.GetIsFinish()) {
			// 攻撃中ではない
			player_->SetIsAttacking(false);
		}
	}
	else { // 終了していない場合
		// 攻撃中ではない
		player_->SetIsAttacking(false);
	}

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

	// 左スティックが入力されているかつ移動によってアクションが終了する状態の場合
	if (Input::GetInstance()->InspectJoyStickInput(0) && isActionEndFromMove_) {
		// 行動を終了する
		isEndAction_ = true;
	}
}
