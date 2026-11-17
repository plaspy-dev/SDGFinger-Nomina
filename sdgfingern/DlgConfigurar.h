#pragma once
#include "afxdtctl.h"
#include "afxwin.h"


// CDlgConfigurar dialog

class CDlgConfigurar : public CDialog
{
	DECLARE_DYNAMIC(CDlgConfigurar)

public:
	CDlgConfigurar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgConfigurar();

// Dialog Data
	enum { IDD = IDD_DDCONFIGURAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	CDateTimeCtrl m_DTTolerancia;
public:
	CColorButton m_BAlerta;
public:
	CColorButton m_BExtra;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	CButton m_CFechas;
public:
	CButton m_CDias;
public:
	CButton m_BCalcular;
public:
	CButton m_BDiferencia;
};
