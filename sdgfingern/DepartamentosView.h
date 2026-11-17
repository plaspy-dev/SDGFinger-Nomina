#pragma once
#include "afxcmn.h"
#include "afxwin.h"
# include <ConectorODBC/ConectorODBC.h>

// CDepartamentosView form view

class CDepartamentosView : public CConectorFormView
{
	DECLARE_DYNCREATE(CDepartamentosView)

	CConectorODBC	*m_pConector;
	CUsuario	*m_pUsuario;
	DWORD		m_dwCurrentID;

protected:
	CDepartamentosView();           // protected constructor used by dynamic creation
	virtual ~CDepartamentosView();

public:
	enum { IDD = IDD_FDEPARTAMENTOS };
	enum { query_departamentos=1, 
			query_insert,
			query_usuarios,
	};
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
	void OnVerUsuarios(int pos);
public:
	afx_msg void OnLvnKeydownLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrlXP m_LCUsuarios;
	afx_msg void OnNMDblclkLusuarios(NMHDR *pNMHDR, LRESULT *pResult);
};


