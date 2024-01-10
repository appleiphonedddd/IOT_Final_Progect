<?php
// Initialize the session
session_start();
require_once("../final/config.php");
// Check if the user is logged in, if not then redirect him to login page

?>
<!DOCTYPE html>
<html >
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>管理會員資料</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-0evHe/X+R7YkIZDRvuzKMRqM+OrBnVFBL6DOitfPri4tjfHxaWutUpFmBp4vmVor" crossorigin="anonymous">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+TC:wght@500&family=Open+Sans:wght@300&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="css/flexslider.css" type="text/css">
    
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/js/bootstrap.bundle.min.js" integrity="sha384-pprn3073KE6tl6bjs2QrFaJGz5/SUsLqktiwsUTF55Jfv3qYSDhgCecCxMW52nD2" crossorigin="anonymous"></script>
</head>
<body>
    <div class="container">
      <header class="d-flex flex-wrap align-items-center justify-content-center justify-content-md-between py-3 mb-4 border-bottom">
        <a href="../final/index.php" class="d-flex align-items-center col-md-7 mb-2 mb-md-0 text-dark text-decoration-none">
          <span class="fs-2">ICMN LAB內部成員</span>
        </a>

        <ul class="nav col-12 col-md-auto mb-2 justify-content-center mb-md-0">
          <li><a href="" class="nav-link link-dark">借用情形</a></li>


      </header>
    </div>
    <div class="container">
        <h2>借用情形</h2>
        <table border="1"> 
        <br>
        
        <?php


///////////////////////////////////////////////////
        $sql = "select * from mem_info ";
        $result = mysqli_query($link,$sql);
        // 顯示表格的標題列
        print "<tr>";
        print "<td bgcolor=#02C874  width='5%' align='center'><b>編號</b></td>";
        print "<td bgcolor=#FF9224  width='30%' align='center'><b>會員帳號</b></td>";
        print "<td bgcolor=#FFA042  width='30%' align='center'><b>借用/規還時間</b></td>";
        print "<td bgcolor=#FFAF60  width='30%' align='center'><b>使用狀態</b></td>";
        print "</tr>";
        // 巢狀迴圈
        while($row = mysqli_fetch_assoc($result)){
            $n = $row["num"];
            print "<tr>";
            print "<td width='50' align='center'>";
            echo $row["num"];
            print "</td>";
            print "<td width='100' align='center'>";
            echo $row["id"];
            echo "</td>";
            print "<td width='100' align='center'>";
            echo $row["time"];
            echo "</td>";
            print "<td width='50' align='center'>";
            echo $row["mode"];

    }
        ?>
        </table><br>
    </div>
    </div>
</body>
</html>