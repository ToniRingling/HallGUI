#ifndef NODEDESCRIPTIONDISPLAY_H_INCLUDED
#define NODEDESCRIPTIONDISPLAY_H_INCLUDED

#include <wx/wx.h>

class NodeDescriptionDisplay : public wxPanel{
private:
    wxFlexGridSizer* sizer;
    wxTextCtrl* nodeNameText;
    wxTextCtrl* nodeDescriptionText;
public:
    /** \brief
     *
     * \param Parent wxWindow* The window which contains this element.
     * \param Id wxWindowID The wxWidgets ID for this element.
     * \param Position const wxPoint& The default position of this element.
     * \param Size const wxSize& The default size of this element.
     * \param Style long The style of this element, used as in wxScrolledWindow.
     *
     */
    NodeDescriptionDisplay(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);

    void setContent(std::string name, std::string description);

private:
    /** \brief Handler for resizing events.
     *
     * \param e wxSizeEvent& The event which causes the resizing.
     * \return void
     *
     */
    void OnResize(wxSizeEvent& e);

    wxDECLARE_EVENT_TABLE();
};

#endif // NODEDESCRIPTIONDISPLAY_H_INCLUDED
