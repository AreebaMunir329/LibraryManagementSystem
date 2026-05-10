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

<<<<<<< HEAD
void loadfiledata()  //loading file data into books arr
{
	count = 0;
	ifstream in;
	in.open(file);
	string line;
	if (!in) return;
	while (getline(in, line) && count < MAXBOOKS)
	{
		if (line.empty()) continue;   //to identify if string is empty
		book booktoload;   //load details of book from file
		if (booktoload.read(line))
			books[count++] = booktoload;
	}
	in.close();
}
//manages all books
class bookmanager
{
	book books[MAXBOOKS];
	int count;
	string file;
	int nextid;
public:
	//ctr
	bookmanager(const string& filename = "books.txt") : count(0), file(filename), nextid(1)
	{
		loadfiledata();
	}

	//file handling
	
	void savefiledata() const    //saving data to file
	{
		ofstream out;
		out.open(file);
		for (int i = 0; i < count; i++)
		{
			books[i].dataentry(out);
		}
		out.close();
	}

	//functionality
	//add book
	void addbook()
	{
		string name, author, isbn, category;
		int cpy;
		if (count >= MAXBOOKS)  //edge case handling
		{
			cout << "Library is full" << endl;
			return;
		}
		cout << "ADD NEW BOOK" << endl;
		cin.ignore();

		cout << "Title: ";
		getline(cin, name);
		for (int i = 0; i < count; i++)
		{
			if (lowercase(books[i].getname()) == lowercase(name))
			{
				cout << "A book with this title already exists" << endl;
				return;
			}
		}

		cout << "Author: ";
		getline(cin, author);

		bool exists;
		do
		{
			exists = false;
			cout << "ISBN: ";
			getline(cin, isbn);
			for (int i = 0; i < count; i++)
			{
				if (books[i].getisbn() == isbn)
				{
					exists = true;
					break;
				}
			}
		} while (exists);  //input validation

		cout << "Cateory: ";
		getline(cin, category);

		do
		{
			cout << "Copies: ";
			cin >> cpy;
		} while (cpy <= 0);  //input validation
		books[count++] = book(nextid++, name, author, isbn, category, cpy);
		savefiledata();
		cout << "NEw book added with ID: " << (nextid - 1) << endl;
	}
	book& getBook(int index)//added by 0521.
	{
		return books[index];
	}
	//remove book 
	void removebook()
	{
		int id;
		cout << "REMOVE BOOK" << endl;
		cout << "Enter book id to remove: ";
		cin >> id;

		int index = getidindex(id);
		if (index == -1)
		{
			cout << "No book with id " << id << " found" << endl;
			return;
		}

		//moves beyond this point if index is valid
		int issuedbooks = books[index].gettotalcpy() - books[index].getavailablecpy();
		if (issuedbooks > 0)  //edge case handling
		{
			cout << "Book cannot be removed, copies still issued to members" << endl;
			return;
		}

		//moves beyond this point if no copy is issued
		char confirm;
		cout << "Removing: " << books[index].getname() << " by " << books[index].getauthor() << endl;
		do
		{
			cout << "Are you sure (Y/N): ";  //input validation     STILL NEEDS CHECK FOR STRING INPUT
			cin >> confirm;
		} while (confirm != 'y' && confirm != 'Y' && confirm != 'n' && confirm != 'N');

		if (confirm == 'y' || confirm == 'Y')
		{
			for (int i = 0; i < count; i++)  //shifts to left i.e. removes the book
			{
				books[i] = books[i + 1];
			}
			count--;
			savefiledata();
			cout << "Book removed successfully!" << endl;
		}
		else
		{
			cout << "Removal request cancelled" << endl;
		}
	}

	//search book
	void searchbook() const
	{
		int choice;
		do
		{
			cout << "Search by: " << endl;
			cout << "1.Title: " << endl;
			cout << "2.Author: " << endl;
			cout << "3.ISBN: " << endl;
			cin >> choice;
		} while (choice != 2 && choice != 1 && choice != 3);
		cin.ignore();

		string keyword;
		cout << "Enter keyword to find: ";
		getline(cin, keyword);
		string lwrkeyword = lowercase(keyword);//convert to lowercase for comparison

		int booksfound = 0; //number of books found
		for (int i = 0; i < count; i++)
		{
			bool match = false;
			if (choice == 1 && lowercase(books[i].getname()).find(lwrkeyword) != string::npos)
			{
				match = true;
			}
			else if (choice == 2 && lowercase(books[i].getauthor()).find(lwrkeyword) != string::npos)
			{
				match = true;
			}
			else if (choice == 3 && books[i].getisbn() == keyword)
			{
				match = true;
			}

			if (match)
			{
				books[i].display();
				booksfound++;
			}
		}
		if (booksfound == 0)
		{
			cout << "No books matched your search" << endl;
		}
		else
		{
			cout << booksfound << " book(s) found" << endl;
		}
	}

	//display all books
	void displayall() const
	{
		if (count == 0)
		{
			cout << "No books in library" << endl;
			return;
		}

		cout << "ALL BOOKS:" << endl;
		for (int i = 0; i < count; i++)
		{
			books[i].display();
		}
		cout << "Total books: " << count << endl;
	}

	//helper functions
	int getidindex(int id) const
	{
		for (int i = 0; i < count; i++)
		{
			if (books[i].getid() == id)
			{
				return i;
			}
		}
		return -1;   //index not found
	}

	string lowercase(const string& word) const
	{
		string res = word;
		for (int i = 0; i < (int)res.size(); i++)
		{
			res[i] = tolower(res[i]);  //convert to lowercase char by char
		}
		return res;
	}
	void menu(bool isadmin)  //implementation after authentication
	{
		do
		{
			int choice;
			cout << "-------------BOOKS MANAGEMENT-----------------" << endl;
			if (isadmin)  //only admin can access these
			{
				cout << "1. Add Book" << endl;
				cout << "2. Remove Book" << endl;
			}
			cout << "3.Search Book" << endl;
			cout << "4.View All Books" << endl;
			cout << "5.Back" << endl;
			do
			{
				cout << "Enter number of your desired function: ";
				cin >> choice;
			} while (choice < 1 || choice > 5);  //input validation
			if (choice == 1 && isadmin)
				addbook();
			else if (choice == 2 && isadmin)
				removebook();
			else if (choice == 3)
				searchbook();
			else if (choice == 4)
				displayall();
			else if (choice == 5)  //exits the loop
				break;
			else
				cout << "Invalid option" << endl;
		} while (true);
	}
};
<<<<<<< HEAD
<<<<<<< Updated upstream

=======
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83

	//helper functions
	int date_to_days(const string& date)
	{
		int day, month, year;
		day = stoi(date.substr(0, 2)); 
		month = stoi(date.substr(3, 2));
		year = stoi(date.substr(6, 4));
		return (year * 365 + month * 30 + day);//return days
	}
	string add_days_for_overdue(const string& date, int days)
	{
		int day, month, year;
		day = stoi(date.substr(0, 2));
		month = stoi(date.substr(3, 2));
		year = stoi(date.substr(6, 4));
		days += 14;//14 days above due date are allowed to return without fine

		int days_in_months[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
		while (days > days_in_months[month])
		{
			days -= days_in_months[month]; m++;
			if (m > 12)
			{
				m = 1; year++; //new year so update month and year
			}
		}
		string dd, mm, yy;
		dd = (days < 10 ? "0" : "") + to_string(days);
		dd = (month < 10 ? "0" : "") + to_string(month);
		yy = to_string(year);//back to dd/mm/yy
		return days + "/" + month + "/" + year;
	}
	int days_overdue(const string& due_date, const string& date_of_return)
	{
		int difference;
		difference = date_to_days(date_of_return) - date_to_days(due_date);
		if (difference > 0)
			return difference;//returned after due date
		else
			return 0;//returned on/before due date
	}
<<<<<<< HEAD
	class Transection
	{
		int transection_id, member_id, book_id;
		string borrow_date, due_date, return_date, status_of_book;
	public:
		Transection()
			: transection_id(0), member_id(0), book_id, borrow_date(""), 
			due_date(""), return_date(""), status_of_book(""){}
		Transection(int t_id, int m_id, int b_id, const string& b_date)
			: transection_id(t_id), member_id(m_id), book_id(b_id), borrow_date(b_date)
=======

	class Transaction
	{
		int transaction_id, member_id, book_id;
		string borrow_date, due_date, return_date, status_of_book;
	public:
		Transaction()
			: transection_id(0), member_id(0), book_id, borrow_date(""), 
			due_date(""), return_date(""), status_of_book(""){}
		Transaction(int t_id, int m_id, int b_id, const string& b_date)
			: transaction_id(t_id), member_id(m_id), book_id(b_id), borrow_date(b_date)
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
			, due_date(add_days_for_overdue(b_date, 14)), return_date(""), status_of_book("Borrowed") {}

		int getBookId() const{
			return book_id;
		}
<<<<<<< HEAD
		int getTransectionId()const{
			return transection_id;
=======
		int getTransactionId()const{
			return transaction_id;
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
		}
		int getMemberId()const{
			return member_id;
		}
		string getBorrowDate()const{
			return borrow_date;
		}
		string getDueDate()const{
			return due_date;
		}
		string getReturnDate()const{
			return return_date;
		}
		string getStatusOfBook()const{
			return status_of_book;
		}
		void setDueDate(const string& date) {
			return_date = date;
		}
		void setStatusOfBook(const string& status) {
			status_of_book = status;
		}
		void display() {
<<<<<<< HEAD
			cout << "Transection Id: " << transection_id << endl;
=======
			cout << "Transaction Id: " << transaction_id << endl;
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
			cout << "Member Id: " << member_id << endl;
			cout << "Book Id: " << book_id << endl;
			cout << "Date of borrow: " << borrow_date << endl;
			cout << "Due Date: " << due_date << endl;
			cout << "Date of return: " << return_date << endl;
			cout << "Status of book:" << status_of_book << endl;
		}
		//file handling
		void DataEntry(ofstream& out) const //constant bcz DO NOT want it to change anything.
		{
<<<<<<< HEAD
			out << Transection_id << "|" << member_id << "|" << book_id << "|" << borrow_date << "|"
=======
			out << Transaction_id << "|" << member_id << "|" << book_id << "|" << borrow_date << "|"
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
				<< due_date << "|" << return_date << "|" << status_of_book << "|";
		}
		bool read(const string& line){
			int index = 0; string data[7], input = "";
			for (int i = 0; i < (int)line.size; i++)
			{
				if (line[i] == '|')
				{
					if (index >= 7)
						return false;//edge case
					data[index++] = input; input = "";
				}
				else
					input += line[i];
			}
			data[index++] = input;

			if (index != 7) return false;

<<<<<<< HEAD
			transection_id = stoi(data[0]); member_id = stoi(data[1]);
=======
			transaction_id = stoi(data[0]); member_id = stoi(data[1]);
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
			book_id = stoi(data[2]); borrow_date = data[3];
			due_date = data[4]; return_date = data[5]; 
			status_of_book = data[6]; return true;
		}
	};

<<<<<<< HEAD
	const int MAXTRANSECTIONS = 500;
	class TransectionManager
	{
		Transection transections[MAXTRANSECTIONS];
		int count, next_id;	string file;
	public:
		TransectionManager(const string& filename = "transection.txt")
=======
	const int MAXTRANSACTIONS = 500;
	class TransactionManager
	{
		Transaction transactions[MAXTRANSACTIONS];
		int count, next_id;	string file;
	public:
		TransactionManager(const string& filename = "transaction.txt")
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
			:count(0), file(filename), next_id(1) {
			loadfiledata();
			for (int i = 0; < count; i++)
			{
<<<<<<< HEAD
				if(transections[i].getTransectionId()>=next_id)
					next_id=transections[i].getTransectionId
			}
		}

=======
				if (transactions[i].getTransactionId() >= next_id)
					next_id = transactions[i].getTransactionId() + 1;
			}
		}
		void loadFileData()  //loading file data into transactions array
		{
			count = 0;
			ifstream in;
			in.open(file);
			string line;
			if (!in) return;
			while (getline(in, line) && count < MAXBOOKS)
			{
				if (line.empty())
					continue;   //to identify if string is empty
				Transaction transaction_to_load;   //load details of transaction from file
				if (transaction_to_load.read(line))
					transactions[count++] = transaction_to_load;
			}
			in.close();
		}
		void saveFileData() const    //saving data to file
		{
			ofstream out;
			out.open(file);
			for (int i = 0; i < count; i++)
			{
				transactions[i].dataentry(out);
			}
			out.close();
		}
		bool isBookIssued(int b_id)const {
			for (int i = 0; i < count; i++) {
				if (transactions[i].getBookId() = b_id && transactions[i].getStatusOfBook() == "Borrowed")
					return true;
			}
			return false;
		}
		int getTIndex(int t_id) const {
			for (int i = 0; i < count; i++)
			{
				if (transactions[i].getTransactionId() == t_id)
					return i;
			}
			return -1;
		}
		int getOverDueDays(int t_id) {
			int index;
			index = getTransactionindex(t_id); if (index == -1) return -1;
			if (transactions[index].getStatusOfBook() != "returned") return 0;
			return days_overdue(transactions[index].getDuedate(), transactions[index].getReturnDate());
		}

		int issueBook(int bookId, int memberId, const string& date_today, bookmanager& book_manager)
		{
			if (isBookIssued(bookId)) {
				cout << "All copies of this book are Issued at the moment." << endl;
					return -1;
			}
			int index = book_manager(getidindex(bookId));
			if (index == -1) {
				cout << "Book not found." << endl; return -1;
			}
			if (!book_manager(getBook(index)).issuebook()) {
				cout << "No copies avalaible." << endl; return -1;
			}
			transactions[count++] = Transaction(nextId++,memberId, bookId, date_today);
			saveFileData(); book_manager.savefiledata();
			cout << "Book issued Successfully.\n Transaction Id: " << (next_id - 1) << endl;
			return next_id - 1;
		}
		void returnBook(int t_id, const string& date_today, bookmanager& b_m, FineManager& f_m)
		{
			int index, book_index;
			index = getTransactionindex(t_id);
			if (index == -1) {
				cout << "Transaction not found." << endl; return;
			}
			if (transactions[index].getStatusOfBook() == "returned") {
				cout << "This book has already been returned." << endl; return;
			}
			transactions[index].getReturnDate(date_today); transactions[index].setStatusOfBook("returned");
			book_index = b_m.getidindex(transactions[index].getBookId());
			if (book_index != -1) {
				b_m.getBook(book_index).returnbook();
				saveFileData(); b_m.savefiledata();
			}
			int overdue_days;
			overdue_days = days_overdue(transactions[index].getDueDate(), date_today);
			if (overdue_days > 0) {
				cout << "Book returned " << overdue_days << " days late." << endl;
				f_m.createFine(t_id, transactions[index].getMemberId, overdue_days);
			}
			else
				cout << "Book returned on time.\n No fine charged." << endl;
		}
		//display functions
		void viewAll()const {
			if (count == 0) {
				cout << "No transactions found." << endl; return;
			}
			cout << "ALL TRANSACTIONS>\n";
			for (int i = 0; i < count; i++)
				transactions[i].display();
			cout << "Total no.of Transactions: " << count << endl;
		}
		void viewByMember(int m_id) const {
			int m_found = 0;
			for (int i = 0; i < count; i++) {
				if (transactions[i].getMemberId() == m_id)
				{
					transactions[i].display(); m_found++;
				}
				if (m_found == 0)
					cout << "No transactions found for this member." << endl;
			}
		}
		void viewByBook(int b_id)const {
			int b_found = 0;
			for (int i = 0; i < count; i++) {
				if (transactions[i].getBookId == b_id)
				{
					transactions[i].display(); b_found++;
				}
				if (b_found == 0)
					cout << "No transactions found for this Book." << endl;
			}
		}
		void Menu(bool isAdmin, bookmanager& b_m) {
			int choice;
			do {
				cout << "-----TRANSACTIONS-----\n";
				cout << "1. Issue Book" << endl; cout << "2. Return Book." << endl;
				cout << "3. View All Transactions." << endl; cout << "4. View My Transactions." << endl;
				if (isAdmin) cout << "5. View Transactions by Book" << endl;
				cout << "6. Back" << endl;
				do {
					cout << "Enter Choice:"; cin >> choice; cout << endl;
				} while (choice < 1 || choice>6);
				switch (choice)
				{
				case 1: {
					int m_id, b_id; string date;
					cout << "Enter Member Id: "; cin >> m_id; cout << endl;
					cout << "Enter Book Id: "; cin >> b_id; cout << endl;
					cout << "Enter Today's Date (DD/MM/YYYY): "; cin >> date; cout << endl;
					issueBook(m_id, b_id, date, b_m); break;
				}
				case 2: {
					int t_id; string date;
					cout << "Enter Transaction Id: "; cin >> t_id; cout << endl;
					cout << "Enter Today's Date (DD/MM/YYYY): "; cin >> date; cout << endl;
					returnBook(t_id, date, b_m); break;
				}
				case 3: {
					viewAll(); break;
				}
				case 4: {
					int m_id;
					cout << "Enter Member Id: "; cin >> m_id; cout << endl;
					viewByMember(m_id); break;
				}
				case 5: {
					if (isAdmin)
					{
						int b_id; 
						cout << "Enter Book ID: "; cin >> b_id; cout << endl;
						viewByBook(b_id); break;
					}
					break;
				}
				default:
					break;
				}
			}
		}
>>>>>>> 4930aa557a0adfc9208fcdfcc09213943ec94a83
	};

=======
#endif
>>>>>>> c01835e2dfe92bacf207fc84bab7b76517eae209
