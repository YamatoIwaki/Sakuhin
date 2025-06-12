#pragma once

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)
#include <Windows.h>
#include <crtdbg.h>

//DirectX9.
#include <d3dx9.h>
//DirectX11.
#include <D3DX11.h>
#include <D3D11.h>
//DirectX10.
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

//�_�C���N�gX�}�X
#include <DirectXMath.h>

#include "MyMacro.h"

//���C�u�����ǂݍ���.
#pragma comment( lib, "winmm.lib" )
//DirectX9.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
//DirectX11.
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
//DirectX10.
#pragma comment( lib, "d3dx10.lib" )	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

//=================================================
//	�萔.
//=================================================
//#define WND_W 1280	//���萔�錾��#define�͎g��Ȃ�.
constexpr int WND_W = 1280;	//�E�B���h�E�̕�.
constexpr int WND_H = 720;	//�E�B���h�E�̍���.
constexpr int FPS = 60;		//�t���[�����[�g.
constexpr float SECOND = 1.0f / FPS;

//=================================================
//	�\����.
//=================================================
//�J�������\����.
struct CAMERA
{
	D3DXVECTOR3	Position;	//���_.
	D3DXVECTOR3 Look;		//�����_.
};
//���C�g���.
struct LIGHT
{
	D3DXVECTOR3	Position;	//�ʒu.
	D3DXVECTOR3	vDirection;	//����.
	D3DXMATRIX	mRotation;	//��]�s��.
	float		Intensity;	//���x(���邳).
};

enum enScene
{
	Title = 0,
	GameMain,
	GameOver,
	GameClear,
};
