// easyViewerDoc.cpp : implementation of the CEasyViewerDoc class
//

#include "stdafx.h"
#include "easyViewer.h"
#include "MainFrm.h"
#include "easyViewerDoc.h"
#include "VertCorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern float rot[3], eye[3];

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerDoc

IMPLEMENT_DYNCREATE(CEasyViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CEasyViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CEasyViewerDoc)
	ON_COMMAND(ID_PrinAlignment, OnPrinAlignment)
	ON_COMMAND(ID_GetPrinAxes, OnGetPrinAxes)
	ON_COMMAND(ID_ShowWireframe, OnShowWireframe)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_ShadingModel, OnShadingModel)
	ON_COMMAND(ID_Fitting, OnFitting)
	ON_COMMAND(ID_ColorCurvature, OnColorCurvature)
	ON_COMMAND(ID_ShowSource, OnShowSource)
	ON_COMMAND(ID_ShowTarget, OnShowTarget)
	ON_COMMAND(ID_Fitting_mult, OnFittingmult)
	ON_COMMAND(ID_FlipNormal, OnFlipNormal)
	ON_COMMAND(ID_OrientFaces, OnOrientFaces)
	ON_COMMAND(ID_ReorientSource, OnReorientSource)
	ON_COMMAND(ID_ReorientTarget, OnReorientTarget)
	ON_COMMAND(ID_Transparent, OnTransparent)
	ON_COMMAND(ID_ReorientCurrent, OnReorientCurrent)
	ON_COMMAND(ID_ShowDistances, OnShowDistances)
	ON_COMMAND(ID_AlignScale, OnAlignScale)
	ON_COMMAND(ID_ShowNearestPnts, OnShowNearestPnts)
	ON_COMMAND(ID_ManiSource, OnManiSource)
	ON_COMMAND(ID_ManiTarget, OnManiTarget)
	ON_COMMAND(ID_ManiAll, OnManiAll)
	ON_COMMAND(ID_OutputCoord, OnOutputCoord)
	ON_COMMAND(ID_OutputTarCoord, OnOutputTarCoord)
	ON_COMMAND(ID_ShowCors, OnShowCors)
	ON_COMMAND(ID_ShowCurrent, OnShowCurrent)
	ON_COMMAND(ID_OutputSrcCoord, OnOutputSrcCoord)
	ON_COMMAND(ID_Mirror, OnMirror)
	ON_COMMAND(ID_AndresenFit, OnAndresenFit)
	ON_COMMAND(ID_Pauly05Fit, OnPauly05Fit)
	ON_COMMAND(ID_Pauly05MultFit, OnPauly05MultFit)
	ON_COMMAND(ID_AddLandmark, OnAddLandmark)
	ON_COMMAND(ID_ShowLandmark, OnShowLandmark)
	ON_COMMAND(ID_OutputSrcLandmarks, OnOutputSrcLandmarks)
	ON_COMMAND(ID_OutputTargetLandmarks, OnOutputTargetLandmarks)
	ON_COMMAND(ID_DelLandmark, OnDelLandmark)
	ON_COMMAND(ID_LmDistances, OnLmDistances)
	ON_COMMAND(ID_VertCors, OnVertCors)
	ON_COMMAND(ID_ReorientAll, OnReorientAll)
	ON_COMMAND(ID_Show3rd, OnShow3rd)
	ON_COMMAND(ID_Show4th, OnShow4th)
	ON_COMMAND(ID_PrDis, OnPrDis)
	ON_COMMAND(ID_Bend, OnBend)
	ON_COMMAND(ID_ShowTexture, OnShowTexture)
	ON_COMMAND(ID_OutputSrcCenters, OnOutputSrcCenters)
	ON_COMMAND(ID_Show5th, OnShow5th)
	ON_COMMAND(ID_Show6th, OnShow6th)
	ON_COMMAND(ID_WeightedFitting, OnWeightedFitting)
	ON_COMMAND(ID_WeightedFitting10, OnWeightedFitting10)
	ON_COMMAND(ID_Show7th, OnShow7th)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerDoc construction/destruction

CEasyViewerDoc::CEasyViewerDoc() {
	// TODO: add one-time construction code here
	m_AddWireframe = 0;
	m_Smooth = 1;
	m_PolygonOffset = -1.0f;
	m_Light = TRUE;
	m_Mode = GL_FILL;
	m_Culling = TRUE;
	isOpenFile = isReadLM = false;
	manip = all;
	isTransparent = false;
	m_SceneGraph.rotM[0] = m_SceneGraph.rotM[1] = 
		m_SceneGraph.rotMAll = m_SceneGraph.rotMCurrent = Identity<GLfloat>();
}

CEasyViewerDoc::~CEasyViewerDoc() {
}

BOOL CEasyViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CEasyViewerDoc serialization

void CEasyViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerDoc diagnostics

#ifdef _DEBUG
void CEasyViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEasyViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerDoc commands

void CEasyViewerDoc::rotate() {
	GeoOp = 0;
	m_ratio = 1;
	/* rotates screen */
    rot[0] += (yPre - yNow) / 2;
    rot[1] -= (xPre - xNow) / 2;
    clamp(rot);
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::clamp(float *v)
{
	int i;
    for (i = 0; i < 3; i ++)
        if (v[i] > 360 || v[i] < -360)
            v[i] = 0;
}

BOOL CEasyViewerDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{    
	// Extension-based checking
	//CString file = lpszPathName;

	
	return TRUE;
}

void CEasyViewerDoc::translate() {
 	GeoOp = 1;
	eye[0] -= (xPre - xNow) * 0.001f;
    eye[1] += (yNow - yPre) * 0.001f;
    clamp(rot);
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::scale() {
	GeoOp = 2;
	m_ratio = (1 - (yNow-yPre) / (double)400);
	size *= m_ratio;
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
	m_ratio = 1;
}

void CEasyViewerDoc::RenderScene() {
	// Main drawing
  if (isTransparent) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
	else glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK,m_Mode);
	if(m_Light)
		::glEnable(GL_LIGHTING);
	else
		::glDisable(GL_LIGHTING);
	m_SceneGraph.glDraw();

	// Add wireframe (no light, and line mode)
	if(m_AddWireframe)
	{
		// Set state
		::glDisable(GL_LIGHTING);
		::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		::glEnable(GL_POLYGON_OFFSET_LINE);
		::glPolygonOffset(m_PolygonOffset,-1.0f);

		// Draw again...
		m_SceneGraph.glDraw(TYPE_MESH3D);

		// Restore light and mode
		::glDisable(GL_POLYGON_OFFSET_LINE);
		::glEnable(GL_LIGHTING);
	}
}

void CEasyViewerDoc::OnPrinAlignment() {
	m_SceneGraph.PrinAlignment();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnGetPrinAxes() {
	m_SceneGraph.TogglePrinAxis(); 
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowWireframe() {
	m_AddWireframe = !m_AddWireframe;
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnFileOpen() {
	DWORD dwFlags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR;  
	CString FileFilter = 
		"Vrml2.0 files(*.wrl)|*.wrl| t files(*.t)|*.t| landmark files(*.lm)|*.lm||";   
	CFileDialog dlg(TRUE, NULL, NULL, dwFlags, FileFilter);  
	CString lpszPathName;
	if (dlg.DoModal() == IDOK) lpszPathName = dlg.GetPathName();
	// Extension
	CString extension = lpszPathName;
	extension = extension.Right(4);
	CString name = lpszPathName;
	int i=0;
	while(i!=-1) {
		i=name.Find("\\");
		name.Delete(0,i+1);
  }
	i=name.ReverseFind('.');
	name = name.Left(i);
	extension.MakeLower();

	// Path "c:\path\file.wrl" -> c:\path
	CString path = lpszPathName;
	path = path.Left(path.ReverseFind('\\'));

	// CDocument
	SetCurrentDirectory(path);
	// Start reading VRML file
	if(extension == ".wrl") {
		TRACE("wrl type\n");
		// Parser VRML 2.0
		CParserVrml parser;
		if(parser.Run((char *)lpszPathName.GetBuffer(0),&m_SceneGraph)){
			m_SceneGraph.BuildAdjacency();
			m_SceneGraph.CalculateNormalPerFace();
			m_SceneGraph.CalculateNormalPerVertex();
			m_SceneGraph.SetColorBinding(COLOR_PER_VERTEX);
			m_SceneGraph.SetNormalBinding(NORMAL_PER_VERTEX);
			glShadeModel(GL_SMOOTH);
			m_SceneGraph.GetPrinAxes();
			isOpenFile = true;
		}
		int srctar = m_SceneGraph.NbObject()-1;
		m_SceneGraph.setMeshName(srctar, name);
		m_SceneGraph.PrepareOriMesh();
	}

	extension = lpszPathName;
	extension = extension.Right(3);
	extension.MakeLower();
	if(extension==".lm") {
		ifstream ifs(lpszPathName);
		assert(ifs);
		// landmarks on the source mesh or target mesh?
		int srctar;
		ifs >> srctar;
		int nLM;
		ifs >> nLM;
		float (*lm)[3]=new float[nLM][3];
		int* lmInd=new int[nLM];
		float x,y,z;
		// read lines
		m_SceneGraph.ClearLandmark(srctar);
		for (int i=0; i<nLM; ++i) {
		  ifs>>x>>y>>z;
		  ifs>>lmInd[i];
		  m_SceneGraph.AddLandmark(lmInd[i],srctar);
		}
		isReadLM = true;
		return;
	}

	extension = lpszPathName;
	extension = extension.Right(2);
	extension.MakeLower();
	// Reading t file...
	if(extension == ".t") {		
		ifstream ifs(lpszPathName);
		assert(ifs);
		//The number of Vertexes and Triangles
		int nVer, nTri;
		ifs >> nTri >> nVer;
		// Add mesh
		CMesh3d *pMesh = new CMesh3d;
		m_SceneGraph.Add(pMesh);
		int (*tris)[3] = new int [nTri][3];
		for (int i = 0; i < nTri; ++i) ifs >> tris[i][0] >> tris[i][1] >> tris[i][2];
		float x, y, z;
		for (i = 0; i < nVer; ++i) {
			ifs >> x >> y >> z;
			pMesh->AddVertex(new CVertex3d(x, y, z));
		}
		for (i = 0; i < nTri; ++i) 
			pMesh->AddFace(new CFace3d(pMesh->m_ArrayVertex[tris[i][0]], 
			pMesh->m_ArrayVertex[tris[i][1]], 
			pMesh->m_ArrayVertex[tris[i][2]]));
	
		m_SceneGraph.BuildAdjacency();
		m_SceneGraph.CalculateNormalPerFace();
		m_SceneGraph.CalculateNormalPerVertex();
		m_SceneGraph.SetColorBinding(COLOR_PER_VERTEX);
		m_SceneGraph.SetNormalBinding(NORMAL_PER_VERTEX);
		glShadeModel(GL_SMOOTH);
		m_SceneGraph.GetPrinAxes();
		isOpenFile = true;
		m_SceneGraph.PrepareOriMesh();
	}
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShadingModel() {
	m_Smooth = !m_Smooth;
	if(m_Smooth)
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_VERTEX);
		m_SceneGraph.SetColorBinding(COLOR_PER_VERTEX);
		::glShadeModel(GL_SMOOTH);
		m_SceneGraph.BuildAdjacency();
	}
	else
	{
		m_SceneGraph.SetNormalBinding(NORMAL_PER_FACE);
		m_SceneGraph.SetColorBinding(COLOR_PER_FACE);
		::glShadeModel(GL_FLAT);
		m_SceneGraph.BuildAdjacency();
	}
	CEasyViewerApp * p=(CEasyViewerApp *)AfxGetApp(); 
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnFitting() {
	m_SceneGraph.NonRigidFitting();
	CString sDisL1, sDisL2, sDisLM;
	sDisL1.Format("%8.6f", m_SceneGraph.dis);
	sDisL2.Format("%8.6f", m_SceneGraph.disL2);
	sDisLM.Format("%8.6f", m_SceneGraph.disLM);
	message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
	CString sTitle;
	sTitle.Format("%d", m_SceneGraph.fittingTimes);
	this->SetTitle(sTitle);
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnWeightedFitting() {
  m_SceneGraph.NonRigidFittingWeighted();
  CString sDisL1, sDisL2, sDisLM;
  sDisL1.Format("%8.6f", m_SceneGraph.dis);
  sDisL2.Format("%8.6f", m_SceneGraph.disL2);
  sDisLM.Format("%8.6f", m_SceneGraph.disLM);
  message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
  CString sTitle;
  sTitle.Format("%d", m_SceneGraph.fittingTimes);
  this->SetTitle(sTitle);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnWeightedFitting10() {
  m_SceneGraph.NonRigidFittingWeighted10();
  CString sDisL1, sDisL2, sDisLM;
  sDisL1.Format("%8.6f", m_SceneGraph.dis);
  sDisL2.Format("%8.6f", m_SceneGraph.disL2);
  sDisLM.Format("%8.6f", m_SceneGraph.disLM);
  message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
  CString sTitle;
  sTitle.Format("%d", m_SceneGraph.fittingTimes);
  this->SetTitle(sTitle);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	 
}

/************************************************************************/
/* IEEE Transactions on Medical Imaging 2000                            */
/************************************************************************/
void CEasyViewerDoc::OnAndresenFit() {
	m_SceneGraph.Andresen2000Fitting();
	CString sDisL1, sDisL2, sDisLM;
	sDisL1.Format("%8.6f", m_SceneGraph.dis);
	sDisL2.Format("%8.6f", m_SceneGraph.disL2);
	sDisLM.Format("%8.6f", m_SceneGraph.disLM);
	message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
	CString sTitle;
	sTitle.Format("%d", m_SceneGraph.fittingTimes);
	this->SetTitle(sTitle);
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

/************************************************************************/
/* Example-Based 3D Scan Completion                                     */
/* Symposium on Geometry Processing 2005                                */
/************************************************************************/
void CEasyViewerDoc::OnPauly05Fit() {
	m_SceneGraph.Pauly2005Fitting();
	CString sDisL1, sDisL2, sDisLM;
	sDisL1.Format("%8.6f", m_SceneGraph.dis);
	sDisL2.Format("%8.6f", m_SceneGraph.disL2);
	sDisLM.Format("%8.6f", m_SceneGraph.disLM);
	message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
	CString sTitle;
	sTitle.Format("%d", m_SceneGraph.fittingTimes);
	this->SetTitle(sTitle);
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnPauly05MultFit() {
	m_SceneGraph.Pauly2005FittingMult();
	CString sDisL1, sDisL2, sDisLM;
	sDisL1.Format("%8.6f", m_SceneGraph.dis);
	sDisL2.Format("%8.6f", m_SceneGraph.disL2);
	sDisLM.Format("%8.6f", m_SceneGraph.disLM);
	message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
	CString sTitle;
	sTitle.Format("%d", m_SceneGraph.fittingTimes);
	this->SetTitle(sTitle);
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnColorCurvature() {
	m_SceneGraph.ColorCurvatue();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowSource() {
	m_SceneGraph.ShowSource();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowTarget() {
	m_SceneGraph.ShowTarget();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnShowCurrent() 
{
	m_SceneGraph.ShowCurrent();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnShow3rd() {
  m_SceneGraph.ShowMesh(3);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnShow4th() {	
  m_SceneGraph.ShowMesh(4);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnShow5th() {
  m_SceneGraph.ShowMesh(5);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnShow6th() {
  m_SceneGraph.ShowMesh(6);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnShow7th() {
  m_SceneGraph.ShowMesh(7);
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}


void CEasyViewerDoc::OnFittingmult() {
	m_SceneGraph.NonRigidFittingM();
	CString sTitle;
	sTitle.Format("%d", m_SceneGraph.fittingTimes);
	this->SetTitle(sTitle);
	CString sDisL1, sDisL2, sDisLM;
	sDisL1.Format("%8.6f", m_SceneGraph.dis);
	sDisL2.Format("%8.6f", m_SceneGraph.disL2);
	sDisLM.Format("%8.6f", m_SceneGraph.disLM);
	message = "L1: "+sDisL1+" L2: "+sDisL2+" LMAX: "+sDisLM;
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnFlipNormal() {
	m_SceneGraph.FlipNormal();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnOrientFaces() {
  m_SceneGraph.OrientFaces();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnReorientSource() {
  m_SceneGraph.ReorientSource();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnReorientTarget() {
	m_SceneGraph.ReorientTarget();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnReorientCurrent() {
	m_SceneGraph.ReorientCurrent();
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnReorientAll() {
  m_SceneGraph.ReorientAll();
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnTransparent() {
	isTransparent = !isTransparent;	
	((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
  UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnPrDis() {
  m_SceneGraph.ComputePairDis();
  CString sPrDis, sTemp;
  sTemp.Format("%8.6f\n",m_SceneGraph.disPrM);
  sPrDis += "Max dis = "+sTemp;
  sTemp.Format("%8.6f\n",m_SceneGraph.disPr2);
  sPrDis += "L2 dis = "+sTemp;
  AfxMessageBox(sPrDis);
  UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnLmDistances() {
  m_SceneGraph.ComputeMarkDis();
  int nMark = m_SceneGraph.disMarks.size();
  CString sMarkDis, sTemp;
  ofstream ost("markDis.txt");
  float sumD=0;
  for (int i=0; i<nMark; ++i) {
    sTemp.Format("\n%d",i);
    sMarkDis += sTemp;
    ost<<i<<"  ";
    sTemp.Format("  %8.6f",m_SceneGraph.disMarks[i]);
    sumD += m_SceneGraph.disMarks[i];
    sMarkDis += sTemp;
    ost<<m_SceneGraph.disMarks[i]<<endl;
  }
  ost<<sumD/nMark<<endl;
  AfxMessageBox(sMarkDis);
  UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowDistances() {
  bool isComp = m_SceneGraph.ComputeDisAll();
  CString sDisL1, sDisL2, sDisLM, arSource, arTarget, arCurrent;
  if (!m_SceneGraph.isGenSrc) m_SceneGraph.GenSrcMesh();
  if (!m_SceneGraph.isGenTarget) m_SceneGraph.GenTargetMesh();
  sDisL1.Format("%8.6f", m_SceneGraph.disSTL1);
  sDisL2.Format("%8.6f", m_SceneGraph.disSTL2);
  sDisLM.Format("%8.6f", m_SceneGraph.disSTLM);
  arSource.Format("%8.6f", m_SceneGraph.avgAspectRatioSource);
  arTarget.Format("%8.6f", m_SceneGraph.avgAspectRatioTarget);
  arCurrent.Format("%8.6f", m_SceneGraph.avgAspectRatioCurrent);
  CString msg="The L1 dis between source and target is "+sDisL1+
    "\nThe L2 dis between source and target is "+sDisL2+
    "\nThe LM dis between source and target is "+sDisLM+
    "\nThe aspect ratio of source mesh is "+arSource+
    "\nThe aspect ratio of target mesh is "+arTarget+
    "\nThe aspect ratio of current mesh is "+arCurrent;
  if (isComp) AfxMessageBox(msg);
  UpdateAllViews(NULL);	
}

void CEasyViewerDoc::OnAlignScale() {
  m_SceneGraph.AlignScale();
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
  UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowNearestPnts() {
  m_SceneGraph.ToggleNearestPntsShow();
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
  UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowCors() {
  m_SceneGraph.ToggleCorrespondences();
  ((CMainFrame *)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0,message); 
  UpdateAllViews(NULL);
}


void CEasyViewerDoc::OnManiSource() {
	manip = source;
}

void CEasyViewerDoc::OnManiTarget() {
	manip = target;	
}

void CEasyViewerDoc::OnManiAll() {
	manip = all;	
}

void CEasyViewerDoc::OnOutputCoord() {
	m_SceneGraph.OnOutputCoord();
}

void CEasyViewerDoc::OnOutputTarCoord() {
	m_SceneGraph.OnOutputTarCoord();
}

void CEasyViewerDoc::OnOutputSrcCoord() {
	m_SceneGraph.OnOutputSrcCoord();
}

void CEasyViewerDoc::OnMirror() {
	m_SceneGraph.onMirror(manip);
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnAddLandmark() {
	m_SceneGraph.isAddingLandmarks = !m_SceneGraph.isAddingLandmarks;
	CMainFrame* pMainFrame;
  pMainFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd); 
	CMenu* cm = pMainFrame->GetMenu();
	CMenu* csm;
	if (cm!=NULL) csm = cm->GetSubMenu(6);
	if (m_SceneGraph.isAddingLandmarks) {
		AfxMessageBox("Now begin adding landmarks\nPlease press ctrl and left click");
    csm->CheckMenuItem(0, MF_BYPOSITION|MF_CHECKED);
	}
	else {
		AfxMessageBox("Now stop adding landmarks");
		csm->CheckMenuItem(0, MF_BYPOSITION|MF_UNCHECKED);
	}
	csm=0; cm=0; pMainFrame=0;
	delete csm; delete cm; delete pMainFrame;
}

void CEasyViewerDoc::OnShowLandmark() {
	m_SceneGraph.isShowLandmarks = !m_SceneGraph.isShowLandmarks;
	CMainFrame* pMainFrame;
	pMainFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd); 
	CMenu* cm = pMainFrame->GetMenu();
	CMenu* csm;
	if (cm!=NULL) csm = cm->GetSubMenu(6);
	if (m_SceneGraph.isShowLandmarks) {
		m_SceneGraph.showLandmarks();
		csm->CheckMenuItem(1, MF_BYPOSITION|MF_CHECKED);
	}
	else {
		csm->CheckMenuItem(1, MF_BYPOSITION|MF_UNCHECKED);
	}
	csm=0; cm=0; pMainFrame=0;
	delete csm; delete cm; delete pMainFrame;
}

void CEasyViewerDoc::OnOutputSrcLandmarks() {
  m_SceneGraph.OnOutputSrcLandmarks();
}

void CEasyViewerDoc::OnOutputTargetLandmarks() {
  m_SceneGraph.OnOutputTargetLandmarks();
}

void CEasyViewerDoc::OnDelLandmark() {
  if (manip==source) m_SceneGraph.DelLandmark(0);
  if (manip==target) m_SceneGraph.DelLandmark(1);
}

void CEasyViewerDoc::OnVertCors() {
  CVertCorDlg vDlg;
  int vertInd = -1, SrcOrTgt = -1;
  if (vDlg.DoModal()==IDOK) {
	vertInd = vDlg.m_vertInd;
	SrcOrTgt = vDlg.SrcOrTgt;
  }
  m_SceneGraph.corNo = vertInd;
  m_SceneGraph.SrcOrTgtCor = SrcOrTgt;
  UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnBend() {
	m_SceneGraph.OnBend(manip);
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnShowTexture() {
  m_SceneGraph.showTex();
	UpdateAllViews(NULL);
}

void CEasyViewerDoc::OnOutputSrcCenters() {
  m_SceneGraph.OutputSrcCenters();
  UpdateAllViews(NULL);
}

