// SVEView.cpp: implementación de la clase CSVEView
//

#include "stdafx.h"
#include "sdgfingern.h"

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

IMPLEMENT_DYNCREATE(CSVEView, CConectorFormView)

BEGIN_MESSAGE_MAP(CSVEView, CConectorFormView)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_ECEDULA, OnEnKillfocusEcedula)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_ODBC, OnMysqlMessage)
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
	ON_MESSAGE(WM_UPDATEHORARIOS, OnUpdateHorarios)
	ON_BN_CLICKED(IDC_BACEPTAR, &CSVEView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BOTRO, &CSVEView::OnBnClickedBotro)
	ON_EN_KILLFOCUS(IDC_EDOCUMENTO, &CSVEView::OnEnKillfocusEdocumento)
END_MESSAGE_MAP()

// Construcción o destrucción de CSVEView

CSVEView::CSVEView()
	: CConectorFormView(CSVEView::IDD)
{
	m_bExiste=false;
	m_pActual=NULL;
}

CSVEView::~CSVEView()
{
	if(m_pActual)
		delete m_pActual;
}

void CSVEView::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ECEDULA, m_EDocumento);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_BUTTON3, m_BCancelar);
	DDX_Control(pDX, IDC_CDEPARTAMENTO, m_CBDepartamento);
	DDX_Control(pDX, IDC_EVALOR, m_EValorHora);
	DDX_Control(pDX, IDC_CHORARIO, m_CBHorario);
	DDX_Control(pDX, IDC_BOTRO, m_BOtro);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_EDOCUMENTO, m_EDocumentoReal);
}

BOOL CSVEView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CConectorFormView::PreCreateWindow(cs);
}

void CSVEView::OnInitialUpdate()
{
	CWaitCursor wait;

	CConectorFormView::OnInitialUpdate();
	
	GetDocument()->SetTitle("Usuarios");
	
	m_pConector= (CConectorODBC *) AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *) AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);
	
	if(m_pConector->CanUpdate()) {

		CStringArray tablas;
		bool found= false;

		m_pConector->GetTables(tablas, this);
		for(int i=0; i<tablas.GetCount(); i++) {
			if(tablas[i]=="horario") {
				found= true;
				break;
			}
		}
		if(!found) {
			m_pConector->Query("CREATE TABLE horario (id AUTOINCREMENT PRIMARY KEY, nombre VARCHAR(255));");
			m_pConector->Query("CREATE TABLE horariost (turno INTEGER, dia INTEGER, entrada DATETIME, salida DATETIME );");
			m_pConector->Query("CREATE TABLE horarios (usuario VARCHAR(255), dia INTEGER, entrada DATETIME, salida DATETIME );");
		}

		CStringArray campos;
		found= false;
		m_pConector->GetCampos("Userinfo", campos, this);
		for(int i=0; i<campos.GetCount(); i++) {
			if(campos[i]=="horario") {
				found=true;
				break;
			}
		}
		if(!found) {
			m_pConector->Query("ALTER TABLE Userinfo ADD horario INTEGER;");
		}
		found=false;
		for(int i=0; i<campos.GetCount(); i++) {
			if(campos[i]=="documento") {
				found=true;
				break;
			}
		}
		if(!found) {
			m_pConector->Query("ALTER TABLE Userinfo ADD documento VARCHAR(255);");
		}
	}

	Clean(true);

	OnUpdateHorarios(1,1);
}

// Controladores de mensaje de CSVEView

void CSVEView::OnBnClickedButton3()
{
	m_EDocumento.Text="";
	m_EDocumentoReal.Text="";
	Clean(true);
	m_szOld="";
	m_szDocumento="";
	SetError("Listo", &m_EDocumento);
}

void CSVEView::OnEnKillfocusEcedula()
{
	CString temp;
	m_EDocumento.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szOld)
		return;
	m_szOld=temp;
	CQuery query("Userinfo");
	
	query+="Userid";
	query+="Name";
	query+="Deptid";
	query+="Salary";
	query+="horario";
	query+="documento";

	temp= query.GetSelect("Userid='" + m_szOld + "'");
	Clean();
	m_pConector->Query(temp, query_usuario, this);
}

void CSVEView::Clean(bool search) {
	
	m_ENombre.SetWindowText("");
	SAFE_DELETE(m_pActual);
	m_bExiste=false;

	m_CBDepartamento.Text="";
	m_CBHorario.Text="";
	m_EDocumentoReal.Text="";
	m_EValorHora.SetNumero(0);
}

void CSVEView::OnDestroy()
{
	OnBnClickedButton3();
	CConectorFormView::OnDestroy();
}

LRESULT CSVEView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	// ODBCMessage *mess=(ODBCMessage *)lParam;
	ODBCMessage query(lParam);

	switch( wParam) {
		case QUERY_END:
			if(query.GetID()==query_horarios) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_CBHorario.GetCount(); i++ ) {
					if(m_CBHorario.GetItemData(i)==-1)
						continue;
					hor= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
					if(hor) {
						m_pConector->Query("SELECT turno,dia,entrada,salida FROM horariost WHERE turno=" + hor->GetID(), query_selectdatosturno, this);
					}
				}
			}
			break;
		case QUERY_RESULT:
			if( query.GetID()==query_usuario) {

				m_bExiste=true;
				m_EDocumento.Text= m_szOld= query["Userid"];
				m_ENombre.Text= query["Name"];
				m_CBDepartamento.SelectedData= atoi(query["Deptid"]);
				m_EDocumentoReal.Text= m_szDocumento= query["documento"];
				m_EValorHora.SetNumero(atof(query["Salary"]));
				bool bHorario=false;

				if(query["horario"].IsEmpty()) { // no tiene un grupo de horarios
					m_CBHorario.SetSelData(0);
				} else {
					CUsuarioHorario *horario;
					for( int i=0; i<m_CBHorario.GetCount(); i++) {
						if(m_CBHorario.GetItemData(i)==-1) {
							if(query["horario"]=="-1") {
								m_CBHorario.SelectedIndex= i;
								SAFE_DELETE(m_pActual);
								break;
							}
							continue;
						}
						horario= (CUsuarioHorario *)m_CBHorario.GetItemData(i);
						if(horario && horario->GetID()==query["horario"]) {
							m_CBHorario.SetCurSel(i);
							bHorario=true;
							SAFE_DELETE(m_pActual);
							break;
						}
					}
				}

				if(!bHorario) {
					SAFE_DELETE(m_pActual);
					m_pActual=new CUsuarioHorario();
					m_pActual->SetNombre(m_szOld);
					m_pConector->Query("SELECT usuario,dia,entrada,salida FROM horarios WHERE usuario='" + m_szOld + "'", query_horariousuario, this);
				}

			} else if(query.GetID()==query_departamentos) {
				m_CBDepartamento.SetItemData(m_CBDepartamento.AddString(query["DeptName"]), atoi(query["DeptId"]));
			} else if(query.GetID()==query_horariousuario) {
				if(m_pActual && m_pActual->GetNombre()==query[0]) {
					m_pActual->AddTipo(atoi(query["dia"]), query["entrada"], query["salida"]);
				}
			} else if(query.GetID()==query_horarios) {
				CUsuarioHorario *hor= new CUsuarioHorario();
				hor->Set(query["id"], query["nombre"]);
				int pos= m_CBHorario.AddString(hor->GetNombre());
				m_CBHorario.SetItemData(pos, (DWORD) hor);
			} else if(query.GetID()==query_selectdatosturno) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_CBHorario.GetCount(); i++) {
					if(m_CBHorario.GetItemData(i)==-1)
						continue;
					hor=(CUsuarioHorario *)m_CBHorario.GetItemData(i);
					if(hor && hor->GetID()==query["turno"]) {
						hor->AddTipo(atoi(query["dia"]), query["entrada"], query["salida"]);
						break;
					}
				}
			} 
			break;
	}
	return 0;
}

LRESULT CSVEView::OnInsertODS(WPARAM wParam, LPARAM lParam) {
	
	CString texto= (const char *)lParam;
	m_szOld="";
	m_EDocumento.SetWindowText(texto);
	OnEnKillfocusEcedula();
	return 0;
}

LRESULT CSVEView::OnUpdateHorarios(WPARAM wParam, LPARAM lParam) {

	if(wParam==1) {
		
		CString texto= m_CBDepartamento.Text;
		m_pDepartamentos.DeleteAll();
		m_CBDepartamento.ResetContent();
		m_CBDepartamento.SetCurSel(m_CBDepartamento.AddString("(Ninguno)"));
		m_CBDepartamento.Text=texto;

		m_pConector->Query("SELECT DeptId,DeptName FROM Dept",query_departamentos, this);
	}
	if(lParam==1) {
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

		m_pConector->Query("SELECT id,nombre FROM horario",query_horarios, this);
	}
	return 0;
}

void CSVEView::OnBnClickedBaceptar()
{
	CQuery query("Userinfo");
	CString temp;

	if(m_bExiste) {
		if(m_EDocumento.Text.IsEmpty()) {
		
			SetError("Por favor ingrese un id", &m_EDocumento, 2);
			return;
		}
	}
	if(m_EDocumentoReal.Text.IsEmpty()) {

		SetError("Por favor ingrese un documento", &m_EDocumento, 2);
		return;
	}
	if(m_ENombre.Text.IsEmpty()) {
		SetError("Por favor ingrese un nombre", &m_ENombre, 2);
		return;
	}
	if(m_CBDepartamento.SelectedIndex==-1) {
		SetError("Por favor ingrese un departamento", &m_CBDepartamento, 2);
		return;
	}

	if(m_CBHorario.SelectedIndex!=-1 && m_CBHorario.SelectedData==-1 && !m_pActual) {
		SetError("Por favor edite el horario.", &m_BOtro, 2);
		return;
	}


	query["Name"]= m_ENombre.Text;
	temp.Format("%d", m_CBDepartamento.SelectedData);
	query["Deptid"]= temp;
	query["documento"]= m_EDocumentoReal.Text;
	if(m_EValorHora.Number.IsEmpty())
		query["Salary"]=m_EValorHora.Number;
	else query.AddCampo("Salary",m_EValorHora.Number, false);
	
	if(m_CBHorario.SelectedData!=-1) {
		if( m_pActual)
			delete m_pActual;
		m_pActual= (CUsuarioHorario *)m_CBHorario.SelectedData;
		if(m_pActual) {
			query["horario"]= m_pActual->GetID();
		} else {
			query["horario"]= "";
		}
	} else {
		query["horario"]="-1";
	}
	
	if(m_CBHorario.SelectedIndex==-1) {
		if(m_pActual && m_pActual->GetID().IsEmpty()) {
			SAFE_DELETE(m_pActual);
		} else m_pActual=NULL;
		SetError("Por favor seleccione un horario", &m_CBHorario, 2);
		return;
	}
	if(m_bExiste) {

		temp.Format("Userid='%s'", m_EDocumento.Text);
		m_pConector->Query(query.GetUpdate(temp));
	} else {
		CStringArray valores;
		// query["Userid"]= m_EDocumento.Text;
		m_pConector->Find(query.GetInsert());
		m_pConector->Find("SELECT @@Identity", this, &valores);
		m_EDocumento.Text= valores[0];
	}

	if(m_pActual) {
		CStringArray valores;
		m_pActual->GetQuerys(valores, m_EDocumento.Text);
		for( int i=0; i<valores.GetCount(); i++) {
			m_pConector->Query(valores.GetAt(i), 0, this);
		}
	}

	if(m_pActual && !m_pActual->GetID().IsEmpty()) 
		m_pActual=NULL;

	OnBnClickedButton3();
}

void CSVEView::OnBnClickedBotro()
{
	CDlgHorario dlg;
	dlg.m_bEditable= true;

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

void CSVEView::OnEnKillfocusEdocumento()
{
	if(m_bExiste)
		return;
	CString temp;
	m_EDocumentoReal.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szDocumento)
		return;
	m_szDocumento=temp;
	CQuery query("Userinfo");

	query+="Userid";
	query+="Name";
	query+="Deptid";
	query+="Salary";
	query+="horario";
	query+="documento";

	temp= query.GetSelect("documento='" + m_szDocumento + "'");
	Clean();
	m_pConector->Query(temp, query_usuario, this);
}
