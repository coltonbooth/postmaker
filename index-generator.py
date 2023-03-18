# Get the user input
title = input("Enter the website title: ")
welcome_message = input("Enter the top banner text: ")
about_us_title = input("Enter the About Us title: ")
about_us_text = input("Enter the About Us text: ")
contact_us_title = input("Enter the title you want for the contact page (usually \"Contact Us\"): ")
contact_us_text = input("Enter the contact page text: ")
email = input("(Optional) Enter the email address: ")
phone = input("(Optional) Enter the phone number: ")
address = input("(Optional) Enter the address: ")
copyright = input("Enter the Footer text: ")

# Create the index.php file
with open('index.php', 'w') as f:
    f.write(f"""<!DOCTYPE html>
        <html>
        <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="style.css">
        <title>{title}</title>
        </head>
        <body>

        <div class="header">
        <h3>{welcome_message}</h3>
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
            if ($page < 1) {{
                $page = 1;
            }} elseif ($page > $totalPages) {{
                $page = $totalPages;
            }}

            // Calculate the start and end indices for the posts to display on the current page
            $startIndex = ($page - 1) * $postsPerPage;
            $endIndex = min($startIndex + $postsPerPage, count($postFiles));

            // Display the posts for the current page
            for ($i = $startIndex; $i < $endIndex; $i++) {{
                readfile($postFiles[$i]);
            }}

            ?>


        </div>

        <div class="rightcolumn">
        <div class="card">
            <h2>{about_us_title}</h2>
            <p>{about_us_text}</p>
        </div>

        <div class="card">
            <h3>{contact_us_title}</h3>
            <p>{contact_us_text}</p>
            {f"<p><strong>Email:</strong> {email}</p>" if email else ""}
            {f"<p><strong>Phone:</strong> {phone}</p>" if phone else ""}
            {f"<p><strong>Address:</strong> <br>{address}</p>" if address else ""}
        </div>

        </div>
        </div>
        
        <div class="pagination-container">
        <?php
        // Display the pagination controller
        echo '<div class="pagination">';
        for ($i = 1; $i <= $totalPages; $i++) {{
            if ($i == $page) {{
                echo '<span class="current-page">' . $i . '</span>';
            }} else {{
                echo '<a href="?page=' . $i . '">' . $i . '</a>';
            }}
        }}
        echo '</div>';
        ?>
        </div>

        <div class="footer">
        <p>{copyright}</p>
        </div>

        </body>
        </html>
    """)
