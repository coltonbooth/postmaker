#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <curses.h>
#include <dirent.h>
#include <unistd.h>
#include "htmlcxx/html/ParserDom.h"
#include "htmlcxx/html/tree.h"


using namespace std;
using namespace htmlcxx;


string posts_dir = "/Users/colton/Documents/BoothSoftware";

string get_post_file_name();
void create_new_post(WINDOW *main_win);
void edit_post(WINDOW *main_win, string file_name);
void main_screen();

bool is_closing_tag(const HTML::Node& node) {
    return (node.isTag() && !node.text().empty() && node.text()[0] == '/');
}

bool is_single_tag(const HTML::Node& node) {
    static const set<string> single_tags = {"area", "base", "br", "col", "embed", "hr", "img", "input", "link", "meta", "param", "source", "track", "wbr"};

    return single_tags.find(node.tagName()) != single_tags.end();
}

string get_tag_attributes(const HTML::Node& node) {
    string attrs_str = "";
    for (const auto& attr : node.attributes()) {
        attrs_str += " " + attr.first + "=\"" + attr.second + "\"";
    }
    return attrs_str;
}

void traverse_dom_tree(const tree<HTML::Node>& dom, tree<HTML::Node>::iterator it, string& result, int level) {
    string indent(level * 4, ' ');

    if (it->isTag() && !is_closing_tag(*it)) {
        result += indent + "<" + it->tagName() + get_tag_attributes(*it) + ">";
        if (!is_single_tag(*it)) {
            result += "\n";
        }
    } else if (!it->isTag() && !it->isComment()) {
        string text = it->text();
        if (!text.empty() && text[0] != '\n') {
            result += indent + it->text() + "\n";
        }
    }

    for (unsigned int i = 0; i < dom.number_of_children(it); ++i) {
        traverse_dom_tree(dom, dom.child(it, i), result, level + 1);
    }

    if (it->isTag() && !is_closing_tag(*it) && !is_single_tag(*it)) {
        result += indent + "</" + it->tagName() + ">\n";
    }
}


string lint_html(const string &input_html) {
    HTML::ParserDom parser;
    tree<HTML::Node> dom = parser.parseTree(input_html);

    string formatted_html;
    if (dom.number_of_children(dom.begin()) > 0) {
        traverse_dom_tree(dom, dom.child(dom.begin(), 0), formatted_html, 0);
    }

    return formatted_html;
}


string get_post_file_name() {
    DIR *dir;
    struct dirent *ent;
    int post_number = 0;
    if ((dir = opendir(posts_dir.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            post_number++;
        }
        closedir(dir);
    }
    stringstream ss;
    ss << post_number + 1;
    string post_number_str = ss.str();
    if (post_number_str.length() == 1) {
        post_number_str = "0" + post_number_str;
    }
    return post_number_str + ".html";
}

void create_new_post(WINDOW *main_win) {
    string file_name = get_post_file_name();
    edit_post(main_win, file_name);
}

void edit_post(WINDOW *main_win, string file_name) {
    // Clear the main_win
    wclear(main_win);
    wrefresh(main_win);
    // Initialize the curses screen
    initscr();
    noecho();
    cbreak();

    // Load the existing post content if editing an existing post
    string post_content = "";
    ifstream post_file(posts_dir + "/" + file_name);
    if (post_file.is_open()) {
        string line;
        while (getline(post_file, line)) {
            post_content += line + "\n";
        }
        post_file.close();
    }

    // Create a new window for the title
    WINDOW *title_win = newwin(1, COLS, 0, 0);
    waddstr(title_win, "Title: ");

    // Create a new window for the date
    WINDOW *date_win = newwin(1, COLS, 1, 0);
    waddstr(date_win, "Date (e.g. March 12th, 2023): ");

    // Create a new window for the content
    WINDOW *content_win = newwin(LINES - 3, COLS, 3, 0);
    waddstr(content_win, "Content (Press Ctrl+G to save and exit):\n");

    char title[256];
    char date[256];

    // Load the existing post details if editing an existing post
    if (post_content != "") {
        string title = "";
        string date = "";
        string content = "";
        bool in_title = false;
        bool in_date = false;
        bool in_content = false;
        stringstream ss(post_content);
        string line;
        while (getline(ss, line)) {
            if (line.find("<h2>") != string::npos) {
                in_title = true;
            } else if (line.find("</h2>") != string::npos) {
                in_title = false;
            } else if (line.find("<h5>") != string::npos) {
                in_date = true;
            } else if (line.find("</h5>") != string::npos) {
                in_date = false;
            } else if (line.find("<p>") != string::npos) {
                in_content = true;
            } else if (line.find("</p>") != string::npos) {
                in_content = false;
            } else if (in_title) {
                title += line;
            } else if (in_date) {
                date += line;
            } else if (in_content) {
                content += line + "\n";
            }
        }

        waddstr(title_win, title.c_str());
        waddstr(date_win, date.c_str());
        waddstr(content_win, content.c_str());
    }

    // Get the title from the user
    echo();
    wgetstr(title_win, title);

    // Get the date from the user
    wgetstr(date_win, date);

    // Get the content from the user
    wmove(content_win, 1, 0);
    noecho();
    vector<char> content;
    while (true) {
        int ch = wgetch(content_win);
        if (ch == KEY_ENTER || ch == '\n') {
            content.push_back('\n');
            waddstr(content_win, "\n");
        } else if (ch == KEY_BACKSPACE || ch == '\x7f') {
            if (!content.empty() && content.back() != '\n') {
                int y, x;
                getyx(content_win, y, x);
                wmove(content_win, y, x - 1);
                wdelch(content_win);
                content.pop_back();
            }
        } else if (ch == '\x07') { // Ctrl+G
            break;
        } else {
            content.push_back(ch);
            waddstr(content_win, string(1, ch).c_str());
        }
    }

    // Cleanup
    noecho();
    cbreak();
    endwin();

    // Format the content into paragraphs
    string formatted_content = "";
    stringstream ss(string(content.begin(), content.end()));
    string line;
    while (getline(ss, line)) {
        if (line.find_first_not_of(' ') != string::npos) {
            formatted_content += "<p>" + line + "</p>\n";
        }
    }

    // Create the post content
    post_content = "<div class=\"card\">\n"
        "<h2>" + string(title) + "</h2>\n"
        "<h5>" + string(date) + "</h5>\n"
        + formatted_content +
        "</div>";
    
    // Lint the HTML content
    string linted_post_content = lint_html(post_content);

    // Write the post content to the file
    ofstream post_file_out(posts_dir + "/" + file_name);
    if (post_file_out.is_open()) {
        post_file_out << linted_post_content;
        post_file_out.close();
    }
}

void main_screen() {
    // Initialize the curses screen
    initscr();
    noecho();
    cbreak();

    // Create a new window for the main screen
    WINDOW *main_win = newwin(LINES, COLS, 0, 0);
    waddstr(main_win, "Welcome to the Booth Software Post Editor\n");
    waddstr(main_win, "\n");
    waddstr(main_win, "Please choose an option:\n");
    waddstr(main_win, "1. Create a new post\n");
    waddstr(main_win, "2. Edit an existing post\n");
    waddstr(main_win, "\n");
    waddstr(main_win, "Enter your choice (1 or 2): ");

    // Get the user's choice
    while (true) {
        int choice = wgetch(main_win);
        if (choice == '1') {
            create_new_post(main_win);
            break;
        } else if (choice == '2') {
            // Get the list of existing post files
            DIR *dir;
            struct dirent *ent;
            vector<string> post_files;
            if ((dir = opendir(posts_dir.c_str())) != NULL) {
                while ((ent = readdir(dir)) != NULL) {
                    post_files.push_back(ent->d_name);
                }
                closedir(dir);
            }
            if (post_files.empty()) {
                waddstr(main_win, "\nNo existing posts found.");
                break;
            }

            wclear(main_win);
            waddstr(main_win, "Please choose a post to edit:\n");
            for (int i = 0; i < post_files.size(); i++) {
                waddstr(main_win, (to_string(i + 1) + ". " + post_files[i] + "\n").c_str());
            }
            waddstr(main_win, "\nEnter the number of the post you want to edit: ");

            // Get the user's choice
            while (true) {
                int post_choice = wgetch(main_win) - '0';
                if (1 <= post_choice && post_choice <= post_files.size()) {
                    edit_post(main_win, post_files[post_choice - 1]);
                    break;
                }
            }

            break;
        }
    }

    // Cleanup
    noecho();
    cbreak();
    endwin();
}




int main() {
    main_screen();
    return 0;
}
