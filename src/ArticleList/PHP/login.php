<!DOCTYPE html>
<meta charset="utf-8">
<head>
    <title>TuKi - Anmeldung</title>
    <link rel="stylesheet" type="text/css" href="../CSS/login.css" media="all"/>
    <script language="JavaScript" src="../JavaScript/login.js"></script>
</head>
<body>

<div class="center">
    <table>
        <tr>
            <td colspan="2"><b>Bitte melden Sie sich an!</b></td>
        </tr>
        <tr>
            <td>Name:</td>
            <td><input type="text" name="username" id="username"/></td>
        </tr>

        <tr>
            <td>Passwort:</td>
            <td><input type="password" name="passwort" id="password" onkeydown="if (event.keyCode == 13) login()"/></td>
        </tr>
        <tr>
            <td></td>
            <td><input type="button" value="Anmelden" onclick="login()"/></td>
        </tr>
    </table>
    <br/>
    <span id="httpResponse"></span>
</div>
</body>
</html>