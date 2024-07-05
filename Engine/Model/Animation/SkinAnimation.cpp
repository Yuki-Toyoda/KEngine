#include "SkinAnimation.h"
#include "../Component/Skelton.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>

void SkinAnimation::Init()
{
	// アニメーション秒数リセット
	animationTime_ = 0.0f;

	// 再生していない状態に
	isPlay_ = false;
	// アニメーションのループを切る
	isLoop_ = false;
}

void SkinAnimation::Update(Skelton* skelton)
{
	// アニメーションが再生されていない場合早期リターン
	if (!isPlay_) { return; }

	// 60FPS固定でアニメーションの秒数加算
	animationTime_ += 1.0f / 60.0f;

	// ループ再生を行う場合
	if (isLoop_) {
		// 最終秒数に到達していた場合、最初の秒数に戻す
		animationTime_ = std::fmod(animationTime_, duration_);
	}
	else {
		// 最終秒数に到達していた場合
		if (animationTime_ > duration_) {
			// 終了しているので再生中ではない
			isPlay_ = false;
		}
	}

	// スケルトンにアニメーションを適用させる
	for (Joint& joint : skelton->joints_) {
		// 対象のジョイントのアニメーションがあれば値の適用を行う
		if (auto it = nodeAnimations_.find(joint.name_); it != nodeAnimations_.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform_.translate_ = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
			joint.transform_.rotate_ = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
			joint.transform_.scale_ = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
		}
	}
}

void SkinAnimation::Start()
{
	// 開始
	Start(0.0f);
}

void SkinAnimation::Start(float startSec)
{
	// アニメーションを開始させる
	isPlay_ = true;
	// 秒数を引数の値でリセット
	animationTime_ = startSec;
}

void SkinAnimation::Stop()
{
	// アニメーションを停止させる
	isPlay_ = false;
}

void SkinAnimation::LoadAnimation(const aiAnimation* anim, std::string& name)
{
	// アニメーション名取得
	animName_ = name;
	// 時間の単位を秒に変換
	duration_ = float(anim->mDuration / anim->mTicksPerSecond);

	// assimpでは個々のNodeAnimationをchannelと呼んでいるので、channelを回してNodeAnimationの情報を取ってくる
	for (uint32_t channelIndex = 0; channelIndex < anim->mNumChannels; channelIndex++) {
		// ノードアニメーションを取得
		aiNodeAnim* nodeAnimationAssimp = anim->mChannels[channelIndex];
		// ノードアニメーションを任意の構造体の形式に変換
		NodeAnimation& nodeAnimation = nodeAnimations_[nodeAnimationAssimp->mNodeName.C_Str()];

		// 位置座標のキー数分ループ
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; keyIndex++) {
			// 位置座標のキー情報を取得する
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			// 位置座標のキーフレーム
			keyframe<Vector3> keyFrame;

			// キーフレーム秒数を取得する
			keyFrame.time = float(keyAssimp.mTime / anim->mTicksPerSecond);
			// キーフレーム値を取得(右手から左手座標系に変換する)
			keyFrame.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };

			// ノードアニメーション配列に値を追加
			nodeAnimation.translate.keyframes.push_back(keyFrame);
		}

		// 回転のキー数分ループ
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; keyIndex++) {
			// 回転のキー情報を取得する
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			// 回転のキーフレーム
			keyframe<Quaternion> keyFrame;

			// キーフレーム秒数を取得する
			keyFrame.time = float(keyAssimp.mTime / anim->mTicksPerSecond);
			// キーフレーム値を取得(右手から左手座標系に変換する)
			keyFrame.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };

			// ノードアニメーション配列に値を追加
			nodeAnimation.rotate.keyframes.push_back(keyFrame);
		}

		// 拡縮のキー数分ループ
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; keyIndex++) {
			// 拡縮のキー情報を取得する
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			// 拡縮のキーフレーム
			keyframe<Vector3> keyFrame;

			// キーフレーム秒数を取得する
			keyFrame.time = float(keyAssimp.mTime / anim->mTicksPerSecond);
			// キーフレーム値を取得(右手から左手座標系に変換する)
			keyFrame.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };

			// ノードアニメーション配列に値を追加
			nodeAnimation.scale.keyframes.push_back(keyFrame);
		}
	}
}

Vector3 SkinAnimation::CalculateValue(const std::vector<keyframe<Vector3>>& keyframes, float time)
{
	// キーがない場合返す値がないので停止させる
	assert(!keyframes.empty());

	// キーが１つ、または秒数がキーフレーム前なら最初の値とする
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		// 最初の値を返す
		return keyframes[0].value;
	}

	// キーフレーム数分ループ
	for (size_t index = 0; index < keyframes.size() - 1; index++) {
		// 次のインデックスを求める
		size_t nextIndex = index + 1;

		// 現在のインデックスと次のインデックスの2つのキーフレームを取得し、範囲内に秒数があるか判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			// 線形補間を行う
			return KLib::Lerp<Vector3>(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	// ここまで来た場合は最後の値を返す
	return (*keyframes.rbegin()).value;
}

Quaternion SkinAnimation::CalculateValue(const std::vector<keyframe<Quaternion>>& keyframes, float time)
{
	// キーがない場合返す値がないので停止させる
	assert(!keyframes.empty());

	// キーが１つ、または秒数がキーフレーム前なら最初の値とする
	if (keyframes.size() == 1 || time <= keyframes[0].time) {
		// 最初の値を返す
		return keyframes[0].value;
	}

	// キーフレーム数分ループ
	for (size_t index = 0; index < keyframes.size() - 1; index++) {
		// 次のインデックスを求める
		size_t nextIndex = index + 1;

		// 現在のインデックスと次のインデックスの2つのキーフレームを取得し、範囲内に秒数があるか判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			// 線形補間を行う
			return Quaternion::Slerp(t, keyframes[index].value, keyframes[nextIndex].value);
		}
	}

	// ここまで来た場合は最後の値を返す
	return (*keyframes.rbegin()).value;
}


