#include <inc/memlayout.h>
#include "shared_memory_manager.h"

#include <inc/mmu.h>
#include <inc/error.h>
#include <inc/string.h>
#include <inc/assert.h>
#include <inc/queue.h>
#include <inc/environment_definitions.h>

#include <kern/proc/user_environment.h>
#include <kern/trap/syscall.h>
#include "kheap.h"
#include "memory_manager.h"

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// [1] INITIALIZE SHARES:
//===========================
//Initialize the list and the corresponding lock
void sharing_init()
{
#if USE_KHEAP
	LIST_INIT(&AllShares.shares_list) ;
	init_kspinlock(&AllShares.shareslock, "shares lock");
	//init_sleeplock(&AllShares.sharessleeplock, "shares sleep lock");
#else
	panic("not handled when KERN HEAP is disabled");
#endif
}

//=========================
// [2] Find Share Object:
//=========================
//Search for the given shared object in the "shares_list"
//Return:
//	a) if found: ptr to Share object
//	b) else: NULL
struct Share* find_share(int32 ownerID, char* name)
{
#if USE_KHEAP
	struct Share * ret = NULL;
	bool wasHeld = holding_kspinlock(&(AllShares.shareslock));
	if (!wasHeld)
	{
		acquire_kspinlock(&(AllShares.shareslock));
	}
	{
		struct Share * shr ;
		LIST_FOREACH(shr, &(AllShares.shares_list))
		{
			//cprintf("shared var name = %s compared with %s\n", name, shr->name);
			if(shr->ownerID == ownerID && strcmp(name, shr->name)==0)
			{
				//cprintf("%s found\n", name);
				ret = shr;
				break;
			}
		}
	}
	if (!wasHeld)
	{
		release_kspinlock(&(AllShares.shareslock));
	}
	return ret;
#else
	panic("not handled when KERN HEAP is disabled");
#endif
}

//==============================
// [3] Get Size of Share Object:
//==============================
int size_of_shared_object(int32 ownerID, char* shareName)
{
	// This function should return the size of the given shared object
	// RETURN:
	//	a) If found, return size of shared object
	//	b) Else, return E_SHARED_MEM_NOT_EXISTS
	//
	struct Share* ptr_share = find_share(ownerID, shareName);
	if (ptr_share == NULL)
		return E_SHARED_MEM_NOT_EXISTS;
	else
		return ptr_share->size;

	return 0;
}
//===========================================================


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
//create frame_storage:
//=====================================
inline struct FrameInfo** create_frames_storage(int numOfFrames) {
	if (numOfFrames <= 0) {
	        return NULL;
	    }
	struct FrameInfo** framesStorage = (struct FrameInfo**) kmalloc(numOfFrames * sizeof(struct FrameInfo*));
	if (framesStorage == NULL) {
	        return NULL;
	    }
	memset(framesStorage, 0, numOfFrames * sizeof(struct FrameInfo*));
	return framesStorage;
}
//=====================================
// [1] Alloc & Initialize Share Object:
//=====================================
//Allocates a new shared object and initialize its member
//It dynamically creates the "framesStorage"
//Return: allocatedObject (pointer to struct Share) passed by reference
struct Share* alloc_share(int32 ownerID, char* shareName, uint32 size, uint8 isWritable)
{
#if USE_KHEAP
	//TODO: [PROJECT'25.IM#3] SHARED MEMORY - #1 alloc_share
	//Your code is here
	struct Share* shared_obj=(struct Share*) kmalloc(sizeof(struct Share));
	if(shared_obj==NULL){
		return NULL;
	}
	unsigned int no_of_frame = (unsigned int)((size + PAGE_SIZE-1)/PAGE_SIZE);
	struct FrameInfo** frames_store = create_frames_storage(no_of_frame);
	if(frames_store ==NULL){
		kfree(shared_obj);
		return NULL;
	}
	shared_obj->ownerID=ownerID;
	shared_obj->size=size;

	strncpy (shared_obj->name ,shareName,sizeof(shared_obj->name) - 1);
	shared_obj->name[sizeof(shared_obj->name)-1]='\0';
	if(isWritable){
		shared_obj->isWritable=PERM_WRITEABLE;
	}else{
		shared_obj->isWritable=0;
	}
	shared_obj->framesStorage=frames_store;
	shared_obj->references = 1;
	uint32 object_address=(uint32)shared_obj;
	shared_obj->ID=object_address & 0x7FFFFFFF;
	return shared_obj;
	//Comment the following line
	//panic("alloc_share() is not implemented yet...!!");
#else
	panic("Error in alloc_share: USE_KHEAP = 0 is not supported...!! ");
	return NULL;
#endif
}


//=========================
// [4] Create Share Object:
//=========================
int create_shared_object(int32 ownerID, char* shareName, uint32 size, uint8 isWritable, void* virtual_address)
{
#if USE_KHEAP
	//TODO: [PROJECT'25.IM#3] SHARED MEMORY - #3 create_shared_object
	//Comment the following line
	//panic("create_shared_object() is not implemented yet...!!");
	//Your code is here
	//panic("createSharedObject is not implemented yet");
    struct Env* myenv = get_cpu_proc();
    uint32 va_start = (uint32)virtual_address;
    int num_pages = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
    acquire_kspinlock(&(AllShares.shareslock));

    if (find_share(ownerID, shareName) != NULL)
    {
        release_kspinlock(&(AllShares.shareslock));
        return E_SHARED_MEM_EXISTS;
    }
    struct Share* shared_obj = alloc_share(ownerID, shareName, size, isWritable);
    if (shared_obj == NULL) {
        release_kspinlock(&(AllShares.shareslock));
        return E_NO_SHARE;
    }
    uint32 perms =  PERM_WRITEABLE | PERM_USER;
    uint32 va = va_start;
    int i = 0;
    for (i = 0; i < num_pages; i++) {
        struct FrameInfo* frame = NULL;
        int ret = allocate_frame(&frame);
        if (ret != 0) {
            for (int j = 0; j < i; j++) {
                struct FrameInfo* frame_to_unmap = shared_obj->framesStorage[j];
                unmap_frame(myenv->env_page_directory, va_start + j * PAGE_SIZE);
                if (frame_to_unmap != NULL) {
                    free_frame(frame_to_unmap);

                }
            }
            kfree(shared_obj->framesStorage);
            kfree(shared_obj);
            release_kspinlock(&(AllShares.shareslock));
            return E_NO_SHARE;
        }
        int map_ret = map_frame(myenv->env_page_directory, frame, va, perms);
        if (map_ret != 0) {
            free_frame(frame);
            for (int j = 0; j < i; j++) {
                struct FrameInfo* frame_to_unmap = shared_obj->framesStorage[j];
                unmap_frame(myenv->env_page_directory, va_start + j * PAGE_SIZE);
                if (frame_to_unmap != NULL) {
                    free_frame(frame_to_unmap);
                }
    }
            kfree(shared_obj->framesStorage);
            kfree(shared_obj);
            release_kspinlock(&(AllShares.shareslock));
            return E_NO_SHARE;
        }
        shared_obj->framesStorage[i] = frame;
        va += PAGE_SIZE;
    }
    LIST_INSERT_TAIL(&(AllShares.shares_list), shared_obj);
    release_kspinlock(&(AllShares.shareslock));
    return shared_obj->ID;
	// This function should create the shared object at the given virtual address with the given size
	// and return the ShareObjectID
	// RETURN:
	//	a) ID of the shared object (its VA after masking out its msb) if success
	//	b) E_SHARED_MEM_EXISTS if the shared object already exists
	//	c) E_NO_SHARE if failed to create a shared object
#else
	panic("Error in create_shared_object: USE_KHEAP = 0 is not supported...!! ");
	return E_NO_SHARE;
#endif
}


//======================
// [5] Get Share Object:
//======================
int get_shared_object(int32 ownerID, char* shareName, void* virtual_address)
{
#if USE_KHEAP
	//TODO: [PROJECT'25.IM#3] SHARED MEMORY - #5 get_shared_object
	//Your code is here
	//Comment the following line
	//panic("get_shared_object() is not implemented yet...!!");
	acquire_kspinlock(&(AllShares.shareslock));

	struct Env* myenv = get_cpu_proc(); //The calling environment
	struct Share * share_obj = NULL;
	{
		struct Share * shr ;
		LIST_FOREACH(shr, &(AllShares.shares_list))
		{
			if(shr->ownerID == ownerID && strcmp(shareName, shr->name)==0)
			{
				share_obj = shr;
				break;
			}
		}
	}
	if (share_obj == NULL) {
		release_kspinlock(&(AllShares.shareslock));
		return E_SHARED_MEM_NOT_EXISTS;
	}
	uint32 va_start = (uint32)virtual_address;
	uint32 numPages = ROUNDUP(share_obj->size, PAGE_SIZE) / PAGE_SIZE;
	for (uint32 i = 0; i < numPages; i++) {
		struct FrameInfo* frame = share_obj->framesStorage[i];
		if (frame == NULL) {
			for (uint32 j = 0; j < i; j++) {
				unmap_frame(myenv->env_page_directory, va_start + (j * PAGE_SIZE));
			}
			release_kspinlock(&(AllShares.shareslock));
			return E_NO_SHARE;
		}
		uint32 permissions = PERM_PRESENT | PERM_USER;
		if (share_obj->isWritable & PERM_WRITEABLE) {
			permissions |= PERM_WRITEABLE;
		}
		uint32 current_va = va_start + (i * PAGE_SIZE);
		int ret = map_frame(myenv->env_page_directory, frame, current_va, permissions);
		if (ret != 0) {
			for (uint32 j = 0; j < i; j++) {
				unmap_frame(myenv->env_page_directory, va_start + (j * PAGE_SIZE));
			}
			release_kspinlock(&(AllShares.shareslock));
			return E_NO_SHARE;

		}
		}
	share_obj->references++;
	release_kspinlock(&(AllShares.shareslock));
	return share_obj->ID;



	// 	This function should share the required object in the heap of the current environment
	//	starting from the given virtual_address with the specified permissions of the object: read_only/writable
	// 	and return the ShareObjectID
	// RETURN:
	//	a) ID of the shared object (its VA after masking out its msb) if success
	//	b) E_SHARED_MEM_NOT_EXISTS if the shared object is not exists
#else
	panic("get_shared_object: USE_KHEAP is off, implementation needed");
	return E_SHARED_MEM_NOT_EXISTS;
#endif
}

//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//
//=========================
// [1] Delete Share Object:
//=========================
//delete the given shared object from the "shares_list"
//it should free its framesStorage and the share object itself
void free_share(struct Share* ptrShare)
{
#if USE_KHEAP
	//TODO: [PROJECT'25.BONUS#5] EXIT #2 - free_share
	//Your code is here
	//Comment the following line
	//panic("free_share() is not implemented yet...!!");
    LIST_REMOVE(&(AllShares.shares_list),ptrShare);
    kfree(ptrShare->framesStorage);
    kfree(ptrShare);
#else
	panic("Error in free_share: USE_KHEAP = 0 is not supported...!! ");
#endif
}


//=========================
// [2] Free Share Object:
//=========================
int delete_shared_object(int32 sharedObjectID, void *startVA)
{
	//TODO: [PROJECT'25.BONUS#5] EXIT #2 - delete_shared_object
	//Your code is here
	//Comment the following line
	panic("delete_shared_object() is not implemented yet...!!");

	struct Env* myenv = get_cpu_proc(); //The calling environment

	// This function should free (delete) the shared object from the User Heapof the current environment
	// If this is the last shared env, then the "frames_store" should be cleared and the shared object should be deleted
	// RETURN:
	//	a) 0 if success
	//	b) E_SHARED_MEM_NOT_EXISTS if the shared object is not exists

	// Steps:
	//	1) Get the shared object from the "shares" array (use get_share_object_ID())
	//	2) Unmap it from the current environment "myenv"
	//	3) If one or more table becomes empty, remove it
	//	4) Update references
	//	5) If this is the last share, delete the share object (use free_share())
	//	6) Flush the cache "tlbflush()"

}
