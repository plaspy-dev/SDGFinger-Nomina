#pragma once

class CHorarios
{
protected:
	int				m_nTipo;
	COleDateTime	m_pDesde;
	bool			m_bHasta;
	COleDateTime	m_pHasta;
public:
	enum {
		dia_lunes=1,
		dia_martes=2,
		dia_miercoles=4,
		dia_jueves=8,
		dia_viernes=16,
		dia_sabado=32,
		dia_festivo=64,
		dia_almuerzo=128,
		dia_todasemana=256
	};
	CHorarios(void);
	CHorarios(int tipo, CTime *desde, CTime *hasta=NULL);
	CHorarios(int tipo, CString desde, CString hasta="");
public:
	~CHorarios(void);
	void SetTipo(int tipo, CString desde, CString hasta="");
	void SetTipo(int tipo, CTime *desde, CTime *hasta=NULL);
	int GetTipo();
	COleDateTime *GetDesde();
	COleDateTime *GetHasta();
	int GetTipo(CString &desde, CString &hasta);
	bool operator ==(CHorarios &horario);
};
