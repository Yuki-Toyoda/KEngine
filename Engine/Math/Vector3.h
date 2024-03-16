#pragma once

// クラスの前方宣言
class Matrix4x4;

/// <summary>
/// 3次元ベクトルクラス
/// </summary>
class Vector3 final
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vector3() noexcept;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="x">x軸初期値</param>
	/// <param name="y">y軸初期値</param>
	/// <param name="z">z軸初期値</param>
	Vector3(float x, float y, float z) noexcept;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="v_">他の3次元ベクトル</param>
	Vector3(const Vector3& v) noexcept;

	// 既定のデストラクタを使用する
	~Vector3() = default;

public: // 演算子のオーバーロード

	/// <summary>
	/// 値を変更せず、自身をそのまま返すオーバーロード
	/// </summary>
	/// <returns>自身の値</returns>
	inline Vector3 operator+() const noexcept {
		// 自分自身を返す
		return *this;
	}
	/// <summary>
	/// 自身の逆ベクトルをそのまま返すオーバーロード
	/// </summary>
	/// <returns>自身の逆ベクトル</returns>
	inline Vector3 operator-() const noexcept {
		// 逆の値を返す
		return { -x, -y, -z };
	}

	/// <summary>
	/// 演算子オーバーロード(+)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator+(const Vector3& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(-)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator-(const Vector3& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(*)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator*(const Vector3& v) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(/)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator/(const Vector3& v) const noexcept;

	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3& operator=(const Vector3& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3& operator=(Vector3&& v) noexcept;

	/// <summary>
	/// 演算子オーバーロード(+=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator+=(const Vector3& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(-=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator-=(const Vector3& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(*=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator*=(const Vector3& v) noexcept;
	/// <summary>
	/// 演算子オーバーロード(/=)
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator/=(const Vector3& v) noexcept;

	/// <summary>
	/// 演算子オーバーロード(スカラー乗算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator*(const float& f) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー除算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator/(const float& f) const noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー乗算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator*=(const float& f) noexcept;
	/// <summary>
	/// 演算子オーバーロード(スカラー除算)
	/// </summary>
	/// <param name="f">右辺値</param>
	/// <returns>計算後値</returns>
	Vector3 operator/=(const float& f) noexcept;

	/// <summary>
	/// 値が等しいかどうかを調べる
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>等しい or 等しくない</returns>
	bool operator==(const Vector3& v) const noexcept;
	/// <summary>
	/// 値の一致、不一致を調べる
	/// </summary>
	/// <param name="v_">右辺値</param>
	/// <returns>一致 or 不一致</returns>
	bool operator!=(const Vector3& v) const noexcept;

	/// <summary>
	/// ワールド座標変換
	/// </summary>
	/// <param name="mat">変換する行列</param>
	/// <returns>変換後ベクトル</returns>
	Vector3 operator*(const Matrix4x4& mat) const;
	/// <summary>
	/// ワールド座標変換
	/// </summary>
	/// <param name="mat">変換する行列</param>
	/// <returns>変換後ベクトル</returns>
	Vector3& operator*=(const Matrix4x4& mat);

public: // メンバ関数

	/// <summary>
	/// 内積を求める
	/// </summary>
	/// <param name="v_">計算するベクトル</param>
	/// <returns>内積</returns>
	float Dot(const Vector3& v) const noexcept;

	/// <summary>
	/// ベクトル長さを求める
	/// </summary>
	/// <returns>ベクトル長さ</returns>
	float Length() const noexcept;

	/// <summary>
	/// クロス積を求める
	/// </summary>
	/// <param name="v_">計算するベクトル</param>
	/// <returns>クロス積</returns>
	Vector3 Cross(const Vector3& v) const noexcept;

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>正規化されたベクトル</returns>
	Vector3 Normalize() const noexcept;

public: // 静的関数

	/// <summary>
	/// ゼロベクトル
	/// </summary>
	static const Vector3 kZero;

	/// <summary>
	/// 内積計算関数
	/// </summary>
	/// <param name="v1">計算するベクトル1</param>
	/// <param name="v2">計算するベクトル2</param>
	/// <returns>内積</returns>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 内積計算関数
	/// </summary>
	/// <param name="v_">計算するベクトル</param>
	/// <returns>内積</returns>
	static float Length(const Vector3& v);

	/// <summary>
	/// クロス積計算関数
	/// </summary>
	/// <param name="v1">計算するベクトル1</param>
	/// <param name="v2">計算するベクトル2</param>
	/// <returns>クロス積</returns>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// 正規化ベクトル計算関数
	/// </summary>
	/// <param name="v_">計算するベクトル</param>
	/// <returns>正規化</returns>
	static Vector3 Normalize(const Vector3& v);

public: // メンバ変数

	// x軸
	float x;
	// y軸
	float y;
	// z軸
	float z;

};

