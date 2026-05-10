#ifndef BOOK_MANAGEMENT_CPP
#define BOOK_MANAGEMENT_CPP

#include <fstream>
#include <string>
using namespace std;

class Book
{
    int id;
    string title;
    string author;
    string isbn;
    string category;
    int totalCopies;
    int availableCopies;
    int timesIssued;

public:
    Book()
    {
        id = 0;
        title = "";
        author = "";
        isbn = "";
        category = "";
        totalCopies = 0;
        availableCopies = 0;
        timesIssued = 0;
    }

    void setData(int i, string t, string a, string is, string c, int total, int avail, int issued)
    {
        id = i;
        title = t;
        author = a;
        isbn = is;
        category = c;
        totalCopies = total;
        availableCopies = avail;
        timesIssued = issued;
    }

    int getId()
    {
        return id;
    }

    string getTitle()
    {
        return title;
    }

    string getAuthor()
    {
        return author;
    }

    string getIsbn()
    {
        return isbn;
    }

    string getCategory()
    {
        return category;
    }

    int getAvailable()
    {
        return availableCopies;
    }

    int getTotal()
    {
        return totalCopies;
    }

    int getTimesIssued()
    {
        return timesIssued;
    }

    bool issue()
    {
        if (availableCopies <= 0)
            return false;

        availableCopies--;   //reduces available copies by 1
        timesIssued++;   //increments total issues for the person
        return true;
    }

    void returnCopy()
    {
        if (availableCopies < totalCopies)
            availableCopies++;
    }

    void save(ofstream& out)
    {
        out << id << "|" << title << "|" << author << "|" << isbn << "|"
            << category << "|" << totalCopies << "|" << availableCopies << "|"
            << timesIssued << endl;
    }
};

#endif
