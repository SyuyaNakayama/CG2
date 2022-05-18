#pragma once
#include <dinput.h>
#include <cassert>
#include <d3dcompiler.h>
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
	void CreateWindowClass(UINT cbSize, WNDPROC lpfnWndProc, LPCWSTR lpszClassName, HINSTANCE hInstance, HCURSOR hCursor)
	{
		w.cbSize = cbSize;
		w.lpfnWndProc = lpfnWndProc; // �E�B���h�E�v���V�[�W����ݒ�
		w.lpszClassName = lpszClassName; // �E�B���h�E�N���X��
		w.hInstance = hInstance; // �E�B���h�E�n���h��
		w.hCursor = hCursor; // �J�[�\���w��

		// �E�B���h�E�N���X��OS�ɓo�^����
		RegisterClassEx(&w);
	}

	void SetSize(int width, int height)
	{
		// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
		RECT wrc = { 0, 0, width, height };
		// �����ŃT�C�Y��␳����
		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	}
};