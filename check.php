<?php
header('Content-Type: application/json');

$servername = "localhost";
$username = "ESP32";
$password = "1";
$dbname = "my_database";

// Создаем соединение
$conn = new mysqli($servername, $username, $password, $dbname);

// Проверяем соединение
if ($conn->connect_error) {
    die(json_encode(['error' => 'Connection failed: ' . $conn->connect_error]));
}

// Получаем номер ячейки из формы
$cell = (int)$_POST['cell'];

// Запрос для получения последних 6 записей для выбранной ячейки
$sql = "SELECT value FROM sensor_data WHERE sensor_id = $cell ORDER BY timestamp DESC  LIMIT 6";

$result = $conn->query($sql);

if ($result->num_rows < 6) {
    // Если записей меньше 6, считаем что изменения есть
    header("Location: index.html?changes=true");
    exit();
}

$values = [];
while ($row = $result->fetch_assoc()) {
    $values[] = (float)$row['value'];
}

// Проверяем отклонения
$reference = $values[0]; // Самое последнее значение
$hasChanges = false;

foreach ($values as $value) {
    if (abs($value - $reference) > 1) {
        $hasChanges = true;
        break;
    }
}

// Закрываем соединение
$conn->close();

// Перенаправляем с результатом проверки
if ($hasChanges) {
    header("Location: index.html?changes=true");
} else {
    header("Location: index.html?changes=false");
}
exit();
?>
