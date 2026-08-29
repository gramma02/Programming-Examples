#include "HashTable.hpp"
#include <string.h>
#include <cstring>
#include <stdexcept> 


unsigned long HashTable::getHashCode(const char *str) 
{
    unsigned long hash = 97;
    int c;

    while ((c = *(str++)) != '\0')
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

bool HashTable::isEmpty(int pos) const 
{
    if(pos >= capacity)
        return false;

    return table[pos] == nullptr;
}

bool HashTable::isTomb(int pos) const 
{
    if(pos >= capacity)
        return false;

    return table[pos] == TOMBSTONE;
}

bool HashTable::isAvailable(int pos) const 
{
    return isEmpty(pos) || isTomb(pos);
}

// Contructor
HashTable::HashTable(int capacity)
{
    // maybe a check for dump capacity needed
    this->capacity = capacity;
    this->size = 0;
    table = new string*[capacity]; //make space

    for(int i=0; i<capacity; i++) //init with null
        table[i] = nullptr;
}

// Copy constructor
HashTable::HashTable(const HashTable &ht) 
{
    this->capacity = ht.capacity;
    this->size = ht.size;
    this->table = new string*[ht.capacity];

    for(int i=0; i<ht.capacity; i++) 
    {
        if(ht.isEmpty(i))
            this->table[i] = nullptr;
        else if(ht.isTomb(i))
            this->table[i] = TOMBSTONE;
        else //its a string so copy it
            this->table[i] = new string(*(ht.table[i]));
    }
}

// Destructor
HashTable::~HashTable() 
{
    for(int i=0; i<capacity; i++) //kill the strings
        if(!isAvailable(i)) //if there are
            delete table[i];
            
    delete[] table; //kill the table
}

int HashTable::getSize() const {return size;}
int HashTable::getCapacity() const {return capacity;}

// Contains 1
bool HashTable::contains(const char *s) const 
{
    int initIndex = getHashCode(s) % capacity;
    int index = initIndex;
    int i = -1;

    // search for it
    while(!isEmpty(index)) // stop when null
    {
        if(++i != 0) // not going in first time
            if(index == initIndex) // every other time check
                return false; // full circle

        if(!isTomb(index)) 
            if(*table[index] == s) // correct comparison for string
                return true; // found

        // else if tomb or other word iterate
        index = (index + 1) % capacity;
    }
    // if we are here, it means we bumped onto a null
    return false;

}

// Contains 2
bool HashTable::contains(const string &s) const 
{
    int initIndex = getHashCode(s.c_str()) % capacity;
    int index = initIndex;
    int i = -1;

    // search for it
    while(!isEmpty(index)) // stop when null
    {
        if(++i != 0) // not going in first time
            if(index == initIndex) // every other time check
                return false; // full circle

        if(!isTomb(index)) 
            if(*table[index] == s) // correct comparison for string
                return true; // found

        // else if tomb or other word iterate
        index = (index + 1) % capacity;
    }
    // if we are here, it means we bumped onto a null
    return false;

}

//thanos
string HashTable::print() const 
{
    string str;
    char buf[128];
  
    for(int i=0; i<capacity; i++) 
    {
        if( !isAvailable(i) ) 
        {
            sprintf(buf, "%2d. -%s-\n", i, (*table[i]).c_str());
            str.append(buf);
        }
    }

    sprintf(buf, " --- CAPACITY: %d, SIZE: %d ---\n", capacity, size);
    str.append(buf);
    return str;
}

// add 1
bool HashTable::add(const string &s) 
{
    int initIndex = getHashCode(s.c_str()) % capacity;
    int index = initIndex;
    int i = -1;

    if(s == "")
        return false;

    // search for it
    while(!isEmpty(index)) // stop when null
    {
        if(++i != 0) //not going in first time
            if(index == initIndex) //every other time check
                throw HashTableException(); //full circle

        if(!isTomb(index)) 
            if(*table[index] == s)
                return false; // found

        //else if tomb or other word iterrate
        index = (index+1) % capacity;
    }
    // if we here means we bumped onto o null
    index = initIndex; //index back to start
    while(!isAvailable(index)) //iterate till you find space
        index = (index+1) % capacity;

    // spot found
    table[index] = new string(s); //create string
    this->size++; // Increment the size
    return true;
}

// add 2
bool HashTable::add(const char *str)
{
    int initIndex = getHashCode(str) % capacity;
    string s = str;
    int index = initIndex;
    int i = -1;

    if(s == "")
        return false;
 
    //search for it
    while(!isEmpty(index)) //stop when null
    {
        if(++i != 0) //not going in first time
            if(index == initIndex) //every other time check
                throw HashTableException(); //full circle

        if(!isTomb(index)) 
            if(*table[index] == s)
                return false; //found

        // else if tomb or other word, iterate
        index = (index + 1) % capacity;
    }

    // if we are here, it means we bumped onto a null
    index = initIndex; // index back to start
    while(!isAvailable(index)) //iterate till you find space
        index = (index + 1) % capacity;

    // spot found
    table[index] = new string(s); //create string
    this->size++; // Increment the size
    return true;
}

// Remove 1
bool HashTable::remove(const string &s)
{
    int initIndex = getHashCode(s.c_str()) % capacity;
    int index = initIndex;
    int i = -1;

    // search for it
    while(!isEmpty(index)) //stop when null
    {
        if(++i != 0) //not going in first time
            if(index == initIndex) //every other time check
                throw HashTableException(); //full circle

        if(!isTomb(index)) 
            if(*table[index] == s)
                break; // found

        // else if tomb or other word iterrate
        index = (index+1) % capacity;
    }

    // If empty spot found. String not there
    if(isEmpty(index))
        return false;
    else // while terminated cause string was found
    {
        delete table[index]; //remove the string
        table[index] = TOMBSTONE; //seal the grave
        this->size--; //reduce the counted strings
        return true;
    }
}

// Remove 2
bool HashTable::remove(const string *s)
{
    int initIndex = getHashCode(s->c_str()) % capacity;
    int index = initIndex;
    int i = -1;

    // search for it
    while(!isEmpty(index)) // stop when null
    {
        if(++i != 0) // not going in first time
            if(index == initIndex) // every other time check
                throw HashTableException(); // full circle

        if(!isTomb(index)) 
            if(*table[index] == *s)
                break; // found

        // else if tomb or other word, iterate
        index = (index + 1) % capacity;
    }

    // If empty spot found, string not there
    if(isEmpty(index))
        return false;
    else // While terminated because string was found
    {
        delete table[index]; // remove the string
        table[index] = TOMBSTONE; // seal the grave
        this->size--; // reduce the counted strings
        return true;
    }
}

// ht1 = ht2 : ht1.operator=(ht2)
// this way it does make a copy 
// they dont point to the same memory
HashTable& HashTable::operator = (const HashTable &ht)
{
    // check for fraud
    if(this == &ht) 
        return *this;

    // Destructor
    this->~HashTable(); 

    // Copy Constructor
    //this = new HashTable(ht); 
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
HashTable& HashTable::operator += (const string &str)
{
    this->add(str);
    return *this;
}

HashTable& HashTable::operator += (const char* s)
{
    this->add(s);
    return *this;
}

// =-
HashTable& HashTable::operator -= (const string &str)
{
    this->remove(str);
    return *this;
}

HashTable& HashTable::operator -= (const char *s)
{
    this->remove(s);
    return *this;
}


// + 
HashTable HashTable::operator + (const string& str) const
{
    //Copy Constructor
    HashTable new_ht((*this));
    new_ht += str; 
    return new_ht;
}
   

HashTable HashTable::operator + (const char* s) const
{
    //Copy Constructor
    HashTable new_ht((*this));
    new_ht += s; 
    return new_ht;
}

// -
HashTable HashTable::operator - (const string& str) const
{
    //Copy Constructor
    HashTable new_ht((*this));
    new_ht -= str; 
    return new_ht;
}

HashTable HashTable::operator - (const char* s) const
{
    //Copy Constructor
    HashTable new_ht((*this));
    new_ht -= s; 
    return new_ht;
}

// print
std::ostream& operator<<(std::ostream &stream, const HashTable &ht)
{
    return stream << ht.print();
}











// Iterator Methods
HashTable::Iterator HashTable::begin() const
{
    return Iterator(this);
}

HashTable::Iterator HashTable::end() const
{
    Iterator it(this);
    return it.set_to_end(this->capacity); //tricky part
}

