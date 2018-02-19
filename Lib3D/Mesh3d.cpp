//********************************************
// Mesh3d.cpp
//********************************************
// class CMesh3d
//********************************************
// alliez@usc.edu
// Created : 15/01/98
// Modified : 15/01/98
//********************************************

#include "stdafx.h"
#include <math.h>
#include "Base3d.h"
#include "Mesh3d.h"
#include "ColorRamp.h"
#include "AVLInd.h" // Gaspard Breton's stuff
#include "WmfTools.h"
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#define Inf (float)HUGE_VAL
#define TWOPI 2*PI
using namespace TNT;
using namespace JAMA;
//#include <gl/ext/glext.h>
//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////

//********************************************
// Constructor
//********************************************
CMesh3d::CMesh3d() {
	m_ListDone = 0;
	m_Modified = 1;
	m_Name = _T("Mesh");
	// Texture
	m_IndexTexture = -1;
	m_pTextureCoordinate = NULL;
	m_pTextureCoordinateIndex = NULL;
	m_Show = 1;
	isShowPrinAxes = false;
	isShowNearestPnts = false;
	isShowCorrespondences = false;
	isBuildAdjacency = false;
	isShowCurvature = false;
  isSetCoordMesh = false;
	m_DistanceEnergy = 0;
	m_ElasticEnergy = 0;
	randomAveNearestDis = 0;
	normalToVertexRatio = 0.2*10; // 10 is the box size
	TCHAR szDirectory[MAX_PATH] = "";
	::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory);
  //AfxMessageBox(szDirectory);
	GLuint texture[2];
	glGenTextures(2, texture);
  glClearColor(0.0, 0.0, 0.0, 1.0);
	if (!glFontCreate(&font,"F:\\luming.glf", texture[0]))
		AfxMessageBox("Failed to create font");
	//init b vector
	initB();
	normalFlip = 1;
  isShowTex = false;
}

//********************************************
// Destructor
//********************************************
CMesh3d::~CMesh3d()
{
	Free();
}

//********************************************
// Free
//********************************************
void CMesh3d::Free()
{
	//TRACE("Cleanup mesh %x\n",this);
	m_ArrayVertex.Free();
	m_ArrayFace.Free();
	if(m_pTextureCoordinate != NULL) delete [] m_pTextureCoordinate;
	if(m_pTextureCoordinateIndex != NULL) delete [] m_pTextureCoordinateIndex;
}

//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////

//********************************************
// BuildList
//********************************************
int CMesh3d::glBuildList() {
	//TRACE(" Start building list ...\n");
	// Check for valid mesh
	if(m_ArrayVertex.GetSize() == 0 || m_ArrayFace.GetSize() == 0) {
		TRACE("CMesh3d::BuildList : empty mesh\n");
		return 0;
	}
	if(!m_Modified && m_ListDone) return 0;
	// Erase last list
	::glDeleteLists(m_ListOpenGL,1);
//   m_pTextureCoordinate = new float[NbVertex()*2]; // x y 
// 	m_pTextureCoordinateIndex = new int[NbFace()*3];           // triangular faces
// 	m_IndexTexture = 0;
// 	
	// Search for a new list
	m_ListOpenGL = ::glGenLists(1);
	if(m_ListOpenGL == 0) {
		TRACE("CMesh3d::BuildList : unable to build DrawList\n");
		return 0;
	}
	
	// Start list
	::glNewList(m_ListOpenGL,GL_COMPILE_AND_EXECUTE);
	glDrawDirect();
	::glEndList();
	
	// List is done now
	m_ListDone = 1;
	m_Modified = 0;
	
	return 1;
}

/*********************************
glDrawDirect 
/**********************************/
int CMesh3d::glDrawDirect() {
  GLubyte stripeImage[6] = {255,255,255,0,0,0};
  GLubyte sqImg[12] = {255,255,255,0,0,0,0,0,0,255,255,255};
  GLubyte cubImg[24] = {255,255,255,0,0,0,0,0,0,255,255,255,0,0,0,255,255,255,255,255,255,0,0,0};
  //glEnable(GL_TEXTURE_GEN_S);
  //glEnable(GL_TEXTURE_GEN_T);
//   if (isShowTex) glEnable(GL_TEXTURE_1D);
//   else glDisable(GL_TEXTURE_1D);
  //glEnable(GL_TEXTURE_2D);
  glEnable(GL_COLOR_MATERIAL);
  if (isShowTex) {
    glEnable(GL_TEXTURE_1D);
    PFNGLTEXIMAGE3DPROC glTexImage3D;
    glTexImage3D = (PFNGLTEXIMAGE3DPROC) wglGetProcAddress("glTexImage3D");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
//     glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexImage3D(GL_TEXTURE_3D, 0, 3, 2, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, cubImg);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage1D(GL_TEXTURE_1D, 0, 3, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, stripeImage);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, sqImg);
  }
  else glDisable(GL_TEXTURE_1D);
	unsigned int NbVertex = (unsigned int)m_ArrayVertex.GetSize();
	unsigned int NbFace = (unsigned int)m_ArrayFace.GetSize();
	if(!NbVertex) return 0;
	if(!NbFace) return 0;
	CFace3d *pFace, *pCoordFace;
	CVector3d *pVector;
	CColor *pColorPrevious;
	::glPushMatrix();

	//nearest points
	if (isShowNearestPnts) {
	  float x,y,z;
		float radius=maxbBoxSize/10;
	  glPushMatrix();
		  glColor3f(1,0,0);
		  x=nearV1[0], y=nearV1[1], z=nearV1[2];
		  glMatrixMode(GL_MODELVIEW);
		  glTranslatef(x, y, z);
		  glutSolidSphere(radius, 32, 32);
		  glColor3f(0,1,0);
		  //undo the first translate first!
		  glTranslatef(-x, -y, -z);
	    x=nearV2[0], y=nearV2[1], z=nearV2[2];
		  glTranslatef(x, y, z);
      glutSolidSphere(radius, 32, 32);
		  glTranslatef(-x, -y, -z);
	  glPopMatrix();
	}
	//show landmarks
  if (isShowLandmarks) {
    int sizeLM = m_ArrayLandmark.GetSize();
    float x,y,z;
    float radius=maxbBoxSize/100;
    float a,b,c;
    if (stcflag==0) {a=1;b=0;c=0;}
    if (stcflag==1) {a=0;b=1;c=0;}
    if (stcflag==2) {a=0;b=0;c=1;}
    glPushMatrix();
	  	glMatrixMode(GL_MODELVIEW);
      glColor3f(a,b,c);
      for (int i=0; i<sizeLM; ++i) {
        x=m_ArrayLandmark[i]->x();
        y=m_ArrayLandmark[i]->y();
        z=m_ArrayLandmark[i]->z();
        string data;
        stringstream ss;
        ss << i;
        data=ss.str();
        char* output = (char*)data.c_str();
        glTranslatef(x, y, z);
        glutSolidSphere(radius, 32, 32);		
        glEnable(GL_TEXTURE_2D);
        //glScalef(0.1, 0.1, 0.1);
        glFontBegin(&font);
        glFontTextOut(output, 1, 1, 0);
        glFontEnd();
        glFlush();
        //glScalef(10,10,10);
        glDisable(GL_TEXTURE_2D);
        glTranslatef(-x, -y, -z);
      }
      glPopMatrix();
	}

	// Position / translation / scaling
// 	glTranslatef(m_Transform.GetTranslation()->x(),
// 		m_Transform.GetTranslation()->y(),
// 							 m_Transform.GetTranslation()->z());
// 	
// 	glScalef(m_Transform.GetScale()->x(),
// 			     m_Transform.GetScale()->y(),
// 				 m_Transform.GetScale()->z());
// 	
// 	glRotatef(m_Transform.GetValueRotation(),
// 		m_Transform.GetRotation()->x(),
// 		m_Transform.GetRotation()->y(),
// 		m_Transform.GetRotation()->z());
	
	// Init color
	pFace = m_ArrayFace[0];
	pColorPrevious = pFace->GetColor();
	::glColor3ub(pFace->GetColor()->r(),pFace->GetColor()->g(),pFace->GetColor()->b());

	// Triangles
	::glBegin(GL_TRIANGLES);
	for(unsigned int i=0;i<NbFace;i++){
		pFace = m_ArrayFace[i];
		ASSERT(pFace != NULL);
		// Normal (per face)
		if(m_NormalBinding == NORMAL_PER_FACE){
			pVector = pFace->GetNormal();
			::glNormal3f(pVector->x(),pVector->y(),pVector->z());
		}
		// Color (per face)
		if(m_ColorBinding == COLOR_PER_FACE && pColorPrevious != pFace->GetColor())
			{
				::glColor3ub(pFace->GetColor()->r(),pFace->GetColor()->g(),pFace->GetColor()->b());
				pColorPrevious = pFace->GetColor();
			}
			
			for(int j=0;j<3;j++)
			{
 				// Normal
 				if(m_NormalBinding == NORMAL_PER_VERTEX)
 				{
 					pVector = pFace->v(j)->GetNormal();
 					::glNormal3f(pVector->x(),pVector->y(),pVector->z());
 				}
				
				// Color (per vertex)
				if(m_ColorBinding == COLOR_PER_VERTEX && pColorPrevious != pFace->v(j)->GetColor()) {
					glColor4ub(pFace->v(j)->GetColor()->r(),
						       pFace->v(j)->GetColor()->g(),
							   pFace->v(j)->GetColor()->b(),127);
					pColorPrevious = pFace->v(j)->GetColor();
				}
				
				// Texture coordinate (if needed)
				// Vertex
        if (isShowTex) {
          pCoordFace = pCoordMesh->m_ArrayFace[i];
          glTexCoord3f(pCoordFace->v(j)->x()/2,
            pCoordFace->v(j)->y(),pCoordFace->v(j)->z());
        }
				glVertex3f(pFace->v(j)->x(),pFace->v(j)->y(),pFace->v(j)->z());
			}
	}
	
	::glEnd();

	//show principal axes
	if (isShowPrinAxes) {
		Vec3f eigenVector1, eigenVector2, eigenVector3;
		for (int i = 0; i < 3; ++i) {
			eigenVector1[i] = m_prinAxes[i][0] * 10;
			eigenVector2[i] = m_prinAxes[i][1] * 10;
			eigenVector3[i] = m_prinAxes[i][2] * 10;
		}
		//float l1 = Length(eigenVector1), l2 = Length(eigenVector2), l3 = Length(eigenVector3);
		glBegin(GL_LINES);
			glColor3ub(255, 0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(eigenVector1[0], eigenVector1[1], eigenVector1[2]);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0, 0, 0);
			glVertex3f(eigenVector2[0], eigenVector2[1], eigenVector2[2]);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0, 0, 0);
			glVertex3f(eigenVector3[0], eigenVector3[1], eigenVector3[2]);
		glEnd();
	}
	glPopMatrix();

  pCoordFace = 0; pFace = 0; pVector = 0;
  delete pCoordFace; delete pFace; delete pVector;
	return 1;
}

// int CMesh3d::glDrawDirect() {
// 	glEnable(GL_COLOR_MATERIAL);
// 	unsigned int NbVertex = (unsigned int)m_ArrayVertex.GetSize();
// 	unsigned int NbFace = (unsigned int)m_ArrayFace.GetSize();
// 	if(!NbVertex) return 0;
// 	if(!NbFace) return 0;
// 	CFace3d *pFace;
// 	CVector3d *pVector;
// 	CColor *pColorPrevious;
// 	::glPushMatrix();
// 
// 	//nearest points
// 	if (isShowNearestPnts) {
// 	  float x,y,z;
// 		float radius=maxbBoxSize/100;
// 	  glPushMatrix();
// 		  glColor3f(1,0,0);
// 		  x=nearV1[0], y=nearV1[1], z=nearV1[2];
// 		  glMatrixMode(GL_MODELVIEW);
// 		  glTranslatef(x, y, z);
// 		  glutSolidSphere(radius, 32, 32);
// 		  glColor3f(0,1,0);
// 		  //undo the first translate first!
// 		  glTranslatef(-x, -y, -z);
// 	    x=nearV2[0], y=nearV2[1], z=nearV2[2];
// 		  glTranslatef(x, y, z);
//       glutSolidSphere(radius, 32, 32);
// 		  glTranslatef(-x, -y, -z);
// 	  glPopMatrix();
// 	}
// 
// 	//show landmarks
// 	if (isShowLandmarks) {
// 		int sizeLM = m_ArrayLandmark.GetSize();
// 		float x,y,z;
// 		float radius=maxbBoxSize/100;
// 		float a,b,c;
// 		if (stcflag==0) {a=1;b=0;c=0;}
// 		if (stcflag==1) {a=0;b=1;c=0;}
// 		if (stcflag==2) {a=0;b=0;c=1;}
// 		glPushMatrix();
// 	  	glMatrixMode(GL_MODELVIEW);
// 		  glColor3f(a,b,c);
// 			for (int i=0; i<sizeLM; ++i) {
//         x=m_ArrayLandmark[i]->x();
// 				y=m_ArrayLandmark[i]->y();
// 				z=m_ArrayLandmark[i]->z();
// 				string data;
// 				stringstream ss;
// 				ss << i;
// 				data=ss.str();
// 				char* output = (char*)data.c_str();
// 				glTranslatef(x, y, z);
// 		    glutSolidSphere(radius, 32, 32);		
// 				glEnable(GL_TEXTURE_2D);
// 				//glScalef(0.1, 0.1, 0.1);
// 				glFontBegin(&font);
// 				glFontTextOut(output, 1, 1, 0);
// 				glFontEnd();
// 				glFlush();
// 				//glScalef(10,10,10);
// 				glDisable(GL_TEXTURE_2D);
// 				glTranslatef(-x, -y, -z);
// 			}
// 		glPopMatrix();
// 	}
// 
// 	// Position / translation / scaling
// // 	glTranslatef(m_Transform.GetTranslation()->x(),
// // 		m_Transform.GetTranslation()->y(),
// // 							 m_Transform.GetTranslation()->z());
// // 	
// // 	glScalef(m_Transform.GetScale()->x(),
// // 			     m_Transform.GetScale()->y(),
// // 				 m_Transform.GetScale()->z());
// // 	
// // 	glRotatef(m_Transform.GetValueRotation(),
// // 		m_Transform.GetRotation()->x(),
// // 		m_Transform.GetRotation()->y(),
// // 		m_Transform.GetRotation()->z());
// 	
// 	// Init color
// 	pFace = m_ArrayFace[0];
// 	pColorPrevious = pFace->GetColor();
// 	::glColor3ub(pFace->GetColor()->r(),pFace->GetColor()->g(),pFace->GetColor()->b());
// 
// 	// Triangles
// 	::glBegin(GL_TRIANGLES);
// 	for(unsigned int i=0;i<NbFace;i++){
// 		pFace = m_ArrayFace[i];
// 		ASSERT(pFace != NULL);
// 		// Normal (per face)
// 		if(m_NormalBinding == NORMAL_PER_FACE){
// 			pVector = pFace->GetNormal();
// 			::glNormal3f(pVector->x(),pVector->y(),pVector->z());
// 		}
// 		// Color (per face)
// 		if(m_ColorBinding == COLOR_PER_FACE && pColorPrevious != pFace->GetColor())
// 			{
// 				::glColor3ub(pFace->GetColor()->r(),pFace->GetColor()->g(),pFace->GetColor()->b());
// 				pColorPrevious = pFace->GetColor();
// 			}
// 			
// 			for(int j=0;j<3;j++)
// 			{
// 				// Normal
// 				if(m_NormalBinding == NORMAL_PER_VERTEX)
// 				{
// 					pVector = pFace->v(j)->GetNormal();
// 					::glNormal3f(pVector->x(),pVector->y(),pVector->z());
// 				}
// 				
// 				// Color (per vertex)
// 				if(m_ColorBinding == COLOR_PER_VERTEX && pColorPrevious != pFace->v(j)->GetColor()) {
// 					glColor4ub(pFace->v(j)->GetColor()->r(),
// 						       pFace->v(j)->GetColor()->g(),
// 							   pFace->v(j)->GetColor()->b(),127);
// 					pColorPrevious = pFace->v(j)->GetColor();
// 				}
// 				
// 				// Texture coordinate (if needed)
// 				if(m_IndexTexture != -1)
// 				{
// 					glTexCoord2f(m_pTextureCoordinate[2*m_pTextureCoordinateIndex[3*i+j]],
// 						m_pTextureCoordinate[2*m_pTextureCoordinateIndex[3*i+j]+1]);	
// 				}
// 				
// 				// Vertex
// 				::glVertex3f(pFace->v(j)->x(),pFace->v(j)->y(),pFace->v(j)->z());
// 			}
// 	}
// 	
// 	::glEnd();
// 
// 	//show principal axes
// 	if (isShowPrinAxes) {
// 		Vec3f eigenVector1, eigenVector2, eigenVector3;
// 		for (int i = 0; i < 3; ++i) {
// 			eigenVector1[i] = m_prinAxes[i][0] * 10;
// 			eigenVector2[i] = m_prinAxes[i][1] * 10;
// 			eigenVector3[i] = m_prinAxes[i][2] * 10;
// 		}
// 		//float l1 = Length(eigenVector1), l2 = Length(eigenVector2), l3 = Length(eigenVector3);
// 		glBegin(GL_LINES);
// 			glColor3ub(255, 0, 0);
// 			glVertex3f(0, 0, 0);
// 			glVertex3f(eigenVector1[0], eigenVector1[1], eigenVector1[2]);
// 			glColor3f(0.0f, 1.0f, 0.0f);
// 			glVertex3f(0, 0, 0);
// 			glVertex3f(eigenVector2[0], eigenVector2[1], eigenVector2[2]);
// 			glColor3f(0.0f, 0.0f, 1.0f);
// 			glVertex3f(0, 0, 0);
// 			glVertex3f(eigenVector3[0], eigenVector3[1], eigenVector3[2]);
// 		glEnd();
// 	}
// 	glPopMatrix();
// 	return 1;
// }

//********************************************
// Draw
//********************************************
int CMesh3d::glDraw() {
	if(!m_Show)	return 0;
	
	// Build list at first
	if(!m_ListDone || m_Modified) glBuildList();
	
	// Search for a new list
	if(::glIsList(m_ListOpenGL)==GL_TRUE)
	{
		::glCallList(m_ListOpenGL);
		return 1;
	}
	else
	{
		TRACE(" CMesh3d::Draw : unable to draw list %d\n",m_ListOpenGL);
		return 0;
	}
}





//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////

//********************************************
// Copy
//********************************************
void CMesh3d::Copy(CMesh3d *pMesh)
{
	// Vertices
	int NbVertex = pMesh->NbVertex();
	m_ArrayVertex.SetSize(NbVertex);
	for(int i=0;i<NbVertex;i++)
		m_ArrayVertex.SetAt(i,new CVertex3d(pMesh->GetVertex(i)));
	
	// Faces
	int NbFace = pMesh->NbFace();
	m_ArrayFace.SetSize(NbFace);
	for(i=0;i<NbFace;i++)
	{
		CFace3d *pFace = pMesh->GetFace(i);
		m_ArrayFace.SetAt(i,new CFace3d(
			m_ArrayVertex[pMesh->IndexFrom(pFace->v1())],
			m_ArrayVertex[pMesh->IndexFrom(pFace->v2())],
			m_ArrayVertex[pMesh->IndexFrom(pFace->v3())]));
	}
	
	// Transform
	m_Transform.Copy(pMesh->GetTransform());
}


//********************************************
// GetType
//********************************************
int CMesh3d::GetType()
{
	return TYPE_MESH3D;
}

//********************************************
// IsValid
//********************************************
int CMesh3d::IsValid()
{
	int NbFace = m_ArrayFace.GetSize();
	for(int i=0;i<NbFace;i++)
		if(!m_ArrayFace[i]->IsValid())
			return 0;
		return 1;
}


//********************************************
// DeleteVertex
//********************************************
int CMesh3d::DeleteVertex(CVertex3d *pVertex)
{
	int size = m_ArrayVertex.GetSize();
	for(int i=0;i<size;i++)
	{
		CVertex3d *pV = m_ArrayVertex[i];
		if(pV == pVertex)
		{
			m_ArrayVertex.RemoveAt(i);
			delete pVertex;
			return 1;
		}
	}
	return 0;
}

//********************************************
// DeleteVertex
//********************************************
int CMesh3d::DeleteVertex(int index)
{
	if(index < m_ArrayVertex.GetSize())
	{
		CVertex3d *pVertex = (CVertex3d *)m_ArrayVertex[index];
		m_ArrayVertex.RemoveAt(index);
		delete pVertex;
		return 1;
	}
	return 0;
}

//********************************************
// DeleteFace
//********************************************
int CMesh3d::DeleteFaceNbNeighbors(int NbNeighbor)
{
	TRACE("Delete faces which has %d neighbors...",NbNeighbor);
	int deleted = 0;
	for(int i=0;i<m_ArrayFace.GetSize();i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		if(pFace->NbFaceNeighbor() == NbNeighbor)
		{
			m_ArrayFace.RemoveAt(i);
			delete pFace;
			i--;
			deleted++;
		}
	}
	TRACE("%d face(s) deleted\n",deleted);
	return deleted;
}


//////////////////////////////////////////////
// RANGE
//////////////////////////////////////////////

//********************************************
// Range
//********************************************
void CMesh3d::Range(int coord, 
					float *min,
					float *max)
{
	ASSERT(coord >= 0 && coord <= 2);
	int NbVertex = m_ArrayVertex.GetSize();
	float Min = m_ArrayVertex[0]->Get(coord);
	float Max = Min;
	for(int i=1;i<NbVertex;i++)
	{
		float value = m_ArrayVertex[i]->Get(coord);
		if(value < Min)
			Min = value;
		if(value > Max)
			Max = value;
	}
	*min = Min;
	*max = Max;
}

//********************************************
// Range (apply)
//********************************************
void CMesh3d::Range(int coord, 
					float min,
					float max)
{
	TRACE("Range mesh...");
	float Min,Max;
	Range(coord,&Min,&Max);
	TRACE("old : (%g,%g) -> (%g %g)",Min,Max,min,max);
	Offset(coord,-Min);
	Scale(coord,(max-min)/(Max-Min));
	Offset(coord,min);
	TRACE("ok\n");
}

//********************************************
// Scale
//********************************************
void CMesh3d::Scale(int coord,
					float scale)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for(int i=0;i<NbVertex;i++)
		m_ArrayVertex[i]->Set(coord,m_ArrayVertex[i]->Get(coord) * scale);
	m_Modified = 1;
}

//********************************************
// Offset
//********************************************
void CMesh3d::Offset(int coord,
					 float offset)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for(int i=0;i<NbVertex;i++)
		m_ArrayVertex[i]->Set(coord,m_ArrayVertex[i]->Get(coord) + offset);
	m_Modified = 1;
}


//////////////////////////////////////////////
// PROCESSING
//////////////////////////////////////////////

//********************************************
// BuildAdjacency
// From VertexArray and FaceArray, build
// neighboring vertices and faces, using
// edge and vertex sharing
//********************************************
int CMesh3d::BuildAdjacency() {
	// Check for valid sizes
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Build adjacency (%d faces, %d vertices)",NbFace,NbVertex);
	if(NbVertex < 3 || NbFace <= 0)
	{
		TRACE("  invalid array sizes\n");
		return 0;
	}
	
	// At first, clear all neighbors for each face
	//TRACE("  clear face neighbors\n");
	TRACE(".");
	for(int i=0;i<NbFace;i++)
		for(int k=0;k<6;k++)
			m_ArrayFace[i]->f(k,NULL);
		
		// At first, clear all neighbors for each vertex
		//TRACE("  clear vertex neighbors\n");
		TRACE(".");
		//get the bounding box of the mesh.
		float xMin = Inf, yMin = Inf, zMin = Inf, xMax = -Inf, yMax = -Inf, zMax = -Inf;
		double centerX, centerY, centerZ, boxSize;
		for(i=0;i<NbVertex;i++) {
			m_ArrayVertex[i]->RemoveAllFaceNeighbor();
			m_ArrayVertex[i]->RemoveAllVertexNeighbor();
			float x = m_ArrayVertex[i]->x();
			float y = m_ArrayVertex[i]->y();
			float z = m_ArrayVertex[i]->z();
			if (x < xMin) xMin = x; if (y < yMin) yMin = y; if (z < zMin) zMin = z;
			if (x > xMax) xMax = x; if (y > yMax) yMax = y; if (z > zMax) zMax = z;
		}
		centerX = 0.5 * (xMin + xMax); centerY = 0.5 * (yMin + yMax); centerZ = 0.5 * (zMin + zMax);
    //centerX = centerY = centerZ = 0;
		CString s;
		s.Format("%8.4f",centerX);
		//AfxMessageBox(s);
		maxbBoxSize = max(xMax-xMin, max(yMax-yMin, zMax-zMin));
		boxSize = 1.0 / max(xMax-xMin, max(yMax-yMin, zMax-zMin));
		//Now regularize
		//After regularization, the mesh center is located at (0, 0, 0), and the box size is 10.0f.
		for(i=0;i<NbVertex;i++) {
			float x = (m_ArrayVertex[i]->x()-centerX);//*boxSize;
			float y = (m_ArrayVertex[i]->y()-centerY);//*boxSize;
			float z = (m_ArrayVertex[i]->z()-centerZ);//*boxSize;
			m_ArrayVertex[i]->Set(x, y, z);
			//set terms in matrix
			m_ArrayVertex[i]->x1C = 0;
			m_ArrayVertex[i]->x2C = 0;
			m_ArrayVertex[i]->y1C = 0;
			m_ArrayVertex[i]->y2C = 0;
			m_ArrayVertex[i]->z1C = 0;
			m_ArrayVertex[i]->z2C = 0;
			//set index
			m_ArrayVertex[i]->index = i;
		}
		
		//*********************************************
		// For each face, set face neighboring vertices 
		//*********************************************
		//TRACE("  set face neighboring vertices\n");
		TRACE(".");
		for(i=0;i<NbFace;i++)
		{
			CFace3d *pFaceCurrent = m_ArrayFace[i];
			pFaceCurrent->index=i;
			for(int j=0;j<3;j++)
				pFaceCurrent->v(j)->AddNeighbor(pFaceCurrent);
		}
		
		//*********************************************
		// For each vertex, set vertex neighboring,
		// just look on neighboring faces 
		//*********************************************
		//TRACE("  set vertices neighboring vertices\n");
		TRACE(".");
		for(i=0;i<NbVertex;i++)
		{
			CVertex3d *pVertexCurrent = m_ArrayVertex[i];
			int NbFaceNeighbor = pVertexCurrent->NbFaceNeighbor();
			for(int j=0;j<NbFaceNeighbor;j++)
			{
				CFace3d *pFace = pVertexCurrent->GetFaceNeighbor(j);
				for(int k=0;k<3;k++)
					pVertexCurrent->AddNeighbor(pFace->v(k));
			}
		}
		
		//*********************************************
		// For each face, set face neighboring,
		// just look on faces neighboring vertices
		//*********************************************
		//TRACE("  set faces neighboring faces\n");
		TRACE(".");
		for(i=0;i<NbFace;i++)
		{
			CFace3d *pFaceCurrent = m_ArrayFace[i];
			// For each edge
			for(int j=0;j<3;j++)
			{
				CVertex3d *pVertex = pFaceCurrent->v(j);
				CVertex3d *pNextVertex = pFaceCurrent->v((j+1)%3);
				int NbFaceNeighbor = pVertex->NbFaceNeighbor();
				for(int k=0;k<NbFaceNeighbor;k++)
				{
					// This face contain pVertex
					CFace3d *pFace = pVertex->GetFaceNeighbor(k);
					if(pFace != pFaceCurrent)
						if(pFaceCurrent->f(j) == NULL)
							if(pFace->HasVertex(pVertex))
								if(pFace->HasVertex(pNextVertex))
									pFaceCurrent->f(j,pFace);
				}
			}
		}
		
		/*
		// Check
		for(i=0;i<NbFace;i++)
		{
		ASSERT(m_ArrayFace[i]->IsValid());
		}
		*/
		
		
		TRACE("ok\n");
		isBuildAdjacency = true;
		BuildAnnKDTree();
		ComputeLocalCenters();
		//Initialize matrix A
		int size = NbVertex*3;
		llm::SparseMat AA(size,size);
		A = AA;
		return TRUE;
}

void CMesh3d::ComputeLocalCenters() {
	int k = 100;
	float x, y, z;
	ANNdistArray dists= new ANNdist[k];	
	ANNidxArray idx= new ANNidx[k];
	ANNpoint p = annAllocPt(3);
  for (int i=0; i<NbVertex(); ++i) {
		x = 0; y = 0; z = 0;
		p[0] = m_ArrayVertex[i]->x();
		p[1] = m_ArrayVertex[i]->y();
		p[2] = m_ArrayVertex[i]->z();
		
		pVert_Tree3->annkFRSearch(p, maxbBoxSize/3, k, idx, dists, 0);
		//pVert_Tree3->annkSearch(p, k, idx, dists, 0);
		int count = 0;
		for (int j=0; j<k; ++j) {
			if (idx[j]<0) break;
			count++;
			x += m_ArrayVertex[idx[j]]->x();
			y += m_ArrayVertex[idx[j]]->y();
			z += m_ArrayVertex[idx[j]]->z();
		}
		x /= count; y /= count; z/= count;
		m_ArrayVertex[i]->localCenter.push_back(x);
		m_ArrayVertex[i]->localCenter.push_back(y);
		m_ArrayVertex[i]->localCenter.push_back(z);
	}
}

//********************************************
// Rebuild
//********************************************
void CMesh3d::Rebuild() {
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	SetModified(1);
}

/////////////////////////////////////////////
// DEBUG
//////////////////////////////////////////////

//********************************************
// Trace
//********************************************
void CMesh3d::Trace()
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	
	TRACE("\n");
	TRACE(" Mesh \n");
	TRACE("Vertex : %d\n",NbVertex);
	TRACE("Face   : %d\n",NbFace);
	
	for(int i=0;i<NbVertex;i++)
		((CVertex3d *)m_ArrayVertex[i])->Trace();
	
	for(int j=0;j<NbFace;j++)
		((CFace3d *)m_ArrayFace[j])->Trace();
}

//********************************************
// IndexFrom
// Return -1 if failed
//********************************************
int CMesh3d::IndexFrom(CFace3d *pFace)
{
	ASSERT(pFace != NULL);
	int NbFace = m_ArrayFace.GetSize();
	for(int i=0;i<NbFace;i++)
		if(m_ArrayFace[i] == pFace)
			return i;
		return -1;
}

//********************************************
// IndexFrom
// Return -1 if failed
//********************************************
int CMesh3d::IndexFrom(CVertex3d *pVertex)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for(int i=0;i<NbVertex;i++)
		if(m_ArrayVertex[i] == pVertex)
			return i;
		return -1;
}

//********************************************
// Move
//********************************************
void CMesh3d::Move(float dx,float dy,float dz)
{
	int NbVertex = m_ArrayVertex.GetSize();
	for(int i=0;i<NbVertex;i++)
		m_ArrayVertex[i]->Move(dx,dy,dz);
	m_Modified = 1;
}


//********************************************
// FindVertexInFaces
//********************************************
int CMesh3d::FindVertex(CVertex3d *pVertex)
{
	int find = 0;
	
	int NbFace = m_ArrayFace.GetSize();
	for(int j=0;j<NbFace;j++)
		if(m_ArrayFace[j]->HasVertex(pVertex))
		{
			find = 1;
			TRACE("Find vertex [%x] in face %d\n",pVertex,j);
		}
		
		int NbVertex = m_ArrayVertex.GetSize();
		for(j=0;j<NbVertex;j++)
			if(m_ArrayVertex[j] == pVertex)
			{
				find = 1;
				TRACE("Find vertex [%x] at position %d\n",pVertex,j);
			}
			
			for(j=0;j<NbVertex;j++)
			{
				CVertex3d *pV = m_ArrayVertex[j];
				if(pV->HasNeighbor(pVertex))
				{
					find = 1;
					TRACE("Find vertex [%x] in neighbors of vertex %d\n",pVertex,j);
				}
			}
			
			return find;
}

//********************************************
// FindFace
//********************************************
int CMesh3d::FindFace(CFace3d *pFace)
{
	int find = 0;
	
	int NbFace = m_ArrayFace.GetSize();
	for(int j=0;j<NbFace;j++)
	{
		CFace3d *pF = m_ArrayFace[j];
		if(pF == pFace)
		{
			find = 1;
			TRACE("Find face [%x] in mesh (index : %d)\n",pFace,j);
		}
		
		for(int i=0;i<3;i++)
			if(pF->f(i) == pFace)
			{
				find = 1;
				TRACE("Find face [%x] in neighbors %d of face %d\n",pFace,i,j);
			}
	}
	
	return find;
}



//////////////////////////////////////////////
// NORMALS
//////////////////////////////////////////////


//********************************************
// CalculateNormalPerVertex
//********************************************
int CMesh3d::CalculateNormalPerVertex(void)
{
	int NbVertex = m_ArrayVertex.GetSize();
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Calculate normal per vertex (%d faces, %d vertices)...",NbFace,NbVertex);
	for(int i=0;i<NbVertex;i++)
	{
		CVertex3d *pVertex = m_ArrayVertex[i];
		int NbNormal = 0;
		CVector3d vector;
		int NbFaceNeighbor = pVertex->NbFaceNeighbor();
		for(int j=0;j<NbFaceNeighbor;j++)
		{
			CFace3d *pFace = pVertex->GetFaceNeighbor(j);
			NbNormal += 1;
			vector += pFace->GetNormal();
		}
		if(NbNormal>=1)
		{
			vector.NormalizeL2();
			pVertex->SetNormal(vector);
		}
	}
	m_Modified = 1;
	TRACE("ok\n");
	
	return 1;
}

//********************************************
// CalculateNormalPerFace
//********************************************
int CMesh3d::CalculateNormalPerFace(void)
{
	int size = m_ArrayFace.GetSize();
	TRACE("Calculate normal per face (%d faces)...",size);
	for(int i=0;i<size;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		if(pFace->IsValid()) {
			pFace->nFlip = normalFlip;
			pFace->CalculateNormal();
		}
	}
	m_Modified = 1;
	TRACE("ok\n");
	return 1;
}

//********************************************
// ColorSharpEdges
//********************************************
int CMesh3d::ColorSharpEdge(float threshold,
							CColor &color)
{
	int NbFace = m_ArrayFace.GetSize();
	
	TRACE(" Start ColorSharpEdges\n");
	TRACE("   Faces : %d\n",NbFace);
	for(int i=0;i<NbFace;i++)
		m_ArrayFace[i]->ColorSharpEdge(threshold,color);
	TRACE(" End ColorSharpEdges\n");
	
	return 1;
}


//********************************************
// SetNormalBinding
//********************************************
void CMesh3d::SetNormalBinding(int type)
{
	m_NormalBinding = type;
	m_Modified = 1;
}

//********************************************
// GetNormalBinding
//********************************************
int CMesh3d::GetNormalBinding(void)
{
	return m_NormalBinding;
}

//********************************************
// SetColorBinding
//********************************************
void CMesh3d::SetColorBinding(int type)
{
	m_ColorBinding = type;
	m_Modified = 1;
}

//********************************************
// SetColor
//********************************************
void CMesh3d::SetColor(CColor &color)
{
	int size = m_ArrayFace.GetSize();
	for(int i=0;i<size;i++)
		m_ArrayFace[i]->SetColor(color);
	m_Modified = 1;
}

//********************************************
// SetColor
//********************************************
void CMesh3d::SetColor(unsigned char r,
					   unsigned char g,
					   unsigned char b)
{
	for(int i=0;i<m_ArrayFace.GetSize();i++)
		m_ArrayFace[i]->SetColor(r,g,b);
	for(i=0;i<m_ArrayVertex.GetSize();i++)
		m_ArrayVertex[i]->SetColor(r,g,b);
	m_Modified = 1;
}

//********************************************
// GetColorBinding
//********************************************
int CMesh3d::GetColorBinding(void)
{
	return m_ColorBinding;
}

//********************************************
// SetFlagOnFaces
//********************************************
void CMesh3d::SetFlagOnFaces(int flag)
{
	int size = m_ArrayFace.GetSize();
	for(int i=0;i<size;i++)
		m_ArrayFace[i]->SetFlag(flag);
}

//********************************************
// GetMaxFlagOnFaces
//********************************************
int CMesh3d::GetMaxFlagOnFaces()
{
	int size = m_ArrayFace.GetSize();
	int max = 0;
	for(int i=0;i<size;i++)
	{
		int tmp = m_ArrayFace[i]->GetFlag();
		max = (tmp > max) ? tmp : max;
	}
	return max;
}

//********************************************
// SetFlagOnVertices
//********************************************
void CMesh3d::SetFlagOnVertices(int flag)
{
	int size = m_ArrayVertex.GetSize();
	for(int i=0;i<size;i++)
		m_ArrayVertex[i]->SetFlag((char)flag);
}


//********************************************
// GetFirstVertexWithFlag
//********************************************
float CMesh3d::GetMeanLengthEdge()
{
	int size = m_ArrayVertex.GetSize();
	double sum = 0;
	for(int i=0;i<size;i++)
		sum += m_ArrayVertex[i]->GetMeanLengthEdgeAround();
	if(size)
		return (float)sum/size;
	else
		return 0.0f;
}

//////////////////////////////////////////////
// SUBDIVISION
//////////////////////////////////////////////

//********************************************
// Alpha
// From Piecewise smooth reconstruction (Hoppe)
//********************************************
float CMesh3d::Alpha(int n)
{
	float tmp = 3.0f + 2.0f * (float)cos(2.0f * 3.14159265359f/(float)n);
	float a = 5.0f/8.0f - (tmp*tmp)/64.0f;
	return (n*(1-a)/a);
}


//********************************************
// SubdivisionLoop
// From Piecewise smooth reconstruction (Hoppe)
//********************************************
// (SIGGRAPH 94) -> Charles Loop 
//********************************************
int CMesh3d::SubdivisionLoop()
{
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();
	SetFlagOnVertices(0); // for boundaries
	
	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Start loop's subdivision (%d faces, %d vertices)\n",NbFace,NbVertex);
	
	TRACE("  subdivision info (%d vertices)\n",m_ArrayVertex.GetSize());
	
	// For each face
	for(int i=0;i<NbFace;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		
		// Check valid neighboring
		if(!pFace->IsValid())
			continue;
		
		// On each edge
		for(int IndexEdge=0;IndexEdge<3;IndexEdge++)
		{
			// Get IndexEdge on neighbor
			int IndexCurrent,IndexNeighbor;
			CFace3d *pFaceNeighbor = pFace->f(IndexEdge);
			
			// No neighbor on this edge, it is a boundary edge
			if(pFaceNeighbor == NULL)
			{
				// Two vertices involved
				CVertex3d *pVertex[2];
				pVertex[0] = pFace->v(IndexEdge);
				pVertex[1] = pFace->v((IndexEdge+1)%3);
				float x = 0.5f*(pVertex[0]->x()+pVertex[1]->x());
				float y = 0.5f*(pVertex[0]->y()+pVertex[1]->y());
				float z = 0.5f*(pVertex[0]->z()+pVertex[1]->z());
				CVertex3d *pNewVertex = new CVertex3d(x,y,z);
				m_ArrayVertex.Add(pNewVertex);
				pFace->v(3+IndexEdge,pNewVertex);
				// Boundary vertices
				pVertex[0]->SetFlag(1);
				pVertex[1]->SetFlag(1);
			}
			else // std case
			{
				VERIFY(pFace->Share2Vertex(pFaceNeighbor,&IndexCurrent,&IndexNeighbor));
				ASSERT(IndexCurrent == IndexEdge);
				
				// If neighboring face has been treated, then get vertex
				// and go to next step
				if(pFaceNeighbor->GetFlag())
				{
					CVertex3d *pVertex = pFaceNeighbor->v(3+(IndexNeighbor%3));
					pFace->v(3+(IndexEdge%3),pVertex);
					continue;
				}
				
				// Vertex weighting
				// 0 & 1 : weight : 3, 2 & 3 : weight : 1
				//这一段等于新点几何位置的确定
				CVertex3d *pVertex[4];
				
				// Weight : 3
				pVertex[0] = pFace->v(IndexEdge);
				pVertex[1] = pFace->v((IndexEdge+1)%3);
				// Weight : 1
				pVertex[2] = pFace->v((IndexEdge+2)%3);
				pVertex[3] = pFaceNeighbor->v((IndexNeighbor+2)%3);
				
				ASSERT(pVertex[0] != NULL &&  
					pVertex[1] != NULL &&  
					pVertex[2] != NULL &&  
					pVertex[3] != NULL);
				
				// For each composant
				float x = (3.0f * (pVertex[0]->x() + pVertex[1]->x()) + 
					pVertex[2]->x() + pVertex[3]->x()) / 8.0f;
				float y = (3.0f * (pVertex[0]->y() + pVertex[1]->y()) + 
					pVertex[2]->y() + pVertex[3]->y()) / 8.0f;
				float z = (3.0f * (pVertex[0]->z() + pVertex[1]->z()) + 
					pVertex[2]->z() + pVertex[3]->z()) / 8.0f;
				
				// Add vertex to global mesh array, and set face's vertex
				CVertex3d *pNewVertex = new CVertex3d(x,y,z);
				m_ArrayVertex.Add(pNewVertex);
				pFace->v(3+IndexCurrent,pNewVertex);
			}
		}
		
		// Set flag
		pFace->SetFlag(1); 
	}
	
	//*****************************
	// Create faces
	//*****************************
	TRACE("  creating faces (%d faces)\n",NbFace);
	// For each valid face
	for(i=0;i<NbFace;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		pFace->SetFlag(0);
		
		// Valid face
		int NbVertex = pFace->NbVertex();
		CFace3d *pNewFace;
		
		switch(NbVertex)
		{
		case 4:
			// Create one face
			
			// On edge 0
			if(pFace->v(3) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(2));
				m_ArrayFace.Add(pNewFace);
				
				// Move current face
				pFace->v(1,pFace->v(3));
			}
			else
				// On edge 1
				if(pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(0),pFace->v(4),pFace->v(2));
					m_ArrayFace.Add(pNewFace);
					
					// Move current face
					pFace->v(2,pFace->v(4));
				}
				else
					// On edge 2
					if(pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(5),pFace->v(1),pFace->v(2));
						m_ArrayFace.Add(pNewFace);
						
						// Move current face
						pFace->v(2,pFace->v(5));
					}
					break;
		case 5:
			// Create two faces
			
			// On edge 0 & 2
			if(pFace->v(3) != NULL && 
				pFace->v(5) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(0),pFace->v(3),pFace->v(5));
				m_ArrayFace.Add(pNewFace);
				pNewFace = new CFace3d(pFace->v(5),pFace->v(3),pFace->v(2));
				m_ArrayFace.Add(pNewFace);
				
				// Move current face
				pFace->v(0,pFace->v(3));
			}
			else
				// On edge 0 & 1
				if(pFace->v(3) != NULL && 
					pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					pNewFace = new CFace3d(pFace->v(0),pFace->v(3),pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					
					// Move current face
					pFace->v(1,pFace->v(4));
				}
				else
					// On edge 1 & 2
					if(pFace->v(4) != NULL && 
						pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(1),pFace->v(4),pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						pNewFace = new CFace3d(pFace->v(4),pFace->v(2),pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						
						// Move current face
						pFace->v(2,pFace->v(5));
					}
					break;
		case 6:
			// Create three faces
			
			// First (v3,v1,v4)
			pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(4));
			m_ArrayFace.Add(pNewFace);
			// Second (v3,v4,v5)
			pNewFace = new CFace3d(pFace->v(3),pFace->v(4),pFace->v(5));
			m_ArrayFace.Add(pNewFace);
			// Third (v5,v4,v2)
			pNewFace = new CFace3d(pFace->v(5),pFace->v(4),pFace->v(2));
			m_ArrayFace.Add(pNewFace);
			
			// Move current face
			pFace->v(1,pFace->v(3));
			pFace->v(2,pFace->v(5));
			
			break;
		}
		// Remove subdivision info
		for(int k=3;k<6;k++)
			pFace->v(k,NULL);
		
	}
	TRACE("  end creating faces (%d faces)\n",m_ArrayFace.GetSize());
	
	
	//*****************************
	// Move original vertices
	//*****************************
	
	// Copy
	TRACE("  copy\n");
	CArray3d<CVertex3d> ArrayVertex;
	ArrayVertex.SetSize(NbVertex);
	for(i=0;i<NbVertex;i++)
		ArrayVertex.SetAt(i,new CVertex3d);
	
	// For each vertex (at least 2 neighbors)
	for(i=0;i<NbVertex;i++)
	{
		CVertex3d *pVertex = m_ArrayVertex[i];
		int n = pVertex->NbVertexNeighbor();
		
		// Spline on boundaries, we need two vertices
		// remind that the adjacency has not been 
		// updated yet
		// weighting: 1 6 1
		if(pVertex->GetFlag())
		{
			CVertex3d *pv[2];
			int index = 0;
			for(int k=0;k<n;k++)
			{
				CVertex3d *pNVertex = pVertex->GetVertexNeighbor(k);
				if(pNVertex->GetFlag())
					pv[index++] = pNVertex;
			}
			ASSERT(index == 2);
			ArrayVertex[i]->Set(0.125f*(6.0f*pVertex->x()+pv[0]->x()+pv[1]->x()),
				0.125f*(6.0f*pVertex->y()+pv[0]->y()+pv[1]->y()),
				0.125f*(6.0f*pVertex->z()+pv[0]->z()+pv[1]->z()));
		}
		else
		{
			float alpha = Alpha(n);
			float tmp = alpha + (float)n;
			
			// For each componant
			for(unsigned int j=0;j<3;j++)
			{
				float value = alpha * pVertex->Get(j);
				for(int k=0;k<n;k++)
					value += pVertex->GetVertexNeighbor(k)->Get(j);
				value /= tmp;
				ArrayVertex[i]->Set(j,value);
			}
		}
	}
	
	// Restore
	TRACE("  restore\n");
	for(i=0;i<NbVertex;i++)
		for(unsigned int j=0;j<3;j++)
			m_ArrayVertex[i]->Set(j,ArrayVertex[i]->Get(j));
		
		ArrayVertex.Free();
		
		// Rebuild adjacency and normals
		BuildAdjacency();
		CalculateNormalPerFace();
		CalculateNormalPerVertex();
		m_Modified = 1;
		
		TRACE("End loop's subdivision (%d faces, %d vertices)\n",
			m_ArrayFace.GetSize(),m_ArrayVertex.GetSize());
		
		return 1;
}


//********************************************
// Subdivision
// Simple : 1->4
//********************************************
int CMesh3d::Subdivision(void)
{
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();
	
	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Start subdivision (%d faces, %d vertices)\n",NbFace,NbVertex);
	
	TRACE("  subdivision info (%d vertices)\n",m_ArrayVertex.GetSize());
	
	// For each face
	for(int i=0;i<NbFace;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		
		// On each edge
		for(int IndexEdge=0;IndexEdge<3;IndexEdge++)
		{
			// Get IndexEdge on neighbor
			int IndexCurrent,IndexNeighbor;
			CFace3d *pFaceNeighbor = pFace->f(IndexEdge);
			
			// No neighbor on this edge, go to next
			if(pFaceNeighbor != NULL)
			{
				
				VERIFY(pFace->Share2Vertex(pFaceNeighbor,&IndexCurrent,&IndexNeighbor));
				ASSERT(IndexCurrent == IndexEdge);
				
				// If neighboring face has been treated, then get vertex
				// and go to next step
				if(pFaceNeighbor->GetFlag())
				{
					CVertex3d *pVertex = pFaceNeighbor->v(3+(IndexNeighbor%3));
					pFace->v(3+(IndexEdge%3),pVertex);
					continue;
				}
			}
			
			// Vertex weighting
			// 0 & 1 : weight : 1
			CVertex3d *pVertex[2];
			
			// Weight : 1
			pVertex[0] = pFace->v(IndexEdge);
			pVertex[1] = pFace->v((IndexEdge+1)%3);
			
			ASSERT(pVertex[0] != NULL &&  pVertex[1] != NULL);
			
			// For each composant
			float coord[3];
			for(int k=0;k<3;k++)
				coord[k] = (pVertex[0]->Get(k) + pVertex[1]->Get(k)) / 2.0f;
			
			// Add vertex to global mesh array, and set face's vertex
			CVertex3d *pNewVertex = new CVertex3d(coord[0],coord[1],coord[2]);
			m_ArrayVertex.Add(pNewVertex);
			
			pFace->v(3+IndexEdge,pNewVertex);
		}
		
		// Set flag
		pFace->SetFlag(1); 
	}
	
	//*****************************
	// Create faces
	//*****************************
	TRACE("  creating faces (%d faces)\n",NbFace);
	// For each valid face
	for(i=0;i<NbFace;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		pFace->SetFlag(0);
		
		// Valid face
		int NbVertex = pFace->NbVertex();
		CFace3d *pNewFace;
		
		switch(NbVertex)
		{
		case 4:
			// Create one face
			
			// On edge 0
			if(pFace->v(3) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(2));
				m_ArrayFace.Add(pNewFace);
				
				// Move current face
				pFace->v(1,pFace->v(3));
			}
			else
				// On edge 1
				if(pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(0),pFace->v(4),pFace->v(2));
					m_ArrayFace.Add(pNewFace);
					
					// Move current face
					pFace->v(2,pFace->v(4));
				}
				else
					// On edge 2
					if(pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(5),pFace->v(1),pFace->v(2));
						m_ArrayFace.Add(pNewFace);
						
						// Move current face
						pFace->v(2,pFace->v(5));
					}
					break;
		case 5:
			// Create two faces
			
			// On edge 0 & 2
			if(pFace->v(3) != NULL && 
				pFace->v(5) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(0),pFace->v(3),pFace->v(5));
				m_ArrayFace.Add(pNewFace);
				pNewFace = new CFace3d(pFace->v(5),pFace->v(3),pFace->v(2));
				m_ArrayFace.Add(pNewFace);
				
				// Move current face
				pFace->v(0,pFace->v(3));
			}
			else
				// On edge 0 & 1
				if(pFace->v(3) != NULL && 
					pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					pNewFace = new CFace3d(pFace->v(0),pFace->v(3),pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					
					// Move current face
					pFace->v(1,pFace->v(4));
				}
				else
					// On edge 1 & 2
					if(pFace->v(4) != NULL && 
						pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(1),pFace->v(4),pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						pNewFace = new CFace3d(pFace->v(4),pFace->v(2),pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						
						// Move current face
						pFace->v(2,pFace->v(5));
					}
					break;
		case 6:
			// Create three faces
			
			// First (v3,v1,v4)
			pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(4));
			m_ArrayFace.Add(pNewFace);
			// Second (v3,v4,v5)
			pNewFace = new CFace3d(pFace->v(3),pFace->v(4),pFace->v(5));
			m_ArrayFace.Add(pNewFace);
			// Third (v5,v4,v2)
			pNewFace = new CFace3d(pFace->v(5),pFace->v(4),pFace->v(2));
			m_ArrayFace.Add(pNewFace);
			
			// Move current face
			pFace->v(1,pFace->v(3));
			pFace->v(2,pFace->v(5));
			
			break;
		}
		// Remove subdivision info
		for(int k=3;k<6;k++)
			pFace->v(k,NULL);
		
	}
	TRACE("  end creating faces (%d faces)\n",m_ArrayFace.GetSize());
	
	// Rebuild adjacency and normals
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	m_Modified = 1;
	
	TRACE("End subdivision (%d faces, %d vertices)\n",
		m_ArrayFace.GetSize(),m_ArrayVertex.GetSize());
	
	return 1;
}

//********************************************
// Smooth
// 30/09/98
//********************************************
int CMesh3d::Smooth(int MoveOnBundary /* = 1 */)
{
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();
	
	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	TRACE("Start smoothing (%d faces, %d vertices)",NbFace,NbVertex);
	
	//*****************************
	// Move original vertices
	//*****************************
	
	// Copy
	TRACE(".");
	CArray3d<CVertex3d> ArrayVertex;
	ArrayVertex.SetSize(NbVertex);
	for(int i=0;i<NbVertex;i++)
		ArrayVertex.SetAt(i,new CVertex3d);
	
	// For each vertex (at least 3 neighbors)
	for(i=0;i<NbVertex;i++)
	{
		CVertex3d *pVertex = m_ArrayVertex[i];
		
		if(!MoveOnBundary)
			if(pVertex->IsOnBoundary())
			{
				ArrayVertex[i]->Set(pVertex);	
				continue;
			}
			
			int n = pVertex->NbVertexNeighbor();
			float alpha = Alpha(n);
			float tmp = alpha + (float)n;
			// For each composant
			for(unsigned int j=0;j<3;j++)
			{
				float value = alpha * pVertex->Get(j);
				for(int k=0;k<n;k++)
					value += pVertex->GetVertexNeighbor(k)->Get(j);
				value /= tmp;
				ArrayVertex[i]->Set(j,value);
			}
	}
	
	// Restore
	TRACE(".");
	for(i=0;i<NbVertex;i++)
		for(unsigned int j=0;j<3;j++)
			m_ArrayVertex[i]->Set(j,ArrayVertex[i]->Get(j));
		
		// Cleanup
		TRACE(".");
		ArrayVertex.Free();
		
		TRACE("ok\n");
		
		// Rebuild adjacency and normals
		BuildAdjacency();
		CalculateNormalPerFace();
		CalculateNormalPerVertex();
		m_Modified = 1;
		
		return 1;
}



//********************************************
// ColorCurvature
// Each face is colored, function of mean curvature
//********************************************
void CMesh3d::ColorCurvature(CColorRamp *pRamp)
{
	TRACE("Start coloring mesh (curvature)\n");
	int NbVertex = m_ArrayVertex.GetSize();
	TRACE("  %d vertices\n",NbVertex);
	
	double *pMax = new double[NbVertex];
	
	// Store curvatures
	for(int i=0;i<NbVertex;i++)
		pMax[i] = m_ArrayVertex[i]->GetMaxCurveAround();
	
	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for(i=0;i<NbVertex;i++)
	{
		min = (pMax[i] < min) ? pMax[i] : min;
		max = (pMax[i] > max) ? pMax[i] : max;
	}
	
	min = (min < 0.0f) ? 0.0f : min;
	double amplitude = max-min;//max-min;
	TRACE("  min : %g\n",min);
	TRACE("  max : %g\n",max);
	TRACE("  amplitude : %g\n",amplitude);
	for(i=0;i<NbVertex;i++)
	{
		unsigned char _grey = (unsigned char)((pMax[i]-min)/amplitude * 255.0f);
		unsigned char grey = _grey > (unsigned char)255 ? (unsigned char)255 : _grey;
		m_ArrayVertex[i]->SetColor(pRamp->Red(grey),pRamp->Green(grey),pRamp->Blue(grey));
	}
	
	SetModified();
	
	TRACE("End coloring mesh (curvature)\n");
}

//********************************************
// ColorSpaceNormal
// Each vertex is colored, function of 
// normal space (sum of angles between
// adjacent faces)
//********************************************
void CMesh3d::ColorNormalSpace(CColorRamp *pRamp)
{
	TRACE("Start coloring mesh (space of normals)\n");
	int NbVertex = m_ArrayVertex.GetSize();
	TRACE("  %d vertices\n",NbVertex);
	
	double *pSum = new double[NbVertex];
	
	// Store curvatures
	for(int i=0;i<NbVertex;i++)
		m_ArrayVertex[i]->NormalMax(&pSum[i]);
	
	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for(i=0;i<NbVertex;i++)
	{
		min = (pSum[i] < min) ? pSum[i] : min;
		max = (pSum[i] > max) ? pSum[i] : max;
	}
	
	min = (min < 0.0f) ? 0.0f : min;
	double amplitude = max-min;//max-min;
	TRACE("  min : %g\n",min);
	TRACE("  max : %g\n",max);
	TRACE("  amplitude : %g\n",amplitude);
	for(i=0;i<NbVertex;i++)
	{
		unsigned char _grey = (unsigned char)((pSum[i]-min)/amplitude * 255.0f);
		unsigned char grey = _grey > (unsigned char)255 ? (unsigned char)255 : _grey;
		//unsigned char grey = (unsigned char)((pCurvature[i]-min)/amplitude * 255.0f);
		m_ArrayVertex[i]->SetColor(pRamp->Red(grey),pRamp->Green(grey),pRamp->Blue(grey));
	}
	
	SetModified();
	delete [] pSum;
	
	TRACE("End coloring mesh (space of normals)\n");
}


//********************************************
// ColorCompacity
// Each face is colored, function of face
// compacity
//********************************************
void CMesh3d::ColorCompacity(CColorRamp *pRamp)
{
	TRACE("Start coloring mesh (compacity)\n");
	int NbFace = m_ArrayFace.GetSize();
	TRACE("  %d face(s)\n",NbFace);
	
	double *pCompacity = new double[NbFace];
	
	// Store compacity
	for(int i=0;i<NbFace;i++)
		pCompacity[i] = m_ArrayFace[i]->Compacity();
	
	// Process extremas
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for(i=0;i<NbFace;i++)
	{
		min = (pCompacity[i] < min) ? pCompacity[i] : min;
		max = (pCompacity[i] > max) ? pCompacity[i] : max;
	}
	
	double amplitude = max-min;//max-min;
	TRACE("  min : %g\n",min);
	TRACE("  max : %g\n",max);
	TRACE("  amplitude : %g\n",amplitude);
	for(i=0;i<NbFace;i++)
	{
		unsigned char _grey = (unsigned char)((pCompacity[i]-min)/amplitude * 255.0f);
		unsigned char grey = _grey > (unsigned char)255 ? (unsigned char)255 : _grey;
		//unsigned char grey = (unsigned char)((pCurvature[i]-min)/amplitude * 255.0f);
		m_ArrayFace[i]->SetColor(pRamp->Red(grey),pRamp->Green(grey),pRamp->Blue(grey));
	}
	
	SetModified();
	delete [] pCompacity;
	
	TRACE("End coloring mesh (compacity)\n");
}

//********************************************
// ColorHeight
// Each vertex is colored, function of height
//********************************************
void CMesh3d::ColorHeight(CColorRamp *pRamp)
{
	// Color vertices
	int NbVertex = m_ArrayVertex.GetSize();
	double min = MAX_DOUBLE;
	double max = 0.0f;
	for(int i=0;i<NbVertex;i++)
	{
		float height = m_ArrayVertex[i]->y();
		min = (height < min) ? height : min;
		max = (height > max) ? height : max;
	}
	double amplitude = max-min;
	for(i=0;i<NbVertex;i++)
	{
		float height = m_ArrayVertex[i]->y();
		unsigned char _grey = (unsigned char)((height-min)/amplitude * 255.0f);
		unsigned char grey = _grey > (unsigned char)255 ? (unsigned char)255 : _grey;
		m_ArrayVertex[i]->SetColor(pRamp->Red(grey),pRamp->Green(grey),pRamp->Blue(grey));
	}
	
	// Color faces
	int NbFace = m_ArrayFace.GetSize();
	TRACE("  %d faces\n",NbFace);
	for(i=0;i<NbFace;i++)
	{
		float height = (m_ArrayFace[i]->v1()->y()+
			m_ArrayFace[i]->v1()->y()+
			m_ArrayFace[i]->v1()->y())/3.0f;
		unsigned char _grey = (unsigned char)((height-min)/amplitude * 255.0f);
		unsigned char grey = _grey > (unsigned char)255 ? (unsigned char)255 : _grey;
		m_ArrayFace[i]->SetColor(pRamp->Red(grey),pRamp->Green(grey),pRamp->Blue(grey));
	}
	SetModified();
}



//////////////////////////////////////////////
// PREDEFINED
//////////////////////////////////////////////

//********************************************
// GenerateBox
//********************************************
int CMesh3d::GenerateBox(float dx,
						 float dy,
						 float dz)
{
	TRACE("Generate box...");
	CVertex3d *pVertex;
	
	pVertex = new CVertex3d(-dx/2,-dy/2,-dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(-dx/2,+dy/2,-dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(+dx/2,+dy/2,-dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(+dx/2,-dy/2,-dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(-dx/2,-dy/2,+dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(-dx/2,+dy/2,+dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(+dx/2,+dy/2,+dz/2);
	m_ArrayVertex.Add(pVertex);
	
	pVertex = new CVertex3d(+dx/2,-dy/2,+dz/2);
	m_ArrayVertex.Add(pVertex);
	
	CFace3d *pFace;
	pFace = new CFace3d(m_ArrayVertex[0],
		m_ArrayVertex[1],
		m_ArrayVertex[3]);
	pFace->SetNormal(0.0f,0.0f,-1.0f);
	m_ArrayFace.Add(pFace);
	
	pFace = new CFace3d(m_ArrayVertex[3],
		m_ArrayVertex[1],
		m_ArrayVertex[2]);
	pFace->SetNormal(0.0f,0.0f,-1.0f);
	m_ArrayFace.Add(pFace);
	
	pFace = new CFace3d(m_ArrayVertex[0],
		m_ArrayVertex[4],
		m_ArrayVertex[1]);
	pFace->SetNormal(-1.0f,0.0f,0.0f);
	m_ArrayFace.Add(pFace);
	
	pFace = new CFace3d(m_ArrayVertex[1],
		m_ArrayVertex[4],
		m_ArrayVertex[5]);
	pFace->SetNormal(-1.0f,0.0f,0.0f);
	m_ArrayFace.Add(pFace);
	
	pFace = new CFace3d(m_ArrayVertex[3],
		m_ArrayVertex[2],
		m_ArrayVertex[7]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(1.0f,0.0f,0.0f);
	
	pFace = new CFace3d(m_ArrayVertex[7],
		m_ArrayVertex[2],
		m_ArrayVertex[6]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(1.0f,0.0f,0.0f);
	
	pFace = new CFace3d(m_ArrayVertex[4],
		m_ArrayVertex[0],
		m_ArrayVertex[3]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f,-1.0f,0.0f);
	
	pFace = new CFace3d(m_ArrayVertex[7],
		m_ArrayVertex[4],
		m_ArrayVertex[3]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f,-1.0f,0.0f);
	
	pFace = new CFace3d(m_ArrayVertex[6],
		m_ArrayVertex[4],
		m_ArrayVertex[7]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f,0.0f,1.0f);
	
	pFace = new CFace3d(m_ArrayVertex[6],
		m_ArrayVertex[5],
		m_ArrayVertex[4]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f,0.0f,1.0f);
	
	pFace = new CFace3d(m_ArrayVertex[1],
		m_ArrayVertex[5],
		m_ArrayVertex[6]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f,1.0f,0.0f);
	
	pFace = new CFace3d(m_ArrayVertex[2],
		m_ArrayVertex[1],
		m_ArrayVertex[6]);
	m_ArrayFace.Add(pFace);
	pFace->SetNormal(0.0f,1.0f,0.0f);
	
	TRACE("ok\n");
	
	return 1;
}

//********************************************
// GenerateMap
//********************************************
int CMesh3d::GenerateMap(int line,
						 int col,
						 float min,
						 float max)
{
	TRACE("Generate map...");
	float x,y,z;
	int i,j;
	
	// Set vertices
	for(i=0;i<col;i++)
		for(j=0;j<line;j++)
		{
			x = min + ((float)i/(float)line)*(max-min);
			z = min + ((float)j/(float)line)*(max-min);
			y = (float)(cos(x)*cos(z));
			m_ArrayVertex.Add(new CVertex3d(x,y,z));
		}
		
		// Set faces
		for(i=0;i<col-1;i++)
			for(j=0;j<line-1;j++)
			{
				CVertex3d *pVertex1 = m_ArrayVertex[line*i+j];
				CVertex3d *pVertex2 = m_ArrayVertex[line*i+j+1];
				CVertex3d *pVertex3 = m_ArrayVertex[line*(i+1)+j+1];
				CVertex3d *pVertex4 = m_ArrayVertex[line*(i+1)+j];
				
				m_ArrayFace.Add(new CFace3d(pVertex1,pVertex2,pVertex3));
				m_ArrayFace.Add(new CFace3d(pVertex1,pVertex3,pVertex4));
			}
			
			TRACE("ok\n");
			return 1;
}


//********************************************
// GenerateMapFromImage
//********************************************
int CMesh3d::GenerateMap(CTexture *pTexture,
						 int width,
						 int height,
						 int FlagColor /* = 1 */)
{
	// Cleanup
	Free();
	
	int WidthImage = pTexture->GetWidth();
	int HeightImage = pTexture->GetHeight();
	unsigned char red,green,blue;
	
	// Vertices
	for(int j=0;j<height;j++)
		for(int i=0;i<width;i++)
		{
			int xImage = (int)((float)i/(float)width*(float)WidthImage);
			int yImage = (int)((float)j/(float)height*(float)HeightImage);
			int index = m_ArrayVertex.Add(new CVertex3d((float)i,(float)pTexture->Grey(xImage,yImage),(float)j));
			if(FlagColor)
			{
				pTexture->Color(xImage,yImage,&red,&green,&blue);
				m_ArrayVertex[index]->SetColor(red,green,blue);
			}
		}
		
		// Faces
		for(j=0;j<height-1;j++)
			for(int i=0;i<width-1;i++)
			{
				int index = m_ArrayFace.Add(new CFace3d(m_ArrayVertex[j*width+i+1],
					m_ArrayVertex[j*width+i],
					m_ArrayVertex[(j+1)*width+i+1]));
				m_ArrayFace.Add(new CFace3d(m_ArrayVertex[(j+1)*width+i+1],
					m_ArrayVertex[j*width+i],
					m_ArrayVertex[(j+1)*width+i]));
				if(FlagColor)
				{
					m_ArrayFace[index]->SetColor(*m_ArrayVertex[j*width+i]->GetColor());
					m_ArrayFace[index+1]->SetColor(*m_ArrayVertex[j*width+i+1]->GetColor());
				}
			}
			
			// Rebuild
			BuildAdjacency();
			CalculateNormalPerFace();
			CalculateNormalPerVertex();
			
			return 1;
}

/////////////////////////////////////////////
// INTERSECTION
/////////////////////////////////////////////

//********************************************
// NearestIntersectionWithLine
// Non-optimized
// Nearest -> distance from pV0 to pVertexResult
//********************************************
int CMesh3d::NearestIntersectionWithLine(CVertex3d *pV0,
										 CVertex3d *pV1,
										 CVertex3d *pVertexResult,
										 CFace3d **ppFaceResult,
										 int *pNbFaceVisited)
{
	return ::NearestIntersectionWithLine(&m_ArrayFace,pV0,pV1,pVertexResult,ppFaceResult,pNbFaceVisited);
}

/////////////////////////////////////////////
// I/O
/////////////////////////////////////////////

//********************************************
// WriteFile
//********************************************
int CMesh3d::WriteFile(CStdioFile &file)
{
	CString string;
	TRY
	{
		// Comment
		string.Format("# Mesh : %d vertices, %d faces\n",NbVertex(),NbFace());
		file.WriteString(string);
		
		// First line
		file.WriteString("DEF Mesh-ROOT Transform {\n");
		
		// Transform
		string.Format("  translation %g %g %g\n",m_Transform.GetTranslation()->x(),
			m_Transform.GetTranslation()->y(),
			m_Transform.GetTranslation()->z());
		file.WriteString(string);
		string.Format("  rotation %g %g %g %g\n",m_Transform.GetRotation()->x(),
			m_Transform.GetRotation()->y(),
			m_Transform.GetRotation()->z(),
			m_Transform.GetValueRotation()/360.0f*2*3.14159265359f);
		file.WriteString(string);
		string.Format("  scale %g %g %g\n",m_Transform.GetScale()->x(),
			m_Transform.GetScale()->y(),
			m_Transform.GetScale()->z());
		file.WriteString(string);
		
		// Material
		file.WriteString("  children [\n");
		file.WriteString("    Shape {\n");
		file.WriteString("      appearance Appearance {\n");
		file.WriteString("        material Material {\n");
		file.WriteString("          diffuseColor 0 0 0\n"); // todo
		file.WriteString("        }\n");
		file.WriteString("      }\n");
		
		// Geometry
		file.WriteString("      geometry DEF Mesh-FACES IndexedFaceSet {\n");
		file.WriteString("        ccw TRUE\n");
		file.WriteString("        solid TRUE\n");
		
		// Vertices
		file.WriteString("        coord DEF Mesh-COORD Coordinate { point [\n");
		int NbVertex = m_ArrayVertex.GetSize();
		for(int i=0;i<NbVertex;i++)
		{
			string.Format("          %g %g %g",m_ArrayVertex[i]->x(),
				m_ArrayVertex[i]->y(),
				m_ArrayVertex[i]->z());
			file.WriteString(string);
			if(i!=(NbVertex-1))
				file.WriteString(",\n");
			else
				file.WriteString("]\n");
		}
		file.WriteString("        }\n");
		
		// Faces
		file.WriteString("        coordIndex [\n");
		int NbFace = m_ArrayFace.GetSize();
		for(i=0;i<NbFace;i++)
		{
			string.Format("          %d, %d, %d, -1",m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v(0)),
				m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v(1)),
				m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v(2)));
			file.WriteString(string);
			if(i!=(NbFace-1))
				file.WriteString(",\n");
			else
				file.WriteString("]\n");
		}
		
		// End
		file.WriteString("        }\n");
		file.WriteString("      }\n");
		file.WriteString("    ]\n");
		file.WriteString("  }\n\n");
		
	}
	CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during writing transform" << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during writing transform");
		return 0;
	}
	END_CATCH
		
		return 1;
}

//********************************************
// WriteFileRaw (binary raw mode)
//********************************************
int CMesh3d::WriteFileRaw(CFile &file)
{
	// A mesh : 
	//*******************************************
	// Transform  : 10 * float 32 bits
	// NbVertices : UINT 32 bits
	// NbFaces    : UINT 32 bits
	// Vertices   : x y z    : 3 x float 32 bits
	// Faces      : v1 v2 v3 : 3 x UINT 32 bits
	//*******************************************
	// Cost : 40 + 8 + 12*(v+f) bytes
	
	CString string;
	TRY
	{
		
		// Transform
		
		// Translation (xyz)
		float x,y,z;
		x = m_Transform.GetTranslation()->x();
		y = m_Transform.GetTranslation()->y();
		z = m_Transform.GetTranslation()->z();
		file.Write(&x,sizeof(float));
		file.Write(&y,sizeof(float));
		file.Write(&z,sizeof(float));
		
		// Rotation (xyz)
		x = m_Transform.GetRotation()->x();
		y = m_Transform.GetRotation()->y();
		z = m_Transform.GetRotation()->z();
		float v = m_Transform.GetValueRotation();
		file.Write(&x,sizeof(float));
		file.Write(&y,sizeof(float));
		file.Write(&z,sizeof(float));
		file.Write(&v,sizeof(float));
		
		// Scale (xyz)
		x = m_Transform.GetScale()->x();
		y = m_Transform.GetScale()->y();
		z = m_Transform.GetScale()->z();
		file.Write(&x,sizeof(float));
		file.Write(&y,sizeof(float));
		file.Write(&z,sizeof(float));
		
		// Geometry 
		// NbVertices
		// NbFaces
		
		unsigned int NbVertex = m_ArrayVertex.GetSize();
		unsigned int NbFace = m_ArrayFace.GetSize();
		file.Write(&NbVertex,sizeof(unsigned int));
		file.Write(&NbFace,sizeof(unsigned int));
		
		// Vertices
		for(unsigned int i=0;i<NbVertex;i++)
		{
			x = m_ArrayVertex[i]->x();
			y = m_ArrayVertex[i]->y();
			z = m_ArrayVertex[i]->z();
			file.Write(&x,sizeof(float));
			file.Write(&y,sizeof(float));
			file.Write(&z,sizeof(float));
		}
		
		// Faces
		unsigned int v1,v2,v3;
		for(i=0;i<NbFace;i++)
		{
			v1 = m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v1());
			v2 = m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v2());
			v3 = m_ArrayVertex.IndexFrom(m_ArrayFace[i]->v3());
			file.Write(&v1,sizeof(unsigned int));
			file.Write(&v2,sizeof(unsigned int));
			file.Write(&v3,sizeof(unsigned int));
		}
		
	}
	CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during writing " << e->m_cause << "\n";
#endif
		AfxMessageBox("Error during writing");
		return 0;
	}
	END_CATCH
		
		return 1;
}


//********************************************
// GetMinArea
//********************************************
double CMesh3d::GetMinArea(CFace3d **ppFace /* NULL */)
{
	double min = MAX_DOUBLE;
	int size = m_ArrayFace.GetSize();
	for(int i=0;i<size;i++)
	{
		double area = m_ArrayFace[i]->Area();
		if(area < min)
		{
			min = area;
			if(ppFace != NULL)
				*ppFace = m_ArrayFace[i];
		}
	}
	return min;
}

//********************************************
// GetMeanArea
//********************************************
double CMesh3d::GetMeanArea()
{
	return ::GetMeanArea(&m_ArrayFace);
}

//*********************************
// GenerateEdgeArray 
//*********************************
int CMesh3d::GenerateEdgeArray(CArray3d<CEdge3d> *pArrayEdge,
							   BOOL FlagOnBoundary,
							   char flag,
							   BOOL *pHasBoundary)
{
	ASSERT(pArrayEdge != NULL);
	
	int NbVertex = m_ArrayVertex.GetSize();
	
	// Set flags
	SetFlagOnVertices(0);
	
	TRACE("  begin generate edge array\n");
	TRACE("    %d vertices\n",NbVertex);
	TRACE("    %d faces\n",NbFace());
	int NbEdgePrevious = NbVertex+NbFace();
	TRACE("    %d edges previous\n",NbEdgePrevious);
	pArrayEdge->SetSize(NbEdgePrevious);
	int NbEdge = 0;
	TRACE("  begin...");
	for(int i=0;i<NbVertex;i++)
	{
		CVertex3d *pVertex = m_ArrayVertex[i];
		int NbVertexNeighbor = pVertex->NbVertexNeighbor();
		for(int j=0;j<NbVertexNeighbor;j++)
		{
			CVertex3d *pNeighbor = pVertex->GetVertexNeighbor(j);
			ASSERT(pNeighbor != NULL);
			if(pNeighbor->GetFlag() == 0)
			{
				// Alloc
				CEdge3d *pEdge = new CEdge3d(pVertex,pNeighbor);
				
				// Set faces (at least one)
				CArray3d<CFace3d> array;
				pVertex->FindFaceAroundContainVertex(pNeighbor,array);
				ASSERT(array.GetSize() >= 1);
				pEdge->f1(array[0]);
				if(array.GetSize() > 1)
					pEdge->f2(array[1]);
				
				if(NbEdge<NbEdgePrevious)
					pArrayEdge->SetAt(NbEdge,pEdge);
				else
					pArrayEdge->Add(pEdge);
				NbEdge++;
				
				// Flag (optional)
				if(FlagOnBoundary)
					if(pVertex->IsOnBoundary() && pNeighbor->IsOnBoundary())
					{
						*pHasBoundary = TRUE;
						pEdge->SetFlag(flag);
					}
			}
		}
		pVertex->SetFlag(1); // done
		
		//if(i%(NbVertex/30)==0) { TRACE("."); }
		
	}
	TRACE("ok\n");
	pArrayEdge->SetSize(NbEdge);
	TRACE("    %d edges\n",NbEdge);
	
	return 1;
}


//********************************************
// glDrawProjectLine
// paint the current mesh via projection 
// in line mode
//********************************************
void CMesh3d::glDrawProjectLine(CDC *pDC,
								double *modelMatrix,
								double *projMatrix,
								int *viewport,
								COLORREF ColorLine,
								double ratio,
								int height)
{
	TRACE("Draw projected mesh in metafile-based device context\n");
	TRACE("  line mode\n");
	TRACE("  viewport : (%d;%d;%d;%d)\n",viewport[0],viewport[1],viewport[2],viewport[3]);
	TRACE("  model : %g\t%g\t%g\n",modelMatrix[0],modelMatrix[1],modelMatrix[2]);
	TRACE("          %g\t%g\t%g\n",modelMatrix[3],modelMatrix[4],modelMatrix[5]);
	TRACE("          %g\t%g\t%g\n",modelMatrix[6],modelMatrix[7],modelMatrix[8]);
	TRACE("   proj : %g\t%g\t%g\n",projMatrix[0],projMatrix[1],projMatrix[2]);
	TRACE("          %g\t%g\t%g\n",projMatrix[3],projMatrix[4],projMatrix[5]);
	TRACE("          %g\t%g\t%g\n",projMatrix[6],projMatrix[7],projMatrix[8]);
	
	// Generate edge array (it saves memory)
	CArray3d<CEdge3d> ArrayEdge;
	GenerateEdgeArray(&ArrayEdge);
	
	// Select pen
	CPen pen(PS_SOLID,0,ColorLine);
	CPen *pOldPen = pDC->SelectObject(&pen);
	
	double x1,y1,x2,y2,z;
	for(int i=0;i<ArrayEdge.GetSize();i++)
	{
		CEdge3d *pEdge = ArrayEdge[i];
		ASSERT(pEdge != NULL);
		
		gluProject((double)pEdge->v1()->x(),
			(double)pEdge->v1()->y(),
			(double)pEdge->v1()->z(),
			modelMatrix,
			projMatrix,
			viewport,&x1,&y1,&z);
		
		gluProject((double)pEdge->v2()->x(),
			(double)pEdge->v2()->y(),
			(double)pEdge->v2()->z(),
			modelMatrix,
			projMatrix,
			viewport,&x2,&y2,&z);
		
		// Crop to window
		if(x1 < viewport[0] || y1 < viewport[1] || 
			x1 > viewport[2] || y1 > viewport[3] ||
			x2 < viewport[0] || y2 < viewport[1] || 
			x2 > viewport[2] || y2 > viewport[3])
			continue; // crop to window
		else // draw
		{
			pDC->MoveTo((int)(ratio*x1),(int)(ratio*((float)height-y1)));
			pDC->LineTo((int)(ratio*x2),(int)(ratio*((float)height-y2)));
		}
	}
	// Cleanup
	ArrayEdge.Free();
	pDC->SelectObject(pOldPen);
}


//********************************************
// glDrawProjectFace
//********************************************
void CMesh3d::glDrawProjectFace(CDC *pDC,
								double *modelMatrix,
								double *projMatrix,
								int *viewport,
								COLORREF ColorLine,
								COLORREF ColorFace,
								double ratio,
								int height, // window height
								float RatioNbFace) // default -> 1.0
{
	TRACE("Draw projected mesh in metafile-based device context\n");
	TRACE("  face mode\n");
	TRACE("  viewport : (%d;%d;%d;%d)\n",viewport[0],viewport[1],viewport[2],viewport[3]);
	TRACE("  model : %g\t%g\t%g\n",modelMatrix[0],modelMatrix[1],modelMatrix[2]);
	TRACE("          %g\t%g\t%g\n",modelMatrix[3],modelMatrix[4],modelMatrix[5]);
	TRACE("          %g\t%g\t%g\n",modelMatrix[6],modelMatrix[7],modelMatrix[8]);
	TRACE("   proj : %g\t%g\t%g\n",projMatrix[0],projMatrix[1],projMatrix[2]);
	TRACE("          %g\t%g\t%g\n",projMatrix[3],projMatrix[4],projMatrix[5]);
	TRACE("          %g\t%g\t%g\n",projMatrix[6],projMatrix[7],projMatrix[8]);
	CWmfFace *pArray = new CWmfFace[m_ArrayFace.GetSize()];
	
	// Many thanks to Gaspard Breton for having 
	// implemented the AVL fast z-sorting part.
	ASSERT(pArray);
	CAVL<CWmfFace,double> avl; 
	CWmfFace bidon;
	avl.Register(&bidon,&bidon.zc,&bidon.avl); // z as key
	
	int NbFaces = m_ArrayFace.GetSize();
	TRACE("  %d faces\n",NbFaces);
	int NbFacesToProcess = (int)(RatioNbFace*(float)NbFaces);
	TRACE("  %d faces to process\n",NbFacesToProcess);
	TRACE("  begin sort...");
	int NbFaceValid = 0;
	for(int i=0;i<NbFaces;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		
		// Compute barycenter as z-reference
		// Sorting by a triangle average depth does not allow
		// to disambiguate some cases.  Handling these cases would 
		// require breaking up the primitives. Please mail any
		// improvement about this
		double xc = (pFace->v1()->x()+pFace->v2()->x()+pFace->v3()->x())/3;
		double yc = (pFace->v1()->y()+pFace->v2()->y()+pFace->v3()->y())/3;
		double zc = (pFace->v1()->z()+pFace->v2()->z()+pFace->v3()->z())/3;
		
		// Project center
		gluProject(xc,yc,zc,
			modelMatrix,
			projMatrix,
			viewport,&pArray[i].xc,&pArray[i].yc,&pArray[i].zc);
		
		// Project three vertices
		gluProject((double)pFace->v1()->x(),
			(double)pFace->v1()->y(),
			(double)pFace->v1()->z(),
			modelMatrix,
			projMatrix,
			viewport,&pArray[i].x1,&pArray[i].y1,&pArray[i].z1);
		
		gluProject((double)pFace->v2()->x(),
			(double)pFace->v2()->y(),
			(double)pFace->v2()->z(),
			modelMatrix,
			projMatrix,
			viewport,&pArray[i].x2,&pArray[i].y2,&pArray[i].z2);
		
		gluProject((double)pFace->v3()->x(),
			(double)pFace->v3()->y(),
			(double)pFace->v3()->z(),
			modelMatrix,
			projMatrix,
			viewport,&pArray[i].x3,&pArray[i].y3,&pArray[i].z3);
		
		// Crop & sort
		if(pArray[i].x1 < viewport[0]  || pArray[i].y1 < viewport[1]  || 
			pArray[i].x1 > viewport[2]  || pArray[i].y1 > viewport[3]  ||
			pArray[i].x2 < viewport[0]  || pArray[i].y2 < viewport[1]  || 
			pArray[i].x2 > viewport[2]  || pArray[i].y2 > viewport[3]  ||
			pArray[i].x3 < viewport[0]  || pArray[i].y3 < viewport[1]  || 
			pArray[i].x3 > viewport[2]  || pArray[i].y3 > viewport[3])
			continue;
		else
		{
			pArray[i].m_Draw = 1;  // yes, insert this triangle
			pArray[i].zc *= -1.0f; // back to front
			avl.Insert(pArray,i);  // insert via sort
			NbFaceValid++;
		}
	}
	TRACE("ok\n");
	
	// Draw
	CPen pen(PS_SOLID,0,ColorLine);
	CBrush BrushFace(ColorFace);
	CPen *pOldPen = pDC->SelectObject(&pen);
	POINT points[3]; // triangular faces only
	
	// Default
	CBrush *pOldBrush = pDC->SelectObject(&BrushFace);
	TRACE("begin draw...");
	int nb = 0;
	for(i=avl.GetFirst(pArray);
	(AVLNULL != i) && nb < NbFacesToProcess;
	i=avl.GetNext(pArray),nb++)
	{
		// Fill and outline the face
		points[0].x = (int)(ratio*pArray[i].x1);
		points[0].y = (int)(ratio*((float)height-pArray[i].y1));
		points[1].x = (int)(ratio*pArray[i].x2);
		points[1].y = (int)(ratio*((float)height-pArray[i].y2));
		points[2].x = (int)(ratio*pArray[i].x3);
		points[2].y = (int)(ratio*((float)height-pArray[i].y3));
		
		// Fill triangle
		pDC->Polygon(points,3);
		
		// Outline triangle
		pDC->MoveTo(points[0]);
		pDC->LineTo(points[1]);
		pDC->LineTo(points[2]);
		pDC->LineTo(points[0]);
	}
	TRACE("ok\n");
	
	// Restore and cleanup
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	delete [] pArray;
}

// ** EOF **
void CMesh3d::GPDEbaseSubdivision()
{
	
}

/************************************************************************/
/* Yang's scheme                                                        */
/************************************************************************/
void CMesh3d::NormSubd()
{
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	// We assume adjacency is built
	int NbVertex = m_ArrayVertex.GetSize();
	SetFlagOnVertices(0); // for boundaries
	
	// Create subdivision info (edge vertices)
	int NbFace = m_ArrayFace.GetSize();
	// For each face
	for(int i=0;i<NbFace;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		// Check valid neighboring
		if(!pFace->IsValid()) continue;
		// On each edge
		for(int IndexEdge=0;IndexEdge<3;IndexEdge++)
		{
			// Get IndexEdge on neighbor
			int IndexCurrent,IndexNeighbor;
			CFace3d *pFaceNeighbor = pFace->f(IndexEdge);
			// No neighbor on this edge, it is a boundary edge
			if(pFaceNeighbor == NULL)
			{
				// Two vertices involved
				CVertex3d *pVertex[2];
				pVertex[0] = pFace->v(IndexEdge);
				pVertex[1] = pFace->v((IndexEdge+1)%3);
				float x = 0.5f*(pVertex[0]->x()+pVertex[1]->x());
				float y = 0.5f*(pVertex[0]->y()+pVertex[1]->y());
				float z = 0.5f*(pVertex[0]->z()+pVertex[1]->z());
				CVertex3d *pNewVertex = new CVertex3d(x,y,z);
				m_ArrayVertex.Add(pNewVertex);
				pFace->v(3+IndexEdge,pNewVertex);
				// Boundary vertices
				pVertex[0]->SetFlag(1);
				pVertex[1]->SetFlag(1);
			}
			else // std case
			{
				VERIFY(pFace->Share2Vertex(pFaceNeighbor,&IndexCurrent,&IndexNeighbor));
				ASSERT(IndexCurrent == IndexEdge);
				// If neighboring face has been treated, then get vertex
				// and go to next step
				if(pFaceNeighbor->GetFlag())
				{
					CVertex3d *pVertex = pFaceNeighbor->v(3+(IndexNeighbor%3));
					pFace->v(3+(IndexEdge%3),pVertex);
					continue;
				}
				//这一段等于新点几何位置的确定
				CVertex3d *pVertex[4];
				// current edge
				pVertex[0] = pFace->v(IndexEdge);
				pVertex[1] = pFace->v((IndexEdge+1)%3);
				// Weight : 1
				pVertex[2] = pFace->v((IndexEdge+2)%3);
				pVertex[3] = pFaceNeighbor->v((IndexNeighbor+2)%3);
				ASSERT(pVertex[0] != NULL && pVertex[1] != NULL);
				CVector3d* n1 = pVertex[0]->GetNormal();
				CVector3d* n2 = pVertex[1]->GetNormal();
				CVector3d edge1, edge2; 
				edge1.Set(pVertex[1], pVertex[0]);
				edge2.Set(pVertex[0], pVertex[1]);
				CVector3d v1, v2;
				*n1 *= 0.5f*(float)(Scalar(n1, &edge1));
				*n2 *= 0.5f*(float)(Scalar(n2, &edge2));
				// For each composant
				float x = 0.5f*(pVertex[0]->x()+pVertex[1]->x())+
					0.1f*(n1->x()+n2->x());
				float y = 0.5f*(pVertex[0]->y()+pVertex[1]->y())+
					0.6f*(n1->y()+n2->y());
				float z = 0.5f*(pVertex[0]->z()+pVertex[1]->z())+
					0.6f*(n1->z()+n2->z());
				// Add vertex to global mesh array, and set face's vertex
				CVertex3d *pNewVertex = new CVertex3d(x,y,z);
				m_ArrayVertex.Add(pNewVertex);
				pFace->v(3+IndexCurrent,pNewVertex);
			}
		}
		// Set flag
		pFace->SetFlag(1); 
	}
	
	//*****************************
	// Create faces
	//*****************************
	// For each valid face
	for(i=0;i<NbFace;i++)
	{
		CFace3d *pFace = m_ArrayFace[i];
		pFace->SetFlag(0);
		
		// Valid face
		int NbVertex = pFace->NbVertex();
		CFace3d *pNewFace;
		switch(NbVertex)
		{
		case 4:
			// Create one face
			// On edge 0
			if(pFace->v(3) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(2));
				m_ArrayFace.Add(pNewFace);
				// Move current face
				pFace->v(1,pFace->v(3));
			}
			else
				// On edge 1
				if(pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(0),pFace->v(4),pFace->v(2));
					m_ArrayFace.Add(pNewFace);
					// Move current face
					pFace->v(2,pFace->v(4));
				}
				else
					// On edge 2
					if(pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(5),pFace->v(1),pFace->v(2));
						m_ArrayFace.Add(pNewFace);
						// Move current face
						pFace->v(2,pFace->v(5));
					}
					break;
		case 5:
			// Create two faces
			// On edge 0 & 2
			if(pFace->v(3) != NULL && 
				pFace->v(5) != NULL)
			{
				pNewFace = new CFace3d(pFace->v(0),pFace->v(3),pFace->v(5));
				m_ArrayFace.Add(pNewFace);
				pNewFace = new CFace3d(pFace->v(5),pFace->v(3),pFace->v(2));
				m_ArrayFace.Add(pNewFace);
				// Move current face
				pFace->v(0,pFace->v(3));
			}
			else
				// On edge 0 & 1
				if(pFace->v(3) != NULL && pFace->v(4) != NULL)
				{
					pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					pNewFace = new CFace3d(pFace->v(0),pFace->v(3),pFace->v(4));
					m_ArrayFace.Add(pNewFace);
					// Move current face
					pFace->v(1,pFace->v(4));
				}
				else
					// On edge 1 & 2
					if(pFace->v(4) != NULL && 
						pFace->v(5) != NULL)
					{
						pNewFace = new CFace3d(pFace->v(1),pFace->v(4),pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						pNewFace = new CFace3d(pFace->v(4),pFace->v(2),pFace->v(5));
						m_ArrayFace.Add(pNewFace);
						// Move current face
						pFace->v(2,pFace->v(5));
					}
					break;
		case 6:
			// Create three faces
			// First (v3,v1,v4)
			pNewFace = new CFace3d(pFace->v(3),pFace->v(1),pFace->v(4));
			m_ArrayFace.Add(pNewFace);
			// Second (v3,v4,v5)
			pNewFace = new CFace3d(pFace->v(3),pFace->v(4),pFace->v(5));
			m_ArrayFace.Add(pNewFace);
			// Third (v5,v4,v2)
			pNewFace = new CFace3d(pFace->v(5),pFace->v(4),pFace->v(2));
			m_ArrayFace.Add(pNewFace);
			// Move current face
			pFace->v(1,pFace->v(3));
			pFace->v(2,pFace->v(5));
			
			break;
		}
		// Remove subdivision info
		for(int k=3;k<6;k++)
			pFace->v(k,NULL);
		
	}
	TRACE("  end creating faces (%d faces)\n",m_ArrayFace.GetSize());
	
	
	// 	//*****************************
	// 	// Move original vertices
	// 	//*****************************
	// 
	// 	// Copy
	// 	TRACE("  copy\n");
	// 	CArray3d<CVertex3d> ArrayVertex;
	// 	ArrayVertex.SetSize(NbVertex);
	// 	for(i=0;i<NbVertex;i++)
	// 		ArrayVertex.SetAt(i,new CVertex3d);
	// 
	// 	// For each vertex (at least 2 neighbors)
	// 	for(i=0;i<NbVertex;i++)
	// 	{
	// 		CVertex3d *pVertex = m_ArrayVertex[i];
	// 		int n = pVertex->NbVertexNeighbor();
	// 
	// 		// Spline on boundaries, we need two vertices
	// 		// remind that the adjacency has not been 
	// 		// updated yet
	// 		// weighting: 1 6 1
	// 		if(pVertex->GetFlag())
	// 		{
	// 			CVertex3d *pv[2];
	// 			int index = 0;
	// 			for(int k=0;k<n;k++)
	// 			{
	// 				CVertex3d *pNVertex = pVertex->GetVertexNeighbor(k);
	// 				if(pNVertex->GetFlag())
	// 					pv[index++] = pNVertex;
	// 			}
	// 			ASSERT(index == 2);
	// 			ArrayVertex[i]->Set(0.125f*(6.0f*pVertex->x()+pv[0]->x()+pv[1]->x()),
	// 				                  0.125f*(6.0f*pVertex->y()+pv[0]->y()+pv[1]->y()),
	// 													0.125f*(6.0f*pVertex->z()+pv[0]->z()+pv[1]->z()));
	// 		}
	// 		else
	// 		{
	// 			float alpha = Alpha(n);
	// 			float tmp = alpha + (float)n;
	// 
	// 			// For each componant
	// 			for(unsigned int j=0;j<3;j++)
	// 			{
	// 				float value = alpha * pVertex->Get(j);
	// 				for(int k=0;k<n;k++)
	// 					value += pVertex->GetVertexNeighbor(k)->Get(j);
	// 				value /= tmp;
	// 				ArrayVertex[i]->Set(j,value);
	// 			}
	// 		}
	// 	}
	// 
	// 	// Restore
	// 	TRACE("  restore\n");
	// 	for(i=0;i<NbVertex;i++)
	// 		for(unsigned int j=0;j<3;j++)
	// 			m_ArrayVertex[i]->Set(j,ArrayVertex[i]->Get(j));
	
	// 	ArrayVertex.Free();
	
	// Rebuild adjacency and normals
	BuildAdjacency();
	CalculateNormalPerFace();
	CalculateNormalPerVertex();
	m_Modified = 1;
	
	TRACE("End loop's subdivision (%d faces, %d vertices)\n",
		m_ArrayFace.GetSize(),m_ArrayVertex.GetSize());
}

/************************************************************************/
/* Construct VBOs                                                       */
/************************************************************************/
void CMesh3d::BuildVBOs()
{
// 	// Generate And Bind The Vertex Buffer 
// 	glGenBuffersARB(1, &m_nVBOVertices);         // Get A Valid Name 
// 	glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_nVBOVertices); // Bind The Buffer 
// 	// Load The Data 
// 	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
// 					m_ArrayVertex.GetSize()*sizeof(CVertex3d), 
// 					&m_ArrayVertex,  
// 					GL_STATIC_DRAW_ARB ); 
// 	// Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card 
// 	m_ArrayVertex.Free();
} 

void CMesh3d::GenPrinAxes()
{
	int nV = NbVertex();
	Matrix<float> m1(3, nV), m2(nV, 3), m;
	for (int i = 0; i < nV; ++i) {
	  m1[0][i] = m_ArrayVertex[i]->x(); m1[1][i] = m_ArrayVertex[i]->y(); m1[2][i] = m_ArrayVertex[i]->z();
	  m2[i][0] = m_ArrayVertex[i]->x(); m2[i][1] = m_ArrayVertex[i]->y(); m2[i][2] = m_ArrayVertex[i]->z();
	}
	m = m1 * m2;
	Array2D<float> mat(3,3);
	float mm[3][3];
	for (i = 0; i < 3; ++i)
	  for (int j = 0; j < 3; ++j) mm[i][j] = mat[i][j] = m[i][j];
	Eigenvalue<float> eV(mat);
	eV.getV(m_prinAxes);
    for (i = 0; i < 3; ++i)
	  for (int j = 0; j < 3; ++j) mm[i][j] = m_prinAxes[i][j];
	printf("sss");
}

void CMesh3d::Rotate(Vec3f &axis, float &angle)
{
	int nV = NbVertex();
	/*float rotM[16]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle/TWOPI*360, axis.x, axis.y, axis.z);
	glGetFloatv(GL_MODELVIEW_MATRIX, rotM);
	glPopMatrix();
	Matrix<float> Coord(nV, 4);
	for (int i = 0; i < nV; ++i)
	{
		Coord[i][0] = m_ArrayVertex[i]->x();
		Coord[i][1] = m_ArrayVertex[i]->y();
		Coord[i][2] = m_ArrayVertex[i]->z();
		Coord[i][3] = 1;
	}
	Matrix<float> rotMat(4, 4);
	for (i = 0; i < 16; ++i) rotMat[i/4][i-i/4*4] = rotM[i];
	Matrix<float> CoordRot = Coord * rotMat;*/
	float cosa = cos(angle), sina = sin(angle);
	float t = 1 - cosa;
	float x = axis[0], y = axis[1], z = axis[2], originalx, originaly, originalz, nx, ny, nz;
	for (int i = 0; i < nV; ++i) 
	{
		originalx = m_ArrayVertex[i]->x();
		originaly = m_ArrayVertex[i]->y();
		originalz = m_ArrayVertex[i]->z();
		nx = (t * x * x + cosa) * originalx + (t * x * y - sina * z) * originaly + (t * x * z + sina * y) * originalz;
		ny = (t * x * y + sina * z) * originalx + (t * y * y + cosa) * originaly + (t * y * z - sina * x) * originalz;
		nz = (t * x * z - sina * y) * originalx + (t * y * z + sina * x) * originaly + (t * z * z + cosa) * originalz;
		m_ArrayVertex[i]->Set(nx, ny, nz);
	}
	BuildAnnKDTree();
	ComputeLocalCenters();
}

void CMesh3d::ComputeCurvature() {

}

void CMesh3d::BuildAnnKDTree() {
	int n = NbVertex();
	Vert_Array = annAllocPts(n, 6);
	V_Array = annAllocPts(n, 3);
	for (int i = 0; i < n; i++) {
		Vert_Array[i][0] = V_Array[i][0] = m_ArrayVertex[i]->x();
		Vert_Array[i][1] = V_Array[i][1] = m_ArrayVertex[i]->y();
		Vert_Array[i][2] = V_Array[i][2] = m_ArrayVertex[i]->z();
		Vert_Array[i][3] = m_ArrayVertex[i]->GetNormal()->x()*normalToVertexRatio;
		Vert_Array[i][4] = m_ArrayVertex[i]->GetNormal()->y()*normalToVertexRatio;
		Vert_Array[i][5] = m_ArrayVertex[i]->GetNormal()->z()*normalToVertexRatio;
	}
	//Build kd Tree for nearest distance searching
	pVert_Tree6 = new ANNkd_tree(Vert_Array, n, 6);
	pVert_Tree3 = new ANNkd_tree(V_Array, n, 3);
}

float CMesh3d::ComputAveNearestDis(CVertex3d* pv) {
	ANNdistArray dists= new ANNdist[1];	
	ANNidxArray idx= new ANNidx[1];
	ANNpoint p = annAllocPt(3);
	p[0] = pv->x(); p[1] = pv->y(); p[2] = pv->z();
	pVert_Tree3->annkSearch(p, 1, idx, dists, 0);
	return dists[0];
}

float CMesh3d::ComputAveNearestDis(CMesh3d *pMesh) {
	float aveNearestDis = 0.0f;
	int k = pMesh->NbVertex();
	for (int i = 0; i < k; ++i)
		aveNearestDis += ComputAveNearestDis(pMesh->m_ArrayVertex[i]);
	return aveNearestDis/k;
}

void CMesh3d::CopyVertex(CMesh3d* pMesh)
{
	if (NbVertex() != pMesh->NbVertex()) 
	{
		m_ArrayVertex.SetSize(pMesh->NbVertex());
	}
	int size = NbVertex();
	for (int i = 0; i < size; ++i)
	{
		m_ArrayVertex.SetAt(i, new CVertex3d(pMesh->m_ArrayVertex[i]->x(), 
							  pMesh->m_ArrayVertex[i]->y(), 
							  pMesh->m_ArrayVertex[i]->z()));
		m_ArrayVertex[i]->Set(pMesh->m_ArrayVertex[i]->x(), 
							  pMesh->m_ArrayVertex[i]->y(), 
							  pMesh->m_ArrayVertex[i]->z());
	}
}

float CMesh3d::ComputeAGD(CVertex3d *pVert)
{
	float d = 0.0f;

	// Generate edge array (it saves memory)
	CArray3d<CEdge3d> ArrayEdge;
	GenerateEdgeArray(&ArrayEdge);
	//Compute edge length
	int eSize = ArrayEdge.GetSize();
	for (int i = 0; i < eSize; ++i) ArrayEdge[i]->ComputeLength();

	//Dijkstra


	return d;
}

/************************************************************************/
/* For the whole mesh, we add short cut edges using hilaga's scheme     */
/************************************************************************/
void CMesh3d::AddShortCuts()
{
	//for each face, rotate three neighbor faces into its same plane
	//compute short cut edges (adding additional neighbors to vertexes)
	for (int i = 0; i < NbFace(); ++i)
	{
		//m_ArrayFace[i]->f1()
	}
}

/************************************************************************/
/* Build consistent correspondences                                     */
/************************************************************************/ 
void CMesh3d::BuildConsistCorrespondences(CMesh3d* pTargetMesh, int k) {
  int size = NbVertex(), i, j, l;
  int* ind;
  // consistent correspondences
  vector<int> largestMerg;
  bool hasMerged = false;
  BuildCorrespondences(pTargetMesh, k);
  // for correspondences found at each vertex, generate patches
  for (i=0; i<size; ++i) {
	  if (m_ArrayVertex[i]->hasCorrespondence) {
	    // using Union-Find data structure to group correspondences
	    int numC = m_ArrayVertex[i]->match.num;
	    ind = new int[numC];
      for (j=0; j<numC; ++j) {
        ind[j] = m_ArrayVertex[i]->match.indices[j];
      }
	    UnionFinder uf(ind, numC);
      // join correspondences which are connected
      CString sj, sl;
      for (j=0; j<numC; ++j) {
        int jj = ind[j];
        if (jj<0) continue;
        for (l=0; l<j; ++l) {
          int ll = ind[l];
          if (ll<0) continue;
          if (pTargetMesh->m_ArrayVertex[ind[j]]->HasNeighbor(pTargetMesh->m_ArrayVertex[ind[l]])==1) 
            uf.join(j,l);
        }
      }
      uf.genGroups();
      if (uf.groupNum>2) {
        CString si, sGN;
        si.Format("%d",i); sGN.Format("%d",uf.groupNum);
        AfxMessageBox(si+"  "+sGN);
      }
      // merging correspondence groups
      for (int ii=0; ii<uf.groupNum; ++ii) {
        Group gi = uf.gps[ii];
        if (!hasMerged) {
          // find the largest group
          int indNumG = -9999999; int bestG = 0;
          if (indNumG>gi.size) {
            indNumG = gi.size;
            bestG = ii;
          }
          // initialize 
        }
        else {
          // merge groups into the current largest correspondence group 
          
        }
      }
    } 
  }
}

void CMesh3d::BuildCorrespondences(CMesh3d* pTargetMesh, int k) {
  int size = NbVertex();
  ANNdistArray dists = new ANNdist[k];	
  ANNidxArray idx = new ANNidx[k];
  ANNdistArray dists1 = new ANNdist[k];	
  ANNidxArray idx1 = new ANNidx[k];
  ANNpoint pSrc = annAllocPt(3);
  CVector3d *nS, *nT;
  float thresDis;
  //find source -----> target correspondence
  for (int i = 0; i < size; ++i) {
    //Find out the nearest vertex in the target mesh for each point in the source mesh
    pSrc[0] = m_ArrayVertex[i]->x(); 
    pSrc[1] = m_ArrayVertex[i]->y(); 
    pSrc[2] = m_ArrayVertex[i]->z();
    nS = m_ArrayVertex[i]->GetNormal();
    Vec3f a(nS->x(), nS->y(), nS->z()); Normalize(a);
    // first find out the nearest suitable target point (check the normal)
    pTargetMesh->pVert_Tree3->annkSearch(pSrc, k, idx1, dists1, 0);
    bool isTarExist = false;
    for (int ii=0; ii<k; ++ii) {
      if (idx1[ii]<0) break;
      nT = pTargetMesh->m_ArrayVertex[idx1[ii]]->GetNormal();    
      Vec3f b(nT->x(), nT->y(), nT->z());
      Normalize(b);
      if (Dot(a,b)>0) {
        thresDis = dists1[ii]*2;
        isTarExist = true;
        break;
      }
    }
    if (!isTarExist) continue;
    pTargetMesh->pVert_Tree3->annkFRSearch(pSrc, thresDis, k, idx, dists, 0);
    //pTargetMesh->pVert_Tree3->annkSearch(pSrc, k, idx, dists, 0);
    for (int j=0; j<k; ++j) {
      if (idx[j]==-1) break;
      nT = pTargetMesh->m_ArrayVertex[idx[j]]->GetNormal();
      Vec3f a(nS->x(), nS->y(), nS->z()), b(nT->x(), nT->y(), nT->z());
      Normalize(a); Normalize(b);
      if (Dot(a,b)>0) {
        m_ArrayVertex[i]->hasCorrespondence = true;
        m_ArrayVertex[i]->match.distance[m_ArrayVertex[i]->match.num] = dists[j];
        m_ArrayVertex[i]->match.indices[m_ArrayVertex[i]->match.num] = idx[j];
        m_ArrayVertex[i]->match.coef[m_ArrayVertex[i]->match.num] = 1.0/k;
        m_ArrayVertex[i]->match.num++;
      }
    }
  }
  //release memory...
  nS=0; nT=0;
  delete nS; delete nT; delete dists; delete idx; delete dists1; delete idx1; 
  annDeallocPt(pSrc);
}

void CMesh3d::BuildSampleCorrespondences(CMesh3d* pTargetMesh, int k) {
  int size = NbVertex();
  ANNdistArray dists = new ANNdist[k];	
  ANNidxArray idx = new ANNidx[k];
  ANNdistArray dists1 = new ANNdist[k];	
  ANNidxArray idx1 = new ANNidx[k];
  ANNpoint pSrc = annAllocPt(3);
  CVector3d *nS, *nT;
  float thresDis;
  //find source -----> target correspondence
  for (int i = 0; i < size; ++i) {
    //Find out the nearest vertex in the target mesh for each point in the source mesh
    pSrc[0] = m_ArrayVertex[i]->x(); 
    pSrc[1] = m_ArrayVertex[i]->y(); 
    pSrc[2] = m_ArrayVertex[i]->z();
    nS = m_ArrayVertex[i]->GetNormal();
    Vec3f a(nS->x(), nS->y(), nS->z()); Normalize(a);
    // first find out the nearest suitable target point (check the normal)
    pTargetMesh->pSample_Tree3->annkSearch(pSrc, k, idx1, dists1, 0);
    bool isTarExist = false;
    for (int ii=0; ii<k; ++ii) {
      if (idx1[ii]<0) break;
      nT = pTargetMesh->m_ArraySample[idx1[ii]]->GetNormal();    
      Vec3f b(nT->x(), nT->y(), nT->z());
      Normalize(b);
      if (Dot(a,b)>0) {
        thresDis = dists1[ii]*2;
        isTarExist = true;
        break;
      }
    }
    if (!isTarExist) continue;
    pTargetMesh->pSample_Tree3->annkFRSearch(pSrc, thresDis, k, idx, dists, 0);
    //pTargetMesh->pVert_Tree3->annkSearch(pSrc, k, idx, dists, 0);
    for (int j=0; j<k; ++j) {
      if (idx[j]==-1) break;
      nT = pTargetMesh->m_ArraySample[idx[j]]->GetNormal();
      Vec3f a(nS->x(), nS->y(), nS->z()), b(nT->x(), nT->y(), nT->z());
      Normalize(a); Normalize(b);
      if (Dot(a,b)<0 || pTargetMesh->m_ArraySample[idx[j]]->hasCorrespondence) {
        m_ArrayVertex[i]->match.indices[j] = -1;
        continue;
      }
      //pTargetMesh->m_ArrayVertex[idx[j]]->hasCorrespondence = true;
      m_ArrayVertex[i]->hasCorrespondence = true;
      m_ArrayVertex[i]->match.distance[m_ArrayVertex[i]->match.num] = dists[j];
      m_ArrayVertex[i]->match.indices[m_ArrayVertex[i]->match.num] = idx[j];
      m_ArrayVertex[i]->match.coef[m_ArrayVertex[i]->match.num] = 1.0/k;
      m_ArrayVertex[i]->match.num++;
    }
  }
  //release memory...
  nS=0; nT=0;
  delete nS; delete nT; delete dists; delete idx; delete dists1; delete idx1; 
  annDeallocPt(pSrc);
}

void CMesh3d::BuildInverseCorrespondences(CMesh3d *pTargetMesh, int k) {
	ANNdistArray dists= new ANNdist[k];	
	ANNidxArray idx= new ANNidx[k];
	ANNdistArray dists1 = new ANNdist[k];	
	ANNidxArray idx1 = new ANNidx[k];
	ANNpoint pTar = annAllocPt(3);
	CVector3d *nS, *nT;
	float thresDis;
  //find target -----> source correspondence
	for (int i = 0; i < pTargetMesh->NbVertex(); ++i) {
		//if (pTargetMesh->m_ArrayVertex[i]->hasCorrespondence) continue;
		pTar[0] = pTargetMesh->m_ArrayVertex[i]->x(); 
		pTar[1] = pTargetMesh->m_ArrayVertex[i]->y(); 
		pTar[2] = pTargetMesh->m_ArrayVertex[i]->z();
		// check normals
		nS = pTargetMesh->m_ArrayVertex[i]->GetNormal(); 
		Vec3f a(nS->x(), nS->y(), nS->z()); Normalize(a); 
		pVert_Tree3->annkSearch(pTar, k, idx1, dists1, 0);
		bool isTarExist = false;
		for (int ii=0; ii<k; ++ii) {
			if (idx1[ii]<0) break;
      nT = m_ArrayVertex[idx1[ii]]->GetNormal();    
			Vec3f b(nT->x(), nT->y(), nT->z());
			Normalize(b);
			if (Dot(a,b)>0) {
				thresDis = dists1[ii]*2;
				isTarExist = true;
				break;
			}
		}
		if (!isTarExist) continue;
		pVert_Tree3->annkFRSearch(pTar, thresDis, k, idx, dists, 0);
		//pVert_Tree3->annkSearch(pTar, k, idx, dists, 0);
		for (int j=0; j<k; ++j) {
			if (idx[j]<0) break;
			nT = m_ArrayVertex[idx[j]]->GetNormal();
			Vec3f b(nT->x(), nT->y(), nT->z()); Normalize(b);		
			if (Dot(a,b)>0) {
			  pTargetMesh->m_ArrayVertex[i]->hasInvCorrespondence = true;
			  pTargetMesh->m_ArrayVertex[i]->match.distance[pTargetMesh->m_ArrayVertex[i]->match.num] = dists[j];
		  	pTargetMesh->m_ArrayVertex[i]->match.indices[pTargetMesh->m_ArrayVertex[i]->match.num] = idx[j];
			  pTargetMesh->m_ArrayVertex[i]->match.coef[pTargetMesh->m_ArrayVertex[i]->match.num] = 1.0/k;
			  pTargetMesh->m_ArrayVertex[i]->match.num++;
      }
    }
	}
	//release memory...
	nS=0; nT=0;
	delete nS; delete nT; delete dists; delete idx; delete dists1; delete idx1; 
	annDeallocPt(pTar);
}

/************************************************************************/
/* This function compute the volume energy of unknown current mesh, and 
fills in the data structure in CVertex3D                                */
/************************************************************************/
void CMesh3d::ComputeVolEnergy(CMesh3d* pTargetMesh, float& weight, int k) {
  CVertex3d *vT, *u, *v, *w, *p;
	//clear all correspondence flags...
	pTargetMesh->clearAllCorFlag();
	clearAllCorFlag();
	BuildSampleCorrespondences(pTargetMesh, k);
	//BuildInverseSampleCorrespondences(pTargetMesh, k);
	double disWeight;
	//treat source --> target correspondences
	int size = NbVertex();
	for (int i = 0; i < size; ++i) {
		disWeight = weight;
		if (!m_ArrayVertex[i]->hasCorrespondence) continue;
		for (int ii=0; ii<k; ++ii) {
			if (m_ArrayVertex[i]->match.indices[ii]<0) continue;
			vT = pTargetMesh->m_ArrayVertex[m_ArrayVertex[i]->match.indices[ii]];
			CVertex3d currentNearV = *vT;
			// if there exist more than one correspondences, then we don't need to find closest points for 
			// every vertex, just use vertex-to-vertex energy.
			if (k<2) {
				//write coefficients into vertices.
				m_ArrayVertex[i]->x2C += disWeight;
				m_ArrayVertex[i]->y2C += disWeight;
				m_ArrayVertex[i]->z2C += disWeight;
				m_ArrayVertex[i]->x1C += currentNearV.x()*disWeight;
				m_ArrayVertex[i]->y1C += currentNearV.y()*disWeight;
				m_ArrayVertex[i]->z1C += currentNearV.z()*disWeight;
        continue;
			}
		}
	}
	//release memory...
	vT = 0; u = 0; v = 0; w = 0; p = 0;
	delete vT; delete u; delete v; delete w; delete p;
}

/************************************************************************/
/* Single correspondence
   Current ---> target                                                  */
/************************************************************************/
void CMesh3d::BuildSingleCorrespondence(CMesh3d* pTargetMesh) {
	int size = NbVertex();
	ANNdistArray dists = new ANNdist[1];	
	ANNidxArray idx = new ANNidx[1];
	ANNpoint pSrc = annAllocPt(3);
	CVector3d *nS, *nT;
	//find source -----> target correspondence
	for (int i=0; i<size; ++i) {
		//Find out the nearest vertex in the target mesh for each point in the source mesh
		pSrc[0] = m_ArrayVertex[i]->x(); 
		pSrc[1] = m_ArrayVertex[i]->y(); 
		pSrc[2] = m_ArrayVertex[i]->z();
		nS = m_ArrayVertex[i]->GetNormal();
		// first find out the nearest suitable target point (check the normal)
		pTargetMesh->pVert_Tree3->annkSearch(pSrc, 1, idx, dists, 0);
		nT = pTargetMesh->m_ArrayVertex[idx[0]]->GetNormal();
		Vec3f a(nS->x(), nS->y(), nS->z()), b(nT->x(), nT->y(), nT->z());
		Normalize(a); Normalize(b);
		if (Dot(a,b)<0) {
			m_ArrayVertex[i]->match.indices[0] = -1;
			m_ArrayVertex[i]->hasCorrespondence = false;
			continue;
		}
		m_ArrayVertex[i]->hasCorrespondence = true;
		m_ArrayVertex[i]->match.distance[0] = dists[0];
		m_ArrayVertex[i]->match.indices[0] = idx[0];
		m_ArrayVertex[i]->match.coef[0] = 1.0;
		m_ArrayVertex[i]->match.num++;
	}
	//release memory...
	nS=0; nT=0;
	delete nS; delete nT; delete dists; delete idx;
	annDeallocPt(pSrc);
}

void CMesh3d::BuildSingleInverseCorrespondence(CMesh3d *pTargetMesh) {
  ANNdistArray dists= new ANNdist[1];	
	ANNidxArray idx= new ANNidx[1];
	ANNdistArray dists1 = new ANNdist[1];	
	ANNidxArray idx1 = new ANNidx[1];
	ANNpoint pTar = annAllocPt(3);
	CVector3d *nS, *nT;
  //find target -----> source correspondence
	for (int i=0; i<pTargetMesh->NbVertex(); ++i) {
		pTar[0] = pTargetMesh->m_ArrayVertex[i]->x(); 
		pTar[1] = pTargetMesh->m_ArrayVertex[i]->y(); 
		pTar[2] = pTargetMesh->m_ArrayVertex[i]->z();
		nT = pTargetMesh->m_ArrayVertex[i]->GetNormal();
		pVert_Tree3->annkSearch(pTar, 1, idx, dists, 0);
		// check normal
// 		nS = m_ArrayVertex[idx[0]]->GetNormal();
// 		Vec3f a(nS->x(), nS->y(), nS->z()), b(nT->x(), nT->y(), nT->z());
// 		Normalize(a); Normalize(b);
// 		if (Dot(a,b)<0) {
// 			pTargetMesh->m_ArrayVertex[i]->match.indices[0] = -1;
// 			pTargetMesh->m_ArrayVertex[i]->hasCorrespondence = false;
// 			continue;
// 		}
		pTargetMesh->m_ArrayVertex[i]->hasInvCorrespondence = true;
		pTargetMesh->m_ArrayVertex[i]->match.distance[0] = dists[0];
		pTargetMesh->m_ArrayVertex[i]->match.indices[0] = idx[0];
		pTargetMesh->m_ArrayVertex[i]->match.coef[0] = 1.0;
		pTargetMesh->m_ArrayVertex[i]->match.num++;
	}  
  //release memory...
	nS=0; nT=0;
	delete nS; delete nT; delete dists; delete idx; delete dists1; delete idx1; 
	annDeallocPt(pTar);
}

/************************************************************************/
/* Multiple two-way version Pauly SGP2005 distance energy               */
/************************************************************************/
void CMesh3d::ComputePaulyMultDisEnergy(CMesh3d* pTargetMesh, float& weight, int k) {
	CVertex3d *vT, *u, *v, *w, *p, *vSN;
	//clear all correspondence flags...
	pTargetMesh->clearAllCorFlag();
	clearAllCorFlag();
	BuildCorrespondences(pTargetMesh, k); 
	BuildInverseCorrespondences(pTargetMesh, k);
	double disWeight, h2 = m_ArrayFace[10]->Area();
	//treat source --> target correspondences
	int size = NbVertex();
	for (int i = 0; i < size; ++i) {
		disWeight = weight;
	  if (!m_ArrayVertex[i]->hasCorrespondence) continue;
		for (int ii=0; ii<k; ++ii) {
			if (m_ArrayVertex[i]->match.indices[ii]<0) continue;
			vT = pTargetMesh->m_ArrayVertex[m_ArrayVertex[i]->match.indices[ii]];
			CVertex3d currentNearV = *vT;
			if (!vT->hasInvCorrespondence) continue;
			if (vT->match.indices[0]<0) continue;
			vSN = m_ArrayVertex[vT->match.indices[0]];
		  disWeight = weight/**exp(-DistanceSquare(m_ArrayVertex[i],vSN)/h2)*//**m_ArrayVertex[i]->GetSumAreaAround()*/;
			// if there exist more than one correspondences, then we don't need to find closest points for 
			// every vertex, just use vertex-to-vertex energy.
			if (k>1) {
				//write coefficients into vertices.
				m_ArrayVertex[i]->x2C += disWeight;
				m_ArrayVertex[i]->y2C += disWeight;
				m_ArrayVertex[i]->z2C += disWeight;
				m_ArrayVertex[i]->x1C += currentNearV.x()*disWeight;
				m_ArrayVertex[i]->y1C += currentNearV.y()*disWeight;
			  m_ArrayVertex[i]->z1C += currentNearV.z()*disWeight;
        continue;
			}
			// otherwise, we found a nearest point on the surface
			CArray3d<CFace3d>* pFaceNabors = vT->GetArrayFaceNeighbor();
			int nFNabor = vT->NbFaceNeighbor();
			double disNear = vT->dist(m_ArrayVertex[i]); 
			for (int j = 0; j < nFNabor; ++j) {
				//Find a nearest point to the source pt inside the triangle
			  CFace3d fNabor = pFaceNabors->GetAt(j);
			  CVertex3d vv = m_ArrayVertex[i]->NearestPt(fNabor,vT);
			  if (vv.dist(m_ArrayVertex[i])<disNear) {
			  	currentNearV = vv;
			  	disNear = vv.dist(m_ArrayVertex[i]);
				}
			}
			pFaceNabors = 0; delete pFaceNabors;
			//write coefficients into vertices.
			m_ArrayVertex[i]->x2C += disWeight;
			m_ArrayVertex[i]->y2C += disWeight;
			m_ArrayVertex[i]->z2C += disWeight;
			m_ArrayVertex[i]->x1C += currentNearV.x()*disWeight;
			m_ArrayVertex[i]->y1C += currentNearV.y()*disWeight;
			m_ArrayVertex[i]->z1C += currentNearV.z()*disWeight;
		}
	}
	// treat target --> source correspondences
	for (i = 0; i < pTargetMesh->NbVertex(); ++i) {
		disWeight = weight;
	  if (!pTargetMesh->m_ArrayVertex[i]->hasInvCorrespondence) continue;
		for (int ii=0; ii<k; ++ii) {
			if (pTargetMesh->m_ArrayVertex[i]->match.indices[ii]<0) continue;
		  vT = m_ArrayVertex[pTargetMesh->m_ArrayVertex[i]->match.indices[ii]];
			if (vT->match.indices[0]<0) continue;
			vSN = pTargetMesh->m_ArrayVertex[vT->match.indices[0]];
		  disWeight = weight/**exp(-DistanceSquare(pTargetMesh->m_ArrayVertex[i],vSN)/h2)**/
			  /*pTargetMesh->m_ArrayVertex[i]->GetSumAreaAround()*/;
			float ar = pTargetMesh->m_ArrayVertex[i]->GetSumAreaAround();
			float dis = DistanceSquare(pTargetMesh->m_ArrayVertex[i],vSN);
			float expDis = exp(-DistanceSquare(pTargetMesh->m_ArrayVertex[i],vSN)/h2);
			CArray3d<CFace3d>* pFaceNabors = vT->GetArrayFaceNeighbor();
			int nFNabor = vT->NbFaceNeighbor();
			// if no nabors, this is an interior point
			if (nFNabor==0) continue;
			CVertex3d currentNearV = *vT;
			if (k==1) {
				int k = pTargetMesh->m_ArrayVertex[i]->match.indices[ii];
        m_ArrayVertex[k]->x2C += disWeight;
				m_ArrayVertex[k]->y2C += disWeight;
				m_ArrayVertex[k]->z2C += disWeight;
				m_ArrayVertex[k]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[k]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[k]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			// if there exist more than one correspondences, then we don't need to find closest points for 
			// every vertex, just use vertex-to-vertex energy.
			// recording nearest distance and triangle index
			double disNear = vT->dist(pTargetMesh->m_ArrayVertex[i]); 
			int nearestTriInd = 0;
			for (int j = 0; j < nFNabor; ++j) {
				//Find a nearest point to the source pt inside the triangle
				CFace3d fNabor = pFaceNabors->GetAt(j);
				CVertex3d vv = pTargetMesh->m_ArrayVertex[i]->NearestPt(fNabor,vT);
				if (vv.dist(pTargetMesh->m_ArrayVertex[i])<disNear) {
					currentNearV = vv;
					disNear = vv.dist(pTargetMesh->m_ArrayVertex[i]);
					nearestTriInd = j;
				}
			}
			// three vertices of the triangle
			u = pFaceNabors->GetAt(nearestTriInd)->v1();
			v = pFaceNabors->GetAt(nearestTriInd)->v2();
			w = pFaceNabors->GetAt(nearestTriInd)->v3();
			// their index in the mesh
			int uInd = u->index;
			int vInd = v->index;
			int wInd = w->index;
			CString su, sv, sw;
			su.Format("%d",uInd); sv.Format("%d",vInd); sw.Format("%d",wInd);
			if (uInd>NbVertex() || vInd>NbVertex() || wInd>NbVertex()) AfxMessageBox(su+"  "+sv+"  "+sw);
			p = &currentNearV;
			if (fabs(p->x()-u->x())<1e-3 && fabs(p->y()-u->y())<1e-3 && fabs(p->z()-u->z())<1e-3) {
				m_ArrayVertex[uInd]->x2C += disWeight;
				m_ArrayVertex[uInd]->y2C += disWeight;
				m_ArrayVertex[uInd]->z2C += disWeight;
				m_ArrayVertex[uInd]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[uInd]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[uInd]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			if (fabs(p->x()-v->x())<1e-3 && fabs(p->y()-v->y())<1e-3 && fabs(p->z()-v->z())<1e-3) {
				m_ArrayVertex[vInd]->x2C += disWeight;
				m_ArrayVertex[vInd]->y2C += disWeight;
				m_ArrayVertex[vInd]->z2C += disWeight;
				m_ArrayVertex[vInd]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[vInd]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[vInd]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			if (fabs(p->x()-w->x())<1e-3 && fabs(p->y()-w->y())<1e-3 && fabs(p->z()-w->z())<1e-3) {
				m_ArrayVertex[wInd]->x2C += disWeight;
				m_ArrayVertex[wInd]->y2C += disWeight;
				m_ArrayVertex[wInd]->z2C += disWeight;
				m_ArrayVertex[wInd]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[wInd]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[wInd]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			// coefficients
			double ux = u->x(), uy = u->y(), uz = u->x();
			double vx = v->x(), vy = v->y(), vz = v->x();
			double wx = w->x(), wy = w->y(), wz = w->x();
			double px = p->x(), py = p->y(), pz = p->x();
			double c1 = (ux-wx)*(ux-wx)+(uy-wy)*(uy-wy)+(uz-wz)*(uz-wz);
			double c2 = (vx-wx)*(ux-wx)+(vy-wy)*(uy-wy)+(vz-wz)*(uz-wz);
			double c3 = (wx-px)*(wx-ux)+(wy-uy)*(wy-py)+(wz-pz)*(wz-pz);
			double c4 = (vx-wx)*(vx-wx)+(vy-wy)*(vy-wy)+(vz-wz)*(vz-wz);
			double c5 = (wx-px)*(wx-vx)+(wy-uy)*(wy-py)+(wz-uz)*(wz-pz);
			double belta = (c2*c2-c1*c5)/(c2*c2-c1*c4);
			double alpha = (c3-c2*belta)/c1;
			double gama = 1-alpha-belta;
			// write into big matrix
			m_ArrayVertex[uInd]->x2C += alpha*alpha*disWeight;
			m_ArrayVertex[uInd]->y2C += alpha*alpha*disWeight;
			m_ArrayVertex[uInd]->z2C += alpha*alpha*disWeight;
			m_ArrayVertex[vInd]->x2C += belta*belta*disWeight;
			m_ArrayVertex[vInd]->y2C += belta*belta*disWeight;
			m_ArrayVertex[vInd]->z2C += belta*belta*disWeight;
			m_ArrayVertex[wInd]->x2C += gama*gama*disWeight;
			m_ArrayVertex[wInd]->y2C += gama*gama*disWeight;
			m_ArrayVertex[wInd]->z2C += gama*gama*disWeight;

			m_ArrayVertex[uInd]->x1C += alpha*pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
			m_ArrayVertex[uInd]->y1C += alpha*pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
			m_ArrayVertex[uInd]->z1C += alpha*pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
			m_ArrayVertex[vInd]->x1C += belta*pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
			m_ArrayVertex[vInd]->y1C += belta*pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
			m_ArrayVertex[vInd]->z1C += belta*pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
			m_ArrayVertex[wInd]->x1C += gama*pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
			m_ArrayVertex[wInd]->y1C += gama*pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
			m_ArrayVertex[wInd]->z1C += gama*pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
			// processing edges
			m_ArrayVertex[uInd]->invCorNabor.push_back(vInd);
			m_ArrayVertex[uInd]->invCorValue.push_back(alpha*belta);
			m_ArrayVertex[uInd]->invCorNabor.push_back(wInd);
			m_ArrayVertex[uInd]->invCorValue.push_back(alpha*gama);
			m_ArrayVertex[vInd]->invCorNabor.push_back(uInd);
			m_ArrayVertex[vInd]->invCorValue.push_back(alpha*belta);
			m_ArrayVertex[vInd]->invCorNabor.push_back(wInd);
			m_ArrayVertex[vInd]->invCorValue.push_back(belta*gama);
			m_ArrayVertex[wInd]->invCorNabor.push_back(uInd);
			m_ArrayVertex[wInd]->invCorValue.push_back(gama*alpha);
			m_ArrayVertex[wInd]->invCorNabor.push_back(vInd);
			m_ArrayVertex[wInd]->invCorValue.push_back(gama*belta);
			pFaceNabors = 0; delete pFaceNabors;
		}
	}
	//release memory...
	vT = 0; u = 0; v = 0; w = 0; p = 0; vSN = 0;
	delete vT; delete u; delete v; delete w; delete p; delete vSN;
}

/************************************************************************/
/* Pauly SGP2005 distance energy                                        */
/************************************************************************/
void CMesh3d::ComputePaulyDisEnergy(CMesh3d* pTargetMesh, float& weight) {
	CVertex3d *vT, *vSN;
	//clear all correspondence flags...
	pTargetMesh->clearAllCorFlag();
	clearAllCorFlag();
	BuildSingleCorrespondence(pTargetMesh);
	BuildSingleInverseCorrespondence(pTargetMesh);
	double disWeight;
	double h2 = m_ArrayFace[10]->Area();
	//treat source --> target correspondences
	int size = NbVertex();
	for (int i=0; i<size; ++i) {
		if (!m_ArrayVertex[i]->hasCorrespondence) continue;
		vT = pTargetMesh->m_ArrayVertex[m_ArrayVertex[i]->match.indices[0]];
		if (!vT->hasInvCorrespondence) continue;
		// now find the single inverse correspondence of vT (vSN on the source mesh).
    vSN = m_ArrayVertex[vT->match.indices[0]];
		disWeight = weight*exp(-Distance(m_ArrayVertex[i],vSN)/h2)*m_ArrayVertex[i]->GetSumAreaAround();
		//write coefficients into vertices.
		m_ArrayVertex[i]->x2C += disWeight;
		m_ArrayVertex[i]->y2C += disWeight;
		m_ArrayVertex[i]->z2C += disWeight;
		m_ArrayVertex[i]->x1C += vT->x()*disWeight;
		m_ArrayVertex[i]->y1C += vT->y()*disWeight;
		m_ArrayVertex[i]->z1C += vT->z()*disWeight;
	}
	vT=0; vSN=0;
	delete vT; delete vSN;
}

/************************************************************************/
/* Simplest single-correspondenc-based distance energy                  */
/************************************************************************/
void CMesh3d::ComputeSingleDisEnergy(CMesh3d* pTargetMesh, float& weight) {
	CVertex3d *vT;
	//clear all correspondence flags...
	pTargetMesh->clearAllCorFlag();
	clearAllCorFlag();
	BuildSingleCorrespondence(pTargetMesh);
	double disWeight;
	//treat source --> target correspondences
	int size = NbVertex();
	for (int i = 0; i < size; ++i) {
		disWeight = weight;
	  if (!m_ArrayVertex[i]->hasCorrespondence) continue;
		vT = pTargetMesh->m_ArrayVertex[m_ArrayVertex[i]->match.indices[0]];
		//write coefficients into vertices.
		m_ArrayVertex[i]->x2C += disWeight;
		m_ArrayVertex[i]->y2C += disWeight;
		m_ArrayVertex[i]->z2C += disWeight;
		m_ArrayVertex[i]->x1C += vT->x()*disWeight;
		m_ArrayVertex[i]->y1C += vT->y()*disWeight;
		m_ArrayVertex[i]->z1C += vT->z()*disWeight;
	}
}

/************************************************************************/
/* This function compute the distance energy of unknown current mesh, and 
fills in the data structure in CVertex3D                                */
/************************************************************************/
void CMesh3d::ComputeDisEnergy(CMesh3d* pTargetMesh, float& weight, int k) {
	CVertex3d *u, *v, *w, *p;
	//clear all correspondence flags...
	clearAllCorFlag();
	BuildCorrespondences(pTargetMesh,k); 
  pTargetMesh->clearAllCorFlag();
	BuildInverseCorrespondences(pTargetMesh,k);
	//merge forward and inverse correspondences
  MergeCorrespondences(pTargetMesh);
	//treat source --> target correspondences
  FillForwardDisEnergy(pTargetMesh,weight,k);
	//treat target --> source correspondences
  FillInverseDisEnergy(pTargetMesh,weight,k);

	/*for (i=0; i<pTargetMesh->NbVertex(); ++i) {
    double area = pTargetMesh->m_ArrayVertex[i]->GetSumAreaAround();
		disWeight = weight*area;
	  if (!pTargetMesh->m_ArrayVertex[i]->hasInvCorrespondence) continue;
		for (int ii=0; ii<pTargetMesh->m_ArrayVertex[i]->match.num; ++ii) {
      // the vertex on the source mesh
		  vT = m_ArrayVertex[pTargetMesh->m_ArrayVertex[i]->match.indices[ii]];
			CArray3d<CFace3d>* pFaceNabors = vT->GetArrayFaceNeighbor();
			int nFNabor = vT->NbFaceNeighbor();
			// if no nabors, this is an interior point
			if (nFNabor==0) continue;
			// recording nearest distance and triangle index
			double disNear = vT->dist(pTargetMesh->m_ArrayVertex[i]); 
			int nearestTriInd = 0;
      CVertex3d currentNearV = *vT;
			for (int j = 0; j < nFNabor; ++j) {
				//Find a nearest point to the source pt inside the triangle
				CFace3d fNabor = pFaceNabors->GetAt(j);
				CVertex3d vv = pTargetMesh->m_ArrayVertex[i]->NearestPt(fNabor,vT);
				if (vv.dist(pTargetMesh->m_ArrayVertex[i])<disNear) {
					currentNearV = vv;
					disNear = vv.dist(pTargetMesh->m_ArrayVertex[i]);
					nearestTriInd = j;
				}
			}
			// three vertices of the triangle
			u = pFaceNabors->GetAt(nearestTriInd)->v1();
			v = pFaceNabors->GetAt(nearestTriInd)->v2();
			w = pFaceNabors->GetAt(nearestTriInd)->v3();
			// their index in the mesh
			int uInd = u->index;
			int vInd = v->index;
			int wInd = w->index;
			CString su, sv, sw;
			su.Format("%d",uInd); sv.Format("%d",vInd); sw.Format("%d",wInd);
			if (uInd>NbVertex() || vInd>NbVertex() || wInd>NbVertex()) AfxMessageBox(su+"  "+sv+"  "+sw);
			p = &currentNearV;
			if (fabs(p->x()-u->x())<1e-3 && fabs(p->y()-u->y())<1e-3 && fabs(p->z()-u->z())<1e-3) {
				m_ArrayVertex[uInd]->x2C += disWeight;
				m_ArrayVertex[uInd]->y2C += disWeight;
				m_ArrayVertex[uInd]->z2C += disWeight;
				m_ArrayVertex[uInd]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[uInd]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[uInd]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			if (fabs(p->x()-v->x())<1e-3 && fabs(p->y()-v->y())<1e-3 && fabs(p->z()-v->z())<1e-3) {
				m_ArrayVertex[vInd]->x2C += disWeight;
				m_ArrayVertex[vInd]->y2C += disWeight;
				m_ArrayVertex[vInd]->z2C += disWeight;
				m_ArrayVertex[vInd]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[vInd]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[vInd]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			if (fabs(p->x()-w->x())<1e-3 && fabs(p->y()-w->y())<1e-3 && fabs(p->z()-w->z())<1e-3) {
				m_ArrayVertex[wInd]->x2C += disWeight;
				m_ArrayVertex[wInd]->y2C += disWeight;
				m_ArrayVertex[wInd]->z2C += disWeight;
				m_ArrayVertex[wInd]->x1C += pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
				m_ArrayVertex[wInd]->y1C += pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
				m_ArrayVertex[wInd]->z1C += pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
				continue;
			}
			// coefficients
			double ux = u->x(), uy = u->y(), uz = u->x();
			double vx = v->x(), vy = v->y(), vz = v->x();
			double wx = w->x(), wy = w->y(), wz = w->x();
			double px = p->x(), py = p->y(), pz = p->x();
			double c1 = (ux-wx)*(ux-wx)+(uy-wy)*(uy-wy)+(uz-wz)*(uz-wz);
			double c2 = (vx-wx)*(ux-wx)+(vy-wy)*(uy-wy)+(vz-wz)*(uz-wz);
			double c3 = (wx-px)*(wx-ux)+(wy-uy)*(wy-py)+(wz-pz)*(wz-pz);
			double c4 = (vx-wx)*(vx-wx)+(vy-wy)*(vy-wy)+(vz-wz)*(vz-wz);
			double c5 = (wx-px)*(wx-vx)+(wy-uy)*(wy-py)+(wz-uz)*(wz-pz);
			double belta = (c2*c2-c1*c5)/(c2*c2-c1*c4);
			double alpha = (c3-c2*belta)/c1;
			double gama = 1-alpha-belta;
			// write into big matrix
			m_ArrayVertex[uInd]->x2C += alpha*alpha*disWeight;
			m_ArrayVertex[uInd]->y2C += alpha*alpha*disWeight;
			m_ArrayVertex[uInd]->z2C += alpha*alpha*disWeight;
			m_ArrayVertex[vInd]->x2C += belta*belta*disWeight;
			m_ArrayVertex[vInd]->y2C += belta*belta*disWeight;
			m_ArrayVertex[vInd]->z2C += belta*belta*disWeight;
			m_ArrayVertex[wInd]->x2C += gama*gama*disWeight;
			m_ArrayVertex[wInd]->y2C += gama*gama*disWeight;
			m_ArrayVertex[wInd]->z2C += gama*gama*disWeight;

			m_ArrayVertex[uInd]->x1C += alpha*pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
			m_ArrayVertex[uInd]->y1C += alpha*pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
			m_ArrayVertex[uInd]->z1C += alpha*pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
			m_ArrayVertex[vInd]->x1C += belta*pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
			m_ArrayVertex[vInd]->y1C += belta*pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
			m_ArrayVertex[vInd]->z1C += belta*pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
			m_ArrayVertex[wInd]->x1C += gama*pTargetMesh->m_ArrayVertex[i]->x()*disWeight;
			m_ArrayVertex[wInd]->y1C += gama*pTargetMesh->m_ArrayVertex[i]->y()*disWeight;
			m_ArrayVertex[wInd]->z1C += gama*pTargetMesh->m_ArrayVertex[i]->z()*disWeight;
			// processing edges
			m_ArrayVertex[uInd]->invCorNabor.push_back(vInd);
			m_ArrayVertex[uInd]->invCorValue.push_back(alpha*belta);
			m_ArrayVertex[uInd]->invCorNabor.push_back(wInd);
			m_ArrayVertex[uInd]->invCorValue.push_back(alpha*gama);
			m_ArrayVertex[vInd]->invCorNabor.push_back(uInd);
			m_ArrayVertex[vInd]->invCorValue.push_back(alpha*belta);
			m_ArrayVertex[vInd]->invCorNabor.push_back(wInd);
			m_ArrayVertex[vInd]->invCorValue.push_back(belta*gama);
			m_ArrayVertex[wInd]->invCorNabor.push_back(uInd);
			m_ArrayVertex[wInd]->invCorValue.push_back(gama*alpha);
			m_ArrayVertex[wInd]->invCorNabor.push_back(vInd);
			m_ArrayVertex[wInd]->invCorValue.push_back(gama*belta);
			pFaceNabors = 0; delete pFaceNabors;
		}
	}*/
	//release memory...
	u = 0; v = 0; w = 0; p = 0;
	delete u; delete v; delete w; delete p;
}

/************************************************************************/
/* Merge forward and inverse correspondences                            
   Check every inverse correspondence, eliminate it if this 
   correspondence has been found in the forward correspondence.         */
/************************************************************************/
void CMesh3d::MergeCorrespondences(CMesh3d* pTargetMesh) {
  CVertex3d *vT, *vS;
  int ind;
  for (int i=0; i<pTargetMesh->NbVertex(); ++i) {
    vT = pTargetMesh->m_ArrayVertex[i];
    // scan every inverse correspondence
    for (int ii=0; ii<vT->match.num; ++ii) {
      ind = vT->match.indices[ii];
      vS = m_ArrayVertex[ind];
      // scan every forward correspondence of the current source vertex
      for (int jj=0; jj<vS->match.num; ++jj) 
        // check if there exist a same correspondence already
        if (i==vS->match.indices[jj]) {
          vT->match.indices[ii] = -1;
          //AfxMessageBox("find redundance");
          break;
        }
    }
  }
  vT=0; vS=0; 
  delete vT; delete vS;
}

/************************************************************************/
/* Fill in matrix for forward correspondences                           */
/************************************************************************/
void CMesh3d::FillForwardDisEnergy(CMesh3d* pTargetMesh, float& weight, int k) {
  CVertex3d *vT;
  double disWeight,area;
  int size = NbVertex();
  for (int i=0; i<size; ++i) {
    area = m_ArrayVertex[i]->GetSumAreaAround();
    disWeight = weight*area;
    if (!m_ArrayVertex[i]->hasCorrespondence) continue;
    //float k1=1.0f;///m_ArrayVertex[i]->match.num;
    for (int ii=0; ii<m_ArrayVertex[i]->match.num; ++ii) {
      vT = pTargetMesh->m_ArrayVertex[m_ArrayVertex[i]->match.indices[ii]];
      CVertex3d currentNearV = *vT;
      CArray3d<CFace3d>* pFaceNabors = vT->GetArrayFaceNeighbor();
      int nFNabor = vT->NbFaceNeighbor();
      double disNear = vT->dist(m_ArrayVertex[i]); 
      for (int j=0; j<nFNabor; ++j) {
        //Find a nearest point to the source pt inside the triangle
        CFace3d fNabor = pFaceNabors->GetAt(j);
        CVertex3d vv = m_ArrayVertex[i]->NearestPt(fNabor,vT);
        if (vv.dist(m_ArrayVertex[i])<disNear) {
          currentNearV = vv;
          disNear = vv.dist(m_ArrayVertex[i]);
        }
      }
      pFaceNabors = 0; delete pFaceNabors;
      //write coefficients into vertices.
      m_ArrayVertex[i]->x2C += disWeight;
      m_ArrayVertex[i]->y2C += disWeight;
      m_ArrayVertex[i]->z2C += disWeight;
      m_ArrayVertex[i]->x1C += currentNearV.x()*disWeight;
      m_ArrayVertex[i]->y1C += currentNearV.y()*disWeight;
      m_ArrayVertex[i]->z1C += currentNearV.z()*disWeight;
    }
	}
  vT = 0; delete vT;
}

/************************************************************************/
/* Fill in matrix for inverse correspondences                           */
/************************************************************************/
void CMesh3d::FillInverseDisEnergy(CMesh3d* pTargetMesh, float& weight, int k) {
  double area, disWeight;
  CVertex3d *vS, *vT;
  for (int i=0; i<pTargetMesh->NbVertex(); ++i) {
    // the vertex on the target mesh
    vT = pTargetMesh->m_ArrayVertex[i];
    if (!vT->hasInvCorrespondence) continue;
    for (int ii=0; ii<vT->match.num; ++ii) {
      if (vT->match.indices[ii]<0) continue;
      // the vertex on the source mesh
      vS = m_ArrayVertex[vT->match.indices[ii]];
      area = vT->GetSumAreaAround();
      disWeight = weight*area;
      // we should still traverse the triangles around vT (target vertex)
      CArray3d<CFace3d>* pFaceNabors = vT->GetArrayFaceNeighbor();
			int nFNabor = vT->NbFaceNeighbor();
      // if no nabors, this is an interior point
      if (nFNabor==0) continue;
      // recording nearest distance
      CVertex3d currentNearV = *vT;
      double disNear = vT->dist(vS); 
      for (int j=0; j<nFNabor; ++j) {
        // find a nearest point to the source pt inside the triangle
        CFace3d fNabor = pFaceNabors->GetAt(j);
        CVertex3d vv = vS->NearestPt(fNabor,vT);
        if (vv.dist(vS)<disNear) {
          currentNearV = vv;
          disNear = vv.dist(vS);
        }
      }
      pFaceNabors = 0; delete pFaceNabors;
      //write coefficients into vertices.
      vS->x2C += disWeight;
      vS->y2C += disWeight;
      vS->z2C += disWeight;
      vS->x1C += currentNearV.x()*disWeight;
      vS->y1C += currentNearV.y()*disWeight;
			vS->z1C += currentNearV.z()*disWeight;
    }
  }
  vS = 0; vT = 0; delete vS; delete vT;
}

/************************************************************************/
/*  Edge based                                                          */
/*  Huang et al. 2006, in Visual Computer                               */    
/*  Pauly et al. 2005 in SGP 2005                                       */
/************************************************************************/
void CMesh3d::ComputeEdgeBasedGeomEnergy(CMesh3d* pSrcObject, float& elasWeight) {
	//for two meshes: the template and the current mesh
	//traverse edges and fill in the associate data-structure
	//Assume the edge set for the current mesh and template (source) mesh are already built
	
	double refArea = m_ArrayVertex[0]->GetSumAreaAround();
	int vSize = NbVertex();
	float elasW;
	int edgeCount=0;
	for (int i = 0; i < vSize; ++i)	{
		m_ArrayVertex[i]->m_NaborWeight.clear();
		if (pSrcObject->m_ArrayVertex[i]->GetArrayFaceNeighbor()->GetSize()<1) continue;
		// calculate difCoords for edge(i,ii) in the former mesh
		double xDif, yDif, zDif;
		vector<int> nabor = m_ArrayVertex[i]->m_NaborIndex;
		for (int j=0; j<nabor.size(); ++j) {
			int ii = nabor[j];
			int prev = j - 1; 
			if (j==0) prev = nabor.size()-1;
			int next = j + 1;
			if (j==nabor.size()-1) next = 0;
			int prevNabor = nabor[prev];
			int nextNabor = nabor[next];
			// In Pauly2005, each edge differential coordinate is assigned a weight equal to the corresponding
			// potion of the voronoi cell of the vertex
			float ajk;
		  // First, compute voronoi vertices.
			// They are circumcenters of triangles (i,prevNabor,ii) and (i,ii,nextNabor)
      float* pVorPrev = new float[2];
			float* pVorNext = new float[2];
			float x1 = pSrcObject->m_ArrayVertex[i]->x(), y1 = pSrcObject->m_ArrayVertex[i]->y();
			float x2 = pSrcObject->m_ArrayVertex[ii]->x(), y2 = pSrcObject->m_ArrayVertex[ii]->y();
			float x3 = pSrcObject->m_ArrayVertex[prevNabor]->x(), y3 = pSrcObject->m_ArrayVertex[prevNabor]->y();
			float x4 = pSrcObject->m_ArrayVertex[nextNabor]->x(), y4 = pSrcObject->m_ArrayVertex[nextNabor]->y();
      centerCircle(pSrcObject->m_ArrayVertex[i]->x(),pSrcObject->m_ArrayVertex[i]->y(),
				           pSrcObject->m_ArrayVertex[prevNabor]->x(),pSrcObject->m_ArrayVertex[prevNabor]->y(),
									 pSrcObject->m_ArrayVertex[ii]->x(),pSrcObject->m_ArrayVertex[ii]->y(),pVorPrev);
      centerCircle(pSrcObject->m_ArrayVertex[i]->x(),pSrcObject->m_ArrayVertex[i]->y(),
				           pSrcObject->m_ArrayVertex[nextNabor]->x(),pSrcObject->m_ArrayVertex[nextNabor]->y(),
									 pSrcObject->m_ArrayVertex[ii]->x(),pSrcObject->m_ArrayVertex[ii]->y(),pVorNext);
      if (x1==x2 && y1==y2 || x1==x3 && y1==y3 || x2==x3 && y2==y3) {
				pVorPrev[0] = 0.333f*(m_ArrayVertex[i]->x()+m_ArrayVertex[ii]->x()+m_ArrayVertex[prevNabor]->x());
				pVorPrev[1] = 0.333f*(m_ArrayVertex[i]->y()+m_ArrayVertex[ii]->y()+m_ArrayVertex[prevNabor]->y());
			}
			if (x1==x2 && y1==y2 || x1==x4 && y1==y4 || x2==x4 && y2==y4) {
				pVorNext[0] = 0.333f*(m_ArrayVertex[i]->x()+m_ArrayVertex[ii]->x()+m_ArrayVertex[nextNabor]->x());
				pVorNext[1] = 0.333f*(m_ArrayVertex[i]->y()+m_ArrayVertex[ii]->y()+m_ArrayVertex[nextNabor]->y());
			}
			// compute triangle (v(i),pVorPrev,pVorNext)'s area ajk as the weight
			float zPVorPrev = 0.333f*(m_ArrayVertex[i]->z()+m_ArrayVertex[ii]->z()+m_ArrayVertex[prevNabor]->z());
			float zPVorNext = 0.333f*(m_ArrayVertex[i]->z()+m_ArrayVertex[ii]->z()+m_ArrayVertex[nextNabor]->z());
			CVertex3d vVorPrev(pVorPrev[0],pVorPrev[1],zPVorPrev);
			CVertex3d vVorNext(pVorNext[0],pVorNext[1],zPVorNext);
			ajk = Area(m_ArrayVertex[i],&vVorPrev,&vVorNext);
			float aaaa = Area(m_ArrayVertex[i],m_ArrayVertex[next],m_ArrayVertex[prev]);
			elasW= elasWeight/**aaaa*/;
			// edge
			xDif = pSrcObject->m_ArrayVertex[i]->x()-pSrcObject->m_ArrayVertex[ii]->x();
			yDif = pSrcObject->m_ArrayVertex[i]->y()-pSrcObject->m_ArrayVertex[ii]->y();
			zDif = pSrcObject->m_ArrayVertex[i]->z()-pSrcObject->m_ArrayVertex[ii]->z();
			m_ArrayVertex[i]->x2C += elasW;
			m_ArrayVertex[i]->y2C += elasW;
			m_ArrayVertex[i]->z2C += elasW;
			m_ArrayVertex[ii]->x2C += elasW;
			m_ArrayVertex[ii]->y2C += elasW;
			m_ArrayVertex[ii]->z2C += elasW;			
			// b = cp_0 - cp_1
			//p_0
			m_ArrayVertex[i]->x1C += xDif*elasW;
			m_ArrayVertex[i]->y1C += yDif*elasW;
			m_ArrayVertex[i]->z1C += zDif*elasW;
			//p_1
			m_ArrayVertex[ii]->x1C -= xDif*elasW;
			m_ArrayVertex[ii]->y1C -= yDif*elasW;
			m_ArrayVertex[ii]->z1C -= zDif*elasW;
			m_ArrayVertex[i]->m_NaborWeight.push_back(elasW);
			// internal edge
// 			xDif = pSrcObject->m_ArrayVertex[prevNabor]->x()-pSrcObject->m_ArrayVertex[nextNabor]->x();
// 			yDif = pSrcObject->m_ArrayVertex[prevNabor]->y()-pSrcObject->m_ArrayVertex[nextNabor]->y();
// 			zDif = pSrcObject->m_ArrayVertex[prevNabor]->z()-pSrcObject->m_ArrayVertex[nextNabor]->z();
// 			m_ArrayVertex[prevNabor]->x2C += 2*elasW;
// 			m_ArrayVertex[prevNabor]->y2C += 2*elasW;
// 			m_ArrayVertex[prevNabor]->z2C += 2*elasW;
// 			m_ArrayVertex[prevNabor]->x1C -= 2*xDif*elasW;
// 			m_ArrayVertex[prevNabor]->y1C -= 2*yDif*elasW;
// 			m_ArrayVertex[prevNabor]->z1C -= 2*zDif*elasW;
			edgeCount++;
		}
	}
}

/************************************************************************/
/* Ring based energy weighted with area and cotangent weighted                                                
   ||\frac{1}{n}\sum p_j - p_i - c||^2                                            
   x^TAx - 2bx + C = 0  ====>   Ax = b                                  */
/************************************************************************/
void CMesh3d::ComputeWeightedRingEnergy(CMesh3d *pSrcObject, CMesh3d *pPreviousMesh, float &elasWeight) {
  //for two meshes: the template and the current mesh
  //traverse vertices' neighbors and fill in the associate data-structure
  //Assume the edge set for the current mesh and template (source) mesh are already built
  
  int vSize = NbVertex();
  int i, j, ii, nn;
  float elasW, cx, cy, cz, nn1;
  for (i = 0; i < vSize; ++i)	{
    if (pSrcObject->m_ArrayVertex[i]->GetArrayFaceNeighbor()->GetSize()<1) continue;		
    vector<int> nabor = m_ArrayVertex[i]->m_NaborIndex;
    // compute c (constant related to the source mesh)
    double area = m_ArrayVertex[i]->GetSumAreaAround();
    double areaSrc1 = 1.0/pSrcObject->m_ArrayVertex[i]->GetSumAreaAround();
    double areaCur1 = 1.0/pPreviousMesh->m_ArrayVertex[i]->GetSumAreaAround();
    elasW = elasWeight*area;
    cx = 0; cy = 0; cz = 0;
    nn = nabor.size();
    nn1 = 1.0;//1.0f/nn;
    // traversing nabors in the source mesh
    CVertex3d *pV, *nV, *cV; 
    CVector3d *e1, *e2;
    double alpha, beta, cotA, cotB, cotSum=0;
    for (j=0; j<nn; ++j) {
      ii = nabor[j];
      cV = pSrcObject->m_ArrayVertex[ii];
      // find 2 faces that contain this nabor vertex
      CArray3d<CFace3d> fArray;
      pSrcObject->m_ArrayVertex[i]->FindSharingFaces(cV,fArray);
      // previous and next nabor
      // traverse vertices of these 2 faces
      for (int iv=0; iv<3; ++iv) {
        if (fArray[0]->v(iv)!=cV && fArray[0]->v(iv)!=pSrcObject->m_ArrayVertex[i]) 
          pV = fArray[0]->v(iv);
        if (fArray[1]->v(iv)!=cV && fArray[1]->v(iv)!=pSrcObject->m_ArrayVertex[i]) 
          nV = fArray[1]->v(iv);
      }
      // find angle alpha
      e1 = new CVector3d(pV,pSrcObject->m_ArrayVertex[i]); 
      e2 = new CVector3d(pV,cV);
      alpha = Angle(e1,e2);
      cotA = fabs(cos(alpha)/sin(alpha));
      // find angle beta
      e1 = new CVector3d(nV,pSrcObject->m_ArrayVertex[i]); 
      e2 = new CVector3d(nV,cV);
      beta = Angle(e1,e2);
      cotB = fabs(cos(beta)/sin(beta));
      cx += pSrcObject->m_ArrayVertex[ii]->x()*nn1*(cotA+cotB);
      cy += pSrcObject->m_ArrayVertex[ii]->y()*nn1*(cotA+cotB);
      cz += pSrcObject->m_ArrayVertex[ii]->z()*nn1*(cotA+cotB);
//       cx += pSrcObject->m_ArrayVertex[ii]->x()*nn1;
//       cy += pSrcObject->m_ArrayVertex[ii]->y()*nn1;
//       cz += pSrcObject->m_ArrayVertex[ii]->z()*nn1;
      cotSum += cotA+cotB;
    }
    cx -= pSrcObject->m_ArrayVertex[i]->x()*cotSum;
    cy -= pSrcObject->m_ArrayVertex[i]->y()*cotSum;
    cz -= pSrcObject->m_ArrayVertex[i]->z()*cotSum;
    cx *= areaSrc1;
    cy *= areaSrc1;
    cz *= areaSrc1;
//     cx -= pSrcObject->m_ArrayVertex[i]->x();
//     cy -= pSrcObject->m_ArrayVertex[i]->y();
//     cz -= pSrcObject->m_ArrayVertex[i]->z();
    // b = c(p_1+p_2+...+p_nn-p_0*nn)
    //traversing nabors in the target mesh
    cotSum = 0;
    for (j = 0; j < nn; ++j) {
      ii = nabor[j];
      // previous and next nabor
      cV = pPreviousMesh->m_ArrayVertex[ii];
      // find 2 faces that contain this nabor vertex
      CArray3d<CFace3d> fArray;
      pPreviousMesh->m_ArrayVertex[i]->FindSharingFaces(cV,fArray);
      // previous and next nabor
      // traverse vertices of these 2 faces
      for (int iv=0; iv<3; ++iv) {
        if (fArray[0]->v(iv)!=cV && fArray[0]->v(iv)!=pPreviousMesh->m_ArrayVertex[i]) 
          pV = fArray[0]->v(iv);
        if (fArray[1]->v(iv)!=cV && fArray[1]->v(iv)!=pPreviousMesh->m_ArrayVertex[i]) 
          nV = fArray[1]->v(iv);
      }
      // find angle alpha
      e1 = new CVector3d(pV,pPreviousMesh->m_ArrayVertex[i]); 
      e2 = new CVector3d(pV,cV);
      alpha = Angle(e1,e2);
      cotA = fabs(cos(alpha)/sin(alpha));
      // find angle beta
      e1 = new CVector3d(nV,pPreviousMesh->m_ArrayVertex[i]); 
      e2 = new CVector3d(nV,cV);
      beta = Angle(e1,e2);
      cotB = fabs(cos(beta)/sin(beta));
      // quadratic terms
      m_ArrayVertex[ii]->x2C += elasW*nn1*nn1*(cotA+cotB)*(cotA+cotB)*areaCur1*areaCur1;
      m_ArrayVertex[ii]->y2C += elasW*nn1*nn1*(cotA+cotB)*(cotA+cotB)*areaCur1*areaCur1;
      m_ArrayVertex[ii]->z2C += elasW*nn1*nn1*(cotA+cotB)*(cotA+cotB)*areaCur1*areaCur1;
      // linear terms 
      m_ArrayVertex[ii]->x1C += elasW*cx*nn1*(cotA+cotB)*areaCur1;
      m_ArrayVertex[ii]->y1C += elasW*cy*nn1*(cotA+cotB)*areaCur1;
      m_ArrayVertex[ii]->z1C += elasW*cz*nn1*(cotA+cotB)*areaCur1;
      cotSum += (cotA+cotB);
    }
    // for the center vertex...
    // quadratic terms
    m_ArrayVertex[i]->x2C += elasW*cotSum*cotSum*areaCur1*areaCur1;
    m_ArrayVertex[i]->y2C += elasW*cotSum*cotSum*areaCur1*areaCur1;
    m_ArrayVertex[i]->z2C += elasW*cotSum*cotSum*areaCur1*areaCur1;
    // linear terms
    m_ArrayVertex[i]->x1C -= elasW*cx*cotSum*areaCur1;
    m_ArrayVertex[i]->y1C -= elasW*cy*cotSum*areaCur1;
    m_ArrayVertex[i]->z1C -= elasW*cz*cotSum*areaCur1;
    pV=0; nV=0; cV=0; e1=0; e2=0;
    delete pV; delete nV; delete cV; delete e1; delete e2;
	}
}

/************************************************************************/
/* Ring based energy                                                    
   ||\frac{1}{n}\sum p_j - p_i - c||^2                                            
   x^TAx - 2bx + C = 0  ====>   Ax = b                                  */
/************************************************************************/
void CMesh3d::ComputeRingBasedGeomEnergy(CMesh3d* pSrcObject, float& elasWeight) {
	//for two meshes: the template and the current mesh
	//traverse vertices' neighbors and fill in the associate data-structure
	//Assume the edge set for the current mesh and template (source) mesh are already built

	int vSize = NbVertex();
	int i, j, ii, nn;
	float elasW, cx, cy, cz, nn1;
	for (i = 0; i < vSize; ++i)	{
		if (pSrcObject->m_ArrayVertex[i]->GetArrayFaceNeighbor()->GetSize()<1) continue;		
		elasW = (100/m_ArrayVertex[i]->GetSumAreaAround())*elasWeight;
		vector<int> nabor = m_ArrayVertex[i]->m_NaborIndex;
		// compute c
		cx = 0; cy = 0; cz = 0;
		nn = nabor.size();
		nn1 = 1.0f/nn;
		for (j = 0; j < nn; ++j) {
			ii = nabor[j];
			cx += pSrcObject->m_ArrayVertex[ii]->x()*nn1;
			cy += pSrcObject->m_ArrayVertex[ii]->y()*nn1;
			cz += pSrcObject->m_ArrayVertex[ii]->z()*nn1;
		}
		cx -= pSrcObject->m_ArrayVertex[i]->x();
		cy -= pSrcObject->m_ArrayVertex[i]->y();
		cz -= pSrcObject->m_ArrayVertex[i]->z();
		// b = c(p_1+p_2+...+p_nn-p_0*nn)
		//traversing nabors
		for (j = 0; j < nn; ++j) {
			ii = nabor[j];
			// quadratic terms
			m_ArrayVertex[ii]->x2C += elasW*nn1*nn1;
			m_ArrayVertex[ii]->y2C += elasW*nn1*nn1;
			m_ArrayVertex[ii]->z2C += elasW*nn1*nn1;
			// linear terms 
			m_ArrayVertex[ii]->x1C += elasW*cx*nn1;
			m_ArrayVertex[ii]->y1C += elasW*cy*nn1;
			m_ArrayVertex[ii]->z1C += elasW*cz*nn1;
		}
		// for the vertex...
		// quadratic terms
		m_ArrayVertex[i]->x2C += elasW;
		m_ArrayVertex[i]->y2C += elasW;
		m_ArrayVertex[i]->z2C += elasW;
		// linear terms
		m_ArrayVertex[i]->x1C -= elasW*cx;
		m_ArrayVertex[i]->y1C -= elasW*cy;
		m_ArrayVertex[i]->z1C -= elasW*cz;
	}
}

void CMesh3d::ComputeElasEnergy(CMesh3d* pSrcObject, float& elasWeight) {
	//ComputeEdgeBasedGeomEnergy(pSrcObject, elasWeight);
	ComputeRingBasedGeomEnergy(pSrcObject, elasWeight);
}

void CMesh3d::ComputeDifCoord() {
	for (int i = 0; i < NbVertex(); ++i)
		m_ArrayVertex[i]->ComputeDifCoord();
}

void CMesh3d::initB() {
	b.clear();
	for (int i = 0; i < NbVertex()*3; ++i) b.push_back(0);
}

void CMesh3d::GenerateEdgeSet() {
	GenerateEdgeArray(&m_ArrayEdge);
	for (int i = 0; i < NbVertex(); ++i)
		m_ArrayVertex[i]->BuildEdgeNeighbor();
}

void CMesh3d::clearAllCorFlag() {
	for (int i = 0; i < NbVertex(); ++i) {
	  m_ArrayVertex[i]->hasCorrespondence = false;
	  m_ArrayVertex[i]->hasInvCorrespondence = false;
	  m_ArrayVertex[i]->invCorNabor.clear();
	  m_ArrayVertex[i]->invCorValue.clear();
	}
}

void CMesh3d::push(int i) {
  if (flags[i]) return;
  assert(sp<m_ArrayFace.GetSize());
  stack[sp++] = i;
}

int CMesh3d::pop() {
  assert(sp>0);
  return stack[--sp];
}

void CMesh3d::reorient(int i) {
  reorients++;
  m_ArrayFace[i]->reorient();
	m_Modified = 1;
}

int CMesh3d::which(int i, CFace3d* t) {
	TRACE("v1Ind=%d",t->v1()->index);
  if (i==t->v1()->index) return 0;
  if (i==t->v2()->index) return 1;
  assert(i==t->v3()->index);
  return 2;
}

int CMesh3d::isalong(int v1, int v2, int t) {
  return which(v2,m_ArrayFace[t])==(which(v1,m_ArrayFace[t])+1)%3;
}

void CMesh3d::_traverse() {
  int i,j,k;
  while(sp>0) {
    i = pop();
    flags[i] = 1;
    for (j=0; j<3; ++j) {
			// find neighbor face which opposite vertex j
			CFace3d* pF = m_ArrayFace[i]->fOpposite(j);
			if (pF==NULL) continue;
	    //if ((k=(m.adj[i][j]))==-1) continue;
			k=pF->index;
	    if (flags[k]) continue;
	    /* fix the orientation of k if necessary */
	    if (isalong(m_ArrayFace[i]->v((j+1)%3)->index,m_ArrayFace[i]->v((j+2)%3)->index,k)) reorient(k);
	    push(k);
		}
	}
}

void CMesh3d::traverse(int i) {
  assert(!sp);
  push(i);
  _traverse();
}

void CMesh3d::OrientFaces() {
	int size = m_ArrayFace.GetSize();
  stack = (int*)malloc(size*sizeof(int));;
  sp = 0;
	reorients = 0;
  flags = (unsigned char*)malloc(size*sizeof(unsigned char));
	memset(flags,0,size*sizeof(unsigned char));
	int i,cs;
  for (cs=i=0; i<size; ++i)
    if (!flags[i]) {
	    cs++;
	    traverse(i);
    }
  TRACE("Connected components: %d, reorients: %d\n",cs,reorients);
}

void CMesh3d::ReorientFaces() {
	int size = m_ArrayFace.GetSize();
	for (int i=0; i<size; ++i) m_ArrayFace[i]->reorient();
}

void CMesh3d::MultMat(CMesh3d* pMesh, float* mat) {
	float x, y, z, w;
	for (int ii=0; ii<NbVertex(); ++ii) {
		x = pMesh->m_ArrayVertex[ii]->x()*mat[0]+pMesh->m_ArrayVertex[ii]->y()*mat[4]+
			pMesh->m_ArrayVertex[ii]->z()*mat[8]+1*mat[12];
		y = pMesh->m_ArrayVertex[ii]->x()*mat[1]+pMesh->m_ArrayVertex[ii]->y()*mat[5]+
			pMesh->m_ArrayVertex[ii]->z()*mat[9]+1*mat[13];
		z = pMesh->m_ArrayVertex[ii]->x()*mat[2]+pMesh->m_ArrayVertex[ii]->y()*mat[6]+
			pMesh->m_ArrayVertex[ii]->z()*mat[10]+1*mat[14];
		w = pMesh->m_ArrayVertex[ii]->x()*mat[3]+pMesh->m_ArrayVertex[ii]->y()*mat[7]+
			pMesh->m_ArrayVertex[ii]->z()*mat[11]+1*mat[15];
		m_ArrayVertex[ii]->Set(x, y, z);
	} 
}

void CMesh3d::MultMat(CObject3d* pObject, float* mat) {
	MultMat((CMesh3d*)pObject, mat);
}

void CMesh3d::GenerateUniformSamples() {
  // uniform sampling points in each triangle of the mesh
  int fSize = NbFace();
  double eps = m_ArrayFace[10]->Area()/2;
	// n indicates how many points need be randomly selected in a triangle
	int n;
	// s and t are two parameters along two edges of a triangle
	double s, t, x, y, z, nx, ny, nz;
	int sCount=0;
	CVertex3d currentNearV;
	vector<float> sampleX, sampleY, sampleZ; 
	for (int i=0; i<fSize; ++i) {
		n = (int)(m_ArrayFace[i]->Area()/eps)+1;
		// pick the first vertex of the triangle as origin 
		CVertex3d* pOrigin = m_ArrayFace[i]->v1();
		// two edge vectors
    CVector3d e1(m_ArrayFace[i]->v1(), m_ArrayFace[i]->v2());
		CVector3d e2(m_ArrayFace[i]->v1(), m_ArrayFace[i]->v3());
		CVertex3d nv1(m_ArrayFace[i]->v1()->GetNormal()->x(), 
			m_ArrayFace[i]->v1()->GetNormal()->y(), m_ArrayFace[i]->v1()->GetNormal()->z());
		CVertex3d nv2(m_ArrayFace[i]->v2()->GetNormal()->x(), 
			m_ArrayFace[i]->v2()->GetNormal()->y(), m_ArrayFace[i]->v2()->GetNormal()->z());
		CVertex3d nv3(m_ArrayFace[i]->v3()->GetNormal()->x(), 
			m_ArrayFace[i]->v3()->GetNormal()->y(), m_ArrayFace[i]->v3()->GetNormal()->z());
		CVector3d ne1(nv1, nv2);
		CVector3d ne2(nv1, nv3);
		e1.NormalizeL2(1.0); e2.NormalizeL2(1.0);
    for (int j=0; j<n; ++j) {
			// s\in(0,1)
      s = (rand()%1000)/1000.0;
			// t\in(0,1-s)
			t = (rand()%1000)/1000.0*(1-s);
			x = pOrigin->x()+e1.x()*s+e2.x()*t;
			y = pOrigin->y()+e1.y()*s+e2.y()*t;
			z = pOrigin->z()+e1.z()*s+e2.z()*t;
			nx = pOrigin->GetNormal()->x()+ne1.x()*s+ne2.x()*t;
			ny = pOrigin->GetNormal()->y()+ne1.y()*s+ne2.y()*t;
			nz = pOrigin->GetNormal()->z()+ne1.z()*s+ne2.z()*t;
      CVector3d nv(nx, ny, nz);
			nv.NormalizeL2(1);
			CVertex3d sample = new CVertex3d(x, y, z);
			sample.SetNormal(nv.x(), nv.y(), nv.z());
			m_ArraySample.Add(&sample);
			sampleX.push_back(x); sampleY.push_back(y); sampleZ.push_back(z);
			sCount++;
		}
	  pOrigin = 0; delete pOrigin;
	}
	ANNpointArray S_Array = annAllocPts(sCount, 3);
	for (i=0; i<sCount; i++) {
		S_Array[i][0] = sampleX[i];
		S_Array[i][1] = sampleY[i];
		S_Array[i][2] = sampleZ[i];
	}
	//Build kd Tree for nearest distance searching
	pSample_Tree3 = new ANNkd_tree(S_Array, sCount, 3);
}

void CMesh3d::ComputeDis(double* rDis, CMesh3d* pMesh) {
	// uniform sampling points in each triangle of the mesh
  double dis = 0, disL2 = 0, disLM = -1;
	int fSize = NbFace();
	// The 10th triangle's area is 0.0108070863
  double eps = m_ArrayFace[10]->Area()/2;
	// n indicates how many points need be randomly selected in a triangle
	int n;
	// s and t are two parameters along two edges of a triangle
	double s, t, x, y, z;
	int lCount=0;
	CVertex3d currentNearV;
	for (int i=0; i<fSize; ++i) {
		n = (int)(m_ArrayFace[i]->Area()/eps)+1;
		// pick the first vertex of the triangle as origin 
		CVertex3d* pOrigin = m_ArrayFace[i]->v1();
		// two edge vectors
    CVector3d e1(m_ArrayFace[i]->v1(), m_ArrayFace[i]->v2());
		CVector3d e2(m_ArrayFace[i]->v1(), m_ArrayFace[i]->v3());
		e1.NormalizeL2(1.0); e2.NormalizeL2(1.0);
    CVertex3d* vT;
		CVertex3d vv;
		ANNdistArray dists;
		ANNidxArray idx;
		ANNpoint p;
		for (int j=0; j<n; ++j) {
		  // s\in(0,1)
      s = (rand()%1000)/1000.0;
		  // t\in(0,1-s)
		  t = (rand()%1000)/1000.0*(1-s);
			x = pOrigin->x()+e1.x()*s+e2.x()*t;
			y = pOrigin->y()+e1.y()*s+e2.y()*t;
			z = pOrigin->z()+e1.z()*s+e2.z()*t;
			CVertex3d vCur(x,y,z);
			dists= new ANNdist[1];	
	    idx= new ANNidx[1];
	    p = annAllocPt(3);
	    p[0] = x; p[1] = y; p[2] = z;
	    pMesh->pVert_Tree3->annkSearch(p, 1, idx, dists, 0);
		  // find nearest point on the surface
			vT = pMesh->m_ArrayVertex[idx[0]];
			currentNearV.Set(vT->x(),vT->y(),vT->z());
			if (fabs(pMesh->m_ArrayVertex[idx[0]]->x())<1e-2 && 
				  fabs(pMesh->m_ArrayVertex[idx[0]]->y())<1e-2 && 
				  fabs(pMesh->m_ArrayVertex[idx[0]]->z())<1e-2) 
				continue;
			lCount++;
	    CArray3d<CFace3d>* pFaceNabors = vT->GetArrayFaceNeighbor();
	    int nFNabor = vT->NbFaceNeighbor();
	    double disNear = dists[0]; 
	    for (int jj=0; jj<nFNabor; ++jj) {
        // Find a nearest point inside the triangle to the source pt 
		    CFace3d fNabor = pFaceNabors->GetAt(jj);
		    vv = vCur.NearestPt(fNabor,vT);
				float xx=vv.x(), yy=vv.y(), zz=vv.z();
		    if (vv.dist(&vCur)<disNear) {
	        currentNearV.Set(vv.x(),vv.y(),vv.z());
		      disNear = vv.dist(&vCur);
				}
			}
			dis += disNear;
			disL2 += disNear*disNear;
			if (disNear>disLM) {
				disLM = disNear;
				//Also record two nearest points
				pMesh->nearV1[0]=x; pMesh->nearV1[1]=y; pMesh->nearV1[2]=z;
				pMesh->nearV2[0]=currentNearV.x(); pMesh->nearV2[1]=currentNearV.y(); pMesh->nearV2[2]=currentNearV.z();
			}
      dists=0; idx=0; p=0; pFaceNabors=0;
			delete dists; delete idx; annDeallocPt(p); delete pFaceNabors;
		}
		vT=0; delete vT;
	}
	dis/=lCount;
	disL2/=lCount; disL2=sqrt(disL2);
	rDis[0]=dis; rDis[1]=disL2; rDis[2]=disLM; 
}

/************************************************************************/
/* Output vertex coordinates to a file                                  */
/************************************************************************/
void CMesh3d::output() {
  ofstream ost("outCurrentMesh.txt");
	for (int i=0; i<NbVertex(); ++i)
	  ost<<m_ArrayVertex[i]->x()<<endl
		   <<m_ArrayVertex[i]->y()<<endl
			 <<m_ArrayVertex[i]->z()<<endl;
}

void CMesh3d::output3() {
  ofstream ost("outCurrentMesh3.txt");
	for (int i=0; i<NbVertex(); ++i)
		ost<<m_ArrayVertex[i]->x()<<" "
			 <<m_ArrayVertex[i]->y()<<" "
			 <<m_ArrayVertex[i]->z()<<","<<endl;
}

double CMesh3d::ComputeAvgEdgeLength() {
  avgEdgeLength = 0;
	/*for (int i=0; i<NbFace(); ++i) {
		avgEdgeLength += m_ArrayFace[i]->
	}*/
	return 0;
}

void CMesh3d::mirror() {
	float x, y, z;
  for (int i=0; i<NbVertex(); ++i) {
		x = m_ArrayVertex[i]->x();
		y = m_ArrayVertex[i]->y();
		z = m_ArrayVertex[i]->z();
		m_ArrayVertex[i]->Set(-x, y, z);
  }
	m_Modified = 1;
}

/************************************************************************/
/* Andresen 2000 TMI evolving                                            
   evolve the current mesh to the target use diffused displacement field*/
/************************************************************************/
void CMesh3d::Evolve(CMesh3d* target) {
  BuildSingleCorrespondence(target);
	BuildSingleInverseCorrespondence(target);
	int size = NbVertex(), k = 30;
	ANNdistArray dists = new ANNdist[k];	
	ANNidxArray idx = new ANNidx[k];
	ANNpoint pSrc = annAllocPt(3);
	// generate displacement vector field
	CVector3d* disV = new CVector3d[size];
  for (int i = 0; i < size; ++i) {
		if (!m_ArrayVertex[i]->hasCorrespondence) continue;
		CVector3d dv(m_ArrayVertex[i],target->m_ArrayVertex[m_ArrayVertex[i]->match.indices[0]]);
		disV[i].Set(dv.x(),dv.y(),dv.z());
	}
	// update vector field with diffusion
	CVector3d* disV_avg = new CVector3d;
	float* x = new float[size];
	float* y = new float[size];
	float* z = new float[size];
	for (i = 0; i < size; ++i) {
		x[i] = y[i] = z[i] = 0;
		float thresDis = 50*sqrt(m_ArrayFace[i]->Area()*2);
		float thresDisSqr = thresDis*thresDis;
		// for each point, find nearby points
		pSrc[0] = m_ArrayVertex[i]->x(); 
		pSrc[1] = m_ArrayVertex[i]->y(); 
		pSrc[2] = m_ArrayVertex[i]->z();
		this->pVert_Tree3->annkFRSearch(pSrc, thresDis, k, idx, dists, 0);
		double w=0, sumW=0;
	  for (int j=0; j<k; ++j) {
			if (idx[j]<0) break;
			int nabor = idx[j];
			if (idx[j]>size-1) continue;
      // compute weights
			double distance = sqrt(
				(m_ArrayVertex[i]->x()-m_ArrayVertex[nabor]->x())*(m_ArrayVertex[i]->x()-m_ArrayVertex[nabor]->x())+
				(m_ArrayVertex[i]->y()-m_ArrayVertex[nabor]->y())*(m_ArrayVertex[i]->y()-m_ArrayVertex[nabor]->y())+
				(m_ArrayVertex[i]->z()-m_ArrayVertex[nabor]->z())*(m_ArrayVertex[i]->z()-m_ArrayVertex[nabor]->z()));
			w = exp(-0.5f*distance*distance/thresDisSqr);
			sumW += w;
      // get displacement vectors at nearby vertices
			x[i] += disV[idx[j]].x()*w;
			y[i] += disV[idx[j]].y()*w;
			z[i] += disV[idx[j]].z()*w;
		}
		x[i]/=sumW; y[i]/=sumW; z[i]/=sumW;
	}
	// update the location of vertices
	double timestep=0.1;
	for (i=0; i<size; ++i)
    m_ArrayVertex[i]->Set(m_ArrayVertex[i]->x()+x[i]*timestep,
		  m_ArrayVertex[i]->y()+y[i]*timestep,m_ArrayVertex[i]->z()+z[i]*timestep);
	x=0; y=0; z=0;
	delete x; delete y; delete z;
}

float CMesh3d::GetAvgAspectRatio() {
	float avgAR=0;
	for (int i=0; i<NbFace(); ++i) 
		avgAR += m_ArrayFace[i]->aspectRatio();
  avgAR /= NbFace();
	return avgAR;
}

void CMesh3d::outputLandmarks(int srctar, CString s) {
  ofstream ost(s);
  int size = m_ArrayLandmark.GetSize();
  ost<<srctar<<"  "<<size<<endl;
  for (int i=0; i<size; ++i)
	ost<<m_ArrayLandmark[i]->x()<<" "
	   <<m_ArrayLandmark[i]->y()<<" "
	   <<m_ArrayLandmark[i]->z()<<" "
	   <<m_ArrayLandmarkInd[i]<<endl;
}

void CMesh3d::setstcflag(int flag) {
  stcflag = flag;
}

/************************************************************************/
/* Bend the model on x direction                                        */
/************************************************************************/
void CMesh3d::bend() {
  //srand (time(NULL));
  srand(314159);
  int i, n = NbVertex();
  float x,y,z;
  float xMin = Inf, yMin = Inf, zMin = Inf, xMax = -Inf, yMax = -Inf, zMax = -Inf;
  float xBoxSize, yBoxSize, zBoxSize;
  for (i=0; i<n; ++i) {
    x = m_ArrayVertex[i]->x();
    y = m_ArrayVertex[i]->y();
    z = m_ArrayVertex[i]->z();
    if (x<xMin) xMin = x;
    if (x>xMax) xMax = x;
    if (y<yMin) yMin = y;
    if (y>yMax) yMax = y;
    if (z<zMin) zMin = z;
    if (z>zMax) zMax = z;
  }
  xBoxSize = xMax-xMin; yBoxSize = yMax-yMin; zBoxSize = zMax-zMin;
  float* dv;
  float level = 1000.0;
  float *a = new float[10];
  float *b = new float[10];
  float *c = new float[10];
  float* noise = new float[n];
  for (i=0; i<10; ++i) 
    a[i] = ((rand()%100)/100.0-0.5)*2;
  for (i=0; i<10; ++i) 
    b[i] = ((rand()%100)/100.0-0.5)*2;
  for (i=0; i<10; ++i) 
    c[i] = ((rand()%100)/100.0-0.5)*2;
  for (i=0; i<n; ++i)
    noise[i] = ((rand()%100)/100.0-0.5)*2*level*0.03f;
  float** dvV = new float*[n];
  float maxNorm = -1;
  float dvZsum=0;
  for (i=0; i<n; ++i) {
    x = m_ArrayVertex[i]->x();
    y = m_ArrayVertex[i]->y();
    z = m_ArrayVertex[i]->z();
    dvV[i] = disVecFunc(x,y,z,xBoxSize,yBoxSize,zBoxSize,a,b,c);
    dvZsum += dvV[i][2];
    float normV = sqrt(x*x+y*y+z*z);
    if (normV>maxNorm) maxNorm=normV;
  }
  dvZsum /= n;
  float factor = level/maxNorm;
  for (i=0; i<n; ++i) {
    x = m_ArrayVertex[i]->x();
    y = m_ArrayVertex[i]->y();
    z = m_ArrayVertex[i]->z();
    m_ArrayVertex[i]->
      Set(x, y, z+(dvV[i][2]-dvZsum)*factor);
  }
  m_Modified = 1;
  dv = 0; delete dv; dvV = 0; delete dvV;
  a = 0; delete a; b = 0; delete b; c = 0; delete c;
}

float* CMesh3d::disVecFunc(float x, float y, float z, 
                           float xBSize, float yBSize, float zBSize, 
                           float* a, float* b, float* c) {
  float vx = fabs(quadFunc(x/xBSize,y/yBSize,z/zBSize,a));
  float vy = fabs(quadFunc(x/xBSize,y/yBSize,z/zBSize,b));
  float vz = fabs(quadFunc(x/xBSize,y/yBSize,z/zBSize,c));
  float *v = new float[3];
  v[0] = vx; v[1] = vy; v[2] = vz;
  return v;
}

float CMesh3d::quadFunc(float x, float y, float z, float* a) {
  return a[0]*x*x+a[1]*y*y+a[2]*z*z+a[3]*x*y+a[4]*x*z+a[5]*y*z+a[6]*x+a[7]*y+a[8]*z+a[9];
}

float CMesh3d::linearFunc(float x, float y, float z, float* a) {
  return a[0]*fabs(x)+a[1]*fabs(y)+a[2]*fabs(z)+a[3];
}

void CMesh3d::showTex() {
  isShowTex = !isShowTex;
}

void CMesh3d::outputCenters() {
  int fSize = NbFace();
  float x,y,z,s,t;
  double eps = m_ArrayFace[10]->Area()/5;
  ofstream ost("randPnts.asc");
  for (int i=0; i<fSize; ++i) {
    // pick the first vertex of the triangle as origin 
    CVertex3d* pOrigin = m_ArrayFace[i]->v1();
    // two edge vectors
    CVector3d e1(m_ArrayFace[i]->v1(), m_ArrayFace[i]->v2());
    CVector3d e2(m_ArrayFace[i]->v1(), m_ArrayFace[i]->v3());
		e1.NormalizeL2(1.0); e2.NormalizeL2(1.0);
    int n = (int)(m_ArrayFace[i]->Area()/eps)+1;
    //for (int i=0; i<n; ++i) {
      // s\in(0,1)
      s = (rand()%1000)/1000.0;
      // t\in(0,1-s)
      t = (rand()%1000)/1000.0*(1-s);
      x = pOrigin->x()+e1.x()*s+e2.x()*t;
      y = pOrigin->y()+e1.y()*s+e2.y()*t;
	    z = pOrigin->z()+e1.z()*s+e2.z()*t;
      ost<<x<<","<<y<<","<<z<<endl;
    //}
  }

//   for (int i=0; i<NbVertex(); ++i) 
//     ost<<m_ArrayVertex[i]->x()<<","<<m_ArrayVertex[i]->y()<<","<<m_ArrayVertex[i]->z()<<endl;
}

void CMesh3d::setTexCoord(CMesh3d* pMesh) {
  pCoordMesh = pMesh;
}




