/*
  Dylan Morrill
  Password Server driver
*/

#include <iostream>
#include <string>
#include "passserver.h"
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;
using namespace cop4530;

void Menu();
int getch(void);

int main() {
  int tableSize;

  cout << "Enter preferred hash table capacity: ";
  cin >> tableSize;
  cout << tableSize;

  PassServer server(tableSize);
  char choice;
  string temp;
  string stemp;
  int c;
  pair<string, string> userPair;

  do {
    Menu();
    cin >> choice;

    switch(choice) {
      case 'l'  : cout << "Enter password file name to load from: ";
                  cin >> userPair.first;

                  if(server.load(userPair.first.c_str()) == false) {
                    cout << "Error: Cannot open file " << userPair.first << endl;
                  }
                  break;
      case 'a'  : cout << "Enter username: ";
                  cin >> userPair.first;
                  cout << "Enter password: ";

                  c = getch();
                  while((c = getch()) != 10) {
                    stemp += (char) c;
                  }
                  cout << stemp << endl;
                  //cin >> userPair.second;
                  cout << endl;

                  if(server.addUser(userPair) == false) {
                    cout << "*****Error: User already exists. Could not add user." << endl;
                  }
                  else {
                    cout << "User " << userPair.first << " added." << endl;
                  }

                  break;
      case 'r'  : cout << "Enter username: ";
                  cin >> userPair.first;

                  if(server.removeUser(userPair.first) == false) {
                    cout << "*****Error: User not found.  Could not delete user" << endl;
                  }
                  else {
                    cout << "User " << userPair.first << " deleted." << endl;
                  }
                  break;
      case 'c'  : cout << "Enter username    : ";
                  cin  >> userPair.first;
                  cout << "Enter password    : ";
                  cin >> userPair.second;
                  cout << endl;
                  cout << "Enter new password: ";
                  cin >> temp;
                  cout << endl;

                  if(server.changePassword(userPair, temp) == false) {
                    cout << "*****Error: Could not change user password" << endl;
                  }
                  else {
                    cout << "Password changed for user " << userPair.first << endl;
                  }
                  break;
      case 'f'  : cout << "Enter username    : ";
                  cin >> userPair.first;
                  cout << endl;

                  if(server.find(userPair.first) == false) {
                    cout << "User '" << userPair.first << "' not found. " << endl;
                  }
                  else {
                    cout << "User '" << userPair.first << "' found. " << endl;
                  }
                  break;
      case 'd'  : server.dump();
                  break;
      case 's'  : cout << "Size of hashtable: " << server.size() << endl;
                  break;
      case 'w'  : cout << "Enter password file name to write to: ";
                  cin >> userPair.first;

                  server.write_to_file(userPair.first.c_str());
                  break;
      case 'x'  : break;
      default   : cout << "*****Error: Invalid entry.  Try again." << endl;
                  break;
    }
  }while(choice != 'x');
}
void Menu()
{
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}

/*implementation of getch found online because OS is not compatible with conio.h
  library, used to hide input as user types password*/
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}