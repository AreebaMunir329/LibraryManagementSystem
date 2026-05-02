#include<iostream>
#include<fstream>
#include<string>
using namespace std;

const int MAX_REVIEWS = 100;

//helper functions for input validation

bool hasDigit(string str)
{
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (isdigit(str[i]))
			return true;
	}
	return false;
}
bool alphaonly(string str)
{
	for (int i = 0; i < (int)str.length(); i++) {
		if (!isalpha(str[i]) && str[i] != ' ') return false;
	}
	return true;
}

bool validint(string str)
{
	if (str.empty()) return false;
	for (int i = 0; i < (int)str.length(); i++)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

//placeholder for ruman's transaction part
bool hasreturnedbook(string memid, string isbn)
{
	return true;
}

class review
{
	string memid;
	string isbn;
	string bookname;
	int rating;
	string cmnt;

public:
	//ctr
	review()
	{
		memid = "";
		isbn = "";
		cmnt = "";
		bookname = "";
		rating = 0;
	}

	//setters
	void setvals(string memid, string isbn, string name, string cmnt, int stars)
	{
		this->memid = memid;
		this->isbn = isbn;
		this->cmnt = cmnt;
		this->bookname = name;
		rating = stars;
	}

	//getters
	string getmemid() { return memid; }
	string getisbn() { return isbn; }
	string getname() { return bookname; }
	string getcmnt() { return cmnt; }
	int getrating() { return rating; }

	//display info
	void display()
	{
		cout << "Member ID: " << memid << endl;
		cout << "Book Title: " << bookname << endl;
		cout << "Rating: ";
		for (int i = 0; i < rating; i++)  // prints stars for reviews
		{
			cout << "* ";
		}
		for (int i = rating; i < 5; i++) //prints dashes for reviews < 5
		{
			cout << "- ";
		}
		cout << " ( " << rating << "/ 5 )" << endl;  //prints rating numerically
		cout << "Review: " << cmnt << endl << endl;
	}
};
//calculate average rating
float avgrating(review revs[], int count, string isbn, string name)
{
	int found = 0;
	float total = 0;

	for (int i = 0; i < count; i++) {
		if (revs[i].getisbn() == isbn && revs[i].getname() == name) {
			total += revs[i].getrating();
			found++;
		}
	}

	if (found == 0) return 0.0f;
	return total / found;
}

//file handling
void loadreviews(review revs[], int& count)
{
	count = 0;
	ifstream in;
	in.open("reviews.txt");

	string memid, isbn, cmnt, bookname;
	int rating;
	while (count < MAX_REVIEWS && getline(in, memid))  //getline to check if we still have smth to read
	{
		getline(in, isbn);
		getline(in, bookname);
		getline(in, cmnt);
		in >> rating;
		in.ignore();
		revs[count].setvals(memid, isbn, bookname, cmnt, rating);
		count++;
	}
	in.close();
}

void savereviews(review revs[], int count)  //writing in file
{
	ofstream out;
	out.open("reviews.txt");
	if (!out)
	{
		cout << "Couldn't open reviews.txt for saving" << endl;
		return;
	}
	for (int i = 0; i < count; i++)
	{
		out << revs[i].getmemid() << endl;
		out << revs[i].getname() << endl;
		out << revs[i].getisbn() << endl;
		out << revs[i].getcmnt() << endl;
		out << revs[i].getrating() << endl;
	}
	cout << "Changes have been made successfully" << endl << endl;
	out.close();
	cout << "--------------------------------" << endl;
}

void addreviews(review revs[], int& count)
{
	if (count >= MAX_REVIEWS)  //checks for space for new reviews
	{
		cout << "Review storage is full" << endl;
		return;
	}

	string memid, isbn, cmnt, bookname;
	int rating;

	cin.ignore();
	cout << "LEAVE A REVIEW" << endl;

	//memid
	cout << "Your Member ID: ";
	getline(cin, memid);
	while (memid.empty() || memid.find_first_not_of(' ') == string::npos)
	{
		cout << "Member ID cannot be empty. Try again: ";
		getline(cin, memid);
	}

	//book title
	cout << "Book Title: ";
	getline(cin, bookname);
	while (bookname.empty() || bookname.find_first_not_of(' ') == string::npos)
	{
		cout << "Member ID cannot be empty. Try again: ";
		getline(cin, bookname);
	}

	//isbn
	cout << "Book ISBN: ";
	getline(cin, isbn);
	//if isbn is empty or contains alphabets only
	while (isbn.empty() || alphaonly(isbn)) {
		cout << "Invalid ISBN. Must contain numbers. Try again: ";
		getline(cin, isbn);
	}

	//check if book has been returned
	if (!hasreturnedbook)
	{
		cout << "You can leave a review only if you have returned the book" << endl;
		return;
	}

	//duplicate check
	for (int i = 0; i < count; i++)
	{
		if (revs[i].getmemid() == memid && revs[i].getisbn() == isbn && revs[i].getname() == bookname)
		{
			cout << "You have entered reviewed this book" << endl;
			return;
		}
	}

	cout << "Rating (1-5): ";
	cin >> rating;
	while (cin.fail() || rating < 1 || rating  > 5)
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid rating. Enter a number between 1 and 5: ";
		cin >> rating;
	}
	cin.ignore();

	while (rating < 1 || rating > 5)  //input validation
	{
		cout << "Invalid rating" << endl;
		cout << "Enter valid rating: ";
		cin >> rating;
	}
	/*cin.ignore();*/
	cout << "Your Review: ";
	getline(cin, cmnt);
	while (cmnt.empty() || cmnt.find_first_not_of(' ') == string::npos)
	{
		cout << "Review cannot be empty, Try again: ";
		getline(cin, cmnt);
	}

	revs[count].setvals(memid, isbn, bookname, cmnt, rating);
	count++;
	savereviews(revs, count);
	cout << "-----------------------------------------" << endl;
}

//view reviews for a book
void viewreviews(review revs[], int count)
{
	string isbn, bookname;
	cin.ignore();
	cout << "VIEW REVIEWS" << endl;

	cout << "Enter Book title:";
	getline(cin, bookname);
	while (bookname.empty() || bookname.find_first_not_of(' ') == string::npos) {
		cout << "Book title cannot be empty. Try again: ";
		getline(cin, bookname);
	}

	cout << "Enter Book ISBN: ";
	getline(cin, isbn);
	while (isbn.empty() || alphaonly(isbn)) {
		cout << "Invalid ISBN. Try again: ";
		getline(cin, isbn);
	}

	int found = 0;  //to keep track of reviews

	for (int i = 0; i < count; i++)
	{
		if (revs[i].getisbn() == isbn && revs[i].getname() == bookname)
		{
			revs[i].display();
			found++;
		}
	}

	if (found == 0)
	{
		cout << "No reviews found for this book" << endl;
		return;
	}
	cout << "Average Rating: " << avgrating(revs, count, isbn, bookname) << "/5 " << endl;
	cout << "-------------------------------------" << endl << endl;
}

void deletereview(review revs[], int& count)
{
	string memid, isbn, bookname;
	cin.ignore();

	cout << "DELETE REVIEW " << endl;

	cout << "Member ID of the review to delete: ";
	getline(cin, memid);
	while (memid.empty() || memid.find_first_not_of(' ') == string::npos) {
		cout << "Member ID cannot be empty. Try again: ";
		getline(cin, memid);
	}

	cout << "Book Title: ";
	getline(cin, bookname);
	while (bookname.empty() || bookname.find_first_not_of(' ') == string::npos) {
		cout << "Book title cannot be empty. Try again: ";
		getline(cin, bookname);
	}

	cout << "Book ISBN: ";
	getline(cin, isbn);
	while (isbn.empty() || alphaonly(isbn)) {
		cout << "Invalid ISBN. Must contain numbers. Try again: ";
		getline(cin, isbn);
	}

	for (int i = 0; i < count; i++) {
		if (revs[i].getmemid() == memid && revs[i].getisbn() == isbn && revs[i].getname() == bookname) {
			for (int j = i; j < count - 1; j++) {
				revs[j] = revs[j + 1];
			}
			count--;
			savereviews(revs, count);
			cout << "Review deleted successfully.\n";
			return;
		}
	}
	cout << "---------------------------------" << endl;
}

//menu
void reviewmenu(review revs[], int& count, bool isAdmin)
{
	int choice;
	do
	{
		cout << "BOOK REVIEW & RATINGS" << endl;
		cout << "1. Leave a Review" << endl;
		cout << "2. View Reviews for a book" << endl;
		if (isAdmin)
		{
			cout << "3. Delete a Review" << endl;
			cout << "4. Back" << endl;
		}
		else
		{
			cout << "3. Back" << endl;
		}
		cout << "Enter your choice: ";
		cin >> choice;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. Enter a number." << endl;
			continue;
		}

		if (choice == 1)
			addreviews(revs, count);
		else if (choice == 2)
			viewreviews(revs, count);
		else if (choice == 3 && isAdmin)
			deletereview(revs, count);
	} while ((isAdmin && choice != 4) || (!isAdmin && choice != 3));
	cout << "---------------------------------------" << endl;
}


