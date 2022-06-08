#pragma once
#include <dinput.h>
#include <cassert>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

struct ConstBufferDataMaterial { XMFLOAT4 color; };
enum BlendMode
{
	BLENDMODE_ADD,
	BLENDMODE_SUB,
	BLENDMODE_COLORFLIP,
	BLENDMODE_ALPHA,
};
void UseBlendMode(D3D12_RENDER_TARGET_BLEND_DESC& blenddesc);
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
	void Init();
public:
	ID3D12Resource* buff;

	void SetResource(size_t width, size_t height, D3D12_RESOURCE_DIMENSION Dimension, bool isTexRes = 0);
	void CreateBuffer(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProp);
};

class ConstBuf :public Buffer
{
public:
	ConstBufferDataMaterial* mapMaterial;
	UINT size;

	ConstBuf(UINT size);
	void Mapping();
};

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class VertexBuf :public Buffer
{
private:
	Vertex* map;
public:
	D3D12_VERTEX_BUFFER_VIEW view;
	UINT size;

	VertexBuf(UINT size);
	void Mapping(Vertex* vertices, const int ARRAY_NUM);
	void CreateView();
};

class IndexBuf :public Buffer
{
private:
	uint16_t* map;
public:
	D3D12_INDEX_BUFFER_VIEW view;
	UINT size;

	IndexBuf(UINT size);
	void Mapping(uint16_t* indices, const int ARRAY_NUM);
	void CreateView();
};

class TextureBuf :public Buffer
{
public:
	D3D12_SHADER_RESOURCE_VIEW_DESC view;

	TextureBuf();
	void Transfer(size_t textureWidth, size_t imageDataCount, XMFLOAT4* imageData);
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
	D3D12_ROOT_PARAMETER params[2];
	D3D12_ROOT_SIGNATURE_DESC desc;
	ID3DBlob* blob;
public:
	ID3D12RootSignature* rs;

	RootSignature()
	{
		params[0] = {};
		params[1] = {};
		desc = {};
		rs = nullptr;
		blob = nullptr;
	}

	void SetParam(D3D12_DESCRIPTOR_RANGE descriptorRange)
	{
		params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// 定数バッファビュー
		params[0].Descriptor.ShaderRegister = 0;					// 定数バッファ番号
		params[0].Descriptor.RegisterSpace = 0;						// デフォルト値
		params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// 全てのシェーダから見える

		params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		params[1].DescriptorTable.pDescriptorRanges = &descriptorRange;
		params[1].DescriptorTable.NumDescriptorRanges = 1;
		params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	}
	void SetRootSignature(D3D12_STATIC_SAMPLER_DESC samplerDesc)
	{
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		desc.pParameters = params;
		desc.NumParameters = _countof(params);
		desc.pStaticSamplers = &samplerDesc;
		desc.NumStaticSamplers = 1;
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

//class ResourceBarrier
//{
//public:
//	D3D12_RESOURCE_BARRIER desc;
//
//	ResourceBarrier(std::vector<ID3D12Resource*> backBuffers, UINT bbIndex)
//	{
//		desc.Transition.pResource = backBuffers[bbIndex]; // バックバッファを指定
//		desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
//		desc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
//	}
//	void BarrierFlip()
//	{
//		desc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
//		desc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
//	}
//};