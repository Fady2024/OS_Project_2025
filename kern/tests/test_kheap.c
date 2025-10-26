#include "test_kheap.h"

#include <inc/memlayout.h>
#include <inc/queue.h>
#include <inc/dynamic_allocator.h>
#include <kern/cpu/sched.h>
#include <kern/disk/pagefile_manager.h>
#include "../mem/kheap.h"
#include "../mem/memory_manager.h"


#define Mega  (1024*1024)
#define kilo (1024)

//2017
#define DYNAMIC_ALLOCATOR_DS 0 //ROUNDUP(NUM_OF_KHEAP_PAGES * sizeof(struct MemBlock), PAGE_SIZE)
#define INITIAL_KHEAP_ALLOCATIONS (DYNAMIC_ALLOCATOR_DS) //( + KERNEL_SHARES_ARR_INIT_SIZE + KERNEL_SEMAPHORES_ARR_INIT_SIZE) //
#define INITIAL_BLOCK_ALLOCATIONS ((2*sizeof(int) + MAX(num_of_ready_queues * sizeof(uint8), DYN_ALLOC_MIN_BLOCK_SIZE)) + (2*sizeof(int) + MAX(num_of_ready_queues * sizeof(struct Env_Queue), DYN_ALLOC_MIN_BLOCK_SIZE)))
#define ACTUAL_START ((KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE + PAGE_SIZE) + INITIAL_KHEAP_ALLOCATIONS)

extern uint32 sys_calculate_free_frames() ;
extern void sys_bypassPageFault(uint8);
extern uint32 sys_rcr2();
extern int execute_command(char *command_string);

extern char end_of_kernel[];

extern int CB(uint32 *ptr_dir, uint32 va, int bn);

struct MyStruct
{
	char a;
	short b;
	int c;
};

/*KMALLOC*/
int test_kmalloc_FF_page();
int test_kmalloc_NF_page();
int test_kmalloc_BF_page();
int test_kmalloc_WF_page();
int test_kmalloc_CF_page();

int test_kmalloc_FF_block();
int test_kmalloc_NF_block();
int test_kmalloc_BF_block();
int test_kmalloc_WF_block();
int test_kmalloc_CF_block();

int test_kmalloc_FF_both();
int test_kmalloc_NF_both();
int test_kmalloc_BF_both();
int test_kmalloc_WF_both();
int test_kmalloc_CF_both();

/*KFREE*/
int test_kfree_FF_page();
int test_kfree_NF_page();
int test_kfree_BF_page();
int test_kfree_WF_page();
int test_kfree_CF_page();

int test_kfree_FF_block();
int test_kfree_NF_block();
int test_kfree_BF_block();
int test_kfree_WF_block();
int test_kfree_CF_block();

int test_kfree_FF_both();
int test_kfree_NF_both();
int test_kfree_BF_both();
int test_kfree_WF_both();
int test_kfree_CF_both();

/*KREALLOC*/
int test_krealloc_FF_page();
int test_krealloc_NF_page();
int test_krealloc_BF_page();
int test_krealloc_WF_page();
int test_krealloc_CF_page();

int test_krealloc_FF_block();
int test_krealloc_NF_block();
int test_krealloc_BF_block();
int test_krealloc_WF_block();
int test_krealloc_CF_block();

int test_krealloc_FF_both();
int test_krealloc_NF_both();
int test_krealloc_BF_both();
int test_krealloc_WF_both();
int test_krealloc_CF_both();

/*FAST PAGE ALLOCATOR*/
int test_fast_FF();
int test_fast_NF();
int test_fast_BF();
int test_fast_WF();
int test_fast_CF();


uint32 da_limit = KERNEL_HEAP_START + DYN_ALLOC_MAX_SIZE ;

int test_kmalloc(uint32 ALLOC_TYPE)
{
	if (get_kheap_strategy() == KHP_PLACE_FIRSTFIT)
	{
		//Test FIRST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kmalloc_FF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kmalloc_FF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kmalloc_FF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_BESTFIT)
	{
		//Test BEST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kmalloc_BF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kmalloc_BF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kmalloc_BF_both();
	}
	else if(get_kheap_strategy() == KHP_PLACE_NEXTFIT)
	{
		//Test NEXT FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kmalloc_NF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kmalloc_NF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kmalloc_NF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_WORSTFIT)
	{
		//Test WORST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kmalloc_WF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kmalloc_WF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kmalloc_WF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_CUSTOMFIT)
	{
		//Test CUSTOM FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kmalloc_CF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kmalloc_CF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kmalloc_CF_both();
	}
	return 0;
}

int test_kfree(uint32 ALLOC_TYPE)
{
	if (get_kheap_strategy() == KHP_PLACE_FIRSTFIT)
	{
		//Test FIRST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kfree_FF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kfree_FF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kfree_FF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_BESTFIT)
	{
		//Test BEST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kfree_BF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kfree_BF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kfree_BF_both();
	}
	else if(get_kheap_strategy() == KHP_PLACE_NEXTFIT)
	{
		//Test NEXT FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kfree_NF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kfree_NF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kfree_NF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_WORSTFIT)
	{
		//Test WORST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kfree_WF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kfree_WF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kfree_WF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_CUSTOMFIT)
	{
		//Test CUSTOM FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_kfree_CF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_kfree_CF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_kfree_CF_both();
	}
	return 0;
}
int test_krealloc(uint32 ALLOC_TYPE)
{
	if (get_kheap_strategy() == KHP_PLACE_FIRSTFIT)
	{
		//Test FIRST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_krealloc_FF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_krealloc_FF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_krealloc_FF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_BESTFIT)
	{
		//Test BEST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_krealloc_BF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_krealloc_BF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_krealloc_BF_both();
	}
	else if(get_kheap_strategy() == KHP_PLACE_NEXTFIT)
	{
		//Test NEXT FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_krealloc_NF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_krealloc_NF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_krealloc_NF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_WORSTFIT)
	{
		//Test WORST FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_krealloc_WF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_krealloc_WF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_krealloc_WF_both();
	}
	else if (get_kheap_strategy() == KHP_PLACE_CUSTOMFIT)
	{
		//Test CUSTOM FIT allocation
		if (ALLOC_TYPE == TST_PAGE_ALLOC)
			test_krealloc_CF_page();
		else if (ALLOC_TYPE == TST_BLOCK_ALLOC)
			test_krealloc_CF_block();
		else if (ALLOC_TYPE == TST_BOTH_ALLOC)
			test_krealloc_CF_both();
	}
	return 0;
}

void* ptr_fast_allocations[(KERNEL_HEAP_MAX - KERNEL_HEAP_START)/PAGE_SIZE] = {0};
int test_fast_page_alloc()
{
	if (get_kheap_strategy() == KHP_PLACE_FIRSTFIT)
	{
		test_fast_FF();
	}
	else if (get_kheap_strategy() == KHP_PLACE_NEXTFIT)
	{
		test_fast_NF();
	}
	else if (get_kheap_strategy() == KHP_PLACE_BESTFIT)
	{
		test_fast_BF();
	}
	else if (get_kheap_strategy() == KHP_PLACE_WORSTFIT)
	{
		test_fast_WF();
	}
	else if (get_kheap_strategy() == KHP_PLACE_CUSTOMFIT)
	{
		test_fast_CF();
	}
	return 0;
}

int test_kheap_phys_addr()
{
	panic("update is required!!");

	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	cprintf("\n1. Allocate some spaces in both allocators \n");
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};
	int eval = 0;
	bool correct = 1;
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("1.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.1 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("1.2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.2 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//[DYNAMIC ALLOCATOR]
		{
			//1 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(1*kilo);
			//if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
			{ correct = 0; cprintf("1.3 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			//if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
			{ correct = 0; cprintf("1.4 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//1.5 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[4] = kmalloc(3*kilo/2);
			//if ((uint32) ptr_allocations[4] < KERNEL_HEAP_START || ptr_allocations[4] >= sbrk(0) || (uint32) ptr_allocations[4] >= da_limit)
			{ correct = 0; cprintf("1.5 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("1.6 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("1.6 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("1.7 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("1.7 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("1.8 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("1.8 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("1.9 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.9 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("1.9 Wrong allocation: pages are not loaded successfully into memory\n"); }
	}

	//[PAGE ALLOCATOR] test kheap_physical_address after kmalloc only [30%]
	cprintf("\n2. [PAGE ALLOCATOR] test kheap_physical_address after kmalloc only [30%]\n");
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 allPAs[(13*Mega + 24*kilo + INITIAL_KHEAP_ALLOCATIONS)/PAGE_SIZE] ;
		i = 0;
		uint32 offset = 1;
		uint32 startVA = da_limit + PAGE_SIZE;
		for (va = startVA; va < endVA; va+=PAGE_SIZE+offset)
		{
			allPAs[i++] = kheap_physical_address(va);
		}
		int ii = i ;
		i = 0;
		int j;
		for (va = startVA; va < endVA; )
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
			{ correct = 0; panic("2.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			for (j = PTX(va); i < ii && j < 1024 && va < endVA; ++j, ++i)
			{
				if (((ptr_table[j] & 0xFFFFF000)+(va & 0x00000FFF))!= allPAs[i])
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("2.2 Wrong kheap_physical_address\n"); }
				}
				va+=PAGE_SIZE+offset;
			}
		}
	}
	if (correct)	eval+=30 ;

	correct = 1 ;
	//[DYNAMIC ALLOCATOR] test kheap_physical_address after kmalloc only [10%]
	cprintf("\n3. [DYNAMIC ALLOCATOR] test kheap_physical_address after kmalloc only [10%]\n");
	{
		int i;
		uint32 va, pa;
		for (i = 2; i <= 4; i++)
		{
			va = (uint32)ptr_allocations[i];
			pa = kheap_physical_address(va);
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
			{ correct = 0; panic("3.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			if (((ptr_table[PTX(va)] & 0xFFFFF000)+(va & 0x00000FFF))!= pa)
			{
				//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
				if (correct)
				{ correct = 0; cprintf("3.2 Wrong kheap_physical_address\n"); }
			}
		}
	}
	if (correct)	eval+=10 ;

	correct = 1 ;
	//kfree some of the allocated spaces
	cprintf("\n4. kfree some of the allocated spaces\n");
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("4.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("4.1 Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("4.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("4.2 Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("4.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("4.3 Wrong kfree: pages in memory are not freed correctly\n"); }
	}

	uint32 expected;
	//[PAGE ALLOCATOR] test kheap_physical_address after kmalloc and kfree [20%]
	cprintf("\n5. [PAGE ALLOCATOR] test kheap_physical_address after kmalloc and kfree [20%]\n");
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 allPAs[(13*Mega + 24*kilo + INITIAL_KHEAP_ALLOCATIONS)/PAGE_SIZE] ;
		i = 0;
		uint32 offset = 121;
		uint32 startVA = da_limit + PAGE_SIZE+offset;

		for (va = startVA; va < endVA; va+=PAGE_SIZE)
		{
			allPAs[i++] = kheap_physical_address(va);
		}
		int ii = i ;
		i = 0;
		int j;
		for (va = startVA; va < endVA; )
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
				if (correct)
				{ correct = 0; panic("5.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			for (j = PTX(va); i < ii && j < 1024 && va < endVA; ++j, ++i)
			{
				expected = 0 ;
				if ((ptr_table[j] & PERM_PRESENT))
				{
					expected = (ptr_table[j] & 0xFFFFF000) + (va & 0x00000FFF);
				}
				//if (((ptr_table[j] & 0xFFFFF000)+((ptr_table[j] & PERM_PRESENT) == 0? 0 : va & 0x00000FFF)) != allPAs[i])
				if (expected != allPAs[i])
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("5.2 Wrong kheap_physical_address\n"); }
				}
				va += PAGE_SIZE;
			}
		}
	}
	if (correct)	eval+=20 ;

	correct = 1 ;
	//[DYNAMIC ALLOCATOR] test kheap_physical_address on the entire allocated area [30%]
	cprintf("\n6. [DYNAMIC ALLOCATOR] test kheap_physical_address on the entire allocated area [30%]\n");
	{
		uint32 va, pa;

		/*if ((uint32)sbrk(0) <= KERNEL_HEAP_START + PAGE_SIZE) panic("6.1 unexpected sbrk value");*/

		/*for (va = KERNEL_HEAP_START; va < (uint32)sbrk(0); va++)*/
		{
			pa = kheap_physical_address(va);
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
				if (correct)
				{ correct = 0; panic("6.2 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			if (((ptr_table[PTX(va)] & 0xFFFFF000)+(va & 0x00000FFF))!= pa)
			{
				//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
				if (correct)
				{ correct = 0; cprintf("6.3 Wrong kheap_physical_address\n"); }
			}
		}
	}
	if (correct)	eval+=30 ;

	correct = 1 ;
	//test kheap_physical_address on non-mapped area [10%]
	cprintf("\n7. test kheap_physical_address on non-mapped area [10%]\n");
	{
		uint32 va;
		uint32 startVA = ACTUAL_START + 16*Mega;
		i = 0;
		for (va = startVA; va < KERNEL_HEAP_MAX; va+=PAGE_SIZE)
		{
			i++;
		}
		int ii = i ;
		i = 0;
		int j;
		long long va2;
		for (va2 = startVA; va2 < (long long)KERNEL_HEAP_MAX; va2+=PTSIZE)
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, (uint32)va2, &ptr_table);
			if (ptr_table == NULL)
			{
				if (correct)
				{ correct = 0; panic("7.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
			}
			for (j = 0; i < ii && j < 1024; ++j, ++i)
			{
				//if ((ptr_table[j] & 0xFFFFF000) != allPAs[i])
				unsigned int page_va = startVA+i*PAGE_SIZE;
				unsigned int supposed_kheap_phys_add = kheap_physical_address(page_va);
				//if (((ptr_table[j] & 0xFFFFF000)+((ptr_table[j] & PERM_PRESENT) == 0? 0 : page_va & 0x00000FFF)) != supposed_kheap_phys_add)
				if (supposed_kheap_phys_add != 0)
				{
					//cprintf("\nVA = %x, table entry = %x, khep_pa = %x\n",va2 + j*PAGE_SIZE, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
					if (correct)
					{ correct = 0; cprintf("7.2 Wrong kheap_physical_address\n"); }
				}
			}
		}
	}
	if (correct)	eval+=10 ;

	cprintf("\ntest kheap_physical_address completed. Eval = %d%\n", eval);

	return 1;

}

int test_kheap_virt_addr()
{
	panic("update is required!!");

	/*********************** NOTE ****************************
	 * WE COMPARE THE DIFF IN FREE FRAMES BY "AT LEAST" RULE
	 * INSTEAD OF "EQUAL" RULE SINCE IT'S POSSIBLE FOR SOME
	 * IMPLEMENTATIONS TO DYNAMICALLY ALLOCATE SPECIAL DATA
	 * STRUCTURE TO MANAGE THE PAGE ALLOCATOR.
	 *********************************************************/

	cprintf("==============================================\n");
	cprintf("MAKE SURE to have a FRESH RUN for this test\n(i.e. don't run any program/test before it)\n");
	cprintf("==============================================\n");

	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	cprintf("\n1. Allocate some spaces in both allocators \n");
	int i, freeFrames, freeDiskFrames ;
	char* ptr;
	int lastIndices[20] = {0};
	int sums[20] = {0};

	int eval = 0;
	bool correct = 1;

	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[0] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (ACTUAL_START)) { correct = 0; cprintf("1.1 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.1 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[1] = kmalloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (ACTUAL_START + 2*Mega)) { correct = 0; cprintf("1.2 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 512) { correct = 0; cprintf("1.2 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//[DYNAMIC ALLOCATOR]
		{
			//1 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[2] = kmalloc(1*kilo);
			//if ((uint32) ptr_allocations[2] < KERNEL_HEAP_START || ptr_allocations[2] >= sbrk(0) || (uint32) ptr_allocations[2] >= da_limit)
			{ correct = 0; cprintf("1.3 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//2 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[3] = kmalloc(2*kilo);
			//if ((uint32) ptr_allocations[3] < KERNEL_HEAP_START || ptr_allocations[3] >= sbrk(0) || (uint32) ptr_allocations[3] >= da_limit)
			{ correct = 0; cprintf("1.4 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.4 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
			//if ((freeFrames - sys_calculate_free_frames()) != 1) { correct = 0; cprintf("Wrong allocation: pages are not loaded successfully into memory\n"); }

			//1.5 KB
			freeFrames = sys_calculate_free_frames() ;
			freeDiskFrames = pf_calculate_free_frames() ;
			ptr_allocations[4] = kmalloc(3*kilo/2);
			//if ((uint32) ptr_allocations[4] < KERNEL_HEAP_START || ptr_allocations[4] >= sbrk(0) || (uint32) ptr_allocations[4] >= da_limit)
			{ correct = 0; cprintf("1.5 Wrong start address for the allocated space... should allocated by the dynamic allocator! check return address of kmalloc and/or sbrk\n"); }
			if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.5 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[5] = kmalloc(7*kilo);
		if ((uint32) ptr_allocations[5] != (ACTUAL_START + 4*Mega /*+ 8*kilo*/)) { correct = 0; cprintf("1.6 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.6 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 2) { correct = 0; cprintf("1.6 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[6] = kmalloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[6] != (ACTUAL_START + 4*Mega + 8*kilo) ) { correct = 0; cprintf("1.7 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.7 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 768) { correct = 0; cprintf("1.7 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[7] = kmalloc(6*Mega-kilo);
		if ((uint32) ptr_allocations[7] != (ACTUAL_START + 7*Mega + 8*kilo)) { correct = 0; cprintf("1.8 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.8 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 1536) { correct = 0; cprintf("1.8 Wrong allocation: pages are not loaded successfully into memory\n"); }

		//14 KB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		ptr_allocations[8] = kmalloc(14*kilo);
		if ((uint32) ptr_allocations[8] != (ACTUAL_START + 13*Mega + 8*kilo)) { correct = 0; cprintf("1.9 Wrong start address for the allocated space... check return address of kmalloc\n"); }
		if ((pf_calculate_free_frames() - freeDiskFrames) != 0) { correct = 0; cprintf("1.9 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((freeFrames - sys_calculate_free_frames()) < 4) { correct = 0; cprintf("1.9 Wrong allocation: pages are not loaded successfully into memory\n"); }
	}

	uint32 allocatedSpace = (13*Mega + 24*kilo + (INITIAL_KHEAP_ALLOCATIONS));
	uint32 allPAs[allocatedSpace/PAGE_SIZE] ;
	int numOfFrames = allocatedSpace/PAGE_SIZE ;

	//test kheap_virtual_address after kmalloc only [20%]
	cprintf("\n2. [PAGE ALLOCATOR] test kheap_virtual_address after kmalloc only [20%]\n");
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 startVA = da_limit + PAGE_SIZE;
		int i = 0;
		int j;
		for (va = startVA; va < endVA; )
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
			{ correct = 0; panic("2.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }

			for (j = PTX(va); i < numOfFrames && j < 1024 && va < endVA; ++j, ++i)
			{
				uint32 offset = j;
				allPAs[i] = (ptr_table[j] & 0xFFFFF000) + offset;
				uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
				//cprintf("va to check = %x\n", va);
				if (retrievedVA != (va+offset))
				{
					if (correct)
					{
						cprintf("\nretrievedVA = %x, Actual VA = %x, table entry = %x, khep_pa = %x\n",retrievedVA, va + offset /*+ j*PAGE_SIZE*/, (ptr_table[j] & 0xFFFFF000) , allPAs[i]);
						correct = 0; cprintf("2.2 Wrong kheap_virtual_address\n");
					}
				}
				va+=PAGE_SIZE;
			}
		}
	}
	if (correct)	eval+=20 ;

	correct = 1 ;
	//kfree some of the allocated spaces
	cprintf("\n3. kfree some of the allocated spaces\n");
	{
		//kfree 1st 2 MB
		int freeFrames = sys_calculate_free_frames() ;
		int freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[0]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("3.1 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512 ) { correct = 0; cprintf("3.1 Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 2nd 2 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[1]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("3.2 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 512) { correct = 0; cprintf("3.2 Wrong kfree: pages in memory are not freed correctly\n"); }

		//kfree 6 MB
		freeFrames = sys_calculate_free_frames() ;
		freeDiskFrames = pf_calculate_free_frames() ;
		kfree(ptr_allocations[7]);
		if ((freeDiskFrames - pf_calculate_free_frames()) != 0) { correct = 0; cprintf("3.3 Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)\n"); }
		if ((sys_calculate_free_frames() - freeFrames) < 6*Mega/4096) { correct = 0; cprintf("3.3 Wrong kfree: pages in memory are not freed correctly\n"); }
	}


	//test kheap_virtual_address after kmalloc and kfree [20%]
	cprintf("\n4. [PAGE ALLOCATOR] test kheap_virtual_address after kmalloc and kfree [20%]\n");
	{
		uint32 va;
		uint32 endVA = ACTUAL_START + 13*Mega + 24*kilo;
		uint32 startVA = da_limit + PAGE_SIZE;
		int i = 0;
		int j;
		//frames of first 4 MB
		uint32 startIndex = (INITIAL_KHEAP_ALLOCATIONS) / PAGE_SIZE;
		for (i = startIndex ; i < startIndex + 4*Mega/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != 0)
			{
				if (correct)
				{ correct = 0; cprintf("4.1 Wrong kheap_virtual_address\n"); }
			}

		}
		//next frames until 6 MB
		for (i = startIndex + 4*Mega/PAGE_SIZE; i < startIndex + (7*Mega + 8*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != ((startVA + i*PAGE_SIZE) + (allPAs[i] & 0xFFF)))
			{
				if (correct)
				{ correct = 0; cprintf("4.2 Wrong kheap_virtual_address\n"); }
			}
		}
		//frames of 6 MB
		for (i = startIndex + (7*Mega + 8*kilo)/PAGE_SIZE; i < startIndex + (13*Mega + 8*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != 0)
			{
				if (correct)
				{ correct = 0; cprintf("4.3 Wrong kheap_virtual_address\n"); }
			}
		}
		//frames of last allocation (14 KB)
		for (i = startIndex + (13*Mega + 8*kilo)/PAGE_SIZE; i < startIndex + (13*Mega + 24*kilo)/PAGE_SIZE; ++i)
		{
			uint32 retrievedVA = kheap_virtual_address(allPAs[i]);
			if (retrievedVA != ((startVA + i*PAGE_SIZE) + (allPAs[i] & 0xFFF)))
			{
				if (correct)
				{ correct = 0; cprintf("4.4 Wrong kheap_virtual_address\n"); }
			}
		}
	}
	if (correct)	eval+=20 ;

	correct = 1 ;
	//[DYNAMIC ALLOCATOR] test kheap_virtual_address each address [40%]
	cprintf("\n5. [DYNAMIC ALLOCATOR] test kheap_virtual_address each address [40%]\n");
	{
		uint32 va, pa;
		/*if ((uint32)sbrk(0) <= KERNEL_HEAP_START + PAGE_SIZE) panic("unexpected sbrk value");
												for (va = KERNEL_HEAP_START; va < (uint32)sbrk(0); va++)*/
		{
			uint32 *ptr_table ;
			get_page_table(ptr_page_directory, va, &ptr_table);
			if (ptr_table == NULL)
			{ correct = 0; panic("5.1 one of the kernel tables is wrongly removed! Tables of Kernel Heap should not be removed\n"); }
			pa = (ptr_table[PTX(va)] & 0xFFFFF000) + (va & 0xFFF);
			uint32 retrievedVA = kheap_virtual_address(pa);
			if (retrievedVA != va)
			{
				if (correct)
				{
					cprintf("\nPA = %x, retrievedVA = %x expectedVA = %x\n", pa, retrievedVA, va);
					correct = 0; cprintf("5.2 Wrong kheap_virtual_address\n");
				}
			}
		}
	}
	if (correct)	eval+=40 ;

	correct = 1 ;
	//test kheap_virtual_address on frames of KERNEL CODE [20%]
	cprintf("\n6. test kheap_virtual_address on frames of KERNEL CODE [20%]\n");
	{
		uint32 i;
		for (i = 1*Mega; i < (uint32)(end_of_kernel - KERNEL_BASE); i+=PAGE_SIZE)
		{
			uint32 retrievedVA = kheap_virtual_address(i);
			if (retrievedVA != 0)
			{
				if (correct)
				{
					cprintf("\nPA = %x, retrievedVA = %x\n", i, retrievedVA);
					correct = 0; cprintf("6.1 Wrong kheap_virtual_address\n");
				}
			}
		}
	}
	if (correct)	eval+=20 ;

	cprintf("\ntest kheap_virtual_address completed. Eval = %d%\n", eval);

	return 1;

}


/**********************************************************************************************/
/********************************** KMALLOC TESTING AREA **************************************/
/**********************************************************************************************/
int test_kmalloc_FF_page()
{
	panic("not implemented function");
}
int test_kmalloc_NF_page()
{
	panic("not implemented function");
}
int test_kmalloc_BF_page()
{
	panic("not implemented function");
}
int test_kmalloc_WF_page()
{
	panic("not implemented function");
}
int test_kmalloc_CF_page()
{
	panic("not implemented function");
}

int test_kmalloc_FF_block()
{
	panic("not implemented function");
}
int test_kmalloc_NF_block()
{
	panic("not implemented function");
}
int test_kmalloc_BF_block()
{
	panic("not implemented function");
}
int test_kmalloc_WF_block()
{
	panic("not implemented function");
}
int test_kmalloc_CF_block()
{
	panic("not implemented function");
}

int test_kmalloc_FF_both()
{
	panic("not implemented function");
}
int test_kmalloc_NF_both()
{
	panic("not implemented function");
}
int test_kmalloc_BF_both()
{
	panic("not implemented function");
}
int test_kmalloc_WF_both()
{
	panic("not implemented function");
}
int test_kmalloc_CF_both()
{
	panic("not implemented function");
}

/**********************************************************************************************/
/*********************************** KFREE TESTING AREA ***************************************/
/**********************************************************************************************/
int test_kfree_FF_page()
{
	panic("not implemented function");
}
int test_kfree_NF_page()
{
	panic("not implemented function");
}
int test_kfree_BF_page()
{
	panic("not implemented function");
}
int test_kfree_WF_page()
{
	panic("not implemented function");
}
int test_kfree_CF_page()
{
	panic("not implemented function");
}

int test_kfree_FF_block()
{
	panic("not implemented function");
}
int test_kfree_NF_block()
{
	panic("not implemented function");
}
int test_kfree_BF_block()
{
	panic("not implemented function");
}
int test_kfree_WF_block()
{
	panic("not implemented function");
}
int test_kfree_CF_block()
{
	panic("not implemented function");
}

int test_kfree_FF_both()
{
	panic("not implemented function");
}
int test_kfree_NF_both()
{
	panic("not implemented function");
}
int test_kfree_BF_both()
{
	panic("not implemented function");
}
int test_kfree_WF_both()
{
	panic("not implemented function");
}
int test_kfree_CF_both()
{
	panic("not implemented function");
}

/**********************************************************************************************/
/********************************** KREALLOC TESTING AREA *************************************/
/**********************************************************************************************/
int test_krealloc_FF_page()
{
	panic("not implemented function");
}
int test_krealloc_NF_page()
{
	panic("not implemented function");
}
int test_krealloc_BF_page()
{
	panic("not implemented function");
}
int test_krealloc_WF_page()
{
	panic("not implemented function");
}
int test_krealloc_CF_page()
{
	panic("not implemented function");
}

int test_krealloc_FF_block()
{
	panic("not implemented function");
}
int test_krealloc_NF_block()
{
	panic("not implemented function");
}
int test_krealloc_BF_block()
{
	panic("not implemented function");
}
int test_krealloc_WF_block()
{
	panic("not implemented function");
}
int test_krealloc_CF_block()
{
	panic("not implemented function");
}

int test_krealloc_FF_both()
{
	panic("not implemented function");
}
int test_krealloc_NF_both()
{
	panic("not implemented function");
}
int test_krealloc_BF_both()
{
	panic("not implemented function");
}
int test_krealloc_WF_both()
{
	panic("not implemented function");
}
int test_krealloc_CF_both()
{
	panic("not implemented function");
}

/**********************************************************************************************/
/*************************** FAST PAGE ALLOCATOR TESTING AREA *********************************/
/**********************************************************************************************/
int test_fast_FF()
{
	panic("not implemented function");
}
int test_fast_NF()
{
	panic("not implemented function");
}
int test_fast_BF()
{
	panic("not implemented function");
}
int test_fast_WF()
{
	panic("not implemented function");
}
int test_fast_CF()
{
	panic("not implemented function");
}




/**********************************************************************************************/
/******************************** OLD IMPLEMENTATION AREA *************************************/
/**********************************************************************************************/

int initFreeFrames;
int initFreeDiskFrames ;
uint8 firstCall = 1 ;
int test_three_creation_functions()
{
	if (firstCall)
	{
		firstCall = 0;
		initFreeFrames = sys_calculate_free_frames() ;
		initFreeDiskFrames = pf_calculate_free_frames() ;
		//Run simple user program
		{
			char command[100] = "run fos_add 4096";
			execute_command(command) ;
		}
	}
	//Ensure that the user directory, page WS and page tables are allocated in KERNEL HEAP
	{
		struct Env * e = NULL;
		struct Env * ptr_env = NULL;
		LIST_FOREACH(ptr_env, &ProcessQueues.env_exit_queue)
		{
			if (strcmp(ptr_env->prog_name, "fos_add") == 0)
			{
				e = ptr_env ;
				break;
			}
		}
		if (e->pageFaultsCounter != 0)
			panic("Page fault is occur while not expected to. Review the three creation functions");

#if USE_KHEAP
		int pagesInWS = LIST_SIZE(&(e->page_WS_list));
#else
		int pagesInWS = env_page_ws_get_size(e);
#endif
		int curFreeFrames = sys_calculate_free_frames() ;
		int curFreeDiskFrames = pf_calculate_free_frames() ;
		//cprintf("\ndiff in page file = %d, pages in WS = %d\n", initFreeDiskFrames - curFreeDiskFrames, pagesInWS);
		if ((initFreeDiskFrames - curFreeDiskFrames) != pagesInWS) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		//cprintf("\ndiff in mem frames = %d, pages in WS = %d\n", initFreeFrames - curFreeFrames, pagesInWS);
		if ((initFreeFrames - curFreeFrames) != 12/*WS*/ + 2*1/*DIR*/ + 2*3/*Tables*/ + 1 /*user WS table*/ + pagesInWS) panic("Wrong allocation: pages are not loaded successfully into memory");

		//allocate 4 KB
		char *ptr = kmalloc(4*kilo);
		if ((uint32) ptr !=  (ACTUAL_START + (12+2*1+2*3+1)*PAGE_SIZE)) panic("Wrong start address for the allocated space... make sure you create the dir, table and page WS in KERNEL HEAP");
	}

	cprintf("\nCongratulations!! test the 3 creation functions is completed successfully.\n");

	return 1;
}



extern void kfreeall() ;

int test_kfreeall()
{
	panic("not implemented function");
}


extern void kexpand(uint32 newSize) ;

int test_kexpand()
{
	panic("not implemented function");
}

extern void kshrink(uint32 newSize) ;

int test_kshrink()
{
	panic("not implemented function");
}


int test_kfreelast()
{
	panic("not implemented function");
}

