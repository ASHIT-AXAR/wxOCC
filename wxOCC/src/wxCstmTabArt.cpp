
// -- wxCstmTabArt class implementation --

#include "../include/wxCstmTabArt.h"
#include <wx/aui/dockart.h>
#include <wx/menu.h>
#include <wx/dcclient.h>




inline bool IsDarkColour(const wxColour& c)
{
    return (c.Red() + c.Green() + c.Blue()) * c.Alpha() * 2 < 3 * 255 * 255;
}

// wxAuiBitmapFromBits() is a utility function that creates a
// masked bitmap from raw bits (XBM format)
wxBitmap wxAuiBitmapFromBits(const unsigned char bits[], int w, int h,
                             const wxColour& color)
{
    wxImage img = wxBitmap((const char*)bits, w, h).ConvertToImage();
    img.Replace(0,0,0,123,123,123);
    img.Replace(255,255,255,color.Red(),color.Green(),color.Blue());
    img.SetMaskColour(123,123,123);
    return wxBitmap(img);
}


wxString wxAuiChopText(wxDC& dc, const wxString& text, int max_size)
{
    wxCoord x,y;

    // first check if the text fits with no problems
    dc.GetTextExtent(text, &x, &y);
    if (x <= max_size)
        return text;

    size_t i, len = text.Length();
    size_t last_good_length = 0;
    for (i = 0; i < len; ++i)
    {
        wxString s = text.Left(i);
        s += wxT("...");

        dc.GetTextExtent(s, &x, &y);
        if (x > max_size)
            break;

        last_good_length = i;
    }

    wxString ret = text.Left(last_good_length);
    ret += wxT("...");
    return ret;
}

static void DrawButtons(wxDC& dc,
                        const wxRect& _rect,
                        const wxBitmap& bmp,
                        const wxColour& bkcolour,
                        int button_state)
{
    wxRect rect = _rect;

    if (button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        rect.x++;
        rect.y++;
    }

    if (button_state == wxAUI_BUTTON_STATE_HOVER ||
        button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        dc.SetBrush(wxBrush(bkcolour.ChangeLightness(120)));
        dc.SetPen(wxPen(bkcolour.ChangeLightness(75)));

        // draw the background behind the button
        dc.DrawRectangle(rect.x, rect.y, 15, 15);
    }

    // draw the button itself
    dc.DrawBitmap(bmp, rect.x, rect.y, true);
}

static void IndentPressedBitmap(wxRect* rect, int button_state)
{
    if (button_state == wxAUI_BUTTON_STATE_PRESSED)
    {
        rect->x++;
        rect->y++;
    }
}


#if defined( __WXMAC__ )
 static const unsigned char close_bits[]={
     0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFE, 0x03, 0xF8, 0x01, 0xF0, 0x19, 0xF3,
     0xB8, 0xE3, 0xF0, 0xE1, 0xE0, 0xE0, 0xF0, 0xE1, 0xB8, 0xE3, 0x19, 0xF3,
     0x01, 0xF0, 0x03, 0xF8, 0x0F, 0xFE, 0xFF, 0xFF };
#elif defined( __WXGTK__)
 static const unsigned char close_bits[]={
     0xff, 0xff, 0xff, 0xff, 0x07, 0xf0, 0xfb, 0xef, 0xdb, 0xed, 0x8b, 0xe8,
     0x1b, 0xec, 0x3b, 0xee, 0x1b, 0xec, 0x8b, 0xe8, 0xdb, 0xed, 0xfb, 0xef,
     0x07, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
#else
 static const unsigned char close_bits[]={
     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xf3, 0xcf, 0xf9,
     0x9f, 0xfc, 0x3f, 0xfe, 0x3f, 0xfe, 0x9f, 0xfc, 0xcf, 0xf9, 0xe7, 0xf3,
     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
#endif

static const unsigned char left_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xfe, 0x3f, 0xfe,
   0x1f, 0xfe, 0x0f, 0xfe, 0x1f, 0xfe, 0x3f, 0xfe, 0x7f, 0xfe, 0xff, 0xfe,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static const unsigned char right_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0x9f, 0xff, 0x1f, 0xff,
   0x1f, 0xfe, 0x1f, 0xfc, 0x1f, 0xfe, 0x1f, 0xff, 0x9f, 0xff, 0xdf, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static const unsigned char list_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x0f, 0xf8, 0xff, 0xff, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


// -- GUI helper classes and functions --

class wxAuiCommandCapture : public wxEvtHandler
{
public:

    wxAuiCommandCapture() { m_lastId = 0; }
    int GetCommandId() const { return m_lastId; }

    bool ProcessEvent(wxEvent& evt)
    {
        if (evt.GetEventType() == wxEVT_MENU)
        {
            m_lastId = evt.GetId();
            return true;
        }

        if (GetNextHandler())
            return GetNextHandler()->ProcessEvent(evt);

        return false;
    }

private:
    int m_lastId;
};





wxCstmTabArt::wxCstmTabArt()
{
    m_normalFont = *wxNORMAL_FONT;
    m_selectedFont = *wxNORMAL_FONT;
    m_selectedFont.SetWeight(wxFONTWEIGHT_BOLD);
    m_measuringFont = m_selectedFont;

    m_flags = 0;
    m_fixedTabWidth = 100;

    wxColour baseColour = wxColour(60,60,60) ;//wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);

    wxColour backgroundColour = baseColour;
    wxColour normaltabColour = wxColour(70,70,70) ;
    //wxColour selectedtabColour = *wxWHITE;
    wxColour selectedtabColour(45, 45, 45);

    m_normalTextColour = IsDarkColour(baseColour) ? wxColour(175,175,175) : *wxBLACK;
    m_selectedTextColour = IsDarkColour(selectedtabColour) ? wxColour(220,220,220) : *wxBLACK;

    m_bkBrush = wxBrush(backgroundColour);
    m_normalBkBrush = wxBrush(normaltabColour);
    m_normalBkPen = wxPen(normaltabColour);
    m_selectedBkBrush = wxBrush(selectedtabColour);
    m_selectedBkPen = wxPen(selectedtabColour);

    m_activeCloseBmp = wxAuiBitmapFromBits(close_bits, 16, 16, *wxBLACK);
    m_disabledCloseBmp = wxAuiBitmapFromBits(close_bits, 16, 16, wxColour(128,128,128));

    m_activeLeftBmp = wxAuiBitmapFromBits(left_bits, 16, 16, *wxBLACK);
    m_disabledLeftBmp = wxAuiBitmapFromBits(left_bits, 16, 16, wxColour(128,128,128));

    m_activeRightBmp = wxAuiBitmapFromBits(right_bits, 16, 16, *wxBLACK);
    m_disabledRightBmp = wxAuiBitmapFromBits(right_bits, 16, 16, wxColour(128,128,128));

    m_activeWindowListBmp = wxAuiBitmapFromBits(list_bits, 16, 16, *wxBLACK);
    m_disabledWindowListBmp = wxAuiBitmapFromBits(list_bits, 16, 16, wxColour(128,128,128));

}

wxCstmTabArt::~wxCstmTabArt()
{
}

wxAuiTabArt* wxCstmTabArt::Clone()
{
    return new wxCstmTabArt(*this);
}

void wxCstmTabArt::SetFlags(unsigned int flags)
{
    m_flags = flags;
}

void wxCstmTabArt::SetSizingInfo(const wxSize& tab_ctrl_size,
                                      size_t tab_count)
{
    m_fixedTabWidth = 100;

    int tot_width = (int)tab_ctrl_size.x - GetIndentSize() - 4;

    if (m_flags & wxAUI_NB_CLOSE_BUTTON)
        tot_width -= m_activeCloseBmp.GetWidth();
    if (m_flags & wxAUI_NB_WINDOWLIST_BUTTON)
        tot_width -= m_activeWindowListBmp.GetWidth();

    if (tab_count > 0)
    {
        m_fixedTabWidth = tot_width/(int)tab_count;
    }


    if (m_fixedTabWidth < 100)
        m_fixedTabWidth = 100;

    if (m_fixedTabWidth > tot_width/2)
        m_fixedTabWidth = tot_width/2;

    if (m_fixedTabWidth > 220)
        m_fixedTabWidth = 220;
}

void wxCstmTabArt::SetColour(const wxColour& colour)
{
    m_bkBrush = wxBrush(colour);
    m_normalBkBrush = wxBrush(colour);
    m_normalBkPen = wxPen(colour);
}

void wxCstmTabArt::SetActiveColour(const wxColour& colour)
{
    m_selectedBkBrush = wxBrush(colour);
    m_selectedBkPen = wxPen(colour);
}

void wxCstmTabArt::DrawBorder(wxDC& dc, wxWindow* wnd, const wxRect& rect)
{
    int i, border_width = GetBorderWidth(wnd);

    wxRect theRect(rect);
    for (i = 0; i < border_width; ++i)
    {
        dc.DrawRectangle(theRect.x, theRect.y, theRect.width, theRect.height);
        theRect.Deflate(1);
    }
}

void wxCstmTabArt::DrawBackground(wxDC& dc,
                                       wxWindow* WXUNUSED(wnd),
                                       const wxRect& rect)
{
    // draw background
    dc.SetBrush(m_bkBrush);
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.DrawRectangle(-1, -1, rect.GetWidth()+2, rect.GetHeight()+2);

    // draw base line
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(0, rect.GetHeight()-1, rect.GetWidth(), rect.GetHeight()-1);
}


// DrawTab() draws an individual tab.
//
// dc       - output dc
// in_rect  - rectangle the tab should be confined to
// caption  - tab's caption
// active   - whether or not the tab is active
// out_rect - actual output rectangle
// x_extent - the advance x; where the next tab should start

void wxCstmTabArt::DrawTab(wxDC& dc,
                                wxWindow* wnd,
                                const wxAuiNotebookPage& page,
                                const wxRect& in_rect,
                                int close_button_state,
                                wxRect* out_tab_rect,
                                wxRect* out_button_rect,
                                int* x_extent)
{
    wxCoord normal_textx, normal_texty;
    wxCoord selected_textx, selected_texty;
    wxCoord textx, texty;

    // if the caption is empty, measure some temporary text
    wxString caption = page.caption;
    if (caption.empty())
        caption = wxT("Xj");

    dc.SetFont(m_selectedFont);
    dc.GetTextExtent(caption, &selected_textx, &selected_texty);

    dc.SetFont(m_normalFont);
    dc.GetTextExtent(caption, &normal_textx, &normal_texty);

    // figure out the size of the tab
    wxSize tab_size = GetTabSize(dc,
                                 wnd,
                                 page.caption,
                                 page.bitmap,
                                 page.active,
                                 close_button_state,
                                 x_extent);

    wxCoord tab_height = tab_size.y;
    wxCoord tab_width = tab_size.x;
    wxCoord tab_x = in_rect.x;
    wxCoord tab_y = in_rect.y + in_rect.height - tab_height;

    caption = page.caption;

    // select pen, brush and font for the tab to be drawn

    if (page.active)
    {
        dc.SetPen(m_selectedBkPen);
        dc.SetBrush(m_selectedBkBrush);
        dc.SetFont(m_selectedFont);
        textx = selected_textx;
        texty = selected_texty;
    }
    else
    {
        dc.SetPen(m_normalBkPen);
        dc.SetBrush(m_normalBkBrush);
        dc.SetFont(m_normalFont);
        textx = normal_textx;
        texty = normal_texty;
    }

/*
    wxPoint points[7];
    points[0].x = tab_x;
    points[0].y = tab_y + tab_height - 1;
    points[1].x = tab_x + tab_height - 3;
    points[1].y = tab_y + 2;
    points[2].x = tab_x + tab_height + 3;
    points[2].y = tab_y;
    points[3].x = tab_x + tab_width - 2;
    points[3].y = tab_y;
    points[4].x = tab_x + tab_width;
    points[4].y = tab_y + 2;
    points[5].x = tab_x + tab_width;
    points[5].y = tab_y + tab_height - 1;
    points[6] = points[0];
 */

    // -- draw line --

    wxPoint points[7];
    points[0].x = tab_x;
    points[0].y = tab_y + tab_height - 1;
    
    points[1].x = tab_x + tab_height - 3;
    points[1].y = tab_y + 2;
    
    points[2].x = tab_x + tab_height + 3;
    points[2].y = tab_y;
    
    points[3].x = tab_x + tab_width - 3;
    points[3].y = tab_y;
    
    points[4].x = tab_x + tab_width + 3;
    points[4].y = tab_y + 2;
    
    points[5].x = tab_x + tab_width + tab_height;
    points[5].y = tab_y + tab_height - 1;
    
    points[6] = points[0];

    dc.SetClippingRegion(in_rect);

    dc.DrawPolygon(WXSIZEOF(points) - 1, points);

    //dc.SetPen(*wxGREY_PEN);
    
    
        if (page.active)
    {
    dc.SetPen(*wxBLACK_PEN);
    }
    else
    {
    dc.SetPen(*wxBLACK_PEN);
    }

    //dc.DrawLines(active ? WXSIZEOF(points) - 1 : WXSIZEOF(points), points);
    dc.DrawLines(WXSIZEOF(points), points);


    int text_offset;

    int close_button_width = 0;
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        close_button_width = m_activeCloseBmp.GetWidth();
        text_offset = tab_x + (tab_height/2) + ((tab_width-close_button_width)/2) - (textx/2);
    }
    else
    {
        text_offset = tab_x + (tab_height/3) + (tab_width/2) - (textx/2);
    }

    // set minimum text offset
    if (text_offset < tab_x + tab_height)
        text_offset = tab_x + tab_height;

    // chop text if necessary
    wxString draw_text = wxAuiChopText(dc,
                          caption,
                          tab_width - (text_offset-tab_x) - close_button_width);

    // draw tab text
    dc.SetTextForeground(page.active ? m_selectedTextColour : m_normalTextColour);
    dc.DrawText(draw_text,
                 text_offset,
                 (tab_y + tab_height)/2 - (texty/2) + 1);


    // draw focus rectangle
    if (page.active && (wnd->FindFocus() == wnd))
    {
        wxRect focusRect(text_offset, ((tab_y + tab_height)/2 - (texty/2) + 1),
            selected_textx, selected_texty);

        focusRect.Inflate(2, 2);

        wxRendererNative::Get().DrawFocusRect(wnd, dc, focusRect, 0);
    }

    // draw close button if necessary
    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
    {
        wxBitmap bmp;
        if (page.active)
            bmp = m_activeCloseBmp;
        else
            bmp = m_disabledCloseBmp;

        wxRect rect(tab_x + tab_width - close_button_width - 1,
                    tab_y + (tab_height/2) - (bmp.GetHeight()/2) + 1,
                    close_button_width,
                    tab_height - 1);
        DrawButtons(dc, rect, bmp, *wxWHITE, close_button_state);

        *out_button_rect = rect;
    }


    *out_tab_rect = wxRect(tab_x, tab_y, tab_width, tab_height);

    dc.DestroyClippingRegion();
}

int wxCstmTabArt::GetIndentSize()
{
    return 0;
}

int wxCstmTabArt::GetBorderWidth(wxWindow* wnd)
{
    wxAuiManager* mgr = wxAuiManager::GetManager(wnd);
    if (mgr)
    {
       wxAuiDockArt*  art = mgr->GetArtProvider();
        if (art)
            return art->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE);
    }
    return 1;
}

int wxCstmTabArt::GetAdditionalBorderSpace(wxWindow* WXUNUSED(wnd))
{
    return 0;
}

wxSize wxCstmTabArt::GetTabSize(wxDC& dc,
                                     wxWindow* WXUNUSED(wnd),
                                     const wxString& caption,
                                     const wxBitmap& WXUNUSED(bitmap),
                                     bool WXUNUSED(active),
                                     int close_button_state,
                                     int* x_extent)
{
    wxCoord measured_textx, measured_texty;

    dc.SetFont(m_measuringFont);
    dc.GetTextExtent(caption, &measured_textx, &measured_texty);

    wxCoord tab_height = measured_texty + 5;
    wxCoord tab_width = measured_textx + tab_height + 18;

    if (close_button_state != wxAUI_BUTTON_STATE_HIDDEN)
        tab_width += m_activeCloseBmp.GetWidth();

    if (m_flags & wxAUI_NB_TAB_FIXED_WIDTH)
    {
        tab_width = m_fixedTabWidth;
    }

    *x_extent = tab_width - (tab_height/2) - 1;

    return wxSize(tab_width, tab_height);
}


void wxCstmTabArt::DrawButton(wxDC& dc,
                                   wxWindow* WXUNUSED(wnd),
                                   const wxRect& in_rect,
                                   int bitmap_id,
                                   int button_state,
                                   int orientation,
                                   wxRect* out_rect)
{
    wxBitmap bmp;
    wxRect rect;

    switch (bitmap_id)
    {
        case wxAUI_BUTTON_CLOSE:
            if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                bmp = m_disabledCloseBmp;
            else
                bmp = m_activeCloseBmp;
            break;
        case wxAUI_BUTTON_LEFT:
            if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                bmp = m_disabledLeftBmp;
            else
                bmp = m_activeLeftBmp;
            break;
        case wxAUI_BUTTON_RIGHT:
            if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                bmp = m_disabledRightBmp;
            else
                bmp = m_activeRightBmp;
            break;
        case wxAUI_BUTTON_WINDOWLIST:
            if (button_state & wxAUI_BUTTON_STATE_DISABLED)
                bmp = m_disabledWindowListBmp;
            else
                bmp = m_activeWindowListBmp;
            break;
    }

    if (!bmp.IsOk())
        return;

    rect = in_rect;

    if (orientation == wxLEFT)
    {
        rect.SetX(in_rect.x);
        rect.SetY(((in_rect.y + in_rect.height)/2) - (bmp.GetHeight()/2));
        rect.SetWidth(bmp.GetWidth());
        rect.SetHeight(bmp.GetHeight());
    }
    else
    {
        rect = wxRect(in_rect.x + in_rect.width - bmp.GetWidth(),
                      ((in_rect.y + in_rect.height)/2) - (bmp.GetHeight()/2),
                      bmp.GetWidth(), bmp.GetHeight());
    }


    DrawButtons(dc, rect, bmp, *wxWHITE, button_state);

    *out_rect = rect;
}

int wxCstmTabArt::ShowDropDown(wxWindow* wnd,
                                    const wxAuiNotebookPageArray& pages,
                                    int active_idx)
{
    wxMenu menuPopup;

    size_t i, count = pages.GetCount();
    for (i = 0; i < count; ++i)
    {
        const wxAuiNotebookPage& page = pages.Item(i);
        menuPopup.AppendCheckItem(1000+i, page.caption);
    }

    if (active_idx != -1)
    {
        menuPopup.Check(1000+active_idx, true);
    }

    // find out where to put the popup menu of window
    // items.  Subtract 100 for now to center the menu
    // a bit, until a better mechanism can be implemented
    wxPoint pt = ::wxGetMousePosition();
    pt = wnd->ScreenToClient(pt);
    if (pt.x < 100)
        pt.x = 0;
    else
        pt.x -= 100;

    // find out the screen coordinate at the bottom of the tab ctrl
    wxRect cli_rect = wnd->GetClientRect();
    pt.y = cli_rect.y + cli_rect.height;

    wxAuiCommandCapture* cc = new wxAuiCommandCapture;
    wnd->PushEventHandler(cc);
    wnd->PopupMenu(&menuPopup, pt);
    int command = cc->GetCommandId();
    wnd->PopEventHandler(true);

    if (command >= 1000)
        return command-1000;

    return -1;
}

int wxCstmTabArt::GetBestTabCtrlSize(wxWindow* wnd,
                                          const wxAuiNotebookPageArray& WXUNUSED(pages),
                                          const wxSize& WXUNUSED(requiredBmp_size))
{
    wxClientDC dc(wnd);
    dc.SetFont(m_measuringFont);
    int x_ext = 0;
    wxSize s = GetTabSize(dc,
                          wnd,
                          wxT("ABCDEFGHIj"),
                          wxNullBitmap,
                          true,
                          wxAUI_BUTTON_STATE_HIDDEN,
                          &x_ext);
    return s.y+3;
}

void wxCstmTabArt::SetNormalFont(const wxFont& font)
{
    m_normalFont = font;
}

void wxCstmTabArt::SetSelectedFont(const wxFont& font)
{
    m_selectedFont = font;
}

void wxCstmTabArt::SetMeasuringFont(const wxFont& font)
{
    m_measuringFont = font;
}