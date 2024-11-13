#include "cache.h"
#include <random>

static CacheSimulation common_sim;

static void initCache(CacheSimulation *cache)
{
    cache->current_time = 0;
    cache->hits = 0;
    cache->misses = 0;
    cache->reads = 0;
    cache->prefetch_index = 0;
    cache->buffers.clear();
    cache->buffers.reserve(16);
    for (auto &set : cache->cache)
    {
        for (int i = 0; i < 64; i++)
        {
            set[0].data[i] = 0;
            set[1].data[i] = 0;
        }
        set[0].tag = 0;
        set[0].valid = false;
        set[1].tag = 0;
        set[1].valid = false;
    }
    for (int i = 0; i < 32; i++)
    {
        cache->prefetch_queue[i].prefetch_addr = 0;
        cache->prefetch_queue[i].prefetch_time = UINT64_MAX;
    }
    cache->prefetcher.cache_ptr = cache;
}

void initCache()
{
    initCache(&common_sim);
}

void registerPolicy(CacheSimulation::Policy policy)
{
    common_sim.policy = policy;
}

void enablePrefetcher()
{
    common_sim.prefetch_enabled = true;
}

void resetCacheStats()
{
    common_sim.hits = 0;
    common_sim.misses = 0;
    common_sim.reads = 0;
    common_sim.current_time = 0;
}
void printStats()
{
    std::cout << "Hit rate: " << (double)common_sim.hits / common_sim.reads << std::endl;
    std::cout << "Miss rate: " << (double)common_sim.misses / common_sim.reads << std::endl;
    std::cout << "Total time: " << common_sim.current_time << std::endl;
    // Add prefetcher stats:
    // prefetched blocks that were useful
    std::cout << "Number of usefull prefetches: " << "tomato" << std::endl;
    // prefetched blocks that were not used before eviction
    std::cout << "Number of wasted prefetches: " << "tomato" << std::endl;
    // ratio : unused prefetches/all preefetches
    std::cout << "Ratio of wasted/all prefetches : " << "tomato" << std::endl;
    // ratio : prefetched hits/all hits
    std::cout << "Ratio of prefetched hits/all hits : " << "tomato" << std::endl;
}
static void prefetchFromQueue(CacheSimulation *cache)
{
    for (int i = 0; i < 32; i++)
    {
        if (cache->prefetch_queue[i].prefetch_time < cache->current_time)
        {
            cache->prefetch_queue[i].prefetch_time = UINT64_MAX;
            // tomato: we want to prefetch here
            uintptr_t aligned_address = /*tomato*/ 0;

            uint8_t set = /*tomato*/ 0;
            uint64_t tag = /*tomato*/ 0;
            int i = 0;
            int block = 0;
            bool hit = false;
            for (; i < 8; i++)
            {
                if (false /*tomato replace this false*/)
                {
                    hit = true;
                    break;
                }
                if (!/*tomato replace this true*/ true)
                {
                    block = i;
                    break;
                }
                /*tomato: take into account your replacement policy.*/
                // switch(cache->policy)?
                if (/*tomato replace this*/ true)
                {
                    block = i;
                }
            }
            if (!hit)
            {
                /*tomato: we are getting the base pointer into the cache*/
                // tomato: dont forget the other modifications to the block
                uint8_t *base_pointer = (uint8_t *)(aligned_address);
                for (int j = 0; j < 64; j++)
                {
                    // tomato: copy here
                }
                // tomato: what to do with replacement bits?
            }
        }
    }
}

int getRandomWay() {
    return rand() % 8;
}

uint64_t cacheAddress(CacheSimulation *cache, uintptr_t address)
{
    // check if a prefetch arrived
    if (cache->prefetch_enabled)
    {
        prefetchFromQueue(cache);
        cache->prefetcher.collect_and_update(address);
        int should_prefetch = cache->prefetcher.condition_function(address);
        if (should_prefetch != -1)
        {
            cache->prefetcher.prefetch_mems(should_prefetch, address);
        }
    }
    // align the address to 64 bytes
    uintptr_t aligned_address = address & (~uintptr_t(0b111111));

    //52 bits tag + 6 bits set index + 6 bits block offset 
    uint8_t set = (address >> 6) & 0b111111;
    uint64_t tag = address >> 12;
    cache->reads++;
    bool hit = false;
    int i;
    uint64_t access_finishes;
    // Hint: 8 is associativity.
    for (i = 0; i < 8; i++)
    {
        if (cache->cache[set][i].valid && cache->cache[set][i].tag == tag)
        {
            hit = true;
            cache->hits++;
            break;
        }
    }
    if (!hit)
    {
        cache->misses++;
        access_finishes = cache->current_time + 20;
        
        // find an empty block first
        i = -1;
        for (int j = 0; j < 8; j++)
        {
            if (not cache->cache[set][j].valid)
            {
                i = j;
                break;
            }
        }

        // if no empty block, find one to replace
        if (i == -1) {
            switch (cache->policy)
            {
            case CacheSimulation::Policy::LRU :
                for (int j = 0; j < 8; j++) {
                    assert(cache->cache[set][j].valid);
                    if (cache->cache[set][j].LRU_cnt == 7) {
                        i = j;
                        break;
                    }
                }
                assert(i != -1);
                break;
            case CacheSimulation::Policy::Random :
                i = getRandomWay();
                assert(i >= 0 && i < 8);
                break;
            case CacheSimulation::Policy::Tree :
                i = 0;
                // printf("%d ", i);
                while (i < 7) {
                    if (cache->treeLRUs[set][i] == false) {
                        cache->treeLRUs[set][i] = true;
                        i = 2 * i + 1;
                    } else {
                        cache->treeLRUs[set][i] = false;
                        i = 2 * i + 2;
                    }
                    // printf("-> %d ", i);
                }
                // printf("\n");
                i = i - 7;
                assert(i >= 0 && i <= 7);
                break;
            default:
                assert(false);
            }
        }

        // initialize cache the new block
        uint8_t *base_pointer = (uint8_t *)(aligned_address);
        for (int j = 0; j < 64; j++)
        {
            cache->cache[set][i].data[j] = base_pointer[j];
        }
        cache->cache[set][i].tag = tag;
        cache->cache[set][i].valid = true;

        /* LRU cache hit updates START */
        for (int j = 0; j < 8; j ++) {
            if (j == i) {
                cache->cache[set][j].LRU_cnt = 0;
            } else {
                cache->cache[set][j].LRU_cnt += 1;
            }
        }
        /* LRU cache hit updates END */
    }
    else
    {
        access_finishes = cache->current_time + 1;
        
        /* LRU cache hit updates START */
        uint8_t cur_LRU = -1;
        for (int j = 0; j < 8; j++) {
            if (cache->cache[set][j].valid && cache->cache[set][j].tag == tag) {
                cur_LRU = cache->cache[set][j].LRU_cnt;
                break;
            }
        }
        assert(cur_LRU != -1);

        for (int j = 0; j < 8; j++) {
            if (cache->cache[set][j].valid) {
                if (cache->cache[set][j].LRU_cnt == cur_LRU) {
                    cache->cache[set][j].LRU_cnt = 0;
                } else if (cache->cache[set][j].LRU_cnt < cur_LRU) {
                    cache->cache[set][j].LRU_cnt += 1;
                } else if (cache->cache[set][j].LRU_cnt > cur_LRU) {
                    // nothing happens
                }
            }
        }
        /* LRU cache hit updates END */
    }
    return access_finishes;
}

uint8_t writeCacheByte(CacheSimulation *cache, uintptr_t address, uint8_t value)
{
    // writethrough
    *(uint8_t *)address = value;
    
    uint8_t set = (address >> 6) & 0b111111;
    uint64_t tag = address >> 12;
    uint64_t offset = address & 0b111111;

    int i = 0;
    for (; i < 8; i++)
    {
        if (cache->cache[set][i].valid && cache->cache[set][i].tag == tag)
        {
            cache->cache[set][i].data[offset] = value;
            return value;
        }
    }

    assert(false);
}

int writeCache(CacheSimulation *cache, uintptr_t address, int value)
{
    uint8_t *addr_ptr = (uint8_t *)address;
    uint8_t *value_ptr = (uint8_t *)&value; 
    // for (int i = 0; i < sizeof(int) / sizeof(uint8_t); i++) {
    for (unsigned i = 0; i < sizeof(int) / sizeof(uint8_t); i++) {
        assert(value_ptr[i] == writeCacheByte(cache, uintptr_t(addr_ptr + i), value_ptr[i]));
    }
    return value;
}

void prefetchCache(void *cache_voidptr, uintptr_t address)
{
    CacheSimulation *cache = reinterpret_cast<CacheSimulation*>(cache_voidptr);
    for (int i = 0; i < 32; i++)
    {
        if (cache->prefetch_queue[i].prefetch_time == UINT64_MAX)
        {
            cache->prefetch_queue[i].prefetch_addr = address;
            cache->prefetch_queue[i].prefetch_time = cache->current_time + 20;
            return;
        }
    }
}

bool ownedByUser(void *cache_voidptr, uintptr_t address)
{
    CacheSimulation *cache = reinterpret_cast<CacheSimulation*>(cache_voidptr);
    const std::vector<int_cached_buffer *> &buffers = cache->buffers;
    for (const auto &buffer : buffers)
    {
        uintptr_t buffer_start = (uintptr_t)buffer->buffer;
        uintptr_t buffer_end = (uintptr_t)buffer->buffer + buffer->size * sizeof(int);
        if (address >= buffer_start && address < buffer_end)
        {
            return true;
        }
    }
    return false;
}

void initBuffer(int_cached_buffer &buffer, int elem_count)
{
    // align to 32kB.
    buffer.buffer = (int *)aligned_alloc(32768, elem_count * sizeof(int));
    buffer.curr_ptr = buffer.buffer;
    buffer.cache_ptr = &common_sim;
    buffer.size = elem_count;
    common_sim.buffers.push_back(&buffer);
}

static void randomFillBuffer(int_cached_buffer &buffer, int *orig_buffer, int elem_count)
{
    for (int i = 0; i < elem_count; i++)
    {
        buffer.buffer[i] = orig_buffer[i] = rand();
    }
}
void initMatrixMultiplicationWithDimensions(int dim, matrix_mult_args &args)
{
    args.dim = dim;
    initBuffer(args.src, dim * dim);
    initBuffer(args.src2, dim * dim);
    args.src_orig = (int *)malloc(dim * dim * sizeof(int));
    args.src2_orig = (int *)malloc(dim * dim * sizeof(int));
    initBuffer(args.dst, dim * dim);
    randomFillBuffer(args.src, args.src_orig, dim * dim);
    randomFillBuffer(args.src2, args.src2_orig, dim * dim);
}

void initConvolutionWithDimensions(int dim, convolution_args &args)
{
    args.dim = dim;
    initBuffer(args.src, dim * dim);
    initBuffer(args.ker, 64);
    args.src_orig = (int *)malloc(dim * dim * sizeof(int));
    args.ker_orig = (int *)malloc(64 * sizeof(int));
    initBuffer(args.dst, dim * dim);
    randomFillBuffer(args.src, args.src_orig, dim * dim);
    randomFillBuffer(args.ker, args.ker_orig, 64);
}
void copyBaseDst(matrix_mult_args &args)
{
    int elem_count = args.dim * args.dim;
    args.dst_result = (int *)malloc(elem_count * sizeof(int));

    for (int i = 0; i < elem_count; i++)
    {
        args.dst_result[i] = args.dst.buffer[i];
    }
}
void copyBaseDst(convolution_args &args)
{
    int elem_count = args.dim * args.dim;
    args.dst_result = (int *)malloc(elem_count * sizeof(int));

    for (int i = 0; i < elem_count; i++)
    {
        args.dst_result[i] = args.dst.buffer[i];
    }
}
void checkBuffers(matrix_mult_args &args)
{
    int elem_count = args.dim * args.dim;
    for (int i = 0; i < elem_count; i++)
    {
        if (args.src_orig[i] != args.src.buffer[i])
        {
            std::cerr << "You cannot change the src buffers\n";
            assert(false);
        }
        if (args.src2_orig[i] != args.src2.buffer[i])
        {
            std::cerr << "You cannot change the src buffers\n";
            assert(false);
        }
        if (args.dst_result[i] != args.dst.buffer[i])
        {
            std::cerr << "The destionation matrix does not match expected\n";
            assert(false);
        }
    }
}
void checkBuffers(convolution_args &args)
{
    int elem_count = args.dim * args.dim;
    for (int i = 0; i < elem_count; i++)
    {
        if (args.src_orig[i] != args.src.buffer[i])
        {
            std::cerr << "You cannot change the src buffer\n";
            assert(false);
        }
        if (args.dst_result[i] != args.dst.buffer[i])
        {
            std::cerr << "The destionation matrix does not match expected\n";
            assert(false);
        }
    }
    for (int j = 0; j < 64; j++)
    {
        if (args.ker_orig[j] != args.ker.buffer[j])
        {
            std::cerr << "You cannot change the kernel\n";
            assert(false);
        }
    }
}