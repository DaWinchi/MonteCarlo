
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

CRITICAL_SECTION cs;

CMonteCarloDlg::CMonteCarloDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MONTECARLO_DIALOG, pParent)
	, _N(50)
	, _J(1)
	, _n(50)
	, _count_points(20)
	, _percent(50)
	, _split(1000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonteCarloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL, _drawerModel);
	DDX_Text(pDX, IDC_EDIT1, _N);
	DDX_Text(pDX, IDC_J, _J);
	DDX_Control(pDX, IDC_GRAPH, _drawerGraph);
	DDX_Text(pDX, IDC_NSTEPS, _n);
	DDX_Text(pDX, IDC_TSTEPS, _count_points);
	DDX_Control(pDX, IDC_GRAPH2, _drawerGraphC);
	DDX_Text(pDX, IDC_PERCENT, _percent);
	DDX_Control(pDX, IDC_RADIO1, _radioNeighbor);
	DDX_Control(pDX, IDC_RADIO2, _radioRandom);
	DDX_Text(pDX, IDC_NSTEPS2, _split);
}

BEGIN_MESSAGE_MAP(CMonteCarloDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN, &CMonteCarloDlg::OnBnClickedRun)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CMonteCarloDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_RADIO1, &CMonteCarloDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMonteCarloDlg::OnBnClickedRadio2)
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
	InitializeCriticalSection(&cs);
	srand(time(NULL));
	_drawerModel._points = &_points;
	_drawerGraph._points = &_pointsGraphEnergy;
	_drawerGraphC._points = &_pointsGraphC;
	_radioNeighbor.SetCheck(STN_ENABLE);
	_isCalculate = false;
	_isRandom = false;

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
	int width = _N + 2, height = _N + 2;
	_points.clear();
	std::vector<bool> row(width, false);
	_points.resize(height, row);
	int countTrue = 0;
	int countTrueMust = width * height*_percent / 100;
	while (countTrue < countTrueMust)
	{
		int i = rand() % height;
		int j = rand() % width;
		_points[i][j] = true;
		countTrue++;
	}
}

void CMonteCarloDlg::OnBnClickedRun()
{
	UpdateData(TRUE);
	InitializeModel();
	SetTimer(1, 20, NULL);
	_isCalculate = true;
	_pointsGraphEnergy.clear();
	_pointsGraphC.clear();
	_drawerGraph.RedrawWindow();
	_drawerGraphC.RedrawWindow();
	hThreadCalculate = CreateThread(
		NULL,		// дескриптор защиты
		0,			// начальный размер стека ( Если это значение нулевое, новый поток использует по умолчанию размер стека исполняемой программы)
		(LPTHREAD_START_ROUTINE)ThreadCalculate,	 // функция потока
		this,		// параметр потока 
		0,			//oпции создания(здесь можно отложить запуск выполнения потока. Для запуска потока сразу же, передаём 0.)
		&pdwThreadCalculate);// идентификатор потока (указатель на переменную, куда будет сохранён идентификатор потока)
}

int CMonteCarloDlg::E()
{
	int s = 0;
#pragma omp parallel for reduction(+:s)
	for (int i = 1; i < _N + 1; ++i)

		for (int j = 1; j < _N + 1; ++j)
		{
			s += spin_at(i, j) * (spin_at(i - 1, j) + spin_at(i, j - 1));
		}
	return -s * _J;
}
int CMonteCarloDlg::spin_at(size_t i, size_t j)
{
	return _points[i][j] ? 1 : -1;
}

void CMonteCarloDlg::ensure_periodic()
{
#pragma omp parallel for
	for (int i = 0; i < _N + 2; ++i)
	{
		_points[i][0] = _points[i][_N];
		_points[i][_N + 1] = _points[i][1];
		_points[0][i] = _points[_N][i];
		_points[_N + 1][i] = _points[1][i];
	}
}

void CMonteCarloDlg::ensure_periodic(size_t i, size_t j)
{
	_points[i][0] = _points[i][_N];
	_points[i][_N + 1] = _points[i][1];
	_points[0][i] = _points[_N][i];
	_points[_N + 1][i] = _points[1][i];
}


void CMonteCarloDlg::next_linear(size_t n)
{
#pragma omp parallel for
	for (int l = 0; l < n * n; ++l)
	{
		size_t i = (rand() % (n - 2)) + 2;
		size_t j = (rand() % (n - 2)) + 2;

		if (_isRandom)
		{
			size_t i2, j2;

			i2 = (rand() % (n - 2)) + 2;
			j2 = (rand() % (n - 2)) + 2;

			if (_points[i][j] == _points[i2][j2]) continue;
			int energyBegin = E();
			const bool ij = _points[i][j];
			const bool i2j2 = _points[i2][j2];

			_points[i][j] = _points[i2][j2];
			_points[i2][j2] = ij;
			int energyEnd = E();

			if ((energyEnd - energyBegin) > 0)
			{
				if ((double)rand() / RAND_MAX > exp(-(energyEnd - energyBegin) / k / _T))
				{
					_points[i][j] = ij;
					_points[i2][j2] = i2j2;
				}
			}

		}
		{
			int horizontalRnd, verticRnd;
			horizontalRnd = rand() % 2 == 1 ? 1 : -1;
			verticRnd = rand() % 2 == 1 ? 1 : -1;
			if (_points[i][j] == _points[i + verticRnd][j + horizontalRnd])
			{
				continue;
			}
			int energyBegin = E();

			const bool ij = _points[i][j];
			const bool i2j2 = _points[i + verticRnd][j + horizontalRnd];

			_points[i][j] = _points[i + verticRnd][j + horizontalRnd];
			_points[i + verticRnd][j + horizontalRnd] = ij;

			int energyEnd = E();


			if ((energyEnd - energyBegin) > 0)
			{
				if ((double)rand() / RAND_MAX > exp(-(energyEnd - energyBegin) / k / _T))
				{
					_points[i][j] = ij;
					_points[i + verticRnd][j + horizontalRnd] = i2j2;
				}
			}
		}
		ensure_periodic(i, j);
	}
}

void CMonteCarloDlg::next()
{
	const size_t n = _N;

	next_linear(n);
}

void CMonteCarloDlg::OnTimer(UINT_PTR nIDEvent)
{
	_drawerModel.RedrawWindow();
	CDialogEx::OnTimer(nIDEvent);
}

DWORD WINAPI ThreadCalculate(PVOID param)
{
	CMonteCarloDlg *dlg = (CMonteCarloDlg*)param;
	double stepCF = (double)(1.2 / dlg->_count_points);
	double cf = 0.5 + stepCF;
	
	while (dlg->_isCalculate && cf < 1.5)
	{
		int zz = 0;
		dlg->_T = cf * dlg->Tc();
		double ea = 0, e2a = 0;
		int split = dlg->_split;
		for (int mksh = 0; mksh < dlg->_n; mksh++)
		{
			EnterCriticalSection(&cs);
			dlg->next();
			LeaveCriticalSection(&cs);			
			if (split > 0)
			{
				split--;
				mksh--;
			}
			else
			{
				double e = std::fabs((double)((dlg->E()) / (dlg->_N*dlg->_N)));
				ea += e;
				e2a += e * e;
			}
			zz++;
		}

		ea /= (double)dlg->_n;
		e2a /= (double)dlg->_n;
		PointF pt(cf, ea);
		dlg->_pointsGraphEnergy.push_back(pt);
		pt.Y = std::fabs(e2a - ea * ea) / (dlg->k*dlg->_T) / (dlg->k*dlg->_T)/ (dlg->_N*dlg->_N);
		dlg->_pointsGraphC.push_back(pt);
		cf += stepCF;
		dlg->_drawerGraph.RedrawWindow();
		dlg->_drawerGraphC.RedrawWindow();

	}
	
	return 0;
}


void CMonteCarloDlg::OnBnClickedStop()
{
	_isCalculate = false;
}


void CMonteCarloDlg::OnBnClickedRadio1()
{
	_isRandom = false;
}


void CMonteCarloDlg::OnBnClickedRadio2()
{
	_isRandom = true;
}
