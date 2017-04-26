var sellerNumber = 0;
var id;
var theDialog;
var decisionDialogPdf;
var decisionDialogSave;
var confirmationDialog;
var lastSelectedRow;

function closeDialog() {
    confirmationDialog.close();
    decisionDialogPdf.close();
    decisionDialogSave.close();
}

function init (sellerNumberValue, idValue) {
    sellerNumber = sellerNumberValue;
    id = idValue;
    confirmationDialog = new Dialog();
    decisionDialogPdf = new Dialog();
    decisionDialogPdf.setButtonText("Zurück zur Liste");
    decisionDialogPdf.addSecondButton("PDF dennoch anzeigen", "closeDialog(); openPdf()");
    decisionDialogPdf.setBackgroundColor("#FFDED6");

    decisionDialogSave = new Dialog();
    decisionDialogSave.setButtonText("Zurück zur Liste");
    decisionDialogSave.addSecondButton("Dennoch speichern", "closeDialog(); save(false, true)");
    decisionDialogSave.setBackgroundColor("#FFDED6");

    theDialog = confirmationDialog;

    var trs = window.document.getElementById("articleList").getElementsByTagName("tr");
    if (trs.length > 1) {
        lastSelectedRow = trs[1];
        trs[1].style.backgroundColor = "lightgrey";
    }

}

function save(checkInput, confirmation) {

    var inputChecked = true;
    if (checkInput) {
        theDialog = decisionDialogSave;
        inputChecked = checkAllInputs();
        theDialog = confirmationDialog;
    }

    var content = "Article List\nVersion 1.0\n" + sellerNumber + "\n";

    content += window.document.getElementById("firstname").value.replace(new RegExp("&", 'g'), "%26") + "\n";
    content += window.document.getElementById("lastname").value.replace(new RegExp("&", 'g'), "%26") + "\n";
    content += window.document.getElementById("phone").value.replace(new RegExp("&", 'g'), "%26") + "\n";

    var data = window.document.getElementsByClassName("data");

    for (var i = 0; i < data.length; i++) {
        var dataContent = data[i].getElementsByTagName("td");

        content += dataContent[0].textContent + "\n";

        for (var j = 1; j < dataContent.length; j++) {
            content += dataContent[j].firstChild.value.replace(new RegExp("&", 'g'), "%26");
            content += "\n";
        }
    }

    var xmlhttp = new XMLHttpRequest();

    if (confirmation && inputChecked) {
        var saveButton = window.document.getElementById("saveButton");
        saveButton.disabled = true;
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4) {
                if (xmlhttp.status == 200) {
                    //alert(xmlhttp.responseText);
                    theDialog.setText(xmlhttp.responseText);
                }
                else {
                    theDialog.setText("Es ist ein Fehler beim Speichern aufgetreten\nBitte versuchen Sie es erneut!");
                }
                theDialog.show();
                saveButton.disabled = false;
            }
        }
    }

    xmlhttp.open("POST", "../PHP/articleListSaver.php", true);
    xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xmlhttp.send("sellerNumber=" + sellerNumber + "&content=" + content);
}

function showPdf() {
    theDialog = decisionDialogPdf;
    if (checkAllInputs()) {
        openPdf();
    }
    theDialog = confirmationDialog;
}

function openPdf() {
    window.open("../PHP/articleListView.php?seller=" + sellerNumber + "&id=" + id + "&mode=print");
}

function checkAllInputs() {
    if (window.document.getElementById("firstname").value == "") {
        //alert("Bitte Vornamen eingeben!");
        theDialog.setText("Bitte Vornamen eingeben!");
        theDialog.show();
        return false;
    }

    if (window.document.getElementById("lastname").value == "") {
        //alert("Bitte Nachnamen eingeben!");
        theDialog.setText("Bitte Nachnamen eingeben!");
        theDialog.show();
        return false;
    }

    if (window.document.getElementById("phone").value  == "") {
        //alert("Bitte Telefonnummer eingeben!");
        theDialog.setText("Bitte Telefonnummer eingeben!");
        theDialog.show();
        return false;
    }

    var data = window.document.getElementsByClassName("data");

    var missingPrices = new Array();

    for (var i = 0; i < data.length; i++) {
        var dataContent = data[i].getElementsByTagName("td");

        if (dataContent[1].firstChild.value == "")
        {
            if (dataContent[3].firstChild.value != "")
            {
                missingPrices.push(dataContent[0].textContent);
            }
        }
    }

    if (missingPrices.length > 0) {
        if (missingPrices.length == 1) {
            //alert("Beim Artikel " + missingPrices[0] + " ist eine Artikelbeschreibung, aber kein Preis angegeben.\nBitte tragen Sie einen Preis ein!");
            theDialog.setText("Beim Artikel " + missingPrices[0] + " ist eine Artikelbeschreibung, aber kein Preis angegeben.\nBitte tragen Sie einen Preis ein!");
            theDialog.show();
            return false;
        }
        else {
            //alert("Bei den Artikeln " + missingPrices.join(", ") + " sind Artikelbeschreibungen, aber keine Preise angegeben.\nBitte tragen Sie die Preise ein!");
            theDialog.setText("Bei den Artikeln " + missingPrices.join(", ") + " sind Artikelbeschreibungen, aber keine Preise angegeben.\nBitte tragen Sie die Preise ein!");
            theDialog.show();
            return false;
        }
    }

    return true;
}

function checkPrice(element) {
    if (element.value == "") {
        return;
    }

    var number = parseFloat(element.value.replace(",", "."));
    if (isNaN(number)) {
        element.value = ""; // first delete, otherwise the following message is doubled (due to the onblur-event, which occurs because of the alert)
        theDialog.setText("Der eingegebene Preis ist ungültig und wurde gelöscht!");
        theDialog.show();
        //alert("Der eingegebene Preis ist ungültig und wurde gelöscht!")
    }
    else {
        if (number < 0) {
            element.value = "";
            theDialog.setText("Negative Preise sind unsinnig!\nDer eingegebene Preis wurde gelöscht");
            theDialog.show();
            //alert("Negative Preise sind unsinnig!\nDer eingegebene Preis wurde gelöscht");
            return;
        }

        if (number >= 50.00) {
            element.value = ""; // set, so onblur-function does return immediately
            theDialog.setText("Achtung, der Preis ist 50,00 € oder höher - bitte kontrollieren!");
            theDialog.show();
        }

        if (number > 999.99) {
            //element.value = ""; // set, so onblur-function does return immediately
            //theDialog.setText("Preise größer als 999,99 € sind nicht erlaubt!\nDer eingegebene Preis wurde auf das Maximum erniedrigt");
            //theDialog.show();
            //alert("Preise größer als 999,99 € sind nicht erlaubt!\nDer eingegebene Preis wurde auf das Maximum erniedrigt");
            number = 999.99;
        }

        element.value = number.toFixed(2);
        element.value = element.value.replace(".", ",");
    }

}

function checkSize(element) {
    if (element.value == "") {
        return;
    }

    var number = parseInt(element.value);
    if (isNaN(number)) {
        element.value = ""; // first delete, otherwise the following message is doubled (due to the onblur-event, which occurs because of the alert)
        theDialog.setText("Die eingegebene Größe ist ungültig und wurde gelöscht!");
        theDialog.show();
        //alert("Die eingegebene Größe ist ungültig und wurde gelöscht!")
    }
    else {
        if (number < 50) {
            element.value = "";
            theDialog.setText("Die eingegebene Größe ist zu klein und wurde gelöscht");
            theDialog.show();
            //alert("Die eingegebene Größe ist zu klein und wurde gelöscht");
            return;
        }

        if (number > 188) {
            element.value = "";
            theDialog.setText("Die eingegebene Größe ist zu groß und wurde gelöscht");
            theDialog.show();
            //alert("Die eingegebene Größe ist zu groß und wurde gelöscht");
            return;
        }

        if ((number - 50) % 6 != 0) {
            element.value = "";
            theDialog.setText("Die eingegebene Größe ist falsch und wurde gelöscht");
            theDialog.show();
            //alert("Die eingegebene Größe ist falsch und wurde gelöscht");
            return;
        }

        element.value = number;
    }

}

function highlightCurrentLine(inputElement) {
    var selectedRow = inputElement.parentNode.parentNode;

    if (selectedRow != lastSelectedRow) {
        lastSelectedRow.style.backgroundColor = "white";
        selectedRow.style.backgroundColor = "lightgrey";

        lastSelectedRow = selectedRow;
    }



}
