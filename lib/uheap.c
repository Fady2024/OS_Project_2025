#include <inc/lib.h>
//=============MY METHODS AND DS=======================
struct alloc {
    void* va;
    int32 size;
};
struct alloc allocs[NUM_OF_UHEAP_PAGES];

// ============METHODS==============
int get_idx(uint32 i){
	return (i-uheapPageAllocStart)/PAGE_SIZE;
}
//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//==============================================
// [1] INITIALIZE USER HEAP:
//==============================================
int __firstTimeFlag = 1;
void uheap_init()
{
	if(__firstTimeFlag)
	{
		initialize_dynamic_allocator(USER_HEAP_START, USER_HEAP_START + DYN_ALLOC_MAX_SIZE);
		uheapPlaceStrategy = sys_get_uheap_strategy();
		uheapPageAllocStart = dynAllocEnd + PAGE_SIZE;
		uheapPageAllocBreak = uheapPageAllocStart;
		__firstTimeFlag = 0;
	}

}

//==============================================
// [2] GET A PAGE FROM THE KERNEL FOR DA:
//==============================================
int get_page(void* va)
{
#if USE_KHEAP
	int ret = __sys_allocate_page(ROUNDDOWN(va, PAGE_SIZE), PERM_USER|PERM_WRITEABLE|PERM_UHPAGE);
	if (ret < 0)
		panic("get_page() in user: failed to allocate page from the kernel");
	return 0;
#else
	panic("get_page() requires USE_KHEAP=1");
	return -1;
#endif
}

//==============================================
// [3] RETURN A PAGE FROM THE DA TO KERNEL:
//==============================================
void return_page(void* va)
{
#if USE_KHEAP
	int ret = __sys_unmap_frame(ROUNDDOWN((uint32)va, PAGE_SIZE));
	if (ret < 0)
		panic("return_page() in user: failed to return a page to the kernel");
#endif
}

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//=================================
// [1] ALLOCATE SPACE IN USER HEAP:
//=================================
void* malloc(uint32 size)
{
#if USE_KHEAP
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	uheap_init();
	if (size == 0) return NULL ;
	//==============================================================
	//TODO: [PROJECT'25.IM#2] USER HEAP - #1 malloc
	//Your code is here
	//Comment the following line
	//panic("malloc() is not implemented yet...!!");
	if(size <=DYN_ALLOC_MAX_BLOCK_SIZE){
		return alloc_block(size);
	}

	uint32 num_pages= (size + PAGE_SIZE-1)/PAGE_SIZE , pages_size = num_pages*PAGE_SIZE;
	uint32 start = 0,end = 0;
	uint32 exact_s = 0,exact_e=0,worst_s=0,worst_e=0;
	for(uint32 i = uheapPageAllocStart; i <uheapPageAllocBreak;i+=PAGE_SIZE){
		int idx = get_idx(i);
		if(allocs[idx].va == NULL){
			end = i+PAGE_SIZE;
			if(start == 0) start = i;
		}
		else {
			if(end - start >=pages_size){
				//exact fit
				if(end - start == pages_size) {
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
	else if(worst_s !=0) page_s = worst_s,page_e=worst_s + pages_size;
	// !exact and !worst
	else{
		if(uheapPageAllocBreak <= USER_HEAP_MAX - pages_size){
			page_s = uheapPageAllocBreak;
			page_e = uheapPageAllocBreak  + pages_size;
			uheapPageAllocBreak = page_e;
		}
		else {
			return NULL;
		}

	}
	for(uint32 i = page_s; i <page_e;i+=PAGE_SIZE){
		uint32 idx = get_idx(i);
		allocs[idx].va = (void*)page_s;
		allocs[idx].size = pages_size;
	}
	sys_allocate_user_mem(page_s,pages_size);
	return (void*)page_s;
#else
	panic("malloc() requires USE_KHEAP=1");
	return NULL;
#endif
}

//=================================
// [2] FREE SPACE FROM USER HEAP:
//=================================
void free(void* virtual_address)
{
#if USE_KHEAP
	//TODO: [PROJECT'25.IM#2] USER HEAP - #3 free
	//Your code is here
	//Comment the following line
	//panic("free() is not implemented yet...!!");
	uint32 va = (uint32)virtual_address;
	if ( va >= dynAllocStart && va <= dynAllocEnd ){
		return free_block(virtual_address);
	}
	uint32 idx = get_idx(va);
	uint32 size = allocs[idx].size;

	virtual_address = allocs[idx].va;
	uint32 end = (int32)virtual_address + size;
	for(uint32 i = (uint32)virtual_address;i<end; i+=PAGE_SIZE){
		idx = get_idx(i);
		allocs[idx].size = 0;
		allocs[idx].va = NULL;
	}
	idx = get_idx(end);
	if(uheapPageAllocBreak == end){
		uheapPageAllocBreak-=size;
		while(uheapPageAllocBreak > uheapPageAllocStart && allocs[get_idx(uheapPageAllocBreak-PAGE_SIZE)].va == NULL)
			uheapPageAllocBreak-=PAGE_SIZE;
	}


	sys_free_user_mem((uint32)virtual_address,size);
#endif
}

//=================================
// [3] ALLOCATE SHARED VARIABLE:
//=================================
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
#if USE_KHEAP
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	uheap_init();
	if (size == 0) return NULL;
	//==============================================================

	//TODO: [PROJECT'25.IM#3] SHARED MEMORY - #2 smalloc
	//Your code is here

	//Comment the following line
	//panic("smalloc() is not implemented yet...!!");
	uint32 num_pages= (size + PAGE_SIZE-1)/PAGE_SIZE , pages_size = num_pages*PAGE_SIZE;
    uint32 start = 0, end = 0;
    uint32 exact_s = 0, worst_s = 0, worst_e = 0;
    for (uint32 i = uheapPageAllocStart; i < uheapPageAllocBreak; i += PAGE_SIZE) {
        uint32 idx = get_idx(i);
        if (allocs[idx].va == NULL) {
            end = i + PAGE_SIZE;
            if (start == 0) start = i;
        } else {
            if (end - start >= pages_size) {
                if (end - start == pages_size){
                    exact_s = start;
                    break;
                }
                else if ((end - start) > (worst_e - worst_s)) {
                    worst_s = start;
                    worst_e = end;
                }
            }
            start = 0;
            end = 0;
        }
    }
    if (start != 0 && (end - start) >= pages_size) {
        if (end - start == pages_size)
            exact_s = start;
        else if ((end - start) > (worst_e - worst_s)) {
            worst_s = start;
            worst_e = end;
        }
    }

    uint32 page_s = 0;
    if (exact_s != 0)
        page_s = exact_s;
    else if (worst_s != 0)
        page_s = worst_s;
    else {
		if (uheapPageAllocBreak + pages_size > USER_HEAP_MAX) {
            return NULL;
        }
        page_s = uheapPageAllocBreak;
        uheapPageAllocBreak += pages_size;
    }

    uint32 page_e = page_s + pages_size;
    for (uint32 i = page_s; i < page_e; i += PAGE_SIZE) {
        uint32 idx = get_idx(i);
        allocs[idx].va = (void*)page_s;
        allocs[idx].size = pages_size;
    }

    int id = sys_create_shared_object(sharedVarName, size, isWritable, (void*)page_s);
    if (id < 0)
        return NULL;
    return (void*)page_s;
#else
	panic("smalloc() requires USE_KHEAP=1");
	return NULL;
#endif
}

//========================================
// [4] SHARE ON ALLOCATED SHARED VARIABLE:
//========================================
void* sget(int32 ownerEnvID, char *sharedVarName)
{
#if USE_KHEAP
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	uheap_init();
	//==============================================================

	//TODO: [PROJECT'25.IM#3] SHARED MEMORY - #4 sget
	//Your code is here
	//Comment the following line
	//panic("sget() is not implemented yet...!!");
    int size = sys_size_of_shared_object(ownerEnvID, sharedVarName);
    if (size <= 0)
        return NULL;
	uint32 num_pages= (size + PAGE_SIZE-1)/PAGE_SIZE , pages_size = num_pages*PAGE_SIZE;
    uint32 start = 0, end = 0;
    uint32 exact_s = 0, worst_s = 0, worst_e = 0;
    for (uint32 i = uheapPageAllocStart; i < uheapPageAllocBreak; i += PAGE_SIZE) {
        int idx = get_idx(i);
        if (allocs[idx].va == NULL) {
            end = i + PAGE_SIZE;
            if (start == 0) start = i;
        } else {
            if (end - start >= pages_size) {
                if (end - start == pages_size)
                    exact_s = start;
                else if ((end - start) > (worst_e - worst_s)) {
                    worst_s = start;
                    worst_e = end;
                }
            }
            start = 0;
            end = 0;
        }
    }
    if (start != 0 && (end - start) >= pages_size) {
        if (end - start == pages_size)
            exact_s = start;
        else if ((end - start) > (worst_e - worst_s)) {
            worst_s = start;
            worst_e = end;
        }
    }
    uint32 page_s = 0;
    if (exact_s != 0)
        page_s = exact_s;
    else if (worst_s != 0)
        page_s = worst_s;
    else {
		if (uheapPageAllocBreak + pages_size > USER_HEAP_MAX) {
            return NULL;
        }
        page_s = uheapPageAllocBreak;
        uheapPageAllocBreak += pages_size;
    }
    uint32 page_e = page_s + pages_size;
    for (uint32 i = page_s; i < page_e; i += PAGE_SIZE) {
        uint32 idx = get_idx(i);
        allocs[idx].va = (void*)page_s;
        allocs[idx].size = pages_size;
    }
    int id = sys_get_shared_object(ownerEnvID, sharedVarName, (void*)page_s);
    if (id < 0)
        return NULL;
    return (void*)page_s;
#else
	panic("sget() requires USE_KHEAP=1");
	return NULL;
#endif
}


//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//


//=================================
// REALLOC USER SPACE:
//=================================
//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_move_user_mem(...)
//		which switches to the kernel mode, calls move_user_mem(...)
//		in "kern/mem/chunk_operations.c", then switch back to the user mode here
//	the move_user_mem() function is empty, make sure to implement it.
void *realloc(void *virtual_address, uint32 new_size)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	uheap_init();
	//==============================================================
	panic("realloc() is not implemented yet...!!");
}


//=================================
// FREE SHARED VARIABLE:
//=================================
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_delete_shared_object(...); which switches to the kernel mode,
//	calls delete_shared_object(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the delete_shared_object() function is empty, make sure to implement it.
void sfree(void* virtual_address)
{
	//TODO: [PROJECT'25.BONUS#5] EXIT #2 - sfree
	//Your code is here
	//Comment the following line
	panic("sfree() is not implemented yet...!!");

	//	1) you should find the ID of the shared variable at the given address
	//	2) you need to call sys_freeSharedObject()
}


//==================================================================================//
//========================== MODIFICATION FUNCTIONS ================================//
//==================================================================================//
