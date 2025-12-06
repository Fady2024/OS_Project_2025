/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//
//==================================
//==================================
// [1] GET PAGE VA:
//==================================
__inline__ uint32 to_page_va(struct PageInfoElement *ptrPageInfo)
{
	if (ptrPageInfo < &pageBlockInfoArr[0] || ptrPageInfo >= &pageBlockInfoArr[DYN_ALLOC_MAX_SIZE/PAGE_SIZE])
			panic("to_page_va called with invalid pageInfoPtr");
	// Get start VA of the page from the corresponding Page Info pointer
	int idxInPageInfoArr = (ptrPageInfo - pageBlockInfoArr);
	return dynAllocStart + (idxInPageInfoArr << PGSHIFT);
}

//==================================
// [2] GET PAGE INFO OF PAGE VA:
//==================================
__inline__ struct PageInfoElement * to_page_info(uint32 va)
{
	int idxInPageInfoArr = (va - dynAllocStart) >> PGSHIFT;
	if (idxInPageInfoArr < 0 || idxInPageInfoArr >= DYN_ALLOC_MAX_SIZE/PAGE_SIZE)
		panic("to_page_info called with invalid pa");
	return &pageBlockInfoArr[idxInPageInfoArr];
}


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
bool is_initialized = 0;
void initialize_dynamic_allocator(uint32 daStart, uint32 daEnd)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		assert(daEnd <= daStart + DYN_ALLOC_MAX_SIZE);
		is_initialized = 1;
	}
	//==================================================================================
	//==================================================================================
	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #1 initialize_dynamic_allocator
	//Your code is here
	//Comment the following line
	//	panic("initialize_dynamic_allocator() Not implemented yet");

	dynAllocStart = daStart;
	dynAllocEnd = daEnd;

	LIST_INIT(&freePagesList);

	for (uint32 current_va = daStart; current_va < daEnd; current_va += PAGE_SIZE) {
		 struct PageInfoElement *page = to_page_info(current_va);
		 page->block_size = 0;
		 page->num_of_free_blocks = 0;

		 LIST_INSERT_TAIL(&freePagesList, page);
	}

	for (uint16 i = 0; i < NUM_OF_BLOCK_SIZES; ++i) {
		LIST_INIT(&freeBlockLists[i]);
	}

}

//===========================
// [2] GET BLOCK SIZE:
//===========================
__inline__ uint32 get_block_size(void *va)
{
	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #2 get_block_size
	//Your code is here
	//Comment the following line
	//	panic("get_block_size() Not implemented yet");

	return to_page_info((uint32)va)->block_size;
}

//===========================
// 3) ALLOCATE BLOCK:
//===========================
void *alloc_block(uint32 size)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		assert(size <= DYN_ALLOC_MAX_BLOCK_SIZE);
	}
	//==================================================================================
	//==================================================================================
	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #3 alloc_block
	//Your code is here
	//Comment the following line
	//	panic("alloc_block() Not implemented yet");

	if (size == 0) {
		return NULL;
	}

	uint16 block_size = DYN_ALLOC_MIN_BLOCK_SIZE, idx = 0;
	while (block_size < size) {
		block_size <<= 1;
		++idx;
	}

	// Found Free Block
	if (LIST_SIZE(&freeBlockLists[idx]) > 0) {
		struct BlockElement *block = LIST_FIRST(&freeBlockLists[idx]);

		struct PageInfoElement *page = to_page_info((uint32)block);
		page->num_of_free_blocks--;

		LIST_REMOVE(&freeBlockLists[idx], block);
		return (void *)block;
	}

	// Found Free Page
	if (LIST_SIZE(&freePagesList) > 0) {
		struct PageInfoElement *page = LIST_FIRST(&freePagesList);

		uint32 current_va = to_page_va(page);
		get_page((void *)current_va); // Allocate The Page In Physical Memory

		uint16 num_of_free = PAGE_SIZE / block_size;

		page->block_size = block_size;
		page->num_of_free_blocks = num_of_free;

		for (uint16 i = 0; i < num_of_free; ++i) {
			struct BlockElement *block = (struct BlockElement *)current_va;
			LIST_INSERT_TAIL(&freeBlockLists[idx], block);

			current_va += block_size;
		}

		LIST_REMOVE(&freePagesList, page);
	}

	// Try to match any size >= block_size
	while (idx < NUM_OF_BLOCK_SIZES) {

		// Found Free Block
		if (LIST_SIZE(&freeBlockLists[idx]) > 0) {
			struct BlockElement *block = LIST_FIRST(&freeBlockLists[idx]);

			struct PageInfoElement *page = to_page_info((uint32)block);
			page->num_of_free_blocks--;

			LIST_REMOVE(&freeBlockLists[idx], block);
			return (void *)block;
		}

		++idx;
	}

	// No Free Blocks
	panic("alloc_block Cannot Find Free Blocks");

	//TODO: [PROJECT'25.BONUS#1] DYNAMIC ALLOCATOR - block if no free block
}

//===========================
// [4] FREE BLOCK:
//===========================
void free_block(void *va)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		assert((uint32)va >= dynAllocStart && (uint32)va < dynAllocEnd);
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'25.GM#1] DYNAMIC ALLOCATOR - #4 free_block
	//Your code is here
	//Comment the following line
//		panic("free_block() Not implemented yet");


	uint32 address = (uint32)va;

	struct PageInfoElement *page = to_page_info(address);

	int idx = 0;
	for (int i = 0; i < NUM_OF_BLOCK_SIZES; ++i) {
		if ((1 << (i + 3)) == page->block_size) // (+3 for starting from 8)
		{
			idx = i;
			break;
		}
	}

	struct BlockElement *block = (struct BlockElement*)address;
	page->num_of_free_blocks++;
	LIST_INSERT_TAIL(&freeBlockLists[idx], block);

	// Free The Page
	if (page->num_of_free_blocks == PAGE_SIZE / page->block_size) {
		uint32 current_va = to_page_va(page);

		for (int i = 0; i < page->num_of_free_blocks; ++i) {
			struct BlockElement *block = (struct BlockElement*)current_va;
			LIST_REMOVE(&freeBlockLists[idx], block);

			current_va += page->block_size;
		}

		page->num_of_free_blocks = 0;
		page->block_size = 0;

		return_page((void*)to_page_va(page));
		LIST_INSERT_TAIL(&freePagesList, page);
	}

}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//===========================
// [1] REALLOCATE BLOCK:
//===========================
void *realloc_block(void* va, uint32 new_size)
{
	//TODO: [PROJECT'25.BONUS#2] KERNEL REALLOC - realloc_block
	//Your code is here
	//Comment the following line
	panic("realloc_block() Not implemented yet");



}
