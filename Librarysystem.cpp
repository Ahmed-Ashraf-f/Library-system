#include <bits/stdc++.h>
using namespace std;

class Logger {
public:
    static void log(const string& message) {
        cout << "[LOG] " << message << '\n';
    }

    static void logBorrow(const string& user_name, const string& book_name) {
        cout << "[LOG] " << user_name << " borrowed " << book_name << '\n';
    }

    static void logReturn(const string& user_name, const string& book_name) {
        cout << "[LOG] " << user_name << " returned " << book_name << '\n';
    }

    static void logExit() {
        cout << "[LOG] Program exited.\n";
    }
};

struct User {
    string name = "";
    int id{};
    set<int> borrowed_id;

    bool operator<(const User& x) const {
        return tie(name, id) < tie(x.name, x.id);
    }

    bool find(const int& book_id) {
        auto it = borrowed_id.find(book_id);
        if (it != borrowed_id.end()) {
            borrowed_id.erase(it);
            return true;
        }
        return false;
    }

    bool print_books() const {
        if (borrowed_id.empty()) return false;
        bool x = false;
        for (auto it : borrowed_id) {
            if (x) cout << " , ";
            cout << it;
            x = true;
        }
        return true;
    }
};

struct Book {
    int id = 0, quantity = 0;
    string name = "";
    set<User> borrow_users;

    void find(const User& u) {
        auto it = borrow_users.find(u);
        borrow_users.erase(it);
    }
};

bool sort_by_id(const Book& x, const Book& y) {
    return x.id < y.id;
}

bool sort_by_name(const Book& x, const Book& y) {
    return x.name < y.name;
}

bool is_prefix_of(const string& s, const string& t) {
    if (t.size() < s.size()) return false;
    return (t.substr(0, s.size()) == s);
}

struct Library_system {
    vector<Book> books;
    vector<User> users;

    void add_book() {
        cout << "Enter book info : id & name & total quantity : ";
        Book new_book;
        cin >> new_book.id >> new_book.name >> new_book.quantity;

        if (new_book.quantity < 0) {
            cout << "Error: Book quantity must be non-negative.\n";
            return;
        }

        books.push_back(new_book);
        Logger::log("Added book: " + new_book.name);
    }

    void search_by_prefix(const string& s) {
        bool found = false;
        for (const auto& book : books) {
            if (is_prefix_of(s, book.name)) {
                cout << book.name << '\n';
                found = true;
            }
        }
        if (!found) {
            cout << "No books with such prefix\n";
        }
    }

    void who_borrowed_by_name(const string& s) {
        bool found = false;
        for (const auto& book : books) {
            if (book.name == s) {
                found = true;
                for (const auto& user : book.borrow_users) {
                    cout << user.name << '\n';
                }
                if (book.borrow_users.empty()) {
                    cout << "No user has borrowed that book.\n";
                }
                break;
            }
        }
        if (!found) {
            cout << "Invalid book name !\n";
        }
    }

    void print_by_id() {
        sort(books.begin(), books.end(), sort_by_id);
        for (const auto& book : books) {
            cout << "id = " << book.id << " name = " << book.name << " total_quantity : " << book.quantity << " total_borrowed : " << book.borrow_users.size() << '\n';
        }
    }

    void print_by_name() {
        sort(books.begin(), books.end(), sort_by_name);
        for (const auto& book : books) {
            cout << "id = " << book.id << " name = " << book.name << " total_quantity : " << book.quantity << " total_borrowed : " << book.borrow_users.size() << '\n';
        }
    }

    void add_user() {
        cout << "Enter user name & national id : ";
        User new_user;
        cin >> new_user.name >> new_user.id;

        users.push_back(new_user);
        Logger::log("Added user: " + new_user.name);
    }

    void user_borrow_book() {
        cout << "Enter username and book name : ";
        string u, b;
        cin >> u >> b;

        auto user_it = find_if(users.begin(), users.end(), [&](const auto& user) { return user.name == u; });
        if (user_it == users.end()) {
            cout << "Invalid username !\n";
            return;
        }

        auto book_it = find_if(books.begin(), books.end(), [&](const auto& book) { return book.name == b; });
        if (book_it == books.end()) {
            cout << "Invalid book name !\n";
            return;
        }

        if (book_it->quantity == 0) {
            cout << "This book is out of stock.\n";
            return;
        }

        book_it->borrow_users.insert(*user_it);
        user_it->borrowed_id.insert(book_it->id);
        book_it->quantity--;

        Logger::logBorrow(u, b);
    }

    void user_return_book() {
        cout << "Enter username and book name : ";
        string u, b;
        cin >> u >> b;

        auto user_it = find_if(users.begin(), users.end(), [&](const auto& user) { return user.name == u; });
        if (user_it == users.end()) {
            cout << "Invalid username !\n";
            return;
        }

        auto book_it = find_if(books.begin(), books.end(), [&](const auto& book) { return book.name == b; });
        if (book_it == books.end()) {
            cout << "Invalid book name !\n";
            return;
        }

        if (!user_it->find(book_it->id)) {
            cout << "User didn't borrow that book !\n";
            return;
        }

        book_it->borrow_users.erase(*user_it);
        user_it->borrowed_id.erase(book_it->id);

        Logger::logReturn(u, b);
    }

    void print_users() {
        bool x = true;
        for (const auto& user : users) {
            x = false;
            cout << "user " << user.name << " id " << user.id << " borrowed books id : ";
            if (!user.print_books()) {
                cout << "No borrowed books.";
            }
            cout << '\n';
        }
        if (x) {
            cout << "There are no users in the system.\n";
        }
    }
};

int option() {
    cout << "Library Menu: \n"
        << "1) add_book\n"
        << "2) search_books_by_prefix\n"
        << "3) print_who_borrowed_book_by_name\n"
        << "4) print_library_by_id\n"
        << "5) print_library_by_name\n"
        << "6) add_user\n"
        << "7) user_borrow_book\n"
        << "8) user_return_book\n"
        << "9) print_users\n"
        << "10) Exit\n\n";
    cout << "Enter your choice [1 : 10] ";
    int x;
    cin >> x;
    cout << "------------------\n";
    return x;
}

void run_system() {
    Library_system library;
    string s;
    while (true) {
        switch (option()) {
        case 1:
            library.add_book();
            cout << "------------------\n";
            break;
        case 2:
            cout << "Enter book name prefix : ";
            cin >> s;
            library.search_by_prefix(s);
            cout << "------------------\n";
            break;
        case 3:
            cout << "Enter book name : ";
            cin >> s;
            library.who_borrowed_by_name(s);
            cout << "------------------\n";
            break;
        case 4:
            library.print_by_id();
            cout << "------------------\n";
            break;
        case 5:
            library.print_by_name();
            cout << "------------------\n";
            break;
        case 6:
            library.add_user();
            cout << "------------------\n";
            break;
        case 7:
            library.user_borrow_book();
            cout << "------------------\n";
            break;
        case 8:
            library.user_return_book();
            cout << "------------------\n";
            break;
        case 9:
            library.print_users();
            cout << "------------------\n";
            break;
        case 10:
            Logger::logExit();
            return;
        default:
            cout << "Enter valid option!! \n\n";
            break;
        }
    }
}

int main() {
    run_system();
    return 0;
}
