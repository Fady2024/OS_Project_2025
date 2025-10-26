/*
 * test_lists_managment.c

 *
 *  Created on: Oct 6, 2022
 *  Updated on: Sept 20, 2023
 *      Author: HP
 */
#include <inc/queue.h>
#include <inc/string.h>
#include <inc/assert.h>
#include <inc/dynamic_allocator.h>
#include <inc/memlayout.h>

//NOTE: ALL tests in this file shall work with USE_KHEAP = 0

/***********************************************************************************************************************/
#define Mega  (1024*1024)
#define kilo (1024)
#define numOfAllocs 7
#define allocCntPerSize 200
#define sizeOfMetaData 8

//NOTE: these sizes include the size of MetaData within it
uint32 allocSizes[numOfAllocs] = {4*kilo, 20*sizeof(char) + sizeOfMetaData, 1*kilo, 3*sizeof(int) + sizeOfMetaData, 2*kilo, 2*sizeOfMetaData, 7*kilo} ;
short* startVAs[numOfAllocs*allocCntPerSize+1] ;
short* midVAs[numOfAllocs*allocCntPerSize+1] ;
short* endVAs[numOfAllocs*allocCntPerSize+1] ;

short* startVAsInit[DYN_ALLOC_MAX_BLOCK_SIZE + 1] ;
short* endVAsInit[DYN_ALLOC_MAX_BLOCK_SIZE + 1] ;

__inline__ uint8 IDX(uint32 size)
{
	size>>= LOG2_MIN_SIZE;
	int index = 0;
	while ((size>>=1) != 0)
	{
		index++;
	}
	return index;
}

int check_dynalloc_datastruct(uint32 curSize, uint32 numOfBlksAtCurSize)
{
	int maxNumOfBlksPerPage = PAGE_SIZE / curSize;
	int expectedNumOfCompletePages = numOfBlksAtCurSize / maxNumOfBlksPerPage;
	int expectedNumOfInCompletePages = numOfBlksAtCurSize % maxNumOfBlksPerPage != 0? 1 : 0;
	int expectedNumOfFreeBlks = expectedNumOfInCompletePages * (maxNumOfBlksPerPage - numOfBlksAtCurSize % maxNumOfBlksPerPage);

	//[1] Check PageBlkInfoArr
	int numOfPages = DYN_ALLOC_MAX_SIZE / PAGE_SIZE;
	int actualNumOfCompletePages = 0;
	int actualNumOfInCompletePages = 0;
	int actualNumOfFreeBlks = 0;
	for (int i = 0; i < numOfPages; ++i)
	{
		if (pageBlockInfoArr[i].block_size == curSize)
		{
			if (pageBlockInfoArr[i].num_of_free_blocks == 0)
			{
				actualNumOfCompletePages++;
			}
			else
			{
				actualNumOfInCompletePages++;
				actualNumOfFreeBlks += pageBlockInfoArr[i].num_of_free_blocks;
			}
		}
	}
	if (actualNumOfCompletePages != expectedNumOfCompletePages ||
			actualNumOfInCompletePages != expectedNumOfInCompletePages ||
			actualNumOfFreeBlks != expectedNumOfFreeBlks)
	{
		cprintf_colored(TEXT_TESTERR_CLR, "PageBlkInfoArr is not set/updated correctly!\n");
		//		cprintf("actualNumOfCompletePages = %d, expectedNumOfCompletePages = %d\n", actualNumOfCompletePages, expectedNumOfCompletePages);
		//		cprintf("actualNumOfInCompletePages = %d, expectedNumOfInCompletePages = %d\n", actualNumOfInCompletePages, expectedNumOfInCompletePages);
		//		cprintf("actualNumOfFreeBlks = %d, expectedNumOfFreeBlks = %d\n", actualNumOfFreeBlks, expectedNumOfFreeBlks);
		return 0;
	}

	//[2] Check freeBlkLists
	int index = IDX(curSize);
	struct BlockElement_List *ptrList = &freeBlockLists[index];
	int n = 0;
	struct BlockElement *ptrBlk;
	LIST_FOREACH(ptrBlk, ptrList)
	{
		n++;
	}
	if (LIST_SIZE(ptrList) != expectedNumOfFreeBlks || n != expectedNumOfFreeBlks)
	{
		cprintf_colored(TEXT_TESTERR_CLR,"freeBlockLists[%d] is not updated correctly!", index);
		return 0;
	}
	return 1;
}
int check_list_size(uint32 expectedListSize)
{
	panic("update is required!!");
	//	if (LIST_SIZE(&freeBlocksList) != expectedListSize)
	{
		//		cprintf("freeBlocksList: wrong size! expected %d, actual %d\n", expectedListSize, LIST_SIZE(&freeBlocksList));
		return 0;
	}
	return 1;
}

extern uint32* ptr_page_directory;
extern void unmap_frame(uint32 *ptr_page_directory, uint32 virtual_address);
extern uint32 sys_calculate_free_frames() ;

void remove_current_mappings(uint32 startVA, uint32 endVA)
{
	assert(startVA >= KERNEL_HEAP_START && endVA >= KERNEL_HEAP_START);
	for (uint32 va = startVA; va < endVA; va+=PAGE_SIZE)
	{
		unmap_frame(ptr_page_directory, va);
	}
}
/***********************************************************************************************************************/

void test_initialize_dynamic_allocator()
{
#if USE_KHEAP
	panic("test_initialize_dynamic_allocator: the kernel heap should be disabled. make sure USE_KHEAP = 0");
	return ;
#endif

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run ANYTHING before or after it)\n");
	cprintf("==============================================\n");

	initialize_dynamic_allocator(KERNEL_HEAP_START - DYN_ALLOC_MAX_SIZE, KERNEL_HEAP_START);

	//Check#1: Limits
	if (dynAllocStart != KERNEL_HEAP_START  - DYN_ALLOC_MAX_SIZE || dynAllocEnd != KERNEL_HEAP_START )
	{
		panic("DA limits are not set correctly");
	}
	//Check#2: PageBlockInfoArr
	int numOfPages = DYN_ALLOC_MAX_SIZE / PAGE_SIZE;
	for (int i = 0; i < numOfPages; ++i)
	{
		if (pageBlockInfoArr[i].block_size || pageBlockInfoArr[i].num_of_free_blocks)
		{
			panic("DA pageBlockInfoArr are not initialized correctly");
		}
	}
	//Check#3: freePagesList
	int n = 0;
	struct PageInfoElement *ptrPI;
	LIST_FOREACH(ptrPI, &freePagesList)
	{
		if (ptrPI != &pageBlockInfoArr[n])
		{
			panic("DA freePagesList is not initialized correctly! Make sure that the pages are added to the list in their correct order");
		}
		n++;
	}
	if (LIST_SIZE(&freePagesList) != numOfPages || n != numOfPages)
	{
		panic("DA freePagesList is not initialized correctly! one or more pages are not added correctly");
	}
	//Check#4: freeBlockLists
	int numOfSizes = LOG2_MAX_SIZE - LOG2_MIN_SIZE + 1;
	for (int i = 0; i < numOfSizes; ++i)
	{
		struct BlockElement_List *ptrList = &freeBlockLists[i];
		if (LIST_SIZE(ptrList) || LIST_FIRST(ptrList) || LIST_LAST(ptrList))
		{
			panic("DA freeBlockLists[%d] is not initialized correctly!", i);
		}
	}

	cprintf("============================================================================="
			"\nCongratulations!! test initialize_dynamic_allocator completed successfully.\n"
			"=============================================================================\n");
}

int test_initial_alloc()
{
#if USE_KHEAP
	panic("test_initial_alloc: the kernel heap should be disabled. make sure USE_KHEAP = 0");
	return 0;
#endif

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run ANYTHING before or after it)\n");
	cprintf("==============================================\n");

	//Remove the current 1-to-1 mapping of the KERNEL HEAP area since the USE_KHEAP = 0 for this test
	uint32 startDA = KERNEL_HEAP_START ;
	uint32 sizeDA = 0x2AE000 ;
	uint32 endDA = KERNEL_HEAP_START + sizeDA ;
	remove_current_mappings(startDA, endDA);
	initialize_dynamic_allocator(startDA, endDA);

	int eval = 0;
	bool is_correct = 1;

	int freeFramesBefore = sys_calculate_free_frames();
	void *va ;
	//====================================================================//
	/*INITIAL ALLOC Scenario 1: Allocate set of blocks for each possible block size (consume entire space)*/
	cprintf("	1: Allocate set of blocks for each possible block size (consume entire space)\n\n") ;
	int curSize = 1<<LOG2_MIN_SIZE ;
	int numOfBlksAtCurSize = 0;
	int maxNumOfBlksAtCurPage = PAGE_SIZE / curSize;
	uint32 expectedVA = KERNEL_HEAP_START;
	for (int s = 1; s <= DYN_ALLOC_MAX_BLOCK_SIZE; ++s)
	{
		va = alloc_block(s);
		startVAsInit[s] = va; *startVAsInit[s] = s ;
		endVAsInit[s] = va + curSize - sizeof(short); *endVAsInit[s] = s ;

		numOfBlksAtCurSize++;
		if (is_correct && ROUNDDOWN((uint32)va, PAGE_SIZE) != expectedVA)
		{
			is_correct = 0;
			cprintf_colored(TEXT_TESTERR_CLR, "alloc_block test#1.%d: WRONG! VA is not correct\n", s);
		}
		if (s == curSize)
		{
			//apply the following check only on the 1st three levels
			if (curSize <= 32)
			{
				if (is_correct)	eval += 5;
				is_correct = 1;
			}
			if (check_dynalloc_datastruct(curSize, numOfBlksAtCurSize) == 0)
			{
				is_correct = 0;
				cprintf_colored(TEXT_TESTERR_CLR, "alloc_block test#2.%d: WRONG! DA data structures are not correct\n", s);
			}
			if (is_correct)	eval += 5;
			//Reinitialize
			{
				curSize <<= 1;
				expectedVA += PAGE_SIZE;
				numOfBlksAtCurSize = 0;
				maxNumOfBlksAtCurPage = PAGE_SIZE / curSize;
				is_correct = 1;
			}
		}
		else if (numOfBlksAtCurSize % maxNumOfBlksAtCurPage == 0)
		{
			expectedVA += PAGE_SIZE;
		}

	}

	//====================================================================//
	/*INITIAL ALLOC Scenario 2: Allocate blocks of same size that consume remaining free blocks at all levels*/
	cprintf("	2: Allocate blocks of same size that consume remaining free blocks at all levels\n\n") ;
	is_correct = 1;
	//calculate expected number of free blocks at all levels
	int size1 = 0;
	int size2 = DYN_ALLOC_MIN_BLOCK_SIZE;
#define numOfLevels (LOG2_MAX_SIZE - LOG2_MIN_SIZE + 1)
	int numOfRemFreeBlks[numOfLevels];
	uint32 expectedPageIndex[numOfLevels];
	uint32 prevAllocPages = 0;
	int idx = 0;
	while (size1 < DYN_ALLOC_MAX_BLOCK_SIZE)
	{
		int numOfAllocBlks = size2 - size1 ;
		int expectedNumOfBlksPerPage = PAGE_SIZE / size2;
		if (numOfAllocBlks % expectedNumOfBlksPerPage != 0)
		{
			numOfRemFreeBlks[idx] = expectedNumOfBlksPerPage - (numOfAllocBlks % expectedNumOfBlksPerPage) ;
			expectedPageIndex[idx] = (prevAllocPages + numOfAllocBlks / expectedNumOfBlksPerPage) ;
			prevAllocPages += numOfAllocBlks / expectedNumOfBlksPerPage + 1;
		}
		else
		{
			numOfRemFreeBlks[idx] = 0;
			expectedPageIndex[idx] = 0;
			prevAllocPages += numOfAllocBlks / expectedNumOfBlksPerPage;
		}
		size1 = size2 ;
		size2 *= 2 ;
		idx++;
	}

	//Allocate a number of blocks of same size to consume all the remaining free blocks
	int blkSize = 1<<LOG2_MIN_SIZE ;
	for (int i = 0; i < numOfLevels; ++i)
	{
		uint32 expectedVA = KERNEL_HEAP_START + expectedPageIndex[i] * PAGE_SIZE;
		is_correct = 1;
		cprintf( "Level#%d: num of remaining free blocks = %d\n", i, numOfRemFreeBlks[i]);
		for (int j = 0; j < numOfRemFreeBlks[i]; ++j)
		{
			va = alloc_block(blkSize);
			int *tmpVal = va ;
			*tmpVal = 353 ;
			if (ROUNDDOWN((uint32)va, PAGE_SIZE) != expectedVA)
			{
				is_correct = 0;
				cprintf_colored(TEXT_TESTERR_CLR, "alloc_block test#3: WRONG! VA is not correct (i = %d, j = %d)\n", i, j);
				break;
			}
			if (*tmpVal != 353)
			{
				is_correct = 0;
				cprintf_colored(TEXT_TESTERR_CLR, "alloc_block test#4: wrong stored value in the allocated block\n");
			}
		}

		if (numOfRemFreeBlks[i] > 0)
		{
			if (LIST_SIZE(&freeBlockLists[i]) != 0)
			{
				is_correct = 0;
				cprintf_colored(TEXT_TESTERR_CLR, "alloc_block test#5: WRONG! there's still free blocks at level %d while not expected to\n", i);
			}
			if (pageBlockInfoArr[expectedPageIndex[i]].num_of_free_blocks != 0)
			{
				is_correct = 0;
				cprintf_colored(TEXT_TESTERR_CLR, "alloc_block test#6: WRONG! there's still free blocks at page %d while not expected to\n", expectedPageIndex[i]);
			}
			if (is_correct)	eval += 5;
		}
	}

	//====================================================================//
	/*INITIAL ALLOC Scenario 3: Check stored data inside each allocated block*/
	cprintf("	3: Check stored data inside each allocated block\n\n") ;
	is_correct = 1;

	for (int s = 1; s <= DYN_ALLOC_MAX_BLOCK_SIZE; ++s)
	{
		if (*(startVAsInit[s]) != s || *(endVAsInit[s]) != s)
		{
			is_correct = 0;
			cprintf_colored(TEXT_TESTERR_CLR, "alloc_block #7.%d: WRONG! content of the block is not correct. Expected %d\n",s, s);
			break;
		}
	}
	if (is_correct)
	{
		eval += 10;
	}
	//====================================================================//
	/*INITIAL ALLOC Scenario 4: Check allocated frames*/
	cprintf("	4: Check allocated frames\n\n") ;
	is_correct = 1;
	int freeFramesAfter = sys_calculate_free_frames();
	int expectedNumOfAllocPages = 686;
	if (freeFramesBefore - freeFramesAfter != expectedNumOfAllocPages)
	{
		is_correct = 0;
		cprintf_colored(TEXT_TESTERR_CLR, "alloc_block #8: WRONG! number of allocated frames is not as expected. Actual: %d, Expected: %d\n",freeFramesBefore - freeFramesAfter , expectedNumOfAllocPages);
	}
	if (is_correct)
	{
		eval += 10;
	}

	return eval;
}

void test_alloc_block()
{
#if USE_KHEAP
	panic("test_alloc_block: the kernel heap should be disabled. make sure USE_KHEAP = 0");
	return;
#endif

	int eval = 0;
	bool is_correct;
	void* va = NULL;
	uint32 actualSize = 0;

	cprintf("=======================================================\n") ;
	cprintf("FIRST: Tests depend on the Allocate Function ONLY [40%]\n") ;
	cprintf("=======================================================\n") ;
	eval = test_initial_alloc();


	cprintf("test alloc_block Evaluation = %d%\n", eval);
	return ;
}

void test_free_block()
{
	panic("update is required!!");

#if USE_KHEAP
	panic("test_free_block: the kernel heap should be disabled. make sure USE_KHEAP = 0");
	return;
#endif

	cprintf("===========================================================\n") ;
	cprintf("NOTE: THIS TEST IS DEPEND ON BOTH ALLOCATE & FREE FUNCTIONS\n") ;
	cprintf("===========================================================\n") ;
	void*expected_va ;
}

void test_realloc_block()
{
	panic("update is required!!");

#if USE_KHEAP
	panic("test_realloc_block_COMPLETE: the kernel heap should be disabled. make sure USE_KHEAP = 0");
	return;
#endif
}


/********************Helper Functions***************************/
