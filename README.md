# postmaker
 postmaker is a simple and lightweight Python and PHP application for creating and editing blog posts. It generates tiny HTML files for each post and displays them on a fast-loading PHP website.
 
 ## Features

Create and edit blog posts using a simple command-line interface.
Posts are stored as individual HTML files in a directory.
Pagination support for displaying a specified number of posts per page.
A responsive design that works well on desktop and mobile devices.

## Usage
### index.php
The index.php file is a PHP script that displays the blog posts on a website. It reads the HTML files from `./posts` and displays them in reverse chronological order, with pagination support.

To use index.php, you will need a web server with PHP support. Copy the index.php file, the style.css file, and the posts directory (with the HTML files) to your web server's document root. Then, navigate to the URL of the index.php file in your web browser to view the blog posts.

### index-generator.py
index-generator.py is used to generate the initial index.php file from the text it prompts you to enter. This is useful if you don't want to edit the index-sample.php included in the repo. Just run `python index.generator.py` to get started.

### C++ version of `postmaker` desktop
postmaker.cpp is where you'll find the source to the C++ version of postmaker. This version uses htmlcxx to format the post html.

`g++ -std=c++11 -I/opt/homebrew/Cellar/htmlcxx/0.87/include/ postmaker.cpp -o postmaker -L/opt/homebrew/lib -lhtmlcxx -lncurses` is the command I used to compile it on macOS (Arm). I will update this readme once I compile it on my linux box. 

You'll need to `brew install htmlcxx` 

Usage is the same as the python version below. Simply run `./postmaker` in the directory you compiled it and you'll be presented with the main screen where you can either write a new post or edit a previous one.

### Python version of `postmaker` desktop
The postmaker.py script is a Python application that allows you to create and edit blog posts using a simple command-line interface. It uses the curses library to provide a text-based user interface.

To run the application, navigate to the directory where postmaker.py is located and run the following command:

```
python postmaker.py
```

This will launch the main screen, where you can choose to create a new post or edit an existing post.

### Create a new post

If you choose to create a new post, you will be prompted to enter the title, date, and content of the post. Press Ctrl+G to save the post and exit the editor. The post will be saved as an HTML file in the posts directory.

### Edit an existing post

If you choose to edit an existing post, you will be presented with a list of existing posts. Choose the post you want to edit by entering its corresponding number. You will then be able to edit the title, date, and content of the post. Press Ctrl+G to save the changes and exit the editor.


## Customization

You can customize the number of posts displayed per page by modifying the $postsPerPage variable in the index.php file.

You can also customize the appearance of the website by modifying the style.css file.

## License

This project is open source and available under the MIT License.
