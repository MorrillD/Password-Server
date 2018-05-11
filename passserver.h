/*
  Dylan Morrill
  Password Server adaptor class
*/

#ifndef PASSSERVER_H
#define PASSSERVER_H
#include "hashtable.h"
#include <iostream>
#include <string>
#include <unistd.h>
//#include <crypt.h>

using namespace std;

namespace cop4530 {

  class PassServer {
    private:
      HashTable<string, string> server;

      /*encrypt the parameter str and return the encrypted string*/
      string encrypt(const string & str);

    public:

      /*constructor, create a hash table of the specified size.*/
      PassServer(size_t size = 101);

      /*destructor*/
      ~PassServer();

      /*load a password file into the HashTable object.*/
      bool load(const char *filename);

      /*add a new username and password.*/
      bool addUser(std::pair<string,  string> & kv);

      /*move version of addUser.*/
      bool addUser(std::pair<string, string> && kv);

      /*delete an existing user with username k.*/
      bool removeUser(const string & k);

      /*change an existing user's password.*/
      bool changePassword(const pair<string, string> &p, const string & newpassword);

      /*check if a user exists (if user is in the hash table).*/
      bool find(const string & user);

      /*show the structure and contents of the HashTable object to the screen.*/
      void dump();

      /*return the size of the HashTable (the number of username/password pairs
        in the table).*/
      size_t size();

      /*save the username and password combination into a file.*/
      bool write_to_file(const char *filename);
  };

  #include "passserver.cpp"
}

#endif