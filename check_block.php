<?php
header('Content-Type: application/json');
$servername = "localhost";
$username = "ESP32";
$password = "1";
$dbname = "my_database";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) die("Connection failed");

$result = $conn->query("SELECT blocked_until FROM block_status WHERE id = 1");
$blocked = false;
$time_left = 0;

if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();
    $blocked_until = $row['blocked_until'];
    if (time() < $blocked_until) {
        $blocked = true;
        $time_left = $blocked_until - time();
    } else {
        // Удаляем устаревшую блокировку
        $conn->query("DELETE FROM block_status WHERE id = 1");
    }
}

echo json_encode([
    "blocked" => $blocked,
    "time_left" => $time_left
]);

$conn->close();
?>
