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
	BYTE oldkey[256];

	void GetInstance(WNDCLASSEX w);
	void SetDataStdFormat();
	void SetCooperativeLevel(HWND hwnd);
	void GetDeviceState();
	void TransferOldkey()
	{
		for (size_t i = 0; i < 256; i++)
		{
			oldkey[i] = key[i];
		}
	}
	bool isInput(const int KEY)
	{
		if (key[KEY]) { return true; }
		return false;
	}
	bool isTrigger(const int KEY)
	{
		bool flag = false;

		if (!oldkey[KEY] && key[KEY]) { flag = true; }
		return flag;
	}
};