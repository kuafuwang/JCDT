/*
* Implementation of an LRU cache with a maximum size.
*
* See http://code.google.com/p/lru-cache-cpp/ for usage and limitations.
*
* Licensed under the GNU LGPL: http://www.gnu.org/copyleft/lesser.html
*
* Pierre-Luc Brunelle, 2011
* pierre-luc.brunelle@polytml.ca
*
* 使用stl中的map替换hash_table
* Peteryfren China, 2012
*
*
*
*/
#pragma once

#include <unordered_map>
#include <cassert>


namespace lru {

	//-------------------------------------------------------------
	// Bucket
	//-------------------------------------------------------------
	enum DIRECTION {
		MRU_TO_LRU = 0,
		LRU_TO_MRU
	};
	template<class K, class V>
	struct LRUValue
	{
		typedef std::pair<const K, LRUValue<K, V> > Val;

		LRUValue()
			: _v(), _older(NULL), _newer(NULL) { }

		LRUValue(const V & v, Val * older, Val * newer)
			: _v(v), _older(older), _newer(newer) { }

		V _v;
		Val * _older;
		Val * _newer;
	};


	//-------------------------------------------------------------
	// Const Iterator
	//-------------------------------------------------------------

	template<class K, class V>
	class LRUNoCacheConstIterator
	{
	public:
		
		typedef LRUNoCacheConstIterator<K, V> const_iterator;
		typedef std::pair<const K, LRUValue<K, V> > Val;
		typedef Val & reference;
		typedef Val * pointer;

		

		LRUNoCacheConstIterator(const Val * ptr = NULL, DIRECTION dir = MRU_TO_LRU);

		const_iterator & operator++();
		const_iterator operator++(int);
	
		bool operator==(const const_iterator & other);
		bool operator!=(const const_iterator & other);
		
		const K & key() const;
		const V & value() const;
	
		const Val* operator*();
		const Val* operator->();
		const Val * _ptr;
	protected:
		
		
		DIRECTION _dir;
	};


	template<class K, class V>
	LRUNoCacheConstIterator<K, V>::LRUNoCacheConstIterator(
		const typename LRUNoCacheConstIterator<K, V>::Val * ptr,
		DIRECTION dir)
		: _ptr(ptr), _dir(dir)
	{
	}


	template<class K, class V>
	LRUNoCacheConstIterator<K, V> & LRUNoCacheConstIterator<K, V>::operator++()
	{
		assert(_ptr);
		_ptr = (_dir == MRU_TO_LRU ? _ptr->second._older : _ptr->second._newer);
		return *this;
	}


	template<class K, class V>
	LRUNoCacheConstIterator<K, V> LRUNoCacheConstIterator<K, V>::operator++(int)
	{
		const_iterator ret = *this;
		++*this;
		return ret;
	}

	template <class K, class V>
	const typename LRUNoCacheConstIterator<K, V>::Val* LRUNoCacheConstIterator<K, V>::operator*()
	{
		return _ptr;
	}

	template <class K, class V>
	const typename LRUNoCacheConstIterator<K, V>::Val* LRUNoCacheConstIterator<K, V>::operator->()
	{
		return _ptr;
	}

	template<class K, class V>
	bool LRUNoCacheConstIterator<K, V>::operator==(const const_iterator & other)
	{
		return _ptr == other._ptr;
	}


	template<class K, class V>
	bool LRUNoCacheConstIterator<K, V>::operator!=(const const_iterator & other)
	{
		return _ptr != other._ptr;
	}

	

	template<class K, class V>
	const K & LRUNoCacheConstIterator<K, V>::key() const
	{
		assert(_ptr);
		return _ptr->first;
	}


	template<class K, class V>
	const V & LRUNoCacheConstIterator<K, V>::value() const
	{
		assert(_ptr);
		return _ptr->second._v;
	}


	//-------------------------------------------------------------
	//  Iterator
	//-------------------------------------------------------------

	template<class K, class V>
	class LRUNoCacheIterator:public LRUNoCacheConstIterator<K,V>
	{
	public:
		typedef LRUNoCacheConstIterator<K,V> _Mybase;
		typedef std::pair<const K, LRUValue<K, V> > Val;
		typedef LRUNoCacheIterator<K, V> iterator;
		typedef Val & reference;
		typedef Val * pointer;
		 K & key() ;
		 V & value() ;



		LRUNoCacheIterator(Val * ptr = NULL, DIRECTION dir = MRU_TO_LRU);
		
		iterator & operator++();
		iterator operator++(int);
		bool operator==(const iterator & other);
		bool operator!=(const iterator & other);

	};

	template <class K, class V>
	K& LRUNoCacheIterator<K, V>::key()
	{
		assert(_Mybase::_ptr);
		return _Mybase::_ptr->first;
	}



	template <class K, class V>
	V& LRUNoCacheIterator<K, V>::value()
	{
		assert(_Mybase::_ptr);
		return ((Val *)(_Mybase::_ptr))->second._v;
	}

	template <class K, class V>
	LRUNoCacheIterator<K, V>::LRUNoCacheIterator(Val* ptr, DIRECTION dir)
		: _Mybase(ptr, dir)
	{

	}

	template<class K, class V>
	LRUNoCacheIterator<K, V> & LRUNoCacheIterator<K, V>::operator++()
	{
		assert(_Mybase::_ptr);
		_Mybase::_ptr = (_Mybase::_dir == MRU_TO_LRU ? _Mybase::_ptr->second._older : _Mybase::_ptr->second._newer);
		return *this;
	}
	
	
	template<class K, class V>
	LRUNoCacheIterator<K, V> LRUNoCacheIterator<K, V>::operator++(int)
	{
		iterator ret = *this;
		++*this;
		return ret;
	}
	
	
	
	template<class K, class V>
	bool LRUNoCacheIterator<K, V>::operator==(const iterator & other)
	{
		return _Mybase::_ptr == other._ptr;
	}
	
	
	template<class K, class V>
	bool LRUNoCacheIterator<K, V>::operator!=(const iterator & other)
	{
		return _Mybase::_ptr != other._ptr;
	}
	
	
	
} // file scope


namespace lru {

	//-------------------------------------------------------------
	// LRU Cache
	//-------------------------------------------------------------

	
	template<class K, class V>
	class LRUNoCache
	{
	public:
		typedef LRUNoCacheIterator<K, V> iterator;
		typedef LRUNoCacheConstIterator<K, V> const_iterator;

		typedef std::pair<const K, LRUValue<K, V> > Val;

		typedef  std::unordered_map<K, LRUValue<K, V> > MAP_TYPE;
		typedef typename MAP_TYPE::size_type size_type;
		

		LRUNoCache(DIRECTION dir = MRU_TO_LRU);
		LRUNoCache(const LRUNoCache & other);
		virtual ~LRUNoCache()
		{
			clear();
		}

		V & operator[](const K & key);
		void insert(const K & key, const V & value);

		int size() const;
	
		bool empty() const;

		iterator find(const K & key);         // updates the MRU
		const_iterator find(const K & key) const;   // does not update the MRU

		const_iterator mru_begin() const; // from MRU to LRU
		

		iterator mru_begin();

		const_iterator lru_begin() const;           // from LRU to MRU
		iterator lru_begin();

	
		const_iterator begin() const;
		iterator begin();

		const_iterator end() const;
		iterator end();

		void dump_mru_to_lru(std::ostream & os) const;
		void SetDirection(DIRECTION _dir);

		DIRECTION getDirection() const;
		size_type erase(const K & key);
		iterator erase(const_iterator& _Where);
		void clear();
	protected:
		
		Val * _update(typename Val * moved);
		virtual Val * _insert(const K & key);
	
		unsigned _size;
		//MAP_TYPE _map;
		Val * _mru;
		Val * _lru;
		DIRECTION dir;
	};

	template<class K, class V>
	class LRUWithCache :public LRUNoCache<K,V>
	{
	public:

		typedef std::pair<const K, LRUValue<K, V> > Val;
		typedef LRUNoCache<K, V> _Mybase;
		LRUWithCache(unsigned maxsize, DIRECTION dir = MRU_TO_LRU);// Pre-condition: maxsize >= 1
		LRUWithCache(const LRUWithCache & other);
		unsigned maxsize() const;

	protected:
		Val* _insert(const K& key) override;
		
		unsigned _maxsize;
	};



	// Reserve enough space to avoid resizing later on and thus invalidate iterators
	template<class K, class V>
	LRUNoCache<K, V>::LRUNoCache(DIRECTION _dir)
		: _mru(NULL),
		 _lru(NULL),
		dir(_dir), _size(0)
	{

	}


	template<class K, class V>
	LRUNoCache<K, V>::LRUNoCache(const LRUNoCache<K, V> & other)
		:_mru(NULL),
		_lru(NULL),
		dir(other.dir)
	{
		for (const_iterator it = other.begin(); it != other.end(); ++it)
			this->insert(it.key(), it.value());	
	}


	template<class K, class V>
	V & LRUNoCache<K, V>::operator[](const K & key)
	{
	
		iterator it = find(key);
		Val* val;
		if (!it._ptr)
			val = _insert(key);
		else
			val = ((Val *)(it._ptr));

		return val->second._v;
	}


	template<class K, class V>
	void LRUNoCache<K, V>::insert(const K & key, const V & value)
	{
		iterator it = find(key);
		Val* val;
		if (!it._ptr)
			val = _insert(key);
		else
			val = ((Val *)(it._ptr));

		 val->second._v = value;
	}


	template<class K, class V>
	int LRUNoCache<K, V>::size() const
	{
		//return _map.size();
		return _size;
	}


	

	template<class K, class V>
	bool LRUNoCache<K, V>::empty() const
	{
		return _size > 0;
	}


	// updates MRU
	template<class K, class V>
	typename LRUNoCache<K, V>::iterator LRUNoCache<K, V>::find(const K & key)
	{
		iterator it = begin();
		//iterator _end = end();
		for (; it._ptr; ++it) {
			if (it._ptr->first == key) {
				return  iterator(_update(((Val *)(it._ptr))), dir);
			}
		}
		return  end();
	}


	// does not update MRU
	template<class K, class V>
	typename LRUNoCache<K, V>::const_iterator LRUNoCache<K, V>::find(const K & key) const
	{
	
		const_iterator it = begin();
		
		//const_iterator _end = end();
		for (; it._ptr; ++it) {
			if (it._ptr->first == key) {
				return   const_iterator(*it, dir);
			}
		}
	
		return end();

	}

	template <class K, class V>
	typename LRUNoCache<K, V>::const_iterator LRUNoCache<K, V>::mru_begin() const
	{
		return const_iterator(_mru, MRU_TO_LRU);
	}

	template<class K, class V>
	void LRUNoCache<K, V>::dump_mru_to_lru(std::ostream & os) const
	{
		os << "LRUCacheH4(" << size() << "/" << maxsize() << "): MRU --> LRU: " << std::endl;
		for (const_iterator it = mru_begin(); it != end(); ++it)
			os << it.key() << ": " << it.value() << std::endl;
	}

	template <class K, class V>
	void LRUNoCache<K, V>::SetDirection(DIRECTION _dir)
	{
		dir =_dir;
	}

	template <class K, class V>
	DIRECTION LRUNoCache<K, V>::getDirection() const
	{
		return dir;
	}

	

	template <class K, class V>
	typename LRUNoCache<K, V>::iterator LRUNoCache<K, V>::mru_begin()
	{
		return iterator(_mru, MRU_TO_LRU);
	}

	template<class K, class V>
	typename LRUNoCache<K, V>::const_iterator LRUNoCache<K, V>::lru_begin() const
	{
		return const_iterator(_lru, LRU_TO_MRU);
	}

	template <class K, class V>
	typename LRUNoCache<K, V>::iterator LRUNoCache<K, V>::lru_begin()
	{
		return iterator(_lru, LRU_TO_MRU);
	}

	template <class K, class V>
	typename LRUNoCache<K, V>::const_iterator LRUNoCache<K, V>::begin() const
	{
		return const_iterator(dir == MRU_TO_LRU ? _mru : _lru, dir);
	}

	template <class K, class V>
	typename LRUNoCache<K, V>::iterator LRUNoCache<K, V>::begin(){
	
		return iterator(dir == MRU_TO_LRU ? _mru : _lru, dir);
	}

	template <class K, class V>
	typename LRUNoCache<K, V>::iterator LRUNoCache<K, V>::end()
	{
	
		return iterator();
	}

	template<class K, class V>
	typename LRUNoCache<K, V>::const_iterator LRUNoCache<K, V>::end() const
	{
		return const_iterator();
	}


	

	template<class K, class V>
	typename LRUNoCache<K, V>::Val * LRUNoCache<K, V>::_update(typename Val * moved)
	{
		// possibly update the LRU
		if (moved == _lru && _lru->second._newer)
			_lru = _lru->second._newer;

		if (moved != _mru) {
			LRUValue<K, V> & v = moved->second;
			Val * older = v._older;
			Val * newer = v._newer;
			// "remove" key from current position
			if (older)
				older->second._newer = newer;
			if (newer)
				newer->second._older = older;

			// "insert" key to MRU position
			v._older = _mru;
			v._newer = NULL;
			_mru->second._newer = moved;
			_mru = moved;
		}

		return moved;
	}


	

	template<class K, class V>
	typename LRUNoCache<K, V>::Val * LRUNoCache<K, V>::_insert(const K & key)
	{
		// insert key to MRU position
	
		//std::pair<typename MAP_TYPE::iterator, bool> ret
		//		= _map.insert(make_pair(key, LRUValue<K, V>(V(), _mru, NULL)));
		//pair<const K, LRUValue<K, V> >
		//Val * inserted = &*ret.first;
		Val * inserted = new Val(key, LRUValue<K, V>(V(), _mru, NULL));
		_size += 1;
		if (_mru)
			_mru->second._newer = inserted;
		_mru = inserted;

		// possibly update the LRU
		if (!_lru)
			_lru = _mru;
		else if (!_lru->second._newer)
			_lru->second._newer = _mru;

		return inserted;
	}

	template <class K, class V>
	LRUWithCache<K, V>::LRUWithCache(unsigned maxsize, DIRECTION dir):_Mybase(dir)
		,_maxsize(maxsize)
	{
		assert(maxsize);
	}

	template <class K, class V>
	LRUWithCache<K, V>::LRUWithCache(const LRUWithCache& other):_Mybase(other),_maxsize(other._maxsize)
	{

	}

	template <class K, class V>
	unsigned LRUWithCache<K, V>::maxsize() const
	{
		return _maxsize;
	}

	template <class K, class V>
	typename LRUWithCache<K, V>::Val* LRUWithCache<K, V>::_insert(const K& key)
	{
		// if we have grown too large, remove LRU
		if (_Mybase::_size >= _maxsize) {
			Val * old_lru = _Mybase::_lru;
			if (_Mybase::_lru->second._newer) {
				_Mybase::_lru = _Mybase::_lru->second._newer;
				_Mybase::_lru->second._older = NULL;
			}
			delete old_lru;
			_Mybase::_size -= 1;
			//_Mybase::_map.erase(old_lru->first);
		}
		return _Mybase::_insert(key);
	}

	template <class K, class V>
	typename LRUNoCache<K, V>::size_type LRUNoCache<K, V>::erase(const K& key) {
		
		if (!_size)
			return 0;

		if (_mru && _mru->first == key) {
			Val* erase_obj = _mru;
		     // 当 map 中的数量等于1的时候，_mru 和 _lru指向同一个地方
			if (_mru->second._older) {
				_mru = _mru->second._older;
				_mru->second._newer = NULL;
			}
			else{
				_mru = nullptr;// _mru 没有后继，说明_mru 和_lru 是同一个东西
				_lru = nullptr;
			}
			delete erase_obj;
			_size -=1;
			return 1;
		  // return	_map.erase(key);	
		}

		if(_lru && _lru->first == key){
			Val* erase_obj = _lru;
			// 到这里的时候 _map的值必然大于1，
			//因为_mru 和 _lru指向同一个地方，能通过之前的检查，则说明_mru 和 _lru不是指向相同位置
			if (_lru->second._newer) {
				_lru = _lru->second._newer;
				_lru->second._older = NULL;
			}
			else{
				assert(false);
			}
			//return	 _map.erase(key);
			delete erase_obj;
			_size -= 1;
			return 1;
		}

		// 到这里的时候 _map的值必然大于2 ，而且要查找的对象不能在头和尾
		//iterator _end = end();
		iterator it = begin();
		++it;
		for (; it._ptr; ++it) {
			if (it._ptr->first == key) {
				Val*  val = ((Val *)(it._ptr));
				if (val->second._older)
					val->second._older->second._newer = val->second._newer;
				if (val->second._newer)
					val->second._newer->second._older = val->second._older;
				//return _map.erase(key);
				delete val;
				_size -= 1;
				return 1;
			}
		}
		
		
		return 0;
	}

	template <class K, class V>
	typename LRUNoCache<K, V>::iterator LRUNoCache<K, V>::erase(const_iterator& _Where)
	{
		if (!_Where._ptr)
			return end();

		iterator it = begin();
		for (; it._ptr; ++it) {
			if(_Where == it){
				const K &  _key = it._ptr->first;
				++it;
				erase(_key);
				return it;
			}	
		}

		return end();
	}

	template <class K, class V>
	void LRUNoCache<K, V>::clear()
	{
		iterator it = begin();
		Val * pre = nullptr;
		for (; it._ptr; ++it) {
			delete pre;
			pre = ((Val *)(it._ptr));
		}
		delete pre;
		_mru = nullptr;
		_lru = nullptr;
		_size = 0;
	}
}  // namespace lru