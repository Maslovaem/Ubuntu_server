<?php
header("Content-Type: application/json");

$conn = new mysqli("localhost", "ESP32", "1", "my_database");

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT date, cell FROM project_base WHERE date < CURDATE()";
$result = $conn->query($sql);

if (!$result) {
    http_response_code(500); 
    echo json_encode(["error" => "Database query failed: " . $conn->error]);
    exit;
}

$response = [];
while ($row = $result->fetch_assoc()) {
    $response[] = [
        "date" => $row["date"],
        "cell" => intval($row["cell"])
    ];
}

echo json_encode($response);

$conn->close();
?>