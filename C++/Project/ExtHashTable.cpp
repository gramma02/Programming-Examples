#include "ExtHashTable.hpp"
#include <stdio.h>
using namespace std;

// Constructor
ExtHashTable::ExtHashTable(double upper_bound_ratio,double lower_bound_ratio,int capacity)
{
    // Father Constructor called here
    // init new things too
    this->upper_bound_ratio = upper_bound_ratio;
    this->lower_bound_ratio = lower_bound_ratio;
}


// Copy Constructor omly worked with initializer list
ExtHashTable::ExtHashTable(const ExtHashTable &t)
    : HashTable(t), 
    upper_bound_ratio(t.upper_bound_ratio), 
    lower_bound_ratio(t.lower_bound_ratio) {}


// rehash
void ExtHashTable::rehash()
{
    if(size == 0)
        return;

    double ratio = (double)this->size / (double)this->capacity;
    int new_cap = 0;
    int old_cap = this->capacity;
    if(ratio >= this->upper_bound_ratio)
        new_cap = this->capacity*2; //double size
    else if(ratio <= lower_bound_ratio)
        new_cap = this->capacity/2; //half the size
    else //no change      new_cap == 0
        return; //so return and change nothing

    // if we here change is needed  
    string **old_table = this->table;       //keep the old table 
    this->table = new string*[new_cap];     //and create new table
    for(int i=0; i<new_cap; i++)            //on the existing HashTable
        this->table[i] = nullptr;           //and init with null
    
    // iteratre the old table and if its a string add it to the new
    this->size = 0; //reset size it and it will be updated by add
    this->capacity = new_cap; //update capacity here cause add will be called
    for(int i=0; i<old_cap; i++)
    {
        if((old_table[i] != nullptr) && (old_table[i] !=TOMBSTONE))
        {
            //call old add to avoid stack overflow
            this->HashTable::add(*(old_table[i])); //call old add
            delete old_table[i]; //delete the string from old
        }
    }
    delete[] old_table; //kill the table    

    // changes happened
    printf("--> Size: %d, New capacity: %d\n", this->size, this->capacity);

}

// add 1
bool ExtHashTable::add(const string &s) 
{
    // old add wont make a full circle so we safe
    bool added = this->HashTable::add(s); 
    if(added)                       
        this->rehash(); //call rehash
    return added;
}

// add 2
bool ExtHashTable::add(const char *s)
{
    // old add wont make a full circle so we safe
    bool added = this->HashTable::add(s); 
    if(added)                       
        this->rehash(); //call rehash
    return added;
}

// remove 1
bool ExtHashTable::remove(const string &str)
{
    bool removed = this->HashTable::remove(str);
    if(removed)
        this->rehash();
    return removed;
}

// remove 2
bool ExtHashTable::remove(const char *s)
{
    bool removed = this->HashTable::remove(s);
    if(removed)
        this->rehash();
    return removed;
}

// = (same with parent's)
ExtHashTable& ExtHashTable::operator = (const ExtHashTable &ht)
{
    // check for fraud
    if(this == &ht) 
        return *this;

    // Destructor
    this->~ExtHashTable(); 

    // Copy Constructor code
    this->capacity = ht.capacity;
    this->size = ht.size;
    this->table = new string*[ht.capacity]; //killed by Destructor

    for(int i=0; i<ht.capacity; i++) 
    {
        if(ht.isEmpty(i))
            this->table[i] = nullptr;
        else if(ht.isTomb(i))
            this->table[i] = TOMBSTONE;
        else //its a string so copy it
            this->table[i] = new string(*(ht.table[i]));
    }

    return *this;
}

// =+  :  ht=+ string   :   ht.operator=+(string)
ExtHashTable& ExtHashTable::operator += (const string &str)
{
    this->add(str);
    return *this;
}

ExtHashTable& ExtHashTable::operator += (const char* s)
{
    this->add(s);
    return *this;
}

// =-
ExtHashTable& ExtHashTable::operator -= (const string &str)
{
    this->remove(str);
    return *this;
}

ExtHashTable& ExtHashTable::operator -= (const char *s)
{
    this->remove(s);
    return *this;
}

// + 
ExtHashTable ExtHashTable::operator + (const string& str) const
{

    //Copy Constructor
    ExtHashTable new_ht((*this));
    new_ht += str; 
    return new_ht;
}
   
ExtHashTable ExtHashTable::operator + (const char* s) const
{
    //Copy Constructor
    ExtHashTable new_ht((*this));
    new_ht += s; 
    return new_ht;
}

// - 
ExtHashTable ExtHashTable::operator - (const string& str) const
{
    //Copy Constructor
    ExtHashTable new_ht((*this));
    new_ht -= str; 
    return new_ht;
}
   
ExtHashTable ExtHashTable::operator - (const char* s) const
{
    //Copy Constructor
    ExtHashTable new_ht((*this));
    new_ht -= s; 
    return new_ht;
}


// + between ExtHashTable
ExtHashTable ExtHashTable::operator+(const ExtHashTable &ht) const
{
    ExtHashTable new_ht((*this));       // make a new one

    for(int i=0; i<ht.capacity; i++)    //iterate the old
        if(!ht.isAvailable(i))          //if string 
            new_ht.add(*(ht.table[i])); //add it to the new

    return new_ht;
}

// += between ExtHashTable
ExtHashTable& ExtHashTable::operator+=(const ExtHashTable &ht)
{
    for(int i=0; i<ht.capacity; i++)    //iterate the right
        if(!ht.isAvailable(i))          //if string 
            this->add(*(ht.table[i]));  //add it to the left

    return *this;
}