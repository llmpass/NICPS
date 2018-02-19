#if !defined(AFX_VERTCORDLG_H__EB4A55D5_6B2E_4AF9_96E5_5E99B15E72F9__INCLUDED_)
#define AFX_VERTCORDLG_H__EB4A55D5_6B2E_4AF9_96E5_5E99B15E72F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VertCorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVertCorDlg dialog

class CVertCorDlg : public CDialog
{
// Construction
public:
	CVertCorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVertCorDlg)
	enum { IDD = IDD_VertCorDlg };
	CComboBox	m_SrcTgtSel;
	int		m_vertInd;
	int     SrcOrTgt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVertCorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVertCorDlg)
	afx_msg void OnSelchangeMSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERTCORDLG_H__EB4A55D5_6B2E_4AF9_96E5_5E99B15E72F9__INCLUDED_)
