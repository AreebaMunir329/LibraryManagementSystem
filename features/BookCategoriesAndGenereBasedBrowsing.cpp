
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
const int MAXBOOKS = 200;
const int MAXCATS=50;
class book{
public:
    int id;
    string name;
    string author;
    string isbn;
    string category;
    int totalcpy;
    int availablecpy;
    void setcategory(string cat){
        category=cat;
    }
    string getcategory()const{
        return category;
    }
    string getisbn()const{
        return isbn;
    }
    int getavailablecpy() const {
        return availablecpy;
    }
    void display() const
        {
            cout << "ID: " << id << endl;
            cout << "Title: " << name << endl;
            cout << "Author: " << author << endl;
            cout << "ISBN: " << isbn << endl;
            cout << "Category: " << category << endl;
            cout << "Available copies: " << availablecpy << "/" << totalcpy << endl;
            cout << "------------------------------------------------------------" << endl;
        }
};
class Category{
public:
    string name;
};
class categoryManagement{
    Category category[MAXCATS];
    int catsCount;
    string filePath;
public:
    categoryManagement():catsCount(0){
        filePath="categories.txt";
        loadData();
    }
    bool exists(string cat)const {
        for(int i=0;i<catsCount;i++){
            if(toLower(category[i].name)==toLower(cat))
                return true;
        }
        return false;
    }
    int getIndex(string cat)const {
        for(int i=0;i<catsCount;i++){
            if(toLower(category[i].name)==toLower(cat)){
                return i;
            }
        }
        return -1;
    }
    string toLower(string cat)const{
        for(int i=0;i<(int)cat.length();i++){
            if(cat[i] >= 'A' && cat[i] <= 'Z')
                        cat[i] = cat[i] + 32;
                }
                return cat;
        }
    bool addCategory(string cat){
        if(exists(cat)||catsCount>=MAXCATS)
            return false;
        
            category[catsCount++].name=cat;
            saveData();
            return true;
    }
    bool removeCategory(string cat){
        int index=getIndex(cat);
        if(index!=-1){
                for(int i=index;i<catsCount-1;i++){
                    category[i]=category[i+1];
                }
                catsCount--;
                saveData();
                return true;
            }
            return false;
        }
    void browseByCategory(string cat,book books[MAXBOOKS],int booksCount){
      
        cout<<"Books in category "<<cat<<" are: "<<endl;
        bool found=false;
        if(toLower(cat)=="all"){
            for(int j=0;j<booksCount;j++){
                books[j].display();
                found=true;
            }
        }
        else{
        for(int i=0;i<booksCount;i++){
            if(toLower(books[i].getcategory())==toLower(cat)){
                books[i].display();
                found=true;
            }
        }
    }
            
            if(!found)
                cout<<"No books found in this category! "<<endl;
        }
        bool reassignCategory(string isbn,string newCat,book books[MAXBOOKS],int booksCount){
            if (!exists(newCat))
                return false;
            for(int i=0;i<booksCount;i++){
                if(books[i].getisbn()==isbn){
                    books[i].setcategory(newCat);
                    //file handling of book management system saves book with particular category
                    return true;
                }
            }
            return false;
             
            }
        void allCategoryStats(book books[MAXBOOKS], int booksCount) {
            cout << "\n--- Complete Library Statistics ---" << endl;
            string processed[MAXCATS]; // To track categories we already counter
            int processedCount = 0;

            for (int i = 0; i < booksCount; i++) {
                string currentCat = toLower(books[i].getcategory());
                bool duplicate = false;

                        // Check if this genre was already reported
                for (int j = 0; j < processedCount; j++) {
                    if (processed[j] == currentCat) {
                        duplicate = true;
                        break;
                    }
                }

                if (!duplicate) {
                    int count = 0;
                            // Count every book in the library belonging to this genre
                    for (int k = 0; k < booksCount; k++) {
                        if (toLower(books[k].getcategory()) == currentCat) {
                            count++;
                            }
                        }
                    cout << "Category: " << currentCat << " | Total Titles: " << count << endl;
                    processed[processedCount++] = currentCat; // Mark as done
                }
            }
        }
            void particularCategoryStats(string cats,book books[],int booksCount) {
                int totalTitles = 0;
                int totalAvailable = 0;
                for (int i = 0; i < booksCount; i++) {
                    if (toLower(books[i].getcategory()) == toLower(cats)) {
                        totalTitles++;
                        totalAvailable += books[i].getavailablecpy();
                    }
                }
                cout << "\nCategory: " << cats << endl;
                cout << "Total Titles: " << totalTitles << endl;
                cout << "Available Copies: " << totalAvailable << endl;
            }
    void loadData() {
        catsCount = 0;

        ifstream file(filePath);
        if (!file.is_open()) return;

        string line;

        while (getline(file, line) && catsCount < MAXCATS) {
            if (!line.empty()) {
                category[catsCount].name = line;
                catsCount++;
            }
        }

        file.close();
    }

            // ----- SAVE (file handling converted from Qt) -----

            void saveData() {
                ofstream file(filePath);

                for (int i = 0; i < catsCount; i++) {
                    file << category[i].name << endl;
                }

                file.close();
            }
};


