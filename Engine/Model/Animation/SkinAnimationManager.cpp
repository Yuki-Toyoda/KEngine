#include "SkinAnimationManager.h"
#include "../../Debug/Debug.h"
#include <cassert>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void SkinAnimationManager::Update(Skelton* skelton)
{
	// どれかしらのアニメーションを再生していれば
	if (playingAnimName_ != "") {
		// 再生中アニメーション再生
		animations_[playingAnimName_].Update(skelton);
	}
}

void SkinAnimationManager::LoadAnimations(const aiScene* scene)
{
	// 全アニメーション分ループ
	for (uint32_t animationCount = 0; animationCount < scene->mNumAnimations; animationCount++) {
		// 引数で指定されたアニメーションを読み取る
		aiAnimation* animationAssimp = scene->mAnimations[animationCount];
		// アニメーション名取得
		aiString a = animationAssimp->mName;
		std::string animName = a.C_Str();

		// 新規アニメーションを読み込む
		animations_[animName].LoadAnimation(animationAssimp, animName);
	}
}

void SkinAnimationManager::PlayAnimation(const std::string& animName, bool isLoop, float startTime)
{
	// アニメーションが存在した場合
	if (animations_.count(animName)) {
		// アニメーション間の補間がまだできていないので再生段階で再生中アニメーションを停止させる
		StopAnimation(playingAnimName_);

		// 再生するアニメーション名の取得
		playingAnimName_ = animName;

		// 指定された名称のアニメーションを再生する
		animations_[animName].Start(startTime);
		// アニメーションをループさせるか
		animations_[animName].isLoop_ = isLoop;
	}
	else {
		// ログ出力
		Debug::Log("Play Error - Animation Not Found.\n");
		// 停止させる
		assert(false);
	}
}

void SkinAnimationManager::StopPlayingAnimation()
{
	// 再生中アニメーションを停止させる
	StopAnimation(playingAnimName_);
}

bool SkinAnimationManager::GetIsPlayingAnimation()
{
	// そもそもアニメーション名が指定されていない場合は再生されていない
	if (playingAnimName_ == "") {
		return false;
	}
	
	// アニメーションが再生中であれば
	if (animations_[playingAnimName_].isPlay_) {
		return true;
	}
	// ここまで到達すれば再生されていない
	return false;
}

bool SkinAnimationManager::GetIsPlayingAnimation(const std::string& animName)
{
	// そもそもアニメーション名が指定されていない場合は再生されていない
	if (playingAnimName_ == "") {
		return false;
	}

	// アニメーションが存在した場合
	if (animations_.count(animName)) {
		// アニメーションが再生中であれば
		if (animations_[animName].isPlay_) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// ログ出力
		Debug::Log("Play Error - Animation Not Found.\n");
		// 停止させる
		assert(false);
	}
	
	// ここまで到達すれば再生されていない
	return false;
}

float SkinAnimationManager::GetPlayingAnimationProgress()
{
	// アニメーション進捗度を返す
	return animations_[playingAnimName_].GetProgress();
}

void SkinAnimationManager::StopAnimation(const std::string& animName)
{
	// 引数で指定された名称のアニメーションを停止させる
	animations_[animName].Stop();
}
