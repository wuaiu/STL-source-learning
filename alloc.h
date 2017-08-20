//一级空间配置器
template<int inst>
class __malloc_alloc_template{
private:
	staic void* oom_malloc(size_t);
	staic void* oom_realloc(void*, size_t);
	staic void * (*__malloc_alloc_oom_handler)();
public:
	static void * allocate(size_t n)
	{
		void* result = malloc(n);
		if (0==result)
		{
			result = oom_malloc(n);
		}
		return result;
	}
	static void deallocate(void *p, size_t)
	{
		free(p);
	}
	static void reallocate(void *p, size_t, size_t new_sz)
	{
		void * result = reallocate(p, new_sz);
		if (0==result)
		{
			result = oom_realloc(p, new_sz);
		}
		return result;
	}

};