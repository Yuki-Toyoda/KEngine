#include "ResultManager.h"
#include "../../Core/Camera.h"
#include "../../User/Player/PlayerAnimManager.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../../../Scene/FadeManager.h"

void ResultManager::Init()
{
	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// フェード演出マネージャ取得
	fadeManager_ = FadeManager::GetInstance();
}

void ResultManager::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// スティックの入力検知
	Vector3 stickVec = Vector3((float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
		0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX);
	// スティック入力の正規化を行う
	stickVec = Math::Normalize(stickVec);

	// 前フレームのスティックの入力検知
	Vector3 preStickVec = Vector3((float)preJoyState_.Gamepad.sThumbLX / SHRT_MAX,
		0.0f, (float)preJoyState_.Gamepad.sThumbLY / SHRT_MAX);
	// スティック入力の正規化を行う
	preStickVec = Math::Normalize(preStickVec);

	if (!isFade_) {
		// 左右方向への入力があった場合
		if ((stickVec.x >= 0.5f && preStickVec.x <= 0.5f) || (stickVec.x <= -0.5f && preStickVec.x >= -0.5f)) {
			if (!isRetry_) {

				// リトライする
				isRetry_ = true;

				// クリアフラグによって処理を変える
				if (isCleared_) {
					// タイトルへ戻るテキスト
					sprites_[2]->texBase_ = { 512.0f, 0.0f };
					// リトライテキスト
					sprites_[3]->texBase_ = { 0.0f, 0.0f };
				}
				else {
					// リトライテキスト
					sprites_[6]->texBase_ = { 0.0f, 0.0f };
					// タイトルへ戻るテキスト
					sprites_[7]->texBase_ = { 512.0f, 0.0f };
				}
			}
			else {
				// リトライする
				isRetry_ = false;

				// クリアフラグによって処理を変える
				if (isCleared_) {
					// タイトルへ戻るテキスト
					sprites_[2]->texBase_ = { 0.0f, 0.0f };
					// リトライテキスト
					sprites_[3]->texBase_ = { 448.0f, 0.0f };
				}
				else {
					// リトライテキスト
					sprites_[6]->texBase_ = { 448.0f, 0.0f };
					// タイトルへ戻るテキスト
					sprites_[7]->texBase_ = { 0.0f, 0.0f };
				}
			}
		}
	}

	// Aボタンを押すとその遷移を開始する
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
		// UIを変更する
		sprites_[0]->texBase_ = Vector2(288.0f, 0.0f);

		// フェード演出を一回も行っていない場合
		if (!isFade_) {
			// フェードアウトさせる
			fadeManager_->ChangeParameter("FadeOut");
			fadeManager_->Play();

			// フェードトリガーtrue
			isFade_ = true;
		}
	}
	else {
		// UIを変更する
		sprites_[0]->texBase_ = Vector2(0.0f, 0.0f);
	}

	// フェードマネージャが演出中ではない、かつフェード演出トリガーがtrueなら
	if (!fadeManager_->GetIsStaging() && isFade_) {
		isSceneChange_ = true;
	}
}

void ResultManager::DisplayImGui()
{
	// 位置座標の表示
	transform_.DisplayImGui();

	// プレイヤーマネージャ
	pam_->DisplayImGui();

	// クリアしたかによってImGuiの表示を変更
	if (isCleared_) {
		vegetablesTransform_.DisplayImGuiWithTreeNode("Vegetables");
	}

	// スプライト情報があれば
	if (sprites_.size() > 0) {
		ImGui::Text("Sprites");
		ImGui::BeginChild("Sprites", ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
		for (Sprite* s : sprites_) {
			s->DisplayImGui();
		}
		ImGui::EndChild();
	}
}

void ResultManager::PostInit(bool isClear)
{
	// クリアフラグ取得
	isCleared_ = isClear;

	// 地面の表示
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Plane", "Plane.obj");
	// 回転させる
	transform_.rotate_.x = (float)std::numbers::pi / 2.0f;
	transform_.scale_ = { 50.0f, 50.0f, 1.0f };
	meshes_[0]->texture_ = TextureManager::Load("./Resources", "GroundTex.png");
	meshes_[0]->material_.uvTransform_.scale_ = { transform_.scale_.x / 2.0f,  transform_.scale_.y / 2.0f, 1.0f };

	/// スプライト追加
	// 決定テキスト
	AddSprite("decision", { 1030.0f, 625.0f }, { 230.0f, 76.0f }, TextureManager::Load("./Resources/UI/Result", "decision.png"));
	sprites_[0]->texSize_ = { 288.0f, 96.0f };

	// クリアしたかによって処理を変える
	if (isCleared_) {

		/// 各種トランスフォームの初期化
		// 野菜の山用
		vegetablesTransform_.Init();
		vegetablesTransform_.translate_ = { 1.25f, 0.0f, 1.75f };
		vegetablesTransform_.rotate_ = { 0.0f, 0.65f, 0.0f };

		/// メッシュ追加
		// 野菜の山
		AddMesh(&vegetablesTransform_, color_, "./Resources/Vegetables", "Vegetables.obj");

		/// スプライト追加
		// クリアテキスト
		AddSprite("Clear", { 640.0f, 125.0f }, { 512.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "clear.png"));
		sprites_[1]->anchorPoint_ = { 0.5f, 0.5f };
		// タイトルへ戻るテキスト
		AddSprite("GoTitle", { 350.0f, 500.0f }, { 512.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "clearGoTitle.png"));
		sprites_[2]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[2]->texSize_ = { 512.0f, 96.0f };
		// リトライテキスト
		AddSprite("Retry", { 930.0f, 500.0f }, { 448.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "clearRetry.png"));
		sprites_[3]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[3]->texBase_ = { 448.0f, 0.0f };
		sprites_[3]->texSize_ = { 448.0f, 96.0f };

		// リトライする
		isRetry_ = false;

	}
	else {

		/// スプライト追加
		// 失敗テキスト
		AddSprite("za", { 384.0f, 125.0f }, { 128.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "za.png"));
		AddSprite("nn1", { 512.0f, 125.0f }, { 128.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "nn.png"));
		AddSprite("ne", { 640.0f, 125.0f }, { 128.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "ne.png"));
		AddSprite("nn2", { 768.0f, 125.0f }, { 128.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "nn.png"));
		AddSprite("...", { 896.0f, 125.0f }, { 128.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "....png"));
		sprites_[1]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[2]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[3]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[4]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[5]->anchorPoint_ = { 0.5f, 0.5f };
		// リトライテキスト
		AddSprite("Retry", { 350.0f, 500.0f }, { 448.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "loseRetry.png"));
		sprites_[6]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[6]->texSize_ = { 448.0f, 96.0f };
		// タイトルへ戻るテキスト
		AddSprite("GoTitle", { 930.0f, 500.0f }, { 512.0f, 96.0f }, TextureManager::Load("./Resources/UI/Result", "loseGoTitle.png"));
		sprites_[7]->anchorPoint_ = { 0.5f, 0.5f };
		sprites_[7]->texBase_ = { 512.0f, 0.0f };
		sprites_[7]->texSize_ = { 512.0f, 96.0f };

		// リトライする
		isRetry_ = true;
	}
}

void ResultManager::CreateClearAnimation()
{

}