#pragma once
#include "afxdtctl.h"
#include "afxwin.h"



// CParametrosView form view

class CParametrosView : public CFormView
{
	DECLARE_DYNCREATE(CParametrosView)

	CConector	*m_pConector;
	CUsuario	*m_pUsuario;

	DWORD		m_dwID[4];

protected:
	CParametrosView();           // protected constructor used by dynamic creation
	virtual ~CParametrosView();

public:
	enum { IDD = IDD_FPARAMETROS };
	enum { query_jornada=1, query_extra, query_extra_dominical, query_ordinaria};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CDateTimeCtrl m_DTDesde;
public:
	CDateTimeCtrl m_DTHasta;
public:
	CEditFloat m_EONocturna;
public:
	CEditFloat m_EEDiurna;
public:
	CEditFloat m_EENocturna;
public:
	CEditFloat m_EODominical;
public:
	CEditFloat m_EEDDominical;
public:
	CEditFloat m_EENDominical;
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnBnClickedBaceptar();
public:
	afx_msg void OnBnClickedBcancelar();
	afx_msg LRESULT OnMysql(WPARAM wParam, LPARAM lParam);
public:
	CButtonXP m_BCancelar;
public:
	CButtonXP m_BAceptar;
};


