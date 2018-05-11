/*
  Dylan Morrill
  Hash Table class implementation
*/

/*constructor~ Where the size of the vector is set
  to prime_below(size) (where size is default  to 101),
  where prime_below() is a private member function of the HashTable and
  provided to you.*/
template <typename K, typename V>
HashTable<K,V>::HashTable(size_t size) {
  currentSize = 0;
  theList.resize(prime_below(size));
}

/*destructor~ Delete all elements*/
template <typename K, typename V>
HashTable<K,V>::~HashTable() {
  clear();
}

/*iterate through the list to check if k is in the hash table*/
template <typename K, typename V>
bool HashTable<K,V>::contains(const K & k) {
  auto & whichList = theList[myhash(k)];
	for(typename list<pair<K, V>>::iterator itr = whichList.begin(); itr != whichList.end(); ++itr) {
    if(itr->first == k) {
      return true;
    }
  }

  return false;
}

/*iterate through the list to check if the key-value pair kv is in the hash table*/
template <typename K, typename V>
bool HashTable<K,V>::match(const std::pair<K, V> &kv) const {
  auto & whichList = theList[myhash(kv.first)];
	if(find(begin(whichList), end(whichList), kv) != end(whichList)) {
      return true;
  }
  return false;
}

/*Don't add if kv is already in the hash table. If the key is the hash table but with a
  different value, the value should be updated to the new one with kv. Return true if kv
  is inserted or the value is updated; return false
  otherwise (i.e., if kv is in the hash table)*/
template <typename K, typename V>
bool HashTable<K,V>::insert(const std::pair<K, V> & kv) {
  auto & whichList = theList[myhash(kv.first)];
	if(find(begin(whichList), end(whichList), kv) != end(whichList)) {
		return false;
  }
  else if(contains(kv.first) == true) {
    for(typename list<pair<K, V>>::iterator itr = whichList.begin(); itr != whichList.end(); ++itr) {
      if(itr->first == kv.first) {
        itr->second = kv.second;
      }
      return true;
    }
  }

	whichList.push_back(kv);

	if(++currentSize > theList.size()) {
		rehash();
    cout << "rehashed" << endl;
  }
	return true;

}

/*move version of insert.*/
template <typename K, typename V>
bool HashTable<K,V>::insert (std::pair<K,  V> && kv) {
  const pair<K, V> newPair = move(kv);
  if(insert(newPair) == true) {
    return true;
  }
  else {
    return false;
  }
}

/*delete the key k and the corresponding value if it is in the hash
  table. Return true if k is deleted, return false otherwise (i.e., if
  key k is not in the hash table).*/
template <typename K, typename V>
bool HashTable<K,V>::remove(const K & k) {
  auto & whichList = theList[myhash(k)];
	for(typename list<pair<K, V>>::iterator itr = whichList.begin(); itr != whichList.end(); ++itr) {
    if(itr->first == k) {
      whichList.erase(itr);
    	--currentSize;
    	return true;
    }
  }
	return false;
}

/*delete all elements in the hash table*/
template <typename K, typename V>
void HashTable<K,V>::clear() {
  makeEmpty();
}

/*load the content of the file with name filename into the hash table.
  In the file, each line contains a single pair of key and value,
  separated by a white space.*/
template <typename K, typename V>
bool HashTable<K,V>::load(const char *filename) {
  pair<K, V> kv;
  string input;
  string holder;

  ifstream in(filename);
  if (in.is_open())
  {
    while (getline(in,input))
    {
      istringstream in(input);
      in >> holder;
      kv.first = holder;
      in >> holder;
      kv.second = holder;
      insert(kv);
    }
    in.close();
    return true;
  }
  else {
    return false;
  }
}

/*display all entries in the hash table. If an entry contains multiple
  key-value pairs, separate them by a semicolon character (:) (see the
  provided executable for the exact output format).*/
template <typename K, typename V>
void HashTable<K,V>::dump() {
  for(int i = 0; i < theList.size(); i++) {
    cout << "v[" << i << "]: ";

    for(typename list<pair<K, V>>::iterator itr = theList[i].begin(); itr != theList[i].end(); ++itr) {
      if(itr == theList[i].begin()) {
        cout << itr->first << ' ' << itr->second;
      }
      else {
        cout << ":" << itr->first << ' ' << itr->second;
      }
    }

    cout << endl;
  }
}

/*return the number of elements in the hash table.*/
template <typename K, typename V>
size_t HashTable<K,V>::size() {
  return currentSize;
}

/*write all elements in the hash table into a file with name filename.
  Similar to the file format in the load function, each line contains a
  pair of key-value pair, separated by a white space.*/
template <typename K, typename V>
bool HashTable<K,V>::write_to_file(const char *filename) {
  ofstream os;
  os.open(filename);
  if(os.is_open()) {
    for(int i = 0; i < theList.size(); i++) {
      for(typename list<pair<K, V>>::iterator itr = theList[i].begin(); itr != theList[i].end(); ++itr) {
        os << itr->first << " " << itr->second << endl;
      }
    }
  }
  else {
    return false;
  }
  os.close();
  return true;
}

/*delete all elements in the hash table. The public interface clear()
  will call this function.*/
template <typename K, typename V>
void HashTable<K,V>::makeEmpty() {
  currentSize = 0;

  for (auto & thisList : theList) {
		thisList.clear();
	}
}

/*Called when the number of elements in the hash table
  is greater than the size of the vector.*/
template <typename K, typename V>
void HashTable<K,V>::rehash() {
  vector<list<pair<K, V>>> oldList = theList;

	// create new double-sized, empty table
	theList.resize(prime_below(2 * theList.size()));
	for (auto & thisList: theList)
		thisList.clear();

	// copy table over
	currentSize = 0;
	for (auto & thisList : oldList)
		for (auto & x : thisList)
			insert(move(x));

}

/*return the index of the vector entry where k should be stored.*/
template <typename K, typename V>
size_t HashTable<K,V>::myhash(const K &k) const {
  static hash<K> hf;

	return hf(k) % theList.size();
}

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	     return n;
      --n;
    }

  return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n)
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}
