// sdgfingern.cpp: define las rutinas de inicialización del archivo DLL.
//

#include "stdafx.h"
#include "sdgfingern.h"
# include <Componentes/Componentes.h>
# include "SVEView.h"
# include "InformesView.h"
# include "HorariosView.h"
# include "FestivosView.h"
# include "DepartamentosView.h"
# include "ParametrosView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE AdministradorDLL= {NULL, NULL};
HINSTANCE g_hInstance=NULL;

//
//TODO: si este archivo DLL se vincula de forma dinámica con los archivos DLL de MFC,
//		las funciones que se exporten desde el archivo DLL y a las que se llame en
//		MFC deben agregar la macro AFX_MANAGE_STATE al
//		principio de la función.
//
//		Por ejemplo:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// colocar aquí el cuerpo de la función estándar
//		}
//
//		Es de suma importancia que esta macro aparezca en todas las funciones
//		antes de que se llamen en MFC. Esto significa que debe aparecer
//		en la primera instrucción de la función, 
//		antes incluso de cualquier declaración de variables de objetos,
//		ya que sus constructores pueden generar llamadas al archivo DLL de MFC.
//		
//
//		Consulte las notas técnicas 33 y 58 de MFC para obtener
//		detalles adicionales.
//

// CsdgfingernApp

BEGIN_MESSAGE_MAP(CsdgfingernApp, CWinApp)
END_MESSAGE_MAP()


// Construcción de CsdgfingernApp

CsdgfingernApp::CsdgfingernApp()
{
	// TODO: agregar aquí el código de construcción,
	// Colocar toda la inicialización importante en InitInstance
}


// El único objeto CsdgfingernApp

CsdgfingernApp theApp;


// Inicialización de CsdgfingernApp

BOOL CsdgfingernApp::InitInstance()
{
	CWinApp::InitInstance();
	g_hInstance= m_hInstance;
	if (!AfxInitExtensionModule(AdministradorDLL, m_hInstance))
		return false;
# ifdef _DEBUG
	DllRegisterServer();
# endif
	return TRUE;
}

__declspec(dllexport) UINT LoadFunciones3( CStringArray *funciones) {

	new CDynLinkLibrary(AdministradorDLL);
	(*funciones).Add("SVEView");
	(*funciones).Add("InformesView");
	(*funciones).Add("HorariosView");
	(*funciones).Add("FestivosView");
	(*funciones).Add("DepartamentosView");
	(*funciones).Add("ParametrosView");
	// (*funciones).Add("");
	return 1;
}

STDAPI DllRegisterServer(void)
{
	CRegKey key;
	HRESULT result;
	// registramos la ruta del dll
	if( (result= key.Create( HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\Addins\\50sdgfinger"))==ERROR_SUCCESS) {

		if( (result= key.SetStringValue( "Nombre", "sdgfingern"))!= ERROR_SUCCESS)
			return result;
		if( (result= key.SetStringValue( "Descripcion", "Modulo principal"))!= ERROR_SUCCESS)
			return result;
		if( (result= key.SetDWORDValue("ValidGrups", 1))!= ERROR_SUCCESS)
			return result;
		TCHAR szEXEPath[MAX_PATH];
		ASSERT(g_hInstance);
		GetModuleFileName(g_hInstance, szEXEPath, MAX_PATH);
		if( (result= key.SetStringValue( "FullPath", szEXEPath))!= ERROR_SUCCESS)
			return result;
		key.Close();
		return S_OK;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	CRegKey key;
	HRESULT result;
	if( (result= key.Open( HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\addins\\"))==ERROR_SUCCESS) {

		if( (result= key.DeleteSubKey("50sdgfinger"))!=ERROR_SUCCESS)
			if( (result= key.RecurseDeleteKey("50sdgfinger"))!=ERROR_SUCCESS)
				return result;
		key.Close();
		return S_OK;
	}
	return NULL;
}

__declspec(dllexport) CMultiDocTemplateXP *SVEView(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CSVEView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Usuarios";
	*parent= "SDGFinger Nómina";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *InformesView(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
{
	/*if(dwGrupo && !CUsuario3::GetPermiso(permisos, "CInformesView")) 
		return NULL;*/
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CInformesView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Informes";
	*parent= "SDGFinger Nómina";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *HorariosView(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
{
	/*if(dwGrupo && !CUsuario3::GetPermiso(permisos, "CHorariosView")) 
		return NULL;*/
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CHorariosView));
	pApp->AddDocTemplate(pDocTemplate);
	// hIcon = pApp->LoadIcon(128);
	*strLabel = "Horarios";
	*parent= "SDGFinger Nómina";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *FestivosView(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
{
	/*if(dwGrupo && !CUsuario3::GetPermiso(permisos, "CFestivosView")) 
		return NULL;*/
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CFestivosView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Festivos";
	*parent= "SDGFinger Nómina";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *DepartamentosView(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
{
	/*if(dwGrupo && !CUsuario3::GetPermiso(permisos, "CDepartamentosView")) 
		return NULL;*/
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CDepartamentosView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Departamentos";
	*parent= "SDGFinger Nómina";
	return pDocTemplate;
}

__declspec(dllexport) CMultiDocTemplateXP *ParametrosView(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
{
	/*if(dwGrupo && !CUsuario3::GetPermiso(permisos, "CParametrosView")) 
		return NULL;*/
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);
	CMultiDocTemplateXP *pDocTemplate;

	pDocTemplate =  new CMultiDocTemplateXP(128,
		RUNTIME_CLASS(CComponentesDoc),
		RUNTIME_CLASS(CMDIChildWndXP),
		RUNTIME_CLASS(CParametrosView));
	pApp->AddDocTemplate(pDocTemplate);
	*strLabel = "Parametros";
	*parent= "SDGFinger Nómina";
	return pDocTemplate;
}

//__declspec(dllexport) CMultiDocTemplateXP *(CString *strLabel, const CMiscelaneosEx &permisos,DWORD dwGrupo, CString *parent)
//{
//	/*if(dwGrupo && !CUsuario3::GetPermiso(permisos, "C")) 
//		return NULL;*/
//	CWinApp* pApp = AfxGetApp();
//	ASSERT(pApp != NULL);
//	CMultiDocTemplateXP *pDocTemplate;
//
//	pDocTemplate =  new CMultiDocTemplateXP(128,
//		RUNTIME_CLASS(CComponentesDoc),
//		RUNTIME_CLASS(CMDIChildWndXP),
//		RUNTIME_CLASS(C));
//	pApp->AddDocTemplate(pDocTemplate);
//	// hIcon = pApp->LoadIcon(128);
//	*strLabel = "";
//	*parent= "SDGFinger Nómina";
//	return pDocTemplate;
//}
