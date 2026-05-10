#ifndef BOOK_RESERVATION_SYSTEM_CPP
#define BOOK_RESERVATION_SYSTEM_CPP

#include <fstream>
#include <string>
using namespace std;

class Reservation
{
public:
    string username;
    string isbn;
    string date;
    int active;

    Reservation()
    {
        username = "";
        isbn = "";
        date = "";
        active = 1;
    }

    void save(ofstream& out)
    {
        out << username << "|" << isbn << "|" << date << "|" << active << endl;
    }
};

#endif
