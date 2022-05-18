#include "MyClass.h"
void DirectInput::Initialize(WNDCLASSEX w)
{
	assert(SUCCEEDED(
		DirectInput8Create(w.hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&input, nullptr)));
}

void Keyboard::GetInstance(WNDCLASSEX w)
{
	Initialize(w);
	assert(SUCCEEDED(input->CreateDevice(GUID_SysKeyboard, &device, NULL)));
}

void Keyboard::SetDataStdFormat()
{
	assert(SUCCEEDED(device->SetDataFormat(&c_dfDIKeyboard)));// 標準形式
}

void Keyboard::SetCooperativeLevel(HWND hwnd)
{
	assert(SUCCEEDED(device->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY)));
}

void Keyboard::GetDeviceState()
{
	device->GetDeviceState(sizeof(key), key);
}

void Keyboard::TransferOldkey()
{
	for (size_t i = 0; i < sizeof(oldkey); i++) { oldkey[i] = key[i]; }
}

bool Keyboard::isInput(const int KEY)
{
	if (key[KEY]) { return true; }
	return false;
}

bool Keyboard::isTrigger(const int KEY)
{
	return (!oldkey[KEY] && key[KEY]);
	return false;
}

void ShaderBlob::CompileFromFile(const LPCWSTR fileName, const LPCSTR target, ID3DBlob* errorBlob)
{
	HRESULT result;

	result = D3DCompileFromFile(
		fileName, // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", target, // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&blob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

WindowsAPI::WindowsAPI(UINT cbSize, WNDPROC lpfnWndProc, LPCWSTR lpszClassName, HINSTANCE hInstance,
	HCURSOR hCursor, int window_width, int window_height)
{
	w.cbSize = cbSize;
	w.lpfnWndProc = lpfnWndProc; // ウィンドウプロシージャを設定
	w.lpszClassName = lpszClassName; // ウィンドウクラス名
	w.hInstance = hInstance; // ウィンドウハンドル
	w.hCursor = hCursor; // カーソル指定

	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	wrc = { 0, 0, window_width, window_height };
	// 自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(w.lpszClassName, // クラス名
		L"LE2A_16_ナカヤマ_シュウヤ_CG2", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW, // 標準的なウィンドウスタイル
		CW_USEDEFAULT, // 表示X座標(OSに任せる)
		CW_USEDEFAULT, // 表示Y座標(OSに任せる)
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr, // 親ウィンドウハンドル
		nullptr, // メニューハンドル
		w.hInstance, // 呼び出しアプリケーションハンドル
		nullptr); // オプション
}