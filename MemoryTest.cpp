#include "Memory.cpp"

TEST(AllocationTest, WorkTest) {
  MemoryAllocator allocator;
  allocator.init();
  
  int *pi = (int*) allocator.alloc(sizeof(int));
  double *pd = (double*) allocator.alloc(sizeof(double));
  int *pa = (int*) allocator.alloc(10 * sizeof(int));
  
  allocator.free(pa);
  allocator.free(pd);
  allocator.free(pi);
  
  allocator.destroy();
}

TEST(MemoryAllocatorTest, FixedMemory)
{
   MemoryAllocator allocator;
   allocator.init();
  
   for (size_t i = 0; i < 1024; i <<= 2) {
     void* pv = allocator.alloc(i);
   
     EXPECT_TRUE((uintptr_t)pv == i);      
     
     allocator.free(pv);
   }
  
  
   allocator.destroy();
}

TEST(MemoryAllocatorTest, Coalesce)
{
    MemoryAllocator allocator;
    allocator.init();
  
    void* pv = allocator.alloc(237);
    
    EXPECT_TRUE((uintptr_t)pv % 8 == 0);
  
    allocator.free(pv);
  
    allocator.destroy();
}
