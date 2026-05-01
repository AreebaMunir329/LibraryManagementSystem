#include<iostream>
#include"features/bookmanagement.cpp"
#include "features/reviews&ratings.cpp"

int main()
{
	//bookmanager 
	bookmanager obj;
	obj.menu(true);

	//reviews & ratings
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

	reviewmenu(reviews, count, isAdmin);
	return 0;
}