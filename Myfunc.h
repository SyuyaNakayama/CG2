#pragma once
#include <d3dcompiler.h>
#include <string>
#include <cassert>

HRESULT ReadShaderFile(ID3DBlob** shBlob, ID3DBlob** errorBlob, LPCWCHAR fileName, LPCSTR target)
{
	return D3DCompileFromFile(
		fileName, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", target, // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		shBlob, errorBlob);
}