#include "ComboManager.h"
#include "App/GameObject/User/Player/Player.h"

void ComboManager::Init(Player* player)
{
	// プレイヤーを取得する
	player_ = player;

	// コンボ配列のクリア
	combos_.clear();

	// パラメーター追加
	AddParam();
	// パラメーター適用
	ApplyParam();
}

void ComboManager::Update()
{
	// コンボ更新
	combo_.Update();
}

void ComboManager::DisplayImGui()
{
	// ImGui開始
	ImGui::Begin("ComboManager", nullptr, ImGuiWindowFlags_MenuBar);
	// メニューバーの開始
	if (ImGui::BeginMenuBar()) {
		// ファイルメニューの表示
		FileMenu();
		// コンボメニューの表示
		ComboMenu();

		ImGui::EndMenuBar();
	}

	// タブ開始
	if (ImGui::BeginTabBar("Combo Manager")) {
		// 選択されているコンボ名のImGuiを表示
		Combo* displayCombo = GetCombo(imGuiDisplayComboName_);
		// 取得したコンボがnullptrだった場合早期リターン
		if (displayCombo == nullptr) { 
			// タブ終了
			ImGui::EndTabBar();
			ImGui::End();
			// 早期リターン
			return;
		}

		// ComboのImGuiを表示
		displayCombo->DisplayImGui();

		// 表示するコンボ名を変更しておく
		imGuiDisplayComboName_ = displayCombo->GetComboName();

		// タブ開始
		if (ImGui::BeginTabItem("Utility Menu")) {
			// デバッグ機能の切り替え
			ImGui::Checkbox("Enable Combo Debug", &imGuiEnableComboDebug_);

			// 削除ボタンはツリーノードで隠す
			if (ImGui::TreeNode("DELETE")) {
				// デバッグ無効
				imGuiEnableComboDebug_ = false;
				// ボタンを押したら該当コンボ削除
				if (ImGui::Button("Delete This Combo")) {
					DeleteCombo(imGuiDisplayComboName_);
				}
				ImGui::TreePop();
			}

			// タブ終了
			ImGui::EndTabItem();
		}

		// タブ終了
		ImGui::EndTabBar();
	}


	ImGui::End();
}

void ComboManager::AddCondition(const std::string& conditionName, bool* condition)
{
	// 条件配列に内容を追加する
	conditions_[conditionName] = condition;
}

void ComboManager::ChangeCombo(const std::string& comboName)
{
	// 該当するコンボが存在するか探査する
	for (Combo& c : combos_) {
		// 該当するコンボが存在した場合はコンボを変更する
		if (c.GetComboName() == comboName) {
			combo_ = c;
		}
	}
	
	// プレイヤーを渡す
	combo_.SetPlayer(player_);
	// 初期化
	combo_.Init(this);
}

Combo* ComboManager::GetCombo(const std::string& name)
{
	// 該当するコンボが存在するか探査する
	for (Combo& c : combos_) {
		// 該当するコンボが存在した場合はそのコンボを返還する
		if (c.GetComboName() == name) {
			return &c;
		}
	}
	// 存在しなかった場合nullptrを返す
	return nullptr;
}

void ComboManager::AddCombo(const std::string& comboID, const std::string& comboName)
{
	// 新規コンボの生成
	Combo newCombo = Combo(comboID, comboName);

	// 同名コンボ個数計算用
	int sameNameCount = 0;

	// 全コンボから同名のコンボがないか探す
	for (Combo& c : combos_) {
		// コンボ名の取得
		std::string cName = c.GetComboName();

		// コンボ名の末尾に数字が含まれている場合は末尾の数字を削除
		while (isdigit(cName.at(cName.size() - 1)))
		{
			// 末尾の文字を削除
			cName.pop_back();
		}

		// 同名コンボがあった場合加算
		if (cName == newCombo.GetComboName()) { sameNameCount++; }
	}

	// 同名オブジェクトが一個でも存在する場合末尾に番号をつける
	if (sameNameCount > 0) {
		// 数字を文字列に変換
		std::string count = std::to_string((sameNameCount));
		newCombo.SetComboName(newCombo.GetComboName() + count);
	}

	// 生成したコンボを配列に追加する
	combos_.push_back(newCombo);
}

void ComboManager::DeleteCombo(const std::string& name)
{
	// 引数で指定した名称のアクションがある場合
	combos_.remove_if([name](Combo& combo) {
		// そのアクションをリストから除外する
		if (combo.GetComboName() == name) {
			return true;
		}
		// 一致しない場合除外しない
		return false;
		});
}

void ComboManager::AddParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボ数を外部ファイルに書き出す
	int32_t comboCount = (int32_t)combos_.size();
	gv->AddItem("ComboGeneralData", "Total Combo Count", comboCount);

	// コンボ配列内のパラメーターも追加
	for (Combo& c : combos_) {
		c.AddParam();
	}
}

void ComboManager::SetValue()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボ数を外部ファイルに書き出す
	int32_t comboCount = (int32_t)combos_.size();
	gv->SetValue("ComboGeneralData", "Total Combo Count", comboCount);

	// コンボ配列内のパラメーターも追加
	for (Combo& c : combos_) {
		c.SetValue();
	}
}

void ComboManager::ApplyParam()
{
	// グローバル変数クラスのインスタンス取得
	GlobalVariables* gv = GlobalVariables::GetInstance();

	// コンボ数を取得
	int32_t comboCount = gv->GetIntValue("ComboGeneralData", "Total Combo Count");

	// コンボ配列クリア
	combos_.clear();

	// コンボ生成
	for (int i = 0; i < comboCount; i++) {
		// コンボIDを求めてコンボ生成
		std::string comboID = "Combo " + std::to_string(i);
		// コンボのIDで名前を取得する
		std::string comboName = gv->GetStringValue("ComboGeneralData", comboID);
		AddCombo(comboID, comboName);
	}
}

void ComboManager::FileMenu()
{
	// ファイル関係メニューの表示
	if (ImGui::BeginMenu("File")) {
		// コンボ追加
		if (ImGui::MenuItem("Add Combo")) {
			std::string comboID = "Combo " + std::to_string(combos_.size());
			AddCombo(comboID);
		}

		// コンボ配列の要素数が1以上の場合
		if (combos_.size() > 0) {
			// コンボの保存を行う
			if (ImGui::MenuItem("Save")) {
				// グローバル変数クラスのインスタンス取得
				GlobalVariables* gv = GlobalVariables::GetInstance();
				// 保存前に全て追加
				AddParam();
				// パラメーターセット
				SetValue();
				// 共用ファイルの保存を行う
				gv->SaveFile("ComboGeneralData");
				// 全てのファイルの保存を行う
				for (Combo& c : combos_) {
					// 保存
					gv->SaveFile(c.GetComboName());
				}
			}

			// コンボの再読み込み
			if (ImGui::MenuItem("Load")) {
				ApplyParam();
			}
		}

		ImGui::EndMenu();
	}
}

void ComboManager::ComboMenu()
{
	// 表示するコンボの切り替えメニューの表示
	if (ImGui::BeginMenu("Combo")) {
		// コンボ配列にデータがない場合
		if (combos_.size() <= 0) { 
			ImGui::Text("No Combo! Please Add Combo.");
			// ここでメニューを終了
			ImGui::EndMenu();
			// これ以降の処理を無視
			return;
		}

		// コンボ配列分ループ
		for (Combo& c : combos_) {
			if (ImGui::MenuItem(c.GetComboName().c_str())) {
				// 選択したコンボ名に設定する
				imGuiDisplayComboName_ = c.GetComboName();
			}
		}

		ImGui::EndMenu();
	}
}
