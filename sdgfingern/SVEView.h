// SVEView.h: interfaz de la clase CSVEView
//

#pragma once

#include "afxwin.h"
#include "afxctl.h"
#include "afxcmn.h"
# include "UsuarioHorario.h"
# include <iohw/iohw.h>

class CSVEView : public CConectorFormView
{
protected: // Crear sólo a partir de serialización
	CSVEView();
	DECLARE_DYNCREATE(CSVEView)

public:
	enum{ IDD = IDD_SVE_FORM };
	enum {query_horarios=1, 
		query_departamentos, 
		query_horariousuario, 
		query_huella, 
		query_showhuellas,
		query_usuario,
		query_selectdatoshuella,
		query_selectdatosturno,
	};
	CString		m_szOld;
	CString		m_szDocumento;

	bool		m_bExiste;

	CConectorODBC	*m_pConector;
	CUsuario		*m_pUsuario;
	CUsuarioHorario	*m_pActual;

// Operaciones
public:

// Reemplazos
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

// Implementación
public:
	virtual ~CSVEView();

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
public:
	CEditXP m_EDocumento;
	CEditXP m_ENombre;
	CButtonXP m_BCancelar;

	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnKillfocusEcedula();
	void Clean(bool search=false);
	afx_msg void OnDestroy();
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);
public:
	CComboBoxXP		m_CBDepartamento;
public:
	CEditFloat		m_EValorHora;
	CMiscelaneosEx	m_pDepartamentos;
protected:
	LRESULT OnUpdateHorarios(WPARAM wParam, LPARAM lParam);
public:
	CComboBoxXP m_CBHorario;
	CButtonXP m_BOtro;
	CButtonXP m_BAceptar;
	afx_msg void OnBnClickedBaceptar();
	afx_msg void OnBnClickedBotro();
	CEditXP m_EDocumentoReal;
	afx_msg void OnEnKillfocusEdocumento();
};


