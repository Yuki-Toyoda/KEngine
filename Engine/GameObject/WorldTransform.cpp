#include "WorldTransform.h"

WorldTransform::WorldTransform()
{
	// 初期化
	Initialize();
}

WorldTransform::WorldTransform(Vector3 scale, Vector3 rotate, Vector3 translate)
{
	// 初期化
	Initialize();
	
	// パブリックメンバ変数に代入
	scale_ = scale;
	rotate_ = rotate;
	translate_ = translate;
}

WorldTransform::WorldTransform(Vector3 rotate, Vector3 translate)
{
	// 初期化
	Initialize();

	// パブリックメンバ変数に代入
	rotate_ = rotate;
	translate_ = translate;
}

WorldTransform::WorldTransform(Vector3 translate)
{
	// 初期化
	Initialize();

	// パブリックメンバ変数に代入
	translate_ = translate;
}

void WorldTransform::Initialize()
{
	// パブリックメンバ変数初期化
	scale_ = { 1.0f, 1.0f, 1.0f };
	rotate_ = {0.0f, 0.0f, 0.0f};
	translate_ = {0.0f, 0.0f, 0.0f};
}

void WorldTransform::SetParent(WorldTransform* parent)
{
	// 親子関係をセット
	parent_ = parent;
}

Matrix4x4 WorldTransform::GetMatWorld() const
{
	// 結果格納用
	Matrix4x4 result = Math::MakeAffineMatrix(scale_, rotate_, translate_);

	// 親がいる場合
	if (parent_) {
		result = result * parent_->GetMatWorld();
	}

	// 結果を返す
	return result;
}

Vector3 WorldTransform::GetWorldPos() const
{
	// ワールド行列の取得
	Matrix4x4 matWorld = GetMatWorld();
	// ワールド座標の取得
	Vector3 result = {
		matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]
	};
	// 結果を返す
	return result;
}
