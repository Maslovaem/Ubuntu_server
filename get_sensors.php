<?php
header("Content-Type: application/json");

$conn = new mysqli("localhost", "ESP32", "1", "my_database");

$sensor_id = $_POST['sensor_id'];
$current_value = (float)$_POST['value'];

$result = $conn->query("SELECT value FROM sensor_data WHERE sensor_id = $sensor_id ORDER BY timestamp DESC LIMIT 5");

$all_differ = true;
$values_count = 0;

while ($row = $result->fetch_assoc()) {
    $last_value = (float)$row['value'];
    $values_count++;

    if (abs($current_value - $last_value) <= 1) {
        $all_differ = false;
        break;
    }
}

if ($all_differ && $values_count === 5) {
    $conn->query(" DELETE FROM project_base WHERE id = ( SELECT MAX(id) FROM ( SELECT id FROM project_base WHERE cell = $sensor_id ) AS tmp ) ");
    echo json_encode(["status" => "deleted"]);
}

$conn->query("INSERT INTO sensor_data (sensor_id, value) VALUES ($sensor_id, $current_value)");
echo json_encode(["status" => "updated"]);

$conn->close();
?>
