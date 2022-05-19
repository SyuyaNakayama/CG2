#pragma once
#include <dinput.h>
#include <cassert>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <string>

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
public:
	WNDCLASSEX w;
	RECT wrc;
	HWND hwnd;

	WindowsAPI(UINT cbSize, WNDPROC lpfnWndProc, LPCWSTR lpszClassName,
		HINSTANCE hInstance, HCURSOR hCursor, int window_width, int window_height);
};

class Buffer
{
public:
	D3D12_RESOURCE_DESC resDesc;
	ID3D12Resource* buff;
	UINT size;

	Buffer(UINT size);
	void SetResource(D3D12_RESOURCE_DIMENSION Dimension);
	void CreateBuffer(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProp);
};

struct ConstBufferDataMaterial { XMFLOAT4 color; };

class ConstBuf :public Buffer
{
	ConstBufferDataMaterial* constMapMaterial;
};