
// MonteCarloDlg.h: файл заголовка
//

#pragma once
#include "Drawer2D.h"
#include "DrawGraph.h"
#include "DrawGraph2.h"

// Диалоговое окно CMonteCarloDlg
class CMonteCarloDlg : public CDialogEx
{
// Создание
public:
	CMonteCarloDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MONTECARLO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
public:
	const double k = 8.617e-5;
	double _J;
	double Tc() const { return 2.269 * std::abs(_J) / k; }
	double _w[2];
	double _T;
	double _energySumm, _energy2Summ;


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer2D _drawerModel;
	void InitializeModel();
	int E();
	int spin_at(size_t i, size_t j);
	void ensure_periodic();
	void ensure_periodic(size_t i, size_t j);
	void next_linear(size_t n);
	void next();
	std::vector<std::vector<bool>> _points;
	std::vector<PointF> _pointsGraphEnergy;
	std::vector<PointF> _pointsGraphC;

	int _N;
	afx_msg void OnBnClickedRun();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	HANDLE hThreadCalculate;
	DWORD pdwThreadCalculate;
	bool _isCalculate;
	afx_msg void OnBnClickedStop();
	DrawGraph _drawerGraph;
	int _n;
	int _count_points;
	bool _isRandom;
	DrawGraph2 _drawerGraphC;
	double _percent;
	CButton _radioNeighbor;
	CButton _radioRandom;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int _split;
};

DWORD WINAPI ThreadCalculate(PVOID param);