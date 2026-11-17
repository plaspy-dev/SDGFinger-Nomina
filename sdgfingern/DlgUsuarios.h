#pragma once

// CDlgUsuarios dialog

class CDlgUsuarios : public CDialog
{
	DECLARE_DYNAMIC(CDlgUsuarios)
	CConectorODBC	*m_pConector;
	CString		m_szGrupo;
	CString		m_szCampo;

public:
	CDlgUsuarios(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUsuarios();

// Dialog Data
	enum { IDD = IDD_DUSUARIOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_LCUsuarios;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnNMDblclkLusuarios(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickLusuarios(NMHDR *pNMHDR, LRESULT *pResult);
};
