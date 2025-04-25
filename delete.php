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



$password = (int)$_POST['password'];

$medicine = (int)$_POST['medicine'];

$cell = (int)$_POST['cell'];



if ($password === $correct_password){

  $sql = " DELETE FROM project_base WHERE cell = '$cell' AND medicine = '$medicine'";



  if ($conn->query($sql) === TRUE) {

      echo "Удаление произошло успешно";



  } else {

      echo "Ошибка удаления: " . $sql . "<br>" . $conn->error;

  }

}



else{

  echo "Неверный пароль.";

}

$conn->close();



header("refresh:3;url=index.html");

?>



