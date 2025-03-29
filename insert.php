<?php
$servername = "localhost";
$username = "ESP32";
$password = "1";
$dbname = "my_database";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Получаем данные из формы
$login = $_POST['login'];
$medicine = $_POST['medicine'];
$date = $_POST['date'];
$cell = $_POST['cell'];

$sql = "INSERT INTO project_base (login, medicine, date, cell) VALUES ('$login', '$medicine', '$date', '$cell')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
