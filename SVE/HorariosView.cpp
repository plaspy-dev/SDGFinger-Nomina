// HorariosView.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "HorariosView.h"

# include "DlgUsuarios.h"
// CHorariosView

IMPLEMENT_DYNCREATE(CHorariosView, CFormView)

CHorariosView::CHorariosView()
	: CFormView(CHorariosView::IDD)
{
	m_pActual= NULL;
}

CHorariosView::~CHorariosView()
{
}

void CHorariosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_RDIARIO, m_RDiario);
	DDX_Control(pDX, IDC_RDIARIOHOR, m_RDiarioHorario);
	DDX_Control(pDX, IDC_DDIARIOENTRADA, m_DTDiarioEntrada);
	DDX_Control(pDX, IDC_DDIARIOSALIDA, m_DTDiarioSalida);
	DDX_Control(pDX, IDC_DDIARIOTURNO, m_DTDiarioHoras);
	DDX_Control(pDX, IDC_CLUNES, m_CLunes);
	DDX_Control(pDX, IDC_CMARTES, m_CMartes);
	DDX_Control(pDX, IDC_CMIERCOLES, m_CMiercoles);
	DDX_Control(pDX, IDC_CJUEVES, m_CJueves);
	DDX_Control(pDX, IDC_CVIERNES, m_CViernes);
	DDX_Control(pDX, IDC_CSABADO, m_CSabado);
	DDX_Control(pDX, IDC_CDOMINGO, m_CDomingo);
	DDX_Control(pDX, IDC_RSEMANAL, m_RSemana);
	DDX_Control(pDX, IDC_DSEMANAHORA, m_DTSemanaHoras);
	DDX_Control(pDX, IDC_CALMUERZO, m_CAlmuerzo);
	DDX_Control(pDX, IDC_RALMUERZOHORARIO, m_RAlmuerzoHorario);
	DDX_Control(pDX, IDC_DALMUERZOSALIDA, m_DTAlmuerzoSalida);
	DDX_Control(pDX, IDC_DALMUERZOENTRADA, m_DTAlmuerzoEntrada);
	DDX_Control(pDX, IDC_DALMUERZOTURNO, m_DTAlmuerzoHoras);
	DDX_Control(pDX, IDC_CSABADOS, m_CSabados);
	DDX_Control(pDX, IDC_RSABADOSHORARIO, m_RSabadosHorario);
	DDX_Control(pDX, IDC_DSABADOSENTRADA, m_DTSabadosEntrada);
	DDX_Control(pDX, IDC_DSABADOSSALIDA, m_DTSabadosSalida);
	DDX_Control(pDX, IDC_DSABADOSTURNO, m_DTSabadosHoras);
	DDX_Control(pDX, IDC_CNOHABILES, m_CNoHabil);
	DDX_Control(pDX, IDC_RNOHABILESHORARIO, m_RNoHabilHorario);
	DDX_Control(pDX, IDC_DNOHABILESENTRADA, m_DTNoHabilEntrada);
	DDX_Control(pDX, IDC_DNOHABILESSALIDA, m_DTNoHabilSalida);
	DDX_Control(pDX, IDC_DNOHABILESTURNO, m_DTNoHabilHoras);
	DDX_Control(pDX, IDC_SFIN, m_SFin);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_LHORARIOS, m_LCHorarios);
}

BEGIN_MESSAGE_MAP(CHorariosView, CFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CHorariosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CHorariosView::OnBnClickedBcancelar)
	ON_BN_CLICKED(IDC_RDIARIOHOR, &CHorariosView::OnBnClickedRdiariohor)
	ON_BN_CLICKED(IDC_RDIARIOTURNO, &CHorariosView::OnBnClickedRdiariohor)
	ON_BN_CLICKED(IDC_RDIARIO, &CHorariosView::OnBnClickedRdiario)
	ON_BN_CLICKED(IDC_RSEMANAL, &CHorariosView::OnBnClickedRsemanal)
	ON_BN_CLICKED(IDC_RALMUERZOHORARIO, &CHorariosView::OnBnClickedRalmuerzohorario)
	ON_BN_CLICKED(IDC_RALMUERZOTURNO, &CHorariosView::OnBnClickedRalmuerzohorario)
	ON_BN_CLICKED(IDC_RSABADOSHORARIO, &CHorariosView::OnBnClickedRsabadoshorario)
	ON_BN_CLICKED(IDC_RSABADOSTURNO, &CHorariosView::OnBnClickedRsabadoshorario)
	ON_BN_CLICKED(IDC_RNOHABILESHORARIO, &CHorariosView::OnBnClickedRnohabileshorario)
	ON_BN_CLICKED(IDC_RNOHABILESTURNO, &CHorariosView::OnBnClickedRnohabileshorario)
	ON_BN_CLICKED(IDC_CALMUERZO, &CHorariosView::OnBnClickedCalmuerzo)
	ON_BN_CLICKED(IDC_CSABADOS, &CHorariosView::OnBnClickedCsabados)
	ON_BN_CLICKED(IDC_CNOHABILES, &CHorariosView::OnBnClickedCnohabiles)
	ON_EN_CHANGE(IDC_ENOMBRE, &CHorariosView::OnEnChangeEnombre)
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_NOTIFY(NM_DBLCLK, IDC_LHORARIOS, &CHorariosView::OnNMDblclkLhorarios)
	ON_NOTIFY(NM_RCLICK, IDC_LHORARIOS, &CHorariosView::OnNMRclickLhorarios)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LHORARIOS, &CHorariosView::OnLvnKeydownLhorarios)
	ON_COMMAND(2000, OnEditar)
	ON_COMMAND(2001, OnEliminar)
	ON_COMMAND(2002, OnVerUsuarios)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CHorariosView diagnostics

#ifdef _DEBUG
void CHorariosView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHorariosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHorariosView message handlers

void CHorariosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_pConector= (CConector	*)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);

	GetDocument()->SetTitle("Horarios");

	COleDateTime otro;

	CString formatofecha;
	formatofecha="hh:mm tt";
	m_DTDiarioEntrada.SetFormat(formatofecha);
	m_DTDiarioSalida.SetFormat(formatofecha);
	m_DTAlmuerzoEntrada.SetFormat(formatofecha);
	m_DTAlmuerzoSalida.SetFormat(formatofecha);
	m_DTSabadosEntrada.SetFormat(formatofecha);
	m_DTSabadosSalida.SetFormat(formatofecha);
	m_DTNoHabilEntrada.SetFormat(formatofecha);
	m_DTNoHabilSalida.SetFormat(formatofecha);

	formatofecha="mm:ss";
	
	m_DTDiarioHoras.SetFormat(formatofecha);
	m_DTAlmuerzoHoras.SetFormat(formatofecha);
	m_DTSabadosHoras.SetFormat(formatofecha);
	m_DTSemanaHoras.SetFormat(formatofecha);
	m_DTNoHabilHoras.SetFormat(formatofecha);

	m_CAlmuerzo.SetNextSectionCtrl(m_CSabados);
	m_CSabados.SetNextSectionCtrl(m_CNoHabil);
	m_CNoHabil.SetNextSectionCtrl(m_SFin);

	 m_RDiarioHorario.SetCheck(1);
	 m_RAlmuerzoHorario.SetCheck(1);
	 m_RSabadosHorario.SetCheck(1);
	 m_RNoHabilHorario.SetCheck(1);

	 m_LCHorarios.InsertColumn(0, "Horario", 0, 100);

	 m_LCHorarios.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	 m_LCHorarios.SetOrdenable();
	 m_LCHorarios.InsertItem(0, "(Ninguno)");

	OnBnClickedBcancelar();

	m_LCHorarios.SetMessageLista("Cargando ...");
	m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=5 AND (szReservado IS NULL OR szReservado!=-1)",1, this);
}

void CHorariosView::OnBnClickedBaceptar()
{
	CWaitCursor wait;
	CString nombre;
	m_ENombre.GetWindowText(nombre);

	int tipo=0;
	if( m_RDiario.GetCheck()) {
		if( m_CLunes.GetCheck())
			tipo|=1;
		if(m_CMartes.GetCheck())
			tipo|=2;
		if(m_CMiercoles.GetCheck())
			tipo|=4;
		if(m_CJueves.GetCheck())
			tipo|=8;
		if(m_CViernes.GetCheck())
			tipo|=16;
		if(m_CSabado.GetCheck())
			tipo|=32;
		if(m_CDomingo.GetCheck())
			tipo|=64;
		if( tipo==0) {
			SetErrorEx(&m_CLunes, "Por lo menos seleccione un dia",2,"horarios");
			return;
		}
	}

	if(!m_pActual) {
		m_pActual= new CUsuarioHorario();
	}

	m_pActual->CleanAll();
	m_pActual->SetNombre(nombre);

	CTime desde, hasta;

	if( m_RDiario.GetCheck()) {
		ASSERT(tipo);
		if( m_RDiarioHorario.GetCheck()) {
			m_DTDiarioEntrada.GetTime(desde);
			m_DTDiarioSalida.GetTime(hasta);
		} else {
			m_DTDiarioHoras.GetTime(desde);
		}
		m_pActual->AddTipo(tipo, &desde,m_RDiarioHorario.GetCheck() ? &hasta : NULL);
	} else if( m_RSemana.GetCheck()) {
		m_DTSemanaHoras.GetTime(desde);
		m_pActual->AddTipo(256, &desde);
	}
	if( m_CAlmuerzo.GetCheck()) {
		if( m_RAlmuerzoHorario.GetCheck()) {
			m_DTAlmuerzoEntrada.GetTime(hasta);
			m_DTAlmuerzoSalida.GetTime(desde);
		} else {
			m_DTAlmuerzoHoras.GetTime(desde);
		}
		m_pActual->AddTipo(128, &desde, m_RAlmuerzoHorario.GetCheck() ? &hasta :NULL);
	}
	if( m_CSabados.GetCheck()) {
		if( m_RSabadosHorario.GetCheck()) {
			m_DTSabadosEntrada.GetTime(desde);
			m_DTSabadosSalida.GetTime(hasta);
		} else {
			m_DTSabadosHoras.GetTime(desde);
		}
		m_pActual->AddTipo(32, &desde, m_RSabadosHorario.GetCheck() ? &hasta :NULL);
	}
	if( m_CNoHabil.GetCheck()) {
		if( m_RNoHabilHorario.GetCheck()) {
			m_DTNoHabilEntrada.GetTime(desde);
			m_DTNoHabilSalida.GetTime(hasta);
		} else {
			m_DTNoHabilHoras.GetTime(desde);
		}
		m_pActual->AddTipo(64, &desde, m_RNoHabilHorario.GetCheck() ? &hasta :NULL);
	}

	m_pConector->Query(m_pActual->GetQuery(), m_pActual->GetID().IsEmpty() ? 10 : 0, this);
	if( !m_pActual->GetID().IsEmpty()) {
		CStringArray querys;
		m_pActual->GetQuerys(querys);
		for( int i=0; i<querys.GetCount(); i++) {
			m_pConector->Query(querys.GetAt(i), 0, this);
		}
		CStringArray documentos;
		if(m_pConector->Find("SELECT documento FROM usuario WHERE grupo='" + m_pActual->GetID() + "'", this, &documentos)>0) {
			for( int j=0; j<documentos.GetCount(); j++) {
				m_pActual->GetQuerys(querys, documentos.GetAt(j));
				for( int i=0; i<querys.GetCount(); i++) {
					m_pConector->Query(querys.GetAt(i), 0, this);
				}
			}
		}

		CUsuarioHorario *hor;
		for( int i=0; i<m_LCHorarios.GetItemCount(); i++) {
			hor= (CUsuarioHorario *)m_LCHorarios.GetItemData(i);
			if( hor==m_pActual) {
				m_LCHorarios.SetItemText(i, 0, m_pActual->GetNombre());
				break;
			}
		}
	} else {
		int pos= m_LCHorarios.InsertItem(m_LCHorarios.GetItemCount(),m_pActual->GetNombre());
		m_LCHorarios.SetItemData(pos, (DWORD) m_pActual);
	}
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_UPDATEHORARIOS, 0, 1);
	OnBnClickedBcancelar();
}

void CHorariosView::OnBnClickedBcancelar()
{
	
	m_pActual= NULL;
	m_ENombre.SetWindowText("");

	CTime tiempo(2000,1,1,8,0,0);
	CTime tiempos(2000,1,1,18,0,0);

	m_DTDiarioEntrada.SetTime(&tiempo);
	m_DTDiarioSalida.SetTime(&tiempos);
	m_DTSabadosEntrada.SetTime(&tiempo);
	m_DTSabadosSalida.SetTime(&tiempos);
	m_DTNoHabilEntrada.SetTime(&tiempo);
	m_DTNoHabilSalida.SetTime(&tiempos);
	
	tiempo= CTime(2000,1,1,12,0,0);
	m_DTAlmuerzoSalida.SetTime(&tiempo);
	tiempo= CTime(2000,1,1,13,0,0);
	m_DTAlmuerzoEntrada.SetTime(&tiempo);

	tiempo= CTime(2000,1,1,0,1,0);
	m_DTDiarioHoras.SetTime(&tiempo);
	m_DTAlmuerzoHoras.SetTime(&tiempo);
	m_DTSabadosHoras.SetTime(&tiempo);
	m_DTNoHabilHoras.SetTime(&tiempo);
	tiempo= CTime(2000,1,1,0,48,0);
	m_DTSemanaHoras.SetTime(&tiempo);


	m_BAceptar.EnableWindow(false);

	m_CLunes.SetCheck(1);
	m_CMartes.SetCheck(1);
	m_CMiercoles.SetCheck(1);
	m_CJueves.SetCheck(1);
	m_CViernes.SetCheck(1);

	m_RDiario.SetCheck(1);
	m_CAlmuerzo.SetCheck(0);
	m_CSabados.SetCheck(0);
	m_CNoHabil.SetCheck(0);

	UpdateSemana(false);

	m_CAlmuerzo.UpdateControlledDlgItems();
	m_CSabados.UpdateControlledDlgItems();
	m_CNoHabil.UpdateControlledDlgItems();

	GetDocument()->SetTitle("Horarios");
	GetDocument()->SetTitle("Horarios");
	m_BAceptar.SetWindowText("Aceptar");
}

void CHorariosView::OnBnClickedRdiariohor()
{
	m_DTDiarioEntrada.EnableWindow(m_RDiarioHorario.GetCheck());
	m_DTDiarioSalida.EnableWindow(m_RDiarioHorario.GetCheck());
	m_DTDiarioHoras.EnableWindow(!m_RDiarioHorario.GetCheck());
}

void CHorariosView::OnBnClickedRdiario()
{
	UpdateSemana(false);
}

void CHorariosView::OnBnClickedRsemanal()
{
	UpdateSemana(true);
}

void CHorariosView::UpdateSemana(bool semana) {

	if(semana)
		m_RDiario.SetCheck(!m_RSemana.GetCheck());
	else m_RSemana.SetCheck(!m_RDiario.GetCheck());
	m_DTSemanaHoras.EnableWindow(m_RSemana.GetCheck());
	m_CLunes.EnableWindow(m_RDiario.GetCheck());
	m_CMartes.EnableWindow(m_RDiario.GetCheck());
	m_CMiercoles.EnableWindow(m_RDiario.GetCheck());
	m_CJueves.EnableWindow(m_RDiario.GetCheck());
	m_CViernes.EnableWindow(m_RDiario.GetCheck());
	m_CSabado.EnableWindow(m_RDiario.GetCheck());
	m_CDomingo.EnableWindow(m_RDiario.GetCheck());
	m_DTDiarioEntrada.EnableWindow(m_RDiario.GetCheck());
	m_DTDiarioSalida.EnableWindow(m_RDiario.GetCheck());
	m_DTDiarioHoras.EnableWindow(m_RDiario.GetCheck());
	m_RDiarioHorario.EnableWindow(m_RDiario.GetCheck());
	CWnd *handle= GetDlgItem(IDC_RDIARIOTURNO);
	if(handle)
		handle->EnableWindow(m_RDiario.GetCheck());
	if( m_RDiario.GetCheck())
		OnBnClickedRdiariohor();
}

void CHorariosView::OnBnClickedRalmuerzohorario()
{
	m_DTAlmuerzoEntrada.EnableWindow(m_RAlmuerzoHorario.GetCheck());
	m_DTAlmuerzoSalida.EnableWindow(m_RAlmuerzoHorario.GetCheck());
	m_DTAlmuerzoHoras.EnableWindow(!m_RAlmuerzoHorario.GetCheck());
}

void CHorariosView::OnBnClickedRsabadoshorario()
{
	m_DTSabadosEntrada.EnableWindow(m_RSabadosHorario.GetCheck());
	m_DTSabadosSalida.EnableWindow(m_RSabadosHorario.GetCheck());
	m_DTSabadosHoras.EnableWindow(!m_RSabadosHorario.GetCheck());
}

void CHorariosView::OnBnClickedRnohabileshorario()
{
	m_DTNoHabilEntrada.EnableWindow(m_RNoHabilHorario.GetCheck());
	m_DTNoHabilSalida.EnableWindow(m_RNoHabilHorario.GetCheck());
	m_DTNoHabilHoras.EnableWindow(!m_RNoHabilHorario.GetCheck());
}

void CHorariosView::OnBnClickedCalmuerzo()
{
	if( m_CAlmuerzo.GetCheck()) {
		OnBnClickedRalmuerzohorario();
	}
}

void CHorariosView::OnBnClickedCsabados()
{
	if( m_CSabados.GetCheck()) {
		OnBnClickedRsabadoshorario();
	}
}

void CHorariosView::OnBnClickedCnohabiles()
{
	if( m_CNoHabil.GetCheck()) {
		OnBnClickedRnohabileshorario();
	}
}

void CHorariosView::OnEnChangeEnombre()
{
	m_BAceptar.EnableWindow(m_ENombre.GetWindowTextLength());
}

LRESULT CHorariosView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {
	
	MySQLMessage *mess=(MySQLMessage *)lParam;

	switch(wParam) {
		case QUERY_RESULT:
			if( mess->GetID()==1) {
				CUsuarioHorario *hor= new CUsuarioHorario();
				hor->Set(mess->GetAt(0), mess->GetAt(1));
				int pos= m_LCHorarios.InsertItem( 0, hor->GetNombre());
				m_LCHorarios.SetItemData(pos,(DWORD) hor);
			} else if( mess->GetID()==2) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_LCHorarios.GetItemCount(); i++) {
					hor=(CUsuarioHorario *)m_LCHorarios.GetItemData(i);
					if(hor && hor->GetID()==mess->GetAt(0)) {
						hor->AddTipo(atoi(mess->GetAt(1)), mess->GetAt(2), mess->GetAt(3));
						break;
					}
				}
			} else if(mess->GetID()==10) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_LCHorarios.GetItemCount(); i++) {
					hor= (CUsuarioHorario *)m_LCHorarios.GetItemData(i);
					if( hor && hor->GetID().IsEmpty()) {
						hor->SetId(mess->GetAt(0));
						CStringArray querys;
						hor->GetQuerys(querys);
						for( i=0; i<querys.GetCount(); i++) {
							m_pConector->Query(querys.GetAt(i), 0, this);
						}
						break;
					}
				}
			}
			break;
		case THREAD_END:
			if( mess->GetID()==1) {
				CUsuarioHorario *hor;
				for( int i=0; i<m_LCHorarios.GetItemCount(); i++) {
					hor=(CUsuarioHorario *)m_LCHorarios.GetItemData(i);
					if(hor)
						m_pConector->Query("SELECT turno,dia,entrada,salida FROM horariost WHERE turno=" + hor->GetID(), 2, this);
				}
				m_LCHorarios.AutoSizeColumns();
				m_LCHorarios.SetMessageLista("No hay horarios");
			}
			break;
	}
	return 0;
}

void CHorariosView::OnNMDblclkLhorarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	OnEditar();
	*pResult = 0;
}

void CHorariosView::OnNMRclickLhorarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;
	menu.CreatePopupMenu();

	if(m_LCHorarios.GetItemData(pNMListView->iItem)) {

		menu.AppendMenu(MF_STRING, 2000, "&Editar");
		menu.AppendMenu(MF_STRING, 2001, "E&liminar");
		menu.AppendMenu(MF_SEPARATOR);
	}
	menu.AppendMenu(MF_STRING, 2002, "&Ver Usuarios");

	menu.SetDefaultItem(m_LCHorarios.GetItemData(pNMListView->iItem) ? 2000 : 2002);
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CHorariosView::OnLvnKeydownLhorarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if( pLVKeyDow->wVKey==VK_DELETE)
		OnEliminar();
	*pResult = 0;
}

void CHorariosView::OnEditar() {

	int index=m_LCHorarios.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	if(m_LCHorarios.GetItemData(index)==0) {
		OnVerUsuarios();
		return;
	}
	GetDocument()->SetTitle("Horarios*");
	GetDocument()->SetTitle("Horarios*");
	m_BAceptar.SetWindowText("&Modificar");
	m_pActual= (CUsuarioHorario *)m_LCHorarios.GetItemData(index);

	m_ENombre.SetWindowText(m_pActual->GetNombre());
	COleDateTime *desde=NULL, *hasta=NULL;
	m_CNoHabil.SetCheck(0);
	m_CAlmuerzo.SetCheck(0);
	m_CSabados.SetCheck(0);
	CButton *radio=NULL;

	for( int i=0; i<m_pActual->GetHorarios(); i++) {
		
		index=m_pActual->GetTipo(i);
		desde= m_pActual->GetDesde(i);
		hasta= m_pActual->GetHasta(i);

		switch(index)
		{
			case CHorarios::dia_todasemana:
				m_RSemana.SetCheck(1);
				m_RDiario.SetCheck(0);
				ASSERT(hasta==NULL);
				m_DTSemanaHoras.SetTime(*desde);
				UpdateSemana(true);
				break;
			case CHorarios::dia_almuerzo:
				m_CAlmuerzo.SetCheck(1);
				m_RAlmuerzoHorario.SetCheck(hasta!=NULL);
				radio=(CButton *) GetDlgItem(IDC_RALMUERZOTURNO);
				if(radio)
					radio->SetCheck(hasta==NULL);
				if( hasta) {
					m_DTAlmuerzoSalida.SetTime(*desde);
					m_DTAlmuerzoEntrada.SetTime(*hasta);
				} else {
					m_DTAlmuerzoHoras.SetTime(*desde);
				}
				break;
			case CHorarios::dia_festivo:
				m_CNoHabil.SetCheck(1);
				m_RNoHabilHorario.SetCheck(hasta!=NULL);
				radio=(CButton *) GetDlgItem(IDC_RNOHABILESTURNO);
				if(radio)
					radio->SetCheck(hasta==NULL);
				if (hasta)
				{
					m_DTNoHabilEntrada.SetTime(*desde);
					m_DTNoHabilSalida.SetTime(*hasta);
				} 
				else
				{
					m_DTNoHabilHoras.SetTime(*desde);
				}
				break;
			case CHorarios::dia_sabado:
				m_CSabados.SetCheck(1);
				m_RSabadosHorario.SetCheck(hasta!=NULL);
				radio=(CButton *) GetDlgItem(IDC_RSABADOSTURNO);
				if(radio)
					radio->SetCheck(hasta==NULL);
				if(hasta) {
					m_DTSabadosEntrada.SetTime(*desde);
					m_DTSabadosSalida.SetTime(*hasta);
				} else {
					m_DTSabadosHoras.SetTime(*desde);
				}
				break;
			default: // todos los dias
				m_RSemana.SetCheck(0);
				m_RDiario.SetCheck(1);
				m_RDiarioHorario.SetCheck(hasta!=NULL);
				radio=(CButton *) GetDlgItem(IDC_RDIARIOTURNO);
				if(radio)
					radio->SetCheck(hasta==NULL);
				if(hasta) {
					m_DTDiarioEntrada.SetTime(*desde);
					m_DTDiarioSalida.SetTime(*hasta);
				} else {
					m_DTDiarioHoras.SetTime(*desde);
				}
				m_CLunes.SetCheck(index&1);
				m_CMartes.SetCheck(index&2);
				m_CMiercoles.SetCheck(index&4);
				m_CJueves.SetCheck(index&8);
				m_CViernes.SetCheck(index&16);
				m_CSabado.SetCheck(index&32);
				m_CDomingo.SetCheck(index&64);
				UpdateSemana(false);
				break;
		}
	}
	m_CSabados.UpdateControlledDlgItems();
	m_CNoHabil.UpdateControlledDlgItems();
	m_CAlmuerzo.UpdateControlledDlgItems();
	OnBnClickedCnohabiles();
	OnBnClickedCsabados();
	OnBnClickedCalmuerzo();
}

void CHorariosView::OnEliminar() {
	
	int index=m_LCHorarios.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	if( m_LCHorarios.GetItemData(index)==0)
		return;
	index= m_LCHorarios.GetSelectedCount();
	CString temp;
	temp.Format("Esta seguro de eliminar %s elemento%s seleccionado%s", index>1 ? "los" : "el", index>1 ? "s" : "", index>1 ? "s" : "");
	if(MessageBox(temp, NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {
		
		CStringArray valores;
		CStringArray campos;
		campos.Add("szReservado");
		valores.Add("-1");

		while( (index= m_LCHorarios.GetNextItem(-1, LVIS_SELECTED))!=-1) {
			
			CUsuarioHorario *hor;
			hor= (CUsuarioHorario *)m_LCHorarios.GetItemData(index);
			if(hor) {
				m_pConector->Query("DELETE FROM horariost WHERE turno=" + hor->GetID(), 0, this);
				m_pConector->Query(CQuery::CreateUpdate("shared", campos, valores, "id=" + hor->GetID()), 0, this);
				if(m_pActual==hor) {
					m_pActual=NULL;
					m_BAceptar.SetWindowText("Aceptar");
					GetDocument()->SetTitle("Horarios");
					GetDocument()->SetTitle("Horarios");
				}
				delete hor;
				m_LCHorarios.DeleteItem(index);
			}
		}
	}
}

void CHorariosView::OnDestroy()
{
	CUsuarioHorario *hor;
	for( int i=0; i<m_LCHorarios.GetItemCount(); i++) {
		hor= (CUsuarioHorario *)m_LCHorarios.GetItemData(i);
		if(hor) {
			delete hor;
		}
	}
	CFormView::OnDestroy();
}


void CHorariosView::OnVerUsuarios() {

	CWaitCursor wait;
	int index= m_LCHorarios.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	CDlgUsuarios dlg;
	dlg.m_pConector= m_pConector;
	CUsuarioHorario *hor= (CUsuarioHorario *)m_LCHorarios.GetItemData(index);
	if(hor)
		dlg.m_szGrupo= hor->GetID();
	if(dlg.DoModal()==IDOK) {
		AfxGetMainWnd()->SendMessage(WM_INSERTODS, 0, (LPARAM) atol(dlg.m_szGrupo));
	}
}