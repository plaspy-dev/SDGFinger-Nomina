// SVEView.h: interfaz de la clase CSVEView
//


#pragma once

#include "afxwin.h"
#include "afxctl.h"
# include "printsve.h"
#include "afxcmn.h"
# include "UsuarioHorario.h"
# include <iohw/iohw.h>
# include "DlgPopUp.h"

class CSVEView : public CFormView
{
	// display lcd
	CDisplayThread	*m_pDisplay;
	CString			m_szLastSearch;
	CString			m_szLastHora;
	CString			m_szLastUsuario;
	bool			m_bSalida;
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
	int			m_nTimer;

	bool		m_bExiste;

	CConector	*m_pConector;
	CUsuario	*m_pUsuario;

	CPrintSVE		*m_pPrint;
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
	DWORD			m_pTemporal;
	CEditXP m_EDocumento;
	CEditXP m_ENombre;
	CEditXP m_EApellido;
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnKillfocusEcedula();
	void Clean(bool search=false);
	afx_msg void OnDestroy();
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	LRESULT OnFingerMessage(WPARAM wParam, LPARAM lParam);

	CFingerBitmap	m_SHuella;

	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

protected:
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnUpdatePrint(CCmdUI *pCmdUI);
public:
	CBitmapPicture m_SLogo;
public:
	CStatic m_SMensaje;
	CFingerBitmap m_SPulgarD;
	CFingerBitmap m_SPulgarI;
	CFingerBitmap m_SIndiceD;
	CFingerBitmap m_SIndiceI;
	CFingerBitmap m_SCorazonD;
	CFingerBitmap m_SCorazonI;
	CFingerBitmap m_SAnularD;
	CFingerBitmap m_SAnularI;
	CFingerBitmap m_SMeniqueD;
	CFingerBitmap m_SMeniqueI;
public:
	afx_msg void OnStnClickedShuella();
	int InsertarHuella(CString documento, CFingerMessage *bitmap, CString dedo);
	CFingerBitmap *GetDedo(int dedo);
public:
	CEditXP m_EObservaciones;
public:
	CComboBoxXP m_CBHorario;
public:
	CButtonXP m_BHorario;
public:
	afx_msg void OnBnClickedBhorario();
public:
	CComboBoxXP m_CBDepartamento;
public:
	CEditFloat		m_EValorHora;
	CMiscelaneosEx	m_pDepartamentos;
	LRESULT OnUpdateCombo(WPARAM wParam, LPARAM lParam);
	CFingerBitmap *GetSeleccion();
	LRESULT OnCloseEx(WPARAM wParam, LPARAM lParam);

	void SetMatrizTexto(CString texto, int estilo=0, CString linea2="");
	CDlgPopUp		m_pDlgPopUp;
	bool		m_bActiva;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	LRESULT OnUpdateHorarios(WPARAM wParam, LPARAM lParam);
};


