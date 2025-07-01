#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//Book class to store book details
class Book {
public:
    string title;
    string author;
    string genre;
    bool isAvailable;

    Book(string t, string a, string g) : title(t), author(a), genre(g), isAvailable(true) {}
};

//Member class to handle users and their borrowed books
class Member {
public:
    string username;
    vector<Book*> borrowedBooks;

    Member(string uname) : username(uname) {}

    // Borrow a book if under limit and available
    bool borrowBook(Book* book) {
        if (borrowedBooks.size() < 5 && book->isAvailable) {
            borrowedBooks.push_back(book);
            book->isAvailable = false;
            return true;
        }
        return false;
    }

    // Return a previously borrowed book
    void returnBook(Book* book) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), book);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            book->isAvailable = true;
        }
    }
};

// Global variables
vector<Book> books;
vector<Member> members;
Member* loggedInMember = nullptr;
const string adminUsername = "mai";
const string adminPassword = "mai";

// Function declarations
void loginAsAdmin();
void loginAsUser();
void adminMenu();
void userMenu();
void addBook();
void updateBook();
void removeBook();
void addMember();
void borrowBook();
void returnBook();
void displayAllBooks();
void displayAllMembers();
Book* findBookByTitle(string title);
Member* findOrCreateMember(string username);

int main() {
    // Preloaded books
    books.push_back(Book("Verity", "Colleen Hoover", "Mystery"));
    books.push_back(Book("Atomic Habits", "James Clear", "Life lesson"));
    books.push_back(Book("A Thousand Splendid Suns", "Khaled Hosseini", "Novel"));
    books.push_back(Book("The Alchemist", "Paulo Coelho", "Fiction"));
    books.push_back(Book("To Kill a Mockingbird", "Harper Lee", "Classic"));
    books.push_back(Book("The Power of Your Subconscious Mind", "Joseph Murphy", "Self-help"));
    books.push_back(Book("The Silent Patient", "Alex Michaelides", "Psychological Thriller"));
    books.push_back(Book("Rich Dad Poor Dad", "Robert T. Kiyosaki", "Finance"));
    books.push_back(Book("Ikigai", "Héctor García", "Philosophy"));
    books.push_back(Book("The Subtle Art of Not Giving a F*ck", "Mark Manson", "Self-help"));

    while (true) {
        cout << "\nEnter role (admin/user): ";
        string role;
        getline(cin, role);

        if (role == "admin") {
            loginAsAdmin();
        } else if (role == "user") {
            loginAsUser();
        } else {
            cout << "? Invalid role. Please try again." << endl;
        }
    }

    return 0;
}

//Admin login and menu access
void loginAsAdmin() {
    string username, password;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    if (username == adminUsername && password == adminPassword) {
        cout << "? Logged in as admin.\n";
        adminMenu();
    } else {
        cout << "? Invalid admin credentials.\n";
    }
}

//User login and menu access
void loginAsUser() {
    string username;
    cout << "Enter username: ";
    getline(cin, username);
    loggedInMember = findOrCreateMember(username);
    cout << "? Logged in as user.\n";
    userMenu();
}

// Admin menu
void adminMenu() {
    while (true) {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Book\n2. Update Book\n3. Remove Book\n4. Add Member\n5. Display All Books\n6. Display All Members\n7. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addBook(); break;
            case 2: updateBook(); break;
            case 3: removeBook(); break;
            case 4: addMember(); break;
            case 5: displayAllBooks(); break;
            case 6: displayAllMembers(); break;
            case 7: return;
            default: cout << "? Invalid choice. Try again.\n";
        }
    }
}

//User menu
void userMenu() {
    while (true) {
        cout << "\n--- User Menu ---\n";
        cout << "1. Borrow Book\n2. Return Book\n3. Display All Books\n4. Display All Members\n5. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: borrowBook(); break;
            case 2: returnBook(); break;
            case 3: displayAllBooks(); break;
            case 4: displayAllMembers(); break;
            case 5: return;
            default: cout << "? Invalid choice. Try again.\n";
        }
    }
}

//Add a new book
void addBook() {
    string title, author, genre;
    cout << "Enter book title: ";
    getline(cin, title);
    cout << "Enter book author: ";
    getline(cin, author);
    cout << "Enter book genre: ";
    getline(cin, genre);

    books.push_back(Book(title, author, genre));
    cout << "? Book added successfully.\n";
}

// Update existing book details
void updateBook() {
    string title;
    cout << "Enter book title to update: ";
    getline(cin, title);

    Book* book = findBookByTitle(title);
    if (book) {
        cout << "Enter new author: ";
        getline(cin, book->author);
        cout << "Enter new genre: ";
        getline(cin, book->genre);
        cout << "? Book updated successfully.\n";
    } else {
        cout << "? Book not found.\n";
    }
}

// Remove a book
void removeBook() {
    string title;
    cout << "Enter book title to remove: ";
    getline(cin, title);

    auto it = remove_if(books.begin(), books.end(), [&](Book& b) { return b.title == title; });
    if (it != books.end()) {
        books.erase(it, books.end());
        cout << "? Book removed successfully.\n";
    } else {
        cout << "? Book not found.\n";
    }
}

// Add a new member
void addMember() {
    string username;
    cout << "Enter member username: ";
    getline(cin, username);
    members.push_back(Member(username));
    cout << "? Member added successfully.\n";
}

//  Borrow a book
void borrowBook() {
    string title;
    cout << "Enter book title to borrow: ";
    getline(cin, title);

    Book* book = findBookByTitle(title);
    if (book && loggedInMember->borrowBook(book)) {
        cout << "? Book borrowed successfully.\n";
    } else {
        cout << "? Book is either unavailable or borrow limit reached.\n";
    }
}

// Return a book
void returnBook() {
    string title;
    cout << "Enter book title to return: ";
    getline(cin, title);

    Book* book = findBookByTitle(title);
    if (book && !book->isAvailable) {
        loggedInMember->returnBook(book);
        cout << "? Book returned successfully.\n";
    } else {
        cout << "? Book not found or it wasn't borrowed.\n";
    }
}

// Show all books
void displayAllBooks() {
    if (books.empty()) {
        cout << "?? No books available.\n";
    } else {
        cout << "\n?? List of Books:\n";
        for (auto& book : books) {
            cout << "Title: " << book.title << ", Author: " << book.author
                 << ", Genre: " << book.genre << ", Available: " << (book.isAvailable ? "Yes" : "No") << "\n";
        }
    }
}

// Show all members
void displayAllMembers() {
    if (members.empty()) {
        cout << "?? No members exist.\n";
    } else {
        cout << "\n?? List of Members:\n";
        for (auto& member : members) {
            cout << "Username: " << member.username << ", Borrowed Books: " << member.borrowedBooks.size() << "\n";
        }
    }
}

// Find a book by its title
Book* findBookByTitle(string title) {
    for (auto& book : books) {
        if (book.title == title) {
            return &book;
        }
    }
    return nullptr;
}

// Find a member or create a new one
Member* findOrCreateMember(string username) {
    for (auto& member : members) {
        if (member.username == username) {
            return &member;
        }
    }
    members.push_back(Member(username));
    return &members.back();
}

