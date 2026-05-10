#ifndef REPORTS_CPP
#define REPORTS_CPP

#include <string>
using namespace std;

class ReportData
{
public:
    int totalBooks;
    int totalMembers;
    int issuedNow;
    int returned;
    double totalFine;
    string topBook;

    ReportData()
    {
        totalBooks = 0;
        totalMembers = 0;
        issuedNow = 0;
        returned = 0;
        totalFine = 0;
        topBook = "None";
    }

    string makeText()
    {
        string ans = "Reports and Analytics:\n";
        ans += "Total books: " + to_string(totalBooks) + "\n";
        ans += "Total members: " + to_string(totalMembers) + "\n";
        ans += "Currently issued books: " + to_string(issuedNow) + "\n";
        ans += "Returned records: " + to_string(returned) + "\n";
        ans += "Outstanding fines: Rs. " + to_string((int)totalFine) + "\n";
        ans += "Most borrowed title: " + topBook + "\n";
        return ans;
    }
};

#endif
