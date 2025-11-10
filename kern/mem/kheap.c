#include "kheap.h"

#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include <kern/conc/sleeplock.h>
#include <kern/proc/user_environment.h>
#include <kern/mem/memory_manager.h>
#include "../conc/kspinlock.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//


// My Helper Methods
int page_free(uint32 x){
	int perm  = pt_get_page_permissions(ptr_page_directory,x);
	if(perm & PERM_PRESENT) return 0;
	return 1;
}
//==============================================
// [1] INITIALIZE KERNEL HEAP:
//==============================================
//TODO: [PROJECT'25.GM#2] KERNEL HEAP - #0 kheap_init [GIVEN]
//Remember to initialize locks (if any)
void kheap_init()
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		initialize_dynamic_allocator(KERNEL_HEAP_START, KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE);
		set_kheap_strategy(KHP_PLACE_CUSTOMFIT);
		kheapPageAllocStart = dynAllocEnd + PAGE_SIZE;
		kheapPageAllocBreak = kheapPageAllocStart;
	}
	//==================================================================================
	//==================================================================================
}

//==============================================
// [2] GET A PAGE FROM THE KERNEL FOR DA:
//==============================================
int get_page(void* va)
{
	int ret = alloc_page(ptr_page_directory, ROUNDDOWN((uint32)va, PAGE_SIZE), PERM_WRITEABLE, 1);
	if (ret < 0)
		panic("get_page() in kern: failed to allocate page from the kernel");
	return 0;
}

//==============================================
// [3] RETURN A PAGE FROM THE DA TO KERNEL:
//==============================================
void return_page(void* va)
{
	unmap_frame(ptr_page_directory, ROUNDDOWN((uint32)va, PAGE_SIZE));
}

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
//===================================
// [1] ALLOCATE SPACE IN KERNEL HEAP:
//===================================
void* kmalloc(unsigned int size)
{

	//TODO: [PROJECT'25.GM#2] KERNEL HEAP - #1 kmalloc
	//Your code is here
	//Comment the following line
	if(size <=DYN_ALLOC_MAX_BLOCK_SIZE)return alloc_block(size);
	int num_pages = (size+PAGE_SIZE-1)/PAGE_SIZE , page_size = num_pages * PAGE_SIZE;

	uint32 start = 0,end = 0;
	uint32 exact_s = 0,exact_e=0,worst_s=0,worst_e=0;
	cprintf("size: %u  -  kheapS: %u  - kheape: %u  \n",size,kheapPageAllocStart,kheapPageAllocBreak);
	cprintf("%u : %u \n",num_pages,page_size);
	cprintf("end: %u  -  K: %u \n",kheapPageAllocBreak,KERNEL_HEAP_MAX);
	for(uint32 i = kheapPageAllocStart; i <kheapPageAllocBreak;i+=PAGE_SIZE){
		if(page_free(i)){
			end = i+PAGE_SIZE;
			if(start == 0) start = i;
		}
		else {
			if(end - start >=page_size){
				//exact fit
				if(end - start == page_size) {
					exact_s= start,exact_e=end;
					break;
				}
				// worst
				else if( (end-start) > worst_e-worst_s){
					worst_e = end,worst_s = start;
				}
			}
			start= 0,end = 0;
		}
	}

	uint32 page_s = 0,page_e = 0;
	// exact fit
	if(exact_s!=0) page_s = exact_s,page_e = exact_e;
	// worst fit
	else if(worst_s !=0) page_s = worst_s,page_e=worst_s + page_size;

	// !exact and !worst
	if(page_s == 0){
		cprintf("ana: %u \n",kheapPageAllocBreak);
		if(kheapPageAllocBreak <= KERNEL_HEAP_MAX - page_size){

			page_s = kheapPageAllocBreak;
			page_e = kheapPageAllocBreak  + page_size;
			cprintf("ps: %u - pe: %u  - size: %u \n",page_s,page_e,page_size);
			kheapPageAllocBreak = page_e;
			cprintf("ana2: %u \n",kheapPageAllocBreak);
		}
		else return NULL;

	}
	cprintf("k1: %u \n",kheapPageAllocBreak);
	for(uint32 i = page_s; i <page_e;i+=PAGE_SIZE) get_page((void*)i);
	cprintf("k2: %u \n",kheapPageAllocBreak);
	return (void*)page_s;


	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//TODO: [PROJECT'25.BONUS#3] FAST PAGE ALLOCATOR
}

//=================================
// [2] FREE SPACE FROM KERNEL HEAP:
//=================================
void kfree(void* virtual_address)
{
	//TODO: [PROJECT'25.GM#2] KERNEL HEAP - #2 kfree
	//Your code is here
	//Comment the following line
	panic("kfree() is not implemented yet...!!");
}

//=================================
// [3] FIND VA OF GIVEN PA:
//=================================
unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT'25.GM#2] KERNEL HEAP - #3 kheap_virtual_address
	//Your code is here
	//Comment the following line
	panic("kheap_virtual_address() is not implemented yet...!!");

	/*EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED */
}

//=================================
// [4] FIND PA OF GIVEN VA:
//=================================
unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT'25.GM#2] KERNEL HEAP - #4 kheap_physical_address
	//Your code is here
	//Comment the following line
	panic("kheap_physical_address() is not implemented yet...!!");

	/*EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED */
}

//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

extern __inline__ uint32 get_block_size(void *va);

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT'25.BONUS#2] KERNEL REALLOC - krealloc
	//Your code is here
	//Comment the following line
	panic("krealloc() is not implemented yet...!!");
}
