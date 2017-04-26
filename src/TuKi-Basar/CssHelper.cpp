#include "CssHelper.h"


QString CssHelper::createCssCode(bool advancedStyle, bool doubleFontSize)
{
    QString cssCode;
    cssCode.append("<style>");
    cssCode.append("body, html{margin: 0px; padding: 0px;}");
    cssCode.append(QString("body {font-family: Verdana; font-size: %1pt; line-height: 150%;}").arg(doubleFontSize ? 20 : 10));
    //cssCode.append("br {margin-bottom: 12px;}");
    /*cssCode.append("h1 {margin-top: 20px; padding-top: 20px; font-size: 130%; padding-bottom: 5px; margin-bottom: 0px;}");
  cssCode.append("h2 {margin-top: 10px; padding-top: 10px; font-size: 120%; padding-bottom: 5px; margin-bottom: 0px;}");
  cssCode.append("h3 {margin-top: 7px; padding-top: 7px; font-size: 110%; padding-bottom: 5px; margin-bottom: 0px;}");
  cssCode.append("h4, h5, h6 {font-size: 105%; padding-bottom: 5px; margin-bottom: 0px;}");*/
    cssCode.append("h1 {font-size: 130%;}");
    cssCode.append("h2 {font-size: 120%;}");
    cssCode.append("h3 {font-size: 110%;}");
    cssCode.append("h4, h5, h6 {font-size: 105%;}");
    cssCode.append("table {border: 2px solid black; border-spacing: 0px;}");

    if (advancedStyle)
    {
        cssCode.append("table ul {margin-left: 20px; padding-left: 0px; margin-top: 0px; margin-bottom: 0px;}");
        cssCode.append("tr:hover, tr:nth-child(even):hover, tr:nth-child(odd):hover {background-color: rgb(187, 188, 210);}");
        cssCode.append("tr:nth-child(even) {background-color: rgb(230, 230, 230);}");
        cssCode.append("tr:nth-child(odd) {background-color: rgb(255, 255, 255);}");
    }

    cssCode.append("th {text-align: left;}");
    cssCode.append("td, th {max-width: 400px; padding: 5px 5px 5px 5px; vertical-align: top; border-bottom: 1px solid grey; border-right: 1px solid grey;}");
    cssCode.append("td:last-child, th:last-child {border-right: 0px;}");
    cssCode.append("tr:last-child > td {border-bottom: 0px;}");
    //cssCode.append(".page{min-height: 26.85cm; max-height: 26.85cm; border-bottom: 1px solid black;}"); //TODO remove border
    cssCode.append(".page{page-break-after:always}");
    cssCode.append(".prize{text-align: right}");
    cssCode.append("</style>");

    return cssCode;
}
