/*
  Dylan Morrill
  Hash Table class template
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

namespace cop4530 {

  template <typename K, typename V>
    class HashTable {
      private:
        /*underlying data structure for HashTable*/
        vector<list<pair<K, V>>> theList;

        /*store number of elements in hashtable*/
        int currentSize;

        /*delete all elements in the hash table.*/
        void makeEmpty();

        /*Rehash function.*/
        void rehash();

        /*return the index of the vector entry where k should be stored.*/
        size_t myhash(const K &k) const;

        /*returns largest prime number <= n or zero if input is too large*/
        unsigned long prime_below (unsigned long);

        /*Sets all prime number indexes to 1*/
        void setPrimes(vector<unsigned long>&);

        // max_prime is used by the helpful functions provided
        // to you.
        static const unsigned int max_prime = 1301081;
        // the default_capacity is used if the initial capacity
        // of the underlying vector of the hash table is zero.
        static const unsigned int default_capacity = 11;

      public:

        /*constructor. Where size is default  to 101*/
        HashTable(size_t size = 101);

        /*destructor*/
        ~HashTable();

        /*check if key k is in the hash table.*/
        bool contains(const K & k);

        /*check if key-value pair is in the hash table.*/
        bool match(const std::pair<K, V> &kv) const;

        /*add the key-value pair kv into the hash table.*/
        bool insert(const std::pair<K, V> & kv);

        /*move version of insert.*/
        bool insert (std::pair<K,  V> && kv);

        /*delete the key k and the corresponding value if it is in the hash
          table.*/
        bool remove(const K & k);

        /*delete all elements in the hash table*/
        void clear();

        /*load the content of the file with name filename into the hash table.*/
        bool load(const char *filename);

        /*display all entries in the hash table.*/
        void dump();

        /*return the number of elements in the hash table.*/
        size_t size();

        /*write all elements in the hash table into a file with name filename.*/
        bool write_to_file(const char *filename);
    };

  #include "hashtable.hpp"
}
#endif
