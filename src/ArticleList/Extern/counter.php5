<?php

// Counterdateiname
$datei="dateien/counter.txt";
$verzeichnis="dateien";
if(!file_exists($verzeichnis)) {
	mkdir($verzeichnis);
}
	chmod($verzeichnis, 0755);
// Anzahl der fuehrenden Nullen
$stellen = 3;

if(file_exists($datei)){
  // Falls die Datei existiert, wird sie ausgelesen und
  // der dort enthaltene Wert um Eins erhoeht.
  $fp=fopen($datei,"r+");
  flock($fp,2);
  $zahl=fgets($fp,$stellen);
  $zahl++;
  rewind($fp);
  fputs($fp,$zahl,$stellen);
  flock($fp,3);
  fclose($fp);
}else{
  // Die Datei counter.txt existiert nicht, sie wird
  // neu angelegt und mit dem Wert 1 gefuellt.
  $fp=fopen($datei,"w");
  $zahl="1";
  fputs($fp,$zahl,$stellen);
  fclose($fp);
}

$zahl=sprintf("%0".$stellen."d",$zahl);

?>
