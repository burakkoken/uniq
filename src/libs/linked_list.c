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
#include <linked_list.h>
#include <uniq/kernel.h>
#include <list.h>

/*
 * linked_list_create,bagli bir liste olusturur.
 */
linked_list_t *linked_list_create(void){

	linked_list_t *new_list = malloc(sizeof(linked_list_t));
	new_list->signature = LINKED_LIST_SIGNATURE;
	new_list->size = 0;
	new_list->first_node = new_list->last_node = NULL;

	return new_list;

}

/*
 * linked_list_link_next,
 *
 * @param linked_list :
 * @param node :
 * @param prev_node
 */
void linked_list_link_next(linked_list_t *linked_list,node_t *node,node_t *prev_node){


}

/*
 * linked_list_link_prev,
 *
 * @param linked_list :
 * @param node :
 * @param prev_node :
 */
void linked_list_link_prev(linked_list_t *linked_list,node_t *node,node_t *next_node){


}

/*
 * linked_list_push_prev,
 *
 * @param linked_list :
 * @param item :
 * @param next_node :
 */
node_t *linked_list_push_prev(linked_list_t *linked_list,void *item,node_t *next_node){


}

/*
 * linked_list_push_next,
 *
 * @param linked_list :
 * @param item :
 * @param prev_node :
 */
node_t *linked_list_push_next(linked_list_t *linked_list,void *item,node_t *prev_node){
	
	
}

/*
 * linked_list_clone, verilen listenin klonunu olusturur.
 *
 * @param linked_list : bagli liste
 */
linked_list_t *linked_list_clone(linked_list_t *linked_list){

	if(!linked_list)
		return NULL;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");
	
	linked_list_t *clone = linked_list_create();
	node_t *node = linked_list->first_node;

	for(;node;node = node->next)
		linked_list_push(clone,node->item);

	return clone;
	
}

/*
 * linked_list_merge, iki listeyi birbirine baglar.
 *
 * @param dest : hedef liste
 * @param src : kaynak liste
 */
linked_list_t *linked_list_merge(linked_list_t *dest,linked_list_t *src){

	if(!dest || !src)
		return NULL;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(dest->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature(destination linked list)");
	assert(src->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature(src linked list)");

	node_t *node = src->first_node;
	for(;node;node = node->next)
		node->link_list = dest;

	/*
	 * eger kaynak listede dugum varsa
	 */
	if(src->first_node)
		src->first_node->prev = dest->last_node;
	else
		return dest;

	/*
	 * eger hedef listenin ilk dugumu bos ise
	 */
	if(!dest->first_node)
		dest->first_node = src->first_node;
	else
		dest->last_node->next = src->first_node;

	/*
	 * son dugum degisimi
	 */
	if(src->last_node)
		dest->last_node = src->last_node;

	free(src);
	dest->size += src->size;

	return dest;

}

/*
 * linked_list_free, bagli listedeki dugumleri siler.
 *
 * @param linked_list : bagli liste
 */
void linked_list_free(linked_list_t *linked_list){

	if(!linked_list)
		return;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	node_t *node = linked_list->first_node;

	for(;node;node = node->next)
		free(node);

	linked_list->size = 0;
	linked_list->first_node = linked_list->last_node = NULL;

}

/*
 * linked_list_clear, bagli liste olarak verilen listenin tum 
 * dugumlerindeki elemanlari bosa cikarir.
 *
 * @param linked_list : bagli liste
 */
void linked_list_clear(linked_list_t *linked_list){

	if(!linked_list)
		return;

	/* 
 	 * linked_list imza kontrolu
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	node_t *node = linked_list->first_node;

	for(;node;node = node->next)
		free(node->item);

}

/*
 * linked_list_destroy, bagli listeyi tum icerigiyle birlikte
 * siler.(bellekte bosa cikarir)
 *
 * @param linked_list : bagli liste
 */
void linked_list_destroy(linked_list_t *linked_list){

	if(!linked_list)
		return;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	linked_list_clear(linked_list);
	linked_list_free(linked_list);
	free(linked_list);

}


/*
 * linked_list_link, verilen dugumu bagli listeye baglar.
 *
 * @param linked_list : bagli
 * @param node : eklenecek dugum
 */
void linked_list_link(linked_list_t *linked_list,node_t *new_node){

	if(!linked_list || !new_node)
		return;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	new_node->link_list = linked_list;
	/*
	 * listenin ilk dugumu bos ise
	 */
	if(!linked_list->first_node){

		linked_list->first_node = new_node;
		linked_list->last_node = new_node;
		new_node->next = new_node->prev = NULL;

	}
	else{
	
		linked_list->last_node->next = new_node;
		new_node->prev = linked_list->last_node;
		new_node->next = NULL;
		linked_list->last_node = new_node;

	}

	linked_list->size++;

}

/*
 * linked_list_push, verilen itemi belirtilen bagli listeye
 * ekler, ve bu itemin dugumunu dondurur.
 *
 * @param linked_list : bagli liste
 * @param item : eklenecek item
 */
node_t *linked_list_push(linked_list_t *linked_list,void *item){
	
	if(!linked_list || !item)
		return NULL;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	/*
 	 * yeni dugum
	 */
	node_t *new_node = malloc(sizeof(node_t));
	new_node->prev = new_node->next = NULL;
	new_node->item = item;
	new_node->link_list = linked_list;
	linked_list_link(linked_list,new_node);

	return new_node;

}

/*
 * linked_list_unlink, belirtilen dugumu bagli listeden ayirir.
 *
 * @param linked_list : bagli liste
 * @param node : dugum
 */
void linked_list_unlink(linked_list_t *linked_list,node_t *node){

	if(!linked_list || !node)
		return;

	/* 
 	 * linked_list imza kontrolu
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	/*
	 * dugum belirtilen listeye ait olmasi gerekiyor.
	 */
	assert(node->link_list == linked_list);
	
	/* eger ilk dugum ise*/
	if(linked_list->first_node == node)
		linked_list->first_node = node->next;

	/* son dugum ise */
	if(linked_list->last_node == node)
		linked_list->last_node = node->prev;

	if(node->next)
		((node_t*)node->next)->prev = node->prev;
	
	if(node->prev)
		((node_t*)node->prev)->next = node->next;

	linked_list->size--;
	node->next = node->prev = node->link_list = NULL;

}

/*
 * linked_list_pop, bagli listenin sonundan bir dugum ayirir.
 *
 * @param linked_list : bagli liste
 */
node_t *linked_list_pop(linked_list_t *linked_list){

	if(!linked_list)
		return NULL;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	if(!linked_list->last_node)
		return NULL;

	node_t *node = linked_list->last_node;
	linked_list_unlink(linked_list,node);

	return node;

}

/*
 * linked_list_remove,belirtilen index'teki elemani bagli listeden
 * siler.
 *
 * @param linked_list : bagli liste.
 * @param node_index : dugum index'i.
 */
void linked_list_remove(linked_list_t *linked_list,uint32_t node_index){

	if(!linked_list || linked_list->size < node_index)
		return;
	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	uint32_t i = 0;
	node_t * node = linked_list->first_node;
	for(;node && i < node_index;node = node->next,i++);

	if(!node)
		return;

	linked_list_unlink(linked_list,node);

}

/*
 * linked_list_search, aranan degerin dugumunu dondurur.
 *
 * @param linked_list : bagli liste
 * @param search : aranan deger
 */
node_t *linked_list_search(linked_list_t *linked_list,void *search){

	if(!search)
		return NULL;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	node_t *node = linked_list->first_node;
	for(;node;node = node->next){

		if(node->item == search)
			return node;	
	
	}

	return NULL;

}

/*
 * linked_list_get_index, aranan degerin index'ini bulur ve geri
 * dondurur. eger bulamazsa -1 doner.
 *
 * @param linked_list : bagli liste
 * @param search : aranan deger
 */
int32_t linked_list_get_index(linked_list_t *linked_list,void *search){

	if(!search)
		return -1;

	/* 
 	 * linked_list imza kontrolu 
	 */
	assert(linked_list->signature == LINKED_LIST_SIGNATURE && "Wrong! linked list signature");

	int32_t index = 0;
	node_t *node = linked_list->first_node;

	for(;node;node = node->next,index++){
		if(node->item == search)
			return index;

	}

	/* bulunamadi :/ */
	return -1;

}

/*
 * __linked_list_test,
 */
void __linked_list_test(void){

	debug_print(KERN_INFO,"linked_list test...");

#if 0	/* test-0 */
	
	debug_print(KERN_INFO,"test-0\n");
	list_t *list = list_create();
	debug_print(KERN_DUMP,"linked_list : %P",list);
	list_destroy(list);

	list_t *new_list = list_create();
	debug_print(KERN_DUMP,"new linked_list : %P",new_list);

#endif

#if 0	/* test-1 */

	debug_print(KERN_INFO,"\ntest-1");
	uint32_t *x = malloc(4);
	list_t *list = list_create();
	debug_print(KERN_DUMP,"linked_list : %P, x : %P",list,x);
	debug_print(KERN_DUMP,"first_node : %P",list->first_node);
	
	list_push(list,x);
	debug_print(KERN_DUMP,"first_node : %P",list->first_node);
	debug_print(KERN_DUMP,"last_node : %P",list->last_node);
	debug_print(KERN_DUMP,"item : %P",list->first_node->item);

	#if 1	/* test-1.1 */
		debug_print(KERN_DUMP,"\ntest-1.1");
		node_t *node = list_pop(list);
		debug_print(KERN_DUMP,"node : %P",node);	
		debug_print(KERN_DUMP,"node -> item : %P",node->item);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);
	
		/* test-1.2 */
		debug_print(KERN_DUMP,"\ntest-1.2");
		list_link(list,node);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);

		/* test-1.3 */
		debug_print(KERN_DUMP,"\ntest-1.3");
		uint32_t *y = malloc(4);
		list_push(list,y);
		int8_t index = list_get_index(list,y);
		debug_print(KERN_DUMP,"item index : %d, y : %P",index,y);
		
		node = list_search(list,x);
		debug_print(KERN_DUMP,"node : %P",node);	
		debug_print(KERN_DUMP,"node -> item : %P",node->item);
	
		/* test-1.4 */
		debug_print(KERN_DUMP,"\ntest-1.4");
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);		
		list_unlink(list,node);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);
		list_link(list,node);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);

		/* test-1.5 */
		debug_print(KERN_DUMP,"\ntest-1.5");
		list_remove(list,1);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);
	#endif

#endif

#if 0	/* test-2 */
	debug_print(KERN_INFO,"\ntest-2");
	uint32_t *x = malloc(4);
	list_t *list = list_create();
	debug_print(KERN_DUMP,"linked_list : %P, x : %P",list,x);
	debug_print(KERN_DUMP,"first_node : %P",list->first_node);
	
	list_push(list,x);
	debug_print(KERN_DUMP,"first_node : %P",list->first_node);
	debug_print(KERN_DUMP,"last_node : %P",list->last_node);
	debug_print(KERN_DUMP,"item : %P",list->first_node->item);

	#if 1	/* test-2.1 */
		debug_print(KERN_DUMP,"\ntest-2.1");
		list_destroy(list);
		list_t *new_list = list_create();
		uint32_t *y = malloc(4);
		debug_print(KERN_DUMP,"new linked_list : %P, y : %P",new_list,y);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);

		/* test-2.2 */
		debug_print(KERN_DUMP,"\ntest-2.2");
		list_push(list,y);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);
		list_free(list);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);
		
		node_t *new_node = malloc(sizeof(node_t));
		uint32_t *w = malloc(4);
		debug_print(KERN_DUMP,"new node : %P, w : %P",new_node,w);
		free(new_node);

		/* test-2.3 */
		debug_print(KERN_DUMP,"\ntest-2.3");
		list_push(list,w);
		list_push(list,y);
		debug_print(KERN_DUMP,"first_node : %P",list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",list->last_node);
		list_clear(list);
		uint32_t *h = malloc(4);
		uint32_t *n = malloc(4);
		debug_print(KERN_DUMP,"h : %P, n : %P",h,n);

		/* test-2.4 */
		debug_print(KERN_DUMP,"\ntest-2.4");
		list_destroy(list);
		list_t *end_list = list_create();
		uint32_t *r = malloc(4);
		uint32_t *t = malloc(4);
		debug_print(KERN_DUMP,"new linked_list : %P, r : %P, t : %P",end_list,r,t);
		debug_print(KERN_DUMP,"first_node : %P",end_list->first_node);
		debug_print(KERN_DUMP,"last_node : %P",end_list->last_node);
	#endif
#endif


#if 0	/* test-3 */
	debug_print(KERN_INFO,"\ntest-3");
	uint32_t *x = malloc(4);
	list_t *list = list_create();
	debug_print(KERN_DUMP,"linked_list : %P, x : %P",list,x);
	debug_print(KERN_DUMP,"first_node : %P, size : %u",list->first_node,list->size);
	list_push(list,x);
	debug_print(KERN_DUMP,"first_node : %P, size : %u",list->first_node,list->size);

	#if 1	/* test-3.1 */
		debug_print(KERN_DUMP,"\ntest-3.1");
		list_t *clone = list_clone(list);
		debug_print(KERN_DUMP,"clone list : %P",clone);
		debug_print(KERN_DUMP,"first_node : %P, size : %u",list->first_node,list->size);

		/* test-3.2 */
		debug_print(KERN_DUMP,"\ntest-3.2");
		list_destroy(list);
		list_destroy(clone);
		list_t *list1 = list_create();
		uint32_t *w = malloc(4);
		list_push(list1,w);
		list_t *list2 = list_create();
		list_t *list3 = list_create();
		debug_print(KERN_DUMP,"list1 : %P, list2 : %P, list3 : %P",list1,list2,list3);
		debug_print(KERN_DUMP,"list1 first_node : %P",list1->first_node);
		debug_print(KERN_DUMP,"list1 last_node : %P",list1->last_node);
		debug_print(KERN_DUMP,"list2 first_node : %P",list2->first_node);
		debug_print(KERN_DUMP,"list2 last_node : %P",list2->last_node);
	#endif
#endif

#if 1	/* test-4 */

#endif

}

MODULE_AUTHOR("Burak Köken");
MODULE_LICENSE("GNU GPL v2");
