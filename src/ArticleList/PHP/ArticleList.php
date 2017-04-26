<?php

require_once '../ExternalResources/FreePDF_v1_7/fpdf.php';

class ArticleList
{
    private $sellerNumber;
    private $fileName;
    private $articleDescription = array();
    private $size = array();
    private $price = array();
    private $fileDescription;
    private $versionNumber;
    private $sellerNumberFile;
    private $firstName;
    private $lastName;
    private $phone;
    private $id;
    private $minArticleNumber;
    private $maxArticleNumber;
    //private $dataValid = true;

    function __construct($sellerNumber, $id, $minArticleNumber, $maxArticleNumber)
    {
        $this->sellerNumber = $sellerNumber;
        $this->id = $id;
        $this->minArticleNumber = $minArticleNumber;
        $this->maxArticleNumber = $maxArticleNumber;
        $this->readFromFile();
    }

    public function appendToPdf($pdf)
    {
        if ($this->firstName == "" || $this->lastName == "" || $this->phone == "") {
            return;
        }

        // Column headings
        $header = array('Nr.', 'Preis', 'Größe', 'Artikelbeschreibung');
        $w = array(12, 15, 21, 140);

        $pdf->SetFont('Arial', '', 12);
        $pdf->AddPage();

        $pdf->SetFillColor(255, 255, 255);
        $pdf->SetTextColor(0);
        $pdf->SetDrawColor(0, 0, 0);
        $pdf->SetLineWidth(.3);

        // Data
        $rowCount = 0;
        for ($i = $this->minArticleNumber; $i <= $this->maxArticleNumber; $i++) {
            if ($rowCount % 41 == 0 && $i < $this->maxArticleNumber) {
                // Header
                $pdf->Cell(200, 10, utf8_decode('Verkäufer Nummer: ' . $this->sellerNumber . '   Name: ' . $this->firstName . ' ' . $this->lastName . '   Telefon: ') . $this->phone);
                $pdf->Ln();
                $pdf->SetFont('', 'B');
                for ($j = 0; $j < count($header); $j++) {
                    $pdf->Cell($w[$j], 7, utf8_decode($header[$j]), 1, 0, 'C', true);
                }
                $pdf->SetFont('');
                $pdf->Ln();
            }
            $pdf->Cell($w[0], 6, utf8_decode($i), 1, 0, 'L', false);
            $pdf->Cell($w[1], 6, utf8_decode($this->price[$i]), 1, 0, 'R', false);
            $pdf->Cell($w[2], 6, utf8_decode($this->size[$i]), 1, 0, 'L', false);
            $pdf->Cell($w[3], 6, utf8_decode($this->articleDescription[$i]), 1, 0, 'L', false);

            $pdf->Ln();
            $rowCount++;
        }
    }

    public function writePdf()
    {
        $pdf = new FPDF();

        $this->appendToPdf($pdf);

        $pdf->Output();
    }

    public function writeHtml($readOnly)
    {
        echo '<!DOCTYPE html>';
        echo '<meta charset="utf-8">';
        echo '<html>';
        echo '<head>';
        echo '<title>TuKi Artikelliste</title>';
        echo '<link rel="stylesheet" type="text/css" href="../CSS/global.css" media="all"/>';
        echo '<script src="../JavaScript/articleList2.js"></script>';
        echo '<script src="../JavaScript/Dialog.js"></script>';
        echo '<script src="../JavaScript/WindowBlocker.js"></script>';
        echo '</head>';

        echo '<body onload="init(' . $this->sellerNumber . ', \'' . $this->id . '\')">';

        /*if (!$this->dataValid) {
            echo "Daten inkonsistent! Bitte Tuki-Team informieren!";
            echo "</body>";
            return;
        }*/

        echo '<noscript>';
		echo '<div class="error">';
        echo 'In Ihrem Browser ist JavaScript nicht implemenitiert, bzw. deaktiviert.<br />';
        echo 'Für den vollen Funktionsumfang ist JavaScript erforderlich!';
	    echo '</div>';
		echo '</noscript>';

        echo '<h1>Artikelliste für Verkäufer Nr. ' . $this->sellerNumber . '</h1>';

        if ($readOnly) {
            echo '<div class="warning">Die Frist für die Eingabe von Daten ist abgelaufen!<br />Die eingegebenen Daten können nicht mehr editiert werden!<br /></div>';
        }

        echo '<table>';
        echo '<tr>';
        echo '<td>Vorname</td>';
        if ($readOnly) {
            echo '<td>' . htmlspecialchars($this->firstName) . '</td>';
        } else {
            echo '<td><input id="firstname" onblur="save()" onchange="save()" value="' . htmlspecialchars($this->firstName) . '" type="text" size="30"/></td>';
        }
        echo '</tr>';

        echo '<tr>';
        echo '<td>Nachname</td>';
        if ($readOnly) {
            echo '<td>' . htmlspecialchars($this->lastName) . '</td>';
        } else {
            echo '<td><input id="lastname" onblur="save()" onchange="save()" value="' . htmlspecialchars($this->lastName) . '" type="text" size="30"/></td>';
        }
        echo '</tr>';

        echo '<tr>';
        echo '<td>Telefonnummer (für Rückfragen)</td>';
        if ($readOnly) {
            echo '<td>' . htmlspecialchars($this->phone) . '</td>';
        } else {
            echo '<td><input id="phone" onblur="save()" onchange="save()" value="' . htmlspecialchars($this->phone) . '" type="text" size="30"/></td>';
        }
        echo '</tr>';
        echo '</table>';
        echo '<br />';

        echo '<table id="articleList">';
        echo '<tr>';
        echo '<th>Artikelnummer</th>';
        echo '<th>Preis</th>';
        echo '<th>Größe</th>';
        echo '<th>Artikelbeschreibung</th>';
        echo '</tr>';

        for ($i = $this->minArticleNumber; $i <= $this->maxArticleNumber; $i++) {
            echo '<tr class="data">';

            echo '<td>' . $i . '</td>';
            if ($readOnly) {
                echo '<td>' . htmlspecialchars($this->price[$i]) . '</td>';
                echo '<td>' . htmlspecialchars($this->size[$i]) . '</td>';
                echo '<td>' . htmlspecialchars($this->articleDescription[$i]) . '</td>';
            } else {
                //TODO wann wird onblur gesendet und wann onchange - muss man wirklicha auf beide Events hören???
                echo '<td><input onfocus="highlightCurrentLine(this)" onblur="checkPrice(this); save()" onchange="checkPrice(this); save()" class="right" value="' . htmlspecialchars($this->price[$i]) . '" type="text" size="6" maxlength="6" /> €</td>';
                echo '<td><input onfocus="highlightCurrentLine(this)" onblur="save()" onchange="save()" value="' . htmlspecialchars($this->size[$i]) . '" type="text" size="7" maxlength="7" /></td>';
                echo '<td><input onfocus="highlightCurrentLine(this)" onblur="save()" onchange="save()" value="' . htmlspecialchars($this->articleDescription[$i]) . '" type="text" size="60" maxlength="50" /></td>';
            }


            echo '</tr>';
        }

        echo '</table>';
        echo '<br />';

        if (!$readOnly) {
            echo '<input type="button" id="saveButton" value="Tabelle Speichern" onclick="save(true, true)"/>';
            echo '<input type="button" value="Tabelle als PDF anzeigen" onclick="showPdf()"/>';
        }
        else {
            echo '<input type="button" value="Tabelle als PDF anzeigen" onclick="openPdf()"/>';
        }



        echo '</body>';
        echo '</html>';
    }

    private function readFromFile()
    {
        $this->fileName = "../Data/articleList_" . $this->sellerNumber . ".txt";

        if (file_exists($this->fileName)) {
            $file = fopen($this->fileName, "r");

            $this->fileDescription = rtrim(fgets($file));
            $this->versionNumber = rtrim(fgets($file));
            $this->sellerNumberFile = rtrim(fgets($file));
            $this->firstName = rtrim(fgets($file));
            $this->lastName = rtrim(fgets($file));
            $this->phone = rtrim(fgets($file));

            for ($i = $this->minArticleNumber; $i <= $this->maxArticleNumber; $i++) {
                $articleNumber = rtrim(fgets($file));

                /*if ($articleNumber != $i) {
                    $this->dataValid = false;
                }*/

                $this->price[$i] = rtrim(fgets($file));
                $this->size[$i] = rtrim(fgets($file));
                $this->articleDescription[$i] = rtrim(fgets($file));
            }
            fclose($file);
        } else {
            for ($i = $this->minArticleNumber; $i <= $this->maxArticleNumber; $i++) {
                $this->price[$i] = "";
                $this->size[$i] = "";
                $this->articleDescription[$i] = "";
            }
        }
    }
}
