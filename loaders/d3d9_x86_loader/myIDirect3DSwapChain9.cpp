#include "proxydll.h"

myIDirect3DSwapChain9::myIDirect3DSwapChain9(IDirect3DSwapChain9* pOriginal, IDirect3DDevice9* pDevice)
{
	m_pIDirect3DSwapChain9 = pOriginal; // store the pointer to original object
	m_pIDirect3DDevice9 = pDevice; 
}

myIDirect3DSwapChain9::~myIDirect3DSwapChain9(void)
{
	m_pIDirect3DSwapChain9 = NULL;
}

HRESULT myIDirect3DSwapChain9::QueryInterface (REFIID riid, void** ppvObj)
{
	// check if original dll can provide interface. then send *our* address
	*ppvObj = NULL;

	HRESULT hRes = m_pIDirect3DSwapChain9->QueryInterface(riid, ppvObj); 

	if (hRes == NOERROR)
	{
		*ppvObj = this;
	}
	
	return hRes;
}

ULONG myIDirect3DSwapChain9::AddRef(void)
{
	return(m_pIDirect3DSwapChain9->AddRef());
}

ULONG myIDirect3DSwapChain9::Release(void)
{
	// ATTENTION: This is a booby-trap ! Watch out !
	// If we create our own sprites, surfaces, etc. (thus increasing the ref counter
	// by external action), we need to delete that objects before calling the original
	// Release function	

	// global var
	extern myIDirect3DSwapChain9*  gl_pmyIDirect3DSwapChain9;
	
	// release/delete own objects
    // .....
	
	// Calling original function now
	ULONG count = m_pIDirect3DSwapChain9->Release();
		
    if (count==0)
	{
		// now, the Original Object has deleted itself, so do we here
		gl_pmyIDirect3DSwapChain9 = NULL;
		delete(this);  // destructor will be called automatically
	}

	return (count);
}

HRESULT myIDirect3DSwapChain9::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion,DWORD dwFlags)
{
	// Some games use this one for presenting. They ignore the Device's present routine
    
    // we may want to draw own things here before flipping surfaces
    // ... draw own stuff ...

	// call original routine
	HRESULT hres = m_pIDirect3DSwapChain9->Present(pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion,dwFlags);
    
	return (hres);
}

HRESULT myIDirect3DSwapChain9::GetFrontBufferData(IDirect3DSurface9* pDestSurface)
{
	return(m_pIDirect3DSwapChain9->GetFrontBufferData(pDestSurface));
}

HRESULT myIDirect3DSwapChain9::GetBackBuffer(UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)
{
	return(m_pIDirect3DSwapChain9->GetBackBuffer(iBackBuffer,Type,ppBackBuffer));
}

HRESULT myIDirect3DSwapChain9::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return(m_pIDirect3DSwapChain9->GetRasterStatus(pRasterStatus));
}

HRESULT myIDirect3DSwapChain9::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return(m_pIDirect3DSwapChain9->GetDisplayMode(pMode));
}

HRESULT myIDirect3DSwapChain9::GetDevice(IDirect3DDevice9** ppDevice)
{
	return(m_pIDirect3DSwapChain9->GetDevice(ppDevice));
}

HRESULT myIDirect3DSwapChain9::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return(m_pIDirect3DSwapChain9->GetPresentParameters(pPresentationParameters));
}
