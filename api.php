<?php
header("Content-Type: application/json"); // Устанавливаем заголовок для JSON

// Подключение к базе данных
$conn = new mysqli("localhost", "ESP32", "1", "my_database");

// Проверка подключения
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// SQL-запрос для получения строки с последним id
$sql = "SELECT param1 FROM my_table ORDER BY id DESC LIMIT 1";
$result = $conn->query($sql);

if (!$result) {
    http_response_code(500); // Ошибка сервера
    echo json_encode(["error" => "Database query failed: " . $conn->error]);
    exit;
}

// Получаем данные
if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $param1 = $row['param1']; // Извлекаем значение param1

    // Формируем JSON-ответ
    $data = ["param1" => $param1]; // Указываем данные для JSON
    echo json_encode($data); // Возвращаем JSON
} else {
    echo json_encode(["error" => "No data found"]); // Если данных нет
}

// Закрываем соединение
$conn->close();
?>
