#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CDepartamentosView form view

class CDepartamentosView : public CFormView
{
	DECLARE_DYNCREATE(CDepartamentosView)

	CConector	*m_pConector;
	CUsuario	*m_pUsuario;
	DWORD		m_dwCurrentID;

protected:
	CDepartamentosView();           // protected constructor used by dynamic creation
	virtual ~CDepartamentosView();

public:
	enum { IDD = IDD_FDEPARTAMENTOS };
	enum { query_departamentos=1, query_insert};
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
	CMyListCtrl m_LCDepartamentos;
public:
	CEditXP m_EDepartamento;
public:
	CEditXP m_EDescripcion;
public:
	CEditFloat m_EValor;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnBnClickedBaceptar();
public:
	afx_msg void OnBnClickedBcancelar();
public:
	afx_msg void OnEnChangeEdepartamento();
public:
	afx_msg void OnNMDblclkLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMRclickLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnMySQL(WPARAM wParam, LPARAM lParam);
	void OnEditar();
	void OnEliminar();
	void OnVerUsuarios();
public:
	afx_msg void OnLvnKeydownLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


