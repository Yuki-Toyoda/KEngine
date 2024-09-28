#include "SkinAnimation.h"

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
	if (!isPlay_ && !isTransitioning_) { return; }

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

	// 補完中であれば
	if (isTransitioning_) {
		// 60FPS固定で補完秒数加算
		currentTransitionTime_ += 1.0f / 60.0f;

		// 最終秒数に到達していた場合
		if (currentTransitionTime_ > transitionDuration_) {
			// 補完終了
			isTransitioning_ = false;
		}

		// スケルトンにアニメーションを適用させる
		for (Joint& joint : skelton->joints_) {
			// 対象のジョイントのアニメーションがあれば値の適用を行う
			if (auto it = nodeAnimations_.find(joint.name_); it != nodeAnimations_.end()) {
				const NodeAnimation& rootNodeAnimation = (*it).second;

				// 補完元のスケルトンのトランスフォームを取得
				QuatWorldTransform wt = prevSkelton_.joints_[prevSkelton_.jointMap_[joint.name_]].transform_;

				joint.transform_.translate_ = CalculateValue(wt.translate_, rootNodeAnimation.translate.keyframes, animationTime_);
				joint.transform_.rotate_ = CalculateValue(wt.rotate_, rootNodeAnimation.rotate.keyframes, animationTime_);
				joint.transform_.scale_ = CalculateValue(wt.scale_, rootNodeAnimation.scale.keyframes, animationTime_);
			}
		}
	}
	else { // 補完中ではない場合
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
	// 補完を行わない設定に
	isTransitioning_ = false;
}


void SkinAnimation::Start(float transitionTime, Skelton* skelton)
{
	// アニメーション開始
	Start(0.0f);
	// 補完元アニメーションの取得
	prevSkelton_ = *skelton;
	// 現在補完秒数リセット
	currentTransitionTime_ = 0.0f;
	// 補完秒数取得
	transitionDuration_ = transitionTime;
	// 補完中状態に
	isTransitioning_ = true;
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

float SkinAnimation::GetProgress() const
{
	// 現在のタイマーの進行度を返す
	return std::clamp(animationTime_ / duration_, 0.f, 1.f);
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

Vector3 SkinAnimation::CalculateValue(const Vector3& prevKeyframes, const std::vector<keyframe<Vector3>>& keyframes, float time)
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
			// 計算結果一時保存用
			Vector3 tempValue = Vector3();
			
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			// 線形補間を行う
			tempValue = KLib::Lerp<Vector3>(keyframes[index].value, keyframes[nextIndex].value, t);
			// 補完用のtを求める
			float transitionT = currentTransitionTime_ / transitionDuration_;
			// 線形補間によるアニメーションの補完を行う
			return KLib::Lerp<Vector3>(prevKeyframes, tempValue, transitionT);
		}
	}

	// ここまで来た場合は最後の値を返す
	return (*keyframes.rbegin()).value;
}

Quaternion SkinAnimation::CalculateValue(const Quaternion& prevKeyframes, const std::vector<keyframe<Quaternion>>& keyframes, float time)
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
			// 計算結果一時保存用
			Quaternion tempValue = Quaternion();
			
			// 範囲内を補完する
			float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
			// 線形補間を行う
			tempValue = Quaternion::Slerp(t, keyframes[index].value, keyframes[nextIndex].value);
			// 補完用のtを求める
			float transitionT = currentTransitionTime_ / transitionDuration_;
			// 線形補間によるアニメーションの補完を行う
			return Quaternion::Slerp(transitionT, prevKeyframes, tempValue);
		}
	}

	// ここまで来た場合は最後の値を返す
	return (*keyframes.rbegin()).value;
}
