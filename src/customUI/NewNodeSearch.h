#ifndef NEWNODESEARCH_H_INCLUDED
#define NEWNODESEARCH_H_INCLUDED

#include <wx/wx.h>
#include <wx/srchctrl.h>

//// FORWARD DECLARATIONS
class NewNodeSelection;
////

/** \brief The search bar used for filtering nodes in the NewNodeSelection element.
 */
class NewNodeSearch : public wxSearchCtrl{
private:
    NewNodeSelection* toSearch;/**<The NewNodeSelection where the searches are to be carried out.*/
public:
    /** \brief Creates this element.
     *
     * \param Parent wxWindow* The window which contains this element.
     * \param Id wxWindowID The wxWidgets id for this element.
     * \param Position const wxPoint& The default position of this element.
     * \param Size const wxSize& The default size of this element.
     * \param Style long The style of this element, as used in wxSearchCtrl.
     *
     */
    NewNodeSearch(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
    /** \brief Sets the NewNodeSelection where the filtering is to happen.
     *
     * \param toRegister NewNodeSelection* The NewNodeSelection which is to be filtered by the inputs.
     * \return void
     *
     */
    void registerTreeDisplay(NewNodeSelection* toRegister);
private:
    /** \brief This will redirect the user input to toSearch in case the search button is pressed.
     *
     * \param evt wxCommandEvent& The event caused by the press of the search button.
     * \return void
     *
     */
    void OnSearch(wxCommandEvent& evt);

    wxDECLARE_EVENT_TABLE();
};

#endif // NEWNODESEARCH_H_INCLUDED
