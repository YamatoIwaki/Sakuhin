#pragma once

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

//_declspec()	:DLLから（関数、クラス、クラスのメンバ関数）エクスポートする.
//align()		:（強制的に）16byteで使用する.
#define ALIGN16	_declspec( align(16) )

#include "CSingletonManager.h"

//前方宣言.
class CDirectX11;

/**************************************************
*	スプライト2Dクラス.
**/
class CConcentLine
	: public SingletonManager<CConcentLine>
{
public:
	//======================================
	//	構造体.
	//======================================
	//コンスタントバッファのアプリ側の定義.
	//※シェーダ内のコンスタントバッファと一致している必要あり.
	struct SHADER_CONSTANT_BUFFER
	{
		ALIGN16	DirectX::XMFLOAT2 screenCenter;		//中心.	
		ALIGN16 float lineCount;		//ライン数.
		ALIGN16 float time;				//時間.
		ALIGN16 float alpha;				//時間.

	};
	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR2	Tex;	//テクスチャ座標.
	};
private:
	CConcentLine();	//コンストラクタ.
public:
	friend class SingletonManager<CConcentLine>;

	~CConcentLine();	//デストラクタ.

	//初期化.
	HRESULT Init();

	//解放.
	void Release();

	//シェーダ作成.
	HRESULT CreateShader();

	//レンダリング用.
	void Render();

	void StartEffect(float t = 0.3f);

	void Draw();

	float GetTime();


	void SetAlpha(float a) { m_Alpha = a; }

	void SetDraw(bool d) { m_IsDraw = d; }

private:
	CDirectX11*				m_pDx11;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;	//コンスタントバッファ.

	float					m_Alpha;
	bool					m_IsDraw;

	float					m_EffecTime;
	float					m_EffecStartTime;
	float					m_EffecCurrentTime;
	bool					m_IsActive;


};