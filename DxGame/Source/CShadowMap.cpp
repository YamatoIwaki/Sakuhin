#include "CShadowMap.h"
#include "CDirectX11.h"

//シェーダファイル名(ディレクトリも含む).
const TCHAR SHADER_NAME[] = _T("Data\\Shader\\ShadowMap.hlsl");

CShadowMap::CShadowMap()
	: m_pDx11				(nullptr)
	, m_pDevice11			(nullptr)
	, m_pContext11			(nullptr)

	, m_pVertexShader		(nullptr)
	, m_pVertexLayout		(nullptr)
	, m_pPixelShader		(nullptr)

	, m_ShadowMapWidth		(10000)
	, m_ShadowMapHeight		(10000)

	, m_ShadowMapTexture	(nullptr)
	, m_ShadowMapDepthTexture(nullptr)
	, m_ShadowMapDSV		(nullptr)
	, m_ShadowMapSRV		(nullptr)
	, m_ShadowRTV			(nullptr)

	, m_pCBuffer			(nullptr)
{

}

CShadowMap::~CShadowMap()
{
	SAFE_RELEASE(m_pCBuffer);
	SAFE_RELEASE(m_ShadowMapDepthTexture);
	SAFE_RELEASE(m_ShadowMapTexture);
	SAFE_RELEASE(m_ShadowMapDSV);
	SAFE_RELEASE(m_ShadowMapSRV);

	m_pDx11 = nullptr;
}

//初期化
HRESULT CShadowMap::Init()
{
	m_pDx11			= CDirectX11::GetInstance();
	m_pDevice11		= m_pDx11->GetDevice();
	m_pContext11	= m_pDx11->GetContext();

	if (FAILED(CreateShader()))
	{
		throw std::runtime_error("Failed to create shader");
	}

	//シャドウマップ深度用のテクスチャを作成
	D3D11_TEXTURE2D_DESC		depthDesc = {};
	depthDesc.Width			= m_ShadowMapWidth;
	depthDesc.Height		= m_ShadowMapHeight;
	depthDesc.MipLevels		= 1;
	depthDesc.ArraySize		= 1;
	depthDesc.MiscFlags		= 0;
	depthDesc.Format			= DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count= 1;
	depthDesc.Usage			= D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags		= D3D11_BIND_DEPTH_STENCIL;

	auto hr = m_pDevice11->CreateTexture2D(&depthDesc, nullptr, &m_ShadowMapDepthTexture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create shadw map texture");
	}

	//シャドウマップ　レンダーターゲット・シェーダーリソースのテクスチャを作成
	D3D11_TEXTURE2D_DESC		textureDesc = {};
	textureDesc.Width			= m_ShadowMapWidth; 
	textureDesc.Height			= m_ShadowMapHeight;
	textureDesc.MipLevels		= 1;
	textureDesc.ArraySize		= 1;
	textureDesc.MiscFlags		= 0;
	textureDesc.Format			= DXGI_FORMAT_R32G32_TYPELESS;
	textureDesc.SampleDesc.Count= 1;
	textureDesc.Usage			= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags		= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = m_pDevice11->CreateTexture2D(&textureDesc, nullptr, &m_ShadowMapTexture);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create shadw map texture");
	}

	//深度ステンシルビューを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format			= DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice11->CreateDepthStencilView(m_ShadowMapDepthTexture, &dsvDesc, &m_ShadowMapDSV);
	
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create depth stencil view");
	}


	//シェーダーリソースビューの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format			= DXGI_FORMAT_R32G32_FLOAT;
	srvDesc.ViewDimension	= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = m_pDevice11->CreateShaderResourceView(m_ShadowMapTexture, &srvDesc, &m_ShadowMapSRV);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create shader resource view");
	}

	//コンスタントバッファを作成
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth		= sizeof(CBUFFER_PER_MESH);
	bufferDesc.MiscFlags		= 0;

	hr = m_pDevice11->CreateBuffer(&bufferDesc, NULL, &m_pCBuffer);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create constant buffer");
	}

	//レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format			= DXGI_FORMAT_R32G32_FLOAT;
	rtvDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice11->CreateRenderTargetView(m_ShadowMapTexture, &rtvDesc, &m_ShadowRTV);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create render target view");
	}

}

HRESULT CShadowMap::CreateShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからバーテックスシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME, nullptr, nullptr, "VS_Main", "vs_5_0",
			uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
	{
		_ASSERT_EXPR(false, _T("hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「バーテックスシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)バーテックスシェーダ.
	{
		_ASSERT_EXPR(false, _T("バーテックスシェーダ作成失敗"));
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//頂点インプットレイアウトの配列要素数を算出.
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);	//要素数算出.
	memcpy_s(layout, sizeof(layout),
		tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//頂点インプットレイアウトを作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)頂点インプットレイアウト.
	{
		_ASSERT_EXPR(false, _T("頂点インプットレイアウト作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからピクセルシェーダのブロブを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME, nullptr, nullptr, "PS_ShadowMap", "ps_5_0",
			uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
	{
		_ASSERT_EXPR(false, _T("hlsl読み込み失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したブロブから「ピクセルシェーダ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)ピクセルシェーダ.
	{
		_ASSERT_EXPR(false, _T("ピクセルシェーダ作成失敗"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);


	return S_OK;
}



//ライトのビュー射影行列を設定
void CShadowMap::SetLightViewProj(const D3DXMATRIX& lightviewproj)
{
	m_LightViewProj = lightviewproj;
}

void CShadowMap::BeginShadow()
{
#if 0
	GetInstance()->m_pContext11->OMSetRenderTargets(1, &GetInstance()->m_ShadowRTV, GetInstance()->m_ShadowMapDSV);

	GetInstance()->m_pContext11->VSSetShader(GetInstance()->m_pVertexShader, nullptr, 0);	//頂点シェーダ.
	GetInstance()->m_pContext11->PSSetShader(GetInstance()->m_pPixelShader, nullptr, 0);	//ピクセルシェーダ.


	D3D11_VIEWPORT viewPort = {};

	viewPort.Width = static_cast<float>(GetInstance()->m_ShadowMapWidth);		//ビューポート幅
	viewPort.Height = static_cast<float>(GetInstance()->m_ShadowMapHeight);	//ビューポート高さ
	viewPort.MinDepth = 0.0f;										//最小深度
	viewPort.MaxDepth = 1.0f;										//最大深度
	viewPort.TopLeftX = 0.0f;										//ビューポートの左上X
	viewPort.TopLeftY = 0.0f;										//ビューポートの左上Y
	GetInstance()->m_pContext11->RSSetViewports(1, &viewPort);						//ビューポートを設定

	GetInstance()->m_pContext11->ClearRenderTargetView(GetInstance()->m_ShadowRTV, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f));
	GetInstance()->m_pContext11->ClearDepthStencilView(GetInstance()->m_ShadowMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif
	m_pContext11->ClearRenderTargetView(m_ShadowRTV, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f));
	m_pContext11->ClearDepthStencilView(m_ShadowMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_pContext11->OMSetRenderTargets(1, &m_ShadowRTV, m_ShadowMapDSV);

	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);	//頂点シェーダ.
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);	//ピクセルシェーダ.


	static D3D11_VIEWPORT viewPort = {};

	viewPort.Width = static_cast<float>(m_ShadowMapWidth);		//ビューポート幅
	viewPort.Height = static_cast<float>(m_ShadowMapHeight);	//ビューポート高さ
	viewPort.MinDepth = 0.0f;										//最小深度
	viewPort.MaxDepth = 1.0f;										//最大深度
	viewPort.TopLeftX = 0.0f;										//ビューポートの左上X
	viewPort.TopLeftY = 0.0f;										//ビューポートの左上Y
	
	m_pContext11->RSSetViewports(1, &viewPort);						//ビューポートを設定


}

void CShadowMap::EndShadow()
{
	m_pDx11->RenderReset();
	m_pDx11->ViewportReset();
}

ID3D11Buffer* CShadowMap::GetConstantBuffer()
{
	return GetInstance()->m_pCBuffer;
}


