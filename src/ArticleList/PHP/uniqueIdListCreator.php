<?php include('authentication.php'); ?>
<!DOCTYPE html>
<meta charset="utf-8">
<html>
<head>
    <title>Unique ID Creator</title>
    <link rel="stylesheet" type="text/css" href="../CSS/global.css" media="all"/>
</head>
<body>

<?php

include('navigation.php');

require_once 'UniqueIdList.php';
require_once 'Settings.php';

$settings = new Settings();



$uniqueIdList = new UniqueIdList();
$uniqueIdList->createNewList($settings->minSellerNumber, $settings->maxSellerNumber);

$uniqueIdList->writeToFile("../Data/uniqueIds.txt");

echo 'Neue Liste wurde erfolgreich erstellt! <br />';

$uniqueIdList->printList();

?>
</body>
</html>
