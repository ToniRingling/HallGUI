#include "Globals.h"

#include <wx/wx.h>
#include <math.h>

namespace{
    const double Pi = 3.141592654;
}

namespace globals{
    //// Variables ////
    wxFont* font;

    int fontSize = 10;
    double diagonalTextAngle = 45.0;
    double diagonalTextPadding = 5;
    int portRadius = 7;

    int fontHeight;
    double diagonalTextDistance;
    double diagonalTextBottomDistance;

    double connectionThickness = 5.0;
    int nodeBorderThickness = 2;
    int nodeBorderPadding = 5;

    int gridSize = 50;

    std::string currentNodeFileVersion = "HGNV-1";
    std::string currentProjectFileVersion = "HGPV-1";

    void initialize(){
        wxFont* font = (wxFont*)wxSWISS_FONT;
        wxMemoryDC DC;
        DC.SetFont(*font);

        fontHeight = DC.GetFontMetrics().height;

        double angleRadians = (diagonalTextAngle / 180.0) * Pi;

        diagonalTextBottomDistance = fontHeight * std::cos(angleRadians);
        double ls = diagonalTextBottomDistance / std::cos((Pi / 2.0) - angleRadians);
        int maxWidth = std::ceil((ls * std::sin((Pi / 2.0) - angleRadians)) + (fontHeight * std::sin(angleRadians))); // the maximum width of the text on a horizontal scan line

        diagonalTextDistance = std::max(maxWidth, 2 * globals::portRadius) + diagonalTextPadding;
    }

    //// Structs ////

}
