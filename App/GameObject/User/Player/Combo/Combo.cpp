#include "Combo.h"
#include "App/GameObject/User/Player/Player.h"

Combo::Combo(const std::string& name)
{
	// コンボ名取得
	name_ = name;
	
	// アクション配列クリア
	actions_.clear();

	// パラメータ追加
	AddParam();
	// パラメータ読み込み
	ApplyParam();
}

void Combo::Init()
{
	// プレイヤーがセットされていない場合早期リターン
	if (player_ == nullptr) { return; }

	// コンボカウントのリセット
	comboCount_ = 0;

	// 全てのアクションにプレイヤーをセットする
	for (Action& action : actions_) {
		// アクションに対してプレイヤーを渡す
		action.SetPlayer(player_);
	}

	// 最初のアクションを初期化する
	actions_.begin()->Init();
}

void Combo::Update()
{
	// プレイヤーがセットされていない場合早期リターン
	if (player_ == nullptr) { return; }

	// コンボ数が配列数を超過している場合はコンボ終了
	if (comboCount_ >= actions_.size()) { isComboEnd_ = true; }

	// 更新を行うアクションを取得
	auto action = std::next(actions_.begin(), comboCount_);

	// アクションが終了していない場合
	if (!action->GetIsActionEnd()) {
		// アクションの更新
		action->Update();
	}
	else { // アクションが終了している場合
		// コンボ変更フラグがtrueの場合はコンボマネージャーへコンボを変更するよう通達する
		if (action->GetIsChangeCombo()) {

		}

		// 次アクションへ移行する場合は次のアクションへ
		if (action->GetIsGoNextAciton()) {
			// 次のアクションへ
			comboCount_++;

			// コンボ数が配列数を超過している場合はコンボ終了
			if (comboCount_ >= actions_.size()) {
				isComboEnd_ = true;
				// 早期リターン
				return;
			}

			// 更新を行うアクションを取得
			auto nextAction = std::next(actions_.begin(), comboCount_);
			nextAction->Init();
		}
		else { // 次アクションに移行しない場合
			// コンボ終了
			isComboEnd_ = true;
		}
	}
}

void Combo::DisplayImGui()
{
	// タブ開始
	if(ImGui::BeginTabItem(name_.c_str())) {
		// アクション名の設定
		ImGui::InputText("ComboName", name_.data(), sizeof(name_.size()));

		// タブ終了
		ImGui::EndTabItem();
	}
}

void Combo::AddParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボのIDで名前を登録する
	gv->AddItem("Combo", comboID_, name_);

	// コンボ数の取得、登録
	int32_t comboCount = static_cast<int32_t>(actions_.size());
	std::string comboCountName = name_ + " - ComboCount : ";
	gv->AddItem("Combo", comboCountName, comboCount);
	
	// アクション数カウント用
	int32_t actionCount = 0;

	// 全てのアクションのパラメーターを追加する
	for (Action& action : actions_) {
		// 各コンボの１つのアクションとして保存する
		std::string actionName = name_ + " - Action : " + std::to_string(actionCount);
		// コンボ名を登録する
		gv->AddItem("Combo", actionName, action.GetActionName());

		action.AddParam();
		
		// アクションカウント加算
		actionCount++;
	}
}

void Combo::ApplyParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボのIDで名前を取得する
	name_ = gv->GetStringValue("Combo", comboID_);

	// コンボ数の取得
	std::string comboCountName = name_ + " - ComboCount : ";
	int32_t comboCount = gv->GetIntValue("Combo", comboCountName);

	// アクション数カウント用
	int32_t actionCount = 0;

	// 全てのアクションのパラメーターを追加する
	for (int i = 0; i < comboCount; i++) {
		// アクション名を取得する
		std::string actionGetName	= name_ + " - Action : " + std::to_string(actionCount);
		std::string actionName		= gv->GetStringValue("Combo", actionGetName);

		// 新規アクション追加
		Action newAction = Action(name_, actionName);

		// アクション配列に追加
		actions_.push_back(newAction);
	}
}

void Combo::AddAction(const std::string& actionName)
{
	// 新規アクションの生成
	Action newAction = Action(name_, actionName);

	// アクション配列に追加
	actions_.push_back(newAction);
}

void Combo::DeleteAction(const std::string& actionName)
{
	// 引数で指定した名称のアクションがある場合
	actions_.remove_if([actionName](Action& action) {
		// そのアクションをリストから除外する
		if (action.GetActionName() == actionName) {
			return true;
		}
		// 一致しない場合除外しない
		return false;
	});
}
