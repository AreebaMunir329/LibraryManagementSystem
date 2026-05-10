
#ifndef LIBRARY_GUI_CPP
#define LIBRARY_GUI_CPP

#include <QApplication>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <string>
using namespace std;

#include "../features/feature 5 file handler.cpp"

string qToString(const QString& text)
{
    return text.toStdString();
}

QString sToQ(const string& text)
{
    return QString::fromStdString(text);
}

class LibraryWindow : public QMainWindow
{
    Library library;
    Admin admin;

    bool adminMode;
    string currentUser;

    QWidget* central;
    QVBoxLayout* rootLayout;
    QLabel* titleLabel;
    QLabel* messageLabel;
    QTextEdit* outputBox;

    // stacked widget: page 0 = login, page 1 = dashboard
    QStackedWidget* stack;

    // login page widgets
    QWidget* loginPage;
    QLineEdit* loginUser, * loginPass;
    QLineEdit* regUser, * regPass, * regName, * regEmail, * regPhone;
    QLineEdit* forgotUser, * forgotEmail, * forgotPass;

    // dashboard page widgets
    QWidget* dashPage;
    QVBoxLayout* dashLayout;
    QLabel* loggedInLabel;
    QTabWidget* tabs;

    QLineEdit* bookTitle, * bookAuthor, * bookIsbn, * bookCategory, * bookCopies, * bookSearch;
    QLineEdit* issueIsbn, * issueDate;
    QLineEdit* returnIsbn, * returnDate;
    QLineEdit* reserveUser, * reserveIsbn, * reserveDate;
    QLineEdit* reviewIsbn, * reviewRating, * reviewComment;
    QLineEdit* accountOldPass, * accountNewPass, * accountName, * accountEmail, * accountPhone;
    QLineEdit* settingsFine, * settingsMaxBooks, * settingsReserveDays;

public:
    LibraryWindow()
    {
        adminMode = false;
        currentUser = "";

        central = new QWidget;
        rootLayout = new QVBoxLayout;
        central->setLayout(rootLayout);
        setCentralWidget(central);
        setWindowTitle("Library Management System");
        resize(1050, 720);

        titleLabel = new QLabel("Library Management System");
        titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

        messageLabel = new QLabel("Default admin login: admin / admin123");
        messageLabel->setStyleSheet("color: #7a3b2e;");

        outputBox = new QTextEdit;
        outputBox->setReadOnly(true);
        outputBox->setMinimumHeight(190);

        stack = new QStackedWidget;

        rootLayout->addWidget(titleLabel);
        rootLayout->addWidget(messageLabel);
        rootLayout->addWidget(stack);
        rootLayout->addWidget(outputBox);

        buildLoginPage();
        buildDashPage();

        stack->addWidget(loginPage);   // index 0
        stack->addWidget(dashPage);    // index 1

        stack->setCurrentIndex(0);
    }

    // ---------------------------------------------------------------
    // helpers
    // ---------------------------------------------------------------

    QLineEdit* makeInput(string placeholder)
    {
        QLineEdit* input = new QLineEdit;
        input->setPlaceholderText(sToQ(placeholder));
        return input;
    }

    QPushButton* makeButton(string text)
    {
        QPushButton* button = new QPushButton(sToQ(text));
        button->setMinimumHeight(34);
        return button;
    }

    void setMessage(string text)
    {
        messageLabel->setText(sToQ(text));
    }

    void setOutput(string text)
    {
        outputBox->setPlainText(sToQ(text));
    }

    // ---------------------------------------------------------------
    // build login page (called once in constructor)
    // ---------------------------------------------------------------

    void buildLoginPage()
    {
        loginPage = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout;
        loginPage->setLayout(layout);

        QGroupBox* loginBox = new QGroupBox("Login");
        QFormLayout* loginLayout = new QFormLayout;
        loginUser = makeInput("Username");
        loginPass = makeInput("Password");
        loginPass->setEchoMode(QLineEdit::Password);
        QPushButton* loginButton = makeButton("Login");
        loginLayout->addRow("Username", loginUser);
        loginLayout->addRow("Password", loginPass);
        loginLayout->addRow(loginButton);
        loginBox->setLayout(loginLayout);

        QGroupBox* registerBox = new QGroupBox("Registration");
        QFormLayout* registerLayout = new QFormLayout;
        regUser = makeInput("Username");
        regPass = makeInput("Password");
        regPass->setEchoMode(QLineEdit::Password);
        regName = makeInput("Full name");
        regEmail = makeInput("Email");
        regPhone = makeInput("Phone");
        QPushButton* registerButton = makeButton("Register");
        registerLayout->addRow("Username", regUser);
        registerLayout->addRow("Password", regPass);
        registerLayout->addRow("Full Name", regName);
        registerLayout->addRow("Email", regEmail);
        registerLayout->addRow("Phone", regPhone);
        registerLayout->addRow(registerButton);
        registerBox->setLayout(registerLayout);

        QGroupBox* forgotBox = new QGroupBox("Forget Password");
        QFormLayout* forgotLayout = new QFormLayout;
        forgotUser = makeInput("Username");
        forgotEmail = makeInput("Registered email");
        forgotPass = makeInput("New password");
        forgotPass->setEchoMode(QLineEdit::Password);
        QPushButton* forgotButton = makeButton("Reset Password");
        forgotLayout->addRow("Username", forgotUser);
        forgotLayout->addRow("Email", forgotEmail);
        forgotLayout->addRow("New Password", forgotPass);
        forgotLayout->addRow(forgotButton);
        forgotBox->setLayout(forgotLayout);

        layout->addWidget(loginBox);
        layout->addWidget(registerBox);
        layout->addWidget(forgotBox);

        connect(loginButton, &QPushButton::clicked, this, &LibraryWindow::doLogin);
        connect(registerButton, &QPushButton::clicked, this, &LibraryWindow::doRegister);
        connect(forgotButton, &QPushButton::clicked, this, &LibraryWindow::doForgotPassword);
    }

    // ---------------------------------------------------------------
    // build dashboard page (called once in constructor)
    // ---------------------------------------------------------------

    void buildDashPage()
    {
        dashPage = new QWidget;
        dashLayout = new QVBoxLayout;
        dashPage->setLayout(dashLayout);

        // top bar: logged-in label + logout button
        QWidget* top = new QWidget;
        QHBoxLayout* topLayout = new QHBoxLayout;
        top->setLayout(topLayout);

        loggedInLabel = new QLabel;
        QPushButton* logoutButton = makeButton("Logout");
        topLayout->addWidget(loggedInLabel);
        topLayout->addStretch();
        topLayout->addWidget(logoutButton);

        // tabs placeholder — rebuilt on every login so role-specific tabs work
        tabs = new QTabWidget;

        dashLayout->addWidget(top);
        dashLayout->addWidget(tabs);

        connect(logoutButton, &QPushButton::clicked, this, &LibraryWindow::doLogout);
    }

    // rebuild tabs based on current role, called on every login
    void rebuildTabs()
    {
        // remove the old tabs widget and replace with a fresh one
        dashLayout->removeWidget(tabs);
        delete tabs;

        tabs = new QTabWidget;
        tabs->addTab(makeBooksTab(), "Books");
        tabs->addTab(makeBorrowTab(), "Issue / Return");
        tabs->addTab(makeAccountTab(), "Account");
        tabs->addTab(makeReviewsTab(), "Reviews");
        tabs->addTab(makeReserveTab(), "Reservations");
        if (adminMode)
            tabs->addTab(makeSettingsTab(), "Settings");
        tabs->addTab(makeReportsTab(), "Reports");

        dashLayout->addWidget(tabs);
    }

    // ---------------------------------------------------------------
    // switch to dashboard after login
    // ---------------------------------------------------------------

    void showDashboard()
    {
        if (adminMode)
            loggedInLabel->setText("Logged in as Admin");
        else
            loggedInLabel->setText(sToQ("Logged in as " + currentUser));

        rebuildTabs();
        stack->setCurrentIndex(1);
        refreshOutput();
    }

    // switch back to login page after logout
    void showLoginPage()
    {
        loginUser->clear();
        loginPass->clear();
        stack->setCurrentIndex(0);
    }

    // ---------------------------------------------------------------
    // tab builders
    // ---------------------------------------------------------------

    QWidget* makeBooksTab()
    {
        QWidget* page = new QWidget;
        QGridLayout* layout = new QGridLayout;
        page->setLayout(layout);

        bookSearch = makeInput("Search title, author, ISBN or category");
        bookTitle = makeInput("Title");
        bookAuthor = makeInput("Author");
        bookIsbn = makeInput("ISBN");
        bookCategory = makeInput("Category");
        bookCopies = makeInput("Copies");

        QPushButton* searchButton = makeButton("Search Books");
        QPushButton* addButton = makeButton("Add Book");
        QPushButton* removeButton = makeButton("Remove Book");

        layout->addWidget(bookSearch, 0, 0, 1, 2);
        layout->addWidget(searchButton, 0, 2);
        layout->addWidget(new QLabel("Title"), 1, 0); layout->addWidget(bookTitle, 1, 1);
        layout->addWidget(new QLabel("Author"), 2, 0); layout->addWidget(bookAuthor, 2, 1);
        layout->addWidget(new QLabel("ISBN"), 3, 0); layout->addWidget(bookIsbn, 3, 1);
        layout->addWidget(new QLabel("Category"), 4, 0); layout->addWidget(bookCategory, 4, 1);
        layout->addWidget(new QLabel("Copies"), 5, 0); layout->addWidget(bookCopies, 5, 1);
        layout->addWidget(addButton, 6, 0);
        layout->addWidget(removeButton, 6, 1);

        addButton->setEnabled(adminMode);
        removeButton->setEnabled(adminMode);

        connect(searchButton, &QPushButton::clicked, this, &LibraryWindow::doSearchBooks);
        connect(addButton, &QPushButton::clicked, this, &LibraryWindow::doAddBook);
        connect(removeButton, &QPushButton::clicked, this, &LibraryWindow::doRemoveBook);
        return page;
    }

    QWidget* makeBorrowTab()
    {
        QWidget* page = new QWidget;
        QGridLayout* layout = new QGridLayout;
        page->setLayout(layout);

        issueIsbn = makeInput("ISBN");
        issueDate = makeInput("YYYY-MM-DD");
        returnIsbn = makeInput("ISBN");
        returnDate = makeInput("YYYY-MM-DD");

        QPushButton* issueButton = makeButton("Issue Book");
        QPushButton* returnButton = makeButton("Return Book");
        QPushButton* clearFineButton = makeButton("Clear Fine");

        // username comes from currentUser automatically — no input needed
        layout->addWidget(new QLabel("Issue ISBN"), 0, 0); layout->addWidget(issueIsbn, 0, 1);
        layout->addWidget(new QLabel("Issue Date"), 1, 0); layout->addWidget(issueDate, 1, 1);
        layout->addWidget(issueButton, 2, 1);
        layout->addWidget(new QLabel("Return ISBN"), 3, 0); layout->addWidget(returnIsbn, 3, 1);
        layout->addWidget(new QLabel("Return Date"), 4, 0); layout->addWidget(returnDate, 4, 1);
        layout->addWidget(returnButton, 5, 1);
        layout->addWidget(clearFineButton, 6, 1);

        // clear fine is admin-only
        clearFineButton->setEnabled(adminMode);

        connect(issueButton, &QPushButton::clicked, this, &LibraryWindow::doIssueBook);
        connect(returnButton, &QPushButton::clicked, this, &LibraryWindow::doReturnBook);
        connect(clearFineButton, &QPushButton::clicked, this, &LibraryWindow::doClearFine);
        return page;
    }

    QWidget* makeAccountTab()
    {
        QWidget* page = new QWidget;
        QFormLayout* layout = new QFormLayout;
        page->setLayout(layout);

        accountOldPass = makeInput("Old password");
        accountOldPass->setEchoMode(QLineEdit::Password);
        accountNewPass = makeInput("New password");
        accountNewPass->setEchoMode(QLineEdit::Password);
        accountName = makeInput("New full name");
        accountEmail = makeInput("New email");
        accountPhone = makeInput("New phone");

        QPushButton* changeButton = makeButton("Change Password");
        QPushButton* updateButton = makeButton("Update Details");

        layout->addRow("Old Password", accountOldPass);
        layout->addRow("New Password", accountNewPass);
        layout->addRow(changeButton);
        layout->addRow("Full Name", accountName);
        layout->addRow("Email", accountEmail);
        layout->addRow("Phone", accountPhone);
        layout->addRow(updateButton);

        changeButton->setEnabled(!adminMode);
        updateButton->setEnabled(!adminMode);

        connect(changeButton, &QPushButton::clicked, this, &LibraryWindow::doChangePassword);
        connect(updateButton, &QPushButton::clicked, this, &LibraryWindow::doUpdateDetails);
        return page;
    }

    QWidget* makeReviewsTab()
    {
        QWidget* page = new QWidget;
        QFormLayout* layout = new QFormLayout;
        page->setLayout(layout);

        reviewIsbn = makeInput("ISBN");
        reviewRating = makeInput("1-5");
        reviewComment = makeInput("Comment");

        layout->addRow("ISBN", reviewIsbn);

        QPushButton* viewButton = makeButton("View Reviews");

        if (!adminMode)
        {
            // members: can view or submit a review
            layout->addRow("Rating (1-5)", reviewRating);
            layout->addRow("Comment", reviewComment);
            QPushButton* submitButton = makeButton("Submit Review");
            layout->addRow(viewButton);
            layout->addRow(submitButton);
            connect(submitButton, &QPushButton::clicked, this, &LibraryWindow::doSubmitReview);
        }
        else
        {
            // admin: can view or delete a review
            QPushButton* deleteButton = makeButton("Delete Review");
            layout->addRow(viewButton);
            layout->addRow(deleteButton);
            connect(deleteButton, &QPushButton::clicked, this, &LibraryWindow::doDeleteReview);
        }

        connect(viewButton, &QPushButton::clicked, this, &LibraryWindow::doViewReviews);
        return page;
    }

    QWidget* makeReserveTab()
    {
        QWidget* page = new QWidget;
        QFormLayout* layout = new QFormLayout;
        page->setLayout(layout);

        reserveUser = makeInput("Username");
        reserveIsbn = makeInput("ISBN");
        reserveDate = makeInput("YYYY-MM-DD");
        QPushButton* reserveButton = makeButton("Reserve Book");

        layout->addRow("Username", reserveUser);
        layout->addRow("ISBN", reserveIsbn);
        layout->addRow("Date", reserveDate);
        layout->addRow(reserveButton);

        connect(reserveButton, &QPushButton::clicked, this, &LibraryWindow::doReserveBook);
        return page;
    }

    QWidget* makeSettingsTab()
    {
        QWidget* page = new QWidget;
        QFormLayout* layout = new QFormLayout;
        page->setLayout(layout);

        settingsFine = makeInput("Fine rate");
        settingsMaxBooks = makeInput("Max books");
        settingsReserveDays = makeInput("Reservation days");
        settingsFine->setText(sToQ(to_string((int)library.settings.fineRate)));
        settingsMaxBooks->setText(sToQ(to_string(library.settings.maxBooks)));
        settingsReserveDays->setText(sToQ(to_string(library.settings.reservationDays)));

        QPushButton* saveButton = makeButton("Save Settings");
        layout->addRow("Fine Rate", settingsFine);
        layout->addRow("Max Books", settingsMaxBooks);
        layout->addRow("Reservation Days", settingsReserveDays);
        layout->addRow(saveButton);

        connect(saveButton, &QPushButton::clicked, this, &LibraryWindow::doSaveSettings);
        return page;
    }

    QWidget* makeReportsTab()
    {
        QWidget* page = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout;
        page->setLayout(layout);

        QPushButton* reportsButton = makeButton("Refresh Reports");
        QPushButton* booksButton = makeButton("View All Books");
        QPushButton* membersButton = makeButton("View Members");
        layout->addWidget(reportsButton);
        layout->addWidget(booksButton);
        layout->addWidget(membersButton);
        layout->addStretch();

        membersButton->setEnabled(adminMode);

        connect(reportsButton, &QPushButton::clicked, this, &LibraryWindow::doReports);
        connect(booksButton, &QPushButton::clicked, this, &LibraryWindow::doViewBooks);
        connect(membersButton, &QPushButton::clicked, this, &LibraryWindow::doViewMembers);
        return page;
    }

    // ---------------------------------------------------------------
    // slots
    // ---------------------------------------------------------------

    void doLogin()
    {
        string username = qToString(loginUser->text());
        string password = qToString(loginPass->text());

        if (admin.login(username, password))
        {
            adminMode = true;
            currentUser = "admin";
            setMessage("Admin login successful.");
            showDashboard();
        }
        else if (library.memberLogin(username, password))
        {
            adminMode = false;
            currentUser = username;
            setMessage("Member login successful.");
            showDashboard();
        }
        else
            setMessage("Wrong username or password.");
    }

    void doRegister()
    {
        bool ok = library.signup(qToString(regUser->text()), qToString(regPass->text()),
            qToString(regName->text()), qToString(regEmail->text()),
            qToString(regPhone->text()));
        if (ok)
            setMessage("Registration successful. You can login now.");
        else
            setMessage("Registration failed. Check all fields — email must contain @ and a dot after it.");
    }

    void doForgotPassword()
    {
        string msg = library.forgotPassword(qToString(forgotUser->text()),
            qToString(forgotEmail->text()),
            qToString(forgotPass->text()));
        setMessage(msg);
    }

    void doLogout()
    {
        adminMode = false;
        currentUser = "";
        setMessage("Logged out.");
        setOutput("");
        showLoginPage();
    }

    void doSearchBooks()
    {
        setOutput(library.listBooks(qToString(bookSearch->text())));
    }

    void doAddBook()
    {
        string msg = library.addBook(qToString(bookTitle->text()), qToString(bookAuthor->text()),
            qToString(bookIsbn->text()), qToString(bookCategory->text()),
            toInt(qToString(bookCopies->text())));
        setMessage(msg);
        setOutput(library.listBooks(""));
    }

    void doRemoveBook()
    {
        string msg = library.removeBook(qToString(bookIsbn->text()));
        setMessage(msg);
        setOutput(library.listBooks(""));
    }

    void doIssueBook()
    {
        string msg = library.issueBook(currentUser, qToString(issueIsbn->text()), qToString(issueDate->text()));
        setMessage(msg);
        setOutput(library.memberDashboard(currentUser));
    }

    void doReturnBook()
    {
        string msg = library.returnBook(currentUser, qToString(returnIsbn->text()), qToString(returnDate->text()));
        setMessage(msg);
        setOutput(library.memberDashboard(currentUser));
    }

    void doClearFine()
    {
        if (!adminMode)
        {
            setMessage("Only admin can clear fines.");
            return;
        }
        // admin enters the member username in the Return ISBN field to clear their fine
        string targetUser = qToString(returnIsbn->text());
        if (targetUser == "")
        {
            setMessage("Enter the member username in the Return ISBN field to clear their fine.");
            return;
        }
        string msg = library.clearFine(targetUser);
        setMessage(msg);
        setOutput(library.listMembers());
    }

    void doChangePassword()
    {
        string msg = library.changePassword(currentUser, qToString(accountOldPass->text()),
            qToString(accountNewPass->text()));
        setMessage(msg);
    }

    void doUpdateDetails()
    {
        string msg = library.updateMemberDetails(currentUser, qToString(accountName->text()),
            qToString(accountEmail->text()),
            qToString(accountPhone->text()));
        setMessage(msg);
        setOutput(library.memberDashboard(currentUser));
    }

    void doViewReviews()
    {
        string isbn = qToString(reviewIsbn->text());
        if (isbn == "")
        {
            setMessage("Enter an ISBN to view reviews.");
            return;
        }
        setOutput(library.reviewsForBook(isbn));
    }

    void doSubmitReview()
    {
        string isbn = qToString(reviewIsbn->text());
        int    rating = toInt(qToString(reviewRating->text()));
        string comment = qToString(reviewComment->text());
        string msg = library.addReview(currentUser, isbn, rating, comment);
        setMessage(msg);
    }

    void doDeleteReview()
    {
        string isbn = qToString(reviewIsbn->text());
        if (isbn == "")
        {
            setMessage("Enter an ISBN to delete a review.");
            return;
        }
        setMessage(library.deleteReview(isbn));
    }

    void doReserveBook()
    {
        string user = adminMode ? qToString(reserveUser->text()) : currentUser;
        string msg = library.reserveBook(user, qToString(reserveIsbn->text()), qToString(reserveDate->text()));
        setMessage(msg);
    }

    void doSaveSettings()
    {
        library.settings.fineRate = toDouble(qToString(settingsFine->text()));
        library.settings.maxBooks = toInt(qToString(settingsMaxBooks->text()));
        library.settings.reservationDays = toInt(qToString(settingsReserveDays->text()));
        library.settings.save();
        setMessage("Settings saved.");
    }

    void doReports()
    {
        setOutput(library.reports() + "\n" + library.leaderboard());
    }

    void doViewBooks()
    {
        setOutput(library.listBooks(""));
    }

    void doViewMembers()
    {
        setOutput(library.listMembers());
    }

    void refreshOutput()
    {
        if (adminMode)
            setOutput(library.listMembers());
        else
            setOutput(library.memberDashboard(currentUser));
    }
};

int runLibraryGUI(int argc, char* argv[])
{
    QApplication app(argc, argv);
    LibraryWindow window;
    window.show();
    return app.exec();
}

#endif
