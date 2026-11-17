// InformesView.cpp: archivo de implementación
//

#include "stdafx.h"
#include "sdgfingern.h"
#include "InformesView.h"
# include "GuardarDlg.h"
# include "UsuarioHorario.h"
# include "DlgConfigurar.h"
// CInformesView

IMPLEMENT_DYNCREATE(CInformesView, CFormView)

# define IDC_CTREELIST	1081

CInformesView::CInformesView()
	: CFormView(CInformesView::IDD)
{
	m_pTreePrint= NULL;
	m_pActual=NULL;
	m_nSegundos=0;
	m_dHoraNocturna=0;
	m_dHoraExtraNocturna=0;
	m_dHoraExtraDiurna=0;
	m_dHoraOrdinariaDominical=0;
	m_dHoraExtraDiurnaDominical=0;
	m_dHoraExtraNocturnaDominical=0;
	m_dHoraRecargoNocturno=0;
	m_dSalarioMinimo=0;
	m_dAuxilioTransporte=0;
	m_clrIluminado=::GetSysColor(COLOR_HIGHLIGHT);
	m_bCalcular=AfxGetApp()->GetProfileInt("informes", "calcular", 0);
	m_bDiferencia=AfxGetApp()->GetProfileInt("informes", "diferencia", 0);
}

CInformesView::~CInformesView()
{
	if(m_pTreePrint)
		delete m_pTreePrint;
}

void CInformesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_DDESDE, m_DTDesde);
	DDX_Control(pDX, IDC_DHASTA, m_DTHasta);
	DDX_Control(pDX, IDC_EDOCUMENTO, m_EDocumento);
	DDX_Control(pDX, IDC_BOPCIONES, m_BOpciones);
	DDX_Control(pDX, IDC_CDEPARTAMENTO, m_CBDepartamento);
}

BEGIN_MESSAGE_MAP(CInformesView, CFormView)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_ODBC, OnMysqlMessage)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdatePrint)
	ON_COMMAND(ID_FILE_SAVE, OnArchivoExportar)
	ON_COMMAND(ID_FILE_SAVE_AS, OnArchivoExportar)
	ON_COMMAND(2510, OnVerUsuario)
	ON_NOTIFY(NM_RCLICK, IDC_CTREELIST, OnRClickArbol)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BOPCIONES, &CInformesView::OnBnClickedBopciones)
	ON_MESSAGE(WM_UPDATEHORARIOS, OnUpdateHorarios)
	ON_COMMAND(ID_FILE_SAVE_AS+300, OnGuardarFijos)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS+300, OnUpdatePrint)
	// ON_COMMAND(ID_FILE_SAVE_AS+301, OnGuardarTemporales)
END_MESSAGE_MAP()


// Diagnósticos de CInformesView

#ifdef _DEBUG
void CInformesView::AssertValid() const
{
	CFormView::AssertValid();
}

void CInformesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CInformesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	
	GetDocument()->SetTitle("Informes");

	m_pConector= (CConectorODBC *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO);


	DWORD style= 0x50810000 | WS_VISIBLE;
	style&=~WS_BORDER;
	m_TLResultados.Create(style, CRect(0,0,0,0), this,IDC_CTREELIST);

	m_TLResultados.SetStyle
		( 0
		| TLC_TREELIST								// TreeList or List
		| TLC_MULTIPLESELECT						// single or multiple select
		| TLC_SHOWSELACTIVE							// show active column of selected item
		| TLC_SHOWSELALWAYS							// show selected item always
		| TLC_SHOWSELFULLROWS						// show selected item in fullrow mode

		| TLC_HEADER								// show header
		| TLC_HGRID									// show horizonal grid lines
		| TLC_VGRID									// show vertical grid lines
		| TLC_TGRID									// show tree horizonal grid lines ( when HGRID & VGRID )
		| TLC_HGRID_EXT								// show extention horizonal grid lines
		| TLC_VGRID_EXT								// show extention vertical grid lines
		| TLC_HGRID_FULL							// show full horizonal grid lines

		| TLC_TREELINE								// show tree line
		| TLC_ROOTLINE								// show root line
		| TLC_BUTTON								// show expand/collapse button [+]
		);


	m_nSegundos= AfxGetApp()->GetProfileInt("informes", "tolerancia", 15*60);

	m_TLResultados.InsertColumn("Ingreso", TLF_DEFAULT_LEFT, 100);
	m_TLResultados.InsertColumn("Salida", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Tiempo", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Entrada", TLF_DEFAULT_LEFT, 100);
	if(m_bDiferencia)
		m_TLResultados.InsertColumn("Diferencia", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Salida", TLF_DEFAULT_LEFT, 100);
	if(m_bDiferencia)
		m_TLResultados.InsertColumn("Diferencia", TLF_DEFAULT_RIGHT, 100);
	
	m_TLResultados.InsertColumn("Extras", TLF_DEFAULT_RIGHT, 100);
	if(m_bCalcular)
		m_TLResultados.InsertColumn("Total", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Diurnas ordinarias", TLF_DEFAULT_RIGHT, 100);
	if(m_bCalcular)
		m_TLResultados.InsertColumn("Total", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Nocturnas ordinarias", TLF_DEFAULT_RIGHT, 100);
	if(m_bCalcular)
		m_TLResultados.InsertColumn("Total", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Diurnas Festivos", TLF_DEFAULT_RIGHT, 100);
	if(m_bCalcular)
		m_TLResultados.InsertColumn("Total", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Nocturnas festivas", TLF_DEFAULT_RIGHT, 100);
	if(m_bCalcular)
		m_TLResultados.InsertColumn("Total", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Recargo Nocturno", TLF_DEFAULT_RIGHT, 100);
	if(m_bCalcular)
		m_TLResultados.InsertColumn("Total", TLF_DEFAULT_RIGHT, 100);
	m_TLResultados.InsertColumn("Observaciones", TLF_DEFAULT_LEFT, 100);

	m_TLResultados.AutosizeColumns();
	m_TLResultados.SetCanShowMenu();
	m_TLResultados.SetCanSort(false);

	CMyTime tiempo(true);
	tiempo+=COleDateTimeSpan(-30);
	m_DTDesde.SetTime(tiempo);
	
#ifdef _DEBUG
	m_EDocumento.Text="80723091";
#endif 
	OnUpdateHorarios(1,3);
	OnBnClickedOk();
}

int	CInformesView::GetColumnaReal(int columna) {

	int resultado= columna;
	if(!m_bDiferencia) {
		ASSERT(columna!=columna_entradadiferencia);
		ASSERT(columna!=columna_salidadiferencia);
		if(columna>columna_entradadiferencia) 
			resultado--;
		if(columna>columna_salidadiferencia) 
			resultado--;
	}
	if(!m_bCalcular) {
		
		ASSERT(columna!=columna_extrastotal);
		ASSERT(columna!=columna_diurnastotal);
		ASSERT(columna!=columna_nocturnastotal);
		ASSERT(columna!=columna_diurnasfestivostotal);
		ASSERT(columna!=columna_nocturnasfestivostotal);
		ASSERT(columna!=columna_recargonocturnototal);

		if(columna>columna_extrastotal) 
			resultado--;
		if(columna>columna_diurnastotal) 
			resultado--;
		if(columna>columna_nocturnastotal) 
			resultado--;
		if(columna>columna_diurnasfestivostotal) 
			resultado--;
		if(columna>columna_nocturnasfestivostotal) 
			resultado--;
		if(columna>columna_recargonocturnototal) 
			resultado--;
	}
	return resultado;
}

void CInformesView::OnBnClickedOk()
{
	if( !m_pConector)
		return;

	if(m_CBDepartamento.GetCount()==0) {
		m_CBDepartamento.SetCurSel(m_CBDepartamento.AddString("(Todos)"));
		m_CBDepartamento.SetItemData(m_CBDepartamento.AddString("(Ninguno)"), -1);
	}

	int pos= m_CBDepartamento.SelectedIndex;
	if(pos==-1) {
		SetErrorEx(&m_CBDepartamento, "Por favor seleccione un departamento.", 2, "informes");
		return;
	}

	m_pConector->KillQuery(query_consultas, this);
	m_pConector->KillQuery(query_horarios, this);
	m_pConector->KillQuery(query_festivos, this);
	
	HTREELISTITEM item= m_TLResultados.GetRootItem();
	HTREELISTITEM subitem, horas;
	CUsuarioHorario *hor;
	CTreeDate *date;

	while( item) { // usuarios
		hor= (CUsuarioHorario *) m_TLResultados.GetItemData(item);
		if( hor) 
			delete hor;
		subitem= m_TLResultados.GetChildItem(item);
		while(subitem) { // dias
			horas= m_TLResultados.GetChildItem(subitem);
			while(horas) {
				date= (CTreeDate *)m_TLResultados.GetItemData(horas);
				if(date)
					delete date;
				horas=m_TLResultados.GetNextSiblingItem(horas);
			}
			subitem= m_TLResultados.GetNextSiblingItem(subitem);
		}
		item= m_TLResultados.GetNextSiblingItem(item);
	}

	m_TLResultados.SetMessageLista("Realizando consulta");
	m_TLResultados.DeleteAllItems();

	CString desde, hasta;
	CString departamento;
	COleDateTime tiempo;

	if(m_CBDepartamento.GetItemData(pos)==-1) {
		departamento=" AND Userinfo.Deptid IS NULL ";
	} else if(m_CBDepartamento.GetItemData(pos)) {
		departamento.Format(" AND Userinfo.Deptid=%d ", m_CBDepartamento.GetItemData(pos));
	}
	
	m_DTDesde.GetTime(tiempo);
	desde= CMyTime(tiempo).GetFormatoMysql(); // .GetCustomFormat("yyyy/MM/dd");
	m_DTHasta.GetTime(tiempo);
	hasta= CMyTime(tiempo).GetFormatoMysql(); //.GetCustomFormat("yyyy/MM/dd");
	CString usuario;
	m_EDocumento.GetWindowText(usuario);

	m_pUsuarioActual.Clean();

	CQuery query("Checkinout");

	query["usuario"]="Checkinout.Userid";
	query["documento"]="Userinfo.documento";
	query["nombre"]="Userinfo.Name";
	query["valor"]="Userinfo.Salary";
	query["entrada"]="Checkinout.Checktime";
	query["tipo"]="Checkinout.CheckType";
	query["departamento"]="Userinfo.Deptid";

	CString join="LEFT JOIN Userinfo ON (CLng(Userinfo.Userid)=Checkinout.Userid)";

	CString condicion;
	condicion.Format("Checkinout.Checktime>=#%s 00:00:01# AND Checkinout.Checktime<=#%s 23:59:59#", desde, hasta);

	if( !usuario.IsEmpty())
		condicion+=" AND Userinfo.documento='" + usuario + "'";
	condicion+= departamento;
	condicion+=" ORDER BY Userinfo.Name,Checkinout.Checktime";

	m_szOldUsuario.Empty();

	m_pConector->Query(query.GetSelect(condicion, join),query_consultas, this);
	// buscamos los dias no habiles
	CQuery festivos("Holiday");

	festivos+="BDate";

	condicion.Format("BDate>=#%s# AND BDate<=#%s#", desde, hasta);
	m_saFestivos.RemoveAll();
	m_pConector->Query(festivos.GetSelect(condicion), query_festivos, this);
}

// nuevas funciones
bool CInformesView::EsFestivo(CMyTime &fecha) {

	if(fecha.GetDayOfWeek()==1) // es un domingo
		return true;
	CString cadena= fecha.GetFormatoMysql();
	for(int i=0; i<m_saFestivos.GetCount(); i++) {
		if(cadena==m_saFestivos[i])
			return true;
	}
	return false;
}

HTREELISTITEM CInformesView::FindUsuario(CString usuario) {

	HTREELISTITEM item= m_TLResultados.GetRootItem();
	while(item) {
		if(usuario.Compare(m_TLResultados.GetItemText(item,GetColumnaReal(columna_ingresotexto)))==0) 
			break;
		item=m_TLResultados.GetNextSiblingItem(item);
	}
	return item;
}

void CInformesView::VerificarAlmuerzo(HTREELISTITEM pDia, COleDateTime *desde, COleDateTime *hasta) {

	HTREELISTITEM item= pDia ? m_TLResultados.GetChildItem(pDia) : NULL;
	if(!item)
		return;
	if(m_TLResultados.GetChildCount(pDia)<3) // no se puede obtener informacion
		return;

	CTreeDate *tiempo= (CTreeDate *)m_TLResultados.GetItemData(item);
	if(!tiempo)
		return;
	if(7==tiempo->m_pEntrada.GetDayOfWeek()) // es un sabado.
		return;
	if(EsFestivo(tiempo->m_pEntrada)) 
		return;
	if(!desde)
		return;

	COleDateTime entrada;
	COleDateTime salida;
	COleDateTimeSpan esperado(0, desde->GetMinute(), desde->GetSecond(), 0);

	while(item) {
		tiempo= (CTreeDate *)m_TLResultados.GetItemData(item);
		if(salida.m_dt!=0) { // ya se marco la salida
			entrada= tiempo->m_pEntrada;
			m_TLResultados.SetItemText(item,tiempo->m_pEntrada.GetHora(), GetColumnaReal(columna_entrada));
			m_TLResultados.SetItemText(m_TLResultados.GetPrevSiblingItem(item),CMyTime(salida).GetHora(), GetColumnaReal(columna_salida));
			if(hasta) { // horario asignado.
				if(m_bDiferencia) {
					m_TLResultados.SetItemText(item, CTreeDate::GetFormatSpan(CTreeDate::NivelarFecha(entrada, *hasta)-entrada), GetColumnaReal(columna_entradadiferencia));
					m_TLResultados.SetItemText(m_TLResultados.GetPrevSiblingItem(item),CTreeDate::GetFormatSpan(salida-CTreeDate::NivelarFecha(salida, *desde)), GetColumnaReal(columna_salidadiferencia));
				}
				SetObservaciones(item,ParseDiferencia(item, CTreeDate::NivelarFecha(entrada, *hasta)-entrada,true), "Entrada de almuerzo temprano", "Entrada de almuerzo tarde", true);
				SetObservaciones(m_TLResultados.GetPrevSiblingItem(item), ParseDiferencia(m_TLResultados.GetPrevSiblingItem(item), salida-CTreeDate::NivelarFecha(salida, *desde),true), "Salida a almuerzo tarde", "Salida a almuerzo temprano", true);
			} else {// aqui va calculado si es por duracion.
				if(m_bDiferencia)
					m_TLResultados.SetItemText(item,CTreeDate::GetFormatSpan(entrada-salida), GetColumnaReal(columna_entradadiferencia));
				SetObservaciones(item,ParseDiferencia(item, entrada-salida,true), "Duración de almuerzo menor", "Duración de almuerzo mayor", true);
			}
		}
		salida= tiempo->m_pSalida;
		item= m_TLResultados.GetNextSiblingItem(item);
	}
}

double CInformesView::SetHorasExtras(HTREELISTITEM item, const COleDateTimeSpan &span, int columna, double valorminutos, double multiplicador, double &valorpadre) {

	int totalminutos=GetTotalHorasExtras(span);
	double valortemp= 0;
	if(totalminutos) {
		
		m_TLResultados.SetItemText(item,CTreeDate::GetFormatSpan(span+CTreeDate::GetSpan(m_TLResultados.GetItemText(item,columna))), columna);
		COleDateTimeSpan padre=span+CTreeDate::GetSpan(m_TLResultados.GetItemText(m_TLResultados.GetParentItem(item),columna));
		m_TLResultados.SetItemText(m_TLResultados.GetParentItem(item),CTreeDate::GetFormatSpan(padre), columna);
		if(valorminutos>0) {

			// primero el padre.
			valortemp= GetTotalHorasExtras(padre)*valorminutos*multiplicador;
			valorpadre+=valortemp;
			if(m_bCalcular)
				m_TLResultados.SetItemText(m_TLResultados.GetParentItem(item),CEditFloat::GetFormatNumber(valortemp), GetColumnaReal(columna+1));
			// luego el hijo.
			valortemp= totalminutos*valorminutos*multiplicador;
			if(m_bCalcular)
				m_TLResultados.SetItemText(item,CEditFloat::GetFormatNumber(valortemp), GetColumnaReal(columna+1));
		} else if(valorminutos==-1 && multiplicador>0) {
			if(m_bCalcular)
				m_TLResultados.SetItemText(item,CEditFloat::GetFormatNumber(multiplicador), GetColumnaReal(columna+1));
			if(valorpadre>0 && m_bCalcular)
				m_TLResultados.SetItemText(m_TLResultados.GetParentItem(item),CEditFloat::GetFormatNumber(valorpadre),GetColumnaReal(columna+1));
		}
	}
	return valortemp;
}

int CInformesView::GetTotalRecargoNocturno(HTREELISTITEM pDia, COleDateTime desde, COleDateTime hasta,COleDateTime entrada, COleDateTime salida) {

	COleDateTimeSpan nocturnas= COleDateTimeSpan(0,0,0,0);

	COleDateTimeSpan span(0,0,0,m_nSegundos);
	COleDateTimeSpan tiempo;
	CMyTime mytiempo;
	
	// validamos en que rango de horas esta, diurnas o nocturnas.
	// aqui toca validar si es festivo.

	while(desde<hasta) {

		if((hasta-desde)<span)
			tiempo= hasta-desde;
		else tiempo= span;
		mytiempo= desde;

		if(desde>CTreeDate::NivelarFecha(desde, m_tEntrada) && desde<CTreeDate::NivelarFecha(desde, m_tSalida)) { // diurna
			/*if(EsFestivo(mytiempo))
				diurnasf+= tiempo;
			else diurnas+= tiempo;*/
		} else { // nocturna.
			if(desde>entrada && desde<salida)
				nocturnas+=tiempo;
		}
		desde+=span;
	}
	// al item actual
	ASSERT(m_TLResultados.GetParentItem(pDia));
	double valorminuto= ((CUsuarioHorario *)m_TLResultados.GetItemData(m_TLResultados.GetParentItem(pDia)))->GetValorHora()/60; // lo partimos en minutos
	double valortotal= 0;
	double valorpadre=0;

	valortotal+=SetHorasExtras(pDia, nocturnas, GetColumnaReal(columna_recargonocturno), valorminuto,m_dHoraRecargoNocturno, valorpadre);
	return 0;
}

int CInformesView::GetTotalHorasExtras(HTREELISTITEM pDia, COleDateTime desde, COleDateTime hasta) {

	COleDateTimeSpan nocturnas= COleDateTimeSpan(0,0,0,0);
	COleDateTimeSpan diurnas= COleDateTimeSpan(0,0,0,0);
	COleDateTimeSpan nocturnasf= COleDateTimeSpan(0,0,0,0);
	COleDateTimeSpan diurnasf= COleDateTimeSpan(0,0,0,0);

	COleDateTimeSpan span(0,0,0,m_nSegundos);
	COleDateTimeSpan tiempo;
	CMyTime mytiempo;
	
	// validamos en que rango de horas esta, diurnas o nocturnas.
	// aqui toca validar si es festivo.

	while(desde<hasta) {

		if((hasta-desde)<span)
			tiempo= hasta-desde;
		else tiempo= span;
		mytiempo= desde;

		if(desde>CTreeDate::NivelarFecha(desde, m_tEntrada) && desde<CTreeDate::NivelarFecha(desde, m_tSalida)) { // diurna
			// verificamos que se festivo
			if(EsFestivo(mytiempo))
				diurnasf+= tiempo;
			else diurnas+= tiempo;
		} else { // nocturna.
			if(EsFestivo(mytiempo))
				nocturnasf+=tiempo;
			else nocturnas+=tiempo;
		}
		desde+=span;
	}
	// al item actual
	ASSERT(m_TLResultados.GetParentItem(pDia));
	double valorminuto= ((CUsuarioHorario *)m_TLResultados.GetItemData(m_TLResultados.GetParentItem(pDia)))->GetValorHora()/60; // lo partimos en minutos
	double valortotal= 0;
	double valorpadre=0;

	valortotal+=SetHorasExtras(pDia, diurnas, GetColumnaReal(columna_diurnas), valorminuto,m_dHoraExtraDiurna, valorpadre);
	valortotal+=SetHorasExtras(pDia, nocturnas, GetColumnaReal(columna_nocturnas), valorminuto,m_dHoraExtraNocturna, valorpadre);
	valortotal+=SetHorasExtras(pDia, diurnasf, GetColumnaReal(columna_diurnasfestivos), valorminuto,m_dHoraExtraDiurnaDominical, valorpadre);
	valortotal+=SetHorasExtras(pDia, nocturnasf, GetColumnaReal(columna_nocturnasfestivos), valorminuto,m_dHoraExtraNocturnaDominical, valorpadre);
	// este es el total.
	SetHorasExtras(pDia, diurnas+nocturnas+diurnasf+nocturnasf, GetColumnaReal(columna_extras), -1, valortotal, valorpadre);
	return 0;
}

int CInformesView::GetTotalHorasExtras(const COleDateTimeSpan &span) {

	int segundos=span.GetTotalSeconds();
	if(segundos>m_nSegundos)
		return segundos/60;
	return 0;
}

int CInformesView::ParseDiferencia(HTREELISTITEM item, const COleDateTimeSpan &span, bool aplicarpadre/*=false*/) {

	int segundos=span.GetTotalSeconds();
	if(abs(segundos)>m_nSegundos) { // lo mostramos segun la tolerancia.
		if(segundos>0) { //extras
			// solo se aplica si no hay una alerta.
			if(m_TLResultados.GetItemColor(item)==m_clrIluminado || m_TLResultados.GetItemColor(item)==0)
				m_TLResultados.SetItemColor(item,AfxGetApp()->GetProfileInt("informes", "extra", RGB(0,255,0)));
			if(aplicarpadre && m_TLResultados.GetParentItem(item)) {
				if(m_TLResultados.GetItemColor(m_TLResultados.GetParentItem(item))==m_clrIluminado || m_TLResultados.GetItemColor(m_TLResultados.GetParentItem(item))==0)
					m_TLResultados.SetItemColor(m_TLResultados.GetParentItem(item),AfxGetApp()->GetProfileInt("informes", "extra", RGB(0,255,0)));
			}
		} else { // menos
			m_TLResultados.SetItemColor(item,AfxGetApp()->GetProfileInt("informes", "alerta", RGB(255,0,0)));
			if(aplicarpadre && m_TLResultados.GetParentItem(item))
				m_TLResultados.SetItemColor(m_TLResultados.GetParentItem(item),AfxGetApp()->GetProfileInt("informes", "alerta", RGB(255,0,0)));
		}
		return segundos;
	}
	return 0;
}

void CInformesView::SetObservaciones(HTREELISTITEM item, int segundos, const CString &positiva, const CString &negativa, bool aplicarpadre/*=false*/) {

	CString temp;
	temp= m_TLResultados.GetItemText(item,GetColumnaReal(columna_observaciones));
	if(!temp.IsEmpty())
		temp+=",";
	if(segundos>0) { // positiva
		temp+=positiva;
	} else if(segundos<0) { // negativa
		temp+=negativa;
	}
	if(segundos) {
		m_TLResultados.SetItemText(item,temp, GetColumnaReal(columna_observaciones));
		if(aplicarpadre && m_TLResultados.GetParentItem(item)) 
			SetObservaciones(m_TLResultados.GetParentItem(item), segundos, positiva, negativa);
	}
}

bool CInformesView::VerificarDia(HTREELISTITEM pDia, int dia, COleDateTime *desde, COleDateTime *hasta) {

	HTREELISTITEM item= pDia ? m_TLResultados.GetChildItem(pDia) : NULL;
	if(!item)
		return false;
	CTreeDate *tiempo= (CTreeDate *)m_TLResultados.GetItemData(item);
	if(!tiempo)
		return false;
	if(dia!=-1) { // no nos importa el dia.
		if(EsFestivo(tiempo->m_pEntrada)) {
			if(dia!=1)
				return false;
		} else if(dia==1) {
			return false;
		} else if(dia!=tiempo->m_pEntrada.GetDayOfWeek()) // verificamos que sea el dia.
			return false;
	}
	if(!desde)
		return false;

	COleDateTime entrada;
	COleDateTime salida;
	COleDateTimeSpan duracion(0,0,0,0);
	COleDateTimeSpan esperado(0, desde->GetMinute(), desde->GetSecond(), 0);

	// obtenemos la entrada y la salida del dia.
	while(item) {

		CTreeDate *tiempo= (CTreeDate *)m_TLResultados.GetItemData(item);
		if(tiempo) {
			duracion+=tiempo->GetSpan();
			if(entrada.m_dt==0) 
				entrada= tiempo->m_pEntrada;
			salida= tiempo->m_pSalida;
		}
		item= m_TLResultados.GetNextSiblingItem(item);
	}

	COleDateTimeSpan spanentrada(0,0,0,0);
	COleDateTimeSpan spansalida(0,0,0,0);

	if(hasta) { //horario normal

		spanentrada= CTreeDate::NivelarFecha(entrada, *desde)-entrada;
		if(salida.GetStatus()==COleDateTime::valid)
			spansalida= salida-CTreeDate::NivelarFecha(salida, *hasta);
		if(m_bDiferencia) {
			m_TLResultados.SetItemText(pDia,CTreeDate::GetFormatSpan(spanentrada), GetColumnaReal(columna_entradadiferencia));
			m_TLResultados.SetItemText(pDia,CTreeDate::GetFormatSpan(spansalida), GetColumnaReal(columna_salidadiferencia));
		}

		SetObservaciones(pDia,ParseDiferencia(pDia, spanentrada), "Entrada temprano", "Entrada tarde");
		SetObservaciones(pDia,ParseDiferencia(pDia, spansalida), "Salida tarde", "Salida temprano");
		
		if(salida.GetStatus()==COleDateTime::valid) {
			GetTotalHorasExtras(pDia, entrada, CTreeDate::NivelarFecha(entrada, *desde)); // llegadas temprano.
			GetTotalHorasExtras(pDia, CTreeDate::NivelarFecha(salida, *hasta), salida);   // salidas temprano.
			GetTotalRecargoNocturno(pDia, entrada,salida, CTreeDate::NivelarFecha(entrada, *desde), CTreeDate::NivelarFecha(salida, *hasta));
		}
		// calcular las horas extras

	} else if(desde) { // por horas
		if(m_bDiferencia)
			m_TLResultados.SetItemText(pDia,CTreeDate::GetFormatSpan(duracion-esperado), GetColumnaReal(columna_entradadiferencia));
		SetObservaciones(pDia,ParseDiferencia(pDia, duracion-esperado), "Duración jornada mayor", "Duración jornada menor");
		GetTotalHorasExtras(pDia, salida-(duracion-esperado), salida);
		GetTotalRecargoNocturno(pDia, salida-(duracion-esperado), salida, salida-(duracion-esperado), salida+esperado);
	} else { // no encaja en ningun dia.
		GetTotalHorasExtras(pDia, salida-duracion, salida);
	}
	return true;
}

void CInformesView::VerificarDia(HTREELISTITEM item, CUsuarioHorario *horario) {
	
	COleDateTime *desde=NULL;
	COleDateTime *hasta=NULL;
	bool parse= false;

	for(int i=0; i<horario->GetHorarios(); i++) {

		int tipo= horario->GetTipo(i);
		desde= horario->GetDesde(i);
		hasta= horario->GetHasta(i);

		if( tipo&CHorarios::dia_lunes)
			parse|= VerificarDia(item, 2, desde, hasta);
		if( tipo&CHorarios::dia_martes)
			parse|= VerificarDia(item, 3, desde, hasta);
		if( tipo&CHorarios::dia_miercoles)
			parse|= VerificarDia(item, 4, desde, hasta);
		if( tipo&CHorarios::dia_jueves)
			parse|= VerificarDia(item, 5, desde, hasta);
		if( tipo&CHorarios::dia_viernes)
			parse|= VerificarDia(item, 6, desde, hasta);
		if( tipo&CHorarios::dia_sabado)
			parse|= VerificarDia(item, 7, desde, hasta);
		if( tipo&CHorarios::dia_festivo)
			parse|= VerificarDia(item, 1, desde, hasta);
		if(tipo==CHorarios::dia_almuerzo)
			VerificarAlmuerzo(item, desde, hasta);
	}
	if(!parse) // no encaja en ningun dia.
		VerificarDia(item, -1, NULL, NULL);
}

void CInformesView::SetHorarios(CString usuario) {

	HTREELISTITEM pUsuario= FindUsuario(usuario);
	if(!pUsuario)
		return;
	CUsuarioHorario *hor=(CUsuarioHorario *)m_TLResultados.GetItemData(pUsuario);
	HTREELISTITEM item=m_TLResultados.GetChildItem(pUsuario);
	while(item) { // estos son los dias
		VerificarDia(item, hor);
		item= m_TLResultados.GetNextSiblingItem(item);
	}
	m_TLResultados.SetItemText(pUsuario, hor->GetDocumento());
}

// hasta aqui se hacen calculos sin horarios.
// esta otra tambien
COleDateTimeSpan CInformesView::RecalcularDia(HTREELISTITEM dia) {

	HTREELISTITEM item= dia ? m_TLResultados.GetChildItem(dia) : NULL;
	COleDateTimeSpan span(0,0,0,0);
	if(!item)
		return span;
	CTreeDate *info;
	CMyTime entrada, salida;
	while(item) {
		info= (CTreeDate *)m_TLResultados.GetItemData(item);
		if(info) {
			span+= info->GetSpan();
			m_TLResultados.SetItemText(item,info->GetFormatSpan(), GetColumnaReal(columna_tiempotexto));
			if(entrada.m_dt==0) 
				entrada= info->m_pEntrada;
			salida= info->m_pSalida;
		}
		item= m_TLResultados.GetNextSiblingItem(item); // esta son las horas 
	}
	m_TLResultados.SetItemText(dia,CTreeDate::GetFormatSpan(span),GetColumnaReal(columna_tiempotexto));
	m_TLResultados.SetItemText(dia,entrada.GetHora(), GetColumnaReal(columna_entrada));
	m_TLResultados.SetItemText(dia,salida.GetHora(), GetColumnaReal(columna_salida));
	return span;
}
// esta tambien
void CInformesView::RecalcularUsuario(CString usuario) {

	if(usuario.IsEmpty())
		return;
	HTREELISTITEM pUsuario=NULL;
	HTREELISTITEM item= FindUsuario(usuario);
	ASSERT(item);
	if(!item) // tiene que existir
		return;
	pUsuario= item;
	item= m_TLResultados.GetChildItem(item);
	COleDateTimeSpan span(0,0,0,0);
	while(item) { // aqui estan los dias
		span+=RecalcularDia(item);
		item= m_TLResultados.GetNextSiblingItem(item);
	}
	m_TLResultados.SetItemText(pUsuario,CTreeDate::GetFormatSpan(span),GetColumnaReal(columna_tiempotexto));
	// temporalmente lo vamos ha hacer con find
	if(AfxGetApp()->GetProfileInt("informes","todoslosdias",1)) { // aqui mostramos todos los dias.
		COleDateTimeSpan span(1,0,0,0);
		COleDateTime desde, hasta;
		m_DTDesde.GetTime(desde);
		m_DTHasta.GetTime(hasta);
		HTREELISTITEM pUsuario= FindUsuario(usuario);
		CString texto;
		CMyTime tentrada;
		COleDateTime temp;
		HTREELISTITEM old;

		while(desde<=hasta) {

			tentrada= desde;
			item= m_TLResultados.GetChildItem(pUsuario);
			old=NULL;
			while(item) { // estos son los dias.
				if(m_TLResultados.GetChildItem(item)) {
					CTreeDate *date= (CTreeDate *)m_TLResultados.GetItemData(m_TLResultados.GetChildItem(item));
					if(date) {
						if(desde==CTreeDate::NivelarFecha(date->m_pEntrada, desde)) {
							break;
						} else if(date->m_pEntrada>desde) {
							item= NULL;
							break;
						}
					}
				}
				old= item;
				item= m_TLResultados.GetNextSiblingItem(item);
			}
			if(!item && !EsFestivo(tentrada)) {
				item= m_TLResultados.InsertItem(tentrada.GetFecha(AfxGetApp()->GetProfileInt("informes", "fechaslargas",1)), pUsuario, old ? old : TLI_FIRST);
				// hacer algo con el item
				m_TLResultados.SetItemColor(item,AfxGetApp()->GetProfileInt("informes", "alerta", RGB(255,0,0)));
				m_TLResultados.SetItemText(item,"No se encontro información del día", GetColumnaReal(columna_observaciones));
				m_TLResultados.SetItemText(item,"No se encontro información del día", GetColumnaReal(columna_salidatexto));
			}
			desde+=span;
		}
	}
}

// esta es la funcion que va a reemplazar a todos los de arriba
HTREELISTITEM CInformesView::AddItem(CString usuario, CString nombre, CString entrada, CString salida, CString valor, CString documento, CString departamento) {

	HTREELISTITEM parent;
	HTREELISTITEM item= FindUsuario(usuario);
	if(!item) {
		item= m_TLResultados.InsertItem(usuario);
		m_TLResultados.SetItemText(item,nombre, GetColumnaReal(columna_salidatexto));
		m_TLResultados.SetItemColor(item,m_clrIluminado);
		m_TLResultados.SetItemBold(item);
		CUsuarioHorario *hor= new CUsuarioHorario(usuario, valor, documento, departamento);
		m_TLResultados.SetItemData(item,(DWORD)hor);
		m_pConector->Query("SELECT usuario,dia,entrada,salida FROM horarios WHERE usuario='" + usuario + "'", query_horarios, this);
	}
	parent= item;
	item= m_TLResultados.GetChildItem(item);

	CMyTime tentrada;
	tentrada.ParseDateTime(entrada);
	CMyTime tsalida;
	tsalida.ParseDateTime(salida);

	CString texto= tentrada.GetFecha(AfxGetApp()->GetProfileInt("informes", "fechaslargas",1));
	while(item) {
		if(texto.Compare(m_TLResultados.GetItemText(item,GetColumnaReal(columna_ingresotexto)))==0) 
			break;
		item= m_TLResultados.GetNextSiblingItem(item);
	}
	if(!item) {
		item= m_TLResultados.InsertItem(texto, parent);
		m_TLResultados.SetItemColor(item,m_clrIluminado);
	}

	if(!nombre.IsEmpty()) { // es un dia especial

		CTreeDate *treedate= new CTreeDate(tentrada, tsalida);
		item= m_TLResultados.InsertItem(tentrada.GetHora(), item);
		m_TLResultados.SetItemText(item,tsalida.GetHora(), GetColumnaReal(columna_salidatexto));
		m_TLResultados.SetItemData(item,(DWORD)treedate);	
	}
	if(m_szOldUsuario.IsEmpty())
		m_szOldUsuario= usuario;
	else if(m_szOldUsuario!=usuario) {
		RecalcularUsuario(m_szOldUsuario);
		m_szOldUsuario= usuario;
	}
	return item;
}

LRESULT CInformesView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage mess(lParam);
	ODBCMessage query(lParam);

	switch( wParam) {
		case QUERY_TITLE:
			if(mess.GetID()==query_departamentos) {
				CString texto= m_CBDepartamento.Text;
				m_CBDepartamento.ResetContent();
				m_CBDepartamento.AddString("(Todos)");
				m_CBDepartamento.SetItemData(m_CBDepartamento.AddString("(Ninguno)"), -1);
				m_CBDepartamento.Text=texto;
			}
			break;
		case QUERY_RESULT:
			if(mess.GetID()==query_consultas) {
				if(m_pUsuarioActual.IsValid(mess["usuario"], mess["entrada"])) {
					AddItem(
						m_pUsuarioActual.m_szUsuario,
						m_pUsuarioActual.m_szNombre,
						m_pUsuarioActual.m_szEntrada,
						m_pUsuarioActual.m_szSalida,
						m_pUsuarioActual.m_szValor,
						m_pUsuarioActual.m_szDocumento,
						m_pUsuarioActual.m_szDepartamento
					);
					m_pUsuarioActual.Clean();
				} else {
					if(m_pUsuarioActual.IsFull())
						AddItem(
							m_pUsuarioActual.m_szUsuario,
							m_pUsuarioActual.m_szNombre,
							m_pUsuarioActual.m_szEntrada,
							"",
							m_pUsuarioActual.m_szValor,
							m_pUsuarioActual.m_szDocumento,
							m_pUsuarioActual.m_szDepartamento
						);
					m_pUsuarioActual.SetDatos(mess["usuario"], mess["nombre"],mess["entrada"],mess["valor"], mess["documento"], mess["departamento"]);
				}
			} else if(mess.GetID()==query_horarios) {

				m_szOldUsuario=mess[0];
				HTREELISTITEM item= FindUsuario(m_szOldUsuario);
				if(item) {
					CUsuarioHorario *hor=(CUsuarioHorario *)m_TLResultados.GetItemData(item);
					if( hor) {
						ASSERT(hor->GetID()==mess.GetAt(0));
						hor->AddTipo(atoi(mess.GetAt(1)), mess.GetAt(2), mess.GetAt(3));
					}
				}
			} else if(mess.GetID()==query_departamentos) {
				m_CBDepartamento.SetItemData(m_CBDepartamento.AddString(query["DeptName"]), atoi(query["DeptId"]));
			} else if(mess.GetID()==query_festivos) {
				m_saFestivos.Add(mess[0]);
			}
			break;
		case THREAD_END:
			if(mess.GetID()==query_consultas) {
				
				if(m_pUsuarioActual.IsFull())
					AddItem(
						m_pUsuarioActual.m_szUsuario,
						m_pUsuarioActual.m_szNombre,
						m_pUsuarioActual.m_szEntrada,
						"",
						m_pUsuarioActual.m_szValor,
						m_pUsuarioActual.m_szDocumento,
						m_pUsuarioActual.m_szDepartamento
					);
				m_pUsuarioActual.Clean();

				RecalcularUsuario(m_szOldUsuario);
				m_TLResultados.SetMessageLista("No se encontraron resultados");
				m_TLResultados.AutosizeColumns();
			} else if(mess.GetID()==query_horarios) {
				if(!m_szOldUsuario.IsEmpty())
					SetHorarios(m_szOldUsuario);
				m_szOldUsuario="";
			}
			break;
	}
	return 0;
}

void CInformesView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(::IsWindow(m_TLResultados)) {
		m_TLResultados.MoveWindow(5, 70, cx-5, cy-70);
	}
}

void CInformesView::OnVerUsuario() {
	
	HTREELISTITEM item=m_TLResultados.GetSelectedItem();
	HTREELISTITEM parent;

	parent= m_TLResultados.GetParentItem(item);
	if(parent!=m_TLResultados.GetParentItem(m_TLResultados.GetRootItem())) {
		item= m_TLResultados.GetParentItem(parent);
		if(item==m_TLResultados.GetParentItem(m_TLResultados.GetRootItem())) {
			item= parent;
		}
	}
	if(item) {
		CUsuarioHorario *hor= (CUsuarioHorario *)m_TLResultados.GetItemData(item);
		CString texto= hor->GetID();
		AfxGetMainWnd()->SendMessage(WM_TREENAVEGADOR,100, (LPARAM)(const char *)"CSVEView");
		((CMDIFrameWndXP *)AfxGetMainWnd())->SendMessageBroadCast(WM_INSERTODS, NULL, (LPARAM)(const char *)texto);
	}
}

BOOL CInformesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	
	if( m_pTreePrint)
		delete m_pTreePrint;
	m_pTreePrint= new CTreeListPrint();

	m_pTreePrint->SetTreeListCtrl(&m_TLResultados);
	m_pTreePrint->SetAppName("SDGFinger Nómina (TimeKeeper)");
	m_pTreePrint->SetDocTitle(GetDocument()->GetTitle());
	m_pTreePrint->SetFooterIzq(m_pUsuario->GetNombre());

	CMyTime tiempo(true);

	m_pTreePrint->SetFooter(tiempo.GetFechaHora(AfxGetApp()->GetProfileInt("fechas", "larga", 0)));

	m_pTreePrint->OnPreparePrinting(pInfo);

	m_pTreePrint->SetMarginBottomPix(AfxGetApp()->GetProfileInt("printer", "cy", 20));
	m_pTreePrint->SetMarginLeftPix(AfxGetApp()->GetProfileInt("printer", "x", 20));
	m_pTreePrint->SetMarginRightPix(AfxGetApp()->GetProfileInt("printer", "cx", 20));
	m_pTreePrint->SetMarginTopPix(AfxGetApp()->GetProfileInt("printer", "y", 20));

	return DoPreparePrinting(pInfo);
}

void CInformesView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pTreePrint->OnBeginPrinting(pDC, pInfo);
}

void CInformesView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pTreePrint->OnPrint(pDC, pInfo);
}

void CInformesView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pTreePrint->OnEndPrinting(pDC, pInfo);
	delete m_pTreePrint;
	m_pTreePrint= NULL;
}

void CInformesView::OnUpdatePrint(CCmdUI *pCmdUI) {

	pCmdUI->Enable(m_TLResultados.GetItemCount()>0);
}

void CInformesView::OnArchivoExportar()
{
	CGuardarDlg guardar;
	if(guardar.DoModal()!=IDOK)
		return;
	int campos=guardar.m_nCampos;

	CExport pExport(NULL);
	CFileDialog dlg( false, pExport.GetDefaultType(true), GetDocument()->GetTitle(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, pExport.GetFileTypes(true));
	if( dlg.DoModal()==IDOK) {

			CWaitCursor wait;

			CStringArray result;
			if(!pExport.CustomExportLocal( dlg.GetPathName(), GetDocument()->GetTitle())) {
				AfxMessageBox("No se puede obtener acceso a \"" + dlg.GetPathName() + "\".\nCompruebe que el archivo no este siendo utilizado por otro proceso.", MB_OK | MB_ICONERROR);
				return;
			}

			int len= m_TLResultados.GetColumnCount();

			result.Add("Documento");
			result.Add("Nombre");
			result.Add("Fecha");
			for( int i=0; i<len; i++) {
				result.Add(m_TLResultados.GetColumnText(i));
				pExport.SetTipoColumna(i+3, m_TLResultados.GetColumnType(i));
			}
			pExport.Inicio(result);
			ULONG filas= m_TLResultados.GetItemCount();
			AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 0, (LPARAM)filas);
		
			HTREELISTITEM parent= m_TLResultados.GetRootItem();
			HTREELISTITEM item,subitem;

			while( parent) {
				for( int i=0; i<result.GetCount(); i++)
					result.SetAt(i,"");
				result.SetAt(0,m_TLResultados.GetItemText(parent));
				result.SetAt(1, m_TLResultados.GetItemText(parent,1));

				for(int i=5; i<result.GetCount(); i++) 
					result.SetAt(i, m_TLResultados.GetItemText(parent,i-3));
				if( campos&1)
					pExport.Agregar(result);
				AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 1);
				item= m_TLResultados.GetChildItem(parent);

				while( item) {
					result.SetAt(2, m_TLResultados.GetItemText(item));
					result.SetAt(3, "");
					result.SetAt(4, "");
					for(int i=5; i<result.GetCount(); i++) 
						result.SetAt(i, m_TLResultados.GetItemText(item,i-3));
					if(campos&2)
						pExport.Agregar(result);
					AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 1);
					subitem= m_TLResultados.GetChildItem(item);
					while(subitem) {

						for(int i=3; i<result.GetCount(); i++) 
							result.SetAt(i, m_TLResultados.GetItemText(subitem,i-3));
						if(campos&4)
							pExport.Agregar(result);
						AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 1);
						subitem= m_TLResultados.GetNextSiblingItem(subitem);
					}
					item= m_TLResultados.GetNextSiblingItem(item);
				}
				parent= m_TLResultados.GetNextSiblingItem(parent);
			}
			pExport.Terminar();
			AfxGetMainWnd()->PostMessage(WM_STATUSBAR, 2);
			if( MyMessageBox("¿Desea ver el archivo?", MB_ICONQUESTION | MB_YESNOCANCEL, "showfile")==IDYES) 
				ShellExecute(NULL, "open", dlg.GetPathName(), NULL,NULL,SW_SHOWNORMAL);
	}
}

void CInformesView::OnRClickArbol( NMHDR* pNMHDR, LRESULT* pResult) {

	LPNMTREELIST lpnmtl = ( LPNMTREELIST )pNMHDR;
	HTREELISTITEM item = 	lpnmtl->pItem;
	
	CNewMenu menu;
	menu.CreatePopupMenu();

	if( item) {
		menu.AppendMenu(MF_STRING, 2510, "&Ver Usuario");
		menu.AppendMenu(MF_SEPARATOR);
	}
	m_TLResultados.GetMenuContextual(&menu);

	POINT point;
    GetCursorPos( &point );

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
	*pResult = 0;
}

void CInformesView::OnDestroy()
{
	CWaitCursor wait;
	m_pConector->KillQuery(-1, this);
	HTREELISTITEM item= m_TLResultados.GetRootItem();
	HTREELISTITEM subitem,horas;
	CUsuarioHorario *hor;
	CTreeDate *date;

	while( item) {
		hor= (CUsuarioHorario *) m_TLResultados.GetItemData(item);
		if( hor) 
			delete hor;
		subitem= m_TLResultados.GetChildItem(item);
		while(subitem) { // dias
			horas= m_TLResultados.GetChildItem(subitem);
			while(horas) {
				date= (CTreeDate *)m_TLResultados.GetItemData(horas);
				if(date)
					delete date;
				horas=m_TLResultados.GetNextSiblingItem(horas);
			}
			subitem= m_TLResultados.GetNextSiblingItem(subitem);
		}
		item= m_TLResultados.GetNextSiblingItem(item);
	}
	m_TLResultados.DeleteAllItems();
	CFormView::OnDestroy();
}

void CInformesView::OnBnClickedBopciones()
{
	CDlgConfigurar dlg;
	if(dlg.DoModal()==IDOK) {
		m_nSegundos= AfxGetApp()->GetProfileInt("informes", "tolerancia", 15*60);
		m_bCalcular=AfxGetApp()->GetProfileInt("informes", "calcular", 0);
		m_bDiferencia=AfxGetApp()->GetProfileInt("informes", "diferencia", 0);
		OnBnClickedOk();
	}
}

LRESULT CInformesView::OnUpdateHorarios(WPARAM wParam, LPARAM lParam) {

	if(wParam==1) {
		m_pConector->Query("SELECT DeptId,DeptName FROM Dept",query_departamentos, this);
	} if(lParam==3) {

		COleDateTime *tiempo;
		UINT tamano;

		if(!AfxGetApp()->GetProfileBinary("configuracion", "desde", (LPBYTE *)&tiempo, &tamano)) {
			tiempo= new COleDateTime();
			tiempo->SetTime(6, 0, 0);
		}
		m_tEntrada.SetTime(tiempo->GetHour(),tiempo->GetMinute(), 0);

		delete tiempo;
		if(!AfxGetApp()->GetProfileBinary("configuracion", "hasta", (LPBYTE *)&tiempo, &tamano)) {
			tiempo= new COleDateTime();
			tiempo->SetTime(22, 0, 0);
		}
		m_tSalida.SetTime(tiempo->GetHour(), tiempo->GetMinute(), 0);
		delete tiempo;

		m_dHoraNocturna= atof(AfxGetApp()->GetProfileString("configuracion", "nocturna", "1.35"));
		m_dHoraExtraNocturna= atof(AfxGetApp()->GetProfileString("configuracion", "dominical", "1.75"));
		m_dHoraExtraDiurna= atof(AfxGetApp()->GetProfileString("configuracion", "diurna", "1.25"));
		m_dHoraOrdinariaDominical= atof(AfxGetApp()->GetProfileString("configuracion", "enocturna", "1.75"));
		m_dHoraExtraDiurnaDominical= atof(AfxGetApp()->GetProfileString("configuracion", "edomical", "2"));
		m_dHoraExtraNocturnaDominical= atof(AfxGetApp()->GetProfileString("configuracion", "ndominical", "2.5"));
		m_dHoraRecargoNocturno= atof(AfxGetApp()->GetProfileString("configuracion", "recargon", "0.35"));

		m_dSalarioMinimo = atof(AfxGetApp()->GetProfileString("configuracion", "smmlv", "515000"));
		m_dAuxilioTransporte= atof(AfxGetApp()->GetProfileString("configuracion", "auxiliotransporte", "61500"));
	}
	return 0;
}

COleDateTimeSpan CInformesView::ParseSpan(const CString &texto) {

	if(texto.IsEmpty()) {
		return COleDateTimeSpan(0,0,0,0);
	} 
	CString derecha= GetRightString(texto, ":");
	return COleDateTimeSpan(0, atoi(GetLeftString(texto, ":")), atoi(GetLeftString(derecha, ":")), atoi(GetRightString(derecha, ":")));
}

void CInformesView::OnGuardarFijos() {

	CFileDialog dlg( false, "xls", "Fijos", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, "Archivos de Excel (*.xls)|*.xls||");
	if(dlg.DoModal()==IDOK) {

		CMyTime desde, hasta;
		CString temp;

		m_DTDesde.GetTime(desde);
		m_DTHasta.GetTime(hasta);

		COleDateTimeSpan diferencia= hasta-desde;
		COleDateTimeSpan spancalculos;

		int nFilas=0;
		BasicExcel *pBasicExcel;

		pBasicExcel= new BasicExcel();
		pBasicExcel->New(1);
		pBasicExcel->RenameWorksheet("Sheet1", "Hoja1");
		BasicExcelWorksheet *pSheet= pBasicExcel->GetWorksheet("Hoja1");

		pSheet->Cell(nFilas, 0)->Set("No"); // numero
		pSheet->Cell(nFilas, 1)->Set("ID"); // id
		pSheet->Cell(nFilas, 2)->Set("Departamento"); // departamento
		pSheet->Cell(nFilas, 3)->Set("Cedula"); // cedula
		pSheet->Cell(nFilas, 4)->Set("Nombres"); // nombres
		pSheet->Cell(nFilas, 5)->Set("Salario"); // salario
		pSheet->Cell(nFilas, 6)->Set("D"); // dias trabajados
		pSheet->Cell(nFilas, 7)->Set("Sueldo"); // sueldo formulado
		pSheet->Cell(nFilas, 8)->Set("Dias incapacidad"); // dias incapacidad
		pSheet->Cell(nFilas, 9)->Set("V/R incapacidad"); // valor dias incapacidad
		pSheet->Cell(nFilas, 10)->Set("Auxilio"); // valor dias incapacidad
		pSheet->Cell(nFilas, 11)->Set("H.E.D."); // valor dias incapacidad
		pSheet->Cell(nFilas, 12)->Set("H.E.N."); // valor dias incapacidad
		pSheet->Cell(nFilas, 13)->Set("H.E.F."); // valor dias incapacidad
		pSheet->Cell(nFilas, 14)->Set("R.N."); // valor dias incapacidad
		pSheet->Cell(nFilas, 15)->Set("Vr. H.E.D."); // valor dias incapacidad
		pSheet->Cell(nFilas, 16)->Set("Vr. H.E.N."); // valor dias incapacidad
		pSheet->Cell(nFilas, 17)->Set("Vr. H.E.F."); // valor dias incapacidad
		pSheet->Cell(nFilas, 18)->Set("Vr. R.N."); // valor dias incapacidad
		pSheet->Cell(nFilas, 19)->Set("Total H.E."); // valor dias incapacidad
		pSheet->Cell(nFilas, 20)->Set("Total Devengado"); // valor dias incapacidad
		pSheet->Cell(nFilas, 21)->Set("EPS"); // valor dias incapacidad
		pSheet->Cell(nFilas, 22)->Set("Pensión"); // valor dias incapacidad
		pSheet->Cell(nFilas, 23)->Set("FS"); // valor dias incapacidad

		nFilas++;

		HTREELISTITEM item=m_TLResultados.GetRootItem();
		while(item) {

			CUsuarioHorario *hor= (CUsuarioHorario *) m_TLResultados.GetItemData(item);
			pSheet->Cell(nFilas, 0)->Set(nFilas+1); // numero
			pSheet->Cell(nFilas, 1)->Set(hor->GetID()); // id
			temp="";
			for(int i=0; i<m_CBDepartamento.GetCount(); i++) {
				if(m_CBDepartamento.GetItemData(i)==atoi(hor->GetDepartamento())) {
					m_CBDepartamento.GetLBText(i, temp);
					break;
				}

			}
			pSheet->Cell(nFilas, 2)->Set(temp); // departamento
			pSheet->Cell(nFilas, 3)->Set(hor->GetDocumento()); // cedula
			pSheet->Cell(nFilas, 4)->Set(m_TLResultados.GetItemText(item, 1)); // nombres
			pSheet->Cell(nFilas, 5)->Set(hor->GetSalario()); // salario
			HTREELISTITEM subitem = m_TLResultados.GetChildItem(item);
			int dias=0, ausencia=0;
			while(subitem) {
			
				if(m_TLResultados.GetChildItem(subitem)) {
					dias++;
				} else {
					ausencia++;
				}
				subitem= m_TLResultados.GetNextSiblingItem(subitem);
			}
			// los feriados
			dias+= diferencia.GetDays()-dias-ausencia;
			temp.Format("%d", dias);
			
			pSheet->Cell(nFilas, 6)->Set(dias); // dias trabajados
			pSheet->Cell(nFilas, 7)->Set(dias*hor->GetSalario()/30); // sueldo formulado
			pSheet->Cell(nFilas, 8)->Set(ausencia); // dias incapacidad
			pSheet->Cell(nFilas, 9)->Set(ausencia*hor->GetSalario()/30); // salario
			double auxtrans=0;
			if(hor->GetSalario()<m_dSalarioMinimo*2)
				auxtrans= m_dAuxilioTransporte*dias/30;
			pSheet->Cell(nFilas, 10)->Set(auxtrans);
			pSheet->Cell(nFilas, 11)->Set(m_TLResultados.GetItemText(item, GetColumnaReal(columna_diurnas)));
			pSheet->Cell(nFilas, 12)->Set(m_TLResultados.GetItemText(item, GetColumnaReal(columna_nocturnas)));
			pSheet->Cell(nFilas, 13)->Set(CTreeDate::GetFormatSpan(
											ParseSpan(m_TLResultados.GetItemText(item, GetColumnaReal(columna_diurnasfestivos)))
											+ParseSpan(m_TLResultados.GetItemText(item, GetColumnaReal(columna_nocturnasfestivos)))));
			pSheet->Cell(nFilas, 14)->Set(m_TLResultados.GetItemText(item, GetColumnaReal(columna_recargonocturno)));
			
			spancalculos= ParseSpan(m_TLResultados.GetItemText(item, GetColumnaReal(columna_diurnas)));
			int horas= spancalculos.GetTotalHours();
			if(spancalculos.GetMinutes()>(m_nSegundos/60))
				horas++;
			double diurnas=horas*hor->GetValorHora()*m_dHoraExtraDiurna;
			pSheet->Cell(nFilas, 15)->Set(diurnas);

			spancalculos= ParseSpan(m_TLResultados.GetItemText(item, GetColumnaReal(columna_nocturnas)));
			horas= spancalculos.GetTotalHours();
			if(spancalculos.GetMinutes()>(m_nSegundos/60))
				horas++;
			double nocturnas=horas*hor->GetValorHora()*m_dHoraExtraNocturna;
			pSheet->Cell(nFilas, 16)->Set(nocturnas);
			
			spancalculos= ParseSpan(m_TLResultados.GetItemText(item, GetColumnaReal(columna_diurnasfestivos)))
				+ParseSpan(m_TLResultados.GetItemText(item, columna_nocturnasfestivos));
			horas= spancalculos.GetTotalHours();
			if(spancalculos.GetMinutes()>(m_nSegundos/60))
				horas++;
			double festivos= horas*hor->GetValorHora()*m_dHoraExtraDiurnaDominical;
			pSheet->Cell(nFilas, 17)->Set(festivos);

			spancalculos= ParseSpan(m_TLResultados.GetItemText(item, GetColumnaReal(columna_recargonocturno)));
			horas= spancalculos.GetTotalHours();
			if(spancalculos.GetMinutes()>(m_nSegundos/60))
				horas++;
			double recargonocturno= horas*hor->GetValorHora()*m_dHoraRecargoNocturno;
			pSheet->Cell(nFilas, 18)->Set(recargonocturno);

			pSheet->Cell(nFilas, 19)->Set(recargonocturno+festivos+nocturnas+diurnas);
			double totalsalario=auxtrans+recargonocturno+festivos+nocturnas+diurnas+(dias*hor->GetSalario()/30)+(ausencia*hor->GetSalario()/30);
			pSheet->Cell(nFilas, 20)->Set(totalsalario);
			totalsalario-=auxtrans;
			pSheet->Cell(nFilas, 21)->Set(totalsalario*0.04);
			pSheet->Cell(nFilas, 22)->Set(totalsalario*0.04);
			if(hor->GetSalario()>m_dSalarioMinimo*4)
				pSheet->Cell(nFilas, 23)->Set(totalsalario*0.01);
			else pSheet->Cell(nFilas, 23)->Set(0);
			
			nFilas++;
			item= m_TLResultados.GetNextSiblingItem(item);
		}

		if(pBasicExcel->SaveAs(dlg.GetPathName())) {
			delete pBasicExcel;
			if( MyMessageBox("¿Desea ver el archivo?", MB_ICONQUESTION | MB_YESNOCANCEL, "showfile")==IDYES) 
				ShellExecute(NULL, "open", dlg.GetPathName(), NULL,NULL,SW_SHOWNORMAL);
		} else {
			AfxMessageBox("No se puede obtener acceso a \"" + dlg.GetPathName() + "\".\nCompruebe que el archivo no este siendo utilizado por otro proceso.", MB_OK | MB_ICONERROR);
			delete pBasicExcel;
		}
		
	}
}

void CInformesView::OnGuardarTemporales() {

}
