#include "IndexList.h"

IndexList::IndexList(int size) : kSize_(size) {
	// オフセットをリセット
	offset_ = 0;
	// 引数のサイズを元に使用状態配列をリサイズする
	array_.resize(kSize_, false);
}

IndexList::IndexList(int size, int offset) : kSize_(size) {
	// オフセットを指定
	offset_ = offset;
	// 引数のサイズを元に使用状態配列をリサイズする
	array_.resize(kSize_, false);
}

bool IndexList::CheckEmpty()
{
	// 未使用のインデックスを検索する
	for (int i = 0; i < kSize_; i++) {
		// 未使用のインデックスがある場合
		if (!array_[i]) {
			// trueを返す
			return true;
		}
	}

	// 空きがない場合falseを返す
	return false;
}

Index IndexList::UseEmpty()
{
	// 未使用のインデックスを検索する
	for (int i = 0; i < kSize_; i++) {
		// 未使用のインデックスを発見した場合
		if (!array_[i]) {
			// 使用したことを記録する
			array_[i] = true;
			// インデックスクラスを返す
			return Index(
				[&](int i) {UnUse(i); }, i + offset_
			);
		}
	}

	// 未使用のインデックスがない場合配列外参照としてエラーを返す
	assert(false);
	return Index(nullptr, -1);
}

void IndexList::UnUse(int index)
{
	// 指定されたインデックスの使用状態を変更
	array_[index] = false;
}
