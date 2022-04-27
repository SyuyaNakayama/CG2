#include "MyClass.h"
void DirectInput::Initialize(WNDCLASSEX w)
{
	result = DirectInput8Create(
		w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&input, nullptr);
	assert(SUCCEEDED(result));
}

void Keyboard::GetInstance(WNDCLASSEX w)
{
	Initialize(w);
	result = input->CreateDevice(GUID_SysKeyboard, &device, NULL);
	assert(SUCCEEDED(result));
}

void Keyboard::SetDataStdFormat()
{
	result = device->SetDataFormat(&c_dfDIKeyboard); // •W€Œ`Ž®
	assert(SUCCEEDED(result));
}

void Keyboard::SetCooperativeLevel(HWND hwnd)
{
	result = device->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Keyboard::GetDeviceState()
{
	device->GetDeviceState(sizeof(key), key);
}