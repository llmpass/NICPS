//********************************************
// SceneGraph.h
// class CSceneGraph
//********************************************
// alliez@usc.edu
// Created : 15/01/98
// Modified : 26/05/98
//********************************************

#ifndef _SCENEGRAPH_3D_
#define _SCENEGRAPH_3D_

#include "Object3d.h"
#include "Texture.h"
#include "Material.h"
#include "Array3d.h"

#include "..\SparseMatrix.h"

class CSceneGraph3d
{
private :
	// Datas
	CArray3d<CObject3d> m_ArrayObject3d;
	CArray3d<CTexture>  m_ArrayTexture;
	CArray3d<CMaterial> m_ArrayMaterial;
	GLuint *m_pIndexTextureBinding;
	unsigned int m_ListDone;
	bool isShowCorrespondences, hasBuiltCorrespondences;

public :
	void NonRigidFittingWeighted10();
	void ComputeWeightedPotentialEnergy(float);
	void NonRigidFittingWeighted();
	void OutputSrcCenters();
	void showTex();
	void OnBend(int);
	void ComputePairDis();
	void ShowMesh(int);
	void ReorientAll();
	void BuildConsistCorrespondences(CMesh3d *src, CMesh3d *tgt, int k);
	void BuildInverseCorrespondences(CMesh3d *src, CMesh3d *tgt, int k);
	void BuildCorrespondences(CMesh3d* src, CMesh3d* tgt,  int k);
	void ComputeMarkDis();
	void setMeshName(int, CString);
	void ClearLandmark(int i);
	void AddLandmark(int ind, int i);
	void DelLandmark(int);
	void OnOutputTargetLandmarks();
	void OnOutputSrcLandmarks();
	void showLandmarks();
	void AddLandmark(float, float, float, int);
	void GenSrcMesh();
	void GenTargetMesh();
	int countObject();
	bool isAddingLandmarks, isShowLandmarks, isGenSrc, isGenTarget;
	void Pauly2005FittingMult();
	void Pauly2005Fitting();
	void onMirror(int);
	void OnOutputSrcCoord();
	void OnOutputCoord();
	void OnOutputTarCoord();
	void PrepareOriMesh();
	void applyMatrices();
	mat4x4<GLfloat> rotM[2], rotMAll, rotMCurrent;
	float modelviewObject[2][16],modelviewCurrent[16];
	void AlignScale();
	void ComputeDis();
	bool ComputeDisAll();
	double dis, disL2, disLM;
	double disSTL1, disSTL2, disSTLM;
  double disPrM, disPr2;
	vector<double> disMarks;
	float avgAspectRatioCurrent, avgAspectRatioSource, avgAspectRatioTarget;
	float nearV1[3], nearV2[3];
	void ReorientCurrent();
	void ReorientTarget();
	void ReorientSource();
	void OrientFaces();
	int normalFlip;
	void FlipNormal();
	void NonRigidFittingM();
	void ShowSource();
	void ShowTarget();
	void ShowCurrent();
	float ComputAveNearestDis(CMesh3d* src, CMesh3d* des);
	bool haveCurrentMesh;
	bool isShowNearestPoints;
	int corNo, SrcOrTgtCor;

	//Alignment procedures
	CMesh3d m_OriSrc, m_OriTar, m_CurrentMesh, m_Src, m_Tar, m_Before;
	void TogglePrinAxis();
	void ToggleNearestPntsShow();
	void ToggleCorrespondences();
	bool GetPrinAxes();
	bool PrinAlignment();
	bool isShowPrinAxes;
	void NonRigidFitting();
	void Andresen2000Fitting();
	void ComputeElasticPotentialEnergy(float elasWeight);
	void ComputeEdgeBasedGeomEnergy(float elasWeight);
	void ComputeSingleDistanceEnergy(float disWeight);
	void ComputePaulyMultDistanceEnergy(float disWeight, int k);
	void ComputeDistanceEnergy(float disWeight, int k);
	void ComputeVolumeEnergy(float disWeight, int k);
	void ColorCurvatue();
	//fitting times
	int fittingTimes;

	// Constructor
	CSceneGraph3d();
	~CSceneGraph3d();

	// Datas
	void Add(CObject3d *pObject3d);
	void RemoveAt(int index);
	CObject3d *GetAt(int index);
	CObject3d *operator[](int index);
	int NbObject(void) { return m_ArrayObject3d.GetSize(); }
	void Free(void);

	// I/O
	int SaveFile(char *name);
	int SaveFileRaw(char *name);
	int WriteHeader(CStdioFile &file,char *name);

	// Misc
	int BuildAdjacency();
	int CalculateNormalPerVertex();
	int CalculateNormalPerFace();

	// Binding
	void SetNormalBinding(int type);
	void SetColorBinding(int type);

	// OpenGL
	virtual int glBuildList();
	void glDraw(void);
	void glDrawDirect(void);
	void glDraw(int type);

	// Textures
	CArray3d<CTexture> *GetArrayTexture() { return &m_ArrayTexture; }
	int HasTexture(char *name,int *index);
	int NbTexture() { return m_ArrayTexture.GetSize(); }
	int AddTexture(CTexture *pTexture) { return m_ArrayTexture.Add(pTexture); }
	void RemoveAllTexture() { m_ArrayTexture.RemoveAll(); }
	void RemoveTexture(int index) { m_ArrayTexture.RemoveAt(index); }
	CTexture *GetTexture(int index) { return m_ArrayTexture[index]; }
};

#endif // _SCENEGRAPH_3D_
