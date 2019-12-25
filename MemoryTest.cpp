#include "Memory.cpp"

TEST(AllocationTest, SimpleTest) {
  MemoryAllocator allocator;
  allocator.init();
  
  int *pi = allocator.alloc(sizeof(int));
  double *pd = allocator.alloc(sizeof(double));
  int *pa = allocator.alloc(10 * sizeof(int));
  
  allocator.free(pa);
  allocator.free(pd);
  allocator.free(pi);
  
  allocator.destroy();
}
