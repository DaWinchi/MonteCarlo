
// MonteCarloDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MonteCarlo.h"
#include "MonteCarloDlg.h"
#include "afxdialogex.h"
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMonteCarloDlg



CMonteCarloDlg::CMonteCarloDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MONTECARLO_DIALOG, pParent)
	, _N(50)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonteCarloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL, _drawerModel);
	DDX_Text(pDX, IDC_EDIT1, _N);
}

BEGIN_MESSAGE_MAP(CMonteCarloDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, &CMonteCarloDlg::OnBnClickedRun)
END_MESSAGE_MAP()


// Обработчики сообщений CMonteCarloDlg

BOOL CMonteCarloDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	srand(time(NULL));
	_drawerModel._points = &_points;

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMonteCarloDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMonteCarloDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMonteCarloDlg::InitializeModel()
{
	int width = _N + 2, height =_N + 2;
	_points.clear();
	for (int h = 0; h < height; h++)
	{
		std::vector<int> row;
		for (int w = 0; w < width; w++)
		{
			row.push_back(rand() % 3 - 1);
		}
		_points.push_back(row);
	}

}

void CMonteCarloDlg::OnBnClickedRun()
{
	UpdateData(TRUE);
	InitializeModel();
	_drawerModel.RedrawWindow();
}
