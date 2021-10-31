#ifndef REWRAPPABLETEXT_H_INCLUDED
#define REWRAPPABLETEXT_H_INCLUDED

#include <wx/wx.h>

/** \brief A GUI element for containing text which can be wrapped repeatedly without permanently altering the string, i.e. wrapping to a higher size may remove line breaks.
 */
class RewrappableText : public wxStaticText{
private:
    wxString fullLabel;/**< The full label without added wrapping line breaks. */
    bool blockResize = false;
public:
    /** \brief
     *
     * \param NULL wxWindow* Parent= The parent window as standard in wxWidgets.
     * \param -1 wxWindowID Id= The id for this element as standard in wxWidgets.
     * \param label std::string The initial label of this element.
     * \param const wxPoint& Position = wxDefaultPosition The position of this element.
     * \param const wxSize& Size = wxDefaultSize The size of this element.
     * \param 0 long Style= The style of this element as used in wxStaticText.
     *
     */
    RewrappableText(wxWindow* Parent = NULL,wxWindowID Id = -1, std::string Label = "", const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);

    /** \brief Wraps the text by the given width, i.e. inserts line breaks where needed or removes such inserted line breaks again.
     *
     * \param width int The target width of the text.
     * \return void
     *
     */
    void Wrap(int width);
};

#endif // REWRAPPABLETEXT_H_INCLUDED
