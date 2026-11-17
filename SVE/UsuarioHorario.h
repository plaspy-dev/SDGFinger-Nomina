#pragma once
# include "horarios.h"

class CUsuarioHorario
{
protected:
	CString		m_szNombre;
	CString		m_szID;
	CPtrArray	m_pHorarios;
	double		m_dValor;
public:
	CUsuarioHorario(CString id="", CString valor="");
public:
	~CUsuarioHorario(void);
	void Set(CString id, CString nombre);
	void SetId(CString id);
	void SetNombre(CString nombre);
	void CleanAll();
	void AddTipo(int tipo, CString desde, CString hasta="");
	void AddTipo(int tipo, CTime *desde, CTime *hasta=NULL);

	CString GetNombre();
	CString GetID();
	int GetHorarios();
	int GetTipo(int index);
	COleDateTime *GetDesde(int index);
	COleDateTime *GetHasta(int index);
	CString GetQuery();
	int GetQuerys(CStringArray &querys);
	int GetQuerys(CStringArray &querys,CString szid);
	bool operator==(CUsuarioHorario &horario);
	double GetValor() const;
};
