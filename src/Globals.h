#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <wx/wx.h>

/** \brief Namespace for all widely/globally used variables, structs and enums.
 *
 *
 */
namespace globals{
    //// Variables ////
    //predefined//
    extern double diagonalTextAngle;/**<The angle, relative to the neighboring edge, of angled text. 0 means parallel, 90 means perpendicular (wxwidgets uses degrees, don't fault me (and I want the conversions to be in this one place only)).*/
    extern double diagonalTextPadding;/**<Additional padding added to both sides of diagonal text for spacing between multiple lines of angled text.*/
    extern int portRadius;/**<The radius of node ports.*/
    extern double connectionThickness;/**<The thickness of connections between node ports.*/
    extern int nodeBorderThickness;/**<The thickness of the inner (between upper, middle and lower section) and outer borders of nodes.*/
    extern int nodeBorderPadding;/**<Additional horizontal padding in nodes to prevent text touching the borders.*/
    extern int gridSize;/**<The distance between the horizontal/vertical background grid lines in the graph editor.*/
    extern wxFont* font;/**<The exact font used for drawing in drawing contexts.*/
    //calculated//
    extern int fontHeight;/**<The maximum size of a line written in wxSWISS_FONT.*/
    extern double diagonalTextDistance;/**<The distance which should be left between the starting points of multiple lines of angled text.*/
    extern double diagonalTextBottomDistance;/**<Distance to the neighboring edge which may be needed for angled text which has the neighboring edge below.*/

    extern std::string currentNodeFileVersion;/**<The current version designation of node files.*/
    extern std::string currentProjectFileVersion;/**<The current version designation of project files.*/

    /** \brief Initializes all global variables. Should be called once as early as possible.
     *
     * \return void
     *
     */
    void initialize();

    //// Structs ////

    //// Enums ////
    enum userMode : int{
        modeUser,
        modeDeveloper,
    };
}

#endif // GLOBALS_H_INCLUDED
