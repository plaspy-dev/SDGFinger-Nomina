// DepartamentosView.cpp : implementation file
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "DepartamentosView.h"
# include "DlgUsuarios.h"


// CDepartamentosView

IMPLEMENT_DYNCREATE(CDepartamentosView, CConectorFormView)

CDepartamentosView::CDepartamentosView()
	: CConectorFormView(CDepartamentosView::IDD)
{
	m_dwCurrentID=0;
}

CDepartamentosView::~CDepartamentosView()
{
}

void CDepartamentosView::DoDataExchange(CDataExchange* pDX)
{
	CConectorFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LDEPARTAMENTOS, m_LCDepartamentos);
	DDX_Control(pDX, IDC_EDEPARTAMENTO, m_EDepartamento);
	DDX_Control(pDX, IDC_BACEPTAR, m_BAceptar);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_LUSUARIOS, m_LCUsuarios);
}

BEGIN_MESSAGE_MAP(CDepartamentosView, CConectorFormView)
	ON_BN_CLICKED(IDC_BACEPTAR, &CDepartamentosView::OnBnClickedBaceptar)
	ON_BN_CLICKED(IDC_BCANCELAR, &CDepartamentosView::OnBnClickedBcancelar)
	ON_EN_CHANGE(IDC_EDEPARTAMENTO, &CDepartamentosView::OnEnChangeEdepartamento)
	ON_NOTIFY(NM_DBLCLK, IDC_LDEPARTAMENTOS, &CDepartamentosView::OnNMDblclkLdepartamentos)
	ON_NOTIFY(NM_RCLICK, IDC_LDEPARTAMENTOS, &CDepartamentosView::OnNMRclickLdepartamentos)
	ON_MESSAGE(WM_ODBC, OnMySQL)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LDEPARTAMENTOS, &CDepartamentosView::OnLvnKeydownLdepartamentos)
	ON_COMMAND(2000,OnEditar)
	ON_COMMAND(2001,OnEliminar)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LUSUARIOS, &CDepartamentosView::OnNMDblclkLusuarios)
END_MESSAGE_MAP()


// CDepartamentosView diagnostics

#ifdef _DEBUG
void CDepartamentosView::AssertValid() const
{
	CConectorFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDepartamentosView::Dump(CDumpContext& dc) const
{
	CConectorFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDepartamentosView message handlers

void CDepartamentosView::OnInitialUpdate()
{
	AddAnchor(IDC_LDEPARTAMENTOS,TOP_LEFT,BOTTOM_LEFT);
	AddAnchor(IDC_SDEPARTAMENTO, TOP_LEFT);
	AddAnchor(IDC_EDEPARTAMENTO, TOP_LEFT);
	AddAnchor(IDC_LUSUARIOS, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BACEPTAR, BOTTOM_RIGHT);
	AddAnchor(IDC_BCANCELAR, BOTTOM_RIGHT);

	CConectorFormView::OnInitialUpdate();
	GetDocument()->SetTitle("Departamentos");
	// este es el nReservado
	m_pConector= (CConectorODBC *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);

	m_LCDepartamentos.AddColumn("Departamento");
	m_LCDepartamentos.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_LCDepartamentos.SetOrdenable(true);
	m_LCDepartamentos.AutoSizeColumns();

	m_pConector->Query("SELECT DeptId,DeptName FROM Dept",query_departamentos, this);

	m_LCUsuarios.InsertColumn(0, "Id", LVCFMT_RIGHT);
	m_LCUsuarios.InsertColumn(1, "Nombre");
	m_LCUsuarios.Mensaje="No hay usuarios";
	m_LCUsuarios.AutoSizeColumns();
	m_LCUsuarios.SetOrdenable();

	m_BAceptar.EnableWindow(m_pConector->CanUpdate());
}

LRESULT CDepartamentosView::OnMySQL(WPARAM wParam, LPARAM lParam) {

	ODBCMessage query(lParam);
	switch(wParam) {
		case QUERY_RESULT:
			if(query.GetID()==query_departamentos) {

				int pos= m_LCDepartamentos.AddItem(query["DeptName"]);
				m_LCDepartamentos.SetItemData(pos, atoi(query["DeptId"]));
			} else if(query.GetID()==query_insert) {
				for( int i=0; i<m_LCDepartamentos.GetItemCount(); i++) {
					if(m_LCDepartamentos.GetItemData(i)==-1) {
						m_LCDepartamentos.SetItemData(i, atol(query[0]));
						break;
					}
				}
			} else if(query.GetID()==query_usuarios) {

				int pos= m_LCUsuarios.AddItem(query["Userid"]);
				m_LCUsuarios.SetItemText(pos, 1, query["Name"]);
			}
			break;
		case THREAD_END:
			if(query.GetID()==query_departamentos) {
				m_LCDepartamentos.AutoSizeColumns();
			} else if(query.GetID()==query_usuarios) {
				m_LCUsuarios.AutoSizeColumns();
			}
			break;
	}
	return 0;
}

void CDepartamentosView::OnBnClickedBaceptar()
{
	CString temp;

	CQuery query("Dept");

	m_EDepartamento.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_EDepartamento, "Por favor ingrese un nombre", 2, "departamentos");
		return;
	}
	query["DeptName"]=m_EDepartamento.Text;

	int pos=0;

	if(m_dwCurrentID) {
		temp.Format("DeptId=%d", m_dwCurrentID);
		m_pConector->Query(query.GetUpdate(temp), 0, this);
		for( int i=0; i<m_LCDepartamentos.GetItemCount(); i++) {
			if( m_LCDepartamentos.GetItemData(i)==m_dwCurrentID) {
				pos=i;
				break;
			}
		}
	} else {
		m_pConector->Query(query.GetInsert());
		pos= m_LCDepartamentos.InsertItem(m_LCDepartamentos.GetItemCount(), "");
		m_LCDepartamentos.SetItemData(pos, -1);
		m_pConector->Query("SELECT @@Identity", query_insert,this);
	}
	m_LCDepartamentos.SetItemText(pos, 0, m_EDepartamento.Text);
	((CMDIFrameWndXP *) AfxGetMainWnd())->SendMessageBroadCast(WM_UPDATEHORARIOS, 1);

	OnBnClickedBcancelar();
}

void CDepartamentosView::OnBnClickedBcancelar()
{	
	m_dwCurrentID=0;
	m_LCUsuarios.DeleteAllItems();
	m_EDepartamento.SetWindowText("");
	GetDocument()->SetTitle("Departamentos");
	SetError("Listo");
}

void CDepartamentosView::OnEnChangeEdepartamento()
{
	m_BAceptar.EnableWindow(m_EDepartamento.GetWindowTextLength()>0 && m_pConector->CanUpdate());
}

void CDepartamentosView::OnNMDblclkLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	OnEditar();
	*pResult = 0;
}

void CDepartamentosView::OnNMRclickLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult)
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 

		return;
	CNewMenu menu;
	menu.CreatePopupMenu();

	if(m_LCDepartamentos.GetItemData(pNMListView->iItem)) {

		menu.AppendMenu(MF_STRING, 2000, "&Editar");
		menu.AppendMenu(MF_STRING, 2001, "E&liminar");
		menu.AppendMenu(MF_SEPARATOR);
	}

	menu.SetDefaultItem(m_LCDepartamentos.GetItemData(pNMListView->iItem) ? 2000 : 2002);

	POINT point;
	GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	*pResult = 0;
}

void CDepartamentosView::OnEditar() {

	int index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1)
		return;
	if(m_LCDepartamentos.GetItemData(index)==0) {
		OnVerUsuarios(index);
		return;
	}
	m_EDepartamento.SetWindowText(m_LCDepartamentos.GetItemText(index, 0));
	m_dwCurrentID= m_LCDepartamentos.GetItemData(index);
	GetDocument()->SetTitle("Departamento - " + m_LCDepartamentos.GetItemText(index, 0));
	OnVerUsuarios(index);
}

void CDepartamentosView::OnEliminar() {

	int index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED);
	if( index==-1 || m_LCDepartamentos.GetItemData(index)==0)
		return;

	index= m_LCDepartamentos.GetSelectedCount();
	CString temp;
	temp.Format("Esta seguro de eliminar %s elemento%s seleccionado%s", index>1 ? "los" : "el", index>1 ? "s" : "", index>1 ? "s" : "");
	if(MessageBox(temp, NULL, MB_ICONQUESTION | MB_YESNO)==IDYES) {

		CString temp;

		while((index= m_LCDepartamentos.GetNextItem(-1, LVIS_SELECTED))!=-1) {
			
			if(m_LCDepartamentos.GetItemData(index)==0)
				continue;
			temp.Format("DELETE FROM Dept WHERE Deptid=%d", m_LCDepartamentos.GetItemData(index));
			m_pConector->Query(temp,0, this);
			if(m_LCDepartamentos.GetItemData(index)==m_dwCurrentID) {
				m_dwCurrentID=0;
				GetDocument()->SetTitle("Departamentos");
			}
			m_LCDepartamentos.DeleteItem(index);
		}
	}
}

void CDepartamentosView::OnVerUsuarios(int pos) {
	
	m_LCUsuarios.DeleteAllItems();
	if( pos==-1)
		return;
	
	CString temp;
	CQuery query("Userinfo");

	query+="Userid";
	query+="Name";

	temp.Format("Deptid=%d ORDER BY Name", m_LCDepartamentos.GetItemData(pos));
	m_pConector->Query(query.GetSelect(temp), query_usuarios, this);
}

void CDepartamentosView::OnLvnKeydownLdepartamentos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if( pLVKeyDow->wVKey==VK_DELETE)
		OnEliminar();
	*pResult = 0;
}

void CDepartamentosView::OnSize(UINT nType, int cx, int cy)
{
	CConectorFormView::OnSize(nType, cx, cy);
}

void CDepartamentosView::OnNMDblclkLusuarios(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if(pNMItemActivate->iItem!=-1) {

		AfxGetMainWnd()->SendMessage(WM_TREENAVEGADOR,100, (LPARAM)(const char *)"CSVEView");
		((CMDIFrameWndXP *)AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTODS, NULL, (LPARAM)(const char *)m_LCUsuarios.GetItemText(pNMItemActivate->iItem, 0));
	}
	*pResult = 0;
}
