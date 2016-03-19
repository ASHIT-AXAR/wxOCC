#include "wx/defs.h"
#include "wx/control.h"
#include "wx/sizer.h"
#include "wx/pen.h"
#include "wx/wx.h"
#include "wx/aui/aui.h"
#include <wx/artprov.h>

class vxCstmArtPrvdr_AUIToolbar  : public wxAuiToolBarArt
{

public:

    vxCstmArtPrvdr_AUIToolbar();
    virtual ~vxCstmArtPrvdr_AUIToolbar();

    //Added Methods
    void ResetToDefault();
    void SetToolBarBaseColour(wxColour);
    void SetButtonHighliteColour(wxColour);

    //Original Code from axAuiDefaultToolbarArt
    virtual wxAuiToolBarArt* Clone();
    virtual void SetFlags(unsigned int flags);
    virtual unsigned int GetFlags();
    virtual void SetFont(const wxFont& font);
    virtual wxFont GetFont();
    virtual void SetTextOrientation(int orientation);
    virtual int GetTextOrientation();


    virtual void DrawBackground(
        wxDC& dc,
        wxWindow* wnd,
        const wxRect& rect);

    virtual void DrawPlainBackground(wxDC& dc,
                                     wxWindow* wnd,
                                     const wxRect& rect);

    virtual void DrawLabel(
        wxDC& dc,
        wxWindow* wnd,
        const wxAuiToolBarItem& item,
        const wxRect& rect);

    virtual void DrawButton(
        wxDC& dc,
        wxWindow* wnd,
        const wxAuiToolBarItem& item,
        const wxRect& rect);

    virtual void DrawDropDownButton(
        wxDC& dc,
        wxWindow* wnd,
        const wxAuiToolBarItem& item,
        const wxRect& rect);

    virtual void DrawControlLabel(
        wxDC& dc,
        wxWindow* wnd,
        const wxAuiToolBarItem& item,
        const wxRect& rect);

    virtual void DrawSeparator(
        wxDC& dc,
        wxWindow* wnd,
        const wxRect& rect);

    virtual void DrawGripper(
        wxDC& dc,
        wxWindow* wnd,
        const wxRect& rect);

    virtual void DrawOverflowButton(
        wxDC& dc,
        wxWindow* wnd,
        const wxRect& rect,
        int state);

    virtual wxSize GetLabelSize(
        wxDC& dc,
        wxWindow* wnd,
        const wxAuiToolBarItem& item);

    virtual wxSize GetToolSize(
        wxDC& dc,
        wxWindow* wnd,
        const wxAuiToolBarItem& item);

    virtual int GetElementSize(int element);
    virtual void SetElementSize(int elementId, int size);

    virtual int ShowDropDown(wxWindow* wnd,
                             const wxAuiToolBarItemArray& items);

protected:

    wxBitmap m_buttonDropDownBmp;
    wxBitmap m_disabledButtonDropDownBmp;
    wxBitmap m_overflowBmp;
    wxBitmap m_disabledOverflowBmp;
    wxColour m_baseColour;
    wxColour m_highlightColour;
    wxFont m_font;
    unsigned int m_flags;
    int m_textOrientation;

    wxPen m_gripperPen1;
    wxPen m_gripperPen2;
    wxPen m_gripperPen3;

    int m_separatorSize;
    int m_gripperSize;
    int m_overflowSize;
};
