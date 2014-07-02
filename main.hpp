#ifndef MAIN_HPP
#define MAIN_HPP
extern "C" void * make_cache();
extern "C" void free_cache(void *);
extern "C" char const * search(void *, char *);
#endif // MAIN_CPP
