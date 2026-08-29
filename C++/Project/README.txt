C++ string hash table.

HashTable stores unique strings with open addressing and tombstones, plus an iterator and +/− operators. ExtHashTable inherits it and automatically doubles or halves capacity when the load factor crosses the given bounds.
