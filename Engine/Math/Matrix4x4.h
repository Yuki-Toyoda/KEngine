#pragma once

// クラスの前方宣言
class Vector3;

/// <summary>
/// 4x4行列クラス
/// </summary>
class Matrix4x4 final
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Matrix4x4();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mat">行列</param>
	Matrix4x4(const Matrix4x4& mat);

	// 既定のデストラクタを使用する
	~Matrix4x4() = default;

public: // 演算子のオーバーロード

	/// <summary>
	/// 演算子オーバーロード(+)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4 operator+(const Matrix4x4& mat) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(-)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4 operator-(const Matrix4x4& mat) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(*)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4 operator*(const Matrix4x4& mat) const noexcept;

	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4& operator=(const Matrix4x4& mat) noexcept;
	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4& operator=(Matrix4x4&& mat) noexcept;

	/// <summary>
	/// 演算子オーバーロード(+=)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4 operator+=(const Matrix4x4& mat) noexcept;
	/// <summary>
	/// 演算子オーバーロード(-=)
	/// </summary>
	/// <param name="mat">右辺値</param>
	/// <returns>計算後値</returns>
	Matrix4x4 operator-=(const Matrix4x4& mat) noexcept;

public: // メンバ関数

	/// <summary>
	/// 単位行列生成
	/// </summary>
	/// <returns>単位行列</returns>
	const Matrix4x4& Identity();

	/// <summary>
	/// 拡大縮小行列生成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns>拡大縮小行列</returns>
	const Matrix4x4& Scale(const Vector3& scale);

	/// <summary>
	/// x軸の回転行列生成
	/// </summary>
	/// <param name="x">x軸の回転角</param>
	/// <returns>回転行列</returns>
	const Matrix4x4& RotateX(const float& x);
	/// <summary>
	/// y軸の回転行列生成
	/// </summary>
	/// <param name="y">y軸の回転角</param>
	/// <returns>回転行列</returns>
	const Matrix4x4& RotateY(const float& y);
	/// <summary>
	/// z軸の回転行列生成
	/// </summary>
	/// <param name="z">z軸の回転角</param>
	/// <returns>回転行列</returns>
	const Matrix4x4& RotateZ(const float& z);
	
	/// <summary>
	/// 全軸回転行列生成
	/// </summary>
	/// <param name="rotate">回転角</param>
	/// <returns>回転行列</returns>
	const Matrix4x4& Rotate(const Vector3& rotate);

	/// <summary>
	/// 平行移動行列生成
	/// </summary>
	/// <param name="translate">位置座標</param>
	/// <returns>平行移動行列</returns>
	const Matrix4x4& Translate(const Vector3& translate);

	/// <summary>
	/// アフィン行列生成
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転角</param>
	/// <param name="translate">位置座標</param>
	/// <returns>アフィン行列</returns>
	const Matrix4x4& Affin(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// 逆行列生成
	/// </summary>
	/// <returns>逆行列</returns>
	const Matrix4x4& Inverse();

	/// <summary>
	/// 正射影行列生成
	/// </summary>
	/// <param name="left">画面左</param>
	/// <param name="top">画面上</param>
	/// <param name="right">画面右</param>
	/// <param name="bottom">画面下</param>
	/// <param name="nearClip">近平面</param>
	/// <param name="farClip">遠平面</param>
	/// <returns>正射影行列</returns>
	const Matrix4x4& OrthGraphic(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip);

	/// <summary>
	/// 透視射影行列生成
	/// </summary>
	/// <param name="fovY">視野角</param>
	/// <param name="aspectRatio">画面アスペクト比</param>
	/// <param name="nearClip">近平面への距離</param>
	/// <param name="farClip">遠平面への距離</param>
	/// <returns>透視射影行列</returns>
	const Matrix4x4& PerspectiveFov(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip);

	/// <summary>
	/// ビューポート変換行列生成関数
	/// </summary>
	/// <param name="left">画面左</param>
	/// <param name="top">画面上</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <param name="minDepth">最低深度</param>
	/// <param name="maxDepth">最大深度</param>
	/// <returns>ビューポート行列</returns>
	const Matrix4x4& Viewport(float left, float top, float width, float height, float minDepth, float maxDepth);

public: // 静的なメンバ関数

	/// <summary>
	/// 単位行列
	/// </summary>
	static const Matrix4x4 kIdentity;

	/// <summary>
	/// 拡大縮小行列
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns></returns>
	static Matrix4x4 MakeScale(const Vector3& scale);

	/// <summary>
	/// x軸回転行列
	/// </summary>
	/// <param name="x">x軸の回転角</param>
	/// <returns>回転行列</returns>
	static Matrix4x4 MakeRotateX(const float& x);
	/// <summary>
	/// y軸回転行列
	/// </summary>
	/// <param name="y">y軸の回転角</param>
	/// <returns>回転行列</returns>
	static Matrix4x4 MakeRotateY(const float& y);
	/// <summary>
	/// z軸回転行列
	/// </summary>
	/// <param name="z">z軸の回転角</param>
	/// <returns>回転行列</returns>
	static Matrix4x4 MakeRotateZ(const float& z);

	/// <summary>
	/// 回転行列
	/// </summary>
	/// <param name="rotate">回転角</param>
	/// <returns>回転行列</returns>
	static Matrix4x4 MakeRotate(const Vector3& rotate);

	/// <summary>
	/// 平行移動行列
	/// </summary>
	/// <param name="translate">位置座標</param>
	/// <returns></returns>
	static Matrix4x4 MakeTranslate(const Vector3& translate);

	/// <summary>
	/// アフィン行列
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転角</param>
	/// <param name="translate">位置座標</param>
	/// <returns>アフィン行列</returns>
	static Matrix4x4 MakeAffin(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="mat">変換する行列</param>
	/// <returns>逆行列</returns>
	static Matrix4x4 MakeInverse(Matrix4x4 mat);

	/// <summary>
	/// 正射影行列生成
	/// </summary>
	/// <param name="left">画面左</param>
	/// <param name="top">画面上</param>
	/// <param name="right">画面右</param>
	/// <param name="bottom">画面下</param>
	/// <param name="nearClip">近平面</param>
	/// <param name="farClip">遠平面</param>
	/// <returns>正射影行列</returns>
	static Matrix4x4 MakeOrthGraphic(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip);

	/// <summary>
	/// 透視射影行列生成
	/// </summary>
	/// <param name="fovY">視野角</param>
	/// <param name="aspectRatio">画面アスペクト比</param>
	/// <param name="nearClip">近平面への距離</param>
	/// <param name="farClip">遠平面への距離</param>
	/// <returns>透視射影行列</returns>
	static Matrix4x4 MakePerspectiveFov(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip);

	/// <summary>
	/// ビューポート変換行列生成関数
	/// </summary>
	/// <param name="left">画面左</param>
	/// <param name="top">画面上</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <param name="minDepth">最低深度</param>
	/// <param name="maxDepth">最大深度</param>
	/// <returns>ビューポート行列</returns>
	static Matrix4x4 MakeViewport(float left, float top, float width, float height, float minDepth, float maxDepth);

public: // メンバ変数

	// 行列要素
	float m[4][4];

};

