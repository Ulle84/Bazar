function WindowBlocker () {
    this.background = window.document.createElement("div");
    this.background.style.position = "fixed";
    this.background.style.left = "0px";
    this.background.style.top = "0px";
    this.background.style.backgroundColor = "black";
    this.background.style.display = "none";
    this.background.style.opacity = "0.50";
    window.document.body.appendChild(this.background);

    this.reposition();
}

WindowBlocker.prototype.block = function () {
    this.background.style.display = "block";
    this.background.style.zIndex = 3;
}

WindowBlocker.prototype.unblock = function () {
    this.background.style.display = "none";
    this.background.style.zIndex = -3;
}

WindowBlocker.prototype.reposition = function () {
    var w = window,
        d = document,
        e = d.documentElement,
        g = d.getElementsByTagName('body')[0],
        top = document.documentElement.scrollTop || document.body.scrollTop,
        width = w.innerWidth || e.clientWidth || g.clientWidth,
        height = w.innerHeight|| e.clientHeight|| g.clientHeight;

    this.background.style.height = height + "px";
    this.background.style.width = width + "px";
}