#include "stdafx.h"
#include "PointList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPointList::CPointList()
{
}

CPointList::~CPointList()
{
	x.EmptyArray();
	y.EmptyArray();
}

void CPointList::AddPoint(int a, int b)
{
	x.AddInt(a);
	y.AddInt(b);
}


void CPointList::AddPoint(POINT* point)
{
	x.AddInt(point->x);
	y.AddInt(point->y);
}

void CPointList::AddPoint(POINT point)
{
	x.AddInt(point.x);
	y.AddInt(point.y);
}


size_t CPointList::GetXValue(int index)
{
	return x.GetIndex(index);
}


size_t CPointList::Get_YValue(int index)
{
	return y.GetIndex(index);
}

POINT CPointList::GetPoint(int index)
{
	POINT pt;
	pt.x = (LONG) x.GetIndex(index);
	pt.y = (LONG) y.GetIndex(index);
	return pt;
}

void CPointList::OverWritePoint(POINT point, int index)
{
	x.OverWriteEntry(point.x, index);
	y.OverWriteEntry(point.y, index);
}

int CPointList::GetCounter()
{
	return counter;
}

void CPointList::IncrementCounter()
{
	counter++;
}

void CPointList::AddConstant(int number)
{
	x.AddConstant(number);
	y.AddConstant(number);
}
