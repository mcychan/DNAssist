#pragma once

class CBubbleSort
{
public:
	CBubbleSort();
	CBubbleSort(const CBubbleSort&);
	~CBubbleSort();
	enum SortDirection { ascending, descending };
	void Sort(int* intarray, int length, int start =0, int end = 0);
	void SortIndirect(int* array, int len, int* result, int begin = 0, int stop = 0);
	// Inlines
	inline void SetSortDirection(SortDirection sortdirection)
	{
		direction = sortdirection;
	}
	inline SortDirection GetSortDirection() const
	{
		return direction;
	}

protected:
	int start; // position in array to start the sorting
	int end;   // position in array to stop the searching
	SortDirection direction;
};

