function Dialog () {
    this.windowBlocker = new WindowBlocker();

    this.background = window.document.createElement("div");
    this.background.style.position = "fixed";
    this.background.style.left = "50%";
    this.background.style.top = "50%";
    this.background.style.width = "480px"
    this.background.style.height = "270px"
    this.background.style.margin = "-135px 0 0 -240px"
    this.background.style.borderRadius = "20px";
    this.background.style.backgroundColor = "#FFFFD1";  // warning #FFFFD1; error #FFDED6
    this.background.style.display = "none";

    this.text = window.document.createElement("div");
    this.text.style.margin = "40px";
    this.background.appendChild(this.text);

    this.button = window.document.createElement("input");
    this.button.type = "button";
    this.button.value = "OK";
    this.button.style.position = "absolute";
    this.button.style.left = "330px";
    this.button.style.top = "200px";
    this.button.setAttribute("onclick", "closeDialog()");

    this.background.appendChild(this.button);

    window.document.body.appendChild(this.background);
}

/*Dialog.prototype.reposition = function () {
    var w = window,
        d = document,
        e = d.documentElement,
        g = d.getElementsByTagName('body')[0],
        top = document.documentElement.scrollTop || document.body.scrollTop,
        width = w.innerWidth || e.clientWidth || g.clientWidth,
        height = w.innerHeight|| e.clientHeight|| g.clientHeight;

    this.background.style.top = (top + height / 2) + "px";
}*/

Dialog.prototype.show = function () {
    //this.reposition();

    //this.windowBlocker.reposition();
    this.windowBlocker.block();

    this.background.style.zIndex = 10;
    this.background.style.display = "block";
}

Dialog.prototype.close = function () {
    this.windowBlocker.unblock();

    this.background.style.zIndex = -10;
    this.background.style.display = "none";
}

Dialog.prototype.setText = function (text) {
    this.text.textContent = text;
}

Dialog.prototype.setButtonText = function (buttonText) {
    this.button.value = buttonText;
}

Dialog.prototype.setBackgroundColor = function (color) {
    this.background.style.backgroundColor = color;
}

Dialog.prototype.addSecondButton = function (buttonText, functionCall) {
    this.button2 = window.document.createElement("input");
    this.button2.type = "button";
    this.button2.value = buttonText;
    this.button2.style.position = "absolute";
    this.button2.style.left = "120px";
    this.button2.style.top = "200px";
    this.button2.setAttribute("onclick", functionCall);

    this.background.appendChild(this.button2);
}