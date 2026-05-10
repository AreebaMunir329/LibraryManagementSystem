#ifndef FILE_HANDLER_AND_LIBRARY_CPP
#define FILE_HANDLER_AND_LIBRARY_CPP

#include <fstream>
#include <string>
using namespace std;

#include "User_Management.cpp"
#include "bookmanagement.cpp"
#include "feature 3 member mangement borrowed book.cpp"
#include "reviews&ratings.cpp"
#include "feature 7 book reservation system.cpp.txt"
#include "Settings&leaderboard.cpp"
#include "BookCategoriesAndGenereBasedBrowsing.cpp"
#include "reports.cpp"

const int MAX_BOOKS = 200;
const int MAX_MEMBERS = 100;
const int MAX_RECORDS = 500;
const int MAX_REVIEWS = 200;
const int MAX_RESERVATIONS = 200;

string lowerText(string s)
{
    for (int i = 0; i < (int)s.length(); i++)
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
    return s;
}

int toInt(string s)
{
    int n = 0;
    for (int i = 0; i < (int)s.length(); i++)
        if (s[i] >= '0' && s[i] <= '9')
            n = n * 10 + (s[i] - '0');
    return n;
}

double toDouble(string s)
{
    double n = 0;
    double div = 10;
    bool afterDot = false;

    for (int i = 0; i < (int)s.length(); i++)
    {
        if (s[i] == '.')
            afterDot = true;
        else if (s[i] >= '0' && s[i] <= '9')
        {
            if (!afterDot)
                n = n * 10 + (s[i] - '0');
            else
            {
                n = n + (s[i] - '0') / div;
                div = div * 10;
            }
        }
    }
    return n;
}

int dateToDays(string date)
{
    if ((int)date.length() < 10)
        return 0;

    int y = toInt(date.substr(0, 4));
    int m = toInt(date.substr(5, 2));
    int d = toInt(date.substr(8, 2));
    return y * 365 + m * 30 + d;
}

string addDays(string date, int days)
{
    if ((int)date.length() < 10)
        return date;

    int y = toInt(date.substr(0, 4));
    int m = toInt(date.substr(5, 2));
    int d = toInt(date.substr(8, 2));
    d = d + days;

    while (d > 30)
    {
        d = d - 30;
        m++;
    }
    while (m > 12)
    {
        m = m - 12;
        y++;
    }

    string mm = "";
    string dd = "";
    if (m < 10)
        mm = "0";
    if (d < 10)
        dd = "0";

    return to_string(y) + "-" + mm + to_string(m) + "-" + dd + to_string(d);
}

class BorrowInfo
{
    Member* member;
    Book* book;
    BorrowRecord* record;

public:
    BorrowInfo()
    {
        member = NULL;
        book = NULL;
        record = NULL;
    }

    void set(Member* m, Book* b, BorrowRecord* r)
    {
        member = m;
        book = b;
        record = r;
    }

    string getLine()
    {
        if (member == NULL || book == NULL || record == NULL)
            return "";

        string line = book->getTitle() + " | ISBN: " + book->getIsbn();
        line += " | Issue: " + record->issueDate;
        line += " | Due: " + record->dueDate;
        if (record->returned == 1)
            line += " | Returned: " + record->returnDate;
        else
            line += " | Currently issued";
        return line;
    }
};

class Library
{
    Book books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    BorrowRecord records[MAX_RECORDS];
    Review reviews[MAX_REVIEWS];
    Reservation reservations[MAX_RESERVATIONS];

    int bookCount;
    int memberCount;
    int recordCount;
    int reviewCount;
    int reservationCount;
    int nextBookId;
    int nextRecordId;

public:
    Settings settings;

    Library()
    {
        bookCount = 0;
        memberCount = 0;
        recordCount = 0;
        reviewCount = 0;
        reservationCount = 0;
        nextBookId = 1;
        nextRecordId = 1;
        loadAll();
    }

    void split(string line, string parts[], int& count)
    {
        count = 0;
        string temp = "";
        for (int i = 0; i <= (int)line.length(); i++)
        {
            if (i == (int)line.length() || line[i] == '|')
            {
                parts[count] = temp;
                count++;
                temp = "";
            }
            else
                temp = temp + line[i];
        }
    }

    void loadAll()
    {
        settings.load();
        loadBooks();
        loadMembers();
        loadRecords();
        loadReviews();
        loadReservations();
    }

    void saveAll()
    {
        saveBooks();
        saveMembers();
        saveRecords();
        saveReviews();
        saveReservations();
        settings.save();
    }

    void loadBooks()
    {
        bookCount = 0;
        ifstream in("books.txt");
        string line, p[10];
        int pc;

        while (getline(in, line) && bookCount < MAX_BOOKS)
        {
            split(line, p, pc);
            if (pc >= 8)
            {
                int id = toInt(p[0]);
                books[bookCount].setData(id, p[1], p[2], p[3], p[4],
                    toInt(p[5]), toInt(p[6]), toInt(p[7]));
                if (id >= nextBookId)
                    nextBookId = id + 1;
                bookCount++;
            }
        }
        in.close();
    }

    void saveBooks()
    {
        ofstream out("books.txt");
        for (int i = 0; i < bookCount; i++)
            books[i].save(out);
        out.close();
    }

    void loadMembers()
    {
        memberCount = 0;
        ifstream in("members.txt");
        string line, p[10];
        int pc;

        while (getline(in, line) && memberCount < MAX_MEMBERS)
        {
            split(line, p, pc);
            if (pc >= 7)
            {
                members[memberCount].setData(p[0], p[1], p[2], p[3], p[4],
                    toDouble(p[5]), toInt(p[6]));
                memberCount++;
            }
        }
        in.close();
    }

    void saveMembers()
    {
        ofstream out("members.txt");
        for (int i = 0; i < memberCount; i++)
            members[i].save(out);
        out.close();
    }

    void loadRecords()
    {
        recordCount = 0;
        ifstream in("records.txt");
        string line, p[10];
        int pc;

        while (getline(in, line) && recordCount < MAX_RECORDS)
        {
            split(line, p, pc);
            if (pc >= 7)
            {
                records[recordCount].id = toInt(p[0]);
                records[recordCount].username = p[1];
                records[recordCount].isbn = p[2];
                records[recordCount].issueDate = p[3];
                records[recordCount].dueDate = p[4];
                records[recordCount].returnDate = p[5];
                records[recordCount].returned = toInt(p[6]);
                if (records[recordCount].id >= nextRecordId)
                    nextRecordId = records[recordCount].id + 1;
                recordCount++;
            }
        }
        in.close();
    }

    void saveRecords()
    {
        ofstream out("records.txt");
        for (int i = 0; i < recordCount; i++)
            records[i].save(out);
        out.close();
    }

    void loadReviews()
    {
        reviewCount = 0;
        ifstream in("reviews.txt");
        string line, p[10];
        int pc;

        while (getline(in, line) && reviewCount < MAX_REVIEWS)
        {
            split(line, p, pc);
            if (pc >= 4)
            {
                reviews[reviewCount].username = p[0];
                reviews[reviewCount].isbn = p[1];
                reviews[reviewCount].rating = toInt(p[2]);
                reviews[reviewCount].comment = p[3];
                reviewCount++;
            }
        }
        in.close();
    }

    void saveReviews()
    {
        ofstream out("reviews.txt");
        for (int i = 0; i < reviewCount; i++)
            reviews[i].save(out);
        out.close();
    }

    void loadReservations()
    {
        reservationCount = 0;
        ifstream in("reservations.txt");
        string line, p[10];
        int pc;

        while (getline(in, line) && reservationCount < MAX_RESERVATIONS)
        {
            split(line, p, pc);
            if (pc >= 4)
            {
                reservations[reservationCount].username = p[0];
                reservations[reservationCount].isbn = p[1];
                reservations[reservationCount].date = p[2];
                reservations[reservationCount].active = toInt(p[3]);
                reservationCount++;
            }
        }
        in.close();
    }

    void saveReservations()
    {
        ofstream out("reservations.txt");
        for (int i = 0; i < reservationCount; i++)
            reservations[i].save(out);
        out.close();
    }

    int findBookByIsbn(string isbn)
    {
        for (int i = 0; i < bookCount; i++)
            if (books[i].getIsbn() == isbn)
                return i;
        return -1;
    }

    int findMember(string username)
    {
        for (int i = 0; i < memberCount; i++)
            if (members[i].getUsername() == username)
                return i;
        return -1;
    }

    string getPersonRole(Person* person)
    {
        if (person == NULL)
            return "Unknown";
        return person->getRole();
    }

    bool isValidEmail(string email)
    {
        int atPos = -1;
        int dotPos = -1;

        for (int i = 0; i < (int)email.length(); i++)
            if (email[i] == '@')
                atPos = i;

        if (atPos <= 0)
            return false;

        for (int i = atPos + 1; i < (int)email.length(); i++)
            if (email[i] == '.')
                dotPos = i;

        // dot must come after @, and there must be characters after the dot
        if (dotPos <= atPos + 1)
            return false;
        if (dotPos >= (int)email.length() - 1)
            return false;

        return true;
    }

    bool signup(string u, string p, string name, string email, string phone)
    {
        if (u == "" || p == "" || memberCount >= MAX_MEMBERS)
            return false;
        if (findMember(u) != -1)
            return false;
        if (!isValidEmail(email))
            return false;

        members[memberCount].setData(u, p, name, email, phone, 0, 0);
        memberCount++;
        saveMembers();
        return true;
    }

    bool memberLogin(string u, string p)
    {
        int index = findMember(u);
        if (index == -1)
            return false;
        return members[index].getPassword() == p;
    }

    string forgotPassword(string username, string email, string newPassword)
    {
        int index = findMember(username);
        if (index == -1)
            return "Member not found.";
        if (members[index].getEmail() != email)
            return "Email does not match.";
        if (newPassword == "")
            return "New password cannot be empty.";

        members[index].setData(members[index].getUsername(), newPassword,
            members[index].getFullName(), members[index].getEmail(),
            members[index].getPhone(), members[index].getFine(),
            members[index].getTotalIssued());
        saveMembers();
        return "Password reset successfully.";
    }

    string changePassword(string username, string oldPassword, string newPassword)
    {
        int index = findMember(username);
        if (index == -1)
            return "Member not found.";
        if (members[index].getPassword() != oldPassword)
            return "Old password is wrong.";
        if (newPassword == "")
            return "New password cannot be empty.";

        members[index].setData(members[index].getUsername(), newPassword,
            members[index].getFullName(), members[index].getEmail(),
            members[index].getPhone(), members[index].getFine(),
            members[index].getTotalIssued());
        saveMembers();
        return "Password changed successfully.";
    }

    string updateMemberDetails(string username, string fullName, string email, string phone)
    {
        int index = findMember(username);
        if (index == -1)
            return "Member not found.";

        if (fullName == "")
            fullName = members[index].getFullName();
        if (email == "")
            email = members[index].getEmail();
        if (phone == "")
            phone = members[index].getPhone();

        members[index].setData(members[index].getUsername(), members[index].getPassword(),
            fullName, email, phone, members[index].getFine(),
            members[index].getTotalIssued());
        saveMembers();
        return "Member details updated.";
    }

    string addBook(string title, string author, string isbn, string cat, int copies)
    {
        if (title == "" || isbn == "" || copies <= 0)
            return "Please enter title, ISBN and copies.";

        int existing = findBookByIsbn(isbn);
        if (existing != -1)
        {
            // book already exists — just add the new copies to the stock
            int oldTotal = books[existing].getTotal();
            int oldAvail = books[existing].getAvailable();
            int oldIssued = books[existing].getTimesIssued();
            books[existing].setData(books[existing].getId(), books[existing].getTitle(),
                books[existing].getAuthor(), isbn,
                books[existing].getCategory(),
                oldTotal + copies, oldAvail + copies, oldIssued);
            saveBooks();
            return "ISBN already exists. Added " + to_string(copies) + " more copies. Total now: " + to_string(oldTotal + copies) + ".";
        }

        if (bookCount >= MAX_BOOKS)
            return "Book storage is full.";

        books[bookCount].setData(nextBookId, title, author, isbn, cat, copies, copies, 0);
        nextBookId++;
        bookCount++;
        saveBooks();
        return "Book added successfully.";
    }

    string removeBook(string isbn)
    {
        int index = findBookByIsbn(isbn);
        if (index == -1)
            return "Book not found.";
        if (books[index].getAvailable() != books[index].getTotal())
            return "Cannot remove. Some copies are issued.";

        for (int i = index; i < bookCount - 1; i++)
            books[i] = books[i + 1];
        bookCount--;
        saveBooks();
        return "Book removed.";
    }


    bool isValidDate(string date)
    {
        if ((int)date.length() != 10)
            return false;
        if (date[4] != '-' || date[7] != '-')
            return false;

        int y = toInt(date.substr(0, 4));
        int m = toInt(date.substr(5, 2));
        int d = toInt(date.substr(8, 2));

        if (y < 2000 || y > 2100)
            return false;
        if (m < 1 || m > 12)
            return false;
        if (d < 1 || d > 31)
            return false;

        return true;
    }
    int currentBorrowed(string username)
    {
        int total = 0;
        for (int i = 0; i < recordCount; i++)
            if (records[i].username == username && records[i].returned == 0)
                total++;
        return total;
    }

    string issueBook(string username, string isbn, string date)
    {
        if (!isValidDate(date))
            return "Invalid date. Use YYYY-MM-DD format (month 1-12, day 1-31, year 2000-2100).";
        int mi = findMember(username);
        int bi = findBookByIsbn(isbn);
        if (mi == -1)
            return "Member not found.";
        if (bi == -1)
            return "Book not found.";
        if (members[mi].getFine() > 0)
            return "Member has pending fine. Clear fine first.";
        if (currentBorrowed(username) >= settings.maxBooks)
            return "Maximum book limit reached.";
        if (!books[bi].issue())
            return "No copy available. You can reserve it.";
        if (recordCount >= MAX_RECORDS)
            return "Record storage full.";

        records[recordCount].id = nextRecordId++;
        records[recordCount].username = username;
        records[recordCount].isbn = isbn;
        records[recordCount].issueDate = date;
        records[recordCount].dueDate = addDays(date, 14);
        records[recordCount].returnDate = "";
        records[recordCount].returned = 0;
        recordCount++;
        members[mi].increaseIssued();
        saveAll();
        return "Book issued. Due date: " + addDays(date, 14);
    }

    string returnBook(string username, string isbn, string date)
    {
        if (!isValidDate(date))
            return "Invalid date. Use YYYY-MM-DD format (month 1-12, day 1-31, year 2000-2100).";
        // return date must be after the issue date
        for (int _check = 0; _check < recordCount; _check++)
        {
            if (records[_check].username == username && records[_check].isbn == isbn && records[_check].returned == 0)
            {
                if (dateToDays(date) < dateToDays(records[_check].issueDate))
                    return "Return date cannot be before the issue date.";
                break;
            }
        }
        int bi = findBookByIsbn(isbn);
        int mi = findMember(username);
        if (bi == -1 || mi == -1)
            return "Member or book not found.";

        for (int i = 0; i < recordCount; i++)
        {
            if (records[i].username == username && records[i].isbn == isbn && records[i].returned == 0)
            {
                records[i].returned = 1;
                records[i].returnDate = date;
                books[bi].returnCopy();

                int late = dateToDays(date) - dateToDays(records[i].dueDate);
                string msg = "Book returned.";
                if (late > 0)
                {
                    double amount = late * settings.fineRate;
                    members[mi].addFine(amount);
                    msg = "Returned late. Fine added: Rs. " + to_string((int)amount);
                }

                string nextUser = nextReservation(isbn);
                if (nextUser != "")
                    msg = msg + "\nReservation alert: notify " + nextUser;

                saveAll();
                return msg;
            }
        }
        return "No active issue record found.";
    }

    string clearFine(string username)
    {
        int mi = findMember(username);
        if (mi == -1)
            return "Member not found.";
        members[mi].clearFine();
        saveMembers();
        return "Fine cleared.";
    }

    string reserveBook(string username, string isbn, string date)
    {
        if (findMember(username) == -1)
            return "Member not found.";
        if (findBookByIsbn(isbn) == -1)
            return "Book not found.";
        if (reservationCount >= MAX_RESERVATIONS)
            return "Reservation storage full.";

        for (int i = 0; i < reservationCount; i++)
            if (reservations[i].username == username && reservations[i].isbn == isbn && reservations[i].active == 1)
                return "Already reserved by this member.";

        reservations[reservationCount].username = username;
        reservations[reservationCount].isbn = isbn;
        reservations[reservationCount].date = date;
        reservations[reservationCount].active = 1;
        reservationCount++;
        saveReservations();
        return "Book reserved.";
    }

    string nextReservation(string isbn)
    {
        for (int i = 0; i < reservationCount; i++)
        {
            if (reservations[i].isbn == isbn && reservations[i].active == 1)
            {
                reservations[i].active = 0;
                saveReservations();
                return reservations[i].username;
            }
        }
        return "";
    }

    bool hasReturned(string username, string isbn)
    {
        for (int i = 0; i < recordCount; i++)
            if (records[i].username == username && records[i].isbn == isbn && records[i].returned == 1)
                return true;
        return false;
    }

    string addReview(string username, string isbn, int rating, string comment)
    {
        if (rating < 1 || rating > 5)
            return "Rating must be from 1 to 5.";
        if (!hasReturned(username, isbn))
            return "Review allowed only after returning book.";
        if (reviewCount >= MAX_REVIEWS)
            return "Review storage full.";

        // one review per user per book — update if already exists
        for (int i = 0; i < reviewCount; i++)
        {
            if (reviews[i].username == username && reviews[i].isbn == isbn)
            {
                reviews[i].rating = rating;
                reviews[i].comment = comment;
                saveReviews();
                return "Review updated.";
            }
        }

        reviews[reviewCount].username = username;
        reviews[reviewCount].isbn = isbn;
        reviews[reviewCount].rating = rating;
        reviews[reviewCount].comment = comment;
        reviewCount++;
        saveReviews();
        return "Review saved.";
    }

    string deleteReview(string isbn)
    {
        for (int i = 0; i < reviewCount; i++)
        {
            if (reviews[i].isbn == isbn)
            {
                for (int j = i; j < reviewCount - 1; j++)
                    reviews[j] = reviews[j + 1];
                reviewCount--;
                saveReviews();
                return "One review deleted.";
            }
        }
        return "No review found.";
    }

    string listBooks(string key)
    {
        string ans = "Books:\n";
        int found = 0;
        key = lowerText(key);

        for (int i = 0; i < bookCount; i++)
        {
            string all = lowerText(books[i].getTitle() + " " + books[i].getAuthor() + " " +
                books[i].getIsbn() + " " + books[i].getCategory());
            if (key == "" || all.find(key) != string::npos)
            {
                ans += books[i].getTitle() + " | " + books[i].getAuthor() +
                    " | ISBN: " + books[i].getIsbn() +
                    " | " + books[i].getCategory() +
                    " | Available: " + to_string(books[i].getAvailable()) + "/" +
                    to_string(books[i].getTotal()) + "\n";
                found++;
            }
        }

        if (found == 0)
            ans += "No books found.\n";
        return ans;
    }

    string listMembers()
    {
        string ans = "Members:\n";
        for (int i = 0; i < memberCount; i++)
        {
            Person* person = &members[i];
            ans += getPersonRole(person) + " | " + members[i].getUsername() + " | " + members[i].getFullName() +
                " | Card: " + members[i].getCardNumber() +
                " | Fine: Rs. " + to_string((int)members[i].getFine()) +
                " | Total issued: " + to_string(members[i].getTotalIssued()) + "\n";
        }
        if (memberCount == 0)
            ans += "No members registered.\n";
        return ans;
    }

    string memberDashboard(string username)
    {
        int mi = findMember(username);
        if (mi == -1)
            return "Member not found.";

        string ans = "Welcome " + members[mi].getFullName() + "\n";
        ans += "Role: " + getPersonRole(&members[mi]) + "\n";
        ans += "Library Card: " + members[mi].getCardNumber() + " (" + members[mi].getCardStatus() + ")\n";
        ans += "Outstanding fine: Rs. " + to_string((int)members[mi].getFine()) + "\n";
        ans += "Borrowed books:\n";

        bool found = false;
        for (int i = 0; i < recordCount; i++)
        {
            if (records[i].username == username)
            {
                found = true;
                int bookIndex = findBookByIsbn(records[i].isbn);
                if (bookIndex != -1)
                {
                    BorrowInfo info;
                    info.set(&members[mi], &books[bookIndex], &records[i]);
                    ans += info.getLine() + "\n";
                }
                else
                {
                    ans += records[i].isbn + " | Issue: " + records[i].issueDate +
                        " | Due: " + records[i].dueDate + "\n";
                }
            }
        }

        if (!found)
            ans += "No borrowing history.\n";
        return ans;
    }

    string reviewsForBook(string isbn)
    {
        string ans = "Reviews for ISBN " + isbn + ":\n";
        int total = 0;
        int found = 0;

        for (int i = 0; i < reviewCount; i++)
        {
            if (reviews[i].isbn == isbn)
            {
                ans += reviews[i].username + " rated " + to_string(reviews[i].rating) +
                    "/5: " + reviews[i].comment + "\n";
                total += reviews[i].rating;
                found++;
            }
        }

        if (found == 0)
            ans += "No reviews yet.\n";
        else
            ans += "Average rating: " + to_string(total / found) + "/5\n";
        return ans;
    }

    string leaderboard()
    {
        Member temp[MAX_MEMBERS];
        for (int i = 0; i < memberCount; i++)
            temp[i] = members[i];

        for (int i = 0; i < memberCount - 1; i++)
        {
            for (int j = i + 1; j < memberCount; j++)
            {
                if (temp[j].getTotalIssued() > temp[i].getTotalIssued())
                {
                    Member swap = temp[i];
                    temp[i] = temp[j];
                    temp[j] = swap;
                }
            }
        }

        string ans = "Most Active Borrowers:\n";
        int limit = memberCount;
        if (limit > 10)
            limit = 10;

        for (int i = 0; i < limit; i++)
            ans += to_string(i + 1) + ". " + temp[i].getUsername() +
            " - " + to_string(temp[i].getTotalIssued()) + " books\n";
        return ans;
    }

    string reports()
    {
        ReportData data;
        data.totalBooks = bookCount;
        data.totalMembers = memberCount;

        for (int i = 0; i < recordCount; i++)
        {
            if (records[i].returned == 0)
                data.issuedNow++;
            else
                data.returned++;
        }

        for (int i = 0; i < memberCount; i++)
            data.totalFine += members[i].getFine();

        int maxIssued = -1;
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].getTimesIssued() > maxIssued)
            {
                maxIssued = books[i].getTimesIssued();
                data.topBook = books[i].getTitle();
            }
        }

        return data.makeText();
    }
};

#endif
