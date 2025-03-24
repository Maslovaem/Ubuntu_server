<?php
$servername = "localhost"; // Адрес сервера MySQL (обычно localhost)
$username = "root";        // Имя пользователя (root)
$password = "1";  // Пароль пользователя root
$dbname = "my_database";   // Имя базы данных

// Создание подключения
$conn = new mysqli($servername, $username, $password, $dbname);

// Проверка подключения
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

// Обработка данных из формы
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  $param1 = $_POST['param1'];
  $param2 = $_POST['param2'];
  $param2 = $_POST['date'];
  
  // SQL-запрос для вставки данных
  $sql = "INSERT INTO my_table (param1, param2, date) VALUES ('$param1', '$param2', '$date')";

  // Выполнение запроса
  if ($conn->query($sql) === TRUE) {
    echo "Данные успешно добавлены!";
  } else {
    echo "Ошибка: " . $sql . "<br>" . $conn->error;
  }
} else {
  echo "Данные не были отправлены методом POST.";
}

// Закрытие подключения
$conn->close();
?>
