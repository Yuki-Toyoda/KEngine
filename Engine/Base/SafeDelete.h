#pragma once

/// <summary>
/// 生成したオブジェクトを安全に削除するための関数
/// </summary>
/// <typeparam name="T">クラス</typeparam>
/// <param name="p">削除するクラス</param>
template<class T> inline void SafeDelete(T*& p) {
	delete p;
	p = nullptr;
}
