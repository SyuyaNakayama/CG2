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

WindowsAPI::WindowsAPI(WNDPROC lpfnWndProc, int window_width, int window_height)
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = lpfnWndProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

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

void Buffer::SetResource(D3D12_RESOURCE_DIMENSION Dimension)
{
	resDesc.Dimension = Dimension;
	resDesc.Width = size;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void Buffer::CreateBuffer(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProp)
{
	assert(SUCCEEDED(
		device->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&buff))));
}

void Buffer::Init(UINT size)
{
	resDesc = {};
	buff = nullptr;
	this->size = size;
}

ConstBuf::ConstBuf(UINT size)
{
	Init(size);
	mapMaterial = nullptr;
}

void ConstBuf::Mapping()
{
	assert(SUCCEEDED(buff->Map(0, nullptr, (void**)&mapMaterial)));
}

VertexBuf::VertexBuf(UINT size)
{
	Init(size);
	map = nullptr;
}

void VertexBuf::Mapping(XMFLOAT3* vertices, const int ARRAY_NUM)
{
	assert(SUCCEEDED(buff->Map(0, nullptr, (void**)&map)));

	for (int i = 0; i < ARRAY_NUM; i++) { map[i] = vertices[i]; }
	buff->Unmap(0, nullptr);
}

IndexBuf::IndexBuf(UINT size)
{
	Init(size);
	map = nullptr;
}

void IndexBuf::Mapping(uint16_t* indices, const int ARRAY_NUM)
{
	assert(SUCCEEDED(buff->Map(0, nullptr, (void**)&map)));

	for (int i = 0; i < ARRAY_NUM; i++) { map[i] = indices[i]; }
	buff->Unmap(0, nullptr);
}