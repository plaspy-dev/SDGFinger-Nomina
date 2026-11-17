#pragma once
#include "afxdtctl.h"
#include "afxwin.h"
# include <JPrint/JPrint.h>
# include "UsuarioHorario.h"

// Vista de formulario de CInformesView

class CTreeDate {
public:
	CMyTime m_pEntrada;
	CMyTime m_pSalida;
	CTreeDate(CMyTime entrada, CMyTime salida) {
		m_pEntrada= entrada;
		m_pSalida= salida;
	}
	static CString GetFormatSpan(COleDateTimeSpan span) {
		
		if(span.GetTotalSeconds()==0)
			return "";
		CString formato;
		int horas= span.GetTotalHours();
		int minutos= span.GetMinutes();
		int segundos= span.GetSeconds();
		if(horas<0 || minutos<0 || segundos<0) {
			horas=abs(horas);
			minutos=abs(minutos);
			segundos=abs(segundos);
			formato+="-";
		}
		if( horas<10)
			formato+="0";
		formato+="%d:";
		if(minutos<10)
			formato+="0";
		formato+="%d:";
		if(segundos<10)
			formato+="0";
		formato+="%d";
		ASSERT(!formato.IsEmpty());
		CString otro=formato;
		otro.Format(formato, horas, minutos, segundos);
		return otro;
	}
	CString GetFormatSpan() {
		return (m_pSalida.EsValido() && m_pEntrada.EsValido()) ? 
			GetFormatSpan(m_pSalida-m_pEntrada) : "";
	}
	COleDateTimeSpan GetSpan() {
		return (m_pSalida.EsValido() && m_pEntrada.EsValido()) ? 
			(m_pSalida-m_pEntrada) : 
			COleDateTimeSpan(0,0,0,0);
	}
	static COleDateTime NivelarFecha(const COleDateTime &fecha, const COleDateTime &hora) {
		return COleDateTime(fecha.GetYear(), fecha.GetMonth(), fecha.GetDay(), hora.GetHour(), hora.GetMinute(), hora.GetSecond());
	}
	static COleDateTimeSpan GetSpan(CString tiempo) {
		int horas= atoi(GetLeftString(tiempo, ":"));
		tiempo= GetRightString(tiempo, ":");
		int minutos= atoi(GetLeftString(tiempo, ":"));
		int segundos= atoi(GetRightString(tiempo, ":"));
		return COleDateTimeSpan(0, horas, minutos, segundos);
	}
};

class CLlegadaYSalida {
public:
	CLlegadaYSalida() {
	}
	void SetDatos(const CString &usuario, const CString &nombre, const CString &entrada, const CString &valor, const CString &documento, const CString &departamento) {

		m_szUsuario= usuario;
		m_szNombre= nombre;
		m_szValor= valor;
		m_szEntrada= entrada; 
		m_szSalida="";
		m_szDocumento= documento;
		m_szDepartamento= departamento;

	}
	bool IsValid(const CString &usuario, const CString &salida) {
		
		if(!IsFull() || usuario!=m_szUsuario) {
			return false;
		}
		CMyTime dtEntrada, dtSalida;
		dtEntrada.ParseDateTime(m_szEntrada);
		dtSalida.ParseDateTime(salida);
		if(dtEntrada.GetDay()==dtSalida.GetDay() && dtEntrada.GetMonth()==dtSalida.GetMonth() && dtEntrada.GetYear()==dtSalida.GetYear()) {
			m_szSalida= salida;
			return true;
		}
		return false;
	}
	void Clean() {
		m_szUsuario="";
	}
	bool IsFull() {
		return !m_szUsuario.IsEmpty();
	}
	CString m_szDocumento;
	CString m_szUsuario;
	CString m_szNombre;
	CString m_szValor;
	CString m_szEntrada;
	CString m_szSalida;
	CString m_szDepartamento;
};

class CInformesView : public CFormView
{
	CLlegadaYSalida m_pUsuarioActual;
	DECLARE_DYNCREATE(CInformesView)

	enum {
		columna_ingresotexto,
		columna_salidatexto,
		columna_tiempotexto,
		columna_entrada,
		columna_entradadiferencia,
		columna_salida,
		columna_salidadiferencia,
		columna_extras,
		columna_extrastotal,
		columna_diurnas,
		columna_diurnastotal,
		columna_nocturnas,
		columna_nocturnastotal,
		columna_diurnasfestivos,
		columna_diurnasfestivostotal,
		columna_nocturnasfestivos,
		columna_nocturnasfestivostotal,
		columna_recargonocturno,
		columna_recargonocturnototal,
		columna_observaciones
	};
protected:

	HTREELISTITEM	m_pActual;
	CInformesView();           // Constructor protegido utilizado por la creación dinámica
	virtual ~CInformesView();
	CConectorODBC	*m_pConector;
	CUsuario	*m_pUsuario;
	CTreeListPrint	*m_pTreePrint;
	CTreeListCtrl	m_TLResultados;
	CString			m_szOldUsuario;
	CStringArray	m_saFestivos;
	COLORREF		m_clrIluminado;

	enum {
		query_departamentos=3,
		query_consultas,
		query_horarios,
		query_jornada,
		query_festivos
	};

public:
	enum { IDD = IDD_FINFORMES };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX o DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CButtonXP m_BAceptar;
	afx_msg void OnBnClickedOk();
	CDateTimeCtrl m_DTDesde;
	CDateTimeCtrl m_DTHasta;
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEditXP m_EDocumento;
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnUpdatePrint(CCmdUI *pCmdUI);
	void OnArchivoExportar();
	void OnVerUsuario();
	void OnRClickArbol( NMHDR* pNMHDR, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
public:
	CButtonXP m_BOpciones;
public:
	afx_msg void OnBnClickedBopciones();
	int		m_nSegundos;
public:
	CComboBoxXP		m_CBDepartamento;
	CMiscelaneosEx	m_pDepartamentos;
	LRESULT OnUpdateHorarios(WPARAM wParam, LPARAM lParam);

	inline bool	EsFestivo(CMyTime &fecha);
	HTREELISTITEM FindUsuario(CString usuario);
	HTREELISTITEM AddItem(CString usuario, CString nombre, CString entrada, CString salida, CString valor, CString documento, CString departamento);
	void		   RecalcularUsuario(CString usuario);
	COleDateTimeSpan RecalcularDia(HTREELISTITEM dia);
	void			SetHorarios(CString usuario);
	void			VerificarDia(HTREELISTITEM item, CUsuarioHorario *horario);
	bool VerificarDia(HTREELISTITEM pDia, int dia, COleDateTime *desde, COleDateTime *hasta);
	void VerificarAlmuerzo(HTREELISTITEM pDia, COleDateTime *desde, COleDateTime *hasta);
	int GetTotalHorasExtras(HTREELISTITEM pDia, COleDateTime desde, COleDateTime hasta);
	int GetTotalRecargoNocturno(HTREELISTITEM pDia, COleDateTime desde, COleDateTime hasta,COleDateTime entrada, COleDateTime salida);
	inline int ParseDiferencia(HTREELISTITEM item, const COleDateTimeSpan &span, bool aplicarpadre=false);
	inline void SetObservaciones(HTREELISTITEM item, int segundos, const CString &positiva, const CString &negativa, bool aplicarpadre=false);
	inline int GetTotalHorasExtras(const COleDateTimeSpan &span);
	inline double SetHorasExtras(HTREELISTITEM item, const COleDateTimeSpan &span, int columna, double valorminutos, double multiplicador, double &valorpadre);

	// horas habiles
	COleDateTime m_tEntrada;
	COleDateTime m_tSalida;
	double		 m_dHoraNocturna;
	double		 m_dHoraExtraNocturna;
	double		 m_dHoraExtraDiurna;
	double		 m_dHoraOrdinariaDominical;
	double		 m_dHoraExtraDiurnaDominical;
	double		 m_dHoraExtraNocturnaDominical;
	double		 m_dHoraRecargoNocturno;
	double		 m_dSalarioMinimo;
	double		m_dAuxilioTransporte;
	int			GetColumnaReal(int columna);
	bool		m_bCalcular;
	bool		m_bDiferencia;
	void		OnGuardarFijos();
	void		OnGuardarTemporales();
	COleDateTimeSpan	ParseSpan(const CString &texto);
};


