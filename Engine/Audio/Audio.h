#pragma once
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <xaudio2.h>

/// <summary>
/// 音再生クラス
/// </summary>
class Audio
{
public: // パブリックな静的メンバ変数

	// 音声データの最大数
	static const int kMaxAudioData = 256;

public: // サブクラス

	// チャンクヘッダ構造体
	struct ChunkHeader {
		char id[4]; // チャンクなどのID
		int32_t size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk; // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; // "fmt "
		WAVEFORMATEX fmt; // 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		BYTE* pBuffer;
		// バッファのサイズ
		unsigned int bufferSize;
		// 名前
		std::string name_;
	};

	// 再生中データ
	struct Voice {
		uint32_t handle = 0u;
		IXAudio2SourceVoice* sourceVoice = nullptr;
	};

	/// <summary>
	/// オーディオコールバック
	/// </summary>
	class XAudio2VoiceCallback : public IXAudio2VoiceCallback {
	public:
		// ボイス処理パスの開始時
		STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) { BytesRequired; };
		// ボイス処理パスの終了時
		STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
		// バッファストリームの再生が終了した時
		STDMETHOD_(void, OnStreamEnd)(THIS) {};
		// バッファの使用開始時
		STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) { pBufferContext; };
		// バッファの末尾に達した時
		STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext) { pBufferContext; };
		// 再生がループ位置に達した時
		STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) { pBufferContext; };
		// ボイスの実行エラー時
		STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT Error) {
			pBufferContext; 
			Error;
		};
	};

private: // デストラクタ

	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = delete;
	const Audio& operator=(const Audio&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(const std::string& directoryPath = "./Resources/");

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAV音声読み込み
	/// </summary>
	/// <param name="filename">WAVファイル名</param>
	/// <returns>サウンドデータハンドル</returns>
	uint32_t LoadWave(const std::string& fileName);

	/// <summary>
	/// サウンドデータの解放
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	void Unload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="soundDataHandle">サウンドデータハンドル</param>
	/// <param name="loopFlag">ループ再生フラグ</param>
	/// <param name="volume">ボリューム
	/// 0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする</param>
	/// <returns>再生ハンドル</returns>
	uint32_t PlayWave(uint32_t soundDataHandle, bool loopFlag = false, float volume = 1.0f);

	/// <summary>
	/// 音声停止
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	void StopWave(uint32_t voiceHandle);

	/// <summary>
	/// 音声再生中かどうか
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	/// <returns>音声再生中かどうか</returns>
	bool IsPlaying(uint32_t voiceHandle);

	/// <summary>
	/// 音量設定
	/// </summary>
	/// <param name="voiceHandle">再生ハンドル</param>
	/// <param name="volume">ボリューム
	/// 0で無音、1がデフォルト音量。あまり大きくしすぎると音割れする</param>
	void SetVolume(uint32_t voiceHandle, float volume);

private: // メンバ変数

	// XAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	// サウンドデータコンテナ
	std::array<SoundData, kMaxAudioData> soundDatas_;
	// 再生中データコンテナ
	std::set<Voice*> voices_;
	// サウンド格納ディレクトリ
	std::string directoryPath_;
	// 次に使うサウンドデータの番号
	uint32_t indexSoundData_ = 0u;
	// 次に使う再生中データの番号
	uint32_t indexVoice_ = 0u;
	// オーディオコールバック
	XAudio2VoiceCallback voiceCallback_;

};

