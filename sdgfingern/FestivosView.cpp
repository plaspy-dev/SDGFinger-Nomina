// FestivosView.cpp : implementation file
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "FestivosView.h"

// CFestivosView

IMPLEMENT_DYNCREATE(CFestivosView, CFormView)

CFestivosView::CFestivosView()
	: CFormView(CFestivosView::IDD)
{
	m_dwCurrentSel=0;
}

CFestivosView::~CFestivosView()
{
}

void CFestivosView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LLISTADO, m_LCResultados);
	DDX_Control(pDX, IDC_DFECHA, m_DTFecha);
	DDX_Control(pDX, IDC_EOBSERVACIONES, m_EObservaciones);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_SFECHA, m_SFecha);
	DDX_Control(pDX, IDC_SOBSERVACIONES, m_SObservaciones);
}

BEGIN_MESSAGE_MAP(CFestivosView, CFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CFestivosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CFestivosView::OnBnClickedBcancelar)
	ON_NOTIFY(NM_DBLCLK, IDC_LLISTADO, &CFestivosView::OnNMDblclkLlistado)
	ON_NOTIFY(NM_RCLICK, IDC_LLISTADO, &CFestivosView::OnNMRclickLlistado)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LLISTADO, &CFestivosView::OnLvnKeydownLlistado)
	ON_WM_SIZE()
	ON_MESSAGE(WM_ODBC, OnMysqlMessage)
	ON_COMMAND(2000, OnEditar)
	ON_COMMAND(2001, OnEliminar)
END_MESSAGE_MAP()


// CFestivosView diagnostics

#ifdef _DEBUG
void CFestivosView::AssertValid() const
{
	CFormView::AssertValid();
}
void CFestivosView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CFestivosView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pConector= (CConectorODBC *) AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);

	GetDocument()->SetTitle("Festivos");

	m_LCResultados.InsertColumn(0, "Día", 0, 100);
	m_LCResultados.InsertColumn(1, "Observaciones", 0, 100);
	m_LCResultados.SetOrdenable();
	m_LCResultados.SetOrdenable(CMyListCtrl::tipocol_hora);
	m_LCResultados.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCResultados.SetMessageLista("Cargando ...");

	m_BAceptar.SetWindowText("&Nuevo");

	CQuery query("Holiday");

	query+="Holidayid";
	query+="Name";
	query+="BDate";

	m_pConector->Query(query.GetSelect(), 1, this);

	m_BAceptar.EnableWindow(m_pConector->CanUpdate());
}

// CFestivosView message handlers
void CFestivosView::OnBnClickedBaceptar()
{
	CString temp;
	COleDateTime tiempo;
	
	CQuery query("Holiday");

	

	m_DTFecha.GetTime(tiempo);
	CMyTime mytiempo(tiempo);

	if( m_dwCurrentSel==0) { // modificacion
		for( int i=0; i<m_LCResultados.GetItemCount(); i++) {
			if( m_LCResultados.GetItemText(i, 0)==mytiempo.GetFecha(true)) {
				temp.Format("%d", i+1);
				SetErrorEx(&m_DTFecha, "Fecha existente en línea "  + temp, 2, "diashabiles");
				return;
			}
		}
	}

	m_EObservaciones.GetWindowText(temp);

	query["Name"]=temp;
	query["BDate"]=mytiempo.GetFormatoMysql();//.GetCustomFormat("yyyy/MM/dd");

	if( m_dwCurrentSel) { // modificacion
		for( int i=0; i<m_LCResultados.GetItemCount(); i++) {
			if( m_LCResultados.GetItemData(i)==m_dwCurrentSel) {
				m_LCResultados.SetItemText(i, 0, mytiempo.GetFecha());
				m_LCResultados.SetItemText(i, 1, temp);
				break;
			}
		}
		temp.Format("%d", m_dwCurrentSel);
		temp= query.GetUpdate("HolidayId=" + temp);
	} else {
		int pos= m_LCResultados.InsertItem(m_LCResultados.GetItemCount(), mytiempo.GetFecha());
		m_LCResultados.SetItemText(pos, 1, temp);
		temp= query.GetInsert();
	}
	temp.Replace("\"","'");
	m_pConector->Query(temp);
	if(!m_dwCurrentSel)
		m_pConector->Query("SELECT @@Identity", 2,this);
	OnBnClickedBcancelar();
}

void CFestivosView::OnBnClickedBcancelar()
{
	GetDocument()->SetTitle("Festivos");
	m_BAceptar.SetWindowText("&Nuevo");
	m_dwCurrentSel=0;
	CMyTime tiempo(true);
	m_DTFecha.SetTime(tiempo);
	m_EObservaciones.SetWindowText("");
}

void CFestivosView::OnEditar() {

	int index=m_LCResultados.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	GetDocument()->SetTitle("Festivos*");
	m_BAceptar.SetWindowText("&Modificar");
	COleDateTime tiempo;
	tiempo.ParseDateTime(m_LCResultados.GetItemText(index, 0));
	m_DTFecha.SetTime(tiempo);
	m_EObservaciones.SetWindowText(m_LCResultados.GetItemText(index,1));
	m_dwCurrentSel= m_LCResultados.GetItemData(index);
}

void CFestivosView::OnEliminar() {
	
	int index=m_LCResultados.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	index= m_LCResultados.GetSelectedCount();
	CString temp;
	temp.Format("Esta seguro de eliminar %s elemento%s seleccionado%s", index>1 ? "los" : "el", index>1 ? "s" : "", index>1 ? "s" : "");
	if(MessageBox(temp, NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {
		
		while( (index= m_LCResultados.GetNextItem(-1, LVIS_SELECTED))!=-1) {
			temp.Format("DELETE FROM Holiday WHERE Holidayid=%d", m_LCResultados.GetItemData(index));
			m_pConector->Query(temp, 0, this);
			m_LCResultados.DeleteItem(index);
		}
	}
}

void CFestivosView::OnNMDblclkLlistado(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	OnEditar();
	*pResult = 0;
}

void CFestivosView::OnNMRclickLlistado(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 2000, "&Editar");
	menu.AppendMenu(MF_STRING, 2001, "E&liminar");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}

void CFestivosView::OnLvnKeydownLlistado(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if( pLVKeyDow->wVKey==VK_DELETE) {
		OnEliminar();
	} else if( pLVKeyDow->wVKey==VK_RETURN) {
		OnEditar();
	}
	*pResult = 0;
}

void CFestivosView::OnSize(UINT nType, int cx, int cy)
{
	if(::IsWindow(m_LCResultados)) {
		CRect rect;
		m_BCancelar.GetWindowRect(rect);
		int alto= cy-rect.Height()-10;
		int ancho=cx-rect.Width()-10;
		m_BCancelar.MoveWindow(ancho, alto, rect.Width(), rect.Height());
		ancho-= 10+rect.Width();
		m_BAceptar.MoveWindow(ancho, alto, rect.Width(), rect.Height());
		alto-= rect.Height()+5;
		m_SObservaciones.GetWindowRect(rect);
		m_SObservaciones.MoveWindow(10, alto+5, rect.Width(), rect.Height());
		ancho= 20+ rect.Width();
		m_EObservaciones.GetWindowRect(rect);
		m_EObservaciones.MoveWindow(ancho, alto, rect.Width(), rect.Height());
		alto-=5+ rect.Height();
		m_SFecha.GetWindowRect(rect);
		m_SFecha.MoveWindow(10, alto+5, rect.Width(), rect.Height());
		m_DTFecha.GetWindowRect(rect);
		m_DTFecha.MoveWindow(ancho, alto, rect.Width(), rect.Height());
		alto-=rect.Height();
		m_LCResultados.MoveWindow(10,10, cx-20, alto+5);
	}
	CFormView::OnSize(nType, cx, cy);
}

LRESULT CFestivosView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {
	
	ODBCMessage query(lParam);

	switch( wParam) {
		case QUERY_RESULT:
			if( query.GetID()==1) {

				CMyTime tiempo(query["BDate"]);
				int pos= m_LCResultados.AddItem(tiempo.GetFecha());
				m_LCResultados.SetItemText(pos, 1, query["Name"]);
				m_LCResultados.SetItemData(pos, atoi(query["Holidayid"]));
			} else if( query.GetID()==2) {
				CString valor=query[0];
				for( int i=0; i<m_LCResultados.GetItemCount(); i++) {
					if( m_LCResultados.GetItemData(i)==0) {
						m_LCResultados.SetItemData(i, atoi(query[0]));
						break;
					}
				}
			}
			break;
		case THREAD_END:
			if( query.GetID()==1) {
				m_LCResultados.SetMessageLista("No hay días");
				m_LCResultados.AutoSizeColumns();
			}
			break;
	}
	return 0;
}
