// easyViewerView.cpp : implementation of the CEasyViewerView class
//

#include "stdafx.h"
#include "easyViewer.h"

#include "easyViewerDoc.h"
#include "easyViewerView.h"
#include "Lib3d/PsRender.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//设置光照
GLfloat mat_diffuseC[] = { 0.1f, 0.1f, 0.1f, 1.0f };//C用灰色
GLfloat mat_diffuseO[] = { 0.7f, 0.0f, 0.0f, 1.0f };//O用红色
GLfloat mat_diffuseN[] = { 0.0f, 1.0f, 0.0f, 1.0f };//N用绿色
GLfloat mat_diffuseH[] = { 1.0f, 1.0f, 1.0f, 1.0f };//H用白色

/* creates a enum type for mouse buttons */
enum {
    BUTTON_LEFT = 0,
		BUTTON_RIGHT,
		BUTTON_LEFT_TRANSLATE,
};

/* set global variables */
int mButton = -1;
int mOldY, mOldX;

/* vectors that makes the rotation and translation of the cube */
float eye[3] = {0.0f, 0.0f, 0.0f};
float rot[3] = {0.2f, 0.5f, 0.5f};

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerView

IMPLEMENT_DYNCREATE(CEasyViewerView, CView)

BEGIN_MESSAGE_MAP(CEasyViewerView, CView)
	//{{AFX_MSG_MAP(CEasyViewerView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerView construction/destruction

CEasyViewerView::CEasyViewerView()
{
	// OpenGL
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;
	
	// Mouse
	m_LeftButtonDown = FALSE;
	m_RightButtonDown = FALSE;

	// Colors
	m_ClearColorRed   = 1.0f;
	m_ClearColorGreen = 1.0f;
	m_ClearColorBlue  = 1.0f;

	// Animation
	m_StepRotationX = 0.0f;
	m_StepRotationY = 5.0f;
	m_StepRotationZ = 0.0f;

	InitGeometry();
}

CEasyViewerView::~CEasyViewerView()
{
}

BOOL CEasyViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerView drawing

void CEasyViewerView::OnDraw(CDC* pDC)
{
	CEasyViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerView printing

BOOL CEasyViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEasyViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEasyViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerView diagnostics

#ifdef _DEBUG
void CEasyViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CEasyViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEasyViewerDoc* CEasyViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEasyViewerDoc)));
	return (CEasyViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEasyViewerView message handlers


void CEasyViewerView::OnSize(UINT nType, int cx, int cy) {
	CView::OnSize(nType, cx, cy);
	width = cx, height = cy;
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	// Set OpenGL perspective, viewport and mode
	double aspect = (cy == 0) ? cx : (double)cx/(double)cy;
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,aspect,0.1,1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDrawBuffer(GL_BACK);

	// Release
	::ReleaseDC(hWnd,hDC);
}

void CEasyViewerView::OnDestroy() 
{
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	
	if(m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	CView::OnDestroy();
}

BOOL CEasyViewerView::OnEraseBkgnd(CDC* pDC) {
	return true;
}

void CEasyViewerView::OnLButtonDown(UINT nFlags, CPoint point) {
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;
	SetCapture();
  CEasyViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_SceneGraph.isAddingLandmarks && GetAsyncKeyState(VK_CONTROL)<0) {
		if (pDoc->m_SceneGraph.countObject()<1) 
			AfxMessageBox("Invalid operation");
		float x = point.x;
	  float y = point.y;
		float z = 0;
		ConvertToGLCoord(x, y, z);
		if (pDoc->manip==source) {
//       if (!pDoc->m_SceneGraph.isGenSrc) {
// 				pDoc->m_SceneGraph.GenSrcMesh();
// 				pDoc->m_SceneGraph.isGenSrc = true;
// 			}
		  pDoc->m_SceneGraph.AddLandmark(x, y, z, 0);	
		}
		else if (pDoc->manip==target) {
      if (pDoc->m_SceneGraph.countObject()<2) 
				AfxMessageBox("Invalid operation");
// 			if (!pDoc->m_SceneGraph.isGenTarget) {
// 				pDoc->m_SceneGraph.GenTargetMesh();
// 				pDoc->m_SceneGraph.isGenTarget = true;
// 			}
			pDoc->m_SceneGraph.AddLandmark(x, y, z, 1);
		}
  }
	pDoc=0; delete pDoc;
	InvalidateRect(NULL,FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CEasyViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}

mat4x4<GLfloat> CEasyViewerView::get_rotation(vec3d<GLfloat> cc, vec3d<GLfloat> cldown) {
	if (cc==cldown) return Identity<GLfloat>();
	else{
		vec3d<GLfloat> axis = cldown^cc;
		if (axis[0]==0 && axis[1]==0 && axis[2]==0) return Identity<GLfloat>();
		axis.normalize();
		return Rotation<GLfloat>(axis[0],axis[1],axis[2],-acos(cldown*cc));
	}
}

void CEasyViewerView::OnMouseMove(UINT nFlags, CPoint point) {
	// Both : rotation
	CEasyViewerDoc *pDoc = (CEasyViewerDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	vec3d<GLfloat> cldown;
  vec3d<GLfloat> cc;
	if (pDoc->manip!=all) {
	  if(m_LeftButtonDown && m_RightButtonDown) {
		  if(m_xyRotation){
			  m_yRotation[pDoc->manip] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			  m_xRotation[pDoc->manip] -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
			}
		  else{
			  m_zRotation[pDoc->manip] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
			  m_xRotation[pDoc->manip] -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
			}
			cldown = scr23d(m_LeftDownPos.x, m_LeftDownPos.y);
			cc = scr23d(point.x, point.y);
			pDoc->m_SceneGraph.rotM[pDoc->manip].rmul(get_rotation(cc, cldown));
			m_LeftDownPos = point;
			m_RightDownPos = point;
		  InvalidateRect(NULL,FALSE);
		}
	  else
	  // Left : x / y translation
	  if(m_LeftButtonDown) {
		  m_xTranslation[pDoc->manip] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
		  m_yTranslation[pDoc->manip] += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;
		  m_LeftDownPos = point;
		  InvalidateRect(NULL,FALSE);
		}
	  else
	  // Right : z translation
	  if(m_RightButtonDown) {
		  m_zTranslation[pDoc->manip] += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
		  m_RightDownPos = point;
		  InvalidateRect(NULL,FALSE);
		}
	}
	else {
		// if all objects are selected...
    if(m_LeftButtonDown && m_RightButtonDown) {
			if(m_xyRotation){
				m_yRotation[0] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
				m_yRotation[1] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
				m_xRotation[0] -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
				m_xRotation[1] -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
			}
			else{
				m_zRotation[0] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
				m_xRotation[0] -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
				m_zRotation[1] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedRotation;
				m_xRotation[1] -= (float)(m_LeftDownPos.y - point.y) * m_SpeedRotation;
			}
			
			cldown = scr23d(m_LeftDownPos.x, m_LeftDownPos.y);
			cc = scr23d(point.x, point.y);
			pDoc->m_SceneGraph.rotMAll.rmul(get_rotation(cc, cldown));
			m_LeftDownPos = point;
			m_RightDownPos = point;
			InvalidateRect(NULL,FALSE);
		}
		else
			// Left : x / y translation
			if(m_LeftButtonDown) {
				/*m_xTranslation[0] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
				m_yTranslation[0] += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;
				m_xTranslation[1] -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
				m_yTranslation[1] += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;*/
				m_xTranslationAll -= (float)(m_LeftDownPos.x - point.x) * m_SpeedTranslation;
				m_yTranslationAll += (float)(m_LeftDownPos.y - point.y) * m_SpeedTranslation;
				m_LeftDownPos = point;
				InvalidateRect(NULL,FALSE);
			}
			else
				// Right : z translation
				if(m_RightButtonDown) {
					//m_zTranslation[0] += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
					//m_zTranslation[1] += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
					m_zTranslationAll += (float)(m_RightDownPos.y - point.y) * m_SpeedTranslation;
					m_RightDownPos = point;
					InvalidateRect(NULL,FALSE);
		}
	}

	/*
	TRACE("\nPosition\n");
	TRACE("Translation : %g %g %g\n",m_xTranslation,m_yTranslation,m_zTranslation);
	TRACE("Rotation    : %g %g %g\n",m_xRotation,m_yRotation,m_zRotation);
	*/
	
	CView::OnMouseMove(nFlags, point);
}

void CEasyViewerView::onWire(){
	isWireFrame = true;
	InvalidateRect(0, 0);
}

void CEasyViewerView::applyMatrices() {
  CEasyViewerDoc *pDoc = (CEasyViewerDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_SceneGraph.applyMatrices();
	pDoc=0; delete pDoc;
}

vec3d<GLfloat> CEasyViewerView::scr23d(GLint x, GLint y){
	float xx = 2*((float)x)/width-1;
	float yy = 1-2*((float)y)/height;
	float x2y2 = 1-xx*xx-yy*yy;
	if (x2y2<0){
		double l = sqrt(xx*xx+yy*yy);
		xx = xx/l; yy = yy/l;
		x2y2 = 0;
	}
	float z = sqrt(x2y2);
	return vec3d<float>(xx,yy,z);      
}

void CEasyViewerView::OnPaint() {
	// Device context for painting
	CPaintDC dc(this); 
	// Model is stored in Document
	CEasyViewerDoc *pDoc = (CEasyViewerDoc *)GetDocument();
	ASSERT_VALID(pDoc);
	// Useful in multidoc templates
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i=0; i<2; ++i) {
	  glPushMatrix();
	    glLoadIdentity();
	    // Position / translation / scale
			glTranslatef(m_xTranslation[i],m_yTranslation[i],m_zTranslation[i]);
			glMultMatrixf(pDoc->m_SceneGraph.rotM[i].e);
			glScalef(m_xScaling[i],m_yScaling[i],m_zScaling[i]);
			//glTranslatef(-m_xTranslation[i],-m_yTranslation[i],-m_zTranslation[i]);
	    //glRotatef(m_xRotation[i], 1.0, 0.0, 0.0);
	    //glRotatef(m_yRotation[i], 0.0, 1.0, 0.0);
	    //glRotatef(m_zRotation[i], 0.0, 0.0, 1.0);	
		  glGetFloatv(GL_MODELVIEW_MATRIX, pDoc->m_SceneGraph.modelviewObject[i]);
    glPopMatrix();
	}
	for (i=0; i<16; ++i)
	  pDoc->m_SceneGraph.modelviewCurrent[i] = pDoc->m_SceneGraph.modelviewObject[0][i];
	applyMatrices();
	// Start rendering...
	glLoadIdentity();
	glTranslatef(0,0,-1.0f);
	glTranslatef(m_xTranslationAll,m_yTranslationAll,m_zTranslationAll);
	glMultMatrixf(pDoc->m_SceneGraph.rotMAll.e);
	glScalef(m_xScalingAll,m_yScalingAll,m_zScalingAll);
	if (pDoc->isOpenFile) pDoc->RenderScene();
	// Double buffer
	SwapBuffers(dc.m_ps.hdc);
	glFlush();
}

void CEasyViewerView::Onrotate() {
	CEasyViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->GeoOp = 0;
}

void CEasyViewerView::OnRButtonDown(UINT nFlags, CPoint point) {
	m_RightButtonDown = TRUE;
	m_RightDownPos = point;
	SetCapture();
	CView::OnRButtonDown(nFlags, point);
}

void CEasyViewerView::OnRButtonUp(UINT nFlags, CPoint point) {
	m_RightButtonDown = FALSE;
	m_LeftButtonDown = FALSE;
	ReleaseCapture();
	CView::OnRButtonUp(nFlags, point);
}

void CEasyViewerView::InitGeometry() {
	for (int i=0; i<2; ++i) {
	  m_xRotation[i] = 0.0f;
	  m_yRotation[i] = 0.0f;
	  m_zRotation[i] = 0.0f;
	  m_xTranslation[i] = m_yTranslation[i] = m_zTranslation[i] = 0.0f;
	  m_xScaling[i] = m_yScaling[i] = m_zScaling[i] = 1.0f;
	}
	m_xTranslationAll = m_yTranslationAll = m_zTranslationAll = 0.0f;
	m_xScalingAll = m_yScalingAll = m_zScalingAll = 1.0f;
	m_SpeedRotation = 0.33f;
	m_SpeedTranslation = 0.02f;
	m_xyRotation = 1;
}


int CEasyViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CView::OnCreate(lpCreateStruct) == -1) return -1;
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if(SetWindowPixelFormat(hDC)==FALSE) return 0;
	if(CreateViewGLContext(hDC)==FALSE) return 0;

	// Default mode
	//glPolygonMode(GL_FRONT,GL_FILL);
	//glPolygonMode(GL_BACK,GL_FILL);
	//glShadeModel(GL_FLAT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_NORMALIZE);

	// Lights properties
  float	ambientProperties[]  = {0.7f, 0.7f, 0.7f, 1.0f};
  float	diffuseProperties[]  = {0.8f, 0.8f, 0.8f, 1.0f};
  float	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float	lPosition[] = {100.0f, 0.0f, 0.0f, 1.0f};
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
  glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
  glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
  glLightfv( GL_LIGHT0, GL_POSITION, lPosition);
  glClearColor(m_ClearColorRed,m_ClearColorGreen,m_ClearColorBlue,1.0f);
  glClearDepth(1.0f);
	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// Default : material
	float MatAmbient[]  = {0.0f, 0.5f, 0.75f, 1.0f};
	float MatDiffuse[]  = {0.0f, 0.5f, 1.0f, 1.0f};
	float MatSpecular[]  = {0.75f, 0.75f, 0.75f, 1.0f};
	float MatShininess[]  = { 64 };
	float MatEmission[]  = {0.0f, 0.0f, 0.0f, 1.0f};
	float MatAmbientBack[]  = {0.0f, 0.5f, 0.0f, 1.0f}; // green material behind
	glMaterialfv(GL_FRONT,GL_AMBIENT,MatAmbient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,MatDiffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,MatSpecular);
	glMaterialfv(GL_FRONT,GL_SHININESS,MatShininess);
	glMaterialfv(GL_FRONT,GL_EMISSION,MatEmission);
	glMaterialfv(GL_BACK,GL_AMBIENT,MatAmbientBack);

	glEnable(GL_DEPTH_TEST);
	return 1;
}

BOOL CEasyViewerView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	
	m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
	if(m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if(DescribePixelFormat(hDC,m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
			return FALSE;
	}
	
	if(!SetPixelFormat(hDC,m_GLPixelIndex,&pixelDesc))
		return FALSE;
	return TRUE;
}

BOOL CEasyViewerView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	
	if(m_hGLContext==NULL)
		return FALSE;
	
	if(wglMakeCurrent(hDC,m_hGLContext)==FALSE)
		return FALSE;
	
	return TRUE;
}

void CEasyViewerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	::ReleaseDC(hWnd,hDC);
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CEasyViewerView::OnExportEps() {
  static char BASED_CODE filter[] = "EPS Files (*.eps)|*.eps";
	CFileDialog SaveDlg(FALSE,"*.eps","mesh.eps",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter);
	if(SaveDlg.DoModal() == IDOK)
	{
		CString string = SaveDlg.GetPathName();
		char *pFilename = string.GetBuffer(MAX_PATH);
		
		// Alloc
		const int size = (int)6e6; // dirty code
		GLfloat *pFeedbackBuffer = new GLfloat[size];
		ASSERT(pFeedbackBuffer);
		
		CDC *pDC = GetDC();
		
		// Useful in multidoc templates
		::wglMakeCurrent(pDC->m_hDC,m_hGLContext);
		
		// Set feedback mode
		::glFeedbackBuffer(size,GL_3D_COLOR,pFeedbackBuffer);
		::glRenderMode(GL_FEEDBACK);
		
		// Render
		::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Position / translation / scale
		::glPushMatrix();
		
		/*::glTranslated(m_xTranslation,m_yTranslation,m_zTranslation);
		::glRotatef(m_xRotation, 1.0, 0.0, 0.0);
		::glRotatef(m_yRotation, 0.0, 1.0, 0.0);
		::glRotatef(m_zRotation, 0.0, 0.0, 1.0);
		::glScalef(m_xScaling,m_yScaling,m_zScaling);*/
		
		// Start rendering
		CEasyViewerDoc *pDoc = GetDocument();
		
		// Std rendering (no superimposed lines anyway)
		//pDoc->m_SceneGraph.glDrawDirect();
		pDoc->RenderScene();
		::glPopMatrix();
		
		// Double buffer
		SwapBuffers(pDC->m_hDC);
		
		int NbValues = glRenderMode(GL_RENDER);
		
		// The stuff here
		CPsRenderer PsRenderer;
		PsRenderer.Run(pFilename,pFeedbackBuffer,NbValues,TRUE);
		
		// Cleanup
		string.ReleaseBuffer();
		delete [] pFeedbackBuffer;
		ReleaseDC(pDC);
	}
}

void CEasyViewerView::OnFileSave() {
  OnExportEps();
}

void CEasyViewerView::ConvertToGLCoord(float &x, float &y, float &z) {
	GLint    viewport[4]; 
	GLdouble modelview[16]; 
	GLdouble projection[16]; 
	GLfloat  winX, winY, winZ; 
	GLdouble posX, posY, posZ; 
	
	glGetIntegerv(GL_VIEWPORT, viewport); 
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); 
	glGetDoublev(GL_PROJECTION_MATRIX, projection); 
	winX = x; 
	winY = viewport[3] - y;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); 
	if (winZ==1) {
		x=y=z=-99999;
		return;
	}
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	x = posX;
	y = posY;
	z = posZ;
}
