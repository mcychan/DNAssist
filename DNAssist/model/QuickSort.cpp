#include "QuickSort.h"

CQuickSort::CQuickSort()
{
}

CQuickSort::~CQuickSort()
{
}

void CQuickSort::QuickSort(int* array, int left, int right)
{
	if(right > left) {
		int v = array[right];
		int i = left-1;
		int j = right;
		for(;;) {
			while(array[++i] < v);
			while (--j >= left && array[j] > v);
			if(i >= j)
				break;
			Swap(array, i, j);
		}
		Swap(array, i, right);
		QuickSort(array, left, i-1);
		QuickSort(array, i+1, right);
	}
}

void CQuickSort::IndirectQuickSort(int* indexes, int* array, int left, int right)
{
	if(right > left) {
		int v = array[indexes[right]];
		int i = left-1;
		int j = right;
		for(;;) {
			while(array[indexes[++i]] < v);
			while ( --j >= left && array[indexes[j]] > v);
			if(i >= j)
				break;
			IndirectSwap(indexes, i, j);
		}
		IndirectSwap(indexes, i, right);
		IndirectQuickSort(indexes, array, left, i-1);
		IndirectQuickSort(indexes, array, i+1, right);
	}
}


void CQuickSort::Swap(int* array, int leftelement, int rightelement)
{
	int temp = array[leftelement];
	array[leftelement] = array[rightelement];
	array[rightelement] = temp;
}

void CQuickSort::IndirectSwap(int* indexes, int leftelement, int rightelement)
{
	int temp = indexes[leftelement];
	indexes[leftelement] = indexes[rightelement];
	indexes[rightelement] = temp;
}
