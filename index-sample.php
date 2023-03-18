<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="style.css">
<title>Lorem Ipsum Inc.</title>
</head>
<body>

<div class="header">
  <h3>Welcome to Lorem Ipsum Inc.!</h3>
</div>

<div class="navbar-container">
  <nav class="navbar">
    <a href="index.php" class="current-page">Home</a>
    <a href="software.php">Software</a>
  </nav>
</div>

<div class="row">
  <div class="leftcolumn">
  <?php
    // Get the current page number from the URL, default to 1 if not set
    $page = isset($_GET['page']) ? (int)$_GET['page'] : 1;
    // Number of posts to display per page
    $postsPerPage = 2;

    // Get an array of all the post filenames
    $postFiles = array_reverse(glob("./posts/*.html"));

    // Calculate the total number of pages
    $totalPages = ceil(count($postFiles) / $postsPerPage);

    // Ensure the current page is within the valid range
    if ($page < 1) {
        $page = 1;
    } elseif ($page > $totalPages) {
        $page = $totalPages;
    }

    // Calculate the start and end indices for the posts to display on the current page
    $startIndex = ($page - 1) * $postsPerPage;
    $endIndex = min($startIndex + $postsPerPage, count($postFiles));

    // Display the posts for the current page
    for ($i = $startIndex; $i < $endIndex; $i++) {
        readfile($postFiles[$i]);
    }

    ?>


  </div>

  <div class="rightcolumn">
  <div class="card">
    <h2>About Us</h2>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer auctor purus at facilisis tempor. Morbi euismod libero elit, vitae malesuada nisl vehicula ut. Sed aliquam metus at sapien lacinia, a luctus elit eleifend.</p>

    <p>Nam tristique vehicula libero, id sodales sem convallis id. Fusce posuere facilisis nibh, sit amet elementum sapien tristique eu. Donec viverra, urna ut consequat consequat, ipsum urna blandit libero, nec aliquet odio orci a sem.</p>

    <p>Phasellus at nunc eu nulla tempus pellentesque. Aenean id sollicitudin lorem, non malesuada lectus. Etiam ut sollicitudin ligula. Integer vitae tristique lorem.</p>
  </div>

  <div class="card">
    <h3>Contact Us</h3>
    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris non risus non elit interdum feugiat vitae ut mauris.</p>
    <p><strong>Email:</strong> lorem@ipsum.com</p>
    <p><strong>Phone:</strong> (123) 456-7890</p>
    <p><strong>Address:</strong> <br>123 Lorem Ipsum St. <br>City, State <br>12345, Country</p>
  </div>

  </div>
</div>
  
<div class="pagination-container">
  <?php
  // Display the pagination controller
  echo '<div class="pagination">';
  for ($i = 1; $i <= $totalPages; $i++) {
      if ($i == $page) {
          echo '<span class="current-page">' . $i . '</span>';
      } else {
          echo '<a href="?page=' . $i . '">' . $i . '</a>';
      }
  }
  echo '</div>';
  ?>
</div>

<div class="footer">
  <p>Copyright 2023 Lorem Ipsum Inc.</p>
</div>

</body>
</html>
