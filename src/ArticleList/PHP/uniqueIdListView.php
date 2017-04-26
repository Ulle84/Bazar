<?php include('authentication.php'); ?>
<!DOCTYPE html>
<meta charset="utf-8">
<html>
<head>
    <title>Unique ID List Viewer</title>
    <link rel="stylesheet" type="text/css" href="../CSS/global.css" media="all"/>
</head>
<body>

<?php

include('navigation.php');

require_once 'UniqueIdList.php';

$uniqueIdList = new UniqueIdList();
$uniqueIdList->readFromFile("../Data/uniqueIds.txt");
$uniqueIdList->printList();

?>
</body>
</html>
