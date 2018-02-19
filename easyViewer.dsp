# Microsoft Developer Studio Project File - Name="easyViewer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=easyViewer - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "easyViewer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "easyViewer.mak" CFG="easyViewer - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "easyViewer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 OpenGL32.lib glu32.lib glut32.lib glaux.lib ANN.lib glew32.lib /nologo /subsystem:windows /machine:I386
# Begin Target

# Name "easyViewer - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\easyViewer.cpp
# End Source File
# Begin Source File

SOURCE=.\easyViewer.rc
# End Source File
# Begin Source File

SOURCE=.\easyViewerDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\easyViewerView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VertCorDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\easyViewer.h
# End Source File
# Begin Source File

SOURCE=.\easyViewerDoc.h
# End Source File
# Begin Source File

SOURCE=.\easyViewerView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Patch.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SparseMatrix.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UnionFinder.h
# End Source File
# Begin Source File

SOURCE=.\vec4.h
# End Source File
# Begin Source File

SOURCE=.\VertCorDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\easyViewer.ico
# End Source File
# Begin Source File

SOURCE=.\res\easyViewer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\easyViewerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Lib3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Lib3D\Array3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Array3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\AVLInd.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Base3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Color.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Color.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ColorRamp.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ColorRamp.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Edge3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Edge3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Face3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Face3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Material.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Material.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Mesh3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Mesh3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Object3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Object3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ParserVrml.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\ParserVrml.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\PsRender.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\PsRender.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\SceneGraph3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\SceneGraph3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Transform.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Transform.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Utils3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Utils3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vector3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vector3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vertex3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib3D\Vertex3d.h
# End Source File
# Begin Source File

SOURCE=.\Lib3D\WmfTools.h
# End Source File
# End Group
# Begin Group "glfont"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\glfont\glfont.cpp
# End Source File
# Begin Source File

SOURCE=.\glfont\glfont.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
