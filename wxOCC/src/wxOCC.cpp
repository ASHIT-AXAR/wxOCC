///////////////////////////////////////////////////////////////////////////////
// Name:        wxOCC.cpp
// Purpose:     OpenCascade Control implemented in wxWidgets
// Author:      R. T. Roe.
// Created:     2016-03-01
// Copyright:   (C) Copyright 2016, Virtex Edge Design Inc.
// Licence:     Attribution-ShareAlike 4.0 International
///////////////////////////////////////////////////////////////////////////////

#include "../include/wxOCC.h"

#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pnt.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_MapOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopExp_Explorer.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepAlgo_Cut.hxx>
#include <BRepAlgo.hxx>


using namespace std;

BEGIN_EVENT_TABLE(wxOCCWindow, wxScrolledWindow)
EVT_SIZE(wxOCCWindow::OnSize)
EVT_IDLE(wxOCCWindow::OnIdle)
EVT_LEFT_DOWN(wxOCCWindow::OnLeftDown)
EVT_LEFT_UP(wxOCCWindow::OnLeftUp)
EVT_MOTION(wxOCCWindow::OnMotion)
EVT_RIGHT_DOWN(wxOCCWindow::OnRightDown)
EVT_RIGHT_UP(wxOCCWindow::OnRightUp)
END_EVENT_TABLE()

wxOCCWindow::wxOCCWindow(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style)
    : wxScrolledWindow(parent, -1, pos, size, style)
{
    ////////////////////////////Cascade/////////////////////////////

    myXmin = 0;
    myYmin = 0;
    myXmax = 0;
    myYmax = 0;
    xMouse = 0;
    yMouse = 0;

    TCollection_ExtendedString a3DName("Visu3D");

    ctrl = new wxPanel(this);

    ctrl->SetEventHandler(this);
    // ctrl->SetBackgroundColour(wxColour(50,50,50));
    //    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    //    sizer->Add(ctrl, 1, wxEXPAND);
    //    this->SetSizer(sizer);
    //    this->SetAutoLayout(true);

    myViewer = Viewer(ctrl, a3DName.ToExtString(), "", 1000.0, V3d_XposYnegZpos, Standard_True, Standard_True);

    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
    myContext = new AIS_InteractiveContext(myViewer);

    Handle(Graphic3d_AspectFillArea3d) anAspect = new Graphic3d_AspectFillArea3d(Aspect_IS_SOLID,
                                                                                 Quantity_NOC_RED,
                                                                                 Quantity_NOC_YELLOW,
                                                                                 Aspect_TOL_SOLID,
                                                                                 1.0,
                                                                                 Graphic3d_NOM_PLASTIC,
                                                                                 Graphic3d_NOM_PLASTIC);
    anAspect->SetEdgeOn();

    Standard_Real myWidth = 200;

    // Creating a Plane
    gp_Pnt aPnt1(-myWidth / 2., 0, -myWidth / 2.);
    gp_Pnt aPnt2(-myWidth / 2., 0, myWidth / 2.);
    gp_Pnt aPnt3(myWidth / 2., 0, myWidth / 2.);
    gp_Pnt aPnt4(myWidth / 2., 0, -myWidth / 2.);

    // Handle(Geom_TrimmedCurve) aSegment1 = ;
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt2, aPnt3);
    Handle(Geom_TrimmedCurve) aSegment3 = GC_MakeSegment(aPnt3, aPnt4);
    Handle(Geom_TrimmedCurve) aSegment4 = GC_MakeSegment(aPnt4, aPnt1);

    // Profile : Define the Topology
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(GC_MakeSegment(aPnt1, aPnt2));
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment3);
    TopoDS_Edge anEdge4 = BRepBuilderAPI_MakeEdge(aSegment4);
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3, anEdge4);

    BRepBuilderAPI_MakeFace MF(aWire);

    TopoDS_Shape shape = MF.Shape();

    Handle(AIS_Shape) stpShape = new AIS_Shape(shape);
    myContext->SetColor(stpShape, Quantity_NOC_AZURE);
    myContext->SetMaterial(stpShape, Graphic3d_NOM_PLASTIC);
    myContext->SetTransparency(stpShape, 0.5);
    myContext->SetDisplayMode(stpShape, 1);
    myContext->Display(stpShape);

    BRepBuilderAPI_MakeEdge edge(aPnt1, aPnt2);
    TopoDS_Edge ds_edge = edge.Edge();
    Handle(AIS_Shape) line = new AIS_Shape(ds_edge);
    myContext->SetColor(line, Quantity_NOC_BLACK);
    myContext->SetMaterial(line, Graphic3d_NOM_PLASTIC);
    myContext->SetDisplayMode(line, 1);
    myContext->Display(line);

    // Build of a curved Face
    TColgp_Array2OfPnt poles1(1, 3, 1, 3);
    poles1(1, 1) = gp_Pnt(0., 0., 0.);
    poles1(2, 1) = gp_Pnt(0.5, 0., 0.);
    poles1(3, 1) = gp_Pnt(1., 0., 0.);
    poles1(1, 2) = gp_Pnt(0., 0.5, 0.5);
    poles1(2, 2) = gp_Pnt(0.5, 0.5, 0.5);
    poles1(3, 2) = gp_Pnt(1., 0.5, 0.2);
    poles1(1, 3) = gp_Pnt(0., 1., 0.);
    poles1(2, 3) = gp_Pnt(0.5, 1., 0.);
    poles1(3, 3) = gp_Pnt(1., 1., 0.);
    Handle_Geom_BezierSurface surf1 = new Geom_BezierSurface(poles1);
    TopoDS_Face aFace = BRepBuilderAPI_MakeFace(surf1, 1e-6).Face();

    Handle(AIS_Shape) BezShape = new AIS_Shape(aFace);
    myContext->SetColor(BezShape, Quantity_NOC_AZURE);
    myContext->SetMaterial(BezShape, Graphic3d_NOM_PLASTIC);
    myContext->SetTransparency(BezShape, 0.5);
    myContext->SetDisplayMode(BezShape, 1);
    myContext->Display(BezShape);

    CreateBottle();
    
    mView->ZBufferTriedronSetup(Quantity_NOC_RED,Quantity_NOC_GREEN,Quantity_NOC_BLUE1,0.8,0.05,12);
    mView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER,Quantity_NOC_WHITE,0.1,V3d_ZBUFFER);

    myContext->CloseAllContexts();
    myContext->OpenLocalContext();
    myContext->ActivateStandardMode(TopAbs_VERTEX);
    myContext->ActivateStandardMode(TopAbs_EDGE);
    myContext->ActivateStandardMode(TopAbs_FACE);
    
}

wxOCCWindow::~wxOCCWindow()
{
    // Need to reset Event Handler or will give a Segment Fault.
    ctrl->SetEventHandler(ctrl);

    // Now Remove the View, all of it, even Rosie O'donald
    mView->Remove();
}

void wxOCCWindow::CreateBottle()
{
    TopoDS_Shape aBottle = MakeBottle(150, 210, 90);
    Handle(AIS_Shape) AISBottle = new AIS_Shape(aBottle);
    myContext->SetMaterial(AISBottle, Graphic3d_NOM_GOLD);
    myContext->SetTransparency(AISBottle, 0.5);
    myContext->SetDisplayMode(AISBottle, 1);
    myContext->Display(AISBottle, Standard_False);
    DoZoomFit();
}

TopoDS_Shape wxOCCWindow::MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness)
{
    // Profile : Define Support Points
    gp_Pnt aPnt1(-myWidth / 2., 0, 0);
    gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt3(0, -myThickness / 2., 0);
    gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt5(myWidth / 2., 0, 0);
    // Profile : Define the Geometry
    Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2, aPnt3, aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);
    // Profile : Define the Topology
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);
    // Complete Profile
    gp_Ax1 xAxis = gp::OX();
    gp_Trsf aTrsf;
    aTrsf.SetMirror(xAxis);
    BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
    TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);
    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile = mkWire.Wire();
    // Body : Prism the Profile
    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
    gp_Vec aPrismVec(0, 0, myHeight);
    TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);
    // Body : Apply Fillets
    BRepFilletAPI_MakeFillet mkFillet(myBody);
    TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
    while(anEdgeExplorer.More()) {
        TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
        // Add edge to fillet algorithm
        mkFillet.Add(myThickness / 12., anEdge);
        anEdgeExplorer.Next();
    }
    myBody = mkFillet.Shape();
    // Body : Add the Neck
    gp_Pnt neckLocation(0, 0, myHeight);
    gp_Dir neckAxis = gp::DZ();
    gp_Ax2 neckAx2(neckLocation, neckAxis);
    Standard_Real myNeckRadius = myThickness / 4.;
    Standard_Real myNeckHeight = myHeight / 10.;
    BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, myNeckRadius, myNeckHeight);
    TopoDS_Shape myNeck = MKCylinder.Shape();
    myBody = BRepAlgoAPI_Fuse(myBody, myNeck);
    // Body : Create a Hollowed Solid
    TopoDS_Face faceToRemove;
    Standard_Real zMax = -1;
    for(TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()) {
        TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
        // Check if <aFace> is the top face of the bottle's neck
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ = aPnt.Z();
            if(aZ > zMax) {
                zMax = aZ;
                faceToRemove = aFace;
            }
        }
    }
    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);
    myBody = BRepOffsetAPI_MakeThickSolid(myBody, facesToRemove, -myThickness / 50, 1.e-3);
    // Threading : Create Surfaces
    Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
    Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);
    // Threading : Define 2D Curves
    gp_Pnt2d aPnt(2. * M_PI, myNeckHeight / 2.);
    gp_Dir2d aDir(2. * M_PI, myNeckHeight / 4.);
    gp_Ax2d anAx2d(aPnt, aDir);
    Standard_Real aMajor = 2. * M_PI;
    Standard_Real aMinor = myNeckHeight / 10;
    Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
    Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
    Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
    Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
    gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
    gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);
    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);
    // Threading : Build Edges and Wires
    TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
    TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
    TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
    TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
    TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
    TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
    BRepLib::BuildCurves3d(threadingWire1);
    BRepLib::BuildCurves3d(threadingWire2);
    // Create Threading
    BRepOffsetAPI_ThruSections aTool(Standard_True);
    aTool.AddWire(threadingWire1);
    aTool.AddWire(threadingWire2);
    aTool.CheckCompatibility(Standard_False);
    TopoDS_Shape myThreading = aTool.Shape();
    // Building the Resulting Compound
    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aRes);
    aBuilder.Add(aRes, myBody);
    aBuilder.Add(aRes, myThreading);

//    Handle(TDF_Data) DF = new TDF_Data();
    // TDF_Label aLabel = DF->Root();

    return aRes;
}

////////////////////////////////////////////////////////////
// Name : OnDraw
// Usage : This function is called whenever the window is
// invalidated e.g if a window on top of it is moved
////////////////////////////////////////////////////////////
void wxOCCWindow::OnIdle(wxIdleEvent& event)
{
    mView->MustBeResized(); // if the user changes the window size
    mView->Redraw();
}

void wxOCCWindow::OnDraw(wxDC& dc)
{
    std::cout << "DRAW" << std::endl;
    mView->Redraw();
}

////////////////////////////////////////////////////////////
// Name : OnSize
// Usage : This function is called whenever the window is
// resized by the user
////////////////////////////////////////////////////////////

void wxOCCWindow::OnSize(wxSizeEvent& event)
{
    int width, height;

    this->GetClientSize(&width, &height);
    ctrl->SetSize(width, height);

    if(!mView.IsNull())
        mView->MustBeResized(); // if the user changes the window size
    // this will re-center the Cascade View
    mView->Redraw();

    Refresh();
    Update();
    event.Skip();
}

void wxOCCWindow::DoSetToShaded()
{
    int width, height;

    ctrl->GetClientSize(&width, &height);

    myContext->Select(0, 0, width, height, mView);

    for(myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent())
        myContext->SetDisplayMode(myContext->Current(), 1);
}

void wxOCCWindow::DoSetToWireframe()
{
    int width, height;

    ctrl->GetClientSize(&width, &height);

    myContext->Select(0, 0, width, height, mView);

    for(myContext->InitCurrent(); myContext->MoreCurrent(); myContext->NextCurrent())
        myContext->SetDisplayMode(myContext->Current(), 0);
}

void wxOCCWindow::DoZoomIn()
{
    static Standard_Real zoomX1(0), zoomY1(0), zoomX2(0), zoomY2(0);
    zoomX1 -= 10;

    zoomY1 -= 10;

    mView->Zoom(zoomX1, zoomY1, zoomX2, zoomY2);

    zoomX2 = zoomX1;

    zoomY2 = zoomY1;
}

void wxOCCWindow::DoZoomOut()
{
    static Standard_Real zoomX1(0), zoomY1(0), zoomX2(0), zoomY2(0);
    zoomX1 += 10;

    zoomY1 += 10;

    mView->Zoom(zoomX1, zoomY1, zoomX2, zoomY2);

    zoomX2 = zoomX1;

    zoomY2 = zoomY1;
}

void wxOCCWindow::DoZoomFit()
{
    mView->FitAll();

    mView->ZFitAll();
}

void wxOCCWindow::DoZoomSelection()
{
    wxWindow::SetCursor(wxCursor(wxCURSOR_MAGNIFIER));

    CurMode = CurAction3d_WindowZooming;
    // int width;
    //
    // int height;
    //
    // ctrl->GetClientSize(&width, &height);
    //
    // myContext->Select (0,0,width,height,mView);
    //
    // for(myContext->InitCurrent();
    // myContext->MoreCurrent();
    // myContext->NextCurrent())
    // myContext->SetDisplayMode(myContext->Current(),0);
}

void wxOCCWindow::OnLeftDown(wxMouseEvent& event)
{
    event.GetPosition(&xMouse, &yMouse);

    myXmin = xMouse;

    myXmax = xMouse;

    myYmax = yMouse;

    myYmin = yMouse;

    switch(CurMode) {
    case CurAction3d_Nothing:

        if(event.m_shiftDown)
            ShiftSelectObject(1); // add objects to current selection by Shift + Left Mouse Click
        else
            SelectObject(1); // select object by left mouse down

        break;

    case CurAction3d_WindowZooming:

        break;

    case CurAction3d_DynamicPanning:

        break;

    case CurAction3d_DynamicRotation:

        mView->StartRotation(xMouse, yMouse);

        break;

    } // end switch (CurMode)
}

void wxOCCWindow::OnLeftUp(wxMouseEvent& event)
{
    event.GetPosition(&xMouse, &yMouse);

    myXmax = xMouse;

    myYmax = yMouse;

    if(CurMode == CurAction3d_Nothing)
        wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));

    if(CurMode == CurAction3d_WindowZooming) {
        DrawRectangle(myXmin, myYmin, xMouse, yMouse, false);

        ZoomWnd(); // zooming by rubber-banding zoom area

        // ReleaseCapture();// ask window to no longer give us events even if outside the view
    }

    if(CurMode == CurAction3d_Nothing) {
        DrawRectangle(myXmin, myYmin, xMouse, yMouse, false);

        // ReleaseCapture();// ask window to no longer give us events even if outside the view
    }
    myContext->SetHilightColor(Quantity_NOC_ORANGE);
    // myContext->InitSelected ();
    // while (myContext->MoreSelected ())
    //{
    //// Do something with: _ais_context->SelectedInteractive ();
    //
    // Handle(AIS_InteractiveObject) ashape = myContext->SelectedInteractive();
    // ashape->SetColor(Quantity_NOC_ORANGE2);
    //    //Handle(AIS_Shape) AISBottle=new AIS_Shape(ashape);
    //    //myContext->SetMaterial(ashape,Graphic3d_NOM_BRASS);
    //    //myContext->SetColor(ashape,Graphic3d_NOM_GOLD);
    // std::cout<<ashape->Signature ()<<std::endl;
    //
    //// to be able to use the picked shape
    //
    // myContext->NextSelected ();
    //}
}

void wxOCCWindow::OnRightDown(wxMouseEvent& event)
{
    event.GetPosition(&xMouse, &yMouse);

    myXmax = xMouse;

    myYmax = yMouse;

    if(event.m_controlDown) {

        mView->StartRotation(xMouse, yMouse); // for CtrlRotate

    } // end if (event.m_controlDown)

    switch(CurMode) {
    case CurAction3d_Nothing:

        if(!event.m_controlDown)
            wxMessageBox("Your Pop-up Menu here", "wxWindow::PopupMenu()");

        break;

    case CurAction3d_WindowZooming:

        wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));

        CurMode = CurAction3d_Nothing;

        break;

    case CurAction3d_DynamicPanning:

        wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));

        CurMode = CurAction3d_Nothing;

        break;

    case CurAction3d_DynamicRotation:

        wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));

        CurMode = CurAction3d_Nothing;

        break;

    } // end switch (CurMode)
}

void wxOCCWindow::OnRightUp(wxMouseEvent& WXUNUSED(event))
{
    wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));
}

void wxOCCWindow::OnMotion(wxMouseEvent& event)
{

    event.GetPosition(&xMouse, &yMouse);
    // cout<<"Mouse (X: "<<xMouse<<" ,Y:"<<yMouse<<")"<<endl;

    ////// note - the "if" & "else if" are used here for mutually exclusive events

    if(event.Dragging() && CurMode == CurAction3d_Nothing && !event.m_controlDown && !event.m_rightDown) {
        // SetCapture((HWND)(this->GetHandle())); // capture mouse movements
        // event if it moves out of window

        wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));

        DrawRectangle(myXmin, myYmin, xMouse, yMouse, false);

        if(event.m_shiftDown)     // add to currently selected objects those
            ShiftSelectObject(0); // included in the rubber-band window
        else
            SelectObject(0); // select objects by rubber-band window

        DrawRectangle(myXmin, myYmin, xMouse, yMouse, true);

    }

    else if(event.Moving() && CurMode == CurAction3d_Nothing && !event.m_controlDown &&
            !event.m_rightDown)  // no buttons pressed, mouse moving
        CurMove(xMouse, yMouse); // makes the AIS objects come alive !!

    ////// end note on mutually exclusive events

    if(event.LeftIsDown() && event.Dragging() && event.m_controlDown && event.ShiftDown())
        DynPan();

    else if(event.LeftIsDown() && event.Dragging() && event.m_controlDown)
        CtrlZoom(myXmax, myYmax, xMouse, yMouse); // zoom with ctrl & left mouse button

    // if(event.MiddleIsDown())
    //    cout<<"m dwn"<<endl;

    if(event.RightIsDown() && event.Dragging() && event.m_controlDown)
        CtrlRotate(xMouse, yMouse); // rotate with ctrl & right mouse down
    else if(event.RightIsDown() && event.Dragging())
        wxWindow::SetCursor(wxCursor(wxCURSOR_ARROW));

    if(event.LeftIsDown() && event.Dragging() && CurMode == CurAction3d_WindowZooming) {
        // SetCapture((HWND)(this->GetHandle())); // capture mouse movements
        // event if it moves out of window

        DrawRectangle(myXmin, myYmin, xMouse, yMouse, false);
        DrawRectangle(myXmin, myYmin, xMouse, yMouse, true);
    }

    if(event.LeftIsDown() && event.Dragging() && CurMode == CurAction3d_DynamicRotation)
        DynRotate(); // rotating with the mouse

    if(event.LeftIsDown() && event.Dragging() && CurMode == CurAction3d_DynamicPanning)
        DynPan(); // panning with the mouse

    mView->Redraw();
}

void wxOCCWindow::CurMove(long& xcoord, long& ycoord)
{

    myContext->MoveTo(xcoord, ycoord, mView); // makes the CAD objects
    // come alive
}

void wxOCCWindow::CtrlZoom(long x1, long y1, long x2, long y2)
{
    wxWindow::SetCursor(wxCursor(wxCURSOR_MAGNIFIER));

    CurMode = CurAction3d_Nothing;

    mView->Zoom(x1, y1, x2, y2);

    myXmax = x2;

    myYmax = y2;
}

void wxOCCWindow::CtrlRotate(long& xcoord, long& ycoord)
{
    wxWindow::SetCursor(wxCursor(wxCURSOR_BULLSEYE));

    CurMode = CurAction3d_Nothing;

    mView->Rotation(xcoord, ycoord);
}

void wxOCCWindow::ZoomWnd()
{
    if((abs(myXmin - myXmax) > 1) || (abs(myYmin - myYmax) > 1)) {
        mView->WindowFitAll(myXmin, myYmin, myXmax, myYmax);
    }
}

void wxOCCWindow::DynRotate()
{
    mView->Rotation(xMouse, yMouse);

    mView->Redraw();
}

void wxOCCWindow::DynPan()
{
    mView->Pan(xMouse - myXmax, myYmax - yMouse);

    myXmax = xMouse;

    myYmax = yMouse;
}

void wxOCCWindow::ShiftSelectObject(const Standard_Integer flag)
{

    if(flag == 0)
        myContext->ShiftSelect(myXmin, myYmin, xMouse, yMouse, mView);

    if(flag == 1)
        myContext->ShiftSelect();
}

void wxOCCWindow::SelectObject(const Standard_Integer flag)
{

    // flag == 1 button down
    // flag == 0 move

    if(flag == 0)
        myContext->Select(myXmin, myYmin, xMouse, yMouse, mView);

    if(flag == 1)
        myContext->Select();

    myContext->InitSelected();
    while(myContext->MoreSelected()) {
        // Do something with: _ais_context->SelectedInteractive ();

        Handle(AIS_InteractiveObject) ashape = myContext->SelectedInteractive();
        ashape->SetColor(Quantity_NOC_ORANGE2);
        // Handle(AIS_Shape) AISBottle=new AIS_Shape(ashape);
        // myContext->SetMaterial(ashape,Graphic3d_NOM_BRASS);
        // myContext->SetColor(ashape,Graphic3d_NOM_GOLD);
        std::cout << "shape: " << ashape->Signature() << std::endl;

        // to be able to use the picked shape

        myContext->NextSelected();
    }
}

//////////
// End***//
//////////

////////////////////////////////////////////////////////////
// Name : DrawRectangle
// Usage : This is just a cosmetic rectangle display for
// the user while selecting or zooming by
// stretching a rubber-band window.
//	It is actually not straight-forward to get it
//	to work correctly !
////////////////////////////////////////////////////////////

void wxOCCWindow::DrawRectangle(long initX, long initY, long finalX, long finalY, bool Draw)
{
    static long storedInitX, storedInitY, storedFinalX, storedFinalY;

    static bool flag;

    wxClientDC dc(this);

    wxPen thePen("black", 1, wxSHORT_DASH);

    dc.SetPen(thePen);

    dc.SetBrush(*wxTRANSPARENT_BRUSH);

    dc.SetLogicalFunction(wxINVERT);

    if(flag && !Draw) {

        dc.DrawLine(storedInitX, storedInitY, storedFinalX, storedInitY);

        dc.DrawLine(storedInitX, storedInitY, storedInitX, storedFinalY);

        dc.DrawLine(storedFinalX, storedInitY, storedFinalX, storedFinalY);

        dc.DrawLine(storedInitX, storedFinalY, storedFinalX, storedFinalY);

        flag = false;

    } // end if(flag && !Draw)

    storedInitX = min(initX, finalX);

    storedInitY = min(initY, finalY);

    storedFinalX = max(initX, finalX);

    storedFinalY = max(initY, finalY);

    if(Draw) {

        dc.DrawLine(storedInitX, storedInitY, storedFinalX, storedInitY);

        dc.DrawLine(storedInitX, storedInitY, storedInitX, storedFinalY);

        dc.DrawLine(storedFinalX, storedInitY, storedFinalX, storedFinalY);

        dc.DrawLine(storedInitX, storedFinalY, storedFinalX, storedFinalY);

        flag = true;

    } // end if (Draw)

    dc.SetPen(wxNullPen);

    dc.SetBrush(wxNullBrush);
}

// =======================================================================
// function : Viewer
// purpose  :
// =======================================================================
Handle(V3d_Viewer) wxOCCWindow::Viewer(wxWindow* panel,
                                       const Standard_ExtString theName,
                                       const Standard_CString theDomain,
                                       const Standard_Real theViewSize,
                                       const V3d_TypeOfOrientation theViewProj,
                                       const Standard_Boolean theComputedMode,
                                       const Standard_Boolean theDefaultComputedMode)
{
    static Handle(OpenGl_GraphicDriver) aGraphicDriver;

    aDisplayConnection = new Aspect_DisplayConnection();

    if(aGraphicDriver.IsNull()) {
        aDisplayConnection = new Aspect_DisplayConnection();

        aGraphicDriver = new OpenGl_GraphicDriver(aDisplayConnection);
    }
    Handle(V3d_Viewer) v = new V3d_Viewer(aGraphicDriver,
                                          theName,
                                          theDomain,
                                          theViewSize,
                                          theViewProj,
                                          Quantity_NOC_GRAY30,
                                          V3d_ZBUFFER,
                                          V3d_GOURAUD,
                                          V3d_WAIT,
                                          theComputedMode,
                                          theDefaultComputedMode,
                                          V3d_TEX_NONE);

    // Create the View
    mView = v->CreateView();

    // This works in GTK 1,0 apperantly but breaks in GTK 2.0...need a better solution
    // GdkWindow* draw_window = GTK_PIZZA(this)->bin_window;

    // First Retreive the GTK Widget (This is handled differently in MSW and MAC)
    GtkWidget* widget = panel->GetHandle();

    // Mandatory. Otherwise, a segfault happens. (Learned this one the hard way)
    gtk_widget_realize(widget);

    // Old Habbts I guess...
    gtk_widget_set_double_buffered(widget, 1);

    // Now retireve the X11 Window
    Window wid = GDK_WINDOW_XWINDOW(widget->window);

    // The above line can be done with the following as well.
    // Window wid = gdk_x11_drawable_get_xid(gtk_widget_get_window(panel->GetHandle()));

    XSync(GDK_WINDOW_XDISPLAY(widget->window), False);

    // Now create the Xw window as a wrapper of the window we've so tirelessly had to go and retreive.
    wind = new Xw_Window(aDisplayConnection, wid);

    // Un Comment this line below to create a seperate window (defeats the purpose of this hole exercise though)
    // Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection,"test", 20,20,400,400);

    // Now set the Window to the View
    mView->SetWindow(wind);

    // Don't forget to Map the Window
    if(!wind->IsMapped())
        wind->Map();

    // Now Draw the Scene
    // mView->Redraw();

    // panel->Refresh();

    return v;
}

int i = 0;
void wxOCCWindow::AddSphere()
{
    i += 150;
    BRepPrimAPI_MakeSphere S(gp_Pnt(100., i * 2, i), 300.);
    Handle(AIS_Shape) anAISShape = new AIS_Shape(S.Shape());
    myContext->SetColor(anAISShape, Quantity_NOC_ORANGERED2);
    myContext->SetMaterial(anAISShape, Graphic3d_NOM_DIAMOND);
    myContext->SetDisplayMode(anAISShape, 1);

    myContext->Display(anAISShape);

    mView->FitAll();
    mView->MustBeResized();

    mView->Redraw();
}
