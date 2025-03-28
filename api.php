<?php
header("Content-Type: application/json"); // Устанавливаем заголовок для JSON

// Подключение к базе данных
$conn = new mysqli("localhost", "ESP32", "1", "my_database");

// Проверка подключения
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// SQL-запрос для получения строки с последним id

/**
* теперь cell(номер ячейки) тоже передается
*/
$sql = "SELECT param1, cell FROM my_table ORDER BY id DESC LIMIT 1";
$result = $conn->query($sql);

if (!$result) {
    http_response_code(500); // Ошибка сервера
    echo json_encode(["error" => "Database query failed: " . $conn->error]);
    exit;
}

// Получаем данные
if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
	
	// Формируем JSON-ответ
    echo json_encode($row);
} else {
    echo json_encode(["error" => "No data found"]);
}

$conn->close();
?>