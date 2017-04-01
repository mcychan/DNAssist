// CSortClass
#include "stdafx.h"
#include "SortClass.h"

CSortClass::CSortClass(CListCtrl * _pWnd, const int _iCol)
{
        pWnd = _pWnd;

        ASSERT(pWnd);
        int max = pWnd->GetItemCount();
        DWORD_PTR dw;
        CString txt;

        // replace Item data with pointer to CSortItem structure
        for (int t = 0; t < max; t++)
        {
                dw = pWnd->GetItemData(t); // save current data to restore it later
                txt = pWnd->GetItemText(t, _iCol);
                pWnd->SetItemData(t, (DWORD_PTR) new CSortItem(dw, txt));
        }
}

CSortClass::~CSortClass()
{
        ASSERT(pWnd);
        int max = pWnd->GetItemCount();
        CSortItem * pItem;
        for (int t = 0; t < max; t++)
        {
                pItem = (CSortItem *) pWnd->GetItemData(t);
                ASSERT(pItem);
                pWnd->SetItemData(t, pItem->dw);
                delete pItem;
        }
}

void CSortClass::Sort(bool _bAsc, EDataType _dtype)
{
        long lParamSort = _dtype;

        // if lParamSort positive - ascending sort order, negative - descending
        if (!_bAsc)
                lParamSort *= -1;

        pWnd->SortItems(Compare, lParamSort);//´Ëº¯Êý»Øµ÷Compareº¯Êý²¢´«µÝlParamSort¸øCompareµÄlParamSort²ÎÊý
}

int CALLBACK CSortClass::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM
                                                                 lParamSort)
{
        CSortItem * item1 = (CSortItem *) lParam1;
        CSortItem * item2 = (CSortItem *) lParam2;
        ASSERT(item1 && item2);

        // restore data type and sort order from lParamSort
        // if lParamSort positive - ascending sort order, negative - descending
        short   sOrder = lParamSort < 0 ? -1 : 1;
        EDataType dType  = (EDataType) (lParamSort * sOrder); // get rid of sign

        // declare typed buffers
        COleDateTime t1, t2;

        switch (dType)
        {
        case  EDataType::dtINT:
                return (_ttol(item1->txt) - _ttol(item2->txt))*sOrder;
        case  EDataType::dtDEC:
                return (_ttoi(item1->txt) < _ttoi(item2->txt) ? -1 : 1)*sOrder;
        case  EDataType::dtDATETIME:
                if (t1.ParseDateTime(item1->txt) && t2.ParseDateTime(item2->txt))
                        return (t1 < t2 ? -1 : 1 )*sOrder;
                else
                        return 0;
        case  EDataType::dtSTRING:
                return item1->txt.CompareNoCase(item2->txt)*sOrder;
        case  EDataType::dtDATE:
                {
                        if(item1->txt.Find(_T("."))==-1)//Èç¹ûÃ»ÓÐ·¢ÏÖ.ËµÃ÷´ËÎÄ¼þÃ»ÓÐ´óÐ¡
                        {
                                return -1*sOrder;
                        }
                        if(item2->txt.Find(_T("."))==-1)
                        {
                                return 1*sOrder;
                        }
                        CString intItem1=item1->txt.Left(item1->txt.Find(_T(".")));//½ØÈ¡.Ç°ÃæµÄÊý×Ö²¢ÔÚÏÂÃæ½øÐÐ±È½Ï
                        CString intItem2=item2->txt.Left(item2->txt.Find(_T(".")));
                        if (item1->txt.Right(1)==item2->txt.Right(1))//Èç¹ûµ¥Î»ÏàµÈ
                        {
                                return (_ttol(intItem1) - _ttol(intItem2))*sOrder;
                        }
                        else
                                {
                                        if (item1->txt.Right(1)==_T("K"))//Èç¹ûµ¥Î»ÊÇKÄÇÃ´item1¿Ï¶¨Ð¡ÓÚitem2
                                        {
                                                return -1*sOrder;
                                        }else if(item1->txt.Right(1)==_T("M"))
                                                {
                                                        return (item2->txt.Right(1)==_T("G")?-1 : 1)*sOrder;
                                                }else
                                                        {
                                                                return 1*sOrder;
                                                        }

                                }
                }
		case  EDataType::dtFILESIZE:
        {
			if (item1->txt.Right(2)==_T("KB") && item2->txt.Right(2)==_T("MB"))
               return -1*sOrder;
            if(item1->txt.Right(2)==_T("MB") && item2->txt.Right(2)==_T("KB"))
				return sOrder;

			CString intItem1=item1->txt.Left(item1->txt.GetLength()-2);
            CString intItem2=item2->txt.Left(item2->txt.GetLength()-2);
			return (_ttol(intItem1) - _ttol(intItem2))*sOrder;
		}
        default:
                ASSERT("Error: attempt to sort a column without type.");
                return 0;
        }
}


CSortClass::CSortItem::CSortItem(const DWORD_PTR _dw, const CString & _txt)
{
        dw  = _dw;
        txt = _txt;
}
