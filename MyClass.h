#pragma once
#include <dinput.h>
#include <cassert>

class DirectInput
{
public:
	IDirectInput8* input;
	HRESULT result;

	void Initialize(WNDCLASSEX w);
};

class Keyboard :DirectInput
{
public:
	IDirectInputDevice8* device;
	BYTE key[256];

	void GetInstance(WNDCLASSEX w);
	void SetDataStdFormat();
	void SetCooperativeLevel(HWND hwnd);
	void GetDeviceState();
};