#pragma once
#include <dinput.h>
#include <cassert>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;

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

	void CompileFromFile(const LPCWSTR fileName,
		const LPCSTR target, ID3DBlob* errorBlob);
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

struct ConstBufferDataMaterial { XMFLOAT4 color; };

class Buffer
{
private:
	D3D12_RESOURCE_DESC resDesc;
protected:
	void Init(UINT size);
public:
	ID3D12Resource* buff;
	UINT size;

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
	VertexBuf(UINT size);
	void Mapping(XMFLOAT3* vertices, const int ARRAY_NUM);
};

class IndexBuf :public Buffer
{
private:
	uint16_t* map;
public:
	IndexBuf(UINT size);
	void Mapping(uint16_t* indices, const int ARRAY_NUM);
};