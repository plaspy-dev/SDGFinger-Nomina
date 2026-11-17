// ParametrosView.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "ParametrosView.h"


// CParametrosView

IMPLEMENT_DYNCREATE(CParametrosView, CFormView)

CParametrosView::CParametrosView()
	: CFormView(CParametrosView::IDD)
{
	for( int i=0; i<4; i++)
		m_dwID[i]=0;
}

CParametrosView::~CParametrosView()
{
}

void CParametrosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DDESDE, m_DTDesde);
	DDX_Control(pDX, IDC_DHASTA, m_DTHasta);
	DDX_Control(pDX, IDC_EONOCTURNO, m_EONocturna);
	DDX_Control(pDX, IDC_EEDIURNA, m_EEDiurna);
	DDX_Control(pDX, IDC_EENOCTURNA, m_EENocturna);
	DDX_Control(pDX, IDC_EODOMINCAL, m_EODominical);
	DDX_Control(pDX, IDC_EEDDOMINICAL, m_EEDDominical);
	DDX_Control(pDX, IDC_EENDOMINICAL, m_EENDominical);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
}

BEGIN_MESSAGE_MAP(CParametrosView, CFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CParametrosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CParametrosView::OnBnClickedBcancelar)
	ON_MESSAGE(WM_MYSQL, OnMysql)
END_MESSAGE_MAP()


// CParametrosView diagnostics

#ifdef _DEBUG
void CParametrosView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CParametrosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CParametrosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Parámetros");

	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);
	
	m_DTDesde.SetFormat("hh:mm tt");
	m_DTHasta.SetFormat("hh:mm tt");

	OnBnClickedBcancelar();
}

void CParametrosView::OnBnClickedBaceptar()
{
	CStringArray valores, campos;
	CString temp;

	campos.Add("nReservado");
	campos.Add("nombre");
	campos.Add("descripcion");
	campos.Add("szReservado");

	valores.Add("7");
	valores.Add("");
	valores.Add("");
	valores.Add("1");

	COleDateTime tiempo;
	m_DTDesde.GetTime(tiempo);
	valores.SetAt(1,tiempo.Format("%H:%M"));
	m_DTHasta.GetTime(tiempo);
	valores.SetAt(2, tiempo.Format("%H:%M"));

	if(m_dwID[0]) {
		temp.Format("id=%d", m_dwID[0]);
		m_pConector->Query(CQuery::CreateUpdate("shared",campos, valores, temp), 0, this);
	} else m_pConector->Query(CQuery::CreateInsert("shared", campos, valores), 0 ,this);
	
	valores.SetAt(3 , "2");
	valores.SetAt(1, m_EONocturna.GetNumero());
	valores.SetAt(2, m_EODominical.GetNumero());

	if(m_dwID[1]) {
		temp.Format("id=%d", m_dwID[1]);
		m_pConector->Query(CQuery::CreateUpdate("shared",campos, valores, temp), 0, this);
	} else m_pConector->Query(CQuery::CreateInsert("shared", campos, valores), 0 ,this);


	valores.SetAt(3 , "3");
	valores.SetAt(1, m_EEDiurna.GetNumero());
	valores.SetAt(2, m_EENocturna.GetNumero());

	if(m_dwID[2]) {
		temp.Format("id=%d", m_dwID[2]);
		m_pConector->Query(CQuery::CreateUpdate("shared",campos, valores, temp), 0, this);
	} else m_pConector->Query(CQuery::CreateInsert("shared", campos, valores), 0 ,this);


	valores.SetAt(3 , "4");
	valores.SetAt(1, m_EEDDominical.GetNumero());
	valores.SetAt(2, m_EENDominical.GetNumero());

	if(m_dwID[3]) {
		temp.Format("id=%d", m_dwID[3]);
		m_pConector->Query(CQuery::CreateUpdate("shared",campos, valores, temp), 0, this);
	} else m_pConector->Query(CQuery::CreateInsert("shared", campos, valores), 0 ,this);

	((CMDIFrameWndXP *)AfxGetMainWnd())->SendMessageBroadCast(WM_UPDATEHORARIOS, 0, 3);
	OnBnClickedBcancelar();
	SetError(NULL, "Configuracion Guardada.");
}

void CParametrosView::OnBnClickedBcancelar()
{
	m_pConector->Query("SELECT id,nombre,descripcion FROM shared WHERE nReservado=7 AND szReservado=1", query_jornada, this);
	m_pConector->Query("SELECT id,nombre,descripcion FROM shared WHERE nReservado=7 AND szReservado=2", query_ordinaria, this);
	m_pConector->Query("SELECT id,nombre,descripcion FROM shared WHERE nReservado=7 AND szReservado=3", query_extra, this);
	m_pConector->Query("SELECT id,nombre,descripcion FROM shared WHERE nReservado=7 AND szReservado=4", query_extra_dominical, this);
}

LRESULT CParametrosView::OnMysql(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;

	switch(wParam) {

		case QUERY_EMPTY:
			if( mess->GetID()==query_jornada) {
				
				COleDateTime tiempo;
				tiempo.SetTime(6, 0, 0);
				m_DTDesde.SetTime(tiempo);

				tiempo.SetTime(22, 0, 0);
				m_DTHasta.SetTime(tiempo);
			} else if(mess->GetID()==query_ordinaria) {
				m_EONocturna.SetNumero(1.35);
				m_EODominical.SetNumero(1.75);
			} else if(mess->GetID()==query_extra) {
				m_EEDiurna.SetNumero(1.25);
				m_EENocturna.SetNumero(1.75);
			} else if(mess->GetID()==query_extra_dominical) {
				m_EEDDominical.SetNumero(2);
				m_EENDominical.SetNumero(2.5);
			}
			break;
		case QUERY_RESULT:
			if( mess->GetID()==query_jornada) {
				int horas= atoi(GetLeftString(mess->GetAt(1), ":"));
				int minutos= atoi(GetRightString(mess->GetAt(1), ":"));
				COleDateTime tiempo;
				tiempo.SetTime(horas, minutos, 0);
				m_DTDesde.SetTime(tiempo);

				horas= atoi(GetLeftString(mess->GetAt(2), ":"));
				minutos= atoi(GetRightString(mess->GetAt(2), ":"));
				tiempo.SetTime(horas, minutos, 0);
				m_DTHasta.SetTime(tiempo);

				m_dwID[0]=atoi(mess->GetAt(0));
			} else if(mess->GetID()==query_ordinaria) {
				m_EONocturna.SetNumero(atof(mess->GetAt(1)));
				m_EODominical.SetNumero(atof(mess->GetAt(2)));
				m_dwID[1]=atoi(mess->GetAt(0));
			} else if(mess->GetID()==query_extra) {
				m_EEDiurna.SetNumero(atof(mess->GetAt(1)));
				m_EENocturna.SetNumero(atof(mess->GetAt(2)));
				m_dwID[2]=atoi(mess->GetAt(0));
			} else if(mess->GetID()==query_extra_dominical) {
				m_EEDDominical.SetNumero(atof(mess->GetAt(1)));
				m_EENDominical.SetNumero(atof(mess->GetAt(2)));
				m_dwID[3]=atoi(mess->GetAt(0));
			}
			break;
	}
	return 0;
}
