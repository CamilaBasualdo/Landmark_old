#pragma once
#include <vector>
#include <set>
#include <limits>
#include <iostream>
/**
 * \brief Vector that acts as memory manager, on insert objects stay in place relative to beginning
 * of vector.
 * Erasing Marks a space as empty, ready to be overwritten
 * \tparam T Storage Type
 * \tparam PosType Data Type used for indexing, unsigned long long is default
 * \tparam NullMemory Set Memory to NULL on erase
 */
template <typename T,typename PosType = size_t,bool NullMemory = false>
class AllocVector
{

	std::vector<T> _DATA;
	std::set<PosType> _Markers;
	const size_t MaxSize = std::numeric_limits<PosType>().max();
	friend class iterator;

	//using AllocType = AllocVector<T, PosType, NullMemory>;
public:
	class iterator
	{

		
		//position on vector
		PosType DataIndex;
		//Owner
		AllocVector* const Owner;

		//number of existing elements to shift by
		void ShiftBy(int shift)
		{
			std::vector<T>& DataRef = Owner->_DATA;
			std::set<PosType>& MarkerRef = Owner->_Markers;

			DataIndex += shift;
			const short Direction = shift > 0 ? 1 : -1;
			while (true)
			{

				if (!MarkerRef.contains(DataIndex)) break;
				else DataIndex += Direction;
			}
		}
	public:
		iterator(AllocVector* _Owner, PosType _Index) : Owner(_Owner)
		{
			DataIndex = _Index;
			//MarkerBehind = Owner->_Markers.lower_bound(DataIndex);
			///MarkerAhead = Owner->_Markers.upper_bound(DataIndex);
		}
		iterator() :Owner(nullptr), DataIndex(-1) {};
		template <typename O>
		iterator(O& other)
			:Owner(
				reinterpret_cast<AllocVector*>(
					static_cast<void*>(
						other.GetOwner())))
			, DataIndex(other.GetIndex()) {
		}
		//moves to the next data 
		iterator& operator++()
		{
			ShiftBy(1);
			return *this;
		}
		iterator& operator+=(PosType shift)
		{
			ShiftBy(shift);
			return *this;
		}
		iterator& operator--()
		{
			ShiftBy(-1);
			return *this;
		}

		iterator& operator-=(PosType shift)
		{
			ShiftBy(-shift);
			return *this;
		}
		bool operator!=(const iterator& other) const
		{
			return DataIndex != other.GetIndex();
		}
		bool operator==(const iterator& other) const
		{
			return !(operator!=(other));
		}
		PosType GetIndex() const { return DataIndex; }
		AllocVector* GetOwner() const { return Owner; }
		T& operator*()
		{
			return Owner->at(DataIndex);
		}


	};



private:
	iterator GetPushLocation()
	{
		if (_Markers.empty())
		{
			if (_DATA.size() >= MaxSize)
			{
				throw std::range_error("Max Capacity Reached");
			}

			return end();
		}
		else
		{
			//get first empty location
			auto InsertLocation = _Markers.begin();
			auto index = *InsertLocation;
			//insert it there

			return iterator(this, index);
		}
	}
public:
	AllocVector() {};
	AllocVector(std::initializer_list<T> Entries)
	{
		for (auto& entry : Entries)
		{
			Push(entry);
		}
	}
	/**
	 * \brief Initialize entries with specified positions
	 * \param Entries 
	 */
	AllocVector(std::initializer_list<std::pair<PosType,T>> Entries)
	{
		
		for (auto& entry : Entries)
			Insert(entry.first, entry.second);
	}

	/**
	 * \brief Initialize with N count of T
	 */
	AllocVector(PosType N,T t) : _DATA(N,t)
	{
		
	}

	iterator begin() 
	{
		auto It = _DATA.begin();
		PosType Index = 0;
		auto begin = iterator(this, Index);
		if (!_Markers.contains(Index)) return begin;
		else return ++begin;
		
		
	}
	iterator end() 
	{
		return iterator(this,_DATA.size());
	}

	bool Exists(iterator it) const
	{
		return Exists(it.GetIndex());
	}
	bool Exists(PosType Index) const
	{
		//if within confines of vector
		if (Index >=0 && Index < _DATA.size())
		{
			//if index is not marked as empty
			if (!_Markers.contains(Index)) return true;
		}
		return false;
	}
	//size of underlying vector
	PosType Size_Vector() const { return _DATA.size(); }
	//number of active elements. (size of vector minus number of free spots)
	PosType Count() const { return _DATA.size() - _Markers.size();  }
	T& at(iterator Index)
	{
		return at[Index.GetIndex()];
	}
	T& at(PosType Index)
	{
		return _DATA[Index];
	}
	T& operator[](iterator it)
	{
		return operator[](it.GetIndex());
	}
	T& operator[](PosType Index)
	{
		return _DATA[Index];
	}
	iterator Push(const T& val)
	{
		const iterator loc = GetPushLocation();
		if (loc == end())
		{
			_DATA.push_back(val);
			return iterator(this, _DATA.size() - 1);
		}
		else
		{
			memcpy(&_DATA[loc.GetIndex()], &val, sizeof(T));
			_Markers.erase(loc.GetIndex());
			return loc;
		}
		
	}
	template<typename... Args>
	iterator Push_Emplace(Args&&... args)
	{
		const iterator loc = GetPushLocation();
		if (loc == end())
		{
			_DATA.emplace_back(std::forward<Args>(args)...);
			return iterator(this, _DATA.size() - 1);
		}
		else
		{
			if (std::is_constructible_v<T,Args>)
			{
				
			}
			T* Target = &_DATA[loc.GetIndex()];
			Target->T(std::forward<Args>(args)...);
			_Markers.erase(loc.GetIndex());
			return loc;
		}
	}

	void Insert(const iterator where, T& val)
	{
		Insert(where.GetIndex(), val);
	}
	void Insert(const PosType where, T& val)
	{
		if (Exists(where)) Erase(where);
		if (where.GetIndex() > _DATA.capacity())_DATA.reserve(where.GetIndex());
		memcpy(&_DATA[where.GetIndex()], &val, sizeof(T));
	}
	template<typename... Args>
	void Insert_Emplace(const iterator where, Args&&... args)
	{

		if (Exists(where)) Erase(where);
		if (where.GetIndex() > _DATA.capacity())_DATA.reserve(where.GetIndex());
		(&_DATA[where.GetIndex()])->T(args...);
	}
	PosType Capacity() const { return MaxSize; }
	void Erase(const iterator where)
	{
		Erase(where.GetIndex());
	}
	void Erase(const PosType where)
	{
		if (_Markers.contains(where))return;

		if (std::is_destructible<T>::value)
			_DATA[where].~T();
		if (NullMemory)
		{
			memset(&_DATA[where], NULL, sizeof(T));
			//dothecommandthatsetsNULLS();
		}

		//if every slot in vector is available
		if (_DATA.size() == _Markers.size()+1)
		{
			_DATA.clear();
			_Markers.clear();
		}
		else _Markers.emplace(where);
		
	}
	

};


/**
 * \brief AllocVector uint32 tracker, 4,294,967,296 max entries
 * \tparam T 
 */
template<typename T, bool NullMemory = false >
using AllocVector_Medium = AllocVector<T,  uint32_t, NullMemory> ;


/**
 * \brief AllocVector uint16 tracker, 65,536 max entries
 * \tparam T 
 */
template<typename T, bool NullMemory = false>
using  AllocVector_Small = AllocVector<T, uint16_t, NullMemory>;

/**
 * \brief AllocVector uint8 tracker, 256 max entries
 * \tparam T
 */
template<typename T, bool NullMemory = false>
using AllocVector_Tiny = AllocVector<T, uint8_t,NullMemory>;