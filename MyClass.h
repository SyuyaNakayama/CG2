#pragma once
#include <dinput.h>
#include <cassert>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;

struct ConstBufferDataMaterial { XMFLOAT4 color; };
enum BlendMode
{
	BLENDMODE_ADD,
	BLENDMODE_SUB,
	BLENDMODE_COLORFLIP,
	BLENDMODE_ALPHA,
};
void UseBlendMode(D3D12_RENDER_TARGET_BLEND_DESC &blenddesc);
void SetBlend(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc, int blendMode = BLENDMODE_ADD);

class DirectInput
{
protected:
	IDirectInput8* input;

	void Initialize(WNDCLASSEX w);
};

class Keyboard :DirectInput
{
private:
	BYTE key[256];
	BYTE oldkey[256];

public:
	IDirectInputDevice8* device;

	void GetInstance(WNDCLASSEX w);
	void SetDataStdFormat();
	void SetCooperativeLevel(HWND hwnd);
	void GetDeviceState();
	void TransferOldkey();
	bool isInput(const int KEY);
	bool isTrigger(const int KEY);
};

class ShaderBlob
{
public:
	ID3DBlob* blob = nullptr;

	ShaderBlob(const LPCWSTR fileName, const LPCSTR target, ID3DBlob* errorBlob);
};

class WindowsAPI
{
private:
	RECT wrc;
public:
	WNDCLASSEX w;
	HWND hwnd;

	WindowsAPI(WNDPROC lpfnWndProc, int window_width, int window_height);
};

class Buffer
{
private:
	D3D12_RESOURCE_DESC resDesc;
protected:
	UINT size;

	void Init(UINT size);
public:
	ID3D12Resource* buff;

	void SetResource(D3D12_RESOURCE_DIMENSION Dimension);
	void CreateBuffer(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProp);
};

class ConstBuf :public Buffer
{
public:
	ConstBufferDataMaterial* mapMaterial;

	ConstBuf(UINT size);
	void Mapping();
};

class VertexBuf :public Buffer
{
private:
	XMFLOAT3* map;
public:
	D3D12_VERTEX_BUFFER_VIEW view;

	VertexBuf(UINT size);
	void Mapping(XMFLOAT3* vertices, const int ARRAY_NUM);
	void CreateView();
};

class IndexBuf :public Buffer
{
private:
	uint16_t* map;
public:
	D3D12_INDEX_BUFFER_VIEW view;

	IndexBuf(UINT size);
	void Mapping(uint16_t* indices, const int ARRAY_NUM);
	void CreateView();
};

class Pipeline
{
public:
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
	ID3D12PipelineState* state;

	Pipeline();
	void SetShader(ShaderBlob vs, ShaderBlob ps);
	void SetSampleMask();
	void SetRasterizer();
	void SetInputLayout(D3D12_INPUT_ELEMENT_DESC* inputLayout, UINT layoutNum);
	void SetPrimitiveTopology();
	void SetOthers();
	void CreatePipelineState(ID3D12Device* device);
};

class RootSignature
{
private:
	D3D12_ROOT_PARAMETER param;
	D3D12_ROOT_SIGNATURE_DESC desc;
	ID3DBlob* blob;
public:
	ID3D12RootSignature* rs;

	RootSignature()
	{
		param = {};
		desc = {};
		rs = nullptr;
		blob = nullptr;
	}

	void SetParam()
	{
		param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
		param.Descriptor.ShaderRegister = 0;					// 定数バッファ番号
		param.Descriptor.RegisterSpace = 0;						// デフォルト値
		param.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える
	}
	void SetRootSignature()
	{
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = &param;
		desc.NumParameters = 1;
	}
	void SerializeRootSignature(ID3D12Device* device, ID3DBlob* errorBlob)
	{
		HRESULT result = D3D12SerializeRootSignature(&desc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &errorBlob);
		assert(SUCCEEDED(result));
		result = device->CreateRootSignature(0, blob->GetBufferPointer(),
			blob->GetBufferSize(), IID_PPV_ARGS(&rs));
		assert(SUCCEEDED(result));
		blob->Release();
	}
};
