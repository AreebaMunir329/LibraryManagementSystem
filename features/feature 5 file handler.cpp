//#include <iostream>
//#include <fstream>
//#include <string>
//using namespace std;
//
//class FileHandler {
//private:
//    string dataDir;
//
//public:
//    FileHandler() {
//        dataDir = "data/";
//    }
//
//    //  CHECK FILE EXISTS 
//    bool fileExists(string path) {
//        ifstream file(path);
//        return file.good(); //this built in function check if file working properly
//    }
//
//    //  SIMPLE BACKUP 
//    bool backupFile(string filename) {
//        string path = resolvePath(filename); // data\
//
//        if (!fileExists(path)) return false;
//
//        string backupPath = path + ".bak"; // simple backup
//
//        ifstream a(path); //source copy from open
//        ofstream backupfile(backupPath); // new file copy into open
//
//        if (!a.is_open() || !backupfile.is_open()) return false;
//
//        backupfile << a.rdbuf(); //copying
//
//        a.close();
//        backupfile.close();
//
//        return true;
//    }
//
//    //  PATH HANDLING 
//    string resolvePath(string filename) {
//        return dataDir + filename;
//    }
//
//    // WRITE FILE
//    void writeFile(string filename, string data) {
//        string path = resolvePath(filename);
//
//        backupFile(filename); // backup before overwrite
//
//        ofstream file(path);
//        if (!file.is_open()) return;
//
//        file << data;
//        file.close();
//    }
//
//    //  READ FILE 
//    string readFile(string filename) {
//        string path = resolvePath(filename);
//
//        ifstream file(path);
//        if (!file.is_open()) return "";
//
//        string line, content;
//        while (getline(file, line)) {
//            content += line + "\n";
//        }
//
//        file.close();
//        return content;
//    }
//};