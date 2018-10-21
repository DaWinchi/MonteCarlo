#include "stdafx.h"
#include "Drawer2D.h"
#include <math.h>

using namespace Gdiplus;
ULONG_PTR token1;
Drawer2D::Drawer2D()
{
	Status s;
	GdiplusStartupInput input;
	s = GdiplusStartup(&token1, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"ERROR!!!", L"Îøèáêà", MB_ICONERROR);
	}
}


Drawer2D::~Drawer2D()
{
	GdiplusShutdown(token1);
}


void Drawer2D::DrawItem(LPDRAWITEMSTRUCT RECT)
{
	Graphics gr(RECT->hDC);
	if (_points != nullptr && !_points->empty())
	{
		size_t width = _points[0][0].size();
		size_t height = _points->size();
		xmin = 0;
		xmax = width;
		ymin = 0;
		ymax = height;
		Bitmap bmpBuffer(width, height);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Color color;
				_points[0][i][j] > 0 ?
					color = Color::MakeARGB(255, 255, 255, 255)
					: color = Color::MakeARGB(255, 0, 0, 0);
				bmpBuffer.SetPixel(j, height - 1 - i, color);
			}
		}
		Rect rect(0, 0, RECT->rcItem.right, RECT->rcItem.bottom);
		gr.DrawImage(&bmpBuffer, rect);
	}
}


REAL Drawer2D::X(LPDRAWITEMSTRUCT RECT, float x)
{
	return (REAL)(RECT->rcItem.right) / (xmax - xmin)*((x)-xmin);
}

REAL Drawer2D::Y(LPDRAWITEMSTRUCT RECT, float y)
{
	return -(REAL)(RECT->rcItem.bottom) / (ymax - ymin)*((y)-ymax);
}

REAL Drawer2D::Width(LPDRAWITEMSTRUCT RECT, float width)
{
	return (REAL)(RECT->rcItem.right) / (xmax - xmin)*width;
}

REAL Drawer2D::Height(LPDRAWITEMSTRUCT RECT, float height)
{
	return (REAL)(RECT->rcItem.bottom) / (ymax - ymin)*height;
}
