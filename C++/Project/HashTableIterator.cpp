#include "HashTable.hpp"

// ITERATOR
// when the table is wasted the curr is set to null and pos out_of _bounds

// Constructor
HashTable::Iterator::Iterator(const HashTable *t)
{
    // No Copy Constructor is called here
    ht = t; //The pointers just point on the same object

    int index = 0;
    while(ht->isAvailable(index)) //iterate till you find a string
    {  
        index++; //written before the check cause cap is +1 than last pos
        if(index == ht->capacity) //table wasted
            break;        
    }

    if(index == ht->capacity)
        curr = nullptr; //curr is null cause table is empty
    else
        curr = &(ht->table[index]); //point curr to the string
    
    position = index; //either valid or not: keep the position 

}

// Copy Constructor
HashTable::Iterator::Iterator(const Iterator &it)
{
    // Nothing in the heap so just make them equal
    this->curr = it.curr;
    this->ht = it.ht;
    this->position = it.position;
}

// The Default Destructor will do the job here cause there
// is no memory to free 

// =
HashTable::Iterator& HashTable::Iterator::operator=(const Iterator &it)
{
    // just Like A Copy Constructor but with a return
    this->curr = it.curr;
    this->ht = it.ht;
    this->position = it.position;

    return *this;
}

// ++ (prefix)
HashTable::Iterator HashTable::Iterator::operator++()
{
    int index = position + 1; //start from the next one
    while(ht->isAvailable(index))
    {
        index++; 
        if(index == ht->capacity) //table wasted
            break;   
    }

    if(index == ht->capacity)
        curr = nullptr; //curr is null
    else
        curr = &(ht->table[index]); //point curr to the string
    
    this->position = index; //either valid or not: keep the position 
    return *this;
}

// ++ (sufix)
HashTable::Iterator HashTable::Iterator::operator++(int a)
{
    Iterator old(*this); //keep the old one
    HashTable::Iterator::operator++(); //move it
    return old; //return the old one

}

// == 
bool HashTable::Iterator::operator==(const Iterator &it) const
{
    // ht and curr are poiters to the same object so we just need to 
    // check if they point to the same address and not the internals
    if((ht == it.ht) && (position == it.position) && curr == it.curr)
        return true;
    else
        return false;

}

// !=
bool HashTable::Iterator::operator!=(const Iterator &it) const
{
    return !operator==(it);
}

// *
const string& HashTable::Iterator::operator*(){return **curr;}

// ->
const string* HashTable::Iterator::operator->(){return *curr;}

int HashTable::Iterator::pos() const{return position;}

// my method
HashTable::Iterator HashTable::Iterator::set_to_end(int cap)
{
    this->curr = nullptr;
    this->position = cap;
    return *this;
}
