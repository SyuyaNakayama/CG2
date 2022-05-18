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

	Buffer(UINT size)
	{
		resDesc = {};
		buff = nullptr;
		this->size = size;
	}

	void SetResource(D3D12_RESOURCE_DIMENSION Dimension)
	{
		resDesc.Dimension = Dimension;
		resDesc.Width = size;
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	}

	void CreateBuffer(ID3D12Device* device, D3D12_HEAP_PROPERTIES heapProp)
	{
		assert(SUCCEEDED(
			device->CreateCommittedResource(
				&heapProp, // ヒープ設定
				D3D12_HEAP_FLAG_NONE,
				&resDesc, // リソース設定
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&buff))));
	}
};