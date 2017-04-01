////////////////////////////////////////////////////////////////////////  
////////
// by Max Poliashenko - rewritten version of class proposed by Iuri Applonio
// This class will sort a List control by a column of text, integer, float or
// date/time type. It could be easily extended for other data types.
// February 11, 1998
//

class CSortClass
{
public:
        enum EDataType {dtNULL, dtINT, dtSTRING, dtDATETIME, dtDEC ,dtDATE, dtFILESIZE};

        CSortClass(CListCtrl * _pWnd, const int _iCol);
        virtual ~CSortClass();
        void Sort(bool bAsc, EDataType _dtype);

protected:
        CListCtrl * pWnd;

        static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM
                lParamSort);//CListView.SortItemsµÄ»Øµôº¯Êý

        struct CSortItem
        {
                CSortItem(const DWORD_PTR _dw, const CString &_txt);
                DWORD_PTR dw;
                CString txt;
        };
};
