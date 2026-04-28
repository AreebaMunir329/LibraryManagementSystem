#include<iostream>
#include<fstream>
#include<string>
using namespace std;
const int limit=100;
void clearBuffer(){
    cin.clear();
    while(cin.get()!='\n' && !cin.eof());
}
//member function to get user data
class Member{
    int userId;
    string userName;
    string password;
    string email;
    string phoneNumber;
    string fullName;
public:
    //default constructor
    Member(): userId(0){}
    int getId(){return userId;}
    //getter functions
    string getUserName(){return userName;}
    string getPassword(){return password;}
    string getEmail(){return email;}
    string getPhoneNumber(){ return phoneNumber;}
    string getFullName(){return fullName;}
    //setter functions
    void setId(int i){userId=i;}
    void setUserName(string n){userName=n;}
    void setPassword(string p){password=p;}
    void setEmail(string e){email=e;}
    void setPhoneNumber(string pn){phoneNumber=pn;}
    void setFullName(string fn){fullName=fn;}
    //checking duplication of password
    bool checkPassword(string p){
        return password==p;
        
    }
    //file handeling (saving into file and loading from file )
    void save(ofstream&f)const{
        f<<userId<<endl
        <<userName<<endl
        <<password<<endl
        <<email<<endl
        <<phoneNumber<<endl
        <<fullName<<endl;
    }
    void load(ifstream&f){
        f>>userId;
        f.ignore();
        getline(f,userName);
        getline(f,password);
        getline(f,email);
        getline(f,phoneNumber);
        getline(f,fullName);
    }
};
class userManagement{
    Member members[limit];
    int memberCount;
    int nextId;
    const string fileName="userData.txt";
    //checking for duplication of names
    int checkUserName(string name){
        for(int j=0;j<memberCount;j++){
            if(name==members[j].getUserName())
                return j;
        }
            return -1;
    }
public:
    userManagement(): memberCount(0),nextId(1){}
    void saveData(){
        ofstream f;
        f.open(fileName);
        f<<memberCount<<endl<<nextId<<endl;
        for(int i=0;i<memberCount;i++){
            members[i].save(f);
        }
    }
    void loadData(){
        ifstream f(fileName);
        if(!f){
            cout<<"failed to open file:("<<endl;
            return;
        }
        f>>memberCount>>nextId;
        f.ignore();
        for(int i=0;i<memberCount;i++){
            members[i].load(f);
        }
    }
    // signup
    void signup(){
        if(memberCount>=limit){
            cout<<"limit exceeded!"<<endl;
            return;
        }
        string uname,pwd,phone,email,fName;
        cout<<"username: ";
        getline(cin,uname);
            while(checkUserName(uname)!=-1){
                cout<<"user name already exists:( enter again:";
            getline(cin,uname);
            }
        cout<<"password: ";
        getline(cin,pwd);
        cout<<"email: ";
        getline(cin,email);
        cout<<"phoneNumber: ";
        getline(cin,phone);
        cout<<"fullName: ";
        getline(cin,fName);
        //sets given data of a new member
        Member m;
        m.setId(nextId++);
        m.setUserName(uname);
        m.setPassword(pwd);
        m.setEmail(email);
        m.setPhoneNumber(phone);
        m.setFullName(fName);
        //saves data of the user
        members[memberCount++] = m;
        saveData();
        cout<<"Registration successful!"<<endl;
    }
    int login(){
        string uname,pwd;
        cout<<"userName: ";
        getline(cin,uname);
        cout<<"password: ";
        getline(cin,pwd);
        int index=checkUserName(uname);
        //checking credentials
        if(index == -1 || !members[index].checkPassword(pwd)){
            cout<<"incorrect credentials! "<<endl;
            return -1;
        }
        cout<<"sucessfull login \n welcome "<<members[index].getUserName()<<" :)"<<endl;
        return index;
    }
    void changepassword(int index){
        if(index < 0 || index >= memberCount){
            cout<<"Invalid user!"<<endl;
            return;
        }
        string pwd,newpwd,cpwd;
        cout<<"old password: ";
        getline(cin,pwd);
        if(members[index].getPassword()!=pwd){
            cout<<"incorrect password!"<<endl;
        return;
    }
    cout<<"new password: ";
        getline(cin,newpwd);
        cout<<"confirm password: ";
        getline(cin,cpwd);
        if(newpwd!=cpwd){
            cout<<"passwords dont match!"<<endl;
        return;
    }
    members[index].setPassword(newpwd);
        saveData();
        cout<<"password updated successfully:)"<<endl;
    }
    void updateData(int index){
        if(index < 0 || index >= memberCount){
            cout<<"Invalid user!"<<endl;
            return;
        }
        string uname,email,phone;
        cout<<"new userName: ";
        getline(cin,uname);
        cout<<"new email: ";
        getline(cin,email);
        cout<<"new phone: ";
        getline(cin,phone);
        if(!uname.empty()){members[index].setUserName(uname);}
        if(!email.empty()){members[index].setEmail(email);}
        if(!phone.empty()){members[index].setPhoneNumber(phone);}
        saveData();
        cout<<"data updated successfully:)"<<endl;
    }
    void forgetPassword(){
        string uname,email,newpwd,cpwd;
        int index;
        cout<<"enter userName: ";
        getline(cin,uname);
        index=checkUserName(uname);
        if(index == -1){
            cout<<"user not found!"<<endl;
            return;
        }
        cout<<"enter email: ";
        getline(cin,email);
        if(email!=members[index].getEmail()){
            cout<<"email not matched:("<<endl;
            return;
        }
        cout<<"new password: ";
            getline(cin,newpwd);
            cout<<"confirm password: ";
            getline(cin,cpwd);
            if(newpwd!=cpwd){
                cout<<"passwords dont match!"<<endl;
            return;
        }
        members[index].setPassword(newpwd);
            saveData();
            cout<<"password updated successfully:)"<<endl;
    }
};
int main(){
    userManagement um;
    um.loadData();
    int choice;
    do{
        cout<<"\n1. Signup\n2. Login\n3. Forget Password\n0. Exit\nChoice: ";
        cin>>choice;
        if(cin.fail()){
            clearBuffer();
            cout<<"Invalid input!"<<endl;
            continue;
        }
        cin.ignore();
        if(choice == 1){
            um.signup();
        }
        else if(choice == 2){
            int idx = um.login();
            if(idx != -1){
                int ch;
                cout<<"\n1. Change Password\n2. Update Data\n0. Logout\nChoice: ";
                cin>>ch;
                if(cin.fail()){
                    clearBuffer();
                    cout<<"Invalid input!"<<endl;
                    continue;
                }
                cin.ignore();
                if(ch != 0 && ch != 1 && ch != 2){
                    cout<<"Invalid choice!"<<endl;
                    continue;
                }
                if(ch == 1){
                    um.changepassword(idx);
                }
                else if(ch == 2){
                    um.updateData(idx);
                }
                // ch == 0  logout (do nothing)
            }
        }
        else if(choice == 3){
            um.forgetPassword();
        }
        else if(choice != 0){
            cout<<"Invalid choice!"<<endl;
        }
    }while(choice != 0);
    return 0;
}
