<?php

$content = $_POST['content'];
$sellerNumber = $_POST['sellerNumber'];

$fileName = "../Data/articleList_" . $sellerNumber . ".txt";
$fh = fopen($fileName, "w");
fwrite($fh, $content);
fclose($fh);

echo 'Artikelliste wurde gespeichert!';

?>