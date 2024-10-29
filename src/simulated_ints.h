#ifndef _SIMULATED_INTS_
#define _SIMULATED_INTS_
struct CacheSimulation;
struct int_cached_value_lazy;
struct int_cached_ptr;
struct int_cached_buffer;
struct int_cache_dependent
{
    int value;
    uint64_t ready_at;
    int operator=(int k);
    int_cache_dependent(int k);
    int_cache_dependent();
    int_cache_dependent(int k, uint64_t ready);
    int_cache_dependent operator+(int k) const;
    int_cache_dependent operator-(int k) const;
    int_cache_dependent operator*(int k) const;
    int_cache_dependent operator/(int k) const;
    int_cache_dependent operator%(int k) const;
    int_cache_dependent operator+(int_cache_dependent other) const;
    int_cache_dependent operator-(int_cache_dependent other) const;
    int_cache_dependent operator*(int_cache_dependent other) const;
    int_cache_dependent operator/(int_cache_dependent other) const;
    int_cache_dependent operator%(int_cache_dependent other) const;
    int_cache_dependent operator+=(int k);
    int_cache_dependent operator-=(int k);
    int_cache_dependent operator*=(int k);
    int_cache_dependent operator/=(int k);
    int_cache_dependent operator%=(int k);
    int_cache_dependent operator+=(int_cache_dependent other);
    int_cache_dependent operator-=(int_cache_dependent other);
    int_cache_dependent operator*=(int_cache_dependent other);
    int_cache_dependent operator/=(int_cache_dependent other);
    int_cache_dependent operator%=(int_cache_dependent other);
    int_cache_dependent operator++();
    int_cache_dependent operator--();
    int_cache_dependent operator++(int);
    int_cache_dependent operator--(int);
    bool operator==(int k) const;
    bool operator!=(int k) const;
    bool operator<(int k) const;
    bool operator>(int k) const;
    bool operator<=(int k) const;
    bool operator>=(int k) const;
    bool operator==(int_cache_dependent other) const;
    bool operator!=(int_cache_dependent other) const;
    bool operator<(int_cache_dependent other) const;
    bool operator>(int_cache_dependent other) const;
    bool operator<=(int_cache_dependent other) const;
    bool operator>=(int_cache_dependent other) const;
};
struct int_cached_value_lazy
{
    int *ptr;
    CacheSimulation *cache_ptr;
    void update_time(uint64_t time) const;
    int operator=(int k);
    int operator=(int_cached_value_lazy other);
    int operator=(int_cache_dependent dep);
    int_cache_dependent operator+(int_cached_value_lazy other) const;
    int_cache_dependent operator+(int_cache_dependent dep) const;
    int_cache_dependent operator+(int k) const;
    int_cache_dependent operator-(int_cached_value_lazy other) const;
    int_cache_dependent operator-(int_cache_dependent dep) const;
    int_cache_dependent operator-(int k) const;
    int_cache_dependent operator*(int_cached_value_lazy other) const;
    int_cache_dependent operator*(int_cache_dependent dep) const;
    int_cache_dependent operator*(int k) const;
    int_cache_dependent operator/(int_cached_value_lazy other) const;
    int_cache_dependent operator/(int_cache_dependent dep) const;
    int_cache_dependent operator%(int_cached_value_lazy other) const;
    int_cache_dependent operator%(int_cache_dependent dep) const;
    int operator+=(int_cached_value_lazy other);
    int operator+=(int_cache_dependent dep);
    int operator+=(int k);
    int operator-=(int_cached_value_lazy other);
    int operator-=(int_cache_dependent dep);
    int operator-=(int k);
    int operator*=(int_cached_value_lazy other);
    int operator*=(int_cache_dependent dep);
    int operator*=(int k);
    int operator/=(int_cached_value_lazy other);
    int operator/=(int_cache_dependent dep);
    int operator/=(int k);
};

struct int_cached_ptr
{
    int *ptr;
    CacheSimulation *cache_ptr;
    int_cached_value_lazy operator*() const;
    int_cached_value_lazy operator[](int i) const;
    int_cached_ptr(int *p, CacheSimulation *cache);
    int_cached_ptr(int_cached_buffer b);
    int_cached_ptr();
    int_cached_ptr &operator++();
    int_cached_ptr &operator--();
    int_cached_ptr operator++(int);
    int_cached_ptr operator--(int);
    int_cached_ptr &operator+=(int n);
    int_cached_ptr &operator-=(int n);
    int_cached_ptr operator+(int n) const;
    int_cached_ptr operator-(int n) const;
    int_cached_ptr &operator=(int_cached_ptr other);
    int_cached_ptr &operator=(int_cached_buffer other);
};

struct int_cached_buffer
{
    int *buffer;
    int *curr_ptr;
    uintptr_t size;
    CacheSimulation *cache_ptr;

    int_cached_value_lazy operator[](int i) const;
    int_cached_value_lazy operator[](int_cache_dependent i) const;
    int_cached_value_lazy operator*() const;
    int_cached_ptr operator+(int n) const;
    int_cached_ptr operator++();
    int_cached_ptr operator++(int);
    int_cached_ptr operator-(int n) const;
    int_cached_ptr operator--();
    int_cached_ptr operator--(int);
    int_cached_buffer &operator+=(int n);
    int_cached_buffer &operator-=(int n);
};

#endif // _SIMULATED_INTS_