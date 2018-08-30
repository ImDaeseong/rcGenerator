#include "ImageLib/zlib.h"
#pragma comment( lib, "ImageLib/zlib" )
#include "ImageLib/png.h"
#pragma comment( lib, "ImageLib/libpng" )
#include "ImageLib/PNGlib.h"
#pragma comment( lib, "ImageLib/pnglib" )

HBITMAP hBitmap;

int	pngWidth, pngHeight;

bool LoadPNGResource(HWND hWnd, PNGINFO *pPngInfo)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
		
	if (!PNG_LoadResource(pPngInfo, hInstance, (char*)IDR_RCDATA1))//if (!PNG_LoadResource(pPngInfo, hInstance, (char*)IDR_RCDATA2))
	{
		MessageBox(hWnd, "Error: couldn't load PNG resource", "", MB_ICONERROR);
		return false;
	}
	
	if (!PNG_Decode(pPngInfo))
	{
		MessageBox(hWnd, "Error: couldn't decode PNG resource", "", MB_ICONERROR);
		return false;
	}
	
	if (!(hBitmap = PNG_CreateBitmap(pPngInfo, hWnd, PNG_OUTF_AUTO, false)))
	{
		MessageBox(hWnd, "Error: couldn't create bitmap", "", MB_ICONERROR);
		return false;
	}
	
	pngWidth  = pPngInfo->iWidth;
	pngHeight = pPngInfo->iHeight;
	
	return true;
}

int LoadPNGData(HWND hWnd)
{
	PNGINFO	pngInfo;
	bool bSuccess;
	PNG_Init(&pngInfo);
	bSuccess = LoadPNGResource(hWnd,&pngInfo);
	PNG_Cleanup(&pngInfo);
	return bSuccess ? 0 : -1;
}

void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits)
{
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	for (int y=0; y<bmpInfo.bmHeight; ++y)
	{
		BYTE *pPixel= pBitmapBits + bmpInfo.bmWidth * 4 * y;

		for (int x=0; x<bmpInfo.bmWidth ; ++x)
		{
			pPixel[0]= pPixel[0]*pPixel[3]/255;
			pPixel[1]= pPixel[1]*pPixel[3]/255;
			pPixel[2]= pPixel[2]*pPixel[3]/255;

			pPixel+= 4;
		}
	}
}

CBitmap m_bmpDialog;
LPVOID m_pImg;
LPVOID m_pImgDecoder;

void UpdateDialog(HWND wnd, CBitmap &bmp, BYTE SourceConstantAlpha=255)
{
	SetWindowLong(wnd, GWL_EXSTYLE, GetWindowLong(wnd, GWL_EXSTYLE) | WS_EX_LAYERED);

	CDC dcScreen;
	CDC dcMemory;

	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);

	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmp);

	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	CRect rectDlg;
	GetWindowRect(wnd, rectDlg);

	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(bmpInfo.bmWidth, bmpInfo.bmHeight);

	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0);

	BOOL bRet= ::UpdateLayeredWindow(wnd, dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

	ASSERT(bRet);

	dcMemory.SelectObject(pOldBitmap);
}

void WindowAlphaBlendFromPNG(LPCTSTR pFileName, HWND wnd)
{
	CWaitCursor wait;

	LPBYTE pBitmapBits = NULL;
	LPVOID pImg= NULL;

	LoadPNGData( wnd );

	m_pImg= pImg;
	m_bmpDialog.DeleteObject();
	m_bmpDialog.Attach(hBitmap);

	BITMAP bmpInfo;
	::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
	::PreMultiplyRGBChannels( m_bmpDialog, (LPBYTE)bmpInfo.bmBits );

	UpdateDialog(wnd, m_bmpDialog);
}