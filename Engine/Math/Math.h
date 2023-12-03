#pragma once
#include <cassert>
#include <cmath>
#include <ctime>
#include <math.h>
#include <numbers>
#include <string>
#include "../../Externals/imgui/imgui.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4x4.h"

/// <summary>
/// 数学系関数クラス
/// </summary>
class Math
{
public:	// メンバ関数

	/// <summary>
	/// 線形補間関数(Normal)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static float Linear(float t, float start, float end, float time);
	/// <summary>
	/// 線形補間関数(Normal)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static int Linear(float t, int start, int end, float time);
	/// <summary>
	/// 線形補間関数(Normal)(Vector3)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static Vector3 Linear(float t, Vector3 start, Vector3 end, float time);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(Normal)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static float Linear(float t, float start, float end);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(Normal)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static int Linear(float t, int start, int end);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(Normal)(Vector3)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static Vector3 Linear(float t, Vector3 start, Vector3 end);

	/// <summary>
	/// 線形補間関数(EaseIn)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static float EaseIn(float t, float start, float end, float time);
	/// <summary>
	/// 線形補間関数(EaseIn)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static int EaseIn(float t, int start, int end, float time);
	/// <summary>
	/// 線形補間関数(EaseIn)(Vector3)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(Vector3)</returns>
	static Vector3 EaseIn(float t, Vector3 start, Vector3 end, float time);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(EaseIn)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static float EaseIn(float t, float start, float end);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(EaseIn)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static int EaseIn(float t, int start, int end);

	/// <summary>
	/// 線形補間関数(EaseOut)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static float EaseOut(float t, float start, float end, float time);
	/// <summary>
	/// 線形補間関数(EaseOut)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static int EaseOut(float t, int start, int end, float time);
	/// <summary>
	/// 線形補間関数(EaseOut)(Vector3)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(Vector3)</returns>
	static Vector3 EaseOut(float t, Vector3 start, Vector3 end, float time);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(EaseOut)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static float EaseOut(float t, float start, float end);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(EaseOut)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static int EaseOut(float t, int start, int end);

	/// <summary>
	/// 線形補間関数(EaseInOut)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static float EaseInOut(float t, float start, float end, float time);
	/// <summary>
	/// 線形補間関数(EaseInOut)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(float)</returns>
	static int EaseInOut(float t, int start, int end, float time);
	/// <summary>
	/// 線形補間関数(EaseInOut)(Vector3)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <param name="time">かかる秒数</param>
	/// <returns>イージングされた値(Vector3)</returns>
	static Vector3 EaseInOut(float t, Vector3 start, Vector3 end, float time);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(EaseInOut)(float)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static float EaseInOut(float t, float start, float end);
	/// <summary>
	/// イージングにかかる秒数を指定しない線形補間関数(EaseInOut)(int)
	/// </summary>
	/// <param name="t">現在のt</param>
	/// <param name="start">開始値</param>
	/// <param name="end">終了値</param>
	/// <returns>イージングされた値(float)</returns>
	static int EaseInOut(float t, int start, int end);

	/// <summary>
	/// 引数の値を 最小値 から 最大値 の値に収める関数
	/// </summary>
	/// <param name="a">値を設定する関数</param>
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns></returns>
	static float Clamp(float a, float min, float max);

	// srandセット関数
	// 返り値：なし
	// 引数なし
	// 指定された範囲内のランダムな値を返す関数(int型)
	static void SetSrand();

	// ランダム関数（int型）
	// 返り値：ランダムの値（int型）
	// 引数：
	// min ... 最小値
	// max ... 最大値
	// 指定された範囲内のランダムな値を返す関数(int型)
	static int Random(int min, int max);

	// ランダム関数（float型）
	// 返り値：ランダムの値（float型）
	// 引数：
	// min ... 最小値
	// max ... 最大値
	// num ... 少数点何桁まで表示するか
	// 指定された桁数で、範囲内のランダムな値を返す関数(float型)
	static float RandomF(float min, float max, int num);

	/// <summary>
	/// ベクトル内積計算関数
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns>内積</returns>
	static float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ベクトル長さ計算関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>ベクトル長さ</returns>
	static float Length(const Vector3& v);

	/// <summary>
	/// 最短角度補間関数
	/// </summary>
	/// <param name="a">角度a</param>
	/// <param name="b">角度b</param>
	/// <param name="t">補間割合(0 ~ 1)</param>
	/// <returns></returns>
	static float LerpShortAngle(float a, float b, float t);

	/// <summary>
	/// ベクトル正規化関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <returns>正規化ベクトル</returns>
	static Vector3 Normalize(const Vector3& v);

	/// <summary>
	/// 行列のクロス積を求める関数
	/// </summary>
	/// <param name="v1">ベクトル1</param>
	/// <param name="v2">ベクトル2</param>
	/// <returns>クロス積</returns>
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>
	/// ワールド座標変換関数
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="m">4x4行列</param>
	/// <returns>ワールド座標</returns>
	static Vector3 Transform(const Vector3& v, const Matrix4x4& m);

	/// <summary>
	/// ワールド座標変換関数(平行移動を考慮しない)
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="m">4x4行列</param>
	/// <returns>ワールド座標</returns>
	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

	/// <summary>
	/// 逆行列計算関数
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	static Matrix4x4 Inverse(const Matrix4x4 m);

	/// <summary>
	/// 単位行列生成関数
	/// </summary>
	/// <returns>単位行列</returns>
	static Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// 平行移動行列生成関数
	/// </summary>
	/// <param name="translate">位置座標</param>
	/// <returns>平行移動行列</returns>
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	/// <summary>
	/// 拡大縮小行列生成関数
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <returns>拡大縮小行列</returns>
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	/// <summary>
	/// X軸回転行列生成関数
	/// </summary>
	/// <param name="X">回転角度(ラジアン)</param>
	/// <returns>X軸回転行列</returns>
	static Matrix4x4 MakeRotateXMatrix(const float& X);
	/// <summary>
	/// Y軸回転行列生成関数
	/// </summary>
	/// <param name="Y">回転角度(ラジアン)</param>
	/// <returns>Y軸回転行列</returns>
	static Matrix4x4 MakeRotateYMatrix(const float& Y);
	/// <summary>
	/// Z軸回転行列生成関数
	/// </summary>
	/// <param name="Z">回転角度(ラジアン)</param>
	/// <returns>Z軸回転行列</returns>
	static Matrix4x4 MakeRotateZMatrix(const float& Z);

	/// <summary>
	/// 回転行列生成関数
	/// </summary>
	/// <param name="rotate">回転角度(Vector3)(radian)</param>
	/// <returns>回転行列</returns>
	static Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate);

	/// <summary>
	/// アフィン行列生成関数
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転角</param>
	/// <param name="translate">位置座標</param>
	/// <returns>アフィン行列</returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	/// <summary>
	/// アフィン変換行列生成関数
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">位置座標</param>
	/// <returns></returns>
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	/// <summary>
	/// 正射影行列生成関数
	/// </summary>
	/// <param name="left">画面左</param>
	/// <param name="top">画面上</param>
	/// <param name="right">画面右</param>
	/// <param name="bottom">画面下</param>
	/// <param name="nearClip">近平面</param>
	/// <param name="farClip">遠平面</param>
	/// <returns>正射影行列</returns>
	static Matrix4x4 MakeOrthGraphicMatrix(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip);

	/// <summary>
	/// 透視射影行列生成関数
	/// </summary>
	/// <param name="fovY">視野角</param>
	/// <param name="aspectRatio">画面アスペクト比</param>
	/// <param name="nearClip">近平面への距離</param>
	/// <param name="farClip">遠平面への距離</param>
	/// <returns>透視射影行列</returns>
	static Matrix4x4 MakePerspectiveFovMatrix(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip);

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
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	/// <summary>
	/// 任意軸の回転行列を生成する関数
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角</param>
	/// <returns>任意軸の回転行列</returns>
	static Matrix4x4 MakeRotateAxisAngle(Vector3 axis, float angle);

	/// <summary>
	/// ベクトルを回転させる回転行列を生成する関数
	/// </summary>
	/// <param name="from">開始ベクトル</param>
	/// <param name="to">終端ベクトル</param>
	/// <returns>ベクトルを回転させる回転行列</returns>
	static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

	/// <summary>
	/// マトリックス内のパラメータを表示する関数
	/// </summary>
	/// <param name="m">マトリックス</param>
	static void MatrixImGui(Matrix4x4 m, std::string paramName);

	/// <summary>
	/// 単位クォータニオンの生成関数
	/// </summary>
	/// <returns>乗法単位元</returns>
	static Quaternion MakeIdentityQuaternion();

	/// <summary>
	/// 乗法単位元
	/// </summary>
	/// <param name="v">ベクトル</param>
	/// <param name="w">実数部</param>
	/// <returns>乗法単位元</returns>
	static Quaternion MakeQuaternion(const Vector3& v);

	/// <summary>
	/// 共役
	/// </summary>
	/// <param name="q">変換するクォータニオン</param>
	/// <returns>共役</returns>
	static Quaternion Conjugate(const Quaternion& q);

	/// <summary>
	/// クォータニオンの長さを求める
	/// </summary>
	/// <param name="q">長さを求めるクォータニオン</param>
	/// <returns>クォータニオンの長さ</returns>
	static float Length(const Quaternion& q);
	/// <summary>
	/// クォータニオンの正規化
	/// </summary>
	/// <param name="q">正規化するクォータニオン</param>
	/// <returns>正規化されたクォータニオン</returns>
	static Quaternion Normalize(const Quaternion& q);

	/// <summary>
	/// 逆クォータニオン生成関数
	/// </summary>
	/// <param name="q">逆にするクォータニオン</param>
	/// <returns>逆クォータニオン</returns>
	static Quaternion Inverse(const Quaternion& q);

	/// <summary>
	/// 任意軸回転を表すクォータニオン生成関数
	/// </summary>
	/// <param name="v">回転軸</param>
	/// <param name="angle">回転角</param>
	/// <returns>任意軸回転を表すクォータニオン</returns>
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& v, float angle);
	/// <summary>
	/// ベクトルをクォータニオンで回転させた後のベクトルを求める関数
	/// </summary>
	/// <param name="v">始点ベクトル</param>
	/// <param name="q">回転量</param>
	/// <returns>回転ベクトル</returns>
	static Vector3 RoatateVector(const Vector3& v, const Quaternion& q);

	/// <summary>
	/// クォータニオンを行列に変換する関数
	/// </summary>
	/// <param name="q">返還するクォータニオン</param>
	/// <returns>行列</returns>
	static Matrix4x4 QuaternionToMatrix(const Quaternion& q);
};