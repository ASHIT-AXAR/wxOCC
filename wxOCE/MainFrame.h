#ifndef MAINFRAME_H
#define MAINFRAME_H
#include "wxcrafter.h"
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Xw_Window.hxx>

class MainFrame : public MainFrameBaseClass
{
public:
    MainFrame(wxWindow* parent);
    virtual ~MainFrame();
      wxPanel* panel;
      Handle(Xw_Window) wind;
     Handle(Aspect_DisplayConnection) aDisplayConnection;
	Handle(V3d_Viewer) myViewer;
    //! the occ view.
    Handle(V3d_View) mView;
	Handle(AIS_InteractiveContext) myContext;

    void OnShow(wxShowEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
      Handle(V3d_Viewer)             Viewer (const Standard_ExtString theName,
                                         const Standard_CString theDomain,
                                         const Standard_Real theViewSize,
                                         const V3d_TypeOfOrientation theViewProj,
                                         const Standard_Boolean theComputedMode,
                                         const Standard_Boolean theDefaultComputedMode );
};
#endif // MAINFRAME_H
