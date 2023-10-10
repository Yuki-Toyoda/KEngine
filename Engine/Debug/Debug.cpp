#include "Debug.h"

/// <summary>
/// 出力ウィンドウにログを出力する関数
/// </summary>
/// <param name="message">出力する文字列</param>
void Debug::Log(const std::string& message) {
	// ログを出力する
	OutputDebugStringA(message.c_str());
}

/// <summary>
/// string->wstringへの変換を行う関数
/// </summary>
/// <param name="str">変換を行う文字列</param>
/// <returns>変換された文字列</returns>
std::wstring Debug::ConvertString(const std::string& str) {
    if (str.empty()) {
        return std::wstring();
    }

    auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
    if (sizeNeeded == 0) {
        return std::wstring();
    }
    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
    return result;
}

/// <summary>
/// wstring->stringへの変換を行う関数
/// </summary>
/// <param name="str">変換を行う文字列</param>
/// <returns>変換された文字列</returns>
std::string Debug::ConvertString(const std::wstring& str) {
    if (str.empty()) {
        return std::string();
    }

    auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
    if (sizeNeeded == 0) {
        return std::string();
    }
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
    return result;
}