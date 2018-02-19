// easyViewerView.h : interface of the CEasyViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EASYVIEWERVIEW_H__4F543CAE_37B9_44C8_862A_14532A0BCBD7__INCLUDED_)
#define AFX_EASYVIEWERVIEW_H__4F543CAE_37B9_44C8_862A_14532A0BCBD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEasyViewerView : public CView
{
protected: // create from serialization only
	CEasyViewerView();
	CClientDC *m_pDC;
	HGLRC m_hRC;
	DECLARE_DYNCREATE(CEasyViewerView)
	void applyMatrices();

// Attributes
public:
	CEasyViewerDoc* GetDocument();
	vec3d<GLfloat> scr23d(GLint, GLint);
	mat4x4<GLfloat> get_rotation(vec3d<GLfloat> cc, vec3d<GLfloat> cldown);
	int yPre, yNow;
	int xPre, xNow;
	int zPre, zNow;	
	bool IsMousePress;
	bool isWireFrame;
	int width, height;

	// Mouse 
	BOOL m_LeftButtonDown;
	BOOL m_RightButtonDown;
	CPoint m_LeftDownPos;
	CPoint m_RightDownPos;
	HCURSOR m_CursorRotation;

	float m_xRotation[2];
	float m_yRotation[2];
	float m_zRotation[2];
	BOOL m_xyRotation;

	float m_xTranslation[2], m_xTranslationAll;
	float m_yTranslation[2], m_yTranslationAll;
	float m_zTranslation[2], m_zTranslationAll;

	float m_xScaling[2], m_xScalingAll;
	float m_yScaling[2], m_yScalingAll;
	float m_zScaling[2], m_zScalingAll;

	float m_SpeedTranslation;
	float m_SpeedRotation;

	// Colors
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	// Animation
	float m_StepRotationX;
	float m_StepRotationY;
	float m_StepRotationZ;
	void InitGeometry();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ConvertToGLCoord(float &x, float &y, float &z);
	void OnExportEps();
	// OpenGL specific
	BOOL CreateViewGLContext(HDC hDC);
	BOOL SetWindowPixelFormat(HDC hDC);
	HGLRC m_hGLContext;
	int m_GLPixelIndex;
	
	void onWire();
	virtual ~CEasyViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEasyViewerView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void Onrotate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in easyViewerView.cpp
inline CEasyViewerDoc* CEasyViewerView::GetDocument()
   { return (CEasyViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYVIEWERVIEW_H__4F543CAE_37B9_44C8_862A_14532A0BCBD7__INCLUDED_)
