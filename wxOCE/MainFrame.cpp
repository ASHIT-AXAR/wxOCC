#include "MainFrame.h"

#include <wx/aboutdlg.h>

#include <Aspect_Handle.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Xw_Window.hxx>
#include <V3d_View.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

#include <Graphic3d_NameOfMaterial.hxx>
  
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TCollection_ExtendedString.hxx>
#include <wx/defs.h>
#include <gdk/gdkx.h>
#include <gtk/gtkwidget.h>

//#include <Graphic3d_WNTGraphicDevice.hxx>
#include <V3d_Viewer.hxx>
//#include <WNT_Window.hxx>
#include <Standard_ErrorHandler.hxx>
#include <Geom_Circle.hxx>
#include <AIS_Circle.hxx>
#include <GC_MakeCircle.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <StdPrs_WFDeflectionShape.hxx>
#include <StdPrs_ShadedShape.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Geom_Plane.hxx>
//#include <AIS_Drawer.hxx>
#include <TopoDS.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <StdSelect_BRepSelectionTool.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Lin.hxx>
#include <Geom_Line.hxx>
#include <AIS_Line.hxx>
#include <AIS_Trihedron.hxx>
#include <Geom_Axis2Placement.hxx>
#include <gp.hxx>
#include <UnitsAPI.hxx>
#include <Geom_BSplineSurface.hxx>
#include "BasicGLPane.h"
#include <wx/app.h>
#include <wx/event.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>


MainFrame::MainFrame(wxWindow* parent)
    : MainFrameBaseClass(parent)
{
  TCollection_ExtendedString a3DName ("Visu3D");

    panel = new wxPanel(this, 0,0, 200,200);
    
    
    
 myViewer = Viewer (a3DName.ToExtString(), "", 1000.0, V3d_XposYnegZpos, Standard_True, Standard_True);
 
 myViewer->SetDefaultLights();
 myViewer->SetLightOn();
myContext = new AIS_InteractiveContext (myViewer);  

////code a simple primitive for display
BRepPrimAPI_MakeSphere S(gp_Pnt(100.,300.,200.), 100.);
Handle(AIS_Shape) anAISShape = new AIS_Shape(S.Shape());
myContext->SetColor(anAISShape,Quantity_NOC_AZURE);
myContext->SetMaterial(anAISShape,Graphic3d_NOM_PLASTIC);
myContext->SetDisplayMode(anAISShape,1);

myContext->Display(anAISShape);
    mView->Redraw();
}

MainFrame::~MainFrame()
{
}

// =======================================================================
// function : Viewer
// purpose  :
// =======================================================================
Handle(V3d_Viewer) MainFrame::Viewer (const Standard_ExtString theName,
                                           const Standard_CString theDomain,
                                           const Standard_Real theViewSize,
                                           const V3d_TypeOfOrientation theViewProj,
                                           const Standard_Boolean theComputedMode,
                                           const Standard_Boolean theDefaultComputedMode )
{
  static Handle(OpenGl_GraphicDriver) aGraphicDriver;

 /*Handle(Aspect_DisplayConnection) */aDisplayConnection=
            new Aspect_DisplayConnection();
  if (aGraphicDriver.IsNull())
  {
    aDisplayConnection = new Aspect_DisplayConnection ();

    aGraphicDriver = new OpenGl_GraphicDriver (aDisplayConnection);
  }
Handle(V3d_Viewer)  v = new V3d_Viewer (aGraphicDriver,
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
                         
    mView = v->CreateView();
    
    std::cout<<panel->GetHandle()<<std::endl;
    
    //GdkWindow* draw_window = GTK_PIZZA(this)->bin_window;
    GtkWidget* widget = panel->GetHandle();
      gtk_widget_realize( widget );   // Mandatory. Otherwise, a segfault happens.
      gtk_widget_set_double_buffered(widget, 0);
    Window wid = GDK_WINDOW_XWINDOW( widget->window );
XSync(GDK_WINDOW_XDISPLAY(widget->window), False);
    
   // Window w = gdk_x11_drawable_get_xid(gtk_widget_get_window(panel->GetHandle()));
    wind = new Xw_Window(aDisplayConnection, wid);
    //Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection,"test", 20,20,400,400);
    
    mView->SetWindow(wind);
    //mView->SetWindow(wind);
    mView->Redraw();
    if(!wind->IsMapped()) wind->Map();
    
    panel->Refresh();
        
    return v;
}
int i = 0;
void MainFrame::OnExit(wxCommandEvent& event)
{
    i+=150;
    BRepPrimAPI_MakeSphere S(gp_Pnt(100.,i*2,i), 300.);
Handle(AIS_Shape) anAISShape = new AIS_Shape(S.Shape());
myContext->SetColor(anAISShape,Quantity_NOC_BLUE2);
myContext->SetMaterial(anAISShape,Graphic3d_NOM_DIAMOND);
myContext->SetDisplayMode(anAISShape,1);

myContext->Display(anAISShape);
    mView->FitAll();
    mView->MustBeResized();
    //wind->DoResize();
    mView->Redraw();
    //if(!wind->IsMapped()) wind->Map();
    
    //wxUnusedVar(event);
    //Close();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
     Standard_Integer w = 600;
     Standard_Integer h = 600;
    //
    //
    panel->SetSize(0,0,600,600);
    //mView->Size(w,h);
    //wind->Size(w,h);
   // wind->DoResize();
    //mView->MustBeResized();
    //mView->Redraw();
    
//    wxUnusedVar(event);
//    wxAboutDialogInfo info;
//    info.SetCopyright(_("My MainFrame"));
//    info.SetLicence(_("GPL v2 or later"));
//    info.SetDescription(_("Short description goes here"));
//    ::wxAboutBox(info);
}


void MainFrame::OnShow(wxShowEvent& event)
{
    std::cout<<"Shown!"<<std::endl;
    mView->Redraw();
    if(!wind->IsMapped()) wind->Map();
     
    
    
    
    
//    wxUnusedVar(event);
//    wxAboutDialogInfo info;
//    info.SetCopyright(_("My MainFrame"));
//    info.SetLicence(_("GPL v2 or later"));
//    info.SetDescription(_("Short description goes here"));
//    ::wxAboutBox(info);
}