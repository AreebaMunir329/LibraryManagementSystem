#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct Reservation {
    string reservationId;
    string isbn;
    string memberUsername;
    string reservedOn;
    int notified=0;
    string notifiedOn;
    int expired=0;
};
class ReservationManager {
private:
    Reservation reservations[100];
    int count = 0;
    string filePath = "reservations.txt";
public:
    // reserve book
    bool reserveBook(string isbn, string username, string date) {
        for (int i = 0; i < count; i++) {
            if (reservations[i].isbn == isbn &&
                reservations[i].memberUsername == username &&
                reservations[i].expired == 0)
                return false;
        }
        Reservation r;
        r.reservationId = to_string(count + 1);
        r.isbn = isbn;
        r.memberUsername = username;
        r.reservedOn = date;
        r.notified = 0;
        r.notifiedOn = "";
        r.expired = 0;

        reservations[count++] = r;
        save();
        return true;
    }

    // CANCEL RESERVATION
    bool cancelReservation(string isbn, string username) {
        for (int i = 0; i < count; i++) {
            if (reservations[i].isbn == isbn &&
                reservations[i].memberUsername == username &&
                reservations[i].expired == 0) {

                reservations[i].expired = 1;
                save();
                return true;
            }
        }
        return false;
    }
    // GET YEAR
    int getYear(string date) {
        return stoi(date.substr(0, 4));
    }
    // GET MONTH
    int getMonth(string date) {
        return stoi(date.substr(5, 2));
    }
    // GET DAY
    int getDay(string date) {
        return stoi(date.substr(8, 2));
    }
    // CONVERT FULL DATE INTO TOTAL DAYS (APPROXIMATE)
    int toDays(string date) {
        int y = getYear(date);
        int m = getMonth(date);
        int d = getDay(date);
        return y * 365 + m * 30 + d; // simple approximation
    }
    // EXPIRE OLD RESERVATIONS (USING WINDOW)
    void expireOldReservations(string today, int windowDays, string isbn) {
        int todayTotal = toDays(today);
        for (int i = 0; i < count; i++) {
            if (reservations[i].isbn == isbn &&
                reservations[i].notified == 1 &&
                reservations[i].expired == 0) {
                int notifiedTotal = toDays(reservations[i].notifiedOn);
                if ((todayTotal - notifiedTotal) > windowDays) {
                    reservations[i].expired = 1;
                }
            }
        }
    }

    // NOTIFY NEXT MEMBER
    string notifyNext(string isbn, string today) {
        for (int i = 0; i < count; i++) {
            if (reservations[i].isbn == isbn &&
                reservations[i].notified == 0 &&
                reservations[i].expired == 0) {

                reservations[i].notified = 1;
                reservations[i].notifiedOn = today;
                save();
                return reservations[i].memberUsername;
            }
        }
        return "";
    }

    // MAIN FUNCTION CALL WHEN BOOK IS RETURNED
    void processReturn(string isbn, string today, int windowDays) {
        //  expire old reservations
        expireOldReservations(today, windowDays, isbn);
// notify next valid user
        string user = notifyNext(isbn, today);
        if (user != "") {
            cout << "Notify: " << user << endl;
        }
        else {
            cout << "No pending reservations\n";
        }

        save();
    }

    // LOAD DATA 
    void load() {
        count = 0;
        ifstream file(filePath);
        if (!file.is_open()) return;

        while (!file.eof()) {
            Reservation r;

            getline(file, r.reservationId, '|');
            getline(file, r.isbn, '|');
            getline(file, r.memberUsername, '|');
            getline(file, r.reservedOn, '|');

            string temp;
            getline(file, temp, '|');
            if (temp == "") continue;
            r.notified = stoi(temp);

            getline(file, r.notifiedOn, '|');

            getline(file, temp);
            r.expired = stoi(temp);

            if (r.reservationId != "")
                reservations[count++] = r;
        }
        file.close();
    }

    // SAVE DATA TO FILE
    void save() {
        ofstream file(filePath);

        for (int i = 0; i < count; i++) {
            file << reservations[i].reservationId << "|"
                << reservations[i].isbn << "|"
                << reservations[i].memberUsername << "|"
                << reservations[i].reservedOn << "|"
                << reservations[i].notified << "|"
                << reservations[i].notifiedOn << "|"
                << reservations[i].expired << "\n";
        }

        file.close();
    }
};
int main() {
    ReservationManager manager;
    manager.load();

    int choice;
    string isbn, username, date;
    int windowDays;

    do {
        cout << "\n===== Reservation System =====\n";
        cout << "1. Reserve Book\n";
        cout << "2. Cancel Reservation\n";
        cout << "3. Process Book Return\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            cout << "Enter ISBN: ";
            cin >> isbn;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Date (YYYY-MM-DD): ";
            cin >> date;

            if (manager.reserveBook(isbn, username, date))
                cout << "Reservation successful\n";
            
            else
                cout << "Already reserved or invalid\n";
            break;

        case 2:
            cout << "Enter ISBN: ";



            cin >> isbn;
            cout << "Enter Username: ";
            cin >> username;

            if (manager.cancelReservation(isbn, username))
                cout << "Reservation cancelled\n";
            else
                cout << "Reservation not found\n";
            break;

        case 3:
            cout << "Enter ISBN: ";
            cin >> isbn;
            cout << "Enter today's date (YYYY-MM-DD): ";
            cin >> date;
            cout << "Enter window days: ";
            cin >> windowDays;

            manager.processReturn(isbn, date, windowDays);
            break;

        case 4:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice\n";
        }

    } while (choice != 4);

    return 0;
}