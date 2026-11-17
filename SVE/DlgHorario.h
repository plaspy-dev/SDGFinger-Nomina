#pragma once

# include "usuariohorario.h"

// CDlgHorario dialog

class CDlgHorario : public CDialog
{
	DECLARE_DYNAMIC(CDlgHorario)

public:
	CDlgHorario(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgHorario();
	CUsuarioHorario	*m_pActual;
	bool			m_bEditable;

// Dialog Data
	enum { IDD = IDD_DHORARIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_RDiario;
public:
	CButton m_RDiarioHorario;
public:
	CDateTimeCtrl m_DTDiarioEntrada;
public:
	CDateTimeCtrl m_DTDiarioSalida;
public:
	CDateTimeCtrl m_DTDiarioHoras;
public:
	CButton m_CLunes;
public:
	CButton m_CMartes;
public:
	CButton m_CMiercoles;
public:
	CButton m_CJueves;
public:
	CButton m_CViernes;
public:
	CButton m_CSabado;
public:
	CButton m_CDomingo;
public:
	CButton m_RSemana;
public:
	CDateTimeCtrl m_DTSemanaHoras;
public:
	CCheckBoxSeparator m_CAlmuerzo;
public:
	CButton m_RAlmuerzoHorario;
public:
	CDateTimeCtrl m_DTAlmuerzoSalida;
public:
	CDateTimeCtrl m_DTAlmuerzoEntrada;
public:
	CDateTimeCtrl m_DTAlmuerzoHoras;
public:
	CCheckBoxSeparator m_CSabados;
public:
	CButton m_RSabadosHorario;
public:
	CDateTimeCtrl m_DTSabadosEntrada;
public:
	CDateTimeCtrl m_DTSabadosSalida;
public:
	CDateTimeCtrl m_DTSabadosHoras;
public:
	CCheckBoxSeparator m_CNoHabil;
public:
	CButton m_RNoHabilHorario;
public:
	CDateTimeCtrl m_DTNoHabilEntrada;
public:
	CDateTimeCtrl m_DTNoHabilSalida;
public:
	CDateTimeCtrl m_DTNoHabilHoras;
public:
	CStaticSeparator m_SFin;
public:
	CButtonXP m_BCancelar;
public:
	CButtonXP m_BAceptar;
public:
	afx_msg void OnBnClickedBaceptar();
public:
	afx_msg void OnBnClickedBcancelar();
public:
	afx_msg void OnBnClickedRdiariohor();
public:
	afx_msg void OnBnClickedRdiario();
public:
	afx_msg void OnBnClickedRsemanal();
	void UpdateSemana(bool semana);
public:
	afx_msg void OnBnClickedRalmuerzohorario();
public:
	afx_msg void OnBnClickedRsabadoshorario();
public:
	afx_msg void OnBnClickedRnohabileshorario();
public:
	afx_msg void OnBnClickedCalmuerzo();
public:
	afx_msg void OnBnClickedCsabados();
public:
	afx_msg void OnBnClickedCnohabiles();
public:
	virtual BOOL OnInitDialog();
	void OnEditar();
};
