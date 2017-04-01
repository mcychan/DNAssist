#include "BubbleSort.h"

CBubbleSort::CBubbleSort()
{
	direction = ascending;
	start = 0;
	end = 0;
}

CBubbleSort::CBubbleSort(const CBubbleSort& passedobject)
{
	direction = passedobject.direction;
	start = passedobject.start;
	end = passedobject.end;
}

CBubbleSort::~CBubbleSort()
{
}

void CBubbleSort::Sort(int* array, int len, int begin, int stop)
{
	start = begin;
	if(!stop)
		end = len-1;

	if(direction == descending) {
		for(int x = start+1; x <= end; x++) {
			int innerloop = x;
			while(innerloop > start && array[innerloop] > array[innerloop-1]) {
				int temp = array[innerloop-1];
				array[innerloop-1] = array[innerloop];
				array[innerloop] = temp;
				innerloop--;
			}
		}
	}
	else if(direction == ascending) {
		for(int x = start+1; x <= end; x++) {
			int innerloop = x;
			while(innerloop > start && array[innerloop] < array[innerloop-1]) {
				int temp = array[innerloop-1];
				array[innerloop-1] = array[innerloop];
				array[innerloop] = temp;
				innerloop--;
			}
		}
	}
}

void CBubbleSort::SortIndirect(int* array, int len, int* result, int begin, int stop)
{
	start = begin;
	if(!stop)
		end = len-1;
	else
		end = stop;

	if(direction == descending) {
		for(int x = start+1; x <= end; x++) {
			int innerloop = x;
			while(innerloop > start && array[result[innerloop]] > array[result[innerloop-1]]) {
				int temp = result[innerloop-1];
				result[innerloop-1] = result[innerloop];
				result[innerloop] = temp;
				innerloop--;
			}
		}
	}
	else if(direction == ascending) {
		for(int x = start+1; x <= end; x++) {
			int innerloop = x;
			while(innerloop > start && array[result[innerloop]] < array[result[innerloop-1]]) {
				int temp = result[innerloop-1];
				result[innerloop-1] = result[innerloop];
				result[innerloop] = temp;
				innerloop--;
			}
		}
	}
}
