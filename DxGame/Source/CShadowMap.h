#pragma once

#include <functional>
#include "CSingletonManager.h"

/*******************************************
*		シャドウマップクラス
**/

//前方宣言
class CDirectX11;

class CShadowMap
	: public SingletonManager<CShadowMap>
{
private:

	CShadowMap();
	~CShadowMap();

public:

	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mWLVP;		//ワールド,ビュー,プロジェクションの合成変換行列.	
	};


	//頂点の構造体.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//頂点座標.
		D3DXVECTOR3	Normal;	//法線(陰影計算に必須).
	};

	//カラーバッファのクリアカラー
	static constexpr float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


public:
	friend class SingletonManager<CShadowMap>;

	//初期設定
	HRESULT Init();

	//シェーダ作成.
	HRESULT CreateShader();

	//ライトからのビュー射影行列を設定し、シャドウマップの生成に使用する
	void SetLightViewProj(const D3DXMATRIX& lightviewproj);

	//シャドウマップのシェーダーリソースビューを取得
	ID3D11ShaderResourceView** GetShadowMapSRV() { return &m_ShadowMapSRV; }

	void BeginShadow();
	void EndShadow();


	ID3D11VertexShader* GetVertexShader() { return m_pVertexShader; }
	ID3D11InputLayout* GetLayoutShader() { return m_pVertexLayout;  }
	ID3D11PixelShader* GetPixcelShader() { return m_pPixelShader; }
	static ID3D11Buffer* GetConstantBuffer();
private:

	//Direct11
	CDirectX11*					m_pDx11;
	ID3D11Device*				m_pDevice11;
	ID3D11DeviceContext*		m_pContext11;

	ID3D11VertexShader*			m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*			m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*			m_pPixelShader;		//ピクセルシェーダ.

	//シャドウマップの解像度
	int							m_ShadowMapWidth;	//幅
	int							m_ShadowMapHeight;	//高さ

	//シャドウマップのテクスチャリソース(深度情報の格納)
	ID3D11Texture2D*			m_ShadowMapTexture;
	ID3D11Texture2D*			m_ShadowMapDepthTexture;

	//シャドウマップの深度ステンシルビュー(描画対象の深度情報)
	ID3D11DepthStencilView*		m_ShadowMapDSV;

	//シャドウマップのシェーダーリソースビュー(他のシェーダーから参照可能にする)
	ID3D11ShaderResourceView*	m_ShadowMapSRV;

	ID3D11RenderTargetView*		m_ShadowRTV	= nullptr;

	//ライトのビュー射影行列
	D3DXMATRIX					m_LightViewProj;

	//コンスタントバッファ
	ID3D11Buffer*				m_pCBuffer;
};

