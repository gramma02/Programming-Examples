
#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string>
#include "HashTableException.hpp"
using namespace std;
#define TOMBSTONE (string*)(0xFFFF)


class HashTable 
{
    protected:
    //string* const TOMBSTONE = (string*)(0xFFFF);    // thanos: #define TOMBSTONE (string*)(0xFFFF)
    int size; //number of strings in the table
    int capacity;
    string **table;
    

    static unsigned long getHashCode(const char *str);

    bool isEmpty(int pos) const;
    bool isTomb(int pos) const;
    bool isAvailable(int pos) const;

public:
    class Iterator; // for the compiler
    
    HashTable(int capacity=8);
    HashTable(const HashTable &ht);
    ~HashTable();

    int getSize() const;
    int getCapacity() const;

    bool contains(const string &s) const;
    bool contains(const char *s) const;
    string print() const;

    virtual bool add(const string &s);
    virtual bool add(const char *s);
    virtual bool remove(const string &s);
    virtual bool remove(const string *s);

    HashTable& operator = (const HashTable &ht);
  
    HashTable& operator += (const string& str);
    HashTable& operator += (const char* s);
    HashTable& operator -= (const string& str);
    HashTable& operator -= (const char* s);
    
    HashTable operator + (const string& str) const;
    HashTable operator + (const char* s) const;
    HashTable operator - (const string& str) const;
    HashTable operator - (const char* s) const;

    friend std::ostream& operator<<(std::ostream &stream, const HashTable &ht);

    Iterator begin() const;
    Iterator end() const;


};

class HashTable::Iterator 
{
    string **curr;
    const HashTable *ht;
    int position;  // Θέση του δείκτη curr στον πίνακα. 
                   // Ξεκινάμε την αρίθμηση από το 0.
    
  public:    
    Iterator(const HashTable *t);
    Iterator(const Iterator &it);
    Iterator& operator=(const Iterator &it);
    Iterator operator++();
    Iterator operator++(int a);
    bool operator==(const Iterator &it) const ;
    bool operator!=(const Iterator &it) const;
    const string& operator*();
    const string* operator->();
    int pos() const;
    Iterator set_to_end(int cap); //mine
};

#endif