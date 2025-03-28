<?php
$servername = "localhost";
$username = "username";
$password = "password";
$dbname = "my_database";

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Получаем данные из формы
$param1 = $_POST['param1'];
$param2 = $_POST['param2'];
$date = $_POST['date'];
$cell = $_POST['cell']; // Новое поле

// SQL-запрос для вставки данных
$sql = "INSERT INTO my_table (param1, param2, date, cell) VALUES ('$param1', '$param2', '$date', '$cell')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
