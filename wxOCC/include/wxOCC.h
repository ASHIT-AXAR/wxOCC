
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <wx/aboutdlg.h>
#include <wx/event.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"

#include "wx/aui/aui.h"
#include <wx/defs.h>

#include <AIS_InteractiveContext.hxx>
#include <AIS_Circle.hxx>
#include <AIS_Line.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_Shape.hxx>
#include <Aspect_Handle.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <Geom_BezierSurface.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <GCE2d_MakeSegment.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRep_Tool.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Line.hxx>
#include <Geom_Axis2Placement.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Graphic3d_AspectFillArea3d.hxx>
#include <BRepLib.hxx>
#include <gp.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Standard_ErrorHandler.hxx>
#include <StdPrs_ShadedShape.hxx>
#include <StdPrs_WFDeflectionShape.hxx>
#include <StdSelect_BRepSelectionTool.hxx>
#include <STEPControl_Reader.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <UnitsAPI.hxx>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <Xw_Window.hxx>

#include <gtk/gtkwidget.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

////////////////////////////////////////////////////////////
// Name : wxOCCWindow
// Usage : This is the main window which will display CAD
//	objects. For the user to work with the mouse,
// the window must be able to receive mouse events,
//	this is possible because wxScrolledWindow is
// derived from wxWindow, which receives mouse evts
////////////////////////////////////////////////////////////

enum { TIMER_ID = 10 };

enum CurAction3d {
    CurAction3d_Nothing,
    CurAction3d_WindowZooming,
    CurAction3d_DynamicPanning,
    CurAction3d_DynamicRotation
};

class wxOCCWindow : public wxScrolledWindow
{
public:
    ////////////////////////////////////////////////////////////
    // Name : wxOCCWindow
    // Usage : Constructor. Initializes the wxScrolledWindow
    // from which it is derived
    ////////////////////////////////////////////////////////////

    wxOCCWindow(wxWindow* parent, const wxPoint& pos, const wxSize& size, long style);

    ~wxOCCWindow();

    wxPanel* ctrl;

    ////////////////////////////////////////////////////////////
    // Name : OnDraw
    // Usage : This function is called whenever the window is
    // invalidated e.g if a window on top of it is moved
    ////////////////////////////////////////////////////////////

    virtual void OnDraw(wxDC& dc);

    void DoSetToShaded();
    void DoSetToWireframe();
    void DoZoomIn();
    void DoZoomOut();
    void DoZoomFit();
    void DoZoomSelection();

    void CreateBottle();

    ////////////////////////////Cascade/////////////////////////////

    Handle(Xw_Window) wind;
    Handle(Aspect_DisplayConnection) aDisplayConnection;
    Handle(V3d_Viewer) myViewer;

    //! the occ view.
    Handle(V3d_View) mView;
    Handle(AIS_InteractiveContext) myContext;

    Handle(V3d_Viewer) Viewer(wxWindow* panel,
                              const Standard_ExtString theName,
                              const Standard_CString theDomain,
                              const Standard_Real theViewSize,
                              const V3d_TypeOfOrientation theViewProj,
                              const Standard_Boolean theComputedMode,
                              const Standard_Boolean theDefaultComputedMode);

    TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
    //////////////////////////////////////////////
    // Begin*
    // Following variables are used in functions
    // that respond to the mouse events pan,
    // rotate, zoom, select
    //////////////////////////////////////////////

    CurAction3d CurMode;

    long myXmin;
    long myYmin;
    long myXmax;
    long myYmax;
    long xMouse;
    long yMouse;

    /***************************************************************************/
    //                                              Event-Table Functions
    /***************************************************************************/

    // Input Related Methods
    void OnLeftDown(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnMotion(wxMouseEvent& event);
    void OnRightDown(wxMouseEvent& event);
    void OnRightUp(wxMouseEvent& event);
    void OnKeyboard(wxKeyEvent& event);
    void CurMove(long& xcoord, // makes the Cascade AIS objects
                 long& ycoord); // alive to mouse movements

    void CtrlZoom(long x1, // Ctrl +Left mouse down
                  long y1, // for zooming
                  long x2,
                  long y2);

    void CtrlRotate(long& xcoord,  // Ctrl+Right mouse down
                    long& ycoord); // for rotating

    void ZoomWnd(); // zooming with rubber-band

    void DrawRectangle(long initX, // general purpose rubber-band display
                       long initY, // during zoom or selection window
                       long finalX,
                       long finalY,
                       bool Draw);

    void DynRotate(); // rotating with the mouse

    void DynPan(); // panning with the mouse

    void ShiftSelectObject(const Standard_Integer flag); // for selecting CAD objects
    // by using Shift a new object
    // will be added to those already
    // selected

    void SelectObject(const Standard_Integer flag); // for selecting CAD objects
                                                    // by dragging a rubber-band
                                                    // window or left mouse selection

    // GUI Related Methods
    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void AddSphere();

    void OnIdle(wxIdleEvent& event);

    /////////////////////End Event-Table Functions////////////////////

private:
    DECLARE_EVENT_TABLE()
};