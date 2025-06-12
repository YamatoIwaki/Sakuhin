#include "CShadowMap.h"
#include "CDirectX11.h"

//�V�F�[�_�t�@�C����(�f�B���N�g�����܂�).
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

//������
HRESULT CShadowMap::Init()
{
	m_pDx11			= CDirectX11::GetInstance();
	m_pDevice11		= m_pDx11->GetDevice();
	m_pContext11	= m_pDx11->GetContext();

	if (FAILED(CreateShader()))
	{
		throw std::runtime_error("Failed to create shader");
	}

	//�V���h�E�}�b�v�[�x�p�̃e�N�X�`�����쐬
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

	//�V���h�E�}�b�v�@�����_�[�^�[�Q�b�g�E�V�F�[�_�[���\�[�X�̃e�N�X�`�����쐬
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

	//�[�x�X�e���V���r���[���쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format			= DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = m_pDevice11->CreateDepthStencilView(m_ShadowMapDepthTexture, &dsvDesc, &m_ShadowMapDSV);
	
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to create depth stencil view");
	}


	//�V�F�[�_�[���\�[�X�r���[�̍쐬
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

	//�R���X�^���g�o�b�t�@���쐬
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

	//�����_�[�^�[�Q�b�g�r���[�쐬
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

	//HLSL����o�[�e�b�N�X�V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME, nullptr, nullptr, "VS_Main", "vs_5_0",
			uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
	{
		_ASSERT_EXPR(false, _T("hlsl�ǂݍ��ݎ��s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�o�[�e�b�N�X�V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader)))	//(out)�o�[�e�b�N�X�V�F�[�_.
	{
		_ASSERT_EXPR(false, _T("�o�[�e�b�N�X�V�F�[�_�쐬���s"));
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//���_�C���v�b�g���C�A�E�g�̔z��v�f�����Z�o.
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC tmp[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	numElements = sizeof(tmp) / sizeof(tmp[0]);	//�v�f���Z�o.
	memcpy_s(layout, sizeof(layout),
		tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);

	//���_�C���v�b�g���C�A�E�g���쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))	//(out)���_�C���v�b�g���C�A�E�g.
	{
		_ASSERT_EXPR(false, _T("���_�C���v�b�g���C�A�E�g�쐬���s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL����s�N�Z���V�F�[�_�̃u���u���쐬.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME, nullptr, nullptr, "PS_ShadowMap", "ps_5_0",
			uCompileFlag, 0, nullptr, &pCompiledShader, &pErrors, nullptr)))
	{
		_ASSERT_EXPR(false, _T("hlsl�ǂݍ��ݎ��s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬�����u���u����u�s�N�Z���V�F�[�_�v���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader)))	//(out)�s�N�Z���V�F�[�_.
	{
		_ASSERT_EXPR(false, _T("�s�N�Z���V�F�[�_�쐬���s"));
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);


	return S_OK;
}



//���C�g�̃r���[�ˉe�s���ݒ�
void CShadowMap::SetLightViewProj(const D3DXMATRIX& lightviewproj)
{
	m_LightViewProj = lightviewproj;
}

void CShadowMap::BeginShadow()
{
#if 0
	GetInstance()->m_pContext11->OMSetRenderTargets(1, &GetInstance()->m_ShadowRTV, GetInstance()->m_ShadowMapDSV);

	GetInstance()->m_pContext11->VSSetShader(GetInstance()->m_pVertexShader, nullptr, 0);	//���_�V�F�[�_.
	GetInstance()->m_pContext11->PSSetShader(GetInstance()->m_pPixelShader, nullptr, 0);	//�s�N�Z���V�F�[�_.


	D3D11_VIEWPORT viewPort = {};

	viewPort.Width = static_cast<float>(GetInstance()->m_ShadowMapWidth);		//�r���[�|�[�g��
	viewPort.Height = static_cast<float>(GetInstance()->m_ShadowMapHeight);	//�r���[�|�[�g����
	viewPort.MinDepth = 0.0f;										//�ŏ��[�x
	viewPort.MaxDepth = 1.0f;										//�ő�[�x
	viewPort.TopLeftX = 0.0f;										//�r���[�|�[�g�̍���X
	viewPort.TopLeftY = 0.0f;										//�r���[�|�[�g�̍���Y
	GetInstance()->m_pContext11->RSSetViewports(1, &viewPort);						//�r���[�|�[�g��ݒ�

	GetInstance()->m_pContext11->ClearRenderTargetView(GetInstance()->m_ShadowRTV, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f));
	GetInstance()->m_pContext11->ClearDepthStencilView(GetInstance()->m_ShadowMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
#endif
	m_pContext11->ClearRenderTargetView(m_ShadowRTV, D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f));
	m_pContext11->ClearDepthStencilView(m_ShadowMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_pContext11->OMSetRenderTargets(1, &m_ShadowRTV, m_ShadowMapDSV);

	m_pContext11->VSSetShader(m_pVertexShader, nullptr, 0);	//���_�V�F�[�_.
	m_pContext11->PSSetShader(m_pPixelShader, nullptr, 0);	//�s�N�Z���V�F�[�_.


	static D3D11_VIEWPORT viewPort = {};

	viewPort.Width = static_cast<float>(m_ShadowMapWidth);		//�r���[�|�[�g��
	viewPort.Height = static_cast<float>(m_ShadowMapHeight);	//�r���[�|�[�g����
	viewPort.MinDepth = 0.0f;										//�ŏ��[�x
	viewPort.MaxDepth = 1.0f;										//�ő�[�x
	viewPort.TopLeftX = 0.0f;										//�r���[�|�[�g�̍���X
	viewPort.TopLeftY = 0.0f;										//�r���[�|�[�g�̍���Y
	
	m_pContext11->RSSetViewports(1, &viewPort);						//�r���[�|�[�g��ݒ�


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


