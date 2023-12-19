#include <iostream>
#include <algorithm>

using namespace std;
const int MAX_SIZE = 100;

struct book {
    int id;
    string name;
    int quantity, len;
    string users[MAX_SIZE];

    book() {
        len = 0;
    }

    book(int &id_, string &name_, int &quan) {
        id = id_;
        name = name_;
        quantity = quan;
        len = 0;
    }

    bool is_prefix(string &prefix) {
        return name.size() >= prefix.size() &&
               name.substr(0, prefix.size()) == prefix;
    }

    bool borrow(string &user_name) {//add the borrower
        if (len == quantity)
            return false;
        users[len++] = user_name;
        return true;
    }

    bool remove_user(string &user_name) {
        int index = -1;
        for (int i = 0; i < len; ++i) {
            if (users[i] == user_name)
                index = i;
        }
        if (index == -1) {
            return false;
        }
        len--;
        //Shifting left the remaining users after removing user with index "index"
        for (int i = index; i < len; ++i) {
            users[i] = users[i + 1];
        }
        return true;
    }

    void print_borrowers() {
        for (int i = 0; i < len; ++i) {
            cout << users[i] << " ";
        }
        cout << "\n";
    }

    void print() const {
        cout << "id : " << id << " ,name : " << name
             << ", Total : " << quantity << " ,Borrowed : " << len << "\n";
    }
};

struct user {
    int id;
    string name;
    int len, book_ids[MAX_SIZE];

    user() {
        len = 0;
    }

    user(const int &id_, const string &name_) {
        id = id_;
        name = name_;
        len = 0;
    }

    void borrow_book(int book_id) {
        book_ids[len++] = book_id;
    }

    bool return_book(int book_id) {
        int index = -1;
        for (int i = 0; i < len; ++i) {
            if (book_id == book_ids[i])
                index = i;
        }
        if (index == -1)
            return false;
        len--;
        for (int i = index; i < len; ++i) {
            book_ids[i] = book_ids[i + 1];
        }
        return true;
    }

    void print() {
        cout << "User : " << name << " ,id : " << id << " borrowed_books_ids : ";
        for (int i = 0; i < len; ++i) {
            cout << book_ids[i] << " ";
        }
        cout << "\n";
    }
};

struct Library {
    book books[MAX_SIZE];
    user users[MAX_SIZE];
    int book_len, user_len;

    Library() {
        book_len = user_len = 0;
    }

    void add_book(int &id, string &name, int quan) {
        if (book_len == MAX_SIZE)
            cout << "Sorry, library has reached its maximum capacity.\n";
        else
            cout << "Book was added successfully.\n";
        books[book_len] = book(id, name, quan);
        book_len++;
    }

    void find_by_prefix(string &prefix) {
        bool found = false;
        for (int i = 0; i < book_len; ++i) {
            if (books[i].is_prefix(prefix)) {
                cout << books[i].name << "\n";
                found = true;
            }
        }
        if (!found)
            cout << "There is no book with prefix : " << prefix << "\n";
    }

    void who_borrowed(string &name) {
        bool not_found = true;
        for (int i = 0; i < book_len; ++i) {
            if (books[i].name == name) {
                books[i].print_borrowers();
                not_found = false;
            }
        }
        if (not_found)
            cout << "Invalid book name.\n";
    }

    void print_books() {
        for (int i = 0; i < book_len; ++i) {
            books[i].print();
        }
    }

    static bool books_by_id(book &a, book &b) {
        return a.id < b.id;
    }

    static bool books_by_name(book &a, book &b) {
        return a.name < b.name;
    }

    void print_by_id() {
        sort(books, books + book_len, books_by_id);
        print_books();
    }

    void print_by_name() {
        sort(books, books + book_len, books_by_name);
        print_books();
    }

    void add_user(int id, string &name) {
        users[user_len++] = user(id, name);
    }

    int get_book_index(string &name) {
        for (int i = 0; i < book_len; ++i) {
            if (books[i].name == name)
                return i;
        }
        return -1;
    }

    int get_user_index(string &name) {
        for (int i = 0; i < user_len; ++i) {
            if (users[i].name == name)
                return i;
        }
        return -1;
    }

    void borrow_book(string &user_name, string &book_name) {
        int user_index = get_user_index(user_name);
        int book_index = get_book_index(book_name);
        if (book_index == -1 || user_index == -1) {
            cout << "no such book or user\n";
            return;
        }
        if (books[book_index].borrow(user_name))
            users[user_index].borrow_book(books[book_index].id);
        else {
            cout << "There is no free copy from this book";
        }

    }

    void return_book(string &user_name, string &book_name) {
        int user_index = get_user_index(user_name);
        int book_index = get_book_index(book_name);
        if (book_index == -1 || user_index == -1) {
            cout << "no such book or user\n";
            return;
        }
        books[book_index].remove_user(user_name);
        users[user_index].return_book(books[book_index].id);
    }

    void print_users() {
        for (int i = 0; i < user_len; ++i) {
            users[i].print();
        }
    }
};

struct library_system {
    Library library;

    void run() {
        while (true) {
            int choice = menu();
            if (choice == 1) {
                cout << "Enter book info : id & name & total quantity : ";
                int id, quantity;
                string name;
                cin >> id >> name >> quantity;
                library.add_book(id, name, quantity);
            } else if (choice == 2) {
                cout << "Enter The prefix : ";
                string prefix;
                cin >> prefix;
                library.find_by_prefix(prefix);
            } else if (choice == 3) {
                cout << "Enter book name : ";
                string name;
                cin >> name;
                library.who_borrowed(name);
            } else if (choice == 4) {
                library.print_by_id();
            } else if (choice == 5) {
                library.print_by_name();
            } else if (choice == 6) {
                cout << "Enter user info : name & id : ";
                int id;
                string name;
                cin >> name >> id;
                library.add_user(id, name);
            } else if (choice == 7) {
                cout << "Enter user name & book name :";
                string user_name, book_name;
                cin >> user_name >> book_name;
                library.borrow_book(user_name, book_name);
            } else if (choice == 8) {
                cout << "Enter user name & book name :";
                string user_name, book_name;
                cin >> user_name >> book_name;
                library.return_book(user_name, book_name);
            } else if (choice == 9) {
                library.print_users();
            } else
                break;
        }
    }

    static int menu() {
        int choice = -1;
        while (choice == -1) {
            cout << "\nEnter your choice\n";
            cout << "1) Add a book\n";
            cout << "2) search books by prefix\n";
            cout << "3) print who borrowed book by name\n";
            cout << "4) print library by id\n";
            cout << "5) print library by name\n";
            cout << "6) add user\n";
            cout << "7) user borrow book\n";
            cout << "8) user return book\n";
            cout << "9) print users\n";
            cout << "10) Exit\n";
            cin >> choice;
            if (choice < 1 || choice > 10) {
                cout << "Sorry ,try a number within [1-10]\n";
                choice = -1;
            }
        }
        return choice;
    }

};


int main() {
    library_system system1;
    system1.run();
    return 0;
}