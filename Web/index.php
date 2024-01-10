<!doctype html>
<html >
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>首頁</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-0evHe/X+R7YkIZDRvuzKMRqM+OrBnVFBL6DOitfPri4tjfHxaWutUpFmBp4vmVor" crossorigin="anonymous">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Noto+Sans+TC:wght@500&family=Open+Sans:wght@300&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="css/flexslider.css" type="text/css">
    <link rel="stylesheet" href="css/style.css" type="text/css">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0-beta1/dist/js/bootstrap.bundle.min.js" integrity="sha384-pprn3073KE6tl6bjs2QrFaJGz5/SUsLqktiwsUTF55Jfv3qYSDhgCecCxMW52nD2" crossorigin="anonymous"></script>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.6.2/jquery.min.js"></script>
    <script src="js/jquery.flexslider.js"></script>
    <script type="text/javascript" charset="utf-8">
      $(window).load(function() {
        $('.flexslider').flexslider({
          slideshowSpeed: 2000,
          directionNav: false,
          animation: "slide",
        });
      });
    </script>    
<style>
    /* Inline style for the body background image */
    body {
      background-image: url('picture/pivvvv.png');
      background-size: cover;
      background-repeat: no-repeat;
    }
  </style>
  </head>
  <body>
      <div class="container">
        <header class="d-flex flex-wrap align-items-center justify-content-center justify-content-md-between py-3 mb-4 border-bottom">
          <a href="../final/index.php" class="d-flex align-items-center col-md-7 mb-2 mb-md-0 text-dark text-decoration-none">
          <h1>ICMN成員系統</h1>
          </a>
        </header>
      </div>
        <div class="body">
          <div class="container">
            <button type="button"onclick="location.href='../final/s_member.php'"><img src="picture/member.png" width="250px" height="250px";></button>
            <button type="button"onclick="location.href='../final/check.php'"><img src="picture/list.png" width="250px" height="250px"></button>
            <button type="button"onclick="location.href='http://icmnlab.ndhu.edu.tw/'"><img src="picture/WEB1.png" width="250px" height="250px"></button>
          </div>
       </div>
  </body>
</html>