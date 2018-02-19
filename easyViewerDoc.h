// easyViewerDoc.h : interface of the CEasyViewerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EASYVIEWERDOC_H__DC554446_145C_49A5_B9EA_43C64AE087DF__INCLUDED_)
#define AFX_EASYVIEWERDOC_H__DC554446_145C_49A5_B9EA_43C64AE087DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Lib3d/Base3d.h"
#include <vector>
using namespace std;

enum Manipulate{
	source,
	target,
	all
};

class CEasyViewerDoc : public CDocument
{
protected: // create from serialization only
	CEasyViewerDoc();
	DECLARE_DYNCREATE(CEasyViewerDoc)

// Attributes
public:
	int step;
	// The scene
	CSceneGraph3d m_SceneGraph;
	CString message;

	// Options
	BOOL m_AddWireframe;
	BOOL m_Smooth;
	float m_PolygonOffset;
	BOOL m_Light;
	GLenum m_Mode;
	BOOL m_Culling;

	bool isOpenFile, isReadLM;

	float modelview[16];
	float size;
	int GeoOp; 
	int w, h;
	int xNow, yNow, zNow;
	int xPre, yPre, zPre;
	double m_ratio;
	double m_displace[3];
	bool isTransparent;
	Manipulate manip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyViewerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RenderScene();
	void scale();
	void translate();
	void clamp(float *v);
	void rotate();
	virtual ~CEasyViewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEasyViewerDoc)
	afx_msg void OnPrinAlignment();
	afx_msg void OnGetPrinAxes();
	afx_msg void OnShowWireframe();
	afx_msg void OnFileOpen();
	afx_msg void OnShadingModel();
	afx_msg void OnFitting();
	afx_msg void OnColorCurvature();
	afx_msg void OnShowSource();
	afx_msg void OnShowTarget();
	afx_msg void OnFittingmult();
	afx_msg void OnFlipNormal();
	afx_msg void OnOrientFaces();
	afx_msg void OnReorientSource();
	afx_msg void OnReorientTarget();
	afx_msg void OnTransparent();
	afx_msg void OnReorientCurrent();
	afx_msg void OnShowDistances();
	afx_msg void OnAlignScale();
	afx_msg void OnShowNearestPnts();
	afx_msg void OnManiSource();
	afx_msg void OnManiTarget();
	afx_msg void OnManiAll();
	afx_msg void OnOutputCoord();
	afx_msg void OnOutputTarCoord();
	afx_msg void OnShowCors();
	afx_msg void OnShowCurrent();
	afx_msg void OnOutputSrcCoord();
	afx_msg void OnMirror();
	afx_msg void OnAndresenFit();
	afx_msg void OnPauly05Fit();
	afx_msg void OnPauly05MultFit();
	afx_msg void OnAddLandmark();
	afx_msg void OnShowLandmark();
	afx_msg void OnOutputSrcLandmarks();
	afx_msg void OnOutputTargetLandmarks();
	afx_msg void OnDelLandmark();
	afx_msg void OnLmDistances();
	afx_msg void OnVertCors();
	afx_msg void OnReorientAll();
	afx_msg void OnShow3rd();
	afx_msg void OnShow4th();
	afx_msg void OnPrDis();
	afx_msg void OnBend();
	afx_msg void OnShowTexture();
	afx_msg void OnOutputSrcCenters();
	afx_msg void OnShow5th();
	afx_msg void OnShow6th();
	afx_msg void OnWeightedFitting();
	afx_msg void OnWeightedFitting10();
	afx_msg void OnShow7th();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYVIEWERDOC_H__DC554446_145C_49A5_B9EA_43C64AE087DF__INCLUDED_)
