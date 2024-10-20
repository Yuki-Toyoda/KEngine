#include "Audio.h"

#include <cassert>
#include <fstream>
#include <windows.h>

#pragma comment(lib, "xaudio2.lib")

#include "../Debug/Debug.h"

Audio* Audio::GetInstance()
{
	static Audio instance;
	// インスタンスを返す
	return &instance;
}

void Audio::Init(const std::string& directoryPath)
{
	// ディレクトリパスを指定
	directoryPath_ = directoryPath;

	// 結果確認用
	HRESULT result = S_FALSE;
	IXAudio2MasteringVoice* masterVoice;

	// MeadiaFoundactionの初期化
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	// 番号初期化
	indexSoundData_ = 0u;
	indexVoice_ = 0u;
}

void Audio::Finalize()
{
	// XAudio2解放
	xAudio2_.Reset();
	// 音声データ解放
	for (auto& soundData : soundDatas_) {
		Unload(&soundData);
	}

	MFShutdown();
}

uint32_t Audio::LoadWave(const std::string& fileName)
{
	// 最大音声数を超過していた場合はエラー
	assert(indexSoundData_ < kMaxAudioData);
	// 返還用インスタンスの生成
	uint32_t handle = indexSoundData_;

	// 読み込み済みサウンドデータを検索
	auto it = std::find_if(soundDatas_.begin(), soundDatas_.end(), [&](const auto& soundData) {
		return soundData.name_ == fileName;
		});
	if (it != soundDatas_.end()) {
		// 読み込み済みサウンドデータの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(soundDatas_.begin(), it));
		return handle;
	}

	// ディレクトリパスとファイル名を連結してフルパスを得る
	bool currentRelative = false;
	if (2 < fileName.size()) {
		currentRelative = (fileName[0] == '.') && (fileName[1] == '/');
	}
	std::string path = currentRelative ? fileName : directoryPath_ + fileName;
	// wstring形式に返還
	std::wstring fullpath = Debug::ConvertString(path);

	// ソースリーダーを生成する
	IMFSourceReader* pMFSourceReader{ nullptr };
	// フルパスからデータ読み込み
	MFCreateSourceReaderFromURL(fullpath.c_str(), NULL, &pMFSourceReader);

	// 読み込むファイルの種類を特定
	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	pMFSourceReader->SetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	pMFSourceReader->GetCurrentMediaType(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &pMFMediaType);

	WAVEFORMATEX* waveFormat{ nullptr };
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

	std::vector<BYTE> mediaData;
	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pMFSourceReader->ReadSample(static_cast<DWORD>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		mediaData.resize(mediaData.size() + cbCurrentLength);
		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}

	// 書き込むサウンドデータの参照
	SoundData& soundData = soundDatas_.at(handle);

	BYTE* pBuffer = new BYTE[mediaData.size()];
	memcpy(pBuffer, mediaData.data(), mediaData.size());

	soundData.wfex = *waveFormat;
	soundData.pBuffer = pBuffer;
	soundData.bufferSize = static_cast<unsigned int>(mediaData.size());
	soundData.name_ = fileName;

	indexSoundData_++;

	pMFMediaType->Release();
	pMFSourceReader->Release();

	return handle;
}

void Audio::Unload(SoundData* soundData)
{
	// バッファのメモリを解放
	delete[] soundData->pBuffer;

	// 様々な
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

uint32_t Audio::PlayWave(uint32_t soundDataHandle, bool loopFlag, float volume)
{
	HRESULT result;

	assert(soundDataHandle <= soundDatas_.size());

	// サウンドデータの参照を取得
	SoundData& soundData = soundDatas_.at(soundDataHandle);
	// 未読み込みの検出
	assert(soundData.bufferSize != 0);

	uint32_t handle = indexVoice_;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex, 0, 2.0f, &voiceCallback_);
	assert(SUCCEEDED(result));

	// 再生中データ
	Voice* voice = new Voice();
	voice->handle = handle;
	voice->sourceVoice = pSourceVoice;
	// 再生中データコンテナに登録
	voices_.insert(voice);

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.pContext = voice;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loopFlag) {
		// 無限ループ
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	pSourceVoice->SetVolume(volume);
	result = pSourceVoice->Start();

	indexVoice_++;

	return handle;
}

void Audio::StopWave(uint32_t voiceHandle)
{
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見
	if (it != voices_.end()) {
		(*it)->sourceVoice->DestroyVoice();

		voices_.erase(it);
	}
}

bool Audio::IsPlaying(uint32_t voiceHandle)
{
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見。再生終わってるのかどうかを判断
	if (it != voices_.end()) {
		XAUDIO2_VOICE_STATE state{};
		(*it)->sourceVoice->GetState(&state);
		return state.SamplesPlayed != 0;
	}
	return false;
}

void Audio::SetVolume(uint32_t voiceHandle, float volume)
{
	// 再生中リストから検索
	auto it = std::find_if(
		voices_.begin(), voices_.end(), [&](Voice* voice) { return voice->handle == voiceHandle; });
	// 発見
	if (it != voices_.end()) {
		(*it)->sourceVoice->SetVolume(volume);
	}
}