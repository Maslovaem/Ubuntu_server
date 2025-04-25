<?php
$servername = "localhost";
$username = "ESP32";
$password = "1";
$dbname = "my_database";
$correct_password = 1234;

$conn = new mysqli($servername, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Получаем данные из формы
$password = (int) $_POST['password'];
$login = $_POST['login'];
$medicine = $_POST['medicine'];
$date = $_POST['date'];
$cell = $_POST['cell'];

if ($password === $correct_password){
  $sql = "INSERT INTO project_base (login, medicine, date, cell) VALUES ('$login', '$medicine', '$date', '$cell')";

  if ($conn->query($sql) === TRUE) {
      echo "New record created successfully";
  } else {
      echo "Error: " . $sql . "<br>" . $conn->error;
  }
}

else{
  echo "Неверный пароль.";
}

$conn->close();
header("refresh:3;url=index.html");
?>


