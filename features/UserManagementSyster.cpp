#ifndef USER_MANAGEMENT_CPP
#define USER_MANAGEMENT_CPP
//assigns role to the person 
#include <fstream>
#include <string>
using namespace std;
class Person
{
protected:
    string username;
    string password;
    string fullName;
public:
    Person()
    {
        username = "";
        password = "";
        fullName = "";
    }
    //overriden in chlid class to get the role  whether a person is admin or a member 
    virtual string getRole()
    {
        return "Person";
    }
    string getUsername()
    {
        return username;
    }
    string getPassword()
    {
        return password;
    }
    string getFullName()
    {
        return fullName;
    }
};
class LibraryCard
{
    string cardNumber;  // represents library card
    string status;   // stores whether the card is active or not 
public:
    LibraryCard()
    {
        cardNumber = "";
        status = "Active";
    }
    void makeCard(string username)
    {
        cardNumber = "CARD-" + username;  // default operator overloaing (concatenation) in string library
        status = "Active";
    }
    string getCardNumber()
    {
        return cardNumber;
    }
    string getStatus()
    {
        return status;
    }
};
class Member : public Person   // inheritance
{
    string email;
    string phone;
    double fine;
    int totalIssued;
    LibraryCard card;        
public:
    Member()    // card initialization not required as it has default ctr
    {
        email = "";
        phone = "";
        fine = 0;
        totalIssued = 0;
    }
    string getRole()
    {
        return "Member";
    }
    void setData(string u, string p, string n, string e, string ph, double f, int issued)
    {
        username = u;
        password = p;
        fullName = n;
        email = e;
        phone = ph;
        fine = f;
        totalIssued = issued;
        card.makeCard(u);    //setter is called as there is no parameterized ctr
    }                       // and is convenient for loading data from a file
    string getEmail()
    {
        return email;
    }
    string getPhone()
    {
        return phone;
    }
    double getFine()
    {
        return fine;
    }
    int getTotalIssued()
    {
        return totalIssued;
    }
    string getCardNumber()
    {
        return card.getCardNumber();
    }
    string getCardStatus()
    {
        return card.getStatus();
    }
    void addFine(double amount)
    {
        fine = fine + amount;
    }
    void clearFine()
    {
        fine = 0;
    }
    void increaseIssued()
    {
        totalIssued++;
    }
    void save(ofstream& out)    // write a member's data to file which is already opened by caller(&-reference)
    {
        out << username << "|" << password << "|" << fullName << "|"
            << email << "|" << phone << "|" << fine << "|" << totalIssued << endl; 
    }
};
class Admin : public Person
{
public:
    Admin()
    {
        username = "admin";
        password = "admin123";
        fullName = "Administrator";
    }
    string getRole()
    {
        return "Admin";
    }
    bool login(string u, string p)
    {
        return u == username && p == password;
    }
};
#endif
