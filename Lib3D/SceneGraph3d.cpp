//********************************************
// SceneGraph.cpp
//********************************************
// class CSceneGraph
//********************************************
// alliez@usc.edu
// Created : 15/01/98
// Modified : 15/01/98
//********************************************

#include "stdafx.h"
#include "Base3d.h"
#include "SceneGraph3d.h"
//////////////////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////////////////

//********************************************
// Constructor
//********************************************
CSceneGraph3d::CSceneGraph3d() {
	m_pIndexTextureBinding = NULL;
	m_ListDone = 0;
	isShowPrinAxes = false;
	fittingTimes = 0;
	normalFlip = 1;
	haveCurrentMesh = false;
	isGenSrc = false;
	isGenTarget = false;
	isShowNearestPoints = true;
	isShowCorrespondences = false;
	corNo = -1;
	SrcOrTgtCor = -1;
	isAddingLandmarks = isGenSrc = isGenTarget = false;
	isShowLandmarks = true;
	dis= -1;
	hasBuiltCorrespondences = false;
  CTexture *pTexture = new CTexture;
  pTexture->ReadFileBMP("F:\\checkerboard.bmp");
  int IndexTexture = AddTexture(pTexture);
}

//********************************************
// Destructor
//********************************************
CSceneGraph3d::~CSceneGraph3d() {
	Free();
}

//********************************************
// Destructor
//********************************************
void CSceneGraph3d::Free(void)
{
	// Textures
	if(m_ArrayTexture.GetSize())
	{
		::glDeleteTextures(m_ArrayTexture.GetSize(),m_pIndexTextureBinding);
		if(m_pIndexTextureBinding != NULL)
		{
			delete [] m_pIndexTextureBinding;
			m_pIndexTextureBinding = NULL;
		}
	}
	m_ArrayTexture.Free();
	// Objects
	m_ArrayObject3d.Free();
}

//////////////////////////////////////////////
// DATAS
//////////////////////////////////////////////

//********************************************
// Add
//********************************************
void CSceneGraph3d::Add(CObject3d *pObject3d)
{
	m_ArrayObject3d.Add(pObject3d);
}

//********************************************
// Add
//********************************************
void CSceneGraph3d::RemoveAt(int index)
{
	m_ArrayObject3d.RemoveAt(index);
}

//********************************************
// GetAt
//********************************************
CObject3d *CSceneGraph3d::GetAt(int index)
{
	ASSERT(index < m_ArrayObject3d.GetSize());
	return m_ArrayObject3d[index];
}
	
//********************************************
// Operator []
//********************************************
CObject3d *CSceneGraph3d::operator[](int index)
{
	ASSERT(index < m_ArrayObject3d.GetSize());
	return m_ArrayObject3d[index];
}
	

//////////////////////////////////////////////
// OPENGL
//////////////////////////////////////////////

//********************************************
// BuildList
//********************************************
int CSceneGraph3d::glBuildList() {
	TRACE("Build list");
	// Meshes
	//***********************************
	unsigned int size = m_ArrayObject3d.GetSize();
	for(unsigned int i=0; i<size; i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d.GetAt(i);
		if(pObject3d != NULL)
		{
			pObject3d->glBuildList();
			TRACE("."); // progressing (debug mode)
		}
	}
	TRACE("ok\n");

	// Textures
	//***********************************
  unsigned int NbTexture = m_ArrayTexture.GetSize();
	if(NbTexture)
	{
		TRACE("SceneGraph : texture binding...(%d texture(s))\n",NbTexture);

		// Cleanup
		if(m_pIndexTextureBinding != NULL)
		{
			::glDeleteTextures(NbTexture,m_pIndexTextureBinding);
			delete [] m_pIndexTextureBinding;
		}
		m_pIndexTextureBinding = new unsigned int[NbTexture];

		::glGenTextures(NbTexture,m_pIndexTextureBinding);
		int error = glGetError();
		ASSERT(error !=  GL_INVALID_VALUE);
		ASSERT(error !=  GL_INVALID_OPERATION);

		TRACE("Bind texture...\n");
		for(i=0;i<NbTexture;i++)
		{
			while (GL_NO_ERROR != glGetError() ) {}

			// Bind texture
// 			glBindTexture(GL_TEXTURE_2D,m_pIndexTextureBinding[i]);
// 
// 			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// 			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// 			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			int error = glGetError();
			ASSERT(error !=  GL_INVALID_ENUM);
			ASSERT(error !=  GL_INVALID_OPERATION);

			// Read datas
			CTexture *pTexture = m_ArrayTexture[i];
			ASSERT(pTexture->GetData() != NULL);
			::glTexImage2D(GL_TEXTURE_2D,0,3,
						   pTexture->GetWidth(),pTexture->GetHeight(),0,			
						   GL_RGB,GL_UNSIGNED_BYTE,pTexture->GetData());
			error = glGetError();
			ASSERT(error !=  GL_INVALID_ENUM);
			ASSERT(error !=  GL_INVALID_OPERATION);
			ASSERT(error !=  GL_INVALID_VALUE);

		}

	}
	m_ListDone = 1;

	return 1;
}

//********************************************
// glDraw
//********************************************
void CSceneGraph3d::glDraw(void) {
  //show correspondences in particular places
  if (SrcOrTgtCor<NbObject() && SrcOrTgtCor>-1) {
	int maxCor;
	if (!isGenSrc) GenSrcMesh();
	if (!isGenTarget) GenTargetMesh();
	if (SrcOrTgtCor==0) maxCor=m_Src.NbVertex(); // the source mesh and current mesh have a same vertex number
  if (corNo>=0 && corNo<maxCor) {
	  glPushMatrix();
	  // show correspodences from source to target
	  glBegin(GL_LINES);
	  glColor3ub(0, 0, 0);
	  if (SrcOrTgtCor==0) {
		  if (!hasBuiltCorrespondences) {
		    BuildCorrespondences(&m_Src,&m_Tar,30);
        hasBuiltCorrespondences=true;
      }
	    if (m_Src.m_ArrayVertex[corNo]->hasCorrespondence) {
		    for (int j=0; j<m_Src.m_ArrayVertex[corNo]->match.num; ++j) {
		      if (m_Src.m_ArrayVertex[corNo]->match.indices[j]<0) continue;
		      glVertex3f(m_Src.m_ArrayVertex[corNo]->x(), 
			               m_Src.m_ArrayVertex[corNo]->y(), 
		       	         m_Src.m_ArrayVertex[corNo]->z());
		      int mInd = m_Src.m_ArrayVertex[corNo]->match.indices[j];
		      glVertex3f(m_Tar.m_ArrayVertex[mInd]->x(), 
			               m_Tar.m_ArrayVertex[mInd]->y(), 
			  		         m_Tar.m_ArrayVertex[mInd]->z());
        }
      }
	  }
	  else {
	    
	  }
	  glEnd();
	  glPopMatrix();
	}
  }
// 	if(!m_ListDone) glBuildList();
  if (haveCurrentMesh) {
	  glPushMatrix();
	//glLoadIdentity();
	//glLoadMatrixf(modelviewCurrent);
	//m_CurrentMesh.MultMat(2, modelviewCurrent);
	m_CurrentMesh.glDraw();
	//correspondence pairs
	if (isShowCorrespondences) {
	  //from m_Before to m_Tar...
	  glBegin(GL_LINES);
	  for (int i=0; i<m_CurrentMesh.NbVertex(); ++i) {
		  if (!m_CurrentMesh.m_ArrayVertex[i]->hasCorrespondence) continue;
		  glColor3ub(0, 0, 0);
		  for (int j=0; j<m_CurrentMesh.m_ArrayVertex[i]->match.num; ++j) {
		    //if (m_CurrentMesh.m_ArrayVertex[i]->match.indices[j]<0) continue;
			  glVertex3f(m_Before.m_ArrayVertex[i]->x(), m_Before.m_ArrayVertex[i]->y(), m_Before.m_ArrayVertex[i]->z());
			  int mInd = m_CurrentMesh.m_ArrayVertex[i]->match.indices[j];
			  glVertex3f(m_Tar.m_ArrayVertex[mInd]->x(), m_Tar.m_ArrayVertex[mInd]->y(), m_Tar.m_ArrayVertex[mInd]->z());
      }
	  }  
	  //from m_Tar to m_CurrentMesh...
	  for (i=0; i<m_Tar.NbVertex(); ++i) {
	    if (!m_Tar.m_ArrayVertex[i]->hasInvCorrespondence) continue;
	    glColor3ub(255, 0, 0);
	    for (int j=0; j<m_Tar.m_ArrayVertex[i]->match.num; ++j) {
		    if (m_Tar.m_ArrayVertex[i]->match.indices[j]<0) continue;
		    int mInd = m_Tar.m_ArrayVertex[i]->match.indices[j];
		    glVertex3f(m_Tar.m_ArrayVertex[i]->x(), m_Tar.m_ArrayVertex[i]->y(), m_Tar.m_ArrayVertex[i]->z());
		    glVertex3f(m_Before.m_ArrayVertex[mInd]->x(), m_Before.m_ArrayVertex[mInd]->y(), m_Before.m_ArrayVertex[mInd]->z());
		  }
	  }
	  glEnd();
	}
	glPopMatrix();
  }
  CObject3d *pObject3d;
  CMesh3d *pMesh;
  unsigned int size = m_ArrayObject3d.GetSize();
  for(unsigned int i=0; i<size; i++) {
	pObject3d = m_ArrayObject3d[i];
  pMesh = (CMesh3d *)pObject3d;
		// Drawing
	glPushMatrix();
		//glLoadIdentity();
		//glMultMatrixf(modelviewObject[i]);
	pMesh->glDraw();
	glPopMatrix();
		// 		}
   // 		else
// 			pObject3d->glDraw();
  }
  pObject3d=0; delete pObject3d; 
  pMesh=0; delete pMesh;
}

//********************************************
// glDraw
//********************************************
void CSceneGraph3d::glDrawDirect(void) {
	unsigned int size = m_ArrayObject3d.GetSize();
	for(unsigned int i=0; i<size; i++) {
		CObject3d *pObject3d = m_ArrayObject3d[i];
		// Meshes
	  CMesh3d *pMesh = (CMesh3d*)pObject3d;
    pMesh->glDrawDirect();
	}
}


//********************************************
// glDraw
// draw only type
//********************************************
void CSceneGraph3d::glDraw(int type) {
	if(!m_ListDone) glBuildList();
	unsigned int size = m_ArrayObject3d.GetSize();
	for(unsigned int i=0; i<size; i++) {
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() == type) pObject3d->glDraw();
	}
	m_CurrentMesh.glDraw();
}


//////////////////////////////////////////////
// MISC
//////////////////////////////////////////////

//********************************************
// BuildAdjacency
// For each mesh
//********************************************
int CSceneGraph3d::BuildAdjacency() {
	int size = m_ArrayObject3d.GetSize();
	if(size==0) return 0;
	
	TRACE("Scene %x : Start BuildAdjacency...\n",this);
	TRACE("  NbObject : %d\n",size);
	CObject3d *pObject3d;
  CMesh3d *pMesh;
	for(int i=0;i<size;i++) {
		pObject3d = m_ArrayObject3d[i];
		pMesh = (CMesh3d *)m_ArrayObject3d[i];
    if (!pMesh->isSetCoordMesh) {
      pMesh->setTexCoord(((CMesh3d*)m_ArrayObject3d[0]));
      pMesh->isSetCoordMesh = true;
      //pMesh->isShowTex = true;
    }
		if (!pMesh->isBuildAdjacency) pMesh->BuildAdjacency();
    TRACE("Scene %x : End BuildAdjacency...\n",this);
		if (i == 0) {
			//m_OriSrc.Copy(pMesh);
			pMesh->setstcflag(0);
			//pMesh->SetColor(127, 0, 127);
			pMesh->SetColor(0, 127, 200);
      pMesh->m_IndexTexture=0;
		}
		if (i == 1) {
			//m_OriTar.Copy(pMesh);
			pMesh->setstcflag(1);
			pMesh->SetColor(200, 127, 100);
		}
		if (i == 2) pMesh->SetColor(200, 127, 200);
		if (i == 3) pMesh->SetColor(200, 0, 0);
		if (i == 4) pMesh->SetColor(0, 0, 200);
    if (i == 5) pMesh->SetColor(0, 100, 0);
		if (i == 6) pMesh->SetColor(200, 125, 0);
	}
	pObject3d = 0; pMesh = 0; delete pObject3d; delete pMesh;
	return size;
}

//********************************************
// CalculateNormalPerVertex
// For each mesh
//********************************************
int CSceneGraph3d::CalculateNormalPerVertex()
{
	int size = m_ArrayObject3d.GetSize();
	if(size ==0)
		return 0;

	for(int i=0;i<size;i++) {
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->CalculateNormalPerVertex();
	}

	return 1;
}

//********************************************
// CalculateNormalPerFace
// For each mesh
//********************************************
int CSceneGraph3d::CalculateNormalPerFace()
{
	int size = m_ArrayObject3d.GetSize();
	if(size ==0)
		return 0;

	for(int i=0;i<size;i++)
		{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->CalculateNormalPerFace();
		}

	return 1;
}


//********************************************
// SetNormalBinding
//********************************************
void CSceneGraph3d::SetNormalBinding(int type)
{
	int size = m_ArrayObject3d.GetSize();
	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->SetNormalBinding(type);
	}
}

//********************************************
// SetColorBinding
//********************************************
void CSceneGraph3d::SetColorBinding(int type)
{
	int size = m_ArrayObject3d.GetSize();
	for(int i=0;i<size;i++)
	{
		CObject3d *pObject3d = m_ArrayObject3d[i];
		if(pObject3d->GetType() != TYPE_MESH3D)
			continue;
		CMesh3d *pMesh = (CMesh3d *)m_ArrayObject3d[i];
		pMesh->SetColorBinding(type);
	}
}


//////////////////////////////////////////////
// TEXTURES
//////////////////////////////////////////////

//********************************************
// HasTexture
//********************************************
int CSceneGraph3d::HasTexture(char *name,
															int *index)
{
	for(int i=0;i<m_ArrayTexture.GetSize();i++)
		if(m_ArrayTexture[i]->GetFileName() == name)
		{
			*index = i;
			return 1;
		}
	return 0;
}


//////////////////////////////////////////////
// I/O
//////////////////////////////////////////////

//********************************************
// SaveFile
//********************************************
int CSceneGraph3d::SaveFile(char *name)
{
	// Check
	if(NbObject() == 0)
	{
		AfxMessageBox("This scene does not contain meshes");
		return 0;
	}

	// Check for valid file
	CStdioFile file;
	CFileException ex;
	
	// Write header
	if(!WriteHeader(file,name))
	{
		AfxMessageBox("Error during writing header");
		return 0;
	}

	// Meshes
	for(int i=0;i<NbObject();i++)
	{
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D)
			((CMesh3d *)pObject)->WriteFile(file);
	}

	// Close file
  file.Close();

	return 1;
}

//********************************************
// SaveFileRaw
//********************************************
int CSceneGraph3d::SaveFileRaw(char *name)
{
	// Check
	if(NbObject() == 0)
	{
		AfxMessageBox("This scene does not contain meshes");
		return 0;
	}

	// Check for valid file
	CFile file;
	CFileException ex;

	// Try to open file (text mode)
	if(!file.Open(name,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary,&ex))
	{
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		AfxMessageBox("Unable to open file for writing");
		return 0;
	}

	
	// Meshes
	unsigned int NbMesh = NbObject();
	file.Write(&NbMesh,sizeof(unsigned int));
	for(unsigned int i=0;i<NbMesh;i++) {
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D)
			((CMesh3d *)pObject)->WriteFileRaw(file);
		pObject=0; delete pObject;
	}

	// Close file
  file.Close();

	return 1;
}


//**********************************************
// WriteHeader
// Do not close file
//**********************************************
int CSceneGraph3d::WriteHeader(CStdioFile &file,
															 char *name)
{
	CFileException ex;
	
	// Try to open file (text mode)
	if(!file.Open(name,CFile::modeCreate | CFile::modeWrite | CFile::typeText,&ex))
	{
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		AfxMessageBox("Unable to open file for writing");
		return 0;
	}

	// ** Header *******************************
	TRACE("\nSave VRML 2.0 File...\n");
	TRACE("  name : %s\n",name);
	TRY
	{
		file.WriteString("#VRML V2.0 utf8\n\n");
		file.WriteString("# Produced by 3d Toolbox 1.0 (Pierre Alliez, CNET / DIH / HDM)\n\n");
	}
	CATCH(CFileException, e)
	{
		#ifdef _DEBUG
				afxDump << "Error during writing " << e->m_cause << "\n";
		#endif
		AfxMessageBox("Error during writing file header");
		file.Close();
		return 0;
	}
	END_CATCH

	// do not close file
	return 1;
}

bool CSceneGraph3d::GetPrinAxes() {
	int size = m_ArrayObject3d.GetSize();
	for (int i = 0; i < size; ++i) {
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D) ((CMesh3d *)pObject)->GenPrinAxes();
		pObject=0; delete pObject;
	}
	return true;
}

bool CSceneGraph3d::PrinAlignment() {
	int size = m_ArrayObject3d.GetSize();
	CObject3d *pObject = m_ArrayObject3d[0];
	Array2D<float> m_Pa(3, 3);
	if(pObject->GetType() == TYPE_MESH3D) m_Pa = ((CMesh3d *)pObject)->m_prinAxes;
	//Get the first principal axis of the standard mesh
	//The longest axis is stored in the last(3rd) column...the blue line
	Vec3f v;
	for (int i = 0; i < 3; ++i) v[i] = m_Pa[i][2]; 
	Normalize(v);
	Vec3f v1;
	for (i = 0; i < 3; ++i) v1[i] = m_Pa[i][1]; 
	Normalize(v1);
	//Rotate the target mesh to the standard place
	for (i = 1; i < size; ++i) {
		//Get the first rotation axes
		CObject3d *pTargetObject = m_ArrayObject3d[i];
		CMesh3d pTargetObjecta; 
		CMesh3d pTargetObjectb;
		pTargetObjecta.CopyVertex((CMesh3d *)pTargetObject);
		pTargetObjectb.CopyVertex((CMesh3d *)pTargetObject);

		if(pTargetObject->GetType() == TYPE_MESH3D) m_Pa = ((CMesh3d *)pTargetObject)->m_prinAxes;
		Vec3f vTarget;
		for (int j = 0; j < 3; ++j) vTarget[j] = m_Pa[j][2]; Normalize(vTarget);
		Vec3f vTarget1;
		for (j = 0; j < 3; ++j) vTarget1[j] = m_Pa[j][1]; Normalize(vTarget1);

		Vec3f axis; float angle;
		Vec3f axisa = Cross(vTarget, v); Normalize(axisa);
		//then get the rotation angle
		float anglea = acos(Dot(v, vTarget));
		//finally apply the mesh rotation
		pTargetObjecta.Rotate(axisa, anglea);
		Vec3f axisb = Cross(-vTarget, v); Normalize(axisb);
		//then get the rotation angle
		float angleb = acos(Dot(v, -vTarget));
		//finally apply the mesh rotation
		pTargetObjectb.Rotate(axisb, angleb);

		//Now check the direction of this principal axis via computing the average nearest distance
		float disa = ((CMesh3d *)pObject)->ComputAveNearestDis(&pTargetObjecta);
		float disb = ((CMesh3d *)pObject)->ComputAveNearestDis(&pTargetObjectb);
		if (disa <= disb) {
			axis = axisa; angle = anglea;
		}
		else {
			axis = axisb; angle = angleb;
		}
 		((CMesh3d *)pTargetObject)->Rotate(axis, angle);
		
		float cosa = cos(angle), sina = sin(angle);
		float t = 1 - cosa;
		float x = axis[0], y = axis[1], z = axis[2], nx, ny, nz;
		nx = (t * x * x + cosa) * vTarget1[0] + (t * x * y - sina * z) * vTarget1[1] + (t * x * z + sina * y) * vTarget1[2];
		ny = (t * x * y + sina * z) * vTarget1[0] + (t * y * y + cosa) * vTarget1[1] + (t * y * z - sina * x) * vTarget1[2];
		nz = (t * x * z - sina * y) * vTarget1[0] + (t * y * z + sina * x) * vTarget1[1] + (t * z * z + cosa) * vTarget1[2];
		vTarget1.x = nx; vTarget1.y = ny; vTarget1.z = nz;
		Normalize(vTarget1);
		Vec3f axis1 = Cross(vTarget1, v1); Normalize(axis1);
		float angle1 = acos(Dot(v1, vTarget1));
		((CMesh3d *)pTargetObject)->Rotate(axis1, angle1);
		((CMesh3d *)pTargetObject)->SetModified();
	}
	GetPrinAxes();
	glDraw();
	return true;
}

float CSceneGraph3d::ComputAveNearestDis(CMesh3d *src, CMesh3d *des) {
	float dis = src->ComputAveNearestDis(des);
	return dis;
}

void CSceneGraph3d::TogglePrinAxis() {
	int size = m_ArrayObject3d.GetSize();
	bool isShowPrinAxes = ((CMesh3d *)m_ArrayObject3d[0])->isShowPrinAxes;
	for (int i = 0; i < size; ++i) {
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D) {
			((CMesh3d *)pObject)->isShowPrinAxes = !isShowPrinAxes;
			((CMesh3d *)pObject)->SetModified();
		}
		pObject=0; delete pObject;
	}
	glDraw();
}

void CSceneGraph3d::ToggleNearestPntsShow() {
	if (haveCurrentMesh) {
    m_CurrentMesh.isShowNearestPnts=!m_CurrentMesh.isShowNearestPnts;
    m_CurrentMesh.SetModified();
    glDraw();
	}
}

void CSceneGraph3d::ToggleCorrespondences() {
	if (haveCurrentMesh) {
		m_CurrentMesh.isShowCorrespondences=!m_CurrentMesh.isShowCorrespondences;
		isShowCorrespondences = !isShowCorrespondences;
		m_CurrentMesh.SetModified();
		glDraw();
	}
}

void CSceneGraph3d::NonRigidFittingM() {
	int size = m_ArrayObject3d.GetSize();
	if (size < 2) {AfxMessageBox("Invalid operation!"); return;}
	
	DWORD dwStart = ::GetTickCount();
	int times = 20;
	for (int iTimes=0; iTimes<times; ++iTimes) {
		if (!isGenSrc) {
			GenSrcMesh(); 
			isGenSrc = true;
		}
		if (!isGenTarget) {
			GenTargetMesh();
			isGenTarget = true;
		}
	m_Before.Copy(&m_CurrentMesh);
	fittingTimes++;
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	//for finding the inverse correspondence
	m_CurrentMesh.BuildAnnKDTree();
	int kCor = 50/pow(1.5, fittingTimes)+1;
	//float thresDisSrc = m_Src.m
	//float thresDisTar = m_Tar.
	float disWeight = pow(1.05, fittingTimes);
	float elasWeight = 1000;
	ComputeDistanceEnergy(disWeight, kCor);
	ComputeElasticPotentialEnergy(elasWeight);
	//now, begin to fill in the matrix A
	int n = m_CurrentMesh.NbVertex();
	int i, j, k, ii, jj, iii, nn;
	//Coefficient matrix for optimization 
	Sparse_Matrix<float> A(n*3, n*3);
	//vector b and initial x
	Vector<float> b(n*3), x(n*3);
	//traverse vertices
	for (i = 0; i < n; ++i) {
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->x2C, 3*i, 3*i);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->y2C, 3*i+1, 3*i+1);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->z2C, 3*i+2, 3*i+2);
		//insert inverse correspondences coefficients
		int invCorNaborNum = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor.size();
		for (k = 0; k < invCorNaborNum; ++k) {
			int ii = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor[k];
			float value = 0.5*m_CurrentMesh.m_ArrayVertex[i]->invCorValue[k]*disWeight;
			A.increase(value, 3*i, 3*ii);
			A.increase(value, 3*i+1, 3*ii+1);
			A.increase(value, 3*i+2, 3*ii+2);
			A.increase(value, 3*ii, 3*i);
			A.increase(value, 3*ii+1, 3*i+1);
			A.increase(value, 3*ii+2, 3*i+2);
		}
		b[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x1C;
		b[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y1C;
		b[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z1C;
		x[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x();
		x[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y();
		x[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z();
		
		// potential energy terms
		// 		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
		// 		for (int j = 0; j < nabor.size(); ++j) {
		// 			int ii = nabor[j];
		// 			//edge based energy term (mixed quadratic terms)
		// 			A.insert(-elasWeight, 3*i, 3*ii);
		// 			A.insert(-elasWeight, 3*i+1, 3*ii+1);
		// 			A.insert(-elasWeight, 3*i+2, 3*ii+2);	
		// 			A.insert(-elasWeight, 3*ii, 3*i);
		// 			A.insert(-elasWeight, 3*ii+1, 3*i+1);
		// 			A.insert(-elasWeight, 3*ii+2, 3*i+2);	
		//internal edges
		// 			int prev = j - 1; 
		// 			if (j==0) prev = nabor.size()-1;
		// 			int next = j + 1;
		// 			if (j==nabor.size()-1) next = 0;
		// 			int prevNabor = nabor[prev];
		// 			int nextNabor = nabor[next];
		// 			A.insert(-elasWeight, 3*prevNabor, 3*nextNabor);
		// 			A.insert(-elasWeight, 3*prevNabor+1, 3*nextNabor+1);
		// 			A.insert(-elasWeight, 3*prevNabor+2, 3*nextNabor+2);	
		// 			A.insert(-elasWeight, 3*nextNabor, 3*prevNabor);
		// 			A.insert(-elasWeight, 3*nextNabor+1, 3*prevNabor+1);
		// 			A.insert(-elasWeight, 3*nextNabor+2, 3*prevNabor+2);
		// 		}
		
		// potential energy terms
		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
		nn = nabor.size();
		float nn1 = 1.0/nn;
    float elasW = (100/m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround())*elasWeight;
		for (j = 0; j < nn; ++j) {
			ii = nabor[j];
			//ring based energy term (mixed quadratic terms)
			A.increase(-elasW*nn1, 3*i, 3*ii);
			A.increase(-elasW*nn1, 3*i+1, 3*ii+1);
			A.increase(-elasW*nn1, 3*i+2, 3*ii+2);	
			A.increase(-elasW*nn1, 3*ii, 3*i);
			A.increase(-elasW*nn1, 3*ii+1, 3*i+1);
			A.increase(-elasW*nn1, 3*ii+2, 3*i+2);	
			for (jj = j+1; jj < nn; ++jj) {
				iii = nabor[jj];
				A.increase(elasW*nn1*nn1, 3*ii, 3*iii);
				A.increase(elasW*nn1*nn1, 3*ii+1, 3*iii+1);
				A.increase(elasW*nn1*nn1, 3*ii+2, 3*iii+2);	
				A.increase(elasW*nn1*nn1, 3*iii, 3*ii);
				A.increase(elasW*nn1*nn1, 3*iii+1, 3*ii+1);
				A.increase(elasW*nn1*nn1, 3*iii+2, 3*ii+2);
			}		
		}
	}
	//ofstream ost("xxx.txt"); 
	//A.print(ost);
	//send A, b & x into Conjugate Gradient
	x = ConjugateGradient(A, b, x, 101);
	//assign x to the current mesh
	for (i = 0; i < n; ++i) m_CurrentMesh.m_ArrayVertex[i]->Set(x[3*i], x[3*i+1], x[3*i+2]);
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	haveCurrentMesh = true;
	avgAspectRatioCurrent = m_CurrentMesh.GetAvgAspectRatio();
	}
	DWORD dwEnd = ::GetTickCount();
	CString sTime;
	sTime.Format("%8.4f",(dwEnd-dwStart)*(1e-3));
  //AfxMessageBox(sTime);
	ComputeDis();
	glDraw();
}

void CSceneGraph3d::Andresen2000Fitting() {
  int size = m_ArrayObject3d.GetSize();
	if (size < 2) {AfxMessageBox("Invalid operation!"); return;}
  
	DWORD dwStart = ::GetTickCount();
	int times = 20;
	for (int iTimes=0; iTimes<times; ++iTimes) {
	if (!isGenSrc) {
		GenSrcMesh(); 
		isGenSrc = true;
	}
	if (!isGenTarget) {
		GenTargetMesh();
		isGenTarget = true;
	}
	m_Before.Copy(&m_CurrentMesh);
	fittingTimes++;
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.BuildAnnKDTree();
  
	// diffusion...main part
	m_CurrentMesh.Evolve(&m_Tar);
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	haveCurrentMesh = true;
	avgAspectRatioCurrent = m_CurrentMesh.GetAvgAspectRatio();
	}
	DWORD dwEnd = ::GetTickCount();
	CString sTime;
	sTime.Format("%8.4f",(dwEnd-dwStart)*(1e-3));
  //AfxMessageBox(sTime);
	ComputeDis();
	glDraw();
}

void CSceneGraph3d::Pauly2005FittingMult() {
  int size = m_ArrayObject3d.GetSize();
	if (size < 2) {AfxMessageBox("Invalid operation!"); return;}
  int times = 10;
	
	for (int iTimes=0; iTimes<times; ++iTimes) {
		if (!isGenSrc) {
			GenSrcMesh(); 
			isGenSrc = true;
		}
		if (!isGenTarget) {
			GenTargetMesh();
			isGenTarget = true;
	}
	m_Before.Copy(&m_CurrentMesh);
	fittingTimes++;
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.BuildAnnKDTree();

  // main part
	int edgeNum = m_Src.m_ArrayEdge.GetSize();
	int kCor = 50/pow(1.5, fittingTimes)+1;
	float disWeight = pow(1.1, fittingTimes);
	float elasWeight = 100;
	ComputePaulyMultDistanceEnergy(disWeight, kCor);
	//ComputeElasticPotentialEnergy(elasWeight);
	ComputeEdgeBasedGeomEnergy(elasWeight);
	//now, begin to fill in the matrix A
	int n = m_CurrentMesh.NbVertex();
	//Coefficient matrix for optimization 
	Sparse_Matrix<float> A(n*3, n*3);
	//vector b and initial x
	Vector<float> b(n*3), x(n*3);
	//traverse vertices
	for (int i = 0; i < n; ++i) {
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->x2C, 3*i, 3*i);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->y2C, 3*i+1, 3*i+1);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->z2C, 3*i+2, 3*i+2);
		//insert inverse correspondences coefficients
		int invCorNaborNum = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor.size();
		for (int k = 0; k < invCorNaborNum; ++k) {
			int ii = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor[k];
			float value = 0.5*m_CurrentMesh.m_ArrayVertex[i]->invCorValue[k]*disWeight;
			A.increase(value, 3*i, 3*ii);
			A.increase(value, 3*i+1, 3*ii+1);
			A.increase(value, 3*i+2, 3*ii+2);
			A.increase(value, 3*ii, 3*i);
			A.increase(value, 3*ii+1, 3*i+1);
			A.increase(value, 3*ii+2, 3*i+2);
		}
		b[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x1C;
		b[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y1C;
		b[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z1C;
		x[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x();
		x[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y();
		x[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z();
		
		// potential energy terms
		// edge based
// 		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
// 		vector<float> naborW = m_CurrentMesh.m_ArrayVertex[i]->m_NaborWeight;
// 		for (int j = 0; j < nabor.size(); ++j) {
// 		  int ii = nabor[j];
// 			float eW = naborW[j];
//  			//edge based energy term (mixed quadratic terms)
//  			A.insert(-eW, 3*i, 3*ii);
//  			A.insert(-eW, 3*i+1, 3*ii+1);
// 			A.insert(-eW, 3*i+2, 3*ii+2);	
// 			A.insert(-eW, 3*ii, 3*i);
// 			A.insert(-eW, 3*ii+1, 3*i+1);
// 			A.insert(-eW, 3*ii+2, 3*i+2);	
// 		}
		// ring based
		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
		int nn = nabor.size();
		float nn1 = 1.0/nn;
		for (int j = 0; j < nn; ++j) {
			int ii = nabor[j];
			//ring based energy term (mixed quadratic terms)
			A.increase(-elasWeight*nn1, 3*i, 3*ii);
			A.increase(-elasWeight*nn1, 3*i+1, 3*ii+1);
			A.increase(-elasWeight*nn1, 3*i+2, 3*ii+2);	
			A.increase(-elasWeight*nn1, 3*ii, 3*i);
			A.increase(-elasWeight*nn1, 3*ii+1, 3*i+1);
			A.increase(-elasWeight*nn1, 3*ii+2, 3*i+2);	
			for (int jj = j+1; jj < nn; ++jj) {
				int iii = nabor[jj];
				A.increase(elasWeight*nn1*nn1, 3*ii, 3*iii);
				A.increase(elasWeight*nn1*nn1, 3*ii+1, 3*iii+1);
				A.increase(elasWeight*nn1*nn1, 3*ii+2, 3*iii+2);	
				A.increase(elasWeight*nn1*nn1, 3*iii, 3*ii);
				A.increase(elasWeight*nn1*nn1, 3*iii+1, 3*ii+1);
				A.increase(elasWeight*nn1*nn1, 3*iii+2, 3*ii+2);
			}		
		}
	}
	//send A, b & x into Conjugate Gradient
	x = ConjugateGradient(A, b, x, 101);
	//assign x to the current mesh
	for (i = 0; i < n; ++i) m_CurrentMesh.m_ArrayVertex[i]->Set(x[3*i], x[3*i+1], x[3*i+2]);

	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	avgAspectRatioCurrent = m_CurrentMesh.GetAvgAspectRatio();
	haveCurrentMesh = true;
	}
	ComputeDis(); 
	glDraw();
}

void CSceneGraph3d::Pauly2005Fitting() {
  int size = m_ArrayObject3d.GetSize();
	if (size < 2) {AfxMessageBox("Invalid operation!"); return;}
  
	DWORD dwStart = ::GetTickCount();
	int times = 20;
	for (int iTimes=0; iTimes<times; ++iTimes) {
	if (!isGenSrc) {
		GenSrcMesh(); 
		isGenSrc = true;
	}
	if (!isGenTarget) {
		GenTargetMesh();
		isGenTarget = true;
	}
	fittingTimes++;
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.BuildAnnKDTree();
  
	// main part
	int edgeNum = m_Src.m_ArrayEdge.GetSize();
	float disWeight = pow(1.1, fittingTimes);
	float elasWeight = 100;
	ComputeSingleDistanceEnergy(disWeight);
	//ComputeElasticPotentialEnergy(elasWeight);
	ComputeEdgeBasedGeomEnergy(elasWeight);
	//now, begin to fill in the matrix A
	int n = m_CurrentMesh.NbVertex();
	//Coefficient matrix for optimization 
	Sparse_Matrix<float> A(n*3, n*3);
	//vector b and initial x
	Vector<float> b(n*3), x(n*3);
	//traverse vertices
	for (int i = 0; i < n; ++i) {
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->x2C, 3*i, 3*i);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->y2C, 3*i+1, 3*i+1);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->z2C, 3*i+2, 3*i+2);
		//insert inverse correspondences coefficients
// 		int invCorNaborNum = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor.size();
// 		for (k = 0; k < invCorNaborNum; ++k) {
// 			int ii = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor[k];
// 			float value = 0.5*m_CurrentMesh.m_ArrayVertex[i]->invCorValue[k]*disWeight;
// 			A.increase(value, 3*i, 3*ii);
// 			A.increase(value, 3*i+1, 3*ii+1);
// 			A.increase(value, 3*i+2, 3*ii+2);
// 			A.increase(value, 3*ii, 3*i);
// 			A.increase(value, 3*ii+1, 3*i+1);
// 			A.increase(value, 3*ii+2, 3*i+2);
// 		}
		b[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x1C;
		b[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y1C;
		b[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z1C;
		x[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x();
		x[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y();
		x[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z();
		
		// potential energy terms
		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
		vector<float> naborW = m_CurrentMesh.m_ArrayVertex[i]->m_NaborWeight;
		for (int j = 0; j < nabor.size(); ++j) {
		  int ii = nabor[j];
			float eW = naborW[j];
 			//edge based energy term (mixed quadratic terms)
 			A.insert(-eW, 3*i, 3*ii);
 			A.insert(-eW, 3*i+1, 3*ii+1);
			A.insert(-eW, 3*i+2, 3*ii+2);	
			A.insert(-eW, 3*ii, 3*i);
			A.insert(-eW, 3*ii+1, 3*i+1);
			A.insert(-eW, 3*ii+2, 3*i+2);	
// 		 	int prev = j - 1; 
// 		  if (j==0) prev = nabor.size()-1;
// 			int next = j + 1;
// 			if (j==nabor.size()-1) next = 0;
// 			int prevNabor = nabor[prev];
// 			int nextNabor = nabor[next];
// 			A.insert(-elasWeight, 3*prevNabor, 3*nextNabor);
// 			A.insert(-elasWeight, 3*prevNabor+1, 3*nextNabor+1);
// 			A.insert(-elasWeight, 3*prevNabor+2, 3*nextNabor+2);	
// 			A.insert(-elasWeight, 3*nextNabor, 3*prevNabor);
// 			A.insert(-elasWeight, 3*nextNabor+1, 3*prevNabor+1);
// 		  A.insert(-elasWeight, 3*nextNabor+2, 3*prevNabor+2);
		}
		
		// potential energy terms
// 		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
// 		int nn = nabor.size();
// 		float nn1 = 1.0/nn;
// 		for (int j = 0; j < nn; ++j) {
// 			int ii = nabor[j];
// 			//ring based energy term (mixed quadratic terms)
// 			A.increase(-elasWeight*nn1, 3*i, 3*ii);
// 			A.increase(-elasWeight*nn1, 3*i+1, 3*ii+1);
// 			A.increase(-elasWeight*nn1, 3*i+2, 3*ii+2);	
// 			A.increase(-elasWeight*nn1, 3*ii, 3*i);
// 			A.increase(-elasWeight*nn1, 3*ii+1, 3*i+1);
// 			A.increase(-elasWeight*nn1, 3*ii+2, 3*i+2);	
// 			for (int jj = j+1; jj < nn; ++jj) {
// 				int iii = nabor[jj];
// 				A.increase(elasWeight*nn1*nn1, 3*ii, 3*iii);
// 				A.increase(elasWeight*nn1*nn1, 3*ii+1, 3*iii+1);
// 				A.increase(elasWeight*nn1*nn1, 3*ii+2, 3*iii+2);	
// 				A.increase(elasWeight*nn1*nn1, 3*iii, 3*ii);
// 				A.increase(elasWeight*nn1*nn1, 3*iii+1, 3*ii+1);
// 				A.increase(elasWeight*nn1*nn1, 3*iii+2, 3*ii+2);
// 			}		
// 		}
	}
	//ofstream ost("xxx.txt"); 
	//A.print(ost);
	//send A, b & x into Conjugate Gradient
	x = ConjugateGradient(A, b, x, 101);
	//assign x to the current mesh
	for (i = 0; i < n; ++i) m_CurrentMesh.m_ArrayVertex[i]->Set(x[3*i], x[3*i+1], x[3*i+2]);

	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	haveCurrentMesh = true;
	avgAspectRatioCurrent = m_CurrentMesh.GetAvgAspectRatio();
	}
	DWORD dwEnd = ::GetTickCount();
	CString sTime;
	sTime.Format("%8.4f",(dwEnd-dwStart)*(1e-3));
  //AfxMessageBox(sTime);
	ComputeDis();
	glDraw();
}

/************************************************************************/
/* Area and cotangent weighted ring-based energy 10 times               */
/************************************************************************/
void CSceneGraph3d::NonRigidFittingWeighted10() {
  int size = m_ArrayObject3d.GetSize();
  if (size < 2) {AfxMessageBox("Invalid operation!"); return;}
  int times = 10;
	for (int iTimes=0; iTimes<times; ++iTimes) {
  if (!isGenSrc) {
    GenSrcMesh(); 
    isGenSrc = true;
  }
  if (!isGenTarget) {
    GenTargetMesh();
    isGenTarget = true;
  }
  m_Before.Copy(&m_CurrentMesh);
  fittingTimes++;
  m_CurrentMesh.BuildAdjacency();
  m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
  //for finding the inverse correspondence
	m_CurrentMesh.BuildAnnKDTree();
	int kCor = 50/pow(1.5, fittingTimes)+1;
	float disWeight = pow(1.1, fittingTimes);
	float elasWeight = 100;
	m_CurrentMesh.GenerateUniformSamples();
	//ComputeVolumeEnergy(disWeight, kCor);
	ComputeDistanceEnergy(disWeight, kCor);
  // KEY difference is here!!!!
	ComputeWeightedPotentialEnergy(elasWeight);
	//now, begin to fill in the matrix A
	int n = m_CurrentMesh.NbVertex();
	int i, j, k, ii, jj, iii, nn;
	//Coefficient matrix for optimization 
	Sparse_Matrix<float> A(n*3, n*3);
	//vector b and initial x
	Vector<float> b(n*3), x(n*3);
	//traverse vertices
	for (i = 0; i < n; ++i) {
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->x2C, 3*i, 3*i);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->y2C, 3*i+1, 3*i+1);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->z2C, 3*i+2, 3*i+2);
		//insert inverse correspondences coefficients
		int invCorNaborNum = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor.size();
		for (k = 0; k < invCorNaborNum; ++k) {
			int ii = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor[k];
			float value = 0.5*m_CurrentMesh.m_ArrayVertex[i]->invCorValue[k];
			A.increase(value, 3*i, 3*ii);
			A.increase(value, 3*i+1, 3*ii+1);
			A.increase(value, 3*i+2, 3*ii+2);
			A.increase(value, 3*ii, 3*i);
			A.increase(value, 3*ii+1, 3*i+1);
			A.increase(value, 3*ii+2, 3*i+2);
		}
		b[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x1C;
		b[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y1C;
		b[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z1C;
		x[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x();
		x[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y();
		x[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z();
		// area and cotangent weighted potential energy terms
		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
		nn = nabor.size();
    double area = m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround();
    double areaCur1 = 1.0/m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround();
    double areaSrc1 = 1.0/m_Src.m_ArrayVertex[i]->GetSumAreaAround();
    float elasW = elasWeight*area;
    double alpha, beta, cotA, cotB, cotSum = 0;
    double* cotArr = new double[nn];
    CVertex3d *pV, *nV, *cV;
    CVector3d *e1, *e2;
    for (j=0; j<nn; ++j) {
      ii = nabor[j];
      cV = m_CurrentMesh.m_ArrayVertex[ii];
      // find 2 faces that contain this nabor vertex
      CArray3d<CFace3d> fArray;
      m_CurrentMesh.m_ArrayVertex[i]->FindSharingFaces(cV,fArray);
      // previous and next nabor
      // traverse vertices of these 2 faces
      for (int iv=0; iv<3; ++iv) {
        if (fArray[0]->v(iv)!=cV && fArray[0]->v(iv)!=m_CurrentMesh.m_ArrayVertex[i]) 
          pV = fArray[0]->v(iv);
        if (fArray[1]->v(iv)!=cV && fArray[1]->v(iv)!=m_CurrentMesh.m_ArrayVertex[i]) 
          nV= fArray[1]->v(iv);
      }
      // find angle alpha
      e1 = new CVector3d(pV,m_CurrentMesh.m_ArrayVertex[i]); 
      e2 = new CVector3d(pV,cV);
      alpha = Angle(e1,e2);
      cotA = fabs(cos(alpha)/sin(alpha));
      // find angle beta
      e1 = new CVector3d(nV,m_CurrentMesh.m_ArrayVertex[i]); 
      e2 = new CVector3d(nV,cV);
      beta = Angle(e1,e2);
      cotB = fabs(cos(beta)/sin(beta));
      cotArr[j] = cotA+cotB;
      cotSum += cotArr[j];
    }
    for (j=0; j<nn; ++j) {
      ii = nabor[j];
			//ring based energy term (mixed quadratic terms)
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*i, 3*ii);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*i+1, 3*ii+1);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*i+2, 3*ii+2);	
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*ii, 3*i);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*ii+1, 3*i+1);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*ii+2, 3*i+2);	
			for (jj = j+1; jj < nn; ++jj) {
				iii = nabor[jj];
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*ii, 3*iii);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*ii+1, 3*iii+1);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*ii+2, 3*iii+2);	
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*iii, 3*ii);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*iii+1, 3*ii+1);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*iii+2, 3*ii+2);
			}		
		}
    cotArr=0; pV=0; nV=0; cV=0; e1=0; e2=0;
    delete cotArr; delete pV; delete nV; delete cV; delete e1; delete e2;
	}
	//ofstream ost("xxx.txt"); 
	//A.print(ost);
	//send A, b & x into Conjugate Gradient
	x = ConjugateGradient(A, b, x, 101);
	//assign x to the current mesh
	for (i=0; i<n; ++i) m_CurrentMesh.m_ArrayVertex[i]->Set(x[3*i], x[3*i+1], x[3*i+2]);
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	haveCurrentMesh = true;}
  ComputeDis();
	glDraw();
}


/************************************************************************/
/* Area and cotangent weighted ring-based energy                        */
/************************************************************************/
void CSceneGraph3d::NonRigidFittingWeighted() {
  int size = m_ArrayObject3d.GetSize();
  if (size < 2) {AfxMessageBox("Invalid operation!"); return;}
  int times = 1;
	for (int iTimes=0; iTimes<times; ++iTimes) {
  if (!isGenSrc) {
    GenSrcMesh(); 
    isGenSrc = true;
  }
  if (!isGenTarget) {
    GenTargetMesh();
    isGenTarget = true;
  }
  m_Before.Copy(&m_CurrentMesh);
  fittingTimes++;
  m_CurrentMesh.BuildAdjacency();
  m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
  //for finding the inverse correspondence
	m_CurrentMesh.BuildAnnKDTree();
	int kCor = 50/pow(1.5, fittingTimes)+1;
	float disWeight = pow(1.1, fittingTimes);
	float elasWeight = 100;
	m_CurrentMesh.GenerateUniformSamples();
	//ComputeVolumeEnergy(disWeight, kCor);
	ComputeDistanceEnergy(disWeight, kCor);
  // KEY difference is here!!!!
	ComputeWeightedPotentialEnergy(elasWeight);
  //ComputeEdgeBasedGeomEnergy(elasWeight);
	//now, begin to fill in the matrix A
	int n = m_CurrentMesh.NbVertex();
	int i, k, ii, jj, iii, nn;
	//Coefficient matrix for optimization 
	Sparse_Matrix<float> A(n*3, n*3);
	//vector b and initial x
	Vector<float> b(n*3), x(n*3);
	//traverse vertices
	for (i=0; i<n; ++i) {
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->x2C, 3*i, 3*i);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->y2C, 3*i+1, 3*i+1);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->z2C, 3*i+2, 3*i+2);
		//insert inverse correspondences coefficients
		int invCorNaborNum = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor.size();
    float k1 = 1.0f/invCorNaborNum;
		for (k = 0; k < invCorNaborNum; ++k) {
			int ii = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor[k];
			float value = 0.5*m_CurrentMesh.m_ArrayVertex[i]->invCorValue[k]*k1;
			A.increase(value, 3*i, 3*ii);
			A.increase(value, 3*i+1, 3*ii+1);
			A.increase(value, 3*i+2, 3*ii+2);
			A.increase(value, 3*ii, 3*i);
			A.increase(value, 3*ii+1, 3*i+1);
			A.increase(value, 3*ii+2, 3*i+2);
		}
		b[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x1C;
		b[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y1C;
		b[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z1C;
		x[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x();
		x[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y();
		x[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z();
    // edge based
    vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
    vector<float> naborW = m_CurrentMesh.m_ArrayVertex[i]->m_NaborWeight;
    /*for (int j = 0; j < nabor.size(); ++j) {
      int ii = nabor[j];
      float eW = naborW[j];
      //edge based energy term (mixed quadratic terms)
      A.insert(-eW, 3*i, 3*ii);
      A.insert(-eW, 3*i+1, 3*ii+1);
      A.insert(-eW, 3*i+2, 3*ii+2);	
      A.insert(-eW, 3*ii, 3*i);
      A.insert(-eW, 3*ii+1, 3*i+1);
      A.insert(-eW, 3*ii+2, 3*i+2);	
    }*/
		// area and cotangent weighted potential energy terms
		nn = nabor.size();
    double area = m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround();
    double areaCur1 = 1.0/m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround();
    double areaSrc1 = 1.0/m_Src.m_ArrayVertex[i]->GetSumAreaAround();
    float elasW = elasWeight*area;
    double alpha, beta, cotA, cotB, cotSum = 0;
    double* cotArr = new double[nn];
    CVertex3d *pV, *nV, *cV;
    CVector3d *e1, *e2;
    for (int j=0; j<nn; ++j) {
      ii = nabor[j];
      cV = m_CurrentMesh.m_ArrayVertex[ii];
      // find 2 faces that contain this nabor vertex
      CArray3d<CFace3d> fArray;
      m_CurrentMesh.m_ArrayVertex[i]->FindSharingFaces(cV,fArray);
      // previous and next nabor
      // traverse vertices of these 2 faces
      for (int iv=0; iv<3; ++iv) {
        if (fArray[0]->v(iv)!=cV && fArray[0]->v(iv)!=m_CurrentMesh.m_ArrayVertex[i]) 
          pV = fArray[0]->v(iv);
        if (fArray[1]->v(iv)!=cV && fArray[1]->v(iv)!=m_CurrentMesh.m_ArrayVertex[i]) 
          nV = fArray[1]->v(iv);
      }
      // find angle alpha
      e1 = new CVector3d(pV,m_CurrentMesh.m_ArrayVertex[i]); 
      e2 = new CVector3d(pV,cV);
      alpha = Angle(e1,e2);
      cotA = fabs(cos(alpha)/sin(alpha));
      // find angle beta
      e1 = new CVector3d(nV,m_CurrentMesh.m_ArrayVertex[i]); 
      e2 = new CVector3d(nV,cV);
      beta = Angle(e1,e2);
      cotB = fabs(cos(beta)/sin(beta));
      cotArr[j] = cotA+cotB;
      cotSum += cotArr[j];
    }
    for (j=0; j<nn; ++j) {
      ii = nabor[j];
			//ring based energy term (mixed quadratic terms)
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*i, 3*ii);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*i+1, 3*ii+1);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*i+2, 3*ii+2);	
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*ii, 3*i);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*ii+1, 3*i+1);
			A.increase(-elasW*cotArr[j]*cotSum*areaCur1*areaCur1, 3*ii+2, 3*i+2);	
			for (jj = j+1; jj < nn; ++jj) {
				iii = nabor[jj];
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*ii, 3*iii);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*ii+1, 3*iii+1);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*ii+2, 3*iii+2);	
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*iii, 3*ii);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*iii+1, 3*ii+1);
				A.increase(elasW*cotArr[j]*cotArr[jj]*areaCur1*areaCur1, 3*iii+2, 3*ii+2);
			}		
		}
    cotArr=0; pV=0; nV=0; cV=0; e1=0; e2=0;
    delete cotArr; delete pV; delete nV; delete cV; delete e1; delete e2;
	}
	//ofstream ost("xxx.txt"); 
	//A.print(ost);
	//send A, b & x into Conjugate Gradient
	x = ConjugateGradient(A, b, x, 101);
	//assign x to the current mesh
	for (i=0; i<n; ++i) m_CurrentMesh.m_ArrayVertex[i]->Set(x[3*i], x[3*i+1], x[3*i+2]);
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	haveCurrentMesh = true;
  }
	ComputeDis();
	glDraw();
}

void CSceneGraph3d::NonRigidFitting() {
	int size = m_ArrayObject3d.GetSize();
	if (size < 2) {AfxMessageBox("Invalid operation!"); return;}

	if (!isGenSrc) {
		GenSrcMesh(); 
		isGenSrc = true;
	}
	if (!isGenTarget) {
		GenTargetMesh();
		isGenTarget = true;
	}
	m_Before.Copy(&m_CurrentMesh);
	fittingTimes++;
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	//for finding the inverse correspondence
	m_CurrentMesh.BuildAnnKDTree();
	int kCor = 50/pow(1.5, fittingTimes)+1;
	float disWeight = pow(1.1, fittingTimes);
	float elasWeight = 100;
	m_CurrentMesh.GenerateUniformSamples();
	//ComputeVolumeEnergy(disWeight, kCor);
	ComputeDistanceEnergy(disWeight, kCor);
	ComputeElasticPotentialEnergy(elasWeight);
	//now, begin to fill in the matrix A
	int n = m_CurrentMesh.NbVertex();
	int i, j, k, ii, jj, iii, nn;
	//Coefficient matrix for optimization 
	Sparse_Matrix<float> A(n*3, n*3);
	//vector b and initial x
	Vector<float> b(n*3), x(n*3);
	//traverse vertices
	for (i = 0; i < n; ++i) {
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->x2C, 3*i, 3*i);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->y2C, 3*i+1, 3*i+1);
		A.increase(m_CurrentMesh.m_ArrayVertex[i]->z2C, 3*i+2, 3*i+2);
		//insert inverse correspondences coefficients
		int invCorNaborNum = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor.size();
		for (k = 0; k < invCorNaborNum; ++k) {
			int ii = m_CurrentMesh.m_ArrayVertex[i]->invCorNabor[k];
			float value = 0.5*m_CurrentMesh.m_ArrayVertex[i]->invCorValue[k]*disWeight;
			A.increase(value, 3*i, 3*ii);
			A.increase(value, 3*i+1, 3*ii+1);
			A.increase(value, 3*i+2, 3*ii+2);
			A.increase(value, 3*ii, 3*i);
			A.increase(value, 3*ii+1, 3*i+1);
			A.increase(value, 3*ii+2, 3*i+2);
		}
		b[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x1C;
		b[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y1C;
		b[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z1C;
		x[3*i] = m_CurrentMesh.m_ArrayVertex[i]->x();
		x[3*i+1] = m_CurrentMesh.m_ArrayVertex[i]->y();
		x[3*i+2] = m_CurrentMesh.m_ArrayVertex[i]->z();

		// potential energy terms
// 		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
// 		for (int j = 0; j < nabor.size(); ++j) {
// 			int ii = nabor[j];
// 			//edge based energy term (mixed quadratic terms)
// 			A.insert(-elasWeight, 3*i, 3*ii);
// 			A.insert(-elasWeight, 3*i+1, 3*ii+1);
// 			A.insert(-elasWeight, 3*i+2, 3*ii+2);	
// 			A.insert(-elasWeight, 3*ii, 3*i);
// 			A.insert(-elasWeight, 3*ii+1, 3*i+1);
// 			A.insert(-elasWeight, 3*ii+2, 3*i+2);	
			//internal edges
// 			int prev = j - 1; 
// 			if (j==0) prev = nabor.size()-1;
// 			int next = j + 1;
// 			if (j==nabor.size()-1) next = 0;
// 			int prevNabor = nabor[prev];
// 			int nextNabor = nabor[next];
// 			A.insert(-elasWeight, 3*prevNabor, 3*nextNabor);
// 			A.insert(-elasWeight, 3*prevNabor+1, 3*nextNabor+1);
// 			A.insert(-elasWeight, 3*prevNabor+2, 3*nextNabor+2);	
// 			A.insert(-elasWeight, 3*nextNabor, 3*prevNabor);
// 			A.insert(-elasWeight, 3*nextNabor+1, 3*prevNabor+1);
// 			A.insert(-elasWeight, 3*nextNabor+2, 3*prevNabor+2);
// 		}

		// potential energy terms
		vector<int> nabor = m_CurrentMesh.m_ArrayVertex[i]->m_NaborIndex;
		nn = nabor.size();
		float nn1 = 1.0/nn;
    float elasW = (100/m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround())*elasWeight;
    //float elasW = m_CurrentMesh.m_ArrayVertex[i]->GetSumAreaAround()*elasWeight;
		for (j = 0; j < nn; ++j) {
			ii = nabor[j];
			//ring based energy term (mixed quadratic terms)
			A.increase(-elasW*nn1, 3*i, 3*ii);
			A.increase(-elasW*nn1, 3*i+1, 3*ii+1);
			A.increase(-elasW*nn1, 3*i+2, 3*ii+2);	
			A.increase(-elasW*nn1, 3*ii, 3*i);
			A.increase(-elasW*nn1, 3*ii+1, 3*i+1);
			A.increase(-elasW*nn1, 3*ii+2, 3*i+2);	
			for (jj = j+1; jj < nn; ++jj) {
				iii = nabor[jj];
				A.increase(elasW*nn1*nn1, 3*ii, 3*iii);
				A.increase(elasW*nn1*nn1, 3*ii+1, 3*iii+1);
				A.increase(elasW*nn1*nn1, 3*ii+2, 3*iii+2);	
				A.increase(elasW*nn1*nn1, 3*iii, 3*ii);
				A.increase(elasW*nn1*nn1, 3*iii+1, 3*ii+1);
				A.increase(elasW*nn1*nn1, 3*iii+2, 3*ii+2);
			}		
		}
	}
	//ofstream ost("xxx.txt"); 
	//A.print(ost);
	//send A, b & x into Conjugate Gradient
	x = ConjugateGradient(A, b, x, 101);
	//assign x to the current mesh
	for (i=0; i<n; ++i) m_CurrentMesh.m_ArrayVertex[i]->Set(x[3*i], x[3*i+1], x[3*i+2]);
	m_CurrentMesh.BuildAdjacency();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
	m_CurrentMesh.SetColorBinding(COLOR_PER_VERTEX);
	m_CurrentMesh.SetNormalBinding(NORMAL_PER_VERTEX);
	m_CurrentMesh.SetColor(127, 0, 127);
	m_CurrentMesh.SetModified(1);
	haveCurrentMesh = true;
	ComputeDis();
	glDraw();
}

void CSceneGraph3d::ComputeVolumeEnergy(float disWeight, int k) {
	//three meshes: source, target, current
	//the distance energy is for the current and target mesh
	//CObject3d *pTargetObject = m_ArrayObject3d[1]; //target mesh
	//((CMesh3d*)pTargetObject)->CalculateNormalPerFace();
	//((CMesh3d*)pTargetObject)->CalculateNormalPerVertex();
	m_CurrentMesh.ComputeVolEnergy(&m_Tar/*(CMesh3d*)pTargetObject*/, disWeight, k);
	//pTargetObject=0; delete pTargetObject;
}

void CSceneGraph3d::ComputeSingleDistanceEnergy(float disWeight) {
  m_CurrentMesh.ComputePaulyDisEnergy(&m_Tar, disWeight);
}

void CSceneGraph3d::ComputePaulyMultDistanceEnergy(float disWeight, int k) {
	m_CurrentMesh.ComputePaulyMultDisEnergy(&m_Tar, disWeight, k);
}

void CSceneGraph3d::ComputeDistanceEnergy(float disWeight, int k) {
	//three meshes: source, target, current
	//the distance energy is for the current and target mesh
	//CObject3d *pTargetObject = m_ArrayObject3d[1]; //target mesh
	//((CMesh3d*)pTargetObject)->CalculateNormalPerFace();
	//((CMesh3d*)pTargetObject)->CalculateNormalPerVertex();
	m_CurrentMesh.ComputeDisEnergy(&m_Tar/*(CMesh3d*)pTargetObject*/, disWeight, k);
	//pTargetObject=0; delete pTargetObject;
}

void CSceneGraph3d::ComputeElasticPotentialEnergy(float elasWeight) {
	//three meshes: source, target, current
	//the elastic (geometry) energy is for the current and former mesh
	m_CurrentMesh.GenerateEdgeSet();
	m_Src.GenerateEdgeSet();
	//The vertex coordinates stored in m_CurrentMesh is obtained by former fitting
	m_CurrentMesh.ComputeElasEnergy(&m_Src, elasWeight); 
}

void CSceneGraph3d::ComputeWeightedPotentialEnergy(float elasWeight) {
  m_CurrentMesh.GenerateEdgeSet();
  m_Src.GenerateEdgeSet();
	m_CurrentMesh.ComputeWeightedRingEnergy(&m_Src, &m_CurrentMesh, elasWeight); 
}

void CSceneGraph3d::ComputeEdgeBasedGeomEnergy(float elasWeight) {
  m_CurrentMesh.GenerateEdgeSet();
	m_Src.GenerateEdgeSet();
	//The vertex coordinates stored in m_CurrentMesh is obtained by former fitting
	m_CurrentMesh.ComputeEdgeBasedGeomEnergy(&m_Src, elasWeight); 
}

void CSceneGraph3d::ColorCurvatue() {
	int size = m_ArrayObject3d.GetSize();
	CColorRamp ramp;
	ramp.BuildRainbow();
	for (int i = 0; i < size; ++i) {
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D) ((CMesh3d *)pObject)->ColorCurvature(&ramp);
		pObject=0; delete pObject;
	}
}

void CSceneGraph3d::ShowSource() {
	if (m_ArrayObject3d.GetSize() < 1) {
		AfxMessageBox("Invalid operation");
		return;
	}
	CObject3d *pObject = m_ArrayObject3d[0];
	((CMesh3d *)pObject)->m_Show = 1 - ((CMesh3d *)pObject)->m_Show;
	pObject=0; delete pObject;
}

void CSceneGraph3d::ShowTarget() {
	if (m_ArrayObject3d.GetSize() < 2) {
		AfxMessageBox("Invalid operation");
		return;
	}
	CObject3d *pObject = m_ArrayObject3d[1];
	((CMesh3d *)pObject)->m_Show = 1 - ((CMesh3d *)pObject)->m_Show;
	pObject=0; delete pObject;
}

void CSceneGraph3d::ShowCurrent() {
  if (haveCurrentMesh) {
		m_CurrentMesh.m_Show = 1 - m_CurrentMesh.m_Show;
	}
}

void CSceneGraph3d::ShowMesh(int i) {
  if (m_ArrayObject3d.GetSize() < i) {
    AfxMessageBox("Invalid operation");
    return;
  }
  CObject3d *pObject = m_ArrayObject3d[i-1];
  ((CMesh3d *)pObject)->m_Show = 1 - ((CMesh3d *)pObject)->m_Show;
	pObject=0; delete pObject;
}

void CSceneGraph3d::FlipNormal()
{
	normalFlip = -normalFlip;
	if (m_ArrayObject3d.GetSize() < 1) {
		AfxMessageBox("Invalid operation");
		return;
	}
	for (int i = 0; i < m_ArrayObject3d.GetSize(); ++i) {
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D) {
			((CMesh3d *)pObject)->normalFlip = normalFlip;
			((CMesh3d *)pObject)->CalculateNormalPerFace();
			((CMesh3d *)pObject)->CalculateNormalPerVertex();
		}
		pObject=0; delete pObject;
	}
	m_CurrentMesh.normalFlip = normalFlip;
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
}

void CSceneGraph3d::OrientFaces() {
  if (m_ArrayObject3d.GetSize() < 1) {
		AfxMessageBox("Invalid operation");
		return;
	}
	for (int i = 0; i < m_ArrayObject3d.GetSize(); ++i) {
		CObject3d *pObject = m_ArrayObject3d[i];
		if(pObject->GetType() == TYPE_MESH3D) {
			((CMesh3d *)pObject)->OrientFaces();
			((CMesh3d *)pObject)->CalculateNormalPerFace();
			((CMesh3d *)pObject)->CalculateNormalPerVertex();
		}
		pObject=0; delete pObject;
	}
  if (isGenSrc) {
	m_Src.OrientFaces();
	m_Src.CalculateNormalPerFace();
	m_Src.CalculateNormalPerVertex();
  }
  if (isGenTarget) {
	m_Tar.OrientFaces();
	m_Tar.CalculateNormalPerFace();
	m_Tar.CalculateNormalPerVertex();
  }
  if (haveCurrentMesh) {
	m_CurrentMesh.OrientFaces();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
  }
}

void CSceneGraph3d::ReorientSource() {
  if (m_ArrayObject3d.GetSize() < 1) {
		AfxMessageBox("Invalid operation");
		return;
	}
	CObject3d *pObject = m_ArrayObject3d[0];
	if(pObject->GetType() == TYPE_MESH3D) {
	  ((CMesh3d *)pObject)->ReorientFaces();
		((CMesh3d *)pObject)->CalculateNormalPerFace();
		((CMesh3d *)pObject)->CalculateNormalPerVertex();
	}
	pObject=0; delete pObject;
	if (isGenSrc) {
	  m_Src.ReorientFaces();
	  m_Src.CalculateNormalPerFace();
	  m_Src.CalculateNormalPerVertex();
	}
}

void CSceneGraph3d::ReorientTarget() {
  if (m_ArrayObject3d.GetSize() < 2) {
		AfxMessageBox("Invalid operation");
		return;
	}
	CObject3d *pObject = m_ArrayObject3d[1];
	if(pObject->GetType() == TYPE_MESH3D) {
	  ((CMesh3d *)pObject)->ReorientFaces();
		((CMesh3d *)pObject)->CalculateNormalPerFace();
		((CMesh3d *)pObject)->CalculateNormalPerVertex();
	}
	pObject=0; delete pObject;
	if (isGenTarget) {
		m_Tar.ReorientFaces();
		m_Tar.CalculateNormalPerFace();
		m_Tar.CalculateNormalPerVertex();
	}
}

void CSceneGraph3d::ReorientCurrent() {
	m_CurrentMesh.ReorientFaces();
	m_CurrentMesh.CalculateNormalPerFace();
	m_CurrentMesh.CalculateNormalPerVertex();
}

void CSceneGraph3d::ReorientAll() {
  if (m_ArrayObject3d.GetSize() < 1) {
    AfxMessageBox("Invalid operation");
    return;
  }
  for (int i = 0; i < m_ArrayObject3d.GetSize(); ++i) {
    CObject3d *pObject = m_ArrayObject3d[i];
    if(pObject->GetType() == TYPE_MESH3D) {
      ((CMesh3d *)pObject)->ReorientFaces();
      ((CMesh3d *)pObject)->CalculateNormalPerFace();
      ((CMesh3d *)pObject)->CalculateNormalPerVertex();
    }
    pObject=0; delete pObject;
	}
}

void CSceneGraph3d::ComputeDis() {
	if (m_ArrayObject3d.GetSize() < 2) {
		AfxMessageBox("Invalid operation");
		return;
	}
  //Compute the distance between the target mesh and the current mesh.
	double* dists = new double[3];
	((CMesh3d *)m_ArrayObject3d[1])->ComputeDis(dists, &m_CurrentMesh);
	dis = dists[0];
	disL2 = dists[1];
	disLM = dists[2];
	dists=0; delete dists;
}

bool CSceneGraph3d::ComputeDisAll() {
	if (m_ArrayObject3d.GetSize()<2) {
		AfxMessageBox("Invalid operation");
		return false;
	}
// 	if (fittingTimes<1) {
// 		AfxMessageBox("Sorry! You must fit once at least!");
// 		return false;
// 	}
  //Compute the distance between the target mesh and the current mesh.
	CMesh3d *pTarMesh = (CMesh3d *)m_ArrayObject3d[1];
	//dis = pTarMesh->ComputeDis(m_CurrentMesh)[0];
	//Compute the distance between the target mesh and the sorce mesh.
	double* dists = new double[3];
	pTarMesh->ComputeDis(dists, (CMesh3d*)m_ArrayObject3d[0]);
	disSTL1 = dists[0]; disSTL2 = dists[1]; disSTLM = dists[2];
	pTarMesh=0; delete pTarMesh;
	dists=0; delete dists;
	return true;
}

void CSceneGraph3d::AlignScale() {
  if (m_ArrayObject3d.GetSize()<2) {AfxMessageBox("Invalid operation"); return;}
	CMesh3d *pSrcMesh = (CMesh3d*)m_ArrayObject3d[0];
	CMesh3d *pTarMesh = (CMesh3d*)m_ArrayObject3d[1];
	m_OriSrc.BuildAdjacency();
	m_OriTar.BuildAdjacency();
  float boxA=m_OriSrc.maxbBoxSize;
	float boxB=m_OriTar.maxbBoxSize;
	float ratio;
	bool needSrcScale;
	//if (boxA>boxB) {
	//	needSrcScale=false;
  //  ratio=boxA/boxB;
	//}
	//else 
	{
		needSrcScale=true;
    ratio=boxB/boxA;
	}
  //Scale 
	if (needSrcScale) {
	  for (int i=0; i<m_OriSrc.NbVertex(); ++i) 
		  m_OriSrc.m_ArrayVertex[i]->Set(m_OriSrc.m_ArrayVertex[i]->x()*ratio, 
																		 m_OriSrc.m_ArrayVertex[i]->y()*ratio, 
																		 m_OriSrc.m_ArrayVertex[i]->z()*ratio);
	  m_OriSrc.SetModified();
		applyMatrices();
	}
	else {
    for (int i=0; i<pTarMesh->NbVertex(); ++i) 
			((CMesh3d *)m_ArrayObject3d[1])->m_ArrayVertex[i]->Set(pTarMesh->m_ArrayVertex[i]->x()*ratio, 
																			pTarMesh->m_ArrayVertex[i]->y()*ratio, 
																			pTarMesh->m_ArrayVertex[i]->z()*ratio);
	  pTarMesh->SetModified();
	}
	pSrcMesh=0; pTarMesh=0;
	delete pSrcMesh; delete pTarMesh;
}

void CSceneGraph3d::applyMatrices() {
	CMesh3d* pMesh;
	if (NbObject()>0) {
		CString s1;
		//pMesh = (CMesh3d*)m_ArrayObject3d[0];
		//s1.Format("%8,4f", pMesh->m_ArrayVertex[0]->x());
		((CMesh3d *)m_ArrayObject3d[0])->MultMat(&m_OriSrc, modelviewObject[0]);
		((CMesh3d *)m_ArrayObject3d[0])->SetModified();
		CString s2;
		//s2.Format("%8,4f", pMesh->m_ArrayVertex[0]->x());
		//AfxMessageBox(s1+"   "+s2);
	}
	if (NbObject()>1) {
		((CMesh3d*)m_ArrayObject3d[1])->MultMat(&m_OriTar, modelviewObject[1]);
	  ((CMesh3d*)m_ArrayObject3d[1])->SetModified();
	}
	pMesh = 0; delete pMesh;
}

void CSceneGraph3d::PrepareOriMesh(){
  if (NbObject()>0) m_OriSrc.Copy((CMesh3d*)m_ArrayObject3d[0]);
  if (NbObject()>1) m_OriTar.Copy((CMesh3d*)m_ArrayObject3d[1]);
}

/************************************************************************/
/* Output Current mesh                                                  */
/************************************************************************/
void CSceneGraph3d::OnOutputCoord() {
  m_CurrentMesh.output3();
}

/************************************************************************/
/* Output Target mesh                                                   */
/************************************************************************/
void CSceneGraph3d::OnOutputTarCoord() {
  ((CMesh3d *)m_ArrayObject3d[1])->output3();
}

/************************************************************************/
/* Output Source mesh                                                   */
/************************************************************************/
void CSceneGraph3d::OnOutputSrcCoord() {
  ((CMesh3d *)m_ArrayObject3d[0])->output3();
}

/************************************************************************/
/* Mirror the surface.                                                  */
/* 0 --- source, 1 --- target, 2 --- all                                */
/************************************************************************/
void CSceneGraph3d::onMirror(int manip) {
  if (manip==0) {m_OriSrc.mirror();}
  if (manip==1) {m_OriTar.mirror();}
  if (manip==2) {m_OriSrc.mirror(); m_OriTar.mirror();}
}

int CSceneGraph3d::countObject() {
  return m_ArrayObject3d.GetSize();
}

void CSceneGraph3d::GenSrcMesh() {
  if (m_ArrayObject3d.GetSize()<1) return;
  m_Src.Copy((CMesh3d*)m_ArrayObject3d[0]);
	m_Src.BuildAdjacency();
	m_Src.CalculateNormalPerFace();
	m_Src.CalculateNormalPerVertex();
	m_Src.GenerateUniformSamples();
	m_Src.BuildAnnKDTree();
	avgAspectRatioSource = m_Src.GetAvgAspectRatio();
	m_CurrentMesh.Copy(&m_Src); 
	isGenSrc = true;
}

void CSceneGraph3d::GenTargetMesh() {
	if (m_ArrayObject3d.GetSize()<2) return;
	m_Tar.Copy((CMesh3d*)m_ArrayObject3d[1]);
	m_Tar.BuildAdjacency();
	m_Tar.CalculateNormalPerFace();
	m_Tar.CalculateNormalPerVertex();
	m_Tar.GenerateUniformSamples();
	m_Tar.BuildAnnKDTree();
	avgAspectRatioTarget = m_Tar.GetAvgAspectRatio();
	isGenTarget = true;
}

void CSceneGraph3d::ClearLandmark(int i) {
	CMesh3d* m = (CMesh3d*)m_ArrayObject3d[i];
	m->m_ArrayLandmark.RemoveAll();
	m->m_ArrayLandmarkInd.clear();
	m->SetModified(1);
	m=0; delete m;
	glDraw();
}

void CSceneGraph3d::AddLandmark(int ind, int i) {
	CMesh3d* m = (CMesh3d*)m_ArrayObject3d[i];
	m->m_ArrayLandmark.Add(m->m_ArrayVertex[ind]);
	m->m_ArrayLandmarkInd.push_back(ind);
	m->SetModified(1);
	m=0; delete m;
	glDraw();
}

void CSceneGraph3d::AddLandmark(float x, float y, float z, int i) {
  ANNdistArray dists = new ANNdist[1];	
  ANNidxArray idx = new ANNidx[1];
  ANNpoint pTar = annAllocPt(3);
  CMesh3d* m = (CMesh3d*)m_ArrayObject3d[i];
  pTar[0] = x; pTar[1] = y; pTar[2] = z;
  m->pVert_Tree3->annkSearch(pTar, 1, idx, dists, 0);
  m->m_ArrayLandmark.Add(m->m_ArrayVertex[idx[0]]);
  m->m_ArrayLandmarkInd.push_back(idx[0]);
  dists=0; idx=0;
  delete dists; delete idx;
  annDeallocPt(pTar);
  m->SetModified(1);
  m=0; delete m;
  glDraw();
}

void CSceneGraph3d::ComputePairDis() {
  int n = ((CMesh3d*)m_ArrayObject3d[0])->NbVertex();
  if (n!=((CMesh3d*)m_ArrayObject3d[1])->NbVertex()) {
    AfxMessageBox("No same vertex number!");
    return;
  }
  double disM=0, disSum=0, dis;
  for (int i=0; i<n; ++i) {
    dis = ((CMesh3d*)m_ArrayObject3d[0])->m_ArrayVertex[i]->
      dist(((CMesh3d*)m_ArrayObject3d[1])->m_ArrayVertex[i]);
    if (disM<dis) disM = dis;
    disSum += dis;
  }
  disPr2 = sqrt(disSum/n); disPrM = sqrt(disM);
}

void CSceneGraph3d::ComputeMarkDis() {
  CArray3d<CVertex3d> lmS = ((CMesh3d*)m_ArrayObject3d[0])->m_ArrayLandmark;
  CArray3d<CVertex3d> lmT = ((CMesh3d*)m_ArrayObject3d[1])->m_ArrayLandmark;
  int n = lmS.GetSize();
  double dis;
  for (int i=0; i<n; ++i) {
	  dis = lmS.GetAt(i)->dist(lmT.GetAt(i));
	  disMarks.push_back(dis);
  }
}

void CSceneGraph3d::DelLandmark(int i) {
  CMesh3d* m = (CMesh3d*)m_ArrayObject3d[i];
  m=0; delete m;
}

void CSceneGraph3d::showLandmarks() {
	if (isShowLandmarks) {
    if (m_ArrayObject3d.GetSize()>0) {
	  ((CMesh3d*)m_ArrayObject3d[0])->isShowLandmarks=true;
      ((CMesh3d*)m_ArrayObject3d[0])->SetModified(1);
		}
    if (isGenTarget) ((CMesh3d*)m_ArrayObject3d[1])->isShowLandmarks=true;
	}
	else {
    if (m_ArrayObject3d.GetSize()>1) {
			((CMesh3d*)m_ArrayObject3d[0])->isShowLandmarks=false;
			((CMesh3d*)m_ArrayObject3d[0])->SetModified(1);
		}
		if (isGenTarget) ((CMesh3d*)m_ArrayObject3d[1])->isShowLandmarks=false;
	}
	glDraw();
}

void CSceneGraph3d::OnOutputSrcLandmarks() {
	CString name = ((CMesh3d *)m_ArrayObject3d[0])->GetName();
  ((CMesh3d *)m_ArrayObject3d[0])->outputLandmarks(0,name+".lm");
}

void CSceneGraph3d::OnOutputTargetLandmarks() {
	CString name = ((CMesh3d *)m_ArrayObject3d[1])->GetName();
  ((CMesh3d *)m_ArrayObject3d[1])->outputLandmarks(1,name+".lm");
}

void CSceneGraph3d::setMeshName(int srctar, CString name) {
  ((CMesh3d*)m_ArrayObject3d[srctar])->SetName(name);
}

void CSceneGraph3d::BuildCorrespondences(CMesh3d *src, CMesh3d *tgt, int k) {
  //src->BuildCorrespondences(tgt, k);
  src->BuildConsistCorrespondences(tgt, k);
}

void CSceneGraph3d::BuildInverseCorrespondences(CMesh3d *src, CMesh3d *tgt, int k) {
  src->BuildInverseCorrespondences(tgt, k);
}

void CSceneGraph3d::BuildConsistCorrespondences(CMesh3d *src, CMesh3d *tgt, int k) {
  src->BuildConsistCorrespondences(tgt, k);	
}

void CSceneGraph3d::OnBend(int manip) {
  if (manip==0) {m_OriSrc.bend();}
  if (manip==1) {m_OriTar.bend();}
  if (manip==2) {m_OriSrc.bend(); m_OriTar.bend();}
}

void CSceneGraph3d::showTex() {
  int size = m_ArrayObject3d.GetSize();
  for (int i=0; i<size; ++i) {
    ((CMesh3d *)m_ArrayObject3d[i])->showTex();
    if (!((CMesh3d *)m_ArrayObject3d[i])->isSetCoordMesh) {
      ((CMesh3d *)m_ArrayObject3d[i])->setTexCoord(((CMesh3d *)m_ArrayObject3d[0]));
      ((CMesh3d *)m_ArrayObject3d[i])->isSetCoordMesh = true;
    }
  }
}

void CSceneGraph3d::OutputSrcCenters() {
  if (NbObject()<1) return;
  ((CMesh3d *)m_ArrayObject3d[0])->outputCenters();
}


// ** EOF **

