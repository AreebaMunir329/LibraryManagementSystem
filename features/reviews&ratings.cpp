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

<<<<<<< HEAD
<<<<<<< HEAD
int main()
{
	review reviews[MAX_REVIEWS];
		int count = 0;
	
		loadreviews(reviews, count);
	
		string role;
		cout << "Login as (admin/member): ";
		cin >> role;
	
		while (role != "Admin" && role != "admin" && role != "member" && role != "Member")
		{
			cout << "Invalid entry. Try again: ";
			cin >> role;
		}
		bool isAdmin;
		if (role == "Admin" || role == "admin")
		{
			isAdmin = true;
		}
		else
		{
			isAdmin = false;
		}
	
		reviewmenu(reviews, count, isAdmin);
		return 0;
}
=======
=======
#endif
>>>>>>> c01835e2dfe92bacf207fc84bab7b76517eae209

>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
