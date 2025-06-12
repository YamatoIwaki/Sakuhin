#pragma once

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>
#include <crtdbg.h>

//DirectX9.
#include <d3dx9.h>
//DirectX11.
#include <D3DX11.h>
#include <D3D11.h>
//DirectX10.
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

//ダイレクトXマス
#include <DirectXMath.h>

#include "MyMacro.h"

//ライブラリ読み込み.
#pragma comment( lib, "winmm.lib" )
//DirectX9.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
//DirectX11.
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
//DirectX10.
#pragma comment( lib, "d3dx10.lib" )	//「D3DX～」の定義使用時に必要.

//=================================================
//	定数.
//=================================================
//#define WND_W 1280	//←定数宣言で#defineは使わない.
constexpr int WND_W = 1280;	//ウィンドウの幅.
constexpr int WND_H = 720;	//ウィンドウの高さ.
constexpr int FPS = 60;		//フレームレート.
constexpr float SECOND = 1.0f / FPS;

//=================================================
//	構造体.
//=================================================
//カメラ情報構造体.
struct CAMERA
{
	D3DXVECTOR3	Position;	//視点.
	D3DXVECTOR3 Look;		//注視点.
};
//ライト情報.
struct LIGHT
{
	D3DXVECTOR3	Position;	//位置.
	D3DXVECTOR3	vDirection;	//方向.
	D3DXMATRIX	mRotation;	//回転行列.
	float		Intensity;	//強度(明るさ).
};

enum enScene
{
	Title = 0,
	GameMain,
	GameOver,
	GameClear,
};
