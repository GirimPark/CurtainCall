#include "pch.h"
#include "D3DRenderer.h"

#include <directxtk/SimpleMath.h>

#include "../Engine/CommonApp.h"

D3DRenderer::D3DRenderer(int screenWidth, int screenHeight)
	: m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
{
}

D3DRenderer::~D3DRenderer()
{
	CoUninitialize();
}

bool D3DRenderer::Initialize()
{
	// �����
	HRESULT hr = 0;

	/// 1. ����ü�� �Ӽ� ���� ����ü ����
	// https://learn.microsoft.com/ko-kr/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// Present()ȣ�� �� ���÷��� ȭ�鿡�� �ȼ��� ó���ϴ� �ɼ�

	// BufferDesc : �� ���� ǥ�� ���
	// BufferDesc - �����(�ؽ�ó)�� ����/���� ũ�� ����
	swapDesc.BufferDesc.Width = m_screenWidth;
	swapDesc.BufferDesc.Height = m_screenHeight;
	// BufferDesc - ȭ�� �ֻ��� ����
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// BufferDesc - ǥ�� ����
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ���� ���ø� �Ű� ���� ����
	swapDesc.SampleDesc.Count = 1;		// ���� ���ø� ���� ����
	swapDesc.SampleDesc.Quality = 0;	// ���� ���ø� ���� ����

	// �� ������ ǥ�� ��뷮 �� CPU �׼��� �ɼ� ����
	// �ش� �ܰ迡�� ����۸� ��¿����� �����߱� ������, RenderTargetView�� �����ϴ� �κп��� Ư�� �÷��׸� ������� �ʾƵ� �ȴ�.
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = CommonApp::GetHwnd();
	swapDesc.Windowed = TRUE;


	/// 2. Device, DeviceContext, SwapChain ����
	// https://learn.microsoft.com/ko-kr/windows/w11/nf-d3d11-d3d11createdeviceandswapchain
	UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), NULL, m_pDeviceContext.GetAddressOf()));


	/// 3. RenderTargetView ����, RTV�� ����� ���ε�
	// https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/nf-d3d11-id3d11device-createrendertargetview
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	// pDesc == NULL�� ���, Mipmap�� ������� �ʴ´�.
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, m_pRenderTargetView.GetAddressOf()));	// �ؽ�ó ���� ���� ����
	SAFE_RELEASE(pBackBufferTexture);

	// ���� Ÿ���� ���� ��� ���������ο� ���ε�
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);


	/// 4. ����Ʈ ����, Rasterization �ܰ迡 ���ε�
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(m_screenWidth);
	viewport.Height = static_cast<float>(m_screenHeight);
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_pDeviceContext->RSSetViewports(1, &viewport);


	/// 5. Depth&Stencil View ����
	// depthDesc �������� �ؽ��ĸ� �����, �ش� �ؽ��ĸ� DSV�� ǥ�� ���ҽ��� ����Ͽ� DSV ����
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = m_screenWidth;
	depthDesc.Height = m_screenHeight;
	depthDesc.MipLevels = 1;	// Mipmap ������� ����
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&depthDesc, nullptr, &textureDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Format = depthDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &DSVDesc, m_pDepthStencilView.GetAddressOf()));
	SAFE_RELEASE(textureDepthStencil);


	/// InitializeScene
	///	1. ���ؽ� ���̴�, �ȼ� ���̴� ����, ��ǲ ���̾ƿ� ���ε�
	// ��ǲ ���̾ƿ��� ���ؽ� ���̴��� �Է¹��� �������� ������ �����Ѵ�.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	for(int i=0; i<MAX_FILE_NUM; ++i)
	{
		const WCHAR* VSFile = CommonApp::m_pInstance->GetVSFiles()[i];
		const WCHAR* PSFile = CommonApp::m_pInstance->GetPSFiles()[i];

		if(VSFile)
		{
			ID3DBlob* vertexShaderBuffer = nullptr; // �����ϵ� �ڵ忡 �׼����� ������ ����
			HR_T(CompileShaderFromFile(VSFile, "main", "vs_5_0", &vertexShaderBuffer));
			HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
				vertexShaderBuffer->GetBufferSize(), NULL, m_pVertexShaders[i].GetAddressOf()));

			HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
				vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_pInputLayout.GetAddressOf()));

			SAFE_RELEASE(vertexShaderBuffer);
		}
		
		if(PSFile)
		{
			ID3DBlob* pixelShaderBuffer = nullptr;
			HR_T(CompileShaderFromFile(PSFile, "main", "ps_5_0", &pixelShaderBuffer));
			HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
				pixelShaderBuffer->GetBufferSize(), NULL, m_pPixelShaders[i].GetAddressOf()));

			SAFE_RELEASE(pixelShaderBuffer);
		}
	}

	/// 2. sample state ����
	D3D11_SAMPLER_DESC sampleDesc = {};
	ZeroMemory(&sampleDesc, sizeof(sampleDesc));
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampleDesc, m_pSamplerLinear.GetAddressOf()));

	/// 3. blend state ����
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;
	// FinalRGB = SrcRGB * SrcBlend + DestRGB * DestBlend;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, m_pAlphaBlendState.GetAddressOf()));

	return true;
}

void D3DRenderer::Render()
{
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	DirectX::SimpleMath::Color color(.5f, .5f, .5f, 1.f);
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

	// Draw �迭 �Լ��� ȣ���ϱ� �� ������ ���������ο� �ʼ� �������� ������ �ؾ��Ѵ�.
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ������ �̾ �׸��� ���
	m_pDeviceContext->IASetInputLayout(m_pInputLayout.Get());
	for(int i=0; i<MAX_FILE_NUM; ++i)
	{
		if(m_pVertexShaders[i])
			m_pDeviceContext->VSSetShader(m_pVertexShaders[i].Get(), nullptr, 0);
		if(m_pPixelShaders[i])
			m_pDeviceContext->PSSetShader(m_pPixelShaders[i].Get(), nullptr, 0);
	}
	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerLinear.GetAddressOf());
	m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xFFFFFFFF);
}
