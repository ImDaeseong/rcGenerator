#pragma once

class CLayerDlg;
class CrcGeneratorDlg : public CDialog
{
public:
	CrcGeneratorDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_RCGENERATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

private:
	CString GetProgramDir();
	BOOL createBin();
	BOOL createBin(CString strPath);

	void LoadLayerDlg();
	void CloseLayerDlg();
	CLayerDlg * m_pLayer;
};
