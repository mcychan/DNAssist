// MemDC.h: interface for the CMemDC class.
//
// Author: Ovidiu Cucu
// Homepage: http://www.codexpert.ro/
// Weblog: http://codexpert.ro/blog/

#pragma once


class CMemoryDC : public CDC  
{
   DECLARE_DYNAMIC(CMemoryDC)

   int m_nSavedDC;
   CBitmap m_bitmap;
public:
	CMemoryDC(CDC* pDC, int nWidth, int nHeight);
	CMemoryDC(CDC* pDC, CBitmap* pBitmap);
	~CMemoryDC();
};

inline CMemoryDC::CMemoryDC(CDC* pDC, int nWidth, int nHeight)
{
   ASSERT_VALID(pDC);
   VERIFY(CreateCompatibleDC(pDC));

   m_nSavedDC = SaveDC();
   VERIFY(m_bitmap.CreateCompatibleBitmap(pDC, nWidth, nHeight));
   SelectObject(&m_bitmap);
}

inline CMemoryDC::CMemoryDC(CDC* pDC, CBitmap* pBitmap)
{
   ASSERT_VALID(pDC);
   ASSERT_VALID(pBitmap);

   VERIFY(CreateCompatibleDC(pDC));
   SelectObject(pBitmap);
}

inline CMemoryDC::~CMemoryDC()
{
   RestoreDC(m_nSavedDC);
   if(NULL != m_bitmap.GetSafeHandle())
      m_bitmap.DeleteObject();
}

