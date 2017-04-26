<?php include('authentication.php');

require_once 'ArticleList.php';
require_once 'Settings.php';

$settings = new Settings();


$pdf = new FPDF();

for ($i = $settings->minSellerNumber; $i <= $settings->maxSellerNumber; $i++) {
    $articleList = new ArticleList($i, "", $settings->minArticleNumber, $settings->maxArticleNumber);
    $articleList->appendToPdf($pdf);
}

$pdf->Output();
