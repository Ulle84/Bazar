<?php
session_start();

$username = $_POST['username'];
$password = $_POST['password'];

if ($username == "TuKi" && $password == "TuKiTante") {
    $_SESSION['isLoggedIn'] = true;
    echo 'OK';
} else {
    echo 'Falsches Passwort eingegeben!';
}

?>