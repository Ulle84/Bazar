function positionBoxes()
{
  var boxes = window.document.getElementsByClassName("barcodeBox");
  for (var i = 0; i < boxes.length; i++)
  {
    var style = "position: absolute; top: " + (400 * i).toString() + "px; left: 30px; width: 300px; height: 200px;";
    boxes[i].setAttribute("style", style);
  }
}