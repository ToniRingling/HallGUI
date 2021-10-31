#ifndef LOGFIELD_H_INCLUDED
#define LOGFIELD_H_INCLUDED

#include <wx/wx.h>
#include <wx/listctrl.h>

/** \brief A namespace for containing the types of loggable messages.
 *
 */
namespace LogMessageTypes{
    enum LogMessageTypes : int{
        regular = 0,
        warning = 1,
        error = 2,
        success = 3,
    };/**<The types of loggable messages.*/
}

/** \brief An area where important messages can be logged and shown to the user, with distinction between different types of messages.
 */
class LogField : public wxListCtrl{
private:
    int contentCount;/**<The number of lines which are currently logged.*/
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
    LogField(wxWindow* Parent = NULL, wxWindowID Id = -1, const wxPoint& Position = wxDefaultPosition, const wxSize& Size = wxDefaultSize, long Style = 0);
    /** \brief Removes all logged messages (and rebuilds the wxListCtrl).
     *
     * \return void
     *
     */
    void reset();
    /** \brief Logs the given message with the given type.
     *
     * \param type LogMessageTypes::LogMessageTypes The type of the messages. This decides the written type and the colour of the message.
     * \param message std::string The message to display.
     * \return void
     *
     */
    void log(LogMessageTypes::LogMessageTypes type, std::string message);
private:
    /** \brief Handler for resizing this element. Not used after initialization in Linux because that causes only one row to be displayed at any one time.
     *
     * \param e wxSizeEvent& The event which causes the resizing.
     * \return void
     *
     */
    void OnResize(wxSizeEvent& e);

    wxDECLARE_EVENT_TABLE();
};
#endif // LOGFIELD_H_INCLUDED
