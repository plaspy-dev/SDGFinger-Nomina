#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"


// CFestivosView form view

class CFestivosView : public CFormView
{
	DECLARE_DYNCREATE(CFestivosView)

protected:
	CConector	*m_pConector;
	DWORD		m_dwCurrentSel;
	CFestivosView();           // protected constructor used by dynamic creation
	virtual ~CFestivosView();

public:
	enum { IDD = IDD_FFESTIVOS };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_LCResultados;
public:
	CDateTimeCtrl m_DTFecha;
public:
	CEditXP m_EObservaciones;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	afx_msg void OnBnClickedBaceptar();
public:
	afx_msg void OnBnClickedBcancelar();
public:
	afx_msg void OnNMDblclkLlistado(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickLlistado(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnLvnKeydownLlistado(NMHDR *pNMHDR, LRESULT *pResult);
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	void OnEditar();
	void OnEliminar();
public:
	CStatic m_SFecha;
public:
	CStatic m_SObservaciones;
};


