; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEasyViewerDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "easyViewer.h"
LastPage=0

ClassCount=6
Class1=CEasyViewerApp
Class2=CEasyViewerDoc
Class3=CEasyViewerView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_ABOUTBOX (English (U.S.))
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Resource3=IDR_MAINFRAME (English (U.S.))
Resource4=IDR_MAINFRAME
Class6=CVertCorDlg
Resource5=IDD_VertCorDlg

[CLS:CEasyViewerApp]
Type=0
HeaderFile=easyViewer.h
ImplementationFile=easyViewer.cpp
Filter=N

[CLS:CEasyViewerDoc]
Type=0
HeaderFile=easyViewerDoc.h
ImplementationFile=easyViewerDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CEasyViewerDoc

[CLS:CEasyViewerView]
Type=0
HeaderFile=easyViewerView.h
ImplementationFile=easyViewerView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CEasyViewerView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_ShowCors
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=easyViewer.cpp
ImplementationFile=easyViewer.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_OutputSrcCoord
Command5=ID_OutputCoord
Command6=ID_OutputTarCoord
Command7=ID_OutputSrcLandmarks
Command8=ID_OutputTargetLandmarks
Command9=ID_OutputSrcCenters
Command10=ID_APP_EXIT
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_ShowSource
Command14=ID_ShowTarget
Command15=ID_ShowCurrent
Command16=ID_Show3rd
Command17=ID_Show4th
Command18=ID_Show5th
Command19=ID_Show6th
Command20=ID_Show7th
Command21=ID_ShowWireframe
Command22=ID_ShadingModel
Command23=ID_GetPrinAxes
Command24=ID_ColorCurvature
Command25=ID_MENUITEM32786
Command26=ID_FlipNormal
Command27=ID_Transparent
Command28=ID_ShowNearestPnts
Command29=ID_ShowCors
Command30=ID_ShowTexture
Command31=ID_PrinAlignment
Command32=ID_WeightedFitting
Command33=ID_WeightedFitting10
Command34=ID_Fitting
Command35=ID_Fitting_mult
Command36=ID_AlignScale
Command37=ID_AndresenFit
Command38=ID_Pauly05Fit
Command39=ID_Pauly05MultFit
Command40=ID_ManiSource
Command41=ID_ManiTarget
Command42=ID_MENUITEM32799
Command43=ID_ManiAll
Command44=ID_Mirror
Command45=ID_Bend
Command46=ID_Subdivision
Command47=ID_OrientFaces
Command48=ID_ReorientSource
Command49=ID_ReorientTarget
Command50=ID_ReorientCurrent
Command51=ID_ReorientAll
Command52=ID_ShowDistances
Command53=ID_PrDis
Command54=ID_LmDistances
Command55=ID_VertCors
Command56=ID_AddLandmark
Command57=ID_DelLandmark
Command58=ID_ShowLandmark
Command59=ID_APP_ABOUT
CommandCount=59

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_OutputSrcCoord
Command5=ID_OutputCoord
Command6=ID_OutputTarCoord
Command7=ID_OutputSrcLandmarks
Command8=ID_OutputTargetLandmarks
Command9=ID_APP_EXIT
Command10=ID_VIEW_TOOLBAR
Command11=ID_VIEW_STATUS_BAR
Command12=ID_ShowSource
Command13=ID_ShowTarget
Command14=ID_ShowCurrent
Command15=ID_ShowWireframe
Command16=ID_ShadingModel
Command17=ID_GetPrinAxes
Command18=ID_ColorCurvature
Command19=ID_MENUITEM32786
Command20=ID_FlipNormal
Command21=ID_Transparent
Command22=ID_ShowNearestPnts
Command23=ID_ShowCors
Command24=ID_PrinAlignment
Command25=ID_Fitting
Command26=ID_Fitting_mult
Command27=ID_AlignScale
Command28=ID_AndresenFit
Command29=ID_Pauly05Fit
Command30=ID_Pauly05MultFit
Command31=ID_ManiSource
Command32=ID_ManiTarget
Command33=ID_MENUITEM32799
Command34=ID_ManiAll
Command35=ID_Mirror
Command36=ID_Subdivision
Command37=ID_OrientFaces
Command38=ID_ReorientSource
Command39=ID_ReorientTarget
Command40=ID_ReorientCurrent
Command41=ID_ShowDistances
Command42=ID_AddLandmark
Command43=ID_DelLandmark
Command44=ID_ShowLandmark
Command45=ID_APP_ABOUT
CommandCount=45

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VertCorDlg]
Type=1
Class=CVertCorDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_VertInd,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_MSelect,combobox,1344340227

[CLS:CVertCorDlg]
Type=0
HeaderFile=VertCorDlg.h
ImplementationFile=VertCorDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_MSelect
VirtualFilter=dWC

