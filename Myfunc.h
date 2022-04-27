#pragma once
#include <d3dcompiler.h>
#include <string>
#include <cassert>

HRESULT ReadShaderFile(ID3DBlob** shBlob, ID3DBlob** errorBlob, LPCWCHAR fileName, LPCSTR target)
{
	return D3DCompileFromFile(
		fileName, // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", target, // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		shBlob, errorBlob);
}