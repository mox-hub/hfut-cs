// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Experiment_Frame_One.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog


CSettingDlg::CSettingDlg(int      iPointSize,  
						 COLORREF PointColor,
						 int      iActiveAlgorithm ,
						 BOOL     bGridOn,
						 CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_Point_Size = iPointSize;
	m_bGridOn = bGridOn;
	//}}AFX_DATA_INIT
	m_iActive_Algorithm = iActiveAlgorithm;
	m_Point_Color       = PointColor;

}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CString sColorBuffer;
	CDialog::DoDataExchange(pDX);

	sColorBuffer.Format("(%03d, %03d, %03d)",GetRValue(this->m_Point_Color),
			                                 GetGValue(this->m_Point_Color),
											 GetBValue(this->m_Point_Color));
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_ALGORITHM_COMBO, m_Algorithms);
	DDX_Text(pDX, IDC_PSIZE_EDIT, m_Point_Size);
	DDV_MinMaxInt(pDX, m_Point_Size, 5, 20);
	DDX_Check(pDX, IDC_GRID_CHECK, m_bGridOn);
	DDX_Text(pDX, IDC_PCOLOR_EDIT, sColorBuffer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_BN_CLICKED(IDC_PCOLOR_BUTTON, OnPcolorButton)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_ALGORITHM_COMBO, OnSelchangeAlgorithmCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

void CSettingDlg::OnPcolorButton() 
{
	CColorDialog ColorDialog(m_Point_Color);
	if ( ColorDialog.DoModal() == IDOK )
	{
		m_Point_Color = ColorDialog.GetColor();
		UpdateData(FALSE);
	}
	return;	
}


BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Algorithms.SetCurSel(this->m_iActive_Algorithm);
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnSelchangeAlgorithmCombo() 
{
	this->m_iActive_Algorithm = m_Algorithms.GetCurSel();	
}
