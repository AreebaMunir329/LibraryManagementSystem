#ifndef SETTINGS_AND_LEADERBOARD_CPP
#define SETTINGS_AND_LEADERBOARD_CPP

#include <fstream>
using namespace std;

class Settings
{
public:
    double fineRate;
    int maxBooks;
    int reservationDays;

    Settings()  //default settings
    {
        fineRate = 10;
        maxBooks = 3;
        reservationDays = 3;
    }

    void load()
    {
        ifstream in("settings.txt");
        if (!in)
            return;

        in >> fineRate >> maxBooks >> reservationDays;
        in.close();
    }

    void save()
    {
        ofstream out("settings.txt");
        out << fineRate << " " << maxBooks << " " << reservationDays << endl;
        out.close();
    }
};

#endif
