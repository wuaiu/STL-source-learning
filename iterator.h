//五种迭代器类型

struct input_iterator_tag{};    //只读
struct output_iterator_tag{};   //维写
struct forward_iterator_tag{};  //读写
struct bidirectional_iterator_tag{};  //双向操作
struct random_access_iterator_tag :public bidirectional_iterator_tag{};

//自行开发的迭代器最好继承自下面这个std::iterator
template<class Category,class T, class Distance = ptrdiff_t,
	      class Pointer = T*,class Reference = T&>
struct iterator{
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
};

//萃取机制
template<class Iterator>
typedef iterator_traits{
	typdef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
};

//针对原生指针而设计的traits偏特化版
template <class T>
struct iterator_traits<T*>{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T*;
	typedef T&;  
};
//针对原生指针之pointer-to-const而设计的traits偏特化
template<class T>
struct iterator_traits<const T *>
{
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef const T * pointer;
	typedef const T&;
};
//这个函数可以很方便地决定某个迭代器的类型
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
	typedef typename iterator_traits<Iterator>::iterator_category;
	return category();
}

//这个函数可以很方便决定某个迭代器的distance_type
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//这个函数可以很方便决定某个迭代器value_type
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}
//以下是整组distance函数
template<class InputIterator>
inline iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
{
	iterator_traits<InputIterator>::dirference_type n = 0;
	while (first!=last)
	{
		++first;
		++n;
	}
	return n;
}
template<class RandomAccessIterator>
inline iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
	return last - first;
}
template<class InputIterator>
inline iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	return __distance(first, last, category());
}
template <class InputIterator,class Distance>
inline void __advance(InputIterator&i, Distance n, input_iterator_tag)
{
	while (n--)
	{
		++i;
	}
}
template<class BidirecionalIterator,class Distance>
inline void __advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
	if (n>=0)
	{
		while (n--)
		{
			++i;
		}
	}
	else
	{
		while (n++)
		{
			--i;
		}
	}
}
template<class RandomAccessIterator ,class Distance>
inline void __advance(RandomAccessIterator &i, Distance m, random_access_iterator_tag)
{
	i += n;
}
template<class InputIterator,class Distance>
inline void advance(InputIterator &i, Distance n)
{
	__advance(i, n, iterator_category(i));
}