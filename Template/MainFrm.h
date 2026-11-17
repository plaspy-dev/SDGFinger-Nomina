// MainFrm.h: interfaz de la clase CMainFrame
//


#pragma once

#include <conector/conector.h>
# include <ConectorODBC/ConectorODBC.h>

class CMainFrame : public CMDIFrameWndXP
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
private:

	// varios
	bool	Messages( MySQLMessage *contenido);
	bool	DeleteHilos( MySQLMessage *hilo);
	bool	LoadDocumentos();
	CPropiedadesBar	m_PropBar;
// Atributos
public:

// Operaciones
public:

// Reemplazos
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementación
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	bool			Conectar();
	CUsuario3		*m_pUsuario;
	void			ResetToolbars();
	CConectorODBC		m_pConector;

	virtual void	SaveState();
	virtual void	LoadState();


// Funciones de asignación de mensajes generadas
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateVerNavegador(CCmdUI *pCmdUI);
	afx_msg void OnConfiguracionServidores();
	afx_msg void OnVerBarradenavegacion();
	afx_msg void OnUpdateVerBarradenavegacion(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg LRESULT OnQueryConector(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySQL(WPARAM wParam, LPARAM lParam);
	int OnMySQLThreadEnd(MySQLMessage *hilo);
	int OnMySQLQueryResult(MySQLMessage *message);
	int OnQueryEmpty(int owner);
	int OnQueryTitle(MySQLMessage *message);
	virtual void InitializeRibbon();
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnVerPropiedades();
	afx_msg void OnUpdateVerPropiedades(CCmdUI *pCmdUI);
	LRESULT OnModificaciones(WPARAM wParam, LPARAM lParam);
};
