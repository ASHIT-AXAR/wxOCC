///////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     integrating OpenCascade with the wxAui interface
// Author:      R. T. Roe.
// Created:     2016-03-01
// Copyright:   (C) Copyright 2016, Virtex Edge Design Inc.
// Licence:     Attribution-ShareAlike 4.0 International
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <wx/aboutdlg.h>

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
//#include "../sample.xpm"
#include "../include/vxOCE.h"

//Images

//      View
#include "../img/view/view_shaded.xpm"
#include "../img/view/view_wireframe.xpm"
#include "../img/view/zoom_extend.xpm"
#include "../img/view/zoom_in.xpm"
#include "../img/view/zoom_out.xpm"
#include "../img/view/zoom_selection.xpm"

// -- application --

class MyApp : public wxApp
{
public:
    bool OnInit();
};

DECLARE_APP(MyApp)
IMPLEMENT_APP(MyApp)


// -- frame --

class MainFrame : public wxFrame
{
    enum
    {
        xID_FILE_NEW,
        xID_FILE_OPEN,
        xID_VIEW_SET_SHADED,
        xID_VIEW_SET_WIREFRAME,
        xID_VIEW_ZOOM_IN,
        xID_VIEW_ZOOM_OUT,
        xID_VIEW_ZOOM_FIT,
        xID_VIEW_ZOOM_SELECTION,
        xID_FILE_NEWBOTTLE,
        ID_CreateTree = wxID_HIGHEST+1,
        ID_AddSphere,
        ID_CreateHTML,
        ID_CreateNotebook,
        ID_HTMLContent,
        ID_NotebookContent,
        ID_CreatePerspective,
        ID_CopyPerspectiveCode,
        ID_AllowFloating,
        ID_AllowActivePane,
        ID_TransparentHint,
        ID_VenetianBlindsHint,
        ID_RectangleHint,
        ID_NoHint,
        ID_HintFade,
        ID_NoVenetianFade,
        ID_TransparentDrag,
        ID_NoGradient,
        ID_VerticalGradient,
        ID_HorizontalGradient,
        ID_LiveUpdate,
        ID_AllowToolbarResizing,
        ID_Settings,
        ID_CustomizeToolbar,
        ID_DropDownToolbarItem,
        ID_NotebookNoCloseButton,
        ID_NotebookCloseButton,
        ID_NotebookCloseButtonAll,
        ID_NotebookCloseButtonActive,
        ID_NotebookAllowTabMove,
        ID_NotebookAllowTabExternalMove,
        ID_NotebookAllowTabSplit,
        ID_NotebookWindowList,
        ID_NotebookScrollButtons,
        ID_NotebookTabFixedWidth,
        ID_NotebookArtGloss,
        ID_NotebookArtSimple,
        ID_NotebookAlignTop,
        ID_NotebookAlignBottom,

        ID_SampleItem,

        ID_FirstPerspective = ID_CreatePerspective+1000
    };

public:
    MainFrame(wxWindow* parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);

    ~MainFrame();

    wxAuiDockArt* GetDockArt();
    void DoUpdate();
    int ITEMCOUNT;
private:

    //Images
    wxBitmap xIMG_SET_SHADED;
    wxBitmap xIMG_SET_WIREFRAME;
    wxBitmap xIMG_ZOOM_IN;
    wxBitmap xIMG_ZOOM_OUT;
    wxBitmap xIMG_ZOOM_SELECTION;
    wxBitmap xIMG_ZOOM_EXTENTS;
    
    wxPoint GetStartPosition();
    wxPanel* CreatewxOCECntrl(wxWindow* parent = NULL);
    wxTextCtrl* CreateTextCtrl(const wxString& text = wxEmptyString);
    wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent = NULL);
    wxAuiNotebook* CreateNotebook();
    wxOCEWindow* GetActiveDocument();
    
    wxString GetIntroText();

private:

    void OnSetToShaded(wxCommandEvent& evt);
    void OnSetToWireframe(wxCommandEvent& evt);
    void OnZoomIn(wxCommandEvent& evt);
    void OnZoomOut(wxCommandEvent& evt);
    void OnZoomFit(wxCommandEvent& evt);
    void OnZoomSelection(wxCommandEvent& evt);

    void OnCreateBottle(wxCommandEvent& evt);

    void AddSphere(wxCommandEvent& evt);
    void OnEraseBackground(wxEraseEvent& evt);
    void OnSize(wxSizeEvent& evt);

    void OnCreateTree(wxCommandEvent& evt);
    void OnCreateGrid(wxCommandEvent& evt);
    void OnCreateHTML(wxCommandEvent& evt);
    void OnCreatewxOCECntrl(wxCommandEvent& evt);
    void OnCreateNotebook(wxCommandEvent& evt);
    void OnCreateText(wxCommandEvent& evt);
    void OnCreateSizeReport(wxCommandEvent& evt);
    void OnChangeContentPane(wxCommandEvent& evt);
    void OnDropDownToolbarItem(wxAuiToolBarEvent& evt);
    void OnCreatePerspective(wxCommandEvent& evt);
    void OnCopyPerspectiveCode(wxCommandEvent& evt);
    void OnRestorePerspective(wxCommandEvent& evt);
    void OnSettings(wxCommandEvent& evt);
    void OnCustomizeToolbar(wxCommandEvent& evt);
    void OnAllowNotebookDnD(wxAuiNotebookEvent& evt);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageClosed(wxAuiNotebookEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnInfo(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnTabAlignment(wxCommandEvent &evt);

    void OnGradient(wxCommandEvent& evt);
    void OnToolbarResizing(wxCommandEvent& evt);
    void OnManagerFlag(wxCommandEvent& evt);
    void OnNotebookFlag(wxCommandEvent& evt);
    void OnUpdateUI(wxUpdateUIEvent& evt);

    void OnPaneClose(wxAuiManagerEvent& evt);

private:

    wxAuiManager m_mgr;
    wxAuiNotebook* m_ntbk;
    wxMenu* m_perspectives_menu;
    long m_notebook_style;
    long m_notebook_theme;

    DECLARE_EVENT_TABLE()
};


// -- wxSizeReportCtrl --
// (a utility control that always reports it's client size)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    wxFrame* frame = new MainFrame(NULL,
                                 wxID_ANY,
                                 wxT("wxOCE Sample"),
                                 wxDefaultPosition,
                                 wxSize(800, 600));
    frame->Show();

    return true;
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_ERASE_BACKGROUND(MainFrame::OnEraseBackground)
    EVT_MENU(MainFrame::xID_FILE_NEW, MainFrame::OnCreatewxOCECntrl)
    EVT_MENU(MainFrame::xID_FILE_NEWBOTTLE, MainFrame::OnCreateBottle)
    EVT_MENU(MainFrame::xID_VIEW_SET_SHADED, MainFrame::OnSetToShaded)
    EVT_MENU(MainFrame::xID_VIEW_SET_WIREFRAME, MainFrame::OnSetToWireframe)
    EVT_MENU(MainFrame::xID_VIEW_ZOOM_IN, MainFrame::OnZoomIn)
    EVT_MENU(MainFrame::xID_VIEW_ZOOM_OUT, MainFrame::OnZoomOut)
    EVT_MENU(MainFrame::xID_VIEW_ZOOM_FIT, MainFrame::OnZoomFit)
    EVT_MENU(MainFrame::xID_VIEW_ZOOM_SELECTION, MainFrame::OnZoomSelection)
    EVT_SIZE(MainFrame::OnSize)
    EVT_MENU(MainFrame::ID_CreateHTML, MainFrame::OnCreateHTML)
    EVT_MENU(MainFrame::ID_CreateNotebook, MainFrame::OnCreateNotebook)
    EVT_MENU(ID_AllowFloating, MainFrame::OnManagerFlag)
    EVT_MENU(ID_TransparentHint, MainFrame::OnManagerFlag)
    EVT_MENU(ID_VenetianBlindsHint, MainFrame::OnManagerFlag)
    EVT_MENU(ID_RectangleHint, MainFrame::OnManagerFlag)
    EVT_MENU(ID_NoHint, MainFrame::OnManagerFlag)
    EVT_MENU(ID_HintFade, MainFrame::OnManagerFlag)
    EVT_MENU(ID_NoVenetianFade, MainFrame::OnManagerFlag)
    EVT_MENU(ID_TransparentDrag, MainFrame::OnManagerFlag)
    EVT_MENU(ID_LiveUpdate, MainFrame::OnManagerFlag)
    EVT_MENU(ID_AllowActivePane, MainFrame::OnManagerFlag)
    EVT_MENU(ID_NotebookTabFixedWidth, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookNoCloseButton, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookCloseButton, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookCloseButtonAll, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookCloseButtonActive, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAllowTabMove, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAllowTabExternalMove, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAllowTabSplit, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookScrollButtons, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookWindowList, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookArtGloss, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookArtSimple, MainFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAlignTop,     MainFrame::OnTabAlignment)
    EVT_MENU(ID_NotebookAlignBottom,  MainFrame::OnTabAlignment)
    EVT_MENU(ID_NoGradient, MainFrame::OnGradient)
    EVT_MENU(ID_VerticalGradient, MainFrame::OnGradient)
    EVT_MENU(ID_HorizontalGradient, MainFrame::OnGradient)
    EVT_MENU(ID_AllowToolbarResizing, MainFrame::OnToolbarResizing)
    EVT_MENU(ID_Settings, MainFrame::OnSettings)
    EVT_MENU(ID_CustomizeToolbar, MainFrame::OnCustomizeToolbar)
    EVT_MENU(ID_HTMLContent, MainFrame::OnChangeContentPane)
    EVT_MENU(ID_NotebookContent, MainFrame::OnChangeContentPane)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_INFO, MainFrame::OnInfo)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_UPDATE_UI(ID_NotebookTabFixedWidth, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookNoCloseButton, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookCloseButton, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookCloseButtonAll, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookCloseButtonActive, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookAllowTabMove, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookAllowTabExternalMove, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookAllowTabSplit, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookScrollButtons, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookWindowList, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_AllowFloating, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_TransparentHint, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_VenetianBlindsHint, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_RectangleHint, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NoHint, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_HintFade, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NoVenetianFade, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_TransparentDrag, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_LiveUpdate, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NoGradient, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_VerticalGradient, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_HorizontalGradient, MainFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_AllowToolbarResizing, MainFrame::OnUpdateUI)
    EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, MainFrame::OnAllowNotebookDnD)
END_EVENT_TABLE()


MainFrame::MainFrame(wxWindow* parent,
                 wxWindowID id,
                 const wxString& title,
                 const wxPoint& pos,
                 const wxSize& size,
                 long style)
        : wxFrame(parent, id, title, pos, size, style)
{
    // tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);
    
    //First Load all Images
    xIMG_SET_SHADED = wxBitmap(wxImage(view_shaded_xpm));
    xIMG_SET_WIREFRAME = wxBitmap(wxImage(view_wireframe_xpm));
    xIMG_ZOOM_IN = wxBitmap(wxImage(zoom_in_xpm));
    xIMG_ZOOM_OUT = wxBitmap(wxImage(zoom_out_xpm));
    xIMG_ZOOM_SELECTION = wxBitmap(wxImage(zoom_selection_xpm));
    xIMG_ZOOM_EXTENTS = wxBitmap(wxImage(zoom_extend_xpm));

    // set frame icon
    //SetIcon(wxIcon(sample_xpm));

    // set up default notebook style
    m_notebook_style = wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER;
    m_notebook_theme = 0;



    //  Create menu
    /***************************************************************************/
    wxMenuBar* mb = new wxMenuBar;

    wxMenu* file_menu = new wxMenu;
    file_menu->Append(xID_FILE_NEW, _("Add OCE envr..."));
    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT);

    wxMenu* view_menu = new wxMenu;
    
    //view_menu->Append(ID_AddSphere, _("Add Spheres..."));

    wxMenu* help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT);

    mb->Append(file_menu, _("&File"));
    mb->Append(view_menu, _("&View"));
    mb->Append(help_menu, _("&Help"));

    SetMenuBar(mb);

    CreateStatusBar();
    GetStatusBar()->SetStatusText(_("Ready"));


    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
    SetMinSize(wxSize(400,300));



    //Maximizes the window
    wxTopLevelWindow::Maximize(true);


    //  Create Toolbars
    /***************************************************************************/
    wxAuiToolBar* tb_main = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE);
    tb_main->SetToolBitmapSize(wxSize(48,48));
    tb_main->AddTool(xID_FILE_NEW, wxT("New"), wxArtProvider::GetBitmap(wxART_NEW));
    //tb_main->AddTool(ID_SampleItem+2, wxT("Open"), wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    tb_main->AddSeparator();
    tb_main->AddTool(xID_FILE_NEWBOTTLE, wxT("Craete Bottle"), wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK));
    tb_main->AddSeparator();
    tb_main->AddTool(wxID_INFO, wxT("Info"), wxArtProvider::GetBitmap(wxART_INFORMATION));
    tb_main->AddTool(wxID_ABOUT, wxT("About"), wxArtProvider::GetBitmap(wxART_HELP));
    tb_main->Realize();
    
        wxAuiToolBar* tb_view = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE);
    tb_view->SetToolBitmapSize(wxSize(48,48));
    tb_view->AddTool(xID_VIEW_ZOOM_IN, wxT("Zoom In"), xIMG_ZOOM_IN);
    tb_view->AddTool(xID_VIEW_ZOOM_OUT, wxT("Zoom Out"),  xIMG_ZOOM_OUT);
    tb_view->AddTool(xID_VIEW_ZOOM_FIT, wxT("Zoom To Extents"),  xIMG_ZOOM_EXTENTS);
    tb_view->AddTool(xID_VIEW_ZOOM_SELECTION, wxT("Zoom To Selection"), xIMG_ZOOM_SELECTION);
    tb_view->AddSeparator();
    tb_view->AddTool(xID_VIEW_SET_SHADED, wxT("Set Render to Shaded"), xIMG_SET_SHADED);
    tb_view->AddTool(xID_VIEW_SET_WIREFRAME, wxT("Set Render to Wireframe"), xIMG_SET_WIREFRAME);
    tb_view->AddSeparator();
    \
    tb_view->Realize();

    // Give this pane an icon, too, just for testing.
    int iconSize = m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE);

    // Make it even to use 16 pixel icons with default 17 caption height.
    iconSize &= ~1;

    m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().Name(wxT("notebook_content")).
                  CenterPane().PaneBorder(false));

                  
    m_mgr.AddPane(tb_view, wxAuiPaneInfo().
                  Name(wxT("tb_view")).Caption(wxT("View")).
                  ToolbarPane().Top());

    // add the toolbars to the manager
    m_mgr.AddPane(tb_main, wxAuiPaneInfo().
                  Name(wxT("tb_main")).Caption(wxT("Main")).
                  ToolbarPane().Top());

    // make some default perspectives
    
    int i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
        if (!all_panes.Item(i).IsToolbar())
            all_panes.Item(i).Hide(); 
    m_mgr.GetPane(wxT("tb_main")).Show();
    m_mgr.GetPane(wxT("tb_view")).Show();
    m_mgr.GetPane(wxT("notebook_content")).Show();
    m_mgr.GetPane(wxT("Welcome")).Show();

    // "commit" all changes made to wxAuiManager
    m_mgr.Update();
    
    
    

}

MainFrame::~MainFrame()
{
    m_mgr.UnInit();
}


/***************************************************************************/
//                                                     VIEW MANAGEMENT CLASSES
/***************************************************************************/


void MainFrame::OnSetToShaded(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->DoSetToShaded();    
}

void MainFrame::OnSetToWireframe(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->DoSetToWireframe();    
}


void MainFrame::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->DoZoomIn();    
}

void MainFrame::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->DoZoomOut();    
}

void MainFrame::OnZoomFit(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->DoZoomFit();
}

void MainFrame::OnZoomSelection(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->DoZoomSelection();    
}


void MainFrame::OnCreateBottle(wxCommandEvent& WXUNUSED(event))
{
    GetActiveDocument()->CreateBottle();    
}






    /***************************************************************************/
    //                                                      AUI Management Classes
    /***************************************************************************/


wxString MainFrame::GetIntroText()
{
    const char* text =
        "<html><body>"
        "<h3>Welcome to wxOCE</h3>"
        "<br/><b>Overview</b><br/>"
        "<p>wxOCE is an implementation of OpenCasecade integrated into "
        "the wxWidgets GUI system. There are a number of tutorials and examples "
        "for Qt, MFC etc, but very few that showed it clearly how to get it working "
        "under the wxWidgets system.</p>"
        "<p><b>Features</b></p>"
        "<p>With wxOCE, developers can create CAD programs using the Open Casecade framework."
        "with wxWidgets as the cross platform GUI Backend. This was originallty done as a proof of concept for the I.R.I.S. CAD "
        "system devloped by Virtex Edge Design. This sample app has been created in hopes to help others " 
        "who are trying to do the same as us.</p>"
        "<p><b>Versions</b></p>"
        "<p><b>V. 0.2.0</b></p>"
        "<p>wxOCE 0.2.0, extended and improved with deeper wxWidgets Integration"
        "<ul>"
        "<li>Integrated with the wxAUI System for better useability.</li>"
        "<li>Added simple Camera Operations for Interation with OpenCascade.</li>"
        "<li>Lots of bug fixes</li>"
        "</ul>"
        "<p><b>V. 0.1.0</b></p>"
        "<p>The first working version of wxOCE 0.1.0"
        "<ul>"
        "<li>Able to create and parent the Open Cascade X11 window into a wxPanel.</li>"
        "</ul>"
        "<p>See README.txt for more information.</p>"
         "<p><b>wxAUI Demo\n</b></p>"
        "\n\n<p>An advanced window management library for wxWidgets\n(c) Copyright 2005-2006, Kirix Corporation</p>"
        "</body></html>";

    return wxString::FromAscii(text);
}

wxPanel* MainFrame::CreatewxOCECntrl(wxWindow* parent)
{
      if (!parent)
        parent = this;
        
    wxOCEWindow* ctrl = new wxOCEWindow( parent, 
        wxPoint(0, 0),
        wxSize(-1, -1),
        0);
    
    return ctrl;
}


wxHtmlWindow* MainFrame::CreateHTMLCtrl(wxWindow* parent)
{
    if (!parent)
        parent = this;

    wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(400,300));
    ctrl->SetPage(GetIntroText());
    return ctrl;
}


wxAuiNotebook* MainFrame::CreateNotebook()
{
   // create the notebook off-window to avoid flicker
   wxSize client_size = GetClientSize();

    m_ntbk = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    m_notebook_style);
                                    
    //Using Freeze/Thaw with OCE causes the controls too not render properly.
   //ctrl->Freeze();

   wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));

ITEMCOUNT++;
    m_ntbk->AddPage(CreatewxOCECntrl(m_ntbk), wxString::Format(wxT("newfile%i.wxoce"), ITEMCOUNT) , false, page_bmp);
   
   m_mgr.AddPane(CreateHTMLCtrl(), wxAuiPaneInfo().
                  Name(wxT("Welcome")).Caption(wxT("Welcome to wxOCE")).
                    MinSize(wxSize(350,100)).                  
                  Right().CloseButton(true));
                  
                  
    m_mgr.Update();
   
   //ctrl->Thaw();
   return m_ntbk;
}
void MainFrame::OnCreatewxOCECntrl(wxCommandEvent& WXUNUSED(event))
{
   wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));

ITEMCOUNT++;
    m_ntbk->AddPage(CreatewxOCECntrl(m_ntbk),wxString::Format(wxT("newfile%i.wxoce"), ITEMCOUNT), false, page_bmp);
    m_mgr.Update();
}

void MainFrame::OnCreateHTML(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateHTMLCtrl(), wxAuiPaneInfo().
                  Caption(wxT("HTML Control")).
                  Float().FloatingPosition(GetStartPosition()).
                  FloatingSize(wxSize(300,200)));
    m_mgr.Update();
}

void MainFrame::OnCreateNotebook(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().
                  Caption(wxT("Notebook")).
                  Float().FloatingPosition(GetStartPosition()).
                  //FloatingSize(300,200).
                  CloseButton(true).MaximizeButton(true));
    m_mgr.Update();
}


wxOCEWindow* MainFrame::GetActiveDocument()
{
    if(m_ntbk->GetPageCount()>0)
    {
        //If it's an OpenGL Pane, then it's a child of the pane
        if (m_ntbk->GetPage(m_ntbk->GetSelection())->IsKindOf(CLASSINFO(wxOCEWindow)))
        {
            return  (wxOCEWindow*)(m_ntbk->GetPage(m_ntbk->GetSelection()));
        }       
    }
    return new wxOCEWindow(this, this->GetPosition(), this->GetSize(), 1);
}











void MainFrame::OnChangeContentPane(wxCommandEvent& evt)
{
    m_mgr.GetPane(wxT("html_content")).Show(evt.GetId() == ID_HTMLContent);
    m_mgr.GetPane(wxT("notebook_content")).Show(evt.GetId() == ID_NotebookContent);
    m_mgr.Update();
}

wxAuiDockArt* MainFrame::GetDockArt()
{
    return m_mgr.GetArtProvider();
}

void MainFrame::DoUpdate()
{
    m_mgr.Update();
}


void MainFrame::OnEraseBackground(wxEraseEvent& event)
{
    event.Skip();
}

void MainFrame::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

void MainFrame::OnSettings(wxCommandEvent& WXUNUSED(evt))
{
    // show the settings pane, and float it
    wxAuiPaneInfo& floating_pane = m_mgr.GetPane(wxT("settings")).Float().Show();

    if (floating_pane.floating_pos == wxDefaultPosition)
        floating_pane.FloatingPosition(GetStartPosition());

    m_mgr.Update();
}

void MainFrame::OnCustomizeToolbar(wxCommandEvent& WXUNUSED(evt))
{
    wxMessageBox(_("Customize Toolbar clicked"));
}

void MainFrame::OnGradient(wxCommandEvent& event)
{
    int gradient = 0;

    switch (event.GetId())
    {
        case ID_NoGradient:         gradient = wxAUI_GRADIENT_NONE; break;
        case ID_VerticalGradient:   gradient = wxAUI_GRADIENT_VERTICAL; break;
        case ID_HorizontalGradient: gradient = wxAUI_GRADIENT_HORIZONTAL; break;
    }

    m_mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, gradient);
    m_mgr.Update();
}

void MainFrame::OnToolbarResizing(wxCommandEvent& WXUNUSED(evt))
{
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    const size_t count = all_panes.GetCount();
    for (size_t i = 0; i < count; ++i)
    {
        wxAuiToolBar* toolbar = wxDynamicCast(all_panes[i].window, wxAuiToolBar);
        if (toolbar)
        {
            all_panes[i].Resizable(!all_panes[i].IsResizable());
        }
    }

    m_mgr.Update();
}

void MainFrame::OnManagerFlag(wxCommandEvent& event)
{
    unsigned int flag = 0;

#if !defined(__WXMSW__) && !defined(__WXMAC__) && !defined(__WXGTK__)
    if (event.GetId() == ID_TransparentDrag ||
        event.GetId() == ID_TransparentHint ||
        event.GetId() == ID_HintFade)
    {
        wxMessageBox(wxT("This option is presently only available on wxGTK, wxMSW and wxMac"));
        return;
    }
#endif

    int id = event.GetId();

    if (id == ID_TransparentHint ||
        id == ID_VenetianBlindsHint ||
        id == ID_RectangleHint ||
        id == ID_NoHint)
    {
        unsigned int flags = m_mgr.GetFlags();
        flags &= ~wxAUI_MGR_TRANSPARENT_HINT;
        flags &= ~wxAUI_MGR_VENETIAN_BLINDS_HINT;
        flags &= ~wxAUI_MGR_RECTANGLE_HINT;
        m_mgr.SetFlags(flags);
    }

    switch (id)
    {
        case ID_AllowFloating: flag = wxAUI_MGR_ALLOW_FLOATING; break;
        case ID_TransparentDrag: flag = wxAUI_MGR_TRANSPARENT_DRAG; break;
        case ID_HintFade: flag = wxAUI_MGR_HINT_FADE; break;
        case ID_NoVenetianFade: flag = wxAUI_MGR_NO_VENETIAN_BLINDS_FADE; break;
        case ID_AllowActivePane: flag = wxAUI_MGR_ALLOW_ACTIVE_PANE; break;
        case ID_TransparentHint: flag = wxAUI_MGR_TRANSPARENT_HINT; break;
        case ID_VenetianBlindsHint: flag = wxAUI_MGR_VENETIAN_BLINDS_HINT; break;
        case ID_RectangleHint: flag = wxAUI_MGR_RECTANGLE_HINT; break;
        case ID_LiveUpdate: flag = wxAUI_MGR_LIVE_RESIZE; break;
    }

    if (flag)
    {
        m_mgr.SetFlags(m_mgr.GetFlags() ^ flag);
    }

    m_mgr.Update();
}


void MainFrame::OnNotebookFlag(wxCommandEvent& event)
{
    int id = event.GetId();

    if (id == ID_NotebookNoCloseButton ||
        id == ID_NotebookCloseButton ||
        id == ID_NotebookCloseButtonAll ||
        id == ID_NotebookCloseButtonActive)
    {
        m_notebook_style &= ~(wxAUI_NB_CLOSE_BUTTON |
                              wxAUI_NB_CLOSE_ON_ACTIVE_TAB |
                              wxAUI_NB_CLOSE_ON_ALL_TABS);

        switch (id)
        {
            case ID_NotebookNoCloseButton: break;
            case ID_NotebookCloseButton: m_notebook_style |= wxAUI_NB_CLOSE_BUTTON; break;
            case ID_NotebookCloseButtonAll: m_notebook_style |= wxAUI_NB_CLOSE_ON_ALL_TABS; break;
            case ID_NotebookCloseButtonActive: m_notebook_style |= wxAUI_NB_CLOSE_ON_ACTIVE_TAB; break;
        }
    }

    if (id == ID_NotebookAllowTabMove)
    {
        m_notebook_style ^= wxAUI_NB_TAB_MOVE;
    }
    if (id == ID_NotebookAllowTabExternalMove)
    {
        m_notebook_style ^= wxAUI_NB_TAB_EXTERNAL_MOVE;
    }
     else if (id == ID_NotebookAllowTabSplit)
    {
        m_notebook_style ^= wxAUI_NB_TAB_SPLIT;
    }
     else if (id == ID_NotebookWindowList)
    {
        m_notebook_style ^= wxAUI_NB_WINDOWLIST_BUTTON;
    }
     else if (id == ID_NotebookScrollButtons)
    {
        m_notebook_style ^= wxAUI_NB_SCROLL_BUTTONS;
    }
     else if (id == ID_NotebookTabFixedWidth)
    {
        m_notebook_style ^= wxAUI_NB_TAB_FIXED_WIDTH;
    }


    size_t i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
    {
        wxAuiPaneInfo& pane = all_panes.Item(i);
        if (pane.window->IsKindOf(CLASSINFO(wxAuiNotebook)))
        {
            wxAuiNotebook* nb = (wxAuiNotebook*)pane.window;

            if (id == ID_NotebookArtGloss)
            {
                nb->SetArtProvider(new wxAuiDefaultTabArt);
                m_notebook_theme = 0;
            }
             else if (id == ID_NotebookArtSimple)
            {
                nb->SetArtProvider(new wxAuiSimpleTabArt);
                m_notebook_theme = 1;
            }


            nb->SetWindowStyleFlag(m_notebook_style);
            nb->Refresh();
        }
    }


}


void MainFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
    unsigned int flags = m_mgr.GetFlags();

    switch (event.GetId())
    {
        case ID_NoGradient:
            event.Check(m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRADIENT_TYPE) == wxAUI_GRADIENT_NONE);
            break;
        case ID_VerticalGradient:
            event.Check(m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRADIENT_TYPE) == wxAUI_GRADIENT_VERTICAL);
            break;
        case ID_HorizontalGradient:
            event.Check(m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRADIENT_TYPE) == wxAUI_GRADIENT_HORIZONTAL);
            break;
        case ID_AllowToolbarResizing:
        {
            wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
            const size_t count = all_panes.GetCount();
            for (size_t i = 0; i < count; ++i)
            {
                wxAuiToolBar* toolbar = wxDynamicCast(all_panes[i].window, wxAuiToolBar);
                if (toolbar)
                {
                    event.Check(all_panes[i].IsResizable());
                    break;
                }
            }
            break;
        }
        case ID_AllowFloating:
            event.Check((flags & wxAUI_MGR_ALLOW_FLOATING) != 0);
            break;
        case ID_TransparentDrag:
            event.Check((flags & wxAUI_MGR_TRANSPARENT_DRAG) != 0);
            break;
        case ID_TransparentHint:
            event.Check((flags & wxAUI_MGR_TRANSPARENT_HINT) != 0);
            break;
        case ID_LiveUpdate:
            event.Check((flags & wxAUI_MGR_LIVE_RESIZE) != 0);
            break;
        case ID_VenetianBlindsHint:
            event.Check((flags & wxAUI_MGR_VENETIAN_BLINDS_HINT) != 0);
            break;
        case ID_RectangleHint:
            event.Check((flags & wxAUI_MGR_RECTANGLE_HINT) != 0);
            break;
        case ID_NoHint:
            event.Check(((wxAUI_MGR_TRANSPARENT_HINT |
                          wxAUI_MGR_VENETIAN_BLINDS_HINT |
                          wxAUI_MGR_RECTANGLE_HINT) & flags) == 0);
            break;
        case ID_HintFade:
            event.Check((flags & wxAUI_MGR_HINT_FADE) != 0);
            break;
        case ID_NoVenetianFade:
            event.Check((flags & wxAUI_MGR_NO_VENETIAN_BLINDS_FADE) != 0);
            break;

        case ID_NotebookNoCloseButton:
            event.Check((m_notebook_style & (wxAUI_NB_CLOSE_BUTTON|wxAUI_NB_CLOSE_ON_ALL_TABS|wxAUI_NB_CLOSE_ON_ACTIVE_TAB)) != 0);
            break;
        case ID_NotebookCloseButton:
            event.Check((m_notebook_style & wxAUI_NB_CLOSE_BUTTON) != 0);
            break;
        case ID_NotebookCloseButtonAll:
            event.Check((m_notebook_style & wxAUI_NB_CLOSE_ON_ALL_TABS) != 0);
            break;
        case ID_NotebookCloseButtonActive:
            event.Check((m_notebook_style & wxAUI_NB_CLOSE_ON_ACTIVE_TAB) != 0);
            break;
        case ID_NotebookAllowTabSplit:
            event.Check((m_notebook_style & wxAUI_NB_TAB_SPLIT) != 0);
            break;
        case ID_NotebookAllowTabMove:
            event.Check((m_notebook_style & wxAUI_NB_TAB_MOVE) != 0);
            break;
        case ID_NotebookAllowTabExternalMove:
            event.Check((m_notebook_style & wxAUI_NB_TAB_EXTERNAL_MOVE) != 0);
            break;
        case ID_NotebookScrollButtons:
            event.Check((m_notebook_style & wxAUI_NB_SCROLL_BUTTONS) != 0);
            break;
        case ID_NotebookWindowList:
            event.Check((m_notebook_style & wxAUI_NB_WINDOWLIST_BUTTON) != 0);
            break;
        case ID_NotebookTabFixedWidth:
            event.Check((m_notebook_style & wxAUI_NB_TAB_FIXED_WIDTH) != 0);
            break;
        case ID_NotebookArtGloss:
            event.Check(m_notebook_style == 0);
            break;
        case ID_NotebookArtSimple:
            event.Check(m_notebook_style == 1);
            break;

    }
}

void MainFrame::OnAllowNotebookDnD(wxAuiNotebookEvent& evt)
{
    // for the purpose of this test application, explicitly
    // allow all noteboko drag and drop events
    evt.Allow();
}

wxPoint MainFrame::GetStartPosition()
{
    static int x = 0;
    x += 20;
    wxPoint pt = ClientToScreen(wxPoint(0,0));
    return wxPoint(pt.x + x, pt.y + x);
}


void MainFrame::OnTabAlignment(wxCommandEvent &evt)
{
    size_t i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
    {
        wxAuiPaneInfo& pane = all_panes.Item(i);
        if (pane.window->IsKindOf(CLASSINFO(wxAuiNotebook)))
        {
            wxAuiNotebook* nb = (wxAuiNotebook*)pane.window;

            long style = nb->GetWindowStyleFlag();
            style &= ~(wxAUI_NB_TOP | wxAUI_NB_BOTTOM);
            if (evt.GetId() == ID_NotebookAlignTop)
                style |= wxAUI_NB_TOP;
            else if (evt.GetId() == ID_NotebookAlignBottom)
                style |= wxAUI_NB_BOTTOM;
            nb->SetWindowStyleFlag(style);

            nb->Refresh();
        }
    }
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}


void MainFrame::OnInfo(wxCommandEvent& WXUNUSED(event))
{
    if(m_mgr.GetPane(wxT("Welcome")).IsShown())   
        m_mgr.GetPane(wxT("Welcome")).Hide();
    else        
        m_mgr.GetPane(wxT("Welcome")).Show();
        
        m_mgr.Update();
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{    
        //wxUnusedVar(event);
    wxAboutDialogInfo info;
    info.SetName("wxOCE");
    info.SetCopyright(_("wxOCE (C) Virtex Edge Design"));
    info.AddDeveloper("R. T. Roe (rtrawr)");
    info.SetVersion(" - v. 0.3.0");
    info.SetWebSite("https://github.com/rtrawr/wxOCE");
    info.SetLicence(_("Attribution-ShareAlike 4.0 International.\n\nSee Github for Full License."));
    info.SetDescription(_("wxOCE is an implementation of OpenCasecade integrated into "
        "the wxWidgets GUI system. \n\n\n There are a number of tutorials and examples "
        "for Qt, MFC etc, but very few that showed it clearly how to get it working "
        "under the wxWidgets system. This sample app has been created in hopes to help others " 
        "who are trying to do the same as us."));
    ::wxAboutBox(info);
}