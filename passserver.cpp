/*
  Dylan Morrill
  Password Server adaptor class implementation
*/

/*You just need to pass this size parameter to the constructor of the HashTable.
  Therefore, the real hash table size could be different from the parameter
  size (because prime_below() will be called in the constructor of the HashTable).*/
PassServer::PassServer(size_t size) {
  HashTable<string, string> temp(size);
  server = temp;
  temp.clear();
}

/*destructor clear all elements in server*/
PassServer::~PassServer() {
  server.clear();
}

/*load a password file into the HashTable object. Each line contains a
  pair of username and encrypted password.*/
bool PassServer::load(const char *filename) {
  return server.load(filename);
}

/*The password passed in is in
  plaintext, it should be encrypted before insertion. The pair should not
  be added if the username already exists in the hash table.*/
bool PassServer::addUser(std::pair<string,  string> & kv) {
  kv.second = encrypt(kv.second);
  return server.insert(kv);
}

/*move version of addUser.*/
bool PassServer::addUser(std::pair<string, string> && kv) {
  kv.second = encrypt(kv.second);
  return server.insert(kv);
}

/*delete an existing user with username k using the remove function from the
  HashTable class*/
bool PassServer::removeUser(const string & k) {
  return server.remove(k);
}

/* Note that both passwords passed in
  are in plaintext. They should be encrypted before you interact with the
  hash table. If the user is not in the hash table, return false. If
  p.second does not match the current password, return false. Also return
  false if the new password and the old password are the same
  (i.e., we cannot update the password).*/
bool PassServer::changePassword(const pair<string, string> &p, const string & newpassword) {
pair<string,string> temp(p.first, encrypt(p.second));
  if(server.match(temp)) {
    //pair<string,string> temp;
    //temp.first = p.first;
    temp.second = encrypt(newpassword);
    server.insert(temp);
    return true;
  }
  return false;
}

/*check if a user exists (if user is in the hash table) using contains function*/
bool PassServer::find(const string & user) {
  return server.contains(user);
}

/*Same format as the dump() function in the HashTable class template.*/
void PassServer::dump() {
  server.dump();
}

/*return number of username,password pairs*/
size_t PassServer::size() {
  return server.size();
}

/*Same format as the write_to_file() function in the HashTable class template.*/
bool PassServer::write_to_file(const char *filename) {
  return server.write_to_file(filename);
}

/*use crypt function from unistd library (crypt.h on linprog)*/
string PassServer::encrypt(const string & str) {
  string temp = crypt(str.c_str(), "$1$########");
  temp.erase(0,12);
  return temp;
}