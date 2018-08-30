#include "stdafx.h"
#include "rcGenerator.h"
#include "LayerDlg.h"
#include "setPngWnd.h"

IMPLEMENT_DYNAMIC(CLayerDlg, CDialog)

CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerDlg::IDD, pParent)
{
}

CLayerDlg::~CLayerDlg()
{
}

void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLayerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CLayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WindowAlphaBlendFromPNG( NULL, m_hWnd );
	
	MoveWindow( 0, 0, 640, 358 );
	ModifyStyle( NULL, WS_SYSMENU | WS_MINIMIZEBOX, 0 );
	SetWindowText( "rcLayer" );
	CenterWindow();

	HDC screenDC;
	if( ( screenDC = CreateDC( "DISPLAY", NULL, NULL, NULL) ) == NULL )
	{
		return 0;
	}
	DeleteDC(screenDC);
	
	SetTimer( 1, 3000, NULL );
	
	return TRUE; 
}

void CLayerDlg::OnPaint()
{
	CPaintDC dc(this);
}

LRESULT CLayerDlg::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}

void CLayerDlg::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 1 )
	{
		HDC screenDC;		
		if( ( screenDC = CreateDC( "DISPLAY", NULL, NULL, NULL) ) == NULL )
		{
			return;
		}
		DeleteDC(screenDC);
		
		WindowAlphaBlendFromPNG( NULL, m_hWnd );
	}

	CDialog::OnTimer(nIDEvent);
}
