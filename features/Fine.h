#pragma once
#ifndef FINE_H
#define FINE_H
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class Fine
{
	int fine_id, t_id, m_id, overdue_days;
	float fine_amount; string status;
public:
	Fine();
	Fine(int f_id, int t_id, int m_id, int days);
	//getters
	int getFineId() const;
	int getTransactionId()const;
	int getMemberId()const;
	int getOverDueDays() const;
	float getFineAmount() const;
	string getFineStatue()const;

	setFineStatus(const string& status);

	void display()const;
	void DataEntry(ofstream& out)const;
	bool Read(const string& line);
};
const int MAX_FINES = 500;

class FineManager
{
	Fine fines[MAX_FINES]; int count, next_id; string file;
public:
	FineManager(const string& filename = "Fines.txt");

	void LoadFileData(); void SaveFileData()const;
	void createFine(int t_id, int m_id, int days_od);
	void payFine(int f_id);

	void viewAll()const; void viewByMember(int m_id) const;
	void viewUnpaid()const;

	int getFineIndex(int f_id) const;
	bool FinesPending(int m_id) const;

	void Menu(bool isAdmin);
};
#endif // !FINE_H
