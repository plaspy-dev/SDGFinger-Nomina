// Template.cpp : define los comportamientos de las clases para la aplicación.
//

#include "stdafx.h"
#include "Template.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTemplateApp

BEGIN_MESSAGE_MAP(CTemplateApp, CWinAppXP)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateSave)
END_MESSAGE_MAP()


// Construcción de CTemplateApp

CTemplateApp::CTemplateApp()
{
}

CTemplateApp theApp;


BOOL CTemplateApp::InitInstance()
{	
	CString myid;
	myid.Format("{6A97833B-7441-47%d-B96A-B9D28D4B4DF9}",CApplicacion::GetID());
	if(!CWinAppXP::InitInstance(myid))
		return false;

	AfxEnableControlContainer();

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Inicializar bibliotecas OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	
	SetRegistryKey(_T("SDG"));

	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// quitar de aqui
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	return TRUE;
}

void CTemplateApp::OnFileNew() {

	if(((CMainFrame* )m_pMainWnd)->m_pUsuario) {
		((CMainFrame* )m_pMainWnd)->m_bInactivo= true;
	} else if( ((CMainFrame* )m_pMainWnd)->Conectar()) { // load papel defaults
		PRINTDLG pd;
		pd.lStructSize=(DWORD)sizeof(PRINTDLG);
		BOOL bRet=GetPrinterDeviceDefaults(&pd);
		if(bRet)
		{
			// protect memory handle with ::GlobalLock and ::GlobalUnlock
			DEVMODE FAR *pDevMode=(DEVMODE FAR *)::GlobalLock(m_hDevMode);
			// set orientation to landscape
			pDevMode->dmOrientation= GetProfileInt("printer", "orientacion", DMORIENT_PORTRAIT);
			pDevMode->dmPaperSize= GetProfileInt("printer", "papel", DMPAPER_LETTER);
			::GlobalUnlock(m_hDevMode);
		}
	}
}

void CTemplateApp::OnFileOpen() {

}

CDocument* CTemplateApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	return NULL;
}

void CTemplateApp::OnFileSave()
{
}

// Comando de la aplicación para ejecutar el cuadro de diálogo
void CTemplateApp::OnAppAbout()
{
	AboutBox(TEMPLATE_VERSION);
}


void CTemplateApp::OnUpdateSave(CCmdUI *pCmdUI) {

	pCmdUI->Enable(false);
}
