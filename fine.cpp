#include<Fine.h>
using namespace std;
const float FINE_RATE = 10.0f;
class Fine
{
	int fine_id, t_id, m_id, overdue_days;
	float fine_amount; string status;
public:
	Fine():
		fine_id(0), t_id(0), m_id(0), overdue_days(0),
		fine_amount(0.0f), status(""){}
	Fine(int f_id, int t_id, int m_id, int od_days):
		fine_id(f_id), t_id(t_id), m_id(m_id), overdue_days(od_days), 
		fine_amount(od_days* FINE_RATE), status("Unpaid") {
	}
	//getters
	int getFineId() const {
		return fine_id;
	}
	int getTransactionId()const {
		return t_id;
	}
	int getMemberId()const {
		return m_id;
	}
	int getOverDueDays() const {
		return overdue_days;
	}
	float getFineAmount() const {
		return fine_amount;
	}
	string getFineStatue()const {
		return ststus;
	}

	void setFineStatus(const string& new_status) {
		status = new_status;
	}
	void display()const {
		cout << "Fine Id: " << fine_id << endl;
		cout << "Transaction Id: " << t_id << endl;
		cout << "Member Id: " << m_id << endl;
		cout << "Date Overdue: " << overdue_days << endl;
		cout << "Fine Amount: " << fine_amount << endl;
		cout << "Status of fine:" << status << endl;
	}
	void DataEntry(ofstream& out)const {
		out << fine_id << "|" << t_id << "|" << m_id << "|" <<
		overdue_day << "|" << fine_amount << "|" << status << "|";
	}
	bool Read(const string& line) {
		int index = 0; string data[6], input = "";
		for (int i = 0; i < (int)line.size; i++)
		{
			if (line[i] == '|')
			{
				if (index >= 6)
					return false;//edge case
				data[index++] = input; input = "";
			}
			else
				input += line[i];
		}
		data[index++] = input;

		if (index != 6) return false;

		fine_id = stoi(data[0]); t_id = stoi(data[1]);
		m_id = stoi(data[2]); overdue_days = stoi(data[3]);
		fine_amount = stof(data[4]); status = data[5];
		return true;
	}
 };

class FineManager
{
	Fine fines[MAX_FINES]; int count, next_id; string file;
public:
	FineManager(const string& filename)
		:count(0), file(filename), next_id(1){
		loadfiledata();
		for (int i = 0; < count; i++)
		{
			if (fines[i].getFineId() >= next_id)
				next_id = fines[i].getFineId() + 1;
		}
	}

	void LoadFileData() {
		count = 0;
		ifstream in;
		in.open(file);
		string line;
		if (!in) return;
		while (getline(in, line) && count < MAX_FINES)
		{
			if (line.empty())
				continue;   //to identify if string is empty
			FIne fine_to_load;   //load details of fine from file
			if (fine_to_load.Read(line))
				fines[count++] =fine_to_load;
		}
		in.close();
	}
	void SaveFileData()const {
		ofstream out;
		out.open(file);
		for (int i = 0; i < count; i++)
		{
			fines[i].DataEntry(out);
		}
		out.close();
	}
	void createFine(int t_id, int m_id, int days_od) {
		if(count>=MAX_FINES)
		{
			cout << "Storage Full.\n"; return;
		}
		fines[count++] = Fine(next_id, t_id, m_id, days_od);
		SaveFileData();
		cout << "Overdue days: " << days_od << endl;
		cout << "Fine Created.\n Amount:\n Rs. " << (days_od * FINE_RATE);
	}
	void payFine(int f_id) {
		int index = getFineIndex(f_id);
		if (index == -1)
			return;
		if (fines[index].getFineStatue() == "paid") 
		{
			cout << "Your Fine is already Paid.\n";
			return;
		}
		fines[index].setFineStatus("paid"); SaveFileData();
		cout << "Fine of Rs. " << fines[index].getFineAmount() << " is marked as paid" << endl;
	}
	void viewAll()const {
		if (count == 0) {
			cout << "No Fines found." << endl; return;
		}
		cout << "ALL FINES>\n";
		for (int i = 0; i < count; i++)
			fines[i].display();
		cout << "Total no.of Fines: " << count << endl;
	}
	void viewByMember(int m_id) const {
		int m_found = 0;
		for (int i = 0; i < count; i++) {
			if (fines[i].getMemberId() == m_id)
			{
				fines[i].display(); m_found++;
			}
			if (m_found == 0)
				cout << "No fines found for this member." << endl;
		}
	}
	void viewUnpaid()const {
		int unpaid_found = 0;
		for (int i = 0; i < count; i++) {
			if (fines[i].getFineStatue() == "unpaid")
			{
				fines[i].display(); unpaid_found++;
			}
		}
		if (unpaid_found == 0)
			cout << "No Fines Found." << endl;
		return;
	}
	//helper
	int getFineIndex(int f_id) const {
		for (int i = 0; i < count; i++)
		{
			if (fines[i].getFineId() == f_id)
				return i;
		}
		return -1;
	}
	bool FinesPending(int m_id) const {
		for (int i = 0; i < count; i++)
		{
			if (fines[i].getMemberId() == m_id && fines[i].getFineStatue() == "unpaid")
				return true;
		}
		return false;
	}

	void Menu(bool isAdmin) {
		do {
			cout << "-----FINES-----\n";
			cout << "1. View All Fines." << endl << "2. View Unpaid Fines." << endl;
			cout << "3. View My Fines" << endl<< "4. Pay a Fine" << endl<< "5. Back" << endl;
			do {
				cout << "Enter Choice:"; cin >> choice; cout << endl;
			} while (choice < 1 || choice>5);
			switch (choice)
			{
			case 1: {
				if (isAdmin) {
					viewAll();
				} break;
			}
			case 2: {
				if (isAdmin) {
					viewUnpaid();
				} break;
			}
			case 3: {
				int m_id;
				cout << "Enter Member ID: "; cin >> m_id; cout << endl;
				viewByMember(m_id); break;
			}
			case 4: {
				int f_id;
				cout << "Enter Transaction ID: "; cin >> f_id; cout << endl;
				payFine(f_id); break;
			}
			default:
				break;
			}
		}
	}
};