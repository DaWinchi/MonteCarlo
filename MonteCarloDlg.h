
// MonteCarloDlg.h: файл заголовка
//

#pragma once
#include "Drawer2D.h"

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
	std::vector<std::vector<int>> _points;
	int _N;
	afx_msg void OnBnClickedRun();
};
