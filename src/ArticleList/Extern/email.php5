<?php
    function writeInformationToFile($reservationNumber) {
        $fileName = "Reservation_" . $reservationNumber . ".txt";
        $fh = fopen($fileName, "w");
        fwrite($fh, $reservationNumber . "\n");
        fwrite($fh, date("d.m.Y H:i:s") . "\n");
        fwrite($fh, $_REQUEST['vorname'] . "\n");
        fwrite($fh, $_REQUEST['nachname'] . "\n");
        fwrite($fh, $_REQUEST['email'] . "\n");
        fwrite($fh, $_REQUEST['telefon']);
        fclose($fh);
    }

    $datei="dateien/counter.txt";
    $stellen = 3;

	$telefon= $_REQUEST['telefon'];
	$empfaenger= $_REQUEST['empfaenger'];
	$empfaenger2= $_REQUEST['empfaenger2'];
	$betreff= $_REQUEST['betreff'];
	$nachricht= $_REQUEST['inhalt'];
	$g_femail=$_REQUEST['email'];
	$datum = date("d.m.Y H:i:s");
	$absender= $_REQUEST['vorname'];
	$absender.= ', ' . $_REQUEST['nachname'];
    $errorMeldung="";

    	if (empty($_REQUEST['vorname'])) {$errorMeldung="F&uuml;llen Sie bitte das Feld Vorname aus";
	       } elseif (empty($_REQUEST['nachname'])) {$errorMeldung="F&uuml;llen Sie bitte das Feld Nachname aus";
	       } elseif (empty($_REQUEST['telefon'])) {$errorMeldung="F&uuml;llen Sie bitte das Feld Telefon aus";
	       } elseif (empty($_REQUEST['email'])) {$errorMeldung="F&uuml;llen Sie bitte das Feld E-Mail aus";
	       } elseif (!empty($g_femail)) {
	          if(ereg("^[_a-zA-Z0-9-]+(\.[_a-zA-Z0-9-]+)*@([a-zA-Z0-9-]+\.)+([a-zA-Z]{2,4})$",$g_femail)){
  		          $absender.= ' <' . $g_femail . '>';
		      } else {
  		          $errorMeldung="Fehlerhafte E-mail! Bitte geben Sie die E-Mail Adresse erneut ein";
		      }
	       }
	       if(!empty($errorMeldung)) {
               echo $errorMeldung .'</p>';
		       $errorMeldung="";
		       echo "Bitte gehen Sie <a href=\"javascript:history.back();\">zur&uuml;ck!</a>";

		   } else {
?>
              <div class="design_rahmen">
               <div class="design_titel">TV Hechtsheim 1882 e.V.</div>
               <div class="design_rahmen_neu">
               <div class="design_banner"><img src="bilder/banner.jpg" border="0"></div>
               <div class="design_navigation">
               <div class="design_navigation_button"><a href="index.php5" style="color: #ffffff">zur&uuml;ck</a></div>
               <div class="design_navigation_button"><a href="../index.php5" style="color: #ffffff">TV Hechtsheim 1882</a></div>
               <div class="design_navigation_button"><a href="index.php5?url=seite&identifier=tvh:impressum&abteilung=<?php echo $gesamtverein->lk; ?>&menu=gesamtverein" style="color: #ffffff">Impressum</a></div>
              </div>
   		      <div class="design_linie_rot"></div>
		      <div class="design_linie_weiss"></div>
<?php
         if(file_exists($datei)){
            $fp2=fopen($datei,"r");
            $zahl=fgets($fp2,$stellen);
            fclose($fp2);
         }
         $zahl=sprintf("%0".$stellen."d",$zahl);
         if ($zahl <"075") {
              include "counter.php5";
              echo "Liebe Interessenten am TuKi-Basar,<br/><br/>
                 wir haben Ihnen eine Verkaufsnummer f&uuml;r den Basar am 16. November 2014 reserviert.<br/><br/>
                 Bitte holen Sie Ihre Unterlagen am Mittwoch, den 15. Oktober 2014 zwischen 19.00 Uhr und 21.00 Uhr ab.<br/>                            in der Halle des TV-Hechtsheim im Klein-Winternheimer-Weg ab.<br/><br/>
                 
                 Bringen Sie dazu bitte die 7 Euro Unterlagengeb&uuml;hr, das ausgedruckte und ausgef&uuml;llte Formular mit Ihrer aktuellen Kontoverbindung mit.<br/><br/>
                 <a href=http://www.tv-hechtsheim.de/tuki/dateien/TuKi_Bankverbindung.pdf target=_blank>Formular Bankverbindung</a><br/><br/>
                 Verkaufsnummern, die bis 21.00 Uhr nicht abgeholt werden, vergeben wir an weitere<br/>
                 Interessenten auf der Warteliste.</p>
                 
                 Bis dahin viele Gr&uuml;&szlig;e, <br/>
                 Ihr TuKi-Team";
              $nachricht="Liebe Interessenten am TuKi-Basar,
              
wir haben Ihnen eine Verkaufsnummer f�r den Basar am 16. November 2014 reserviert.

Bitte holen Sie Ihre Unterlagen am Mittwoch, den 15. Oktober 2014 zwischen 19.00 Uhr und 21.00 Uhr
in der Halle des TV-Hechtsheim im Klein-Winternheimer-Weg ab.
Bringen Sie dazu bitte die 7 Euro Unterlagengeb�hr, das ausgedruckte und ausgef�llte Formular 

(bitte hier herunterladen http://www.tv-hechtsheim.de/tuki/dateien/TuKi_Bankverbindung.pdf) mit.

Verkaufsnummern, die bis 21.00 Uhr nicht abgeholt werden, vergeben wir an weitere Interessenten auf der Warteliste.

Bis dahin viele Gr��e,
Ihr TuKi-Team";

              $nachricht2 = "Liebes Tuki-Team,
                        zur INFO:			
                        am " . $datum . " wurde fuer
                        " . $absender . "
                        Telefon: " . $telefon . "
                        folgende Nummer reserviert: " . $zahl . "
                        Gru�";

                       $header = 'From: ' . "Tuki-Team" . "\r\n" .
				        'Reply-To: ' . $absender . "\r\n" .
			         	'X-Mailer: PHP/' . phpversion();
			             mail($g_femail, $betreff, $nachricht, $header);

             writeInformationToFile($zahl);

			  $header = 'From: ' . $g_femail . "\r\n" .
			        	'Reply-To: ' . $absender . "\r\n" .
			        	'X-Mailer: PHP/' . phpversion();
			            mail($empfaenger, $betreff, $nachricht2, $header);

			  $header = 'From: ' . $g_femail . "\r\n" .
			        	'Reply-To: ' . $absender . "\r\n" .
			        	'X-Mailer: PHP/' . phpversion();
			             mail($empfaenger2, $betreff, $nachricht2, $header);

              echo "Bitte gehen Sie hier <a href=\"index.php5\"><b>ZUR&Uuml;CK</b></a> zur Startseite!";


     } else {
            echo "Liebe TuKi-Interessenten,<br/><br/>
                leider ist unser Nummernkontingent f&uuml;r diesen Basar schon vergeben. <br/>
                Wir w&uuml;rden uns aber freuen, wenn Sie bei unserem n&auml;chsten Basar im M&auml;rz den 08.03.2015<br /> 
                wieder dabei w&auml;ren. Die Online Nummernvergabe f&uuml;r den Fr�hjahrs-Basar findet<br />
                am 07. Februar 2015 statt.<br/><br/>
                Freundliche Gr&uuml;&szlig;e,<br />
                Ihr TuKi-Team<br /></p>";
           echo "Bitte gehen Sie hier <a href=\"index.php5\"><b>ZUR&Uuml;CK</b></a> zur Startseite!";
	}
   }
?>
