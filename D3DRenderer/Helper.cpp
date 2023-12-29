#include "pch.h"
#include "Helper.h"
#include <comdef.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

LPCWSTR GetComErrorString(HRESULT hr)
{
	_com_error err(hr);
	LPCWSTR errMsg = err.ErrorMessage();
	return errMsg;
}

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging, 최적화 건너뛰기
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	// D3DCompileFromFile
	// (파일명, 셰이더 매크로, 컴파일러가 포함 파일을 처리하는 포인터(include Shared.fxh), 진입점 함수 이름, 셰이더 모델,
	//	셰이더 컴파일 옵션, 효과 컴파일 옵션, 컴파일된 코드에 액세스할 포인터 변수, 컴파일러 오류 메시지에 액세스할 포인터 변수)
	
	hr = D3DCompileFromFile(szFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "", MB_OK);
			pErrorBlob->Release();
		}
		return hr;
	}

	if (pErrorBlob)
	{
		pErrorBlob->Release();
	}

	return S_OK;
}