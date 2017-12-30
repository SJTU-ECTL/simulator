#ifndef __BOOL__NET__MEMORIZE__H__
#define __BOOL__NET__MEMORIZE__H__

#include <cassert>

template <class T>
class memorize {
	bool __is_valid;
	T __data;
public:
	memorize();
	inline bool is_valid() const ;
	inline const T& get() const ;
	virtual void set(const T &v);
	virtual void set(T &&v);
	virtual ~memorize() = default;
};

template <class T>
memorize<T>::memorize() : __is_valid(false) {}

template <class T>
bool memorize<T>::is_valid() const {
	return __is_valid;
}

template <class T>
const T & memorize<T>::get() const {
	if (!__is_valid) assert(false);
	else return __data;
}

template <class T>
void memorize<T>::set(const T &v) {
	__is_valid = true;
	__data = v;
}

template <class T>
void memorize<T>::set(T &&v) {
	__is_valid = true;
	__data = v;
}


#endif