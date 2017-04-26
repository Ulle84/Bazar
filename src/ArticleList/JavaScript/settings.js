function saveSettings() {
    var content = window.document.getElementById("minSellerNumber").value;
    content += "\n" + window.document.getElementById("maxSellerNumber").value;
    content += "\n" + window.document.getElementById("minArticleNumber").value;
    content += "\n" + window.document.getElementById("maxArticleNumber").value;
    content += "\n" + window.document.getElementById("deadline").value;

    var xmlhttp = new XMLHttpRequest();

    xmlhttp.open("POST", "../PHP/settingsSaver.php", true);
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xmlhttp.send("content=" + content);
}