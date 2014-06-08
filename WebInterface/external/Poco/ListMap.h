//
// ListMap.h
//
// $Id: //poco/1.4/Foundation/include/Poco/ListMap.h#1 $
//
// Library: Foundation
// Package: Hashing
// Module:  ListMap
//
// Definition of the ListMap class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_ListMap_INCLUDED
#define Foundation_ListMap_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/String.h"
#include "Poco/Exception.h"
#include <list>
#include <utility>


namespace Poco {


template <class Key, class Mapped, class Container = std::list<std::pair<Key, Mapped> >, bool CaseSensitive = false >
class ListMap
	/// This class implements a multimap in terms of a sequential container.
	/// The use for this type of associative container is wherever automatic
	/// ordering of elements is not desirable. Naturally, this container will
	/// have inferior data retrieval performance and it is not recommended for
	/// use with large datasets. The main purpose within POCO is for Internet
	/// messages (email message, http headers etc), to prevent autmomatic 
	/// header entry reordering.
{
public:
	typedef Key                 KeyType;
	typedef Mapped              MappedType;
	typedef Mapped&             Reference;
	typedef const Mapped&       ConstReference;
	typedef Mapped*             Pointer;
	typedef const Mapped*       ConstPointer;
	
	typedef typename Container::value_type     ValueType;
	typedef typename Container::size_type      SizeType;
	typedef typename Container::iterator       Iterator;
	typedef typename Container::const_iterator ConstIterator;
	
	ListMap()
		/// Creates an empty ListMap.
	{
	}
	
	ListMap(std::size_t initialReserve):
		_list(initialReserve)
		/// Creates the ListMap with room for initialReserve entries.
	{
	}
	
	ListMap& operator = (const ListMap& map)
		/// Assigns another ListMap.
	{
		ListMap tmp(map);
		swap(tmp);
		return *this;
	}
	
	void swap(ListMap& map)
		/// Swaps the ListMap with another one.
	{
		_list.swap(map._list);
	}
	
	ConstIterator begin() const
		/// Returns the beginning of the map.
	{
		return _list.begin();
	}
	
	ConstIterator end() const
		/// Returns the end of the map.
	{
		return _list.end();
	}
	
	Iterator begin()
		/// Returns the beginning of the map.
	{
		return _list.begin();
	}
	
	Iterator end()
		/// Returns the end of the map.
	{
		return _list.end();
	}
	
	ConstIterator find(const KeyType& key) const
		/// Finds the first occurence of the key and
		/// returns iterator pointing to the found entry
		/// or iterator pointing to the end if entry is
		/// not found.
	{
		typename Container::const_iterator it = _list.begin();
		typename Container::const_iterator end = _list.end();
		for(; it != end; ++it)
		{
			if (isEqual(it->first, key)) return it;
		}
		return end;
	}

	Iterator find(const KeyType& key)
		/// Finds the first occurence of the key and
		/// returns iterator pointing to the found entry
		/// or iterator pointing to the end if entry is
		/// not found.
	{
		typename Container::iterator it = _list.begin();
		typename Container::iterator end = _list.end();
		for(; it != end; ++it)
		{
			if (isEqual(it->first, key)) return it;
		}
		return end;
	}

	Iterator insert(const ValueType& val)
		/// Inserts the value into the map. If one or more values 
		/// already exist, new value is inserted at the end of the 
		/// block. Thus, all the equal value entries are located
		/// sequentially at all times.
		/// Returns iterator pointing to the newly inserted value 
	{
		Iterator it = find(val.first);

		if (it == _list.end())
		{
			_list.push_back(val);
			it = _list.end();
			--it;
		}
		else
		{
			_list.insert(it, 1, val);
		}

		return it;
	}
	
	void erase(Iterator it)
	{
		_list.erase(it);
	}
	
	SizeType erase(const KeyType& key)
	{
		SizeType count = 0;
		Iterator it = find(key);
		bool removed = false;
		while (it != _list.end())
		{
			if (isEqual(it->first, key))
			{
				++count;
				it = _list.erase(it);
				removed = true;
			}
			else
			{
				if (removed) return count;
				++it;
			}
		}
		return count;
	}
	
	void clear()
	{
		_list.clear();
	}

	std::size_t size() const
	{
		return _list.size();
	}

	bool empty() const
	{
		return _list.empty();
	}

	ConstReference operator [] (const KeyType& key) const
	{
		ConstIterator it = find(key);
		if (it != _list.end())
			return it->second;
		else
			throw NotFoundException();
	}

	Reference operator [] (const KeyType& key)
	{
		Iterator it = find(key);
		if (it != _list.end())
			return it->second;
		else
		{
			ValueType value(key, Mapped());
			Iterator it = insert(value);
			return it->second;
		}
	}

private:
	template <typename T1, typename T2>
	bool isEqual(T1 val1, T2 val2) const
	{
		return val1 == val2;
	}

	bool isEqual(const std::string& s1, const std::string& s2) const
	{
		if (!CaseSensitive)
			return Poco::icompare(s1, s2) == 0;
		else
			return s1 == s2;
	}

	bool isEqual(const std::string& s1, const char* s2) const
	{
		return isEqual(s1, std::string(s2));
	}

	bool isEqual(const char* s1, const std::string& s2) const
	{
		return isEqual(std::string(s1), s2);
	}

	bool isEqual(const char* s1, const char* s2) const
	{
		return isEqual(std::string(s1), std::string(s2));
	}

	Container _list;
};


} // namespace Poco


#endif // Foundation_ListMap_INCLUDED
