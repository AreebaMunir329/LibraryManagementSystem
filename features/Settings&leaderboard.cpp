#include<iostream>
#include<fstream>
#include<string>
using namespace std;

const int MAX_MEMBERS = 100;
const double MAX_FINE_RATE = 100.0;
const int MAX_BOOKS_LIMIT = 20;


//helper functions
void clearBuffer()
{
	cin.clear();
	while (cin.get() != '\n' && !cin.eof());
}

bool validnum(string str)  //handles wrong inputs / negatives (return false)
{
	if (str.empty())   //can't be empty
		return false;
	bool dotexists = false;
	int start = 0;

	if (str[0] == '-')   //incase number is negative
		start = 1;
	if (start == (int)str.length())   //if there's no actual number after '-'
		return false;

	for (int i = 0; i < (int)str.length(); i++)
	{
		if (str[i] == '.')  //incase number is float
		{
			if (dotexists)
				return false;  //two dots are invalid
			dotexists = true;
		}
		else if (str[i] < '0' || str[i] > '9')
		{
			return false;  //letter or symbol
		}
	}
	return true;
}

class leaderboardentry
{
public:
	string username;
	int borrowedbooks;
	string fullname;
	leaderboardentry() : username(""), borrowedbooks(0), fullname(""){}
};

class settings
{
	double finerate;  //per day
	int maxbooks;  //per member
	string settingsfile;
public:
	//ctr
	settings() : finerate(5.0), maxbooks(3), settingsfile("settings.txt")
	{
		loadsettings();
	}

	//getters
	double getfinerate() { return finerate; }
	int getmaxbooks() { return maxbooks; }

	//load settings
	void loadsettings()
	{
		ifstream in;
		in.open(settingsfile);
		if (!in)  //unable to open file
			return;

		string line;
		while (getline(in, line))   //as long as it reads smth from file
		{
			if (line.empty())   //can't be empty
				continue;

			int index = line.find('=');
			if (index == -1)
				continue;

			string attrib = line.substr(0, index);  //stores attribute name
			string val = line.substr(index + 1);  //stores attribute

			if (val.empty())  //no vlalue has been entered
				continue;

			if (attrib == "finerate")
			{ 
				if (!validnum(val))  //entered value is not valid
					continue;

				double load = stod(val);
				if (load >= 0 && load <= MAX_FINE_RATE)
				{
					finerate = load;    //otherwise contains the default value
				}
			}
			else if (attrib == "maxbooks")
			{
				if (!validnum(val))
					continue;

				if (val.find('.') != string::npos)   //shouldn't be a decimal
					continue;

				int load = stoi(val);
				if (load > 0 && load <= MAX_BOOKS_LIMIT)
				{
					maxbooks = load;
				}
			}
		}
		in.close();
	}

	//save settings
	void savesettings()
	{
		ofstream out;
		out.open(settingsfile);

		if (!out)
		{
			cout << "Couldn't save settings" << endl;
			return;
		}

		out << "finerate=" << finerate << endl;
		out << "maxbooks=" << maxbooks << endl;
		out.close();
	}

	//update fine rate
	void updatefinerate()
	{
		cout << "Current fine rate: Rs. " << finerate << " per day" << endl;
		cout << "Enter new fine rate (0 to " << MAX_FINE_RATE << "): Rs. ";

		string newrate;
		cin >> newrate;
		cin.ignore();

		while (!validnum(newrate) || stod(newrate) < 0 || stod(newrate) > MAX_FINE_RATE)
		{
			cout << "Invalid entry. Try again: ";
			cin >> newrate;
		}

		finerate = stod(newrate);
		savesettings();

		cout << "Fine rate updated to Rs. " << finerate << " per day" << endl;
		cout << "---------------------------------------------" << endl;
	}

	//update max books
	void updatemaxbooks()
	{
		cout << "Current max books per member: " << maxbooks << endl;
		cout << "Enter new limit (1 to " << MAX_BOOKS_LIMIT << "): ";

		string newval;
		cin >> newval;
		cin.ignore();

		while (!validnum(newval) || newval.find('.') != string::npos || stoi(newval) <= 0 || stoi(newval) > MAX_BOOKS_LIMIT)
		{
			cout << "Invalid value. Try again: ";
			cin >> newval;
		}

		maxbooks = stoi(newval);
		savesettings();
		cout << "Max books limit updated to " << maxbooks << endl;
		cout << "-----------------------------------------" << endl;
	}

	//display settings
	void displaysettings()
	{
		cout << "LIBRARY SETTINGS" << endl;
		cout << "Fine Rate: Rs." << finerate << " per day" << endl;
		cout << "Max Books: " << maxbooks << " per day" << endl;
		cout << "-----------------------------------------" << endl;
	}

	//settings menu
	void settingsmenu()
	{
		string input;
		int choice;

		do
		{
			cout << "SETTINGS" << endl;
			cout << "1. View Settings" << endl;
			cout << "2. Update Fine Rate" << endl;
			cout << "3. Update Max Books Per Member" << endl;
			cout << "0. Back" << endl;

			cout << "Enter choice: ";
			cin >> input;
			cin.ignore();

			while (!validnum(input) || input.find('.') != string::npos) {
				cout << "Invalid choice! Enter a whole number." << endl;
				cin >> input;
			}

			choice = stoi(input);

			if (choice == 1)
				displaysettings();
			else if (choice == 2)
				updatefinerate();
			else if (choice == 3)
				updatemaxbooks();
			else if (choice != 0)
				cout << "Invalid choice";

		} while (choice != 0);
	}
};

//---------------------------------------------------------------------

class leaderboard
{
	leaderboardentry entries[MAX_MEMBERS];
	string memfile;
	int count;
public:
	leaderboard() : count(0), memfile("members.txt"){}

	void loadfromfile()
	{
		count = 0;
		ifstream in(memfile);
		if (!in.is_open())
		{
			cout << "Could not open members file" << endl;
			return;
		}

		string line;
		while (getline(in, line) && count < MAX_MEMBERS)
		{
			if (line.empty())
				continue;

			string attribs[5];  //username, fullname, email, booksborrowed, outstandingfine
			int index = 0;
			string temp = line;

			while (index < 5)
			{
				int position = temp.find('|');
				if (position == -1)
				{
					attribs[index++] = temp;
					break;
				}
				attribs[index++] = temp.substr(0, position);
				temp = temp.substr(position + 1);
			}
			if (index < 4) //need at least 4 attributes
				continue;

			if (attribs[0].empty())
				continue;

			if (!validnum(attribs[3]))  //borrowed books shouldn't be decimal
				continue;
			if (attribs[3].find('.') != string::npos)
				continue;

			int borrowed = stoi(attribs[3]);
			if (borrowed < 0)
				continue;

			entries[count].username = attribs[0];
			entries[count].fullname = attribs[1].empty() ? "Unknown"  : attribs[3];
			entries[count].borrowedbooks = borrowed; 
			count++;
		}
		in.close();
	}

	//sort (selection)
	void sort()
	{
		for (int i = 0; i < count - 1; i++) {

			int maxIdx = i;
			for (int j = i + 1; j < count; j++) {
				if (entries[j].borrowedbooks > entries[maxIdx].borrowedbooks)
					maxIdx = j;
			}

			if (maxIdx != i) {
				leaderboardentry temp = entries[i];
				entries[i] = entries[maxIdx];
				entries[maxIdx] = temp;
			}
		}

	}

	void displayleaderboard()
	{
		loadfromfile();

		if (count == 0)
		{
			cout << "No members found to display" << endl;
			return;
		}

		bool progress = false;
		for (int i = 0; i < count; i++)
		{
			if (entries[i].borrowedbooks > 0)
			{
				progress = true;
				break;
			}
		}
		if (!progress)  //if no book is borrowed, no need to display leaderboard
		{
			cout << "No borrowing activity recorded yet" << endl;
			return;
		}

		sort();

		cout << "MOST ACTIVE BORROWERS" << endl;
		cout << "Rank |          Username           | Books Borrowed" << endl;
		cout << "----------------------------------------------------" << endl;

		int display = count < 10 ? count : 10;     //displays top ten

		for (int i = 0; i < display; i++)
		{
			if (entries[i].borrowedbooks == 0)
				break;

			string username = entries[i].username;
			if ((int)username.length() > 18)
				username = username.substr(0, 15) + "...";   // to handle long usernames

			cout << i + 1 << " |";
			cout << username;

			int spacing = 18 - (int)username.length();
			for (int j = 0; i < spacing; j++)
				cout << " ";

			cout << " | " << entries[i].borrowedbooks << endl;
		}
		cout << "------------------------------------------------------" << endl;
	}
};

void menu()
{
	settings set;
	leaderboard lead;

	string input;
	int choice;
	do
	{
		cout << "SETTINGS & LEADERBOARD" << endl;
		cout << "1. Settings" << endl;
		cout << "2. Active Borrower Leaderboard" << endl;
		cout << "0. Back" << endl;
		cout << "Choice: ";

		cin >> input;
		cin.ignore();

		if (!validnum(input) || input.find('.') != string::npos) {
			cout << "Invalid choice.Enter a whole number" << endl;
			choice = -1;
			continue;
		}

		choice = stoi(input);

		if (choice == 1)
			set.settingsmenu();
		else if (choice == 2)
			lead.displayleaderboard();
		else if (choice != 0)
			cout << "Invalid choice" << endl;
	} while (choice != 0);
}

