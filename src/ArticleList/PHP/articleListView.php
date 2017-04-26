<?php

require_once 'UniqueIdList.php';
require_once 'ArticleList.php';
require_once 'Settings.php';

if (isset($_GET['seller'])) {
    $sellerNumber = $_GET['seller'];
} else {
    echo "Keine Verkäufernummer angegeben!";
    exit;
}

if (isset($_GET['id'])) {
    $id = $_GET['id'];
} else {
    echo "Keine ID angegeben!";
    exit;
}

if (isset($_GET['mode'])) {
    $mode = $_GET['mode'];
} else {
    $mode = 'view';
}

$admin = false;
if (isset($_GET['admin'])) {
    if ($_GET['admin'] == "true") {
        $admin = true;
    }
}

$uniqueIdList = new UniqueIdList();
$uniqueIdList->readFromFile("../Data/uniqueIds.txt");
if (!$uniqueIdList->checkId($sellerNumber, $id)) {
    echo "Link wurde falsch eingegeben - kein Zugang!";
    exit;
}

$settings = new Settings();
$articleList = new ArticleList($sellerNumber, $id, $settings->minArticleNumber, $settings->maxArticleNumber);

if ($mode == 'edit') {
    $exploded = explode('.', $settings->deadline);

    $now = new DateTime();
    $deadline = new DateTime($exploded[2] . '-' . $exploded[1] . '-' . $exploded[0]);
    $interval = $now->diff($deadline);

    if ($interval->format('%R%a') < 0) {
        $readOnly = true;
    }
    else {
        $readOnly = false;
    }

    if ($admin) {
        $readOnly = false;
    }

    $articleList->writeHtml($readOnly);
}
else {
    $articleList->writePdf();
}

?>