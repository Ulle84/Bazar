<?php include('authentication.php'); ?>
<!DOCTYPE html>
<meta charset="utf-8">
<html>
<head>
    <title>TuKi Einstellungen</title>
    <link rel="stylesheet" type="text/css" href="../CSS/global.css" media="all"/>
    <script language="JavaScript" src="../JavaScript/settings.js"></script>
</head>

<body>

<?php

include('navigation.php');

require_once 'Settings.php';

$settings = new Settings();

echo '<h1>Einstellungen</h1>';

echo '<table>';
echo '<tr>';
echo '<td>Verkäufernummern von</td><td><input onchange="saveSettings()" id="minSellerNumber" type="text" size="3" maxlength="3" value="' . $settings->minSellerNumber . '"/></td>';
echo '<td>bis</td><td><input onchange="saveSettings()" id="maxSellerNumber" type="text" size="3" maxlength="3" value="' . $settings->maxSellerNumber . '"/></td>';
echo '</tr>';
echo '<tr>';
echo '<td>Artikelnummern von</td><td><input onchange="saveSettings()" id="minArticleNumber" type="text" size="3" maxlength="3" value="' . $settings->minArticleNumber . '"/></td>';
echo '<td>bis</td><td><input onchange="saveSettings()" id="maxArticleNumber" type="text" size="3" maxlength="3" value="' . $settings->maxArticleNumber . '"/></td>';
echo '</tr>';
echo '<tr>';
echo '<td>Frist für Eingabe:</td><td colspan="3"><input onchange="saveSettings()" id="deadline" type="text" size="10" maxlength="10" value="' . $settings->deadline . '"/></td>';
echo '</tr>';


echo '</table>';

?>

</body>
</html>

