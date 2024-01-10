<?php
// Initialize the session
session_start();
require_once("../final/config.php");
// Check if the user is logged in, if not then redirect him to login page
if(!isset($_SESSION["loggedin"]) || $_SESSION["loggedin"] !== true){
    header("location: personlogin.php");
    exit;
}
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
    <link rel="stylesheet" href="css/style.css" type="text/css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/js/bootstrap.bundle.min.js" integrity="sha384-pprn3073KE6tl6bjs2QrFaJGz5/SUsLqktiwsUTF55Jfv3qYSDhgCecCxMW52nD2" crossorigin="anonymous"></script>
</head>
<body>
    <div class="container">
      <header class="d-flex flex-wrap align-items-center justify-content-center justify-content-md-between py-3 mb-4 border-bottom">
        <a class="d-flex align-items-center col-md-3 mb-2 mb-md-0 text-dark text-decoration-none">
          <span class="fs-2">ICMN LAB內部成員</span>
        </a>

        <ul class="nav col-12 col-md-auto mb-2 justify-content-center mb-md-0">
          <li><a href="../final/s_member.php" class="nav-link link-dark">會員資料</a></li>

        </ul>
        <div class="col-md-3 text-end">
            <h5 class="me-2" >Hi, <b><?php echo htmlspecialchars($_SESSION["username"]); ?></b></h5>
            <a href="logout.php" class="btn btn-outline-danger me-2">登出</a>
        </div>
      </header>
    </div>
    <div class="container">
        <h2>會員資料</h2>
        <table border="1"> 
        <br>
        <?php
        echo "<br>";
        if ($_GET["del"]){
            $del = $_GET["del"];
            $sql = "delete from users where id= '$del'" ;
            $result = mysqli_query($link, $sql) ;
            echo "您已刪除此會員:".$del;
        }
        $sql = "select * from users ";
        $result = mysqli_query($link,$sql);
        // 顯示表格的標題列
        print "<tr>";
        print "<td bgcolor=#FFD78C  width='5%' align='center'><b>編號</b></td>";
        print "<td bgcolor=#FFD78C  width='30%' align='center'><b>會員帳號</b></td>";
        print "<td bgcolor=#FFD78C  width='30%' align='center'><b>創建時間</b></td>";
        print "<td bgcolor=#FFD78C  width='10%' align='center'><b>選擇刪除資料</b></td>";
        print "</tr>";
        // 巢狀迴圈
        while($row = mysqli_fetch_assoc($result)){
                $n = $row["id"];
                print "<tr>";
                print "<td width='50' align='center'>";
                echo $row["id"];
                print "</td>";
                print "<td width='100' align='center'>";
                echo $row["username"];
                echo "</td>";
                print "<td width='50' align='center'>";
                echo $row["created_at"];
                echo "</td>";
                print "<td width='50' align='center'>";
                print "<a href=s_member.php?del=$n>";
                echo "<img src=picture/icon-delete.png width=25 alt=Remove Item />";
                echo "</a>";
                echo "</td>";
            print "</tr>";
        }
        ?>
        </table><br>
    </div>
    </div>
</body>
</html>