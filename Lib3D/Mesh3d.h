//********************************************
// Mesh3d.h
// class CMesh3d
//********************************************
// A mesh : simplicial complex
// + Array of vertex
// + Array of faces
// + Binding infos
//********************************************
// alliez@usc.edu
// Created : 15/01/98
// Modified : 07/07/98
//********************************************

#ifndef _MESH_3D_
#define _MESH_3D_

#include "tnt.h"
#include "Array3d.h"
#include "Vertex3d.h"
#include "Face3d.h"
#include "Vector3d.h"
#include "Material.h"
#include "Transform.h"
#include "Vec3f.h"
#include "ANN/ANN.h"
#include "..\SparseMatrix.h"
#include "..\glfont\glfont.h"

class CColorRamp;
class CTexture;

class CMesh3d : public CObject3d
{
friend class CParserVrml;

public :
  bool isSetCoordMesh;
  CMesh3d* pCoordMesh;
	int stcflag; // 0...source, 1...target, 2...current
	GLFONT font;
	// VBO's name 
  unsigned int m_nVBOVertices;     //Vertex VBO's name

	// Datas
	CArray3d<CVertex3d> m_ArrayVertex;
	CArray3d<CVertex3d> m_ArraySample;
	CArray3d<CFace3d>   m_ArrayFace;
	CArray3d<CEdge3d> m_ArrayEdge;
	// landmark points
	CArray3d<CVertex3d> m_ArrayLandmark;
	vector<int> m_ArrayLandmarkInd;

 	CTransform m_Transform;
	CString m_Name;
	
	// Apparence & texture
	CMaterial m_Material; // material
	int m_NormalBinding;  // normals defined at vertices or faces ? (Gouraud)
	int m_ColorBinding;   // colors defined at vertices or faces ? (Gouraud)
  bool isShowTex;

	// Texture
	int m_IndexTexture;   // which texture in SceneGraph ? (-1 : no texture)
	float *m_pTextureCoordinate;
	int *m_pTextureCoordinateIndex;

	// OpenGL-specific
	unsigned int m_ListOpenGL;
	unsigned int m_ListDone;
	int m_Modified;
	int m_Show;

	//Principal axes(1st, 2nd, 3rd), a matrix stored as columns order...
	//The eigen vector corresponding to the largest eigen value is stored in the last(3rd) column.
	//These are for the use of rigid alignment
	Array2D<float> m_prinAxes;
	bool isShowPrinAxes;
	bool isShowNearestPnts;
	bool isShowCorrespondences;
	int corNo;
	bool isShowLandmarks;
	bool isBuildAdjacency;
	//ANN == Approximate Nearest Neighbor. 
	//For selecting appropriate principal directions
	ANNpointArray Vert_Array, V_Array;
	//The criterion...
	double randomAveNearestDis;
	ANNkd_tree* pVert_Tree3;
	ANNkd_tree* pSample_Tree3;
	ANNkd_tree* pVert_Tree6;
	double normalToVertexRatio;
	float avgEdgeLength;
	//two current nearest points
	float nearV1[3], nearV2[3];
	//maximum bounding box size
	float maxbBoxSize;

	//Curvature related
	bool isShowCurvature;
	//normal flipping
	int normalFlip;
	//Energies. For morphable alignment use.
	float m_DistanceEnergy;
	float m_ElasticEnergy;
	vector<float> b; //the optimization function Ax = b, where A matrix will be stored in edges and vertices
  llm::SparseMat A;

public :
	void MergeCorrespondences(CMesh3d*);
	void FillForwardDisEnergy(CMesh3d*, float&, int);
	void FillInverseDisEnergy(CMesh3d*, float&, int);
	void setTexCoord(CMesh3d*);
	void outputCenters();
  float linearFunc(float x, float y, float z, float* a);
	float quadFunc(float x, float y, float z, float* a);
	float* disVecFunc(float x, float y, float z, float xBSize, float yBSize, 
    float zBSize, float* a, float* b, float* c);
	void showTex();
	void bend();
	void BuildConsistCorrespondences(CMesh3d*, int);
	void setstcflag(int);
	void outputLandmarks(int srctar, CString s);
	float GetAvgAspectRatio();
	void Evolve(CMesh3d*);
	void GenerateUniformSamples();
	void mirror();
	double ComputeAvgEdgeLength();
	void output();
	void output3();
	void MultMat(CMesh3d*, float*);
	void MultMat(CObject3d*, float*);
	void ComputeDis(double*, CMesh3d*);
	void ComputeLocalCenters();
	void ReorientFaces();
	//for orient faces
	int* stack;
  int sp;
  unsigned char* flags;
	int reorients;
	void traverse(int);
	void OrientFaces();
	void _traverse();
	void push(int);
	int pop();
	void reorient(int);
	int isalong(int, int, int);
	int which(int, CFace3d*);
	
	void clearAllCorFlag();
	void GenerateEdgeSet();
	void initB();
	void ComputeDifCoord();
	void ComputeElasEnergy(CMesh3d*, float& elasWeight);
	void ComputeEdgeBasedGeomEnergy(CMesh3d*, float& elasWeight);
	void ComputeRingBasedGeomEnergy(CMesh3d*, float& elasWeight);
  void ComputeWeightedRingEnergy(CMesh3d* pSrcObject, CMesh3d *pPreviousMesh, float& elasWeight);
	void ComputePaulyDisEnergy(CMesh3d* pTargetMesh, float& weight);
	void ComputePaulyMultDisEnergy(CMesh3d*, float&, int);
	void ComputeSingleDisEnergy(CMesh3d*, float& disWeight);
	void ComputeDisEnergy(CMesh3d*, float& disWeight, int k);
	void ComputeVolEnergy(CMesh3d*, float& weight, int k);
	void AddShortCuts();
	float ComputeAGD(CVertex3d* pVert);
	void CopyVertex(CMesh3d* pMesh);
	float ComputAveNearestDis(CMesh3d* pMesh);
	float ComputAveNearestDis(CVertex3d* pv);
	void BuildAnnKDTree();
	void BuildSingleCorrespondence(CMesh3d*);
	void BuildSingleInverseCorrespondence(CMesh3d *pTargetMesh);
	void BuildCorrespondences(CMesh3d*, int);
	void BuildInverseCorrespondences(CMesh3d*, int);
	void BuildSampleCorrespondences(CMesh3d*, int);
	void ComputeCurvature();
	void Rotate(Vec3f& axe, float& angle);
	void GenPrinAxes();
	void BuildVBOs();
	void NormSubd();
	void GPDEbaseSubdivision();
  int bestMatchIndex(CMesh3d*, int);

	// Constructor
	CMesh3d();
	virtual ~CMesh3d();

	// Datas
	void Free();
	virtual int GetType();
	int IsValid();
	void Copy(CMesh3d *pMesh);

	// Faces
	int NbFace() { return m_ArrayFace.GetSize(); }
	CFace3d *GetFace(int index) { return m_ArrayFace[index]; }
	void AddFace(CFace3d *pFace) { m_ArrayFace.Add(pFace); }
	void SetNbFace(int NbFace) { m_ArrayFace.SetSize(NbFace); }
	CArray3d<CFace3d> *GetArrayFace() { return &m_ArrayFace; }
	int Has(CFace3d *pFace) { return m_ArrayFace.Has(pFace); }
	int FindFace(CFace3d *pFace); // even in neighborign faces

	// Flags
	void SetFlagOnFaces(int flag);
	void SetFlagOnVertices(int flag);
	CVertex3d *GetFirstVertexWithFlag(int flag);
	int GetMaxFlagOnFaces();
	int FindSmallestFlagOnVerticesDiffThan(int flag,int *pFounded);
	int FindFlagOnVerticesDiffThan(int flag);
	CVertex3d *GetAnyVertexWithFlagRootPrefered(int flag);


	// Vertices
	int NbVertex() { return m_ArrayVertex.GetSize(); }
	void SetNbVertex(int NbVertex)  { m_ArrayVertex.SetSize(NbVertex); }
	CArray3d<CVertex3d> *GetArrayVertex() { return &m_ArrayVertex; }
	void AddVertex(CVertex3d *pVertex) { m_ArrayVertex.Add(pVertex); }
	int DeleteVertex(CVertex3d *pVertex);
	int DeleteVertex(int index);
	int DeleteFaceNbNeighbors(int NbNeighbor);
	CVertex3d *GetVertex(int index) {return m_ArrayVertex[index];}
	int Has(CVertex3d *pVertex) { return m_ArrayVertex.Has(pVertex); }

	// Edges
	float GetMeanLengthEdge();

	// Name
	CString GetName(void) { return m_Name; } 
	void SetName(CString &string) { m_Name = string; } 

	// I/O
	int WriteFile(CStdioFile &file);
	int WriteFileRaw(CFile &file);

	// Transform
	void SetTransform(CTransform &transform) { m_Transform.Copy(transform); }
	CTransform *GetTransform(void) { return &m_Transform; }

	// Range
	void Range(int coord,float *min,float *max); 
	void Range(int coord,float min,float max); 
	void Offset(int coord,float offset);
	void Scale(int coord,float scale);
	void Move(float dx,float dy,float dz);

	// Predefined
	int GenerateMap(int line,int col,float min,float max);
	int GenerateBox(float dx,float dy,float dz);
	int GenerateMap(CTexture *pTexture,int width,int height,int FlagColor = 1);

	// OpenGL
	virtual int glBuildList();
	virtual int glDraw();
	virtual int glDrawDirect(); // no display list
	void Show(int flag) { m_Show = flag; }

	// Wmf drawing
	void glDrawProjectLine(CDC *pDC,double *modelMatrix,double *projMatrix,
		int *viewport,COLORREF ColorLine,double ratio,int height);
	void glDrawProjectFace(CDC *pDC,double *modelMatrix,double *projMatrix, int *viewport,
		COLORREF ColorLine,COLORREF ColorFace,double ratio,int height,float RatioNbFace = 1.0f);
	int GenerateEdgeArray(CArray3d<CEdge3d> *pArrayEdge,BOOL FlagOnBoundary = FALSE,
		                    char flag = 1,BOOL *pHasBoundary = NULL);

	// Debug
	void Trace();

	// Adjacency 
	int BuildAdjacency();

	// Processing
	int IndexFrom(CFace3d *pFace);
	int IndexFrom(CVertex3d *pVertex);

	// Modif
	void SetModified(int flag = 1) { m_Modified = flag; }
	int GetModified() { return m_Modified; }


	// Vertex removal
	int VertexRemoval(CVertex3d *pV);
	int VertexRemoval();

	// Search
	int FindVertex(CVertex3d *pVertex);

	// Normals
	int CalculateNormalPerVertex(void);
	int CalculateNormalPerFace(void);
	void SetNormalBinding(int type);
	int GetNormalBinding(void);
	CVectorSet3d *GetVectorSetNormalPerFace(void); // alloc on the global heap

	// Adjacency
	void Rebuild();

	// Distance
	double SquareDistanceToVertex(CVertex3d *pVertex,CVertex3d **pVertexRef);
	double SquareDistanceToFace(CVertex3d *pVertex,CVertex3d *pVertexRef,CFace3d **ppFaceRef);

	// Sharp edges
	int ColorSharpEdge(float threshold,CColor &color);
	CVectorSet3d *GetSharpEdges(float threshold);

	// Strings
	CVectorSet3d *GetStringBetweenVertices();

	// Color
	void SetColorBinding(int type);
	void SetColor(CColor &color);
	void SetColor(unsigned char r,unsigned char g,unsigned char b);
	int GetColorBinding(void);

	// Material
	CMaterial *GetMaterial() { return &m_Material; }
	void SetMaterial(CMaterial *pMaterial) { m_Material.Copy(pMaterial); }

	// Texture
	int GetTextureIndex() { return m_IndexTexture; }

	// Subdivision
	int SubdivisionLoop(); // Charles Loop (1987)
	int Subdivision(void); 
	float Alpha(int n);

	// Smoothing
	int Smooth(int MoveOnBundary = 1);

	// Coloring (curvature, compacity, etc...)
	void ColorCurvature(CColorRamp *pRamp);
	void ColorNormalSpace(CColorRamp *pRamp);
	void ColorCompacity(CColorRamp *pRamp);
	void ColorHeight(CColorRamp *pRamp);
	void ColorFacesFromFlagPastels();
	void ColorFacesFromFlagGrey();

	// Intersection
	int NearestIntersectionWithLine(CVertex3d *pV0,CVertex3d *pV1,CVertex3d *pVertexResult,
		                              CFace3d **ppFaceResult,int *pNbFaceVisited);
	// Area
	double GetMinArea(CFace3d **ppFace = NULL);
	double GetMeanArea();

};


#endif // _MESH_3D_
