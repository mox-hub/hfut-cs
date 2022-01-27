// Polygon_Conversion.h : main header file for the POLYGON_CONVERSION application
//

#if !defined(AFX_POLYGON_CONVERSION_H__95894509_6169_411E_A470_4F3C92F4E695__INCLUDED_)
#define AFX_POLYGON_CONVERSION_H__95894509_6169_411E_A470_4F3C92F4E695__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPolygon_ConversionApp:
// See Polygon_Conversion.cpp for the implementation of this class
//

class CPolygon_ConversionApp : public CWinApp
{
public:
	CPolygon_ConversionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPolygon_ConversionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPolygon_ConversionApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POLYGON_CONVERSION_H__95894509_6169_411E_A470_4F3C92F4E695__INCLUDED_)
