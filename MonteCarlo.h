
// MonteCarlo.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMonteCarloApp:
// Сведения о реализации этого класса: MonteCarlo.cpp
//

class CMonteCarloApp : public CWinApp
{
public:
	CMonteCarloApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMonteCarloApp theApp;
