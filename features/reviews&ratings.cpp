#ifndef REVIEWS_AND_RATINGS_CPP
#define REVIEWS_AND_RATINGS_CPP

#include <fstream>
#include <string>
using namespace std;

class Review
{
public:
    string username;
    string isbn;
    int rating;
    string comment;

    Review()
    {
        username = "";
        isbn = "";
        rating = 0;
        comment = "";
    }

    void save(ofstream& out)
    {
        out << username << "|" << isbn << "|" << rating << "|" << comment << endl;
    }
};

#endif

