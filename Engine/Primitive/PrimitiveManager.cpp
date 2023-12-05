#include "PrimitiveManager.h"
#include "../Base/DirectXCommon.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Core/Camera.h"

PrimitiveManager* PrimitiveManager::GetInstance()
{
	static PrimitiveManager instance;
	return &instance;
}

void PrimitiveManager::Initialize()
{
	// コマンドマネージャーの取得
	commandManager_ = DirectXCommon::GetInstance()->GetCommandManager();

	// 全登録形状を削除
	primitives_.remove_if([](std::unique_ptr<BasePrimitive>& primitive) {
		primitive;
		if (true) {
			return true;
		}
		return false;
	});

	// リストのクリア
	primitives_.clear();
}

void PrimitiveManager::Update()
{
	// 登録形状がNullだった場合その要素を削除
	primitives_.remove_if([](std::unique_ptr<BasePrimitive>& primitive) {
		if (primitive->isDestroy_)
			return true;

		return false;
	});

	// レイヤー番号で昇順にソート
	primitives_.sort([](std::unique_ptr<BasePrimitive>& primitive1, std::unique_ptr<BasePrimitive>& primitive2) {return primitive1->layerNo_ > primitive2->layerNo_; });

	// 使用中カメラのビュー行列取得
	Matrix4x4 viewMat = Math::Inverse(GameObjectManager::GetInstance()->GetUseCamera()->GetViewMatrix());
	// ビュー行列の移動要素を消す
	viewMat.m[3][0] = 0.0f;
	viewMat.m[3][1] = 0.0f;
	viewMat.m[3][2] = 0.0f;

	// ビルボード処理を行う場合こちらで設定を行う
	for (std::unique_ptr<BasePrimitive>& primitive : primitives_) {
		// 一度登録ワールド行列を削除
		primitive->transform_->DeleteWorldMat();

		// ビルボードフラグ確認
		if (primitive->isBillboard_) {
			// ワールド行列の計算
			Matrix4x4 worldMatrix = Math::MakeAffineMatrix(
				primitive->transform_->scale_,
				primitive->transform_->rotate_,
				primitive->transform_->translate_
				);
			primitive->billboardMat_ = viewMat * worldMatrix;

			// 計算したワールド行列を代入
			primitive->transform_->SetWorldMat(primitive->billboardMat_);
		}
	}
}

void PrimitiveManager::Draw()
{
	// 全形状を描画
	for (std::unique_ptr<BasePrimitive>& primitive : primitives_) {
		primitive->Draw(commandManager_);
	}	
}
