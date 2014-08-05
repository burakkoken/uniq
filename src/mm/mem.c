/*
 *  Copyright(C) 2014 Codnect Team
 *  Copyright(C) 2014 Burak Köken
 *
 *  This file is part of Uniq.
 *  
 *  Uniq is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, version 2 of the License.
 *
 *  Uniq is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Uniq.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <uniq/module.h>
#include <uniq/kernel.h>
#include <mm/mem.h>
#include <mm/heap.h>
#include <uniq/task.h>
#include <string.h>

#define PAGE_FAULT_INT		14

uint32_t *frame_map;
uint32_t nframe;
extern uint32_t last_addr;

page_dir_t *kernel_dir = NULL;
page_dir_t *current_dir = NULL;

/*
 * total_memory_size
 */
uint32_t total_memory_size(void){
	


}

/*
 * use_memory_size
 */
uint32_t use_memory_size(void){
	
	
	
}

/*
 * enable_paging
 */
void enable_paging(void){

	uint32_t cr0;
	__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	__asm__ volatile("mov %0, %%cr0" :: "r"(cr0));

}

/*
 * disable_paging
 */
void disable_paging(void){

	uint32_t cr0;
	__asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 &= ~0x80000000;
	__asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
	
}


/*
 * set_frame
 *
 * @param frame_addr :
 */
void set_frame(uintptr_t frame_addr){

	uint32_t frame  = frame_addr/FRAME_SIZE_BYTE;
	uint32_t index  = FRAME_INDEX_BIT(frame); 
	uint32_t offset = FRAME_OFFSET_BIT(frame);
	frame_map[index] |= (0x1 << offset); 

}

/*
 * remove_frame
 *
 * @param frame_addr :
 */
void remove_frame(uintptr_t frame_addr){
	
	uint32_t frame  = frame_addr/FRAME_SIZE_BYTE;
	uint32_t index  = FRAME_INDEX_BIT(frame); 
	uint32_t offset = FRAME_OFFSET_BIT(frame);
	frame_map[index] &= ~(0x1 << offset);

}

/*
 * cntrl_frame
 *
 * @param frame_addr :
 */
uint32_t cntrl_frame(uintptr_t frame_addr){
	
	uint32_t frame  = frame_addr/FRAME_SIZE_BYTE;
	uint32_t index  = FRAME_INDEX_BIT(frame); 
	uint32_t offset = FRAME_OFFSET_BIT(frame);
	
	return (frame_map[index] & (0x1 << offset));

}

/*
 * find_free_frame
 */
uint32_t find_free_frame(void){

	uint32_t index,offset;
	uint32_t offset_limit,max_index,fremainder;
	offset_limit = max_index = fremainder = 0;
	max_index = nframe / 32;
	fremainder = nframe % 32;
	#define MAX_OFFSET	32
	
	if(fremainder){
		max_index++
		offset_limit = fremainder;
	}
	
	
	for(index = 0; index < max_index; index++){
		
		if(frame_map[index] != MAX_LIMIT){
			
			if((offset_limit) && (index == max_index - 1)){
				for(offset = 0; offset < offset_limit; offset++){
					
					uint32_t cntrl = 0x1 << offset;
					if(!(frame_map[index] & cntrl))
						return index * 32 + offset;
				}
			}
			else{
				for(offset = 0; offset < MAX_OFFSET; offset++){
				
					uint32_t cntrl = 0x1 << offset;
					if(!(frame_map[index] & cntrl))
						return index * 32 + offset;	

				}
			}
			
		}
		
	}

}

/*
 * alloc_frame
 *
 * @param page :
 * @param rw :
 * @param user :
 */
void alloc_frame(page_t *page,bool rw,bool user){

	if(page->frame)
		return;

	uint32_t index = find_free_frame();

	if(index == MAX_LIMIT)
		printf("no free frames!");
	
	set_frame(index * FRAME_SIZE_BYTE);

	page->present = PAGE_PRESENT;
	page->rw      = (rw) ? PAGE_RWRITE : PAGE_RONLY;
	page->user    = (user) ? PAGE_USER_ACCESS : PAGE_KERNEL_ACCESS;
	page->frame   = index;
	
}

/*
 * dma_frame
 *
 * @param page :
 * @param rw :
 * @param user :
 * @param addr :
 */
void dma_frame(page_t *page,bool rw,bool user,uintptr_t addr){
	
	
	
}

/*
 * free_frame
 *
 * @param page :
 */
void free_frame(page_t *page){

	if(!page->frame)
		return;

	remove_frame(page->frame * FRAME_SIZE_BYTE);
	page->frame = 0;
	
}

/*
 * page_fault
 * 
 * @param regs :
 */
void page_fault(registers_t *regs){

	uint32_t fault_addr;
	__asm__ volatile("mov %%cr2, %0" : "=r"(fault_addr));
	
	uint32_t present = !(regs->err_code & 0x1);
	
	printf("page fault !");
	if(present)
		printf(" (present) ");
	halt_system();
 
}

/*
 * get_page
 *
 * @param addr :
 * @param make :
 * @param dir :
 */
page_t *get_page(uint32_t addr,bool make,page_dir_t *dir){

	addr /= FRAME_SIZE_BYTE;
	uint32_t table_index = addr / 1024;

	if(dir->tables[table_index])
		return &dir->tables[table_index]->pages[addr % 1024];
	else if(make){

		uint32_t temp;
		dir->tables[table_index] = (page_table_t *)kmalloc_aphysic(sizeof(page_table_t),&temp);
		memset(dir->tables[table_index],0,FRAME_SIZE_BYTE);
		dir->tables_physic[table_index] = temp | 0x7;	/* present,rw,us */
		return &dir->tables[table_index]->pages[addr % 1024];
	}
	else
		return NULL;

}

/*
 * change_page_dir
 *
 * @param new_dir :
 */
void change_page_dir(page_dir_t *new_dir){

	current_dir = new_dir;
	__asm__ volatile("mov %0, %%cr3" :: "r"(&new_dir->tables_physic));
	enable_paging();

}

/*
 * paging_init
 *
 * @param mem_size :
 */
void paging_init(uint32_t mem_size){
	
	nframe = mem_size / FRAME_SIZE_KIB;
	
	uint32_t alloc_frame_byte,frame_remainder;
	alloc_frame_byte = nframe / 8;
	frame_remainder = nframe % 8;

	if(frame_remainder)
		alloc_frame_byte++;
	
	frame_map = (uint32_t *)kmalloc(alloc_frame_byte);
 	memset(frame_map,NULL,alloc_frame_byte);
	
	kernel_dir = (page_dir_t *)kmalloc_align(sizeof(page_dir_t));
	memset(kernel_dir,NULL,sizeof(page_dir_t));
	current_dir = kernel_dir;

	uint32_t i = 0;
	while(i < last_addr){
		alloc_frame(get_page(i,true,kernel_dir),PAGE_RONLY,PAGE_KERNEL_ACCESS);
		i += FRAME_SIZE_BYTE;
	}

	isr_add_handler(PAGE_FAULT_INT,page_fault);
	change_page_dir(kernel_dir);

}
 
MODULE_AUTHOR("Burak Köken");
MODULE_LICENSE("GNU GPL v2");
