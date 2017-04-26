<?php
if (isset($_SESSION['isLoggedIn']) && $_SESSION['isLoggedIn'] == "true")
{
    echo '<div id="navigation">';
    echo '<a href="index.php">Übersicht</a>';
    echo '&nbsp;&nbsp;&nbsp;<a href="logout.php">Abmelden</a>';
    echo '</div>';
}

/*$client = "";
if (isset($_SESSION['clientName'])) {
    $client = $_SESSION['clientName'];
}

echo '<div id="navigation">';
if (isset($_SESSION['assistantName'])) {
    echo 'Klient: ' . $client . '&nbsp;&nbsp;&nbsp;&nbsp;';
    if ($_SESSION['assistantName'] != "") {
        echo 'Assistent: <span id="username">' . $_SESSION['assistantName'] . '</span>&nbsp;&nbsp;&nbsp;&nbsp;';
    }
    else {
        echo '<span class="hidden" id="username">' . $_SESSION['clientName'] . '</span>';
    }
    echo '&nbsp;&nbsp;&nbsp;<a href="changePassword.php">Passwort ändern</a>';
    echo '&nbsp;&nbsp;&nbsp;<a href="logout.php">Abmelden</a>';

    echo '</div>';

    echo '<div id="sidebar">';
    echo '<a href="rosterView.php">Dienst-Plan</a><br/>';

    $settings = new Settings($_SESSION['clientName']);
    if ($settings->showToDoManager == 1) {
        echo '<a href="toDoManagerView.php">Aufgaben</a><br/>';
    }

    if ($_SESSION['isClient']) {
        //echo '<a href="monthPlanView.php">Monats-Plan</a><br/>';
        echo '<a href="teamView.php">Team</a><br/>';
        echo '<a href="settingsView.php">Einstellungen</a><br/>';
    }
    else {
        echo '<a href="calendarView.php">Kalender</a><br/>';
    }

    //echo '<a href="documentation.php">Dokumentation</a><br/>';
    echo '<a href="feedbackView.php">Feedback</a><br/>';
    echo '<a href="impressum.php">Impressum</a><br/>';
}
else
{
    echo '<a href="index.php">Willkommen</a>';
    echo '&nbsp;&nbsp;&nbsp;<a href="createNewAccount.php">Neuen Klienten-Zugang einrichten</a>';
    echo '&nbsp;&nbsp;&nbsp;<a href="login.php">Anmelden</a>';
}
echo '</div>';
?>*/