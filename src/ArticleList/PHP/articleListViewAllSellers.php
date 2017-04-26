<?php include('authentication.php'); ?>
<!DOCTYPE html>
<meta charset="utf-8">
<html>
<head>
    <title>TuKi - Alle Verkäuferlisten</title>
    <link rel="stylesheet" type="text/css" href="../CSS/global.css" media="all"/>
</head>

<body>
<?php

include('navigation.php');

require_once 'UniqueIdList.php';
require_once 'ArticleList.php';
require_once 'Settings.php';

$uniqueIdList = new UniqueIdList();
$uniqueIdList->readFromFile("../Data/uniqueIds.txt");

$settings = new Settings();

for ($sellerNumber = $settings->minSellerNumber; $sellerNumber <= $settings->maxSellerNumber; $sellerNumber++) {
    $id = $uniqueIdList->getId($sellerNumber);
    $articleList = new ArticleList($sellerNumber, $id, $settings->minArticleNumber, $settings->maxArticleNumber);

    echo '<a target="_blank" href="articleListView.php?seller=' . $sellerNumber . '&id=' . $id . '&mode=edit&admin=true">Liste für Verkäufer ' . $sellerNumber . '</a><br />';
}

?>

</body>
</html>