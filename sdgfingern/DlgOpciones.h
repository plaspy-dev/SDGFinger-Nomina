#pragma once
#include "afxwin.h"


// CDlgOpciones dialog

class CDlgOpciones : public CDialog
{
	DECLARE_DYNAMIC(CDlgOpciones)

public:
	CDlgOpciones(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgOpciones();

// Dialog Data
	enum { IDD = IDD_DOPCIONES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBcerrar();
public:
	afx_msg void OnBnClickedOk();
public:
	CButton m_BProtector;
public:
	CButton m_BDisplay;
public:
	CEditXP m_EDireccion;
public:
	afx_msg void OnBnClickedCdisplay();
public:
	CButtonXP m_BCerrar;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	virtual BOOL OnInitDialog();
public:
	CCheckBoxSeparator m_BCliente;
public:
	afx_msg void OnBnClickedCcliente();
};
