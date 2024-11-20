#include "TextureConverter.h"

// 省略
using namespace DirectX;

void TextureConverter::ConvertTextureWICToDDS(const std::string& filePath)
{
	// テクスチャファイルの読み込み
	LoadWICTextureFromFile(filePath);

	// DDSファイルとして書き出し
	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	// ファイルパスをワイド文字列に変換する
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	// WICテクスチャのロード
	HRESULT hr = LoadFromWICFile(wFilePath.c_str(), WIC_FLAGS_NONE, &metadata_, scratchImage_);
	// 成功確認
	assert(SUCCEEDED(hr));

	// 読み込んだテクスチャのファイルパスを分解する
	SeparateFilePath(wFilePath);
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	// 一時変数群
	size_t pos1{};
	std::wstring exceptExt{};

	// 区切り文字 . がでてくる一番最後の部分を検索する
	pos1 = filePath.rfind('.');
	// 検索ヒット時
	if (pos1 != std::wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}

	// 区切り文字 \\ がでてくる一番最後の部分を検索する
	pos1 = filePath.rfind('\\');
	// 検索ヒット時
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存する
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		// 早期リターン
		return;
	}
	// 区切り文字 / がでてくる一番最後の部分を検索する
	pos1 = filePath.rfind('/');
	// 検索ヒット時
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存する
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		// 早期リターン
		return;
	}
	
	// 区切り文字が存在しない場合、ファイルのみとして扱う
	directoryPath_ = L"";
	fileName_ = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// ミップマップの生成開始
	ScratchImage mipChain;
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), scratchImage_.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	// ミップマップ生成の成否確認
	if (SUCCEEDED(result)) {
		// イメージとメタデータをミップマップ番で置き換える
		scratchImage_	= std::move(mipChain);
		metadata_		= scratchImage_.GetMetadata();
	}

	// 圧縮形式に変換する
	ScratchImage converted;
	result = Compress(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_,
		DXGI_FORMAT_BC7_UNORM_SRGB, TEX_COMPRESS_BC7_QUICK | TEX_COMPRESS_SRGB_OUT |
		TEX_COMPRESS_PARALLEL, 1.0f, converted);
	// 変換成否確認
	if (SUCCEEDED(result)) {
		scratchImage_	= std::move(converted);
		metadata_		= scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	metadata_.format = MakeSRGB(metadata_.format);

	// 出力ファイル名を設定
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDSファイル書き出し
	result = SaveToDDSFile(scratchImage_.GetImages(), scratchImage_.GetImageCount(), metadata_, DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(result));
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// 文字数取得
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ワイド文字列を生成
	std::wstring ws;
	ws.resize(filePathBufferSize);

	// 変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &ws[0], filePathBufferSize);

	// 返還する
	return ws;
}
