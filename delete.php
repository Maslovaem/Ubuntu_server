<?php
$servername = "localhost";
$username = "ESP32";
$password = "1";
$dbname = "my_database";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$medicine = (int)$_POST['medicine'];
$cell = (int)$_POST['cell'];

$sql = " DELETE FROM project_base WHERE cell = '$cell' AND medicine = '$medicine'";

if ($conn->query($sql) === TRUE) {
    echo "Удаление произошло успешно";

} else {
    echo "Ошибка удаления: " . $sql . "<br>" . $conn->error;
}

$conn->close();

header("refresh:3;url=index.html");
?>


