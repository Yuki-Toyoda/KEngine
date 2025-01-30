#pragma once
#include <Windows.h>
#include <format>
#include <string>

/// <summary>
/// ログの出力などのデバック関係を行うクラス
/// </summary>
class Debug
{
public: // メンバ関数

	/// <summary>
	/// 出力ウィンドウにログを出力する関数
	/// </summary>
	/// <param name="message">出力する文字列</param>
	static void Log(const std::string& message);

	/// <summary>
	/// string->wstringへの変換を行う関数
	/// </summary>
	/// <param name="str">変換を行う文字列</param>
	/// <returns>変換された文字列</returns>
	static std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// wstring->stringへの変換を行う関数
	/// </summary>
	/// <param name="str">変換を行う文字列</param>
	/// <returns>変換された文字列</returns>
	static std::string ConvertString(const std::wstring& str);

};

