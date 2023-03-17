import curses
import os

posts_dir = "/var/www/posts"

def get_post_file_name():
    post_number = len(os.listdir(posts_dir)) + 1
    return f"{post_number:02d}.html"

def create_new_post():
    file_name = get_post_file_name()
    edit_post(file_name)

def edit_post(file_name):
    # Initialize the curses screen
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    # Load the existing post content if editing an existing post
    post_content = ""
    if os.path.exists(os.path.join(posts_dir, file_name)):
        with open(os.path.join(posts_dir, file_name), 'r') as f:
            post_content = f.read()

    try:
        # Create a new window for the title
        title_win = curses.newwin(1, curses.COLS, 0, 0)
        title_win.addstr("Title: ")

        # Create a new window for the date
        date_win = curses.newwin(1, curses.COLS, 1, 0)
        date_win.addstr("Date (e.g. March 12th, 2023): ")

        # Create a new window for the content
        content_win = curses.newwin(curses.LINES - 3, curses.COLS, 3, 0)
        content_win.addstr("Content (Press Ctrl+G to save and exit):\n")

        # Load the existing post details if editing an existing post
        if post_content:
            from bs4 import BeautifulSoup
            soup = BeautifulSoup(post_content, 'html.parser')
            title = soup.h2.text
            date = soup.h5.text
            content = '\n'.join([p.text for p in soup.find_all('p')])

            title_win.addstr(title)
            date_win.addstr(date)
            content_win.addstr(content)

        # Get the title from the user
        curses.echo()
        title = title_win.getstr(0, 7).decode()

        # Get the date from the user
        date = date_win.getstr(0, 32).decode()

        # Get the content from the user
        content_win.move(1, 0)
        curses.noecho()
        content = []
        while True:
            ch = content_win.getch()
            if ch == curses.KEY_ENTER or ch == ord('\n'):
                content.append('\n')
                content_win.addstr('\n')
            elif ch == curses.KEY_BACKSPACE or ch == ord('\x7f'):
                if content and content[-1] != '\n':
                    y, x = content_win.getyx()
                    content_win.move(y, x - 1)
                    content_win.delch()
                    content.pop()
            elif ch == ord('\x07'): # Ctrl+G
                break
            else:
                content.append(chr(ch))
                content_win.addstr(chr(ch))

    finally:
        # Cleanup
        curses.nocbreak()
        curses.echo()
        curses.endwin()

    # Format the content into paragraphs
    formatted_content = '\n'.join([f"<p>{paragraph}</p>" for paragraph in ''.join(content).split('\n') if paragraph.strip()])

    # Create the post content
    post_content = f"""<div class="card">
        <h2>{title}</h2>
        <h5>{date}</h5>
        {formatted_content}
    </div>"""

    # Write the post content to the file
    with open(os.path.join(posts_dir, file_name), 'w') as f:
        f.write(post_content)

def main_screen():
    # Initialize the curses screen
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    try:
        # Create a new window for the main screen
        main_win = curses.newwin(curses.LINES, curses.COLS, 0, 0)
        main_win.addstr("Welcome to the Booth Software Post Editor\n")
        main_win.addstr("\n")
        main_win.addstr("Please choose an option:\n")
        main_win.addstr("1. Create a new post\n")
        main_win.addstr("2. Edit an existing post\n")
        main_win.addstr("\n")
        main_win.addstr("Enter your choice (1 or 2): ")

        # Get the user's choice
        while True:
            choice = main_win.getch()
            if choice == ord('1'):
                create_new_post()
                break
            elif choice == ord('2'):
                # Get the list of existing post files
                post_files = sorted(os.listdir(posts_dir))
                if not post_files:
                    main_win.addstr("\nNo existing posts found.")
                    break

                main_win.clear()
                main_win.addstr("Please choose a post to edit:\n")
                for i, post_file in enumerate(post_files):
                    main_win.addstr(f"{i + 1}. {post_file}\n")
                main_win.addstr("\nEnter the number of the post you want to edit: ")

                # Get the user's choice
                while True:
                    post_choice = main_win.getch() - ord('0')
                    if 1 <= post_choice <= len(post_files):
                        edit_post(post_files[post_choice - 1])
                        break

                break

    finally:
        # Cleanup
        curses.nocbreak()
        curses.echo()
        curses.endwin()

main_screen()

