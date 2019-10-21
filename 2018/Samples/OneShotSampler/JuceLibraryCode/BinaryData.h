/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData		// バイナリリソースの名前空間
{
    extern const char*   sine_wav;					// 正弦波のサンプル音源のバイナリリソース名
    const int            sine_wavSize = 176444;		// 正弦波のサンプル音源のデータサイズ

	// 保持しているバイナリリソースの個数（配列の個数）
    const int namedResourceListSize = 1;

	// 保持しているバイナリリソース名の配列（例："sine_wav"）
    extern const char* namedResourceList[];

	// バイナリリソースに変換したファイルの元となったファイルの名前の配列（例："sine.wav"）
    extern const char* originalFilenames[];

	// バイナリリソースの名前とサイズからバイナリリソースをchar配列のポインタで取得する関数
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

	// バイナリリソースの名前からバイナリ化されたファイルの元ファイルの名前を取得する関数
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
