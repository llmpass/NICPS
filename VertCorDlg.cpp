// VertCorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "easyViewer.h"
#include "VertCorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVertCorDlg dialog


CVertCorDlg::CVertCorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVertCorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVertCorDlg)
	m_vertInd = 0;
	SrcOrTgt = 0;
	//}}AFX_DATA_INIT
}


void CVertCorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVertCorDlg)
	DDX_Control(pDX, IDC_MSelect, m_SrcTgtSel);
	DDX_Text(pDX, IDC_VertInd, m_vertInd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVertCorDlg, CDialog)
	//{{AFX_MSG_MAP(CVertCorDlg)
	ON_CBN_SELCHANGE(IDC_MSelect, OnSelchangeMSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVertCorDlg message handlers

void CVertCorDlg::OnSelchangeMSelect() {
  SrcOrTgt = m_SrcTgtSel.GetCurSel();              
}
