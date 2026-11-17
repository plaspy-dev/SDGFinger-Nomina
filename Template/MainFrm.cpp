// MainFrm.cpp : implementación de la clase CMainFrame
//

#include "stdafx.h"
#include "Template.h"
#include "MainFrm.h"
# include <iofile/IOFile.h>
# include <JPrint/JPrint.h>
#include "odbcinst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

# define	ANIMATION_TIMER			300
# define	TIMER_EVENT				32000


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndXP)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndXP)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VER_NAVEGADOR, OnUpdateVerNavegador)
	ON_COMMAND(ID_CONFIGURACION_SERVIDORES, OnConfiguracionServidores)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_MESSAGE(WM_QUERYCONECTOR,OnQueryConector)
	ON_MESSAGE(WM_QUERYUSUARIO,OnQueryUsuario) 
	ON_MESSAGE(WM_ODBC,OnMySQL)
	ON_UPDATE_COMMAND_UI_RANGE(VENTANAS_PREFIJO, VENTANAS_PREFIJO+50, OnUpdateVentanas)
	ON_COMMAND_RANGE(VENTANAS_PREFIJO, VENTANAS_PREFIJO+50, OnCommandVentanas)
	ON_COMMAND(ID_VER_PROPIEDADES, OnVerPropiedades)
	ON_UPDATE_COMMAND_UI(ID_VER_PROPIEDADES, OnUpdateVerPropiedades)
	ON_MESSAGE(WM_UPDATEMODIFICATION, OnModificaciones)
END_MESSAGE_MAP()

// Construcción y destrucción de CMainFrame

CMainFrame::CMainFrame()
{
	m_pUsuario= NULL;
	// m_nAnimation=0;
	// usadas solo para el linker
	CFileInfo info;
	CNumberClass temp;
}

CMainFrame::~CMainFrame()
{
	// borramos todos los archivos
	CFileInfoArray fileinfo;
	fileinfo.AddDir(CImport::GetTemporalPath(""), "*.*", true);
	for(int i=0; i<fileinfo.GetCount(); i++) 
		DeleteFile(((CFileInfo )fileinfo.GetAt(i)).GetFilePath());
	RemoveDirectory(CImport::GetTemporalPath(""));
	if(m_pUsuario)
		delete m_pUsuario;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndXP::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_PropBar.Create(_T("Propiedades"), this, CRect(0, 0, 200, 200),TRUE, 514, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create my ODS Bar\n");
		return -1;
	}

	m_PropBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_PropBar);

	AfxGetApp()->WriteProfileString("conector","separadori", "[");
	AfxGetApp()->WriteProfileString("conector","separadorf", "]");

	// el conector
	m_pConector.SetMainFrame(this);
	m_pConector.Cargar();

	SetFlash(CApplicacion::GetAppPath( AfxGetApp()->GetProfileString("swfop", "archivo", "logo.swf")), 
		AfxGetApp()->GetProfileInt("swfop", "ancho", 456),
		AfxGetApp()->GetProfileInt("swfop", "alto", 238),
		AfxGetApp()->GetProfileInt("swfop", "bkcolor", -1)
		);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndXP::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}


// Diagnósticos de CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// Controladores de mensajes de CMainFrame


void CMainFrame::ResetToolbars() {

}

void CMainFrame::LoadState() {

	if( !AfxGetApp()->GetProfileInt("barras", "store",0)) {
		m_PropBar.SetAutoHideMode(TRUE,CBRS_LEFT, NULL, FALSE);
	}
	CMDIFrameWndXP::LoadState();
}

void CMainFrame::SaveState() {

	if( !m_pUsuario || !m_pUsuario->GetID())
		((CWinAppXP *)AfxGetApp())->SetSaveState(false);
	else CMDIFrameWndXP::SaveState();
}

bool CMainFrame::Conectar() {

	char szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	char *pszBuf = szBuf;
	CString sDriver;

	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut)) {
		MessageBox("No se puede tener acceso a las bases de datos instaladas");
		return false;
	}

	// Search for the driver...
	do
	{
		if( strstr( pszBuf, "Access" ) != 0 )
		{
			// Found !
			sDriver = CString( pszBuf );
			break;
		}
		pszBuf = strchr( pszBuf, '\0' ) + 1;
	}
	while( pszBuf[1] != '\0' );

	if(sDriver.IsEmpty()) {
		MessageBox("No se encontro la instalación del driver de MS Access");
		return false;
	}
	CString ruta;
	ruta=AfxGetApp()->GetProfileString("conectorodbc", "ruta");
	if(ruta.IsEmpty()) {

		CMyRegKey key(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Components\\82EF0911266887046991D06486F7D062");
		ruta= key.GetString("", "66E2FD99366249E4196E819E820C93D5");
		if(ruta.IsEmpty()) {
			MessageBox("No se encontro la instalación de TimeKeeper, por favor seleccione la base de datos.");
			CFileDialog dlg(true, "mdb", NULL, NULL,"Archivos de Access|*.mdb|Todos los archivos (*.*)|*.*||");
			if(dlg.DoModal()!=IDOK)
				return false;
			ruta= dlg.GetPathName();
			AfxGetApp()->WriteProfileString("conectorodbc", "ruta", ruta);
		}
	}
	m_pConector.SetMainFrame(this);
	m_pConector.AddDataBase(ruta, "", "", sDriver);

	if(m_pConector.TestConection(this)) {

		if(!m_pConector.CanUpdate()) 
			MessageBox("La base de datos '" +ruta + "' es de solo lectura", NULL, MB_ICONWARNING);

		m_pUsuario= new CUsuario3("Administrador", 1, 0);
		m_pUsuario->SetNombreCompleto("Administrador");
		
		m_pNavBar.m_Tree.DeleteAllItems();
		
		m_pConector.SetPasswd(m_pUsuario->GetContrasena());
		m_pConector.SetUser(m_pUsuario->GetCodificado());
		
		CMFCRibbonStatusBarPane *elemento= (CMFCRibbonStatusBarPane *)m_wndStatusBar.FindElement(ID_STATUSBAR_USUARIO);
		if(elemento) 
			elemento->SetText(m_pUsuario->GetNombreCompleto());
		// m_PropBar.SetConector(&m_pConector);

		// registrarse en el chat
		SendMessage(WM_REGISTRARUSUARIO, 0, -2);
		AfxGetApp()->WriteProfileInt("control", "loadstate", 1);
		LoadDocumentos();
		LoadState();
		
		SendMessageBroadCast(WM_QUERYUSUARIO, 0, (LPARAM)m_pUsuario);
		SendMessageBroadCast(WM_QUERYCONECTOR, 0, (LPARAM)&m_pConector);
		SendMessageBroadCast(WM_INACTIVIDAD, 0,1);
		AfxGetApp()->WriteProfileInt("control", "loadstate", 0);
		m_bConectando= false;
		return true;
	}
	return false;
}

bool CMainFrame::LoadDocumentos() {

	FUNCTION_STACK_TRACE("AAAAAAAB");

# ifdef _DEBUG
	CMDIFrameWndXP::LoadDocumentosEx("sdgfingernd.dll", NULL, *m_pUsuario->GetPermisos(), m_pUsuario->GetGrupo());
# endif 

	CMDIFrameWndXP::LoadDocumentos(m_pUsuario->GetGrupo());
	return CMDIFrameWndXP::LoadDocumentos(*m_pUsuario->GetPermisos(), m_pUsuario->GetGrupo());
}

// Diagnósticos de CMainFrame

LRESULT CMainFrame::OnQueryConector(WPARAM wParam, LPARAM lParam) {

	if(lParam) {
		CWnd *temp= (CWnd *) lParam;
		temp->SendMessage( WM_QUERYCONECTOR, 0, (LPARAM) &m_pConector);
	}
	return (LRESULT) &m_pConector;
}

LRESULT CMainFrame::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	if(lParam) {
		CWnd *temp= (CWnd *) lParam;
		temp->SendMessage( WM_QUERYUSUARIO, 1, (LPARAM) m_pUsuario);
	}
	return (LRESULT) m_pUsuario;
} 

int CMainFrame::OnMySQLThreadEnd(MySQLMessage *hilo) {

	OnStatusBar(2, 0);
	if( hilo->GetID()>49000 || TIMER_EVENT==hilo->GetID()) {
		return 0;
	} else if( hilo->GetID()>99) {
	} else DeleteHilos(hilo);
	return 0;
}

int CMainFrame::OnMySQLQueryResult(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
			{
				CMyTime tiempo(message->GetAt(0));
				m_pTServidor=tiempo;
				m_pTLocal= CTime::GetCurrentTime();
				m_psDiferencia= m_pTLocal-m_pTServidor;
				SetTimer(TIMER_EVENT, 1000, NULL);
				SendMessageBroadCast(WM_QUERYTIME, 0, (LPARAM)&m_pTServidor);
			}
		case 50001: // no se hace nada
			return 0;
	}
	if( message->GetID()>99) {
		OnStatusBar(1, 0);
	} else Messages(message); // SOLO en el caso de NEW_XML
	return 0;
}

int CMainFrame::OnQueryEmpty(int owner) {

	switch( owner) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
			return 0;
	}
	return 0;
}

int CMainFrame::OnQueryTitle(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
		case 50000: // chat
			return 0;
	}
	return 0;
}

LRESULT CMainFrame::OnMySQL(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage *mensaje= (CMySQLMessage *)lParam;
	switch( wParam) 
	{
	case THREAD_END:
		if(mensaje->GetParent()==this)
			OnMySQLThreadEnd(mensaje);
		else 
			OnStatusBar(2, 0);
		break;
	case QUERY_COUNT2:
		OnStatusBar(0, mensaje->GetCount());
		break;
	case QUERY_RESULT:
		if(mensaje->GetParent()==this)
			OnMySQLQueryResult(mensaje);
		else 
			OnStatusBar(1, 0);
		break;
	case QUERY_TITLE:
		if(mensaje->GetParent()==this)
			OnQueryTitle(mensaje);
		break;
	case QUERY_EMPTY:
		if(mensaje->GetParent()==this)
			OnQueryEmpty(mensaje->GetID());
		break;
	case QUERY_ERROR:
		SetErrorEx(NULL, mensaje->GetAt(0)+ " \r\nMientras se realizaba la consulta\r\n" + mensaje->GetAt(1), 3);
		break;
	}
	return 0;
}

bool CMainFrame::DeleteHilos( MySQLMessage *hilo) {
	return true;
}

bool CMainFrame::Messages( MySQLMessage *contenido) {

	FUNCTION_STACK_TRACE("AAAAAAAE");
	OnStatusBar(1, 0);
	return true;
}

void CMainFrame::OnUpdateVerNavegador(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	CMDIFrameWndXP::OnUpdateVerNavegador(pCmdUI);
}

void CMainFrame::OnConfiguracionServidores()
{
	CFileDialog dlg(true, "mdb", NULL, NULL,"Archivos de Access|*.mdb|Todos los archivos (*.*)|*.*||");
	if(dlg.DoModal()==IDOK) {
		
		AfxGetApp()->WriteProfileString("conectorodbc", "ruta", dlg.GetPathName());
		if( m_pUsuario) {
			SetErrorEx(NULL, "Debe Reiniciar " + CApplicacion::GetAppName() + " para realizar los cambios", 0, "conector");
		} else {
			m_pConector.SetMainFrame( this);
			Conectar();
		}
	}
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	if( nIDEvent==TIMER_EVENT) {
		CTime tiempo= CTime::GetCurrentTime();
		CTimeSpan span(0, 0, 1, 0);
		if( tiempo<m_pTLocal || span < tiempo-m_pTLocal) {
			KillTimer(TIMER_EVENT);
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		} else {
			m_pTLocal= tiempo;
			m_pTServidor= m_pTLocal-m_psDiferencia;
		}
	}
	CMDIFrameWndXP::OnTimer(nIDEvent);
}

void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pUsuario || AfxGetApp()->GetProfileInt("control", "timeout", 1));
}

void CMainFrame::InitializeRibbon()
{
	// Load panel images:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// Init main button:
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText("Archivo");

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory("Archivo", IDB_FILESMALL, IDB_FILELARGE);

	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, "Nuevo", 0, 0));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_OPEN, "Abrir", 1, 1));
	CMFCRibbonButton* pBtnSave = new CMFCRibbonButton(ID_FILE_SAVE_AS, "Guardar Como", 3, 3);
	pBtnSave->AddSubItem(new CMFCRibbonButton(ID_FILE_SAVE_AS+300, "Formato Fijos", 3, 3, TRUE));
	pBtnSave->AddSubItem(new CMFCRibbonButton(ID_FILE_SAVE_AS+301, "Formato Temporales", 3, 3, TRUE));
	pMainPanel->Add(pBtnSave);

	CMFCRibbonButton* pBtnPrint = new CMFCRibbonButton(ID_FILE_PRINT, "Imprimir", 6, 6);
	pBtnPrint->SetKeys(_T("p"), _T("w"));
	pBtnPrint->AddSubItem(new CMFCRibbonLabel("Previsualizar e imprimir el documento"));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_DIRECT, "&Impresión rapida", 7, 7, TRUE));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_PREVIEW, "&Vista preliminar", 8, 8, TRUE));
	pBtnPrint->AddSubItem(new CMFCRibbonButton(ID_FILE_PRINT_SETUP, "&Configurar página", 11, 11, TRUE));
	pMainPanel->Add(pBtnPrint);
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));
	pMainPanel->Add(new CMFCRibbonButton(ID_CONFIGURACION_SERVIDORES, "Base de datos", 2, 2));
	pMainPanel->Add(new CMFCRibbonSeparator(TRUE));
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_CLOSE, "Cerrar", 9, 9));

	pMainPanel->AddRecentFilesList("");
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, "Salir", 15));
	// este es el menu principal

	// Add "Home" category with "Clipboard" panel:
	CMFCRibbonCategory* pCategoryHome = m_wndRibbonBar.AddCategory("Inicio", IDB_WRITESMALL, IDB_WRITELARGE);

	// crear panel de impresion
	CMFCRibbonPanel* pPanelImpresion = pCategoryHome->AddPanel("Impresión\ne", m_PanelImages.ExtractIcon(27));
	pPanelImpresion->Add(new CMFCRibbonButton(ID_FILE_PRINT, "Imprimir\nv", 11, 2));

	// Create "Clipboard" panel:
	CMFCRibbonPanel* pPanelClipboard = pCategoryHome->AddPanel("Portapapeles\nd", m_PanelImages.ExtractIcon(27));

	CMFCRibbonButton* pBtnPaste = new CMFCRibbonButton(ID_EDIT_PASTE, "Pegar\nv", 0, 0);
	pPanelClipboard->Add(pBtnPaste);

	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_CUT, "Cortar\nx", 1));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_COPY, "Copiar\nc", 2));
	pPanelClipboard->Add(new CMFCRibbonButton(ID_EDIT_SELECT_ALL, "Seleccionar Todo\ne", -1));

	// Create and add a "View" panel:
	CMFCRibbonPanel* pPanelView = pCategoryHome->AddPanel("Ver", m_PanelImages.ExtractIcon (7));

	CMFCRibbonButton* pBtnStatusBar = new CMFCRibbonCheckBox(ID_VIEW_STATUS_BAR, "Barra de estado");
	pPanelView->Add(pBtnStatusBar);
	pPanelView->Add(new CMFCRibbonCheckBox(ID_VER_NAVEGADOR, "Navegador"));
	pPanelView->Add(new CMFCRibbonCheckBox(ID_VER_PROPIEDADES, "Propiedades"));

	// Create and add a "Windows" panel:
	CMFCRibbonPanel* pPanelWindow = pCategoryHome->AddPanel("Ventana", m_PanelImages.ExtractIcon (7));

	CMFCRibbonButton* pBtnWindows = new CMFCRibbonButton(ID_WINDOW_MANAGER, "Ventanas", -1, 1);
	pBtnWindows->SetMenu(IDR_WINDOWS_MENU, TRUE);
	pPanelWindow->Add(pBtnWindows);

	// crear panel de impresion
	CMFCRibbonPanel* pPanelAyuda = pCategoryHome->AddPanel("Ayuda\ne", m_PanelImages.ExtractIcon(27));
	pPanelAyuda->Add(new CMFCRibbonButton(ID_HELP, "Ayuda\nv", 12, 3));


	// Add quick access toolbar commands:
	CList<UINT, UINT> lstQATCmds;

	lstQATCmds.AddTail(ID_FILE_SAVE_AS);
	lstQATCmds.AddTail(ID_FILE_PRINT);
	lstQATCmds.AddTail(ID_FILE_OPEN);

	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);
	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon(0)));

	CMDIFrameWndXP::InitializeRibbon();
}

void CMainFrame::OnVerPropiedades()
{
	if(::IsWindow(m_PropBar)) 
		m_PropBar.ShowPane(!m_PropBar.IsWindowVisible(), false, true);
}

void CMainFrame::OnUpdateVerPropiedades(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	pCmdUI->SetCheck( m_PropBar.IsWindowVisible());
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	BOOL retorno= CMDIFrameWndXP::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
	if(m_PropBar.IsAutoHideMode())
		m_PropBar.UnSetAutoHideMode(NULL);
	m_PropBar.ShowPane(false,false,false);
	return retorno;
}

LRESULT CMainFrame::OnModificaciones(WPARAM wParam, LPARAM lParam) {

	m_PropBar.SetPropiedades(*(CPropiedades *)lParam, wParam);
	return 0;
}

