/***********************************************************************
 * Header:
 *    map
 * Summary:
 *    Our custom implementation of a std::map
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 *        map::iterator       : An iterator through a map
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
template <class K, class V>
class map
{
   friend ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend void swap(map<KK, VV>& lhs, map<KK, VV>& rhs);
public:
   using Pairs = custom::pair<K, V>;

   // 
   // Construct
   //
   map() : bst()
   {
   }
   map(const map &  rhs) 
   {
       this->bst = rhs.bst;
   }
   map(map && rhs) :bst(std::move(rhs.bst))
   {
       
   }
   template <class Iterator>
   map(Iterator first, Iterator last) 
   {
       auto it = first;
       while(it != last)
       {
           bst.insert(*it);
           it++;
       }
   }
   map(const std::initializer_list <Pairs>& il) 
   {
       bst.clear();
       for (auto & element : il)
          bst.insert(element);
   }
  ~map()         
   {
      clear();
   }

   //
   // Assign
   //
   map & operator = (const map & rhs) 
   {
       this->bst = rhs.bst;
      return *this;
   }
   map & operator = (map && rhs)
   {
       this->bst = std::move(rhs.bst);
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
       clear();
       for(auto element :il)
           bst.insert(element);
      return *this;
   }
   
   // 
   // Iterator
   //
   class iterator;
   iterator begin() 
   { 
      return iterator(bst.begin());
   }
   iterator end() 
   { 
      return iterator();    
   }

   // 
   // Access
   //
   const V & operator [] (const K & k) const;
         V & operator [] (const K & k);
   const V & at (const K& k) const;
         V & at (const K& k);
   iterator    find(const K & k)
   {
      return iterator(bst.find(Pairs(k, V())));
   }

   //
   // Insert
   //
   custom::pair<typename map::iterator, bool> insert(Pairs && rhs)
   {
       auto pair = bst.insert(rhs);
       
      return make_pair(iterator(pair.first), pair.second);
   }
   custom::pair<typename map::iterator, bool> insert(const Pairs & rhs)
   {
       auto pair = bst.insert(rhs);
       return make_pair(iterator(pair.first), pair.second);
//      return make_pair(iterator(), false);
   }

   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       
   }
   void insert(const std::initializer_list <Pairs>& il)
   {

   }

   //
   // Remove
   //
   void clear() noexcept { bst.clear(); }
   size_t erase(const K& k);
   iterator erase(iterator it);
   iterator erase(iterator first, iterator last);

   //
   // Status
   //
   bool empty() const noexcept
    {
        return bst.numElements == 0;
       
   }
   size_t size() const noexcept { return bst.numElements; }


private:

   // the students DO NOT need to use a nested class
   BST < pair <K, V >> bst;
};


/**********************************************************
 * MAP ITERATOR
 * Forward and reverse iterator through a Map, just call
 * through to BSTIterator
 *********************************************************/
template <typename K, typename V>
class map <K, V> :: iterator
{
   friend class ::TestMap; // give unit tests access to the privates
   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //
   iterator()
   {
       
   }
   iterator(const typename BST < pair <K, V> > :: iterator & rhs)
   {
       this->it = rhs;
   }
   iterator(const iterator & rhs) 
   {
       this->it = rhs.it;
   }

   //
   // Assign
   //
   iterator & operator = (const iterator & rhs)
   {
       this->it = rhs.it;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator & rhs) const { return false; }
   bool operator != (const iterator & rhs) const { return false; }

   // 
   // Access
   //
   const pair <K, V> & operator * () const
   {
      return *it;
   }

   //
   // Increment
   //
   iterator & operator ++ ()
   {
       
       it++;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      return *this;
   }
   iterator & operator -- ()
   {
      return *this;
   }
   iterator  operator -- (int postfix)
   {
      return *this;
   }

private:

   // Member variable
   typename BST < pair <K, V >>  :: iterator it;   
};


/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> :: operator [] (const K& key)
{
   auto pair = Pairs(key, V());
   auto returnValue = bst.insert(pair, true);
   auto it = returnValue.first;
   return it.pNode->data.second;
}

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> :: operator [] (const K& key) const
{
   auto pair = Pairs(key, V());
   auto it = bst.find(pair);
   if (it != bst.end())
      return it.pNode->data.second;
   else
      return pair.second;
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
V& map <K, V> ::at(const K& key)
{
   auto pair = Pairs(key, V());
   auto it = bst.find(pair);
   if (it != bst.end())
      return it.pNode->data.second;
   else
      throw std::out_of_range("invalid map<K, T> key");
}

/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
template <typename K, typename V>
const V& map <K, V> ::at(const K& key) const
{
   auto pair = Pairs(key, V());
   auto it = bst.find(pair);
   if (it != bst.end())
      return it.pNode->data.second;
   else
      throw std::out_of_range("invalid map<K, T> key");
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
template <typename K, typename V>
void swap(map <K, V>& lhs, map <K, V>& rhs)
{
    std::swap(lhs.bst, rhs.bst);
//    std::swap(lhs.numElements, rhs.numElements);
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
size_t map<K, V>::erase(const K& k)
{
    auto pair = Pairs(k, V());
    auto it = find(k);
    if (it == end())
        return 0;
    erase(it);
    return 1;
}

/*****************************************************
 * ERASE
 * Erase several elements
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator first, map<K, V>::iterator last)
{
   return iterator();
}

/*****************************************************
 * ERASE
 * Erase one element
 ****************************************************/
template <typename K, typename V>
typename map<K, V>::iterator map<K, V>::erase(map<K, V>::iterator it)
{
   return iterator();
}

}; //  namespace custom

