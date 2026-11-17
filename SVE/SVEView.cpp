// SVEView.cpp: implementación de la clase CSVEView
//

#include "stdafx.h"
#include "SVE.h"

#include "SVEDoc.h"
#include "SVEView.h"
# include "UsuarioHorario.h"
# include "DlgHorario.h"
# include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


# define RETRASO_MENSAJE		5*1000
# define CLEAN_OLD				10*60*1000


// CSVEView

IMPLEMENT_DYNCREATE(CSVEView, CFormView)

BEGIN_MESSAGE_MAP(CSVEView, CFormView)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_ECEDULA, OnEnKillfocusEcedula)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_MESSAGE(WM_FINGERPRINT, OnFingerMessage)
	ON_BN_CLICKED(IDC_BHORARIO, &CSVEView::OnBnClickedBhorario)
	ON_MESSAGE(WM_REALUPDATECOMBO, OnUpdateCombo)
	ON_MESSAGE(WM_CLOSEEX, OnCloseEx)
	ON_WM_TIMER()
	ON_MESSAGE(WM_UPDATEHORARIOS, OnUpdateHorarios)
END_MESSAGE_MAP()

// Construcción o destrucción de CSVEView

CSVEView::CSVEView()
	: CFormView(CSVEView::IDD)
{
	m_pPrint=NULL;
	m_bExiste=false;
	m_pActual=NULL;
	m_pDisplay=NULL;
	m_bActiva=false;
	m_bSalida=false;
}

CSVEView::~CSVEView()
{
	if( m_pPrint) 
		delete m_pPrint;
	if(m_pActual)
		delete m_pActual;
}

void CSVEView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ECEDULA, m_EDocumento);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_BUTTON3, m_BCancelar);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_SLOGO, m_SLogo);
	DDX_Control(pDX, IDC_SMENSAJEI, m_SMensaje);
	DDX_Control(pDX, IDC_SPULGARD, m_SPulgarD);
	DDX_Control(pDX, IDC_SPULGARI, m_SPulgarI);
	DDX_Control(pDX, IDC_SINDICED, m_SIndiceD);
	DDX_Control(pDX, IDC_SINDICEI, m_SIndiceI);
	DDX_Control(pDX, IDC_SCORAZOND, m_SCorazonD);
	DDX_Control(pDX, IDC_SCORAZONI, m_SCorazonI);
	DDX_Control(pDX, IDC_SANULARD, m_SAnularD);
	DDX_Control(pDX, IDC_SANULARI, m_SAnularI);
	DDX_Control(pDX, IDC_SMENIQUED, m_SMeniqueD);
	DDX_Control(pDX, IDC_SMENIQUEI, m_SMeniqueI);
	DDX_Control(pDX, IDC_EOBSERVACIONES, m_EObservaciones);
	DDX_Control(pDX, IDC_CHORARIO, m_CBHorario);
	DDX_Control(pDX, IDC_BHORARIO, m_BHorario);
	DDX_Control(pDX, IDC_CDEPARTAMENTO, m_CBDepartamento);
	DDX_Control(pDX, IDC_EVALOR, m_EValorHora);
}

BOOL CSVEView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CSVEView::OnInitialUpdate()
{
	CWaitCursor wait;

	CFormView::OnInitialUpdate();
	
	GetDocument()->SetTitle("Ingreso");
	
	m_pConector= (CConector *) AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *) AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);

	if(AfxGetApp()->GetProfileInt("cliente", "reemplazar", 0)) {
		if(AfxGetApp()->GetProfileInt("opciones", "display", 0)) {
			m_pDisplay= new CDisplayThread(AfxGetApp()->GetProfileInt("opciones", "direccion", 888));
			m_pDisplay->Run();
		}
		m_pDlgPopUp.Create(CDlgPopUp::IDD, this);
	}

	m_SLogo.SetBitmap(CApplicacion::GetBitmap());
	
	m_SHuella.HideInfo();
	m_SPulgarD.HideInfo();
	m_SPulgarI.HideInfo();
	m_SIndiceD.HideInfo();
	m_SIndiceI.HideInfo();
	m_SCorazonD.HideInfo();
	m_SCorazonI.HideInfo();
	m_SAnularI.HideInfo();
	m_SAnularD.HideInfo();
	m_SMeniqueI.HideInfo();
	m_SMeniqueD.HideInfo();

	m_SPulgarD.SetMessage("Pulgar");
	m_SPulgarI.SetMessage("Pulgar");
	m_SIndiceD.SetMessage("Indice");
	m_SIndiceI.SetMessage("Indice");
	m_SCorazonD.SetMessage("Corazón");
	m_SCorazonI.SetMessage("Corazón");
	m_SAnularD.SetMessage("Anular");
	m_SAnularI.SetMessage("Anular");
	m_SMeniqueD.SetMessage("Meñique");
	m_SMeniqueI.SetMessage("Meñique");
	m_SHuella.SetMessage("Buscar");

	m_SHuella.SetDefault();
	m_BAceptar.EnableWindow(!m_pUsuario->Pertenece(GRUPO_USUARIOS));

	OnTimer(2);
	SetTimer(2, RETRASO_MENSAJE, NULL);

	Clean(true);

	OnUpdateHorarios(1,1);
}

// Controladores de mensaje de CSVEView

void CSVEView::SetMatrizTexto(CString texto, int estilo, CString linea2) {

	if(!::IsWindow(m_pDlgPopUp))
		return;
	KillTimer(2);
	switch(estilo) {
		case 2: // rojo
			// m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(255, 60, 0), RGB(103, 30, 0));
			KillTimer(5);
			SetTimer(2, RETRASO_MENSAJE, NULL);
			break;
		case 1: // verde
			// m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(0, 255, 50), RGB(0, 103, 30));
			KillTimer(5);
			SetTimer(2, RETRASO_MENSAJE, NULL);
			break;
		default: // azul
			// m_SMatriz.SetDisplayColors(RGB(0, 0, 0), RGB(63, 181, 255), RGB(23, 64,  103));
			break;
	}
	if(texto.GetLength()<MATRIZ_ANCHO) {
		for( int i= texto.GetLength(); i<MATRIZ_ANCHO; i++)
			texto+=" ";
	}
	if(linea2.GetLength()<MATRIZ_ANCHO) {
		for( int i= linea2.GetLength(); i<MATRIZ_ANCHO; i++)
			linea2+=" ";
	}
	if(m_pDisplay) {
		m_pDisplay->SetMessage(texto, linea2);
	}
	if( estilo && !m_bActiva) 
		m_pDlgPopUp.Show(texto+linea2, estilo);
}

void CSVEView::OnBnClickedOk()
{
	CWaitCursor wait;

	CStringArray valores, campos;
	CString temp,archivo;
	CString codigo;
	CString documento;
	
	campos.Add("documento");
	campos.Add("nombre");
	campos.Add("apellido");
	campos.Add("grupo");
	campos.Add("observaciones");
	campos.Add("departamento");
	campos.Add("valorhora");

	m_EDocumento.GetWindowText(documento);
	if( documento.IsEmpty()) {
		SetErrorEx(&m_EDocumento, "Por favor ingrese un documento", 2, "ingreso");
		return;
	}
	// documento
	valores.Add(documento);
	m_ENombre.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Por favor ingrese un nombre", 2, "ingreso");
		return;
	}
	valores.Add(temp);
	m_EApellido.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_EApellido, "Por favor ingrese un apellido", 2, "ingreso");
		return;
	}
	valores.Add(temp);
	// grupo

	temp.Empty();
	int pos= m_CBHorario.GetCurSelEx();
	if(pos==-1) {
		SetErrorEx(&m_CBHorario, "Por favor seleccione un horario.", 2, "ingreso");
		return;
	}
	if( m_CBHorario.GetItemData(pos)==-1 && !m_pActual) {
		SetErrorEx(&m_BHorario, "Por favor edite el horario.", 2, "ingreso");
		return;
	}
	if(m_CBHorario.GetItemData(pos)!=-1) {
		if( m_pActual)
			delete m_pActual;
		m_pActual= (CUsuarioHorario *)m_CBHorario.GetItemData(pos);
		if(m_pActual)
			temp= m_pActual->GetID();
	}
	valores.Add(temp);

	// observaciones
	m_EObservaciones.GetWindowText(temp);
	valores.Add(temp);
	// departamento
	pos= m_CBDepartamento.GetCurSelEx();
	if(pos==-1) {
		if(m_pActual && m_pActual->GetID().IsEmpty()) {
			SAFE_DELETE(m_pActual);
		} else m_pActual=NULL;
		SetErrorEx(&m_CBDepartamento, "Por favor seleccione un departamento", 2, "ingreso");
		return;
	}
	CMiscelaneosEx *mis= (CMiscelaneosEx *)m_CBDepartamento.GetItemData(pos);
	temp.Empty();
	if(mis) {
		temp.Format("%d", mis->GetData());
	}
	valores.Add(temp);
	// valorhora
	valores.Add(m_EValorHora.GetNumero());


	int cantidad=0;
	if(m_SPulgarD.HasHuella())
		cantidad++;
	if(m_SPulgarI.HasHuella())
		cantidad++;
	if(m_SIndiceD.HasHuella())
		cantidad++;
	if(m_SIndiceI.HasHuella())
		cantidad++;
	if(m_SCorazonD.HasHuella())
		cantidad++;
	if(m_SCorazonI.HasHuella())
		cantidad++;
	if(m_SAnularD.HasHuella())
		cantidad++;
	if(m_SAnularI.HasHuella())
		cantidad++;
	if(m_SMeniqueD.HasHuella())
		cantidad++;
	if(m_SMeniqueI.HasHuella())
		cantidad++;
	if( cantidad<2) {
		if(m_pActual && m_pActual->GetID().IsEmpty()) {
			SAFE_DELETE(m_pActual);
		} else m_pActual=NULL;
		SetErrorEx(&m_SIndiceD, "Por favor coloque dos huellas", 2, "IngresoH");
		return;
	}
	// usuario
	m_pConector->Query(m_bExiste ? CQuery::CreateUpdate("usuario", campos, valores, "documento=" + documento) 
		: CQuery::CreateInsert("usuario", campos, valores), 0, this);
	// propiedades
	
	codigo= documento;
	valores.RemoveAll();
	campos.RemoveAll();


	if(!m_bExiste) {
		campos.Add("codigo");
		valores.Add(codigo);
	}
	campos.Add("usuario");
	campos.Add("fecha");
	
	CString usuario;
	usuario.Format("%d", m_pUsuario->GetID());
	valores.Add(usuario);
	valores.Add("SYSDATE()");

	m_pConector->Query(m_bExiste ? CQuery::CreateUpdate("propiedades", campos, valores, "codigo=" + codigo)
		: CQuery::CreateInsert("propiedades",campos, valores), 0, this);



	valores.RemoveAll();
	if(m_pActual) {
		m_pActual->GetQuerys(valores, documento);
		for( int i=0; i<valores.GetCount(); i++) {
			m_pConector->Query(valores.GetAt(i), 0, this);
		}
	}

	m_pConector->Query("DELETE FROM huella WHERE documento='" + documento+ "'", 0 , this);
	m_pConector->Query("DELETE FROM template WHERE documento='" + documento+ "'", 0 , this);


	InsertarHuella(documento, m_SPulgarD.GetHuella(), "0");
	InsertarHuella(documento, m_SPulgarI.GetHuella(), "1");
	InsertarHuella(documento, m_SIndiceD.GetHuella(), "2");
	InsertarHuella(documento, m_SIndiceI.GetHuella(), "3");
	InsertarHuella(documento, m_SCorazonD.GetHuella(), "4");
	InsertarHuella(documento, m_SCorazonI.GetHuella(), "5");
	InsertarHuella(documento, m_SAnularD.GetHuella(), "6");
	InsertarHuella(documento, m_SAnularI.GetHuella(), "7");
	InsertarHuella(documento, m_SMeniqueD.GetHuella(), "8");
	InsertarHuella(documento, m_SMeniqueI.GetHuella(), "9");

	m_szOld="";
	if(m_pActual && !m_pActual->GetID().IsEmpty()) 
		m_pActual=NULL;
	OnBnClickedButton3();
}

int CSVEView::InsertarHuella(CString documento, CFingerMessage *bitmap, CString dedo) {

	if(!bitmap->IsValid())
		return 0; 

	CStringArray valores;
	CString temp;

	valores.RemoveAll();
	valores.Add(documento);
	// dedo
	valores.Add(dedo);
	// ancho
	temp.Format("%d", bitmap->GetAncho());
	valores.Add(temp);
	// alto
	temp.Format("%d", bitmap->GetAlto());
	valores.Add(temp);
	// resolucion
	temp.Format("%d", bitmap->GetResolucion());
	valores.Add(temp);
	// huella
	valores.Add("%0%");
	CString app;
	BYTE *buffer= bitmap->GetRaw();
	for( int i=0; i< bitmap->GetSize(); i++) {
		switch( (buffer)[i]) {
			case 0:
				app+="\\0";
				break;
			case '\\':
				app+="\\\\";
				break;
			case '\"':
				app+="\\\"";
				break;
			default:
				app+= (buffer)[i];
				break;
		}
	}
	CString query= CQuery::CreateInsert("huella", valores);
	if(AfxGetApp()->GetProfileInt("control", "compress", 1))
		query.Replace("\"%0%\"", "COMPRESS(\"%0%\")");
	query.Replace("%0%", app);
	m_pConector->Query(query, 0 , this);

	// vectores de la huella
	valores.RemoveAll();
	valores.Add(documento);
	// dedo
	valores.Add(dedo);
	// huella
	valores.Add("%0%");
	app.Empty();
	buffer= bitmap->GetTemplate();
	for( int i=0; i<bitmap->GetTamanoTemplate(); i++) {
		switch( (buffer)[i]) {
			case 0:
				app+="\\0";
				break;
			case '\\':
				app+="\\\\";
				break;
			case '\"':
				app+="\\\"";
				break;
			default:
				app+= (buffer)[i];
				break;
		}
	}
	query= CQuery::CreateInsert("template", valores);
	query.Replace("%0%", app);
	m_pConector->Query(query, 0 , this);
	return 1;
}

void CSVEView::OnBnClickedButton3()
{
	m_EDocumento.SetWindowText("");
	Clean(true);
	m_szOld="";
	SetError(&m_EDocumento, "Inicializando Huellas");
}

void CSVEView::OnEnKillfocusEcedula()
{
	CString temp;
	m_EDocumento.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szOld)
		return;
	m_szOld=temp;
	temp.Format("SELECT nombre,apellido,grupo,observaciones FROM usuario WHERE documento='%s'", m_szOld);
	Clean();
	m_EDocumento.SetWindowText(m_szOld);
	m_pConector->Query(temp, query_usuario, this);
}

void CSVEView::Clean(bool search) {
	
	m_ENombre.SetWindowText("");
	m_EApellido.SetWindowText("");
	m_EObservaciones.SetWindowText("");

	SAFE_DELETE(m_pActual);

	m_bExiste=false;

	m_SPulgarD.Clean();
	m_SPulgarI.Clean();
	m_SIndiceD.Clean();
	m_SIndiceI.Clean();
	m_SCorazonD.Clean();
	m_SCorazonI.Clean();
	m_SAnularD.Clean();
	m_SAnularI.Clean();
	m_SMeniqueD.Clean();
	m_SMeniqueI.Clean();
	m_CBHorario.SetSelData(0);
	m_CBDepartamento.SetSelData(0);
	m_EValorHora.SetNumero(0);

	m_BHorario.SetWindowText("&Editar");
	m_BAceptar.EnableWindow(!m_pUsuario->Pertenece(GRUPO_USUARIOS));
	m_EApellido.SetReadOnly(false);
	m_ENombre.SetReadOnly(false);
	m_EObservaciones.SetReadOnly(false);
	m_CBHorario.EnableWindow();
	m_CBDepartamento.EnableWindow();
	m_EValorHora.SetReadOnly(false);
}

void CSVEView::OnDestroy()
{
	OnBnClickedButton3();

	if(m_pDisplay)
		m_pDisplay->SetMessageModal("Por favor", "Iniciar " + CApplicacion::GetAppName());

	SAFE_DELETE(m_pDisplay);

	CUsuarioHorario *hor;
	for( int i=0; i<m_CBHorario.GetCount(); i++) {
		if(m_CBHorario.GetItemData(i)==-1)
			continue;
		hor= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
		if(hor) 
			delete hor;
	}
	m_CBHorario.ResetContent();
	CFormView::OnDestroy();
}

CFingerBitmap *CSVEView::GetSeleccion() {

	CWnd *focus= GetFocus();
	if(focus->IsKindOf(RUNTIME_CLASS(CFingerBitmap)))
		return (CFingerBitmap *)focus;
	return &m_SHuella;
}

LRESULT CSVEView::OnFingerMessage(WPARAM wParam, LPARAM lParam) {

	if(wParam==GR_FINGER_UP) {
		CFingerMessage *mess=(CFingerMessage *)lParam;
		CWnd *focus=GetFocus();
		if( ( (focus && !focus->IsKindOf(RUNTIME_CLASS(CFingerBitmap))) || focus==&m_SHuella || !m_bActiva)  && (mess= m_SHuella.SetHuella(lParam))) {
			Clean();
			m_EDocumento.SetReadOnly(true);
			m_EDocumento.SetWindowText("");
			m_SMensaje.SetWindowText("Buscando");
			mess->Prepare();
			m_pTemporal= GetTickCount();
			m_pConector->QueryBinary("SELECT huella,documento,dedo FROM template", query_huella, this);
		} else {
			CFingerBitmap *bitmap= GetSeleccion();
			CFingerMessage *mess=NULL;
			if(bitmap && (mess=bitmap->SetHuella(lParam)))
				return PostMessage(WM_FINGERPRINT);
		}
	} else if(wParam==0) {

		CFingerBitmap *bitmap= GetSeleccion();
		if(bitmap) {
			if(ConfirmarDlg(bitmap->GetHuella())!=IDOK) {
				SetError(NULL, "Confirmación cancelada.");
				if(bitmap && ::IsWindow(*bitmap)) {
					bitmap->Clean();
				}
			} else {
				SetError(NULL, "Huella confirmada.");
			}
		}
	}
	return 0;
}

CFingerBitmap *CSVEView::GetDedo(int dedo) {

	switch(dedo) {
		case 0:
			return &m_SPulgarD;
		case 1:
			return &m_SPulgarI;
		case 2:
			return &m_SIndiceD;
		case 3:
			return &m_SIndiceI;
		case 4:
			return &m_SCorazonD;
		case 5:
			return &m_SCorazonI;
		case 6:
			return &m_SAnularD;
		case 7:
			return &m_SAnularI;
		case 8:
			return &m_SMeniqueD;
		case 9:
			return &m_SMeniqueI;
	}
	return NULL;
}

LRESULT CSVEView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;

	switch( wParam) {
		case THREAD_END:
			if(mess->GetID()==query_huella) {
				m_EDocumento.SetReadOnly(false);
				if(!m_bExiste) {
					CString resultado;
					resultado.Format("Huella no encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
					m_SMensaje.SetWindowText(resultado);
					SetMatrizTexto("Usuario No Encontrado." ,2);
				}
				m_EDocumento.SetPos(m_EDocumento.GetMaximo());
			} else if(mess->GetID()==query_horarios) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_CBHorario.GetCount(); i++ ) {
					if(m_CBHorario.GetItemData(i)==-1)
						continue;
					hor= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
					if(hor) {
						m_pConector->Query("SELECT turno,dia,entrada,salida FROM horariost WHERE turno=" + hor->GetID(), query_selectdatosturno, this);
					}
				}
			} else if(mess->GetID()==query_showhuellas) {
				m_EDocumento.SetPos(m_EDocumento.GetMaximo());
			}
			break;
		case QUERY_COUNT2:
			if(mess->GetID()==query_huella) {
				m_EDocumento.SetRange(0, mess->GetCount());
			}
			break;
		case QUERY_EMPTY:
			if( mess->GetID()==query_usuario) {
				m_bExiste=false;
				Clean(true);
			}
			break;
		case QUERY_TITLE:
			if(mess->GetID()==query_horarios) {

				CString texto= m_CBHorario.Text;
				CUsuarioHorario *hor;
				for( int i=0; i<m_CBHorario.GetCount(); i++) {
					if(m_CBHorario.GetItemData(i)==-1)
						continue;
					hor= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
					if(hor) 
						delete hor;
				}
				m_CBHorario.ResetContent();
				m_CBHorario.SetCurSel(m_CBHorario.AddString("(Ninguno)"));
				m_CBHorario.SetItemData(m_CBHorario.AddString("(Otro)"), -1);
				m_CBHorario.Text= texto;
			} else if(mess->GetID()==query_departamentos) {

				CString texto= m_CBDepartamento.Text;
				m_pDepartamentos.DeleteAll();
				m_CBDepartamento.ResetContent();
				m_CBDepartamento.SetCurSel(m_CBDepartamento.AddString("(Ninguno)"));
				m_CBDepartamento.Text=texto;
			} else if(mess->GetID()==query_usuario) {
				if(mess->GetCount())
					m_EDocumento.SetRange(0, 10);
			}
			break;
		case QUERY_RESULT:
			if( mess->GetID()==query_usuario) {
				
				CWaitCursor wait;
				m_ENombre.SetWindowText(mess->GetAt(0));
				m_EApellido.SetWindowText(mess->GetAt(1));
				bool bHorario=false;
				if(mess->GetAt(2).IsEmpty()) { // no tiene un grupo de horarios
					m_CBHorario.SetSelData(0);
				} else {
					CUsuarioHorario *horario;
					for( int i=0; i<m_CBHorario.GetCount(); i++) {
						if(m_CBHorario.GetItemData(i)==-1)
							continue;
						horario= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
						if(horario && horario->GetID()==mess->GetAt(2)) {
							m_CBHorario.SetCurSel(i);
							bHorario=true;
							if(m_pActual)
								delete m_pActual;
							m_pActual=NULL;
							break;
						}
					}
				}
				m_EObservaciones.SetWindowText(mess->GetAt(3));
				m_bExiste=true;
				
				AfxGetMainWnd()->SendMessage(WM_INSERTODS, 10, (LPARAM)(const char *)m_szOld);
				
				if(!m_pUsuario->Pertenece(GRUPO_ADMINISTRADOR)) {

					m_EApellido.SetReadOnly();
					m_ENombre.SetReadOnly();
					m_EObservaciones.SetReadOnly();
					m_CBHorario.EnableWindow(false);
					m_CBDepartamento.EnableWindow(false);
					m_EValorHora.SetReadOnly(true);
					m_BHorario.SetWindowText("&Ver");
					m_BAceptar.EnableWindow(false);
				}

				if(!bHorario) {
					if(m_pActual)
						delete m_pActual;
					m_pActual=new CUsuarioHorario();
					m_pActual->SetNombre(m_szOld);
					m_pConector->Query("SELECT usuario,dia,entrada,salida FROM horarios WHERE usuario='" + m_szOld + "'", query_horariousuario, this);
				}
				m_pConector->Query("SELECT ancho,alto,resolucion,dedo FROM huella WHERE documento=" + m_szOld, query_selectdatoshuella, this);
				if(AfxGetApp()->GetProfileInt("control", "compress", 1))
					m_pConector->QueryBinary("SELECT UNCOMPRESS(huella),dedo FROM huella WHERE documento=" + m_szOld, query_showhuellas, this);
				else m_pConector->QueryBinary("SELECT huella,dedo FROM huella WHERE documento=" + m_szOld, query_showhuellas, this);
				m_EDocumento.Step();
			} else if( mess->GetID()==query_selectdatoshuella) {
				CFingerBitmap *bitmap=GetDedo(atoi(mess->GetAt(3)));
				if(bitmap) {
					bitmap->SetParametros(atoi(mess->GetAt(1)),
						atoi(mess->GetAt(0)),
						atoi(mess->GetAt(2)));
				}
			} else if(mess->GetID()==query_horarios) {
				CUsuarioHorario *hor= new CUsuarioHorario();
				hor->Set(mess->GetAt(0), mess->GetAt(1));
				int pos= m_CBHorario.AddString(hor->GetNombre());
				m_CBHorario.SetItemData(pos, (DWORD) hor);
			} else if(mess->GetID()==query_selectdatosturno) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_CBHorario.GetCount(); i++) {
					if(m_CBHorario.GetItemData(i)==-1)
						continue;
					hor=(CUsuarioHorario *)m_CBHorario.GetItemData(i);
					if(hor && hor->GetID()==mess->GetAt(0)) {
						hor->AddTipo(atoi(mess->GetAt(1)), mess->GetAt(2), mess->GetAt(3));
						break;
					}
				}
			} else if(mess->GetID()==query_horariousuario) {
				if(m_pActual && m_pActual->GetNombre()==mess->GetAt(0)) {
					m_CBHorario.SetSelData(-1);
					m_pActual->AddTipo(atoi(mess->GetAt(1)), mess->GetAt(2), mess->GetAt(3));
				}
			} else if(mess->GetID()==query_departamentos) {
				CMiscelaneosEx *dep= m_pDepartamentos.Add(mess->GetAt(1), atol(mess->GetAt(0)), mess->GetAt(2));
				m_CBDepartamento.SetItemData(m_CBDepartamento.AddString(dep->GetNombre()), (DWORD) dep);
			} else if( mess->GetID()==query_showhuellas) {
				CFingerBitmap *bitmap=GetDedo(atoi(mess->GetAt(0)));
				ASSERT(bitmap);
				if(bitmap) 
					VERIFY(bitmap->SetHuella(mess->GetBinaryData()));
				m_EDocumento.Step();
			} else if(mess->GetID()==query_huella) {
				m_EDocumento.Step();
				CFingerMessage *huella= m_SHuella.GetHuella();
				if(huella) {
					int score=0;
					if(huella->Compare((const char *)mess->GetBinaryData()->GetData(), &score)) {
						CString resultado;
						resultado.Format("Huella encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
						if(m_pConector)
							m_pConector->KillQuery(query_huella, this);
						m_SMensaje.SetWindowText(resultado);
						CString texto= mess->GetAt(0);
						m_EDocumento.SetWindowText(texto);

						if(::IsWindow(m_pDlgPopUp)) {

							if(m_pConector && m_szLastSearch!=texto) {

								m_szLastSearch= texto;
								CString key;
								CStringArray valores;
								if(m_pConector->Find("SELECT id FROM sucesos WHERE usuario='" + mess->GetAt(0) +"' AND salida IS NULL", this, &valores)>0 && !valores.IsEmpty()) {
									key= valores.GetAt(0);
								}
								valores.RemoveAll();
								if( key.IsEmpty()) {

									valores.Add("");
									// usuario
									valores.Add(mess->GetAt(0));
									// entrada 
									valores.Add("SYSDATE()");
									// salida 
									valores.Add("");
									m_pConector->Query(CQuery::CreateInsert("sucesos", valores), 0, this);
									if(m_pConector->Find("SELECT CONCAT(nombre,' ',apellido),SYSDATE() FROM usuario WHERE documento=" + mess->GetAt(0), this, &valores) && !valores.IsEmpty()) {
										CMyTime tiempo(valores.GetAt(1));
										m_szLastUsuario= valores.GetAt(0);
										m_szLastHora= tiempo.GetHora();
										SetMatrizTexto("Bienvenido", 1, valores.GetAt(0));
										m_bSalida=false;
									}
								} else {
									// salida
									CStringArray campos;
									campos.Add("salida");
									valores.Add("SYSDATE()");
									m_pConector->Query(CQuery::CreateUpdate("sucesos", campos,valores, "id="+ key), 0, this);
									if(m_pConector->Find("SELECT CONCAT(nombre,' ',apellido),SYSDATE() FROM usuario WHERE documento=" + mess->GetAt(0), this, &valores) && !valores.IsEmpty()) {
										CMyTime tiempo(valores.GetAt(1));
										m_szLastUsuario= valores.GetAt(0);
										m_szLastHora= tiempo.GetHora();
										SetMatrizTexto("Hasta luego", 1, valores.GetAt(0));
										m_bSalida=true;
									}
								}
							} else {
								CString linea1, linea2;
								linea2= m_szLastUsuario;
								if( m_bSalida) {
									linea1= "Hasta luego";
								} else {
									linea1= "Bienvenido";
								}
								SetMatrizTexto(linea1, 1, linea2);
							}
							m_szLastSearch= texto;
						}
						KillTimer(3);
						SetTimer(3, CLEAN_OLD, NULL);
						OnEnKillfocusEcedula();
						m_bExiste=true;
					}
				}
			}
			break;
	}
	return 0;
}

LRESULT CSVEView::OnInsertODS(WPARAM wParam, LPARAM lParam) {
	
	CString texto;
	texto.Format("%d", lParam);
	m_szOld="";
	m_EDocumento.SetWindowText(texto);
	OnEnKillfocusEcedula();
	return 0;
}

void CSVEView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnBeginPrinting(pDC, pInfo);
}

void CSVEView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnPrint(pDC, pInfo);
}

BOOL CSVEView::OnPreparePrinting(CPrintInfo* pInfo)
{	
	return DoPreparePrinting(pInfo);
}

void CSVEView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
}

void CSVEView::OnUpdatePrint(CCmdUI *pCmdUI) {

	pCmdUI->Enable(false);
}

void CSVEView::OnBnClickedBhorario()
{
	CDlgHorario dlg;
	dlg.m_bEditable= m_pUsuario->Pertenece(GRUPO_ADMINISTRADOR) || !m_bExiste;

	int pos=m_CBHorario.GetCurSelEx();
	if(pos!=-1) {
		if(m_CBHorario.GetItemData(pos)!=-1 && m_CBHorario.GetItemData(pos)) {
			dlg.m_pActual= (CUsuarioHorario *)m_CBHorario.GetItemData(pos);
		} else dlg.m_pActual= m_pActual;
	}

	if(dlg.DoModal()==IDOK) {

		if(m_pActual) 
			delete m_pActual;

		m_CBHorario.SetSelData(-1);
		m_pActual= dlg.m_pActual;
		CUsuarioHorario *hor;
		for( int i=0; i<m_CBHorario.GetCount(); i++) {
			if(m_CBHorario.GetItemData(i)==-1)
				continue;
			hor= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
			if(hor) {
				if((*hor)==(*dlg.m_pActual)) {
					m_pActual= NULL;
					m_CBHorario.SetCurSel(i);
					delete dlg.m_pActual;
					break;
				}
			}
		}
	}
}

LRESULT CSVEView::OnUpdateCombo(WPARAM wParam, LPARAM lParam) {

	if(wParam==m_CBDepartamento.GetDlgCtrlID()) {
		if( lParam==-1) {
			m_EValorHora.SetNumero("");
		} else {
			CMiscelaneosEx *dep= (CMiscelaneosEx *)m_CBDepartamento.GetItemData(lParam);
			m_EValorHora.SetNumero(dep ? dep->GetExtra() : "");
		}
	}
	return 0;
}

LRESULT CSVEView::OnCloseEx(WPARAM wParam, LPARAM lParam) {

#ifndef _DEBUG
	if(m_pUsuario->Pertenece(GRUPO_USUARIOS))
		if(MessageBox("¿Esta seguro de salir de SDGFingerN?", NULL, MB_YESNOCANCEL)!=IDYES)
			return 1;
#endif
	return 0;
}

void CSVEView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==5 || nIDEvent==2) {
		if( nIDEvent==2) {
			KillTimer(2);
			SetTimer(5, 1000, NULL);
		}
		CMyTime tiempo= *((CTime *) AfxGetMainWnd()->SendMessage(WM_QUERYTIME));
		CString temp= tiempo.GetFechaHora();
		SetMatrizTexto("Listo.", 0, temp);
	} else if(nIDEvent==3) {
		KillTimer(3);
		m_szLastSearch.Empty();
	}
	CFormView::OnTimer(nIDEvent);
}

void CSVEView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	m_bActiva= bActivate;
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

LRESULT CSVEView::OnUpdateHorarios(WPARAM wParam, LPARAM lParam) {

	if(wParam==1)
		m_pConector->Query("SELECT id,nombre,szReservado FROM shared WHERE nReservado=6 AND (szReservado IS NULL OR szReservado!=-1)",query_departamentos, this);
	if(lParam==1)
		m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=5 AND (szReservado IS NULL OR szReservado!=-1)",query_horarios, this);
	return 0;
}
