<?php
header("Content-Type: application/json");

$conn = new mysqli("localhost", "ESP32", "1", "my_database");

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT date, cell FROM project_base";
$result = $conn->query($sql);

if (!$result) {
    http_response_code(500); 
    echo json_encode(["error" => "Database query failed: " . $conn->error]);
    exit;
}

// // Получаем данные
// if ($result->num_rows > 0) {
    // $row = $result->fetch_assoc();
	
	// // Формируем JSON-ответ
    // echo json_encode($row);
// } else {
    // echo json_encode(["error" => "No data found"]);
// }

$response = [];
if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $response["date"] = isset($row["date"]) ? $row["date"] : null;
    $response["cell"] = isset($row["cell"]) ? intval($row["cell"]) : null;
} else {
    $response["error"] = "No data found";
}

echo json_encode($response);

$conn->close();
?>
