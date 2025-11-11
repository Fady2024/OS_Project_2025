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
int is_free(uint32 x){
	int perm  = pt_get_page_permissions(ptr_page_directory,x);
	if(perm & PERM_PRESENT) return 0;
	return 1;
}

// my st
struct alloc allocs[NUM_OF_KHEAP_PAGES];


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
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");
	if(size <=DYN_ALLOC_MAX_BLOCK_SIZE)return alloc_block(size);
	int num_pages = (size+PAGE_SIZE-1)/PAGE_SIZE , page_size = num_pages * PAGE_SIZE;

	uint32 start = 0,end = 0;
	uint32 exact_s = 0,exact_e=0,worst_s=0,worst_e=0;
	for(uint32 i = kheapPageAllocStart; i <kheapPageAllocBreak;i+=PAGE_SIZE){
		if(is_free(i)){
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
//		cprintf("ana: %u \n",kheapPageAllocBreak);
		if(kheapPageAllocBreak <= KERNEL_HEAP_MAX - page_size){

			page_s = kheapPageAllocBreak;
			page_e = kheapPageAllocBreak  + page_size;
//			cprintf("ps: %u - pe: %u  - size: %u \n",page_s,page_e,page_size);
			kheapPageAllocBreak = page_e;
//			cprintf("ana2: %u \n",kheapPageAllocBreak);
		}
		else return NULL;

	}
	for(uint32 i = page_s; i <page_e;i+=PAGE_SIZE){
		uint32 idx = (i  - kheapPageAllocStart)/PAGE_SIZE;
		allocs[idx].va = (void*)page_s;
		allocs[idx].size = page_size;
		get_page((void*)i);
	}
//	cprintf("k2: %u \n",kheapPageAllocBreak);
	return (void*)page_s;



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
	//panic("kfree() is not implemented yet...!!");
	//cprintf("%u\n", (KERNEL_HEAP_MAX - kheapPageAllocStart) / PAGE_SIZE); // 32766
	if( (uint32)virtual_address < kheapPageAllocStart)return free_block(virtual_address);

	uint32 idx = (((uint32)virtual_address) - kheapPageAllocStart)/PAGE_SIZE;
	if(allocs[idx].va == NULL){
		panic("kfree() in kern: Invalid Address\n");
	}
	uint32 page_start = (uint32)allocs[idx].va;

	uint32 sz = allocs[idx].size;

	uint32 page_end = page_start + sz;

	for(uint32 i = page_start;i<page_end;i+=PAGE_SIZE){
		uint32 idx = (i  - kheapPageAllocStart)/PAGE_SIZE;
		allocs[idx].va = NULL;
		allocs[idx].size = 0;
		return_page((void*)i);
	}

	if(kheapPageAllocBreak == page_end){
		kheapPageAllocBreak-=sz;
		while(kheapPageAllocBreak > kheapPageAllocStart && is_free(kheapPageAllocBreak-PAGE_SIZE))kheapPageAllocBreak-=PAGE_SIZE;
	}

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
	//panic("krealloc() is not implemented yet...!!");

	// ALLOCATE NEW SPACE IN KERNEL
	if(virtual_address == NULL) return kmalloc(new_size);

	// FREE SPACE FROM KERNEL
	if(new_size == 0){
		kfree(virtual_address);
		return NULL;
	}

	// IF THE NEW SIZE NO MORE NEEDS PAGES
	if(new_size <= DYN_ALLOC_MAX_BLOCK_SIZE)return realloc_block(virtual_address,new_size);


	uint32 va = (uint32)virtual_address;
	int32 idx = (va  - kheapPageAllocStart)/PAGE_SIZE;
	va = (uint32)allocs[idx].va;

	int32 old_pages_size = allocs[idx].size;
	int num_pages = (new_size+PAGE_SIZE-1)/PAGE_SIZE , new_pages_size = num_pages * PAGE_SIZE;




	// IF THE NEW SIZE < OLD SIZE
	if(new_pages_size < old_pages_size){

		int32 start = va + new_pages_size;
		int32 end = va + old_pages_size;

		for(int i = start ; i<end ; i+=PAGE_SIZE){
			uint32 id = (i  - kheapPageAllocStart)/PAGE_SIZE;
			allocs[id].va = NULL;
			allocs[id].size = 0;
			return_page((void*)i);
		}
		return virtual_address;

	}

	// Extend UP
	bool can_extend =1;
	int n_start = va + old_pages_size;
	int n_end = va + new_pages_size;
	int mx_end = n_start;
	if(n_end > KERNEL_HEAP_MAX)can_extend = 0;
	else{
		for(int i = n_start;i<n_end;i+=PAGE_SIZE)
			if(!is_free(i)){mx_end = i - PAGE_SIZE;
				can_extend = 0;break;}
	}
	if(can_extend){
		for(int i = n_start;i<n_end;i+=PAGE_SIZE){
			uint32 idx= (i  - kheapPageAllocStart)/PAGE_SIZE;
			allocs[idx].va = (void*)va;
			allocs[idx].size = new_pages_size;
			get_page((void*)i);
		}
		if(n_end > kheapPageAllocBreak)kheapPageAllocBreak = n_end;
		return (void*) va;
	}

	//EXTEND DOWN
	can_extend = 1;
	n_start = mx_end - new_pages_size;
	for(int i = n_start; i<mx_end; i+=PAGE_SIZE){
		if(!is_free(i) && allocs[(i  - kheapPageAllocStart)/PAGE_SIZE].va !=(void*)va){
			can_extend = 0;
		}
	}
	if(can_extend){
		for(int i = n_start; i<mx_end;i+=PAGE_SIZE){
			uint32 idx= (i  - kheapPageAllocStart)/PAGE_SIZE;
			allocs[idx].va = (void*)n_start;
			allocs[idx].size = new_pages_size;
			if(!is_free(i)) get_page((void*)i);
		}
		return (void*)n_start;
	}

	// CAN`T EXTEND
	void* n_va = kmalloc(new_size);
	if(n_va == NULL) return NULL;
	for(int i = va ; i <va + old_pages_size;i+=PAGE_SIZE){
		uint32 idx= (i  - kheapPageAllocStart)/PAGE_SIZE;
		allocs[idx].va = NULL;
		allocs[idx].size = 0;
		return_page((void*)i);
	}
	return n_va;

}
