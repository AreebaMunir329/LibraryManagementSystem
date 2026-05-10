#ifndef MEMBER_BORROWED_BOOK_CPP
#define MEMBER_BORROWED_BOOK_CPP

#include <fstream>
#include <string>
using namespace std;

class BorrowRecord
{
public:
    int id;
    string username;
    string isbn;
    string issueDate;
    string dueDate;
    string returnDate;
    int returned;

    BorrowRecord()
    {
        id = 0;
        username = "";
        isbn = "";
        issueDate = "";
        dueDate = "";
        returnDate = "";
        returned = 0;
    }

    void save(ofstream& out)
    {
        out << id << "|" << username << "|" << isbn << "|" << issueDate << "|"
            << dueDate << "|" << returnDate << "|" << returned << endl;
    }
};

#endif
