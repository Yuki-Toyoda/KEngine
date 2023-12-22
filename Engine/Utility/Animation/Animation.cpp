#include "Animation.h"

void Animation::Init(const std::string& name)
{
	// アニメーション名の取得
	name_ = name;

	// キー配列クリア
	animationKeys_.clear();
}

void Animation::Update()
{
	// 配列内の全てのキーを更新する
	for (auto& keys : animationKeys_) {
		std::visit([](auto& key) { key.Update(); }, keys);
	}
}

void Animation::AddItem()
{

}

void Animation::SetItem()
{

}

void Animation::ApplyItem()
{

}



