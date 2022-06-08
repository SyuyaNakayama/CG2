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

ShaderBlob::ShaderBlob(const LPCWSTR fileName, const LPCSTR target, ID3DBlob* errorBlob)
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

// ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
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

void Buffer::SetResource(size_t width, size_t height, D3D12_RESOURCE_DIMENSION Dimension)
{
	resDesc.Dimension = Dimension;
	resDesc.Width = width;
	resDesc.Height = height;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
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
void Buffer::Init()
{
	resDesc = {};
	buff = nullptr;
}

ConstBuf::ConstBuf(UINT size)
{
	Init();
	this->size = size;
	mapMaterial = nullptr;
}
void ConstBuf::Mapping()
{
	assert(SUCCEEDED(buff->Map(0, nullptr, (void**)&mapMaterial)));
}

VertexBuf::VertexBuf(UINT size)
{
	Init();
	this->size = size;
	map = nullptr;
	view = {};
}
void VertexBuf::Mapping(Vertex* vertices, const int ARRAY_NUM)
{
	assert(SUCCEEDED(buff->Map(0, nullptr, (void**)&map)));

	for (int i = 0; i < ARRAY_NUM; i++) { map[i] = vertices[i]; }
	buff->Unmap(0, nullptr);
}
void VertexBuf::CreateView()
{
	view.BufferLocation = buff->GetGPUVirtualAddress();
	view.SizeInBytes = size;
	view.StrideInBytes = sizeof(Vertex);
}

IndexBuf::IndexBuf(UINT size)
{
	Init();
	this->size = size;
	map = nullptr;
	view = {};
}
void IndexBuf::Mapping(uint16_t* indices, const int ARRAY_NUM)
{
	assert(SUCCEEDED(buff->Map(0, nullptr, (void**)&map)));

	for (int i = 0; i < ARRAY_NUM; i++) { map[i] = indices[i]; }
	buff->Unmap(0, nullptr);
}
void IndexBuf::CreateView()
{
	view.BufferLocation = buff->GetGPUVirtualAddress();
	view.Format = DXGI_FORMAT_R16_UINT;
	view.SizeInBytes = size;
}

TextureBuf::TextureBuf()
{
	Init();
	view = {};
	metadata = {};
	scratchImg = {};
	mipChain = {};
}
void TextureBuf::Transfer()
{
	HRESULT result;

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* IMG = scratchImg.GetImage(i, 0, 0);
		result = buff->WriteToSubresource(
			(UINT)i, nullptr, IMG->pixels,
			(UINT)IMG->rowPitch, (UINT)IMG->slicePitch);
		assert(SUCCEEDED(result));
	}
}
void TextureBuf::CreateView()
{
	view.Format = resDesc.Format;
	view.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	view.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	view.Texture2D.MipLevels = resDesc.MipLevels;
}

Pipeline::Pipeline()
{
	desc = {};
	state = nullptr;
}
void Pipeline::SetShader(ShaderBlob vs, ShaderBlob ps)
{
	desc.VS.pShaderBytecode = vs.blob->GetBufferPointer();
	desc.VS.BytecodeLength = vs.blob->GetBufferSize();
	desc.PS.pShaderBytecode = ps.blob->GetBufferPointer();
	desc.PS.BytecodeLength = ps.blob->GetBufferSize();
}
void Pipeline::SetSampleMask()
{
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
}
void Pipeline::SetRasterizer()
{
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	desc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に
}
void Pipeline::SetInputLayout(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT layoutNum)
{
	desc.InputLayout.pInputElementDescs = inputLayout;
	desc.InputLayout.NumElements = layoutNum;
}
void Pipeline::SetPrimitiveTopology()
{
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
}
void Pipeline::SetOthers()
{
	desc.NumRenderTargets = 1; // 描画対象は1つ
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	desc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング
}
void Pipeline::CreatePipelineState(ID3D12Device* device)
{
	assert(SUCCEEDED(device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&state))));
}

void UseBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc)
{
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// 使わない
}

void SetBlend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, int blendMode)
{
	switch (blendMode)
	{
	case BLENDMODE_ADD:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BLENDMODE_SUB:
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
		break;
	case BLENDMODE_COLORFLIP:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLENDMODE_ALPHA:
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	}
}