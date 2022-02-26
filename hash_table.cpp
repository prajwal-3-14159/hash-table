#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <string.h>

#define TABLE_SIZE 29209   //prime number larger than 20,000 is used so we get a more evenly distributed hash table

using namespace std;

typedef struct User        //struct user stores the login details of the user
{
    string username;
    string password;
    struct User* next=NULL;
} users;

unsigned int Hash (string name)   //given hash funtion gives values between 0 and 29208
{
    int length = name.length();
    unsigned int hash_value = 1;
    for (int i=0; i<length; i++)
    {
        hash_value += name[i];
        hash_value = hash_value * name[i];
    }
    hash_value = hash_value % TABLE_SIZE;
    return hash_value;
}
  
bool hash_table_insert(users* hash_table[], string username, string password) //inserts username and password in hash table
{
    users* person = new users;
    
    person->username = username;
    person->password = password;

    if (person == NULL){
        return false;
    }
    long int index = Hash(person->username);

    if (hash_table[index] == NULL)
    {
        hash_table[index] = person;
    }

    else{
        users* temp = new users();
        temp = person;
        temp->next = hash_table[index];
        hash_table[index] = person;
    }
   
    return true;
}


void Print_table(users* hash_table[])       //can be used to print the hash table
{
    for (int i=0; i<TABLE_SIZE; i++)
    {
        if (hash_table[i] == NULL)
        {
            cout<<i<<" "<<"---"<<endl;
        }
        else{
            cout<<i<<" ";
            users* temp = hash_table[i];
            while (temp != NULL)
            {
                cout<<temp->username<<" ";
                temp = temp->next;
            }
            cout<<endl;
        }
    }
}

users* find_user(string username, users* hash_table[])  //used to find user by username
{
    int index = Hash(username);
    users* temp = hash_table[index];
    
    while ( temp != NULL && (temp->username != username))
    {
        temp = temp->next;
    }
    
    return temp;

}

int main()
{
    users *hash_table[TABLE_SIZE]{NULL};
    users arr[100];

    ifstream my_file;
    my_file.open("userInfo.csv");     //opening the csv file

    if(!my_file.is_open()) cout<< "ERROR: file open" <<endl;

    string username;
    string password;

    while(my_file.good())
    {
        getline(my_file, username, ',');
        getline(my_file, password, '\n');
        hash_table_insert(hash_table, username, password);   //adding elements from csv  file in hash table
    }

    my_file.close();

    //Print_table(hash_table);

    string user_name, pass_word;        //the login interface
    cout<<"username: ";
    cin>>user_name;
    
    if (find_user(user_name, hash_table) == NULL)
    {
        cout<<endl<<"Error: Name not found"<<endl;
        return 0;
    }
    else{
         cout<<"password: ";
         cin>>pass_word;
         if (pass_word == find_user(user_name, hash_table)->password)
         {
             cout<<endl<<"Login successful"<<endl;
         }
         else{
             cout<<endl<<"Incorrect password"<<endl;
         }
    }
    
    return 0;
}