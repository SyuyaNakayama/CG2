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
	assert(SUCCEEDED(device->SetDataFormat(&c_dfDIKeyboard)));// �W���`��
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

void ShaderBlob::CompileFromFile(const LPCWSTR fileName, const LPCSTR target, ID3DBlob* errorBlob)
{
	HRESULT result;

	result = D3DCompileFromFile(
		fileName, // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&blob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

WindowsAPI::WindowsAPI(UINT cbSize, WNDPROC lpfnWndProc, LPCWSTR lpszClassName, HINSTANCE hInstance,
	HCURSOR hCursor, int window_width, int window_height)
{
	w.cbSize = cbSize;
	w.lpfnWndProc = lpfnWndProc; // �E�B���h�E�v���V�[�W����ݒ�
	w.lpszClassName = lpszClassName; // �E�B���h�E�N���X��
	w.hInstance = hInstance; // �E�B���h�E�n���h��
	w.hCursor = hCursor; // �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);

	wrc = { 0, 0, window_width, window_height };
	// �����ŃT�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(w.lpszClassName, // �N���X��
		L"LE2A_16_�i�J���}_�V���E��_CG2", // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW, // �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT, // �\��X���W(OS�ɔC����)
		CW_USEDEFAULT, // �\��Y���W(OS�ɔC����)
		wrc.right - wrc.left, // �E�B���h�E����
		wrc.bottom - wrc.top, // �E�B���h�E�c��
		nullptr, // �e�E�B���h�E�n���h��
		nullptr, // ���j���[�n���h��
		w.hInstance, // �Ăяo���A�v���P�[�V�����n���h��
		nullptr); // �I�v�V����
}