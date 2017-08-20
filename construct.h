template<class T1,class T2>
inline void construct(T1*p, const T2& value)
{
	new (p)T1(value);

}

//以下是destroy第一个版本，接受一个指针
template<class T>
inline void destroy(T* pointer)
{
	pointer->~T();
}

//以下是destroy的第二个版本接受两个迭代器，设法找出元素的数值类别，进而利用__type_tarits<>求取最适当措施
template<class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	__destroy(first, last, value_type(first));
}


//判断元素的数值型别是否有trivial destructor
template<class ForwardIterator,class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destroy_aux(first, last, trivial_destructor());
}

//如果元素的数值型别有non-trivial destructor
template <class ForwardIterator>
inline void 
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first < last;++first)
	{
		destroy(&*first);
	}
}

//如果元素数值型别有trivial destructor
template < class ForwardIterator>
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type)
{

}

//以下是destroy第二版本针对迭代器为char* 和 wchar_t*的特化版本
inline void destroy(char*, char*){}
inline void destroy(wchar_t*, wchar_t*){}

//uninitialized_copy,将内存配置与对象的构造行为分开，如果作为输出目的地的[result,result+(last-first))范围内的每一个迭代器都指向未初始化区域，则unitiallized_copy( )会使用copy constructor
//从输入范围中的每个对象拷贝一份放进输出范围
template<class InputIterator,class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}

template<class InputIterator,class ForwradIterator,class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator,class ForwardIterator>
inline ForwardIterator 
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
	return copy(first, last, result);
}
template <class InputIterator, class ForwardIterator>
__uninitialized_copy_anx(InputIterator first, InputIterator last, ForwardIterator result, __flase_type)
{
	ForwardIter cur = result;
	for (; first != last;++first,++cur)
	{
		construct(&*cur, *first);
	}
	return cur;
}
inline char* uninitialized_copy(const char* first, const char* last, char*result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}
inline wchar_t*uninitialized_copy(const wchar_t* first, const wchar_t*last, wchar_t* result)
{
	memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}
//uninitialized_fill如果[first,last)范围内的每个迭代器都指向未初始化的内存，那么uninitialized_fill会在该范围内产生x的复制品。
template<class ForwardIterator,class T>
void unitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	__uninitialized_fill(first, last, x, value_type(first));
}
template <class ForwardIterator, class T,class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T&x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	__uninitialized_fill_aux(first, last, x, is_POD());
}

template <class ForwardIterator,class T>
inline void 
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T&x, __true_type)
{
	fill(first, last, x);
}
template <class ForwardIterator,class T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T&x, __false_type)
{
	ForwardIterator cur = first;
	for (; cur != last;++cur)
	{
		construct(&*cur, x);
	}
}
//uninitialized_fill_n，会在[first,first+n)范围内产生T的复制品
template<class ForwardIterator,class Size,class T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return __uninitialized_fill_n(first, n, x, value_type(first));
}
template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T&x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
template<class ForwardIterator,class Size,class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T&x, __true_type)
{
	return fill_n(first, n, x);
}
template<class ForwardIterator,class Size,class T>
ForwardIterator 
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T&x, __false_type)
{
	ForwardIterator cur = first;
	for (; n > 0;--n,++cur)
	{
		construct(&*cur, x);
	}
	return cur;
}

//fill在迭代器指向范围内赋值，fill_n在first的前n个位置赋值

//标准库中copy算法的实现
template<class InputIterator ,class OutputIterator>
inline OutputItrator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	return __copy_distpatch<InputIterator, OutputIterator>()
		(first, last, result);
}

inline char*copy(const char*first, const char*last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t*result)
{
	memmove(result, first, sizeof(wchar_t)*(last - first));
}


template < class InputIterator,class OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator ()(InputIterator first, InputIterator last, OutputIterator result)
	{
		return __copy(first, last, result, iterator_category(first));
	}
};
template<class T>
struct __copy_distpatch<T*, T*>
{
	T* operator(T* first, T*last, T*result)
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, alst, result, t());
	}
};
template <class T>
struct __copy_dispatch <const T*, T*>
{
	T*operator()(const T*first, const T*last, T*result)
	{
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template<class InputIterator ,class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator resutl, input_iterator_tag)
{
	for (; first != last;++result,++first)
	{
		*result = *first;
	}
	return result;
}
template<class RandomAccessIterator,class OutputIterator>
inline OutputIterator
__copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result,
random_access_iterator_tag)
{
	return __copy_d(first, last, result, distance_type(first));
}
template<class RandomAccessIterator,class OutputIterator ,class Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomIterator last, OutputIterator result, Distance*)
{
	for (Distance n = last - first; n > 0;--n,++result,++first)
	{
		*result = *first;
	}
	return result;
}

template<class T>
inline T*__copy_t(const T*first, const T*last, T*result, __true_type)
{
	memmove(result, first, sizeof(T)*(last - first));
	return result + (last - first);
}
template <class T>
inline T* __copy_t(const T*first, const T*last, T*result, __false_type)
{
	return __copy_d(first, last result, (ptrdiff_t *)0);
}