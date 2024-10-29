#include "cache.h"
#include "simulated_ints.h"

int int_cache_dependent::operator=(int k)
{
    value = k;
    ready_at = 0;
    return k;
}
int_cache_dependent::int_cache_dependent(int k) : value(k), ready_at(0) {}
int_cache_dependent::int_cache_dependent() : ready_at(0) {}
int_cache_dependent::int_cache_dependent(int k, uint64_t ready) : value(k), ready_at(ready) {}
int_cache_dependent int_cache_dependent::operator+(int k) const
{
    return {value + k, ready_at};
}
int_cache_dependent int_cache_dependent::operator-(int k) const
{
    return {value - k, ready_at};
}
int_cache_dependent int_cache_dependent::operator*(int k) const
{
    return {value * k, ready_at};
}
int_cache_dependent int_cache_dependent::operator/(int k) const
{
    return {value / k, ready_at};
}
int_cache_dependent int_cache_dependent::operator%(int k) const
{
    return {value % k, ready_at};
}
int_cache_dependent int_cache_dependent::operator+(int_cache_dependent other) const
{
    return {value + other.value, std::max(ready_at, other.ready_at)};
}
int_cache_dependent int_cache_dependent::operator-(int_cache_dependent other) const
{
    return {value - other.value, std::max(ready_at, other.ready_at)};
}
int_cache_dependent int_cache_dependent::operator*(int_cache_dependent other) const
{
    return {value * other.value, std::max(ready_at, other.ready_at)};
}
int_cache_dependent int_cache_dependent::operator/(int_cache_dependent other) const
{
    return {value / other.value, std::max(ready_at, other.ready_at)};
}
int_cache_dependent int_cache_dependent::operator%(int_cache_dependent other) const
{
    return {value % other.value, std::max(ready_at, other.ready_at)};
}
int_cache_dependent int_cache_dependent::operator+=(int k)
{
    value += k;
    return *this;
}
int_cache_dependent int_cache_dependent::operator-=(int k)
{
    value -= k;
    return *this;
}
int_cache_dependent int_cache_dependent::operator*=(int k)
{
    value *= k;
    return *this;
}
int_cache_dependent int_cache_dependent::operator/=(int k)
{
    value /= k;
    return *this;
}
int_cache_dependent int_cache_dependent::operator%=(int k)
{
    value %= k;
    return *this;
}
int_cache_dependent int_cache_dependent::operator+=(int_cache_dependent other)
{
    value += other.value;
    ready_at = std::max(ready_at, other.ready_at);
    return *this;
}
int_cache_dependent int_cache_dependent::operator-=(int_cache_dependent other)
{
    value -= other.value;
    ready_at = std::max(ready_at, other.ready_at);
    return *this;
}
int_cache_dependent int_cache_dependent::operator*=(int_cache_dependent other)
{
    value *= other.value;
    ready_at = std::max(ready_at, other.ready_at);
    return *this;
}
int_cache_dependent int_cache_dependent::operator/=(int_cache_dependent other)
{
    value /= other.value;
    ready_at = std::max(ready_at, other.ready_at);
    return *this;
}
int_cache_dependent int_cache_dependent::operator%=(int_cache_dependent other)
{
    value %= other.value;
    ready_at = std::max(ready_at, other.ready_at);
    return *this;
}
int_cache_dependent int_cache_dependent::operator++()
{
    value++;
    return *this;
}
int_cache_dependent int_cache_dependent::operator--()
{
    value--;
    return *this;
}
int_cache_dependent int_cache_dependent::operator++(int)
{
    int_cache_dependent res = *this;
    value++;
    return res;
}
int_cache_dependent int_cache_dependent::operator--(int)
{
    int_cache_dependent res = *this;
    value--;
    return res;
}
bool int_cache_dependent::operator==(int k) const
{
    return value == k;
}
bool int_cache_dependent::operator!=(int k) const
{
    return value != k;
}
bool int_cache_dependent::operator<(int k) const
{
    return value < k;
}
bool int_cache_dependent::operator>(int k) const
{
    return value > k;
}
bool int_cache_dependent::operator<=(int k) const
{
    return value <= k;
}
bool int_cache_dependent::operator>=(int k) const
{
    return value >= k;
}
bool int_cache_dependent::operator==(int_cache_dependent other) const
{
    return value == other.value;
}
bool int_cache_dependent::operator!=(int_cache_dependent other) const
{
    return value != other.value;
}
bool int_cache_dependent::operator<(int_cache_dependent other) const
{
    return value < other.value;
}
bool int_cache_dependent::operator>(int_cache_dependent other) const
{
    return value > other.value;
}
bool int_cache_dependent::operator<=(int_cache_dependent other) const
{
    return value <= other.value;
}
bool int_cache_dependent::operator>=(int_cache_dependent other) const
{
    return value >= other.value;
}
// int_cached_value_lazy
void int_cached_value_lazy::update_time(uint64_t time) const
{
    if (cache_ptr->current_time < time)
    {
        cache_ptr->current_time = time;
    }
}
int int_cached_value_lazy::operator=(int k)
{
    uint64_t time = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(time);
    writeCache(cache_ptr, (uintptr_t)ptr, k);
    return k;
}
int int_cached_value_lazy::operator=(int_cached_value_lazy other)
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(time_this);
    update_time(time_other);
    writeCache(cache_ptr, (uintptr_t)ptr, *other.ptr);
    return *other.ptr;
}
int int_cached_value_lazy::operator=(int_cache_dependent dep)
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    uint64_t time_dep = dep.ready_at;
    update_time(time_this);
    update_time(time_dep);
    writeCache(cache_ptr, (uintptr_t)ptr, dep.value);
    return dep.value;
}
int_cache_dependent int_cached_value_lazy::operator+(int_cached_value_lazy other) const
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);

    return {*ptr + *other.ptr, std::max(time_this, time_other)};
}
int_cache_dependent int_cached_value_lazy::operator+(int_cache_dependent dep) const
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr + dep.value, std::max(time_this, time_dep)};
}
int_cache_dependent int_cached_value_lazy::operator+(int k) const
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr + k, time_this};
}
int_cache_dependent int_cached_value_lazy::operator-(int_cached_value_lazy other) const
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr - *other.ptr, std::max(time_this, time_other)};
}
int_cache_dependent int_cached_value_lazy::operator-(int_cache_dependent dep) const
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr - dep.value, std::max(time_this, time_dep)};
}
int_cache_dependent int_cached_value_lazy::operator-(int k) const
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr - k, time_this};
}
int_cache_dependent int_cached_value_lazy::operator*(int_cached_value_lazy other) const
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr * *other.ptr, std::max(time_this, time_other)};
}
int_cache_dependent int_cached_value_lazy::operator*(int_cache_dependent dep) const
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr * dep.value, std::max(time_this, time_dep)};
}
int_cache_dependent int_cached_value_lazy::operator*(int k) const
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr * k, time_this};
}
int_cache_dependent int_cached_value_lazy::operator/(int_cached_value_lazy other) const
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr / *other.ptr, std::max(time_this, time_other)};
}
int_cache_dependent int_cached_value_lazy::operator/(int_cache_dependent dep) const
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr / dep.value, std::max(time_this, time_dep)};
}
int_cache_dependent int_cached_value_lazy::operator%(int_cached_value_lazy other) const
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr % *other.ptr, std::max(time_this, time_other)};
}
int_cache_dependent int_cached_value_lazy::operator%(int_cache_dependent dep) const
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    return {*ptr % dep.value, std::max(time_this, time_dep)};
}
int int_cached_value_lazy::operator+=(int_cached_value_lazy other)
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_other));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr + *other.ptr);
    return *ptr + *other.ptr;
}
int int_cached_value_lazy::operator+=(int_cache_dependent dep)
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_dep));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr + dep.value);
    return *ptr + dep.value;
}
int int_cached_value_lazy::operator+=(int k)
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(time_this);
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr + k);
    return *ptr + k;
}
int int_cached_value_lazy::operator-=(int_cached_value_lazy other)
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_other));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr - *other.ptr);
    return *ptr - *other.ptr;
}
int int_cached_value_lazy::operator-=(int_cache_dependent dep)
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_dep));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr - dep.value);
    return *ptr - dep.value;
}
int int_cached_value_lazy::operator-=(int k)
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(time_this);
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr - k);
    return *ptr - k;
}
int int_cached_value_lazy::operator*=(int_cached_value_lazy other)
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_other));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr * *other.ptr);
    return *ptr * *other.ptr;
}
int int_cached_value_lazy::operator*=(int_cache_dependent dep)
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_dep));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr * dep.value);
    return *ptr * dep.value;
}
int int_cached_value_lazy::operator*=(int k)
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(time_this);
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr * k);
    return *ptr * k;
}
int int_cached_value_lazy::operator/=(int_cached_value_lazy other)
{
    uint64_t time_other = cacheAddress(cache_ptr, (uintptr_t)other.ptr);
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_other));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr / *other.ptr);
    return *ptr / *other.ptr;
}
int int_cached_value_lazy::operator/=(int_cache_dependent dep)
{
    uint64_t time_dep = dep.ready_at;
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(std::max(time_this, time_dep));
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr / dep.value);
    return *ptr / dep.value;
}
int int_cached_value_lazy::operator/=(int k)
{
    uint64_t time_this = cacheAddress(cache_ptr, (uintptr_t)ptr);
    update_time(time_this);
    writeCache(cache_ptr, (uintptr_t)ptr, *ptr / k);
    return *ptr / k;
}
// int_cached_ptr
int_cached_value_lazy int_cached_ptr::operator*() const
{
    return {ptr, cache_ptr};
}
int_cached_value_lazy int_cached_ptr::operator[](int i) const
{
    return {(int *)((uintptr_t)ptr + i * sizeof(int)), cache_ptr};
}

int_cached_ptr::int_cached_ptr(int *p, CacheSimulation *cache) : ptr(p), cache_ptr(cache) {}
int_cached_ptr::int_cached_ptr(int_cached_buffer b) : ptr(b.curr_ptr), cache_ptr(b.cache_ptr) {}
int_cached_ptr::int_cached_ptr() : ptr(nullptr), cache_ptr(nullptr) {}
int_cached_ptr &int_cached_ptr::operator++()
{
    ptr++;
    return *this;
}
int_cached_ptr &int_cached_ptr::operator--()
{
    ptr--;
    return *this;
}
int_cached_ptr int_cached_ptr::operator++(int)
{
    int_cached_ptr res = *this;
    ptr++;
    return res;
}
int_cached_ptr int_cached_ptr::operator--(int)
{
    int_cached_ptr res = *this;
    ptr--;
    return res;
}
int_cached_ptr &int_cached_ptr::operator+=(int n)
{
    ptr += n;
    return *this;
}
int_cached_ptr &int_cached_ptr::operator-=(int n)
{
    ptr -= n;
    return *this;
}
int_cached_ptr int_cached_ptr::operator+(int n) const
{
    int_cached_ptr res = *this;
    res += n;
    return res;
}
int_cached_ptr int_cached_ptr::operator-(int n) const
{
    int_cached_ptr res = *this;
    res -= n;
    return res;
}
int_cached_ptr &int_cached_ptr::operator=(int_cached_ptr other)
{
    ptr = other.ptr;
    cache_ptr = other.cache_ptr;
    return *this;
}
int_cached_ptr &int_cached_ptr::operator=(int_cached_buffer other)
{
    ptr = other.curr_ptr;
    cache_ptr = other.cache_ptr;
    return *this;
}

// int_cached_buffer

int_cached_value_lazy int_cached_buffer::operator[](int i) const { return {(int *)((uintptr_t)curr_ptr + i * sizeof(int)), cache_ptr}; }
int_cached_value_lazy int_cached_buffer::operator[](int_cache_dependent i) const { return {(int *)((uintptr_t)curr_ptr + i.value), cache_ptr}; }
int_cached_value_lazy int_cached_buffer::operator*() const { return {curr_ptr, cache_ptr}; }
int_cached_ptr int_cached_buffer::operator+(int n) const
{
    return {curr_ptr + n, cache_ptr};
}
int_cached_ptr int_cached_buffer::operator++()
{
    curr_ptr++;
    return {curr_ptr, cache_ptr};
}
int_cached_ptr int_cached_buffer::operator++(int)
{
    int_cached_ptr cp = {curr_ptr, cache_ptr};
    curr_ptr++;
    return cp;
}
int_cached_ptr int_cached_buffer::operator-(int n) const
{
    return {curr_ptr - n, cache_ptr};
}
int_cached_ptr int_cached_buffer::operator--()
{
    curr_ptr--;
    return {curr_ptr, cache_ptr};
}
int_cached_ptr int_cached_buffer::operator--(int)
{
    int_cached_ptr cp = {curr_ptr, cache_ptr};
    curr_ptr--;
    return cp;
}
int_cached_buffer &int_cached_buffer::operator+=(int n)
{
    curr_ptr += n;
    return *this;
}
int_cached_buffer &int_cached_buffer::operator-=(int n)
{
    curr_ptr -= n;
    return *this;
}