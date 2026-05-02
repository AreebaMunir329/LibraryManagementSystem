//#include <iostream>
//#include <string>
//#include <fstream>
//using namespace std;
//
////  BOOK CLASS 
//class Book {
//public:
//    string bookId;
//    string title;
//    bool isIssued;
//
//    Book() {
//        isIssued = false;
//    }
//
//    Book(string id, string t) {
//        bookId = id;
//        title = t;
//        isIssued = false;
//    }
//};
//
//// BORROW RECORD 
//class BorrowRecord {
//public:
//    string bookId;
//    string title;
//    string issueDate;
//    string dueDate;
//    bool returned;
//
//    BorrowRecord() {
//        returned = false;
//    }
//};
//
////  MEMBER CLASS 
//class Member {
//public:
//    string username;
//    string fullname;
//    string email;
//
//    int booksborrowed;
//    double outstandingfine;
//
//    BorrowRecord history[20]; //max 20 books can be borrowed by one member
//    int hcount;// no. of books borrowed by a person
//
//    Member() {
//        booksborrowed = 0;
//        outstandingfine = 0.0;
//        hcount = 0;
//    }
//
//    Member(string u, string n, string e) {
//        username = u;
//        fullname = n;
//        email = e;
//        booksborrowed = 0;
//        outstandingfine = 0.0;
//        hcount = 0;
//    }
//};
//
//// MEMBER MANAGER 
//class MemberManager {
//private:
//    Member memberslist[100]; //lists of member max 100
//    int count; // total no. of members 
//    string filepath;
//
//    Book books[200]; //list of books in library max 200
//    int bookCount; //no. of books in library
//    string bookFile; 
//
//public:
//    MemberManager() {
//        count = 0;
//        bookCount = 0;
//        filepath = "members.txt";
//        bookFile = "books.txt";  
//
//        loadBooks();
//        load();
//    }
//
//    //  BOOK FUNCTIONS 
//    Book* findBook(string id) {
//        for (int i = 0; i < bookCount; i++)
//            if (books[i].bookId == id)
//                return &books[i];
//        return nullptr;
//    }
//
//    void saveBooks() { 
//        ofstream file(bookFile);
//        if (!file.is_open()) return;
//
//        for (int i = 0; i < bookCount; i++) {
//            file << books[i].bookId << "|"
//                << books[i].title << "|"
//                << books[i].isIssued << "\n";
//        }
//        file.close();
//    }
//
//    void loadBooks() {
//        ifstream file(bookFile);
//        if (!file.is_open()) return;
//
//        string line;
//        while (getline(file, line)) {
//            if (line == "") continue;
//
//            string parts[3];
//            int index = 0;
//
//            while (index < 3) {
//                int pos = line.find('|');
//                if (pos == -1) {
//                    parts[index++] = line;
//                    break;
//                }
//                parts[index++] = line.substr(0, pos);
//                line = line.substr(pos + 1);
//            }
//
//            if (index < 3) continue; 
//
//            books[bookCount].bookId = parts[0];
//            books[bookCount].title = parts[1];
//            books[bookCount].isIssued = stoi(parts[2]);
//
//            bookCount++;
//        }
//        file.close();
//    }
//
//    //  MEMBER CHECK 
//    bool exists(string u) {
//        for (int i = 0; i < count; i++)
//            if (memberslist[i].username == u)
//                return true;
//        return false;
//    }
//    void addfine(string u, double amount) {
//        Member* m = find(u);
//        if (m) {
//            m->outstandingfine += amount;
//            save();
//        }
//    }
//
//    void clearfine(string u) {
//        Member* m = find(u);
//        if (m) {
//            m->outstandingfine = 0;
//            save();
//        }
//    }
//
//    bool registerMember(string u, string f, string e) {
//        if (exists(u) || count >= 100)
//            return false;
//
//        memberslist[count++] = Member(u, f, e);
//        save();
//        return true;
//    }
//
//    Member* find(string u) {
//        for (int i = 0; i < count; i++)
//            if (memberslist[i].username == u)
//                return &memberslist[i];
//        return nullptr;
//    }
//
//    //  BORROW BOOK
//    void borrowBook(string u, string bookId, string issue, string due) {
//        Member* m = find(u);
//        Book* b = findBook(bookId);
//
//        if (!m || !b || b->isIssued) {
//            cout << "Cannot borrow book!\n";
//            return;
//        }
//
//        b->isIssued = true;
//        m->booksborrowed++;
//
//        int i = m->hcount++;
//        m->history[i].bookId = bookId;
//        m->history[i].title = b->title;
//        m->history[i].issueDate = issue;
//        m->history[i].dueDate = due;
//        m->history[i].returned = false;
//
//        saveBooks(); 
//        save();
//
//        cout << "Book issued successfully!\n";
//    }
//
//    //  RETURN BOOK 
//    void returnBook(string u, string bookId) {
//        Member* m = find(u);
//        Book* b = findBook(bookId);
//
//        if (!m || !b) return;
//
//        b->isIssued = false;
//        if (m->booksborrowed > 0)
//            m->booksborrowed--;
//
//        for (int i = 0; i < m->hcount; i++) {
//            if (m->history[i].bookId == bookId && !m->history[i].returned) {
//                m->history[i].returned = true;
//                break;
//            }
//        }
//
//        saveBooks();  
//        save();
//
//        cout << "Book returned!\n";
//    }
//
//    // BOOK DASHBOARD 
//    void dashboard(string u) {
//        Member* m = find(u);
//        if (!m) {
//            cout << "Member not found!\n";
//            return;
//        }
//
//        cout << "\n===== MEMBER DASHBOARD =====\n";
//        cout << "Name: " << m->fullname << endl;
//        cout << "Email: " << m->email << endl;
//        cout << "Books Borrowed: " << m->booksborrowed << endl;
//        cout << "Fine: " << m->outstandingfine << endl;
//
//        cout << "\n--- Current & Past Borrowings ---\n";
//        for (int i = 0; i < m->hcount; i++) {
//            cout << "Book: " << m->history[i].title << endl;
//            cout << "Issued: " << m->history[i].issueDate << endl;
//            cout << "Due: " << m->history[i].dueDate << endl;
//            cout << "Status: "
//                << (m->history[i].returned ? "Returned" : "Issued") << endl;
//            cout << "---------------------\n";
//        }
//    }
//
//    //  SHOW ALL MEMBERS RECORD
//    void showAll() {
//        cout << "\n----- ALL MEMBERS -----\n";
//        for (int i = 0; i < count; i++) {
//            cout << "Username: " << memberslist[i].username << endl;
//            cout << "Name: " << memberslist[i].fullname << endl;
//            cout << "Email: " << memberslist[i].email << endl;
//            cout << "Books Borrowed: " << memberslist[i].booksborrowed << endl;
//            cout << "Fine: " << memberslist[i].outstandingfine << endl;
//            cout << "------------------\n";
//        }
//    }
//
//    // SAVE MEMBERS 
//    void save() {
//        ofstream file(filepath);
//        for (int i = 0; i < count; i++) {
//            file << memberslist[i].username << "|"
//                << memberslist[i].fullname << "|"
//                << memberslist[i].email << "|"
//                << memberslist[i].booksborrowed << "|"
//                << memberslist[i].outstandingfine << "\n";
//        }
//        file.close();
//    }
//
//    //  LOAD MEMBERS FILE
//    void load() {
//        ifstream file(filepath);
//        string line;
//
//        while (getline(file, line)) {
//            if (line == "") continue;
//
//            string parts[5];
//            int index = 0;
//
//            while (index < 5) {
//                int pos = line.find('|');
//                if (pos == -1) {
//                    parts[index++] = line;
//                    break;
//                }
//                parts[index++] = line.substr(0, pos);
//                line = line.substr(pos + 1);
//            }
//
//            if (index < 5) continue;
//
//            memberslist[count] = Member(parts[0], parts[1], parts[2]);
//            memberslist[count].booksborrowed = stoi(parts[3]);
//            memberslist[count].outstandingfine = stod(parts[4]);
//            count++;
//        }
//        file.close();
//    }
//};
//
//int main() {
//    MemberManager manager;
//
//    int choice;
//    double amount;
//    string u, f, e, bookId, date, due;
//
//    while (true) {
//        cout << "\n==== LIBRARY SYSTEM ====\n";
//        cout << "1. Register Member\n";
//        cout << "2. Borrow Book\n";
//        cout << "3. Return Book\n";
//        cout << "4. View All Members\n";
//        cout << "5. Member Dashboard\n";
//        cout << "6. Add fine\n";
//        cout << "7. clear fine\n";
//        cout << "0. Exit\n";
//        cout << "Choice: ";
//        cin >> choice;
//
//        if (choice == 0) break;
//
//        switch (choice) {
//        case 1:
//            cout << "Username: ";
//            cin >> u;
//            cout << "Full Name: ";
//            cin.ignore();
//            getline(cin, f);
//            cout << "Email: ";
//            getline(cin, e);
//
//            if (manager.registerMember(u, f, e))
//                cout << "Registered!\n";
//            else
//                cout << "Failed!\n";
//            break;
//
//        case 2:
//            cout << "Username: ";
//            cin >> u;
//            cout << "Book ID: ";
//            cin >> bookId;
//            cout << "Issue Date: ";
//            cin >> date;
//            cout << "Due Date: ";
//            cin >> due;
//
//            manager.borrowBook(u, bookId, date, due);
//            break;
//
//        case 3:
//            cout << "Username: ";
//            cin >> u;
//            cout << "Book ID: ";
//            cin >> bookId;
//
//            manager.returnBook(u, bookId);
//            break;
//
//        case 4:
//            manager.showAll();
//            break;
//
//        case 5:
//            cout << "Enter Username: ";
//            cin >> u;
//            manager.dashboard(u);
//            break;
//        case 6:
//            cout << "Username: ";
//            cin >> u;
//            cout << "Fine: ";
//            cin >> amount;
//            manager.addfine(u, amount);
//            break;
//
//        case 7:
//            cout << "Username: ";
//            cin >> u;
//            manager.clearfine(u);
//            break;
//        }
//    }
//
//    return 0;
//}