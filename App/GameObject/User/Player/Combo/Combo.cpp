#include "Combo.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/Player/Combo/ComboManager.h"

Combo::Combo(const std::string& comboID, const std::string& comboName)
{
	// ID取得
	comboID_ = comboID;
	// コンボ名取得
	name_ = comboName;

	// アクション配列クリア
	actions_.clear();
	
	if (comboID != "") {
		// パラメータ追加
		AddParam();
		// パラメータ読み込み
		ApplyParam();
	}
}

void Combo::Init(ComboManager* manager)
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

	// コンボマネージャーをセットする
	comboManager_ = manager;
	// 最初のアクションを初期化する
	actions_.begin()->Init(comboManager_);
}

void Combo::Update()
{
	// プレイヤーがセットされていない場合早期リターン
	if (player_ == nullptr) { return; }

	// コンボ数が配列数を超過している場合はコンボ終了
	if (comboCount_ > actions_.size()) { isComboEnd_ = true; }

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
			comboManager_->ChangeCombo(action->GetNextComboName());
			// 以降の処理を無視する
			return;
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
			nextAction->Init(comboManager_);
		}
		else { // 次アクションに移行しない場合
			// コンボ終了
			isComboEnd_ = true;
			// コンボ終了時点で攻撃していない設定に
			player_->SetIsAttacking(false);
		}
	}
}

void Combo::DisplayImGui()
{
	// タブ開始
	if(ImGui::BeginTabItem("Combo")) {
		// コンボ名の設定
		ImGui::InputText("ComboName", imGuiComboName_, sizeof(imGuiComboName_));
		name_ = imGuiComboName_;

		// アクション名の設定
		ImGui::InputText("Add ActionName", imGuiAddActionName_, sizeof(imGuiAddActionName_));
		std::string addActionName = imGuiAddActionName_;

		// ボタンによるコンボの追加
		if (ImGui::Button("Add Action")) {
			// アクションの追加
			AddAction(addActionName);
		}

		// タブ終了
		ImGui::EndTabItem();
	}

	// 全アクションのImGuiを表示する
	for (Action& action : actions_) {
		// アクション名
		std::string actionName = "Action " + std::to_string(action.GetComboCount());
		if (ImGui::BeginTabItem(actionName.c_str())) {
			// 親のコンボ名を渡し続ける
			action.SetComboName(name_);
			
			// アクションのImGuiを表示
			action.DisplayImGui();

			// ボタンによるアクションの削除
			if (ImGui::Button("Delete Action")) {
				// アクションの削除
				DeleteAction(action.GetActionName());

				// タブ終了
				ImGui::EndTabItem();
				break;
			}

			// タブ終了
			ImGui::EndTabItem();
		}
	}
}

void Combo::AddParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボのIDで名前を登録する
	gv->AddItem("ComboGeneralData", comboID_, name_);

	// コンボ数の取得、登録
	int32_t comboCount = static_cast<int32_t>(actions_.size());
	std::string comboCountName = "ComboCount : ";
	gv->AddItem(name_, comboCountName, comboCount);
	
	// アクション数カウント用
	int32_t actionCount = 0;

	// 全てのアクションのパラメーターを追加する
	for (Action& action : actions_) {
		// 各コンボの１つのアクションとして保存する
		std::string actionName = "Action : " + std::to_string(actionCount);
		// コンボ名を登録する
		gv->AddItem(name_, actionName, action.GetActionName());
		// パラメーター追加
		action.AddParam();
		
		// アクションカウント加算
		actionCount++;
	}
}

void Combo::SetValue()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボのIDで名前を登録する
	gv->SetValue("ComboGeneralData", comboID_, name_);

	// コンボ数の取得、登録
	int32_t comboCount = static_cast<int32_t>(actions_.size());
	std::string comboCountName = "ComboCount : ";
	gv->SetValue(name_, comboCountName, comboCount);

	// アクション数カウント用
	int32_t actionCount = 0;

	// 全てのアクションのパラメーターを追加する
	for (Action& action : actions_) {
		// 各コンボの１つのアクションとして保存する
		std::string actionName = "Action : " + std::to_string(actionCount);
		// コンボ名を登録する
		gv->SetValue(name_, actionName, action.GetActionName());
		// パラメーター追加
		action.SetValue();

		// アクションカウント加算
		actionCount++;
	}
}

void Combo::ApplyParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボのIDで名前を取得する
	name_ = gv->GetStringValue("ComboGeneralData", comboID_);

	// コンボ数の取得
	std::string comboCountName = "ComboCount : ";
	int32_t comboCount = gv->GetIntValue(name_, comboCountName);

	// アクション配列を一度クリアしておく
	actions_.clear();

	// 全てのアクションのパラメーターを追加する
	for (int i = 0; i < comboCount; i++) {
		// アクション名を取得する
		std::string actionGetName = "Action : " + std::to_string(i);
		std::string actionName		= gv->GetStringValue(name_, actionGetName);

		// 新規アクション追加
		Action newAction = Action(name_, actionName, i);

		// アクション配列に追加
		actions_.push_back(newAction);
	}

	// ImGui用変数の名称を変更しておく
	strncpy_s(imGuiComboName_, sizeof(imGuiComboName_), name_.c_str(), _TRUNCATE);
}

bool Combo::GetIsTransitionReady()
{
	// 再生中アクションの取得
	auto action = std::next(actions_.begin(), comboCount_);

	return action->GetIsTransitionReady();
}

void Combo::AddAction(const std::string& actionName)
{
	// 新規アクションの生成
	Action newAction = Action(name_, actionName, static_cast<int32_t>(actions_.size()));

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

	// 一度コンボ番号を振りなおす
	int32_t count = 0;
	for (Action& a : actions_) {
		a.SetComboCount(count);
		count++;
	}
}
