#ifndef BOOK_CATEGORIES_CPP
#define BOOK_CATEGORIES_CPP

#include <string>
using namespace std;

class Category
{
public:
    string name;

    Category()
    {
        name = "";
    }

    Category(string n)
    {
        name = n;
    }
};

#endif
