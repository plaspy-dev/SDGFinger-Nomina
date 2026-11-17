// MainFrm.h: interfaz de la clase CMainFrame
//
#pragma once

#include "conector/conector.h"
# include "PropiedadesBar.h"

class CMainFrame : public CMDIFrameWndXP
{
	DECLARE_DYNAMIC(CMainFrame)
	// implementado para tener mas dinamismo en los archivos

private:

	// varios
	bool	Messages( MySQLMessage *contenido);
	bool	DeleteHilos( CMySQLMessage *hilo);
	bool	LoadDocumentos();
	CFingerPrint	*m_pFinger;
	CSplashScreenEx *m_pSplash;
protected:
	CToolBarXP		m_wndToolBar;
// Reemplazos
public:
	bool			Conectar();
	CUsuario		*m_pUsuario;
	void			ResetToolbars();
	CPropiedadesBar	m_PropBar;

	CConector		m_pConector;

	virtual void	SaveState();
	virtual void	LoadState();

	CMainFrame();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

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
	int OnMySQLThreadEnd(CMySQLMessage *hilo);
	int OnMySQLQueryResult(MySQLMessage *message);
	int OnQueryEmpty(int owner);
	int OnQueryTitle(MySQLMessage *message);
	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);
	void OnVerPropiedades();
	void OnUpdateVerPropiedades(CCmdUI *pCmdUI);
public:
	afx_msg void OnDestroy();
	LRESULT OnQueryFinger(WPARAM wParam, LPARAM lParam);
	LRESULT OnFingerPrint(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnHerramientasOpciones();
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	CakTrayIcon m_TrayIcon;
	LRESULT OnTrayIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMostrar();
	afx_msg void OnAcercaDe();
	afx_msg void OnSalir();
};
