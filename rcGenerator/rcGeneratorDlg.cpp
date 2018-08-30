#include "stdafx.h"
#include "rcGenerator.h"
#include "rcGeneratorDlg.h"
#include "LayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CrcGeneratorDlg::CrcGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CrcGeneratorDlg::IDD, pParent)
{
	m_pLayer = NULL;
}

void CrcGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CrcGeneratorDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CrcGeneratorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CrcGeneratorDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CrcGeneratorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  
}

void CrcGeneratorDlg::OnPaint()
{
	CPaintDC dc(this);
}


void CrcGeneratorDlg::OnBnClickedButton2()
{
	CloseLayerDlg();
	LoadLayerDlg();
}

void CrcGeneratorDlg::LoadLayerDlg()
{
	m_pLayer = new CLayerDlg();
	m_pLayer->Create(IDD_LAYER_DIALOG);
	m_pLayer->ShowWindow(SW_SHOW); 
}

void CrcGeneratorDlg::CloseLayerDlg()
{
	if(m_pLayer)
	{
		if(m_pLayer->GetSafeHwnd())
		{
			m_pLayer->DestroyWindow();
			delete m_pLayer;
			m_pLayer = NULL;
		}
	}
}

void CrcGeneratorDlg::OnBnClickedButton1()
{
	if(createBin())
	{
		AfxMessageBox("Success");
	}
	else 
	{
		AfxMessageBox("failed");
	}

	if(createBin("D:\\bg.png"))
	{
		AfxMessageBox("Success");
	}
	else 
	{
		AfxMessageBox("failed");
	}
}

CString CrcGeneratorDlg::GetProgramDir()
{
	CString szExePath;
	char    Filename[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), Filename, MAX_PATH);

	szExePath = Filename;
	szExePath = szExePath.Left(szExePath.ReverseFind('\\'));
	
	return szExePath;
}

BOOL CrcGeneratorDlg::createBin()
{
	HINSTANCE hInstance = AfxGetInstanceHandle();

	HRSRC hrsrc = FindResource(hInstance, MAKEINTRESOURCE(IDB_PNG_BG), "PNG");
	if (hrsrc == NULL)
		return FALSE;

	DWORD dwFileSize = SizeofResource(hInstance, hrsrc);
	if (0 == dwFileSize)
		return FALSE;

	HGLOBAL hglob = LoadResource(hInstance, hrsrc);
	if (hglob == NULL)
		return FALSE;

	LPVOID rdata = LockResource(hglob);
	if (rdata == NULL)
		return FALSE;

	CString strPath;
	strPath.Format("%s\\bg.bin", GetProgramDir()); 

	HANDLE hFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	try
	{
		DWORD writ;
		WriteFile(hFile, rdata, dwFileSize, &writ, NULL);
	}
	catch (...)
	{
	}
	CloseHandle(hFile);

	return TRUE;
}

BOOL CrcGeneratorDlg::createBin(CString strImgPath)
{
	HANDLE hFile = CreateFile(strImgPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == NULL)
		return FALSE;

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (0 == dwFileSize)
		return FALSE;

	HGLOBAL m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
	if (m_hBuffer == NULL)
		return FALSE;

	void* pBuffer = ::GlobalLock(m_hBuffer);
	if (pBuffer == NULL)
		return FALSE;
	
	DWORD dwBytesRead = 0;
	BOOL bRead = ReadFile(hFile, pBuffer, dwFileSize, &dwBytesRead, NULL);

	if(bRead)
	{
		CString strPath;
		strPath.Format("%s\\bg1.bin", GetProgramDir()); 

		HANDLE hWFile = CreateFile(strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		try
		{
			DWORD writ;
			WriteFile(hWFile, pBuffer, dwFileSize, &writ, NULL);
		}
		catch (...)
		{
		}
		CloseHandle(hWFile);
	}

	::GlobalUnlock(m_hBuffer);
	::GlobalFree(m_hBuffer);

	CloseHandle(hFile);	

	return TRUE;
}
