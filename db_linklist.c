/*
 * =====================================================================================
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc.
#
# See the COPYING file for license information.
#
# Copyright (c) 2013 neo <xuechuance@gmail.com>
 * =====================================================================================
 */

#include "double_linklist.h"

db_list_t* db_list_create(void ){
	db_list_t* list_head;
	list_head=(db_list_t* )malloc(sizeof(db_list_t));
	if(list_head==NULL){
	      errno=ENOMEM;
	      return NULL;
	}
	
	list_head->list_size=0;
	list_head->head=(db_lnode_t* )malloc(sizeof(db_lnode_t));
	if(list_head->head==NULL){
		free(list_head);
		errno=ENOMEM;
		return NULL;
	}
	list_head->head->next=list_head->head->prev=NULL;
	list_head->head->data=NULL;
	list_head->tail=list_head->head;
	return list_head;
}

int db_list_insert_before(db_list_t** list_head, int num, void* new_node_data){
	int counter=1;
	db_lnode_t* current;
	db_lnode_t* new_node;
	if( *list_head==NULL || list_head==NULL){
		return -ENOMEM;
	}
	if((*list_head)->list_size!=0){
		new_node=(db_lnode_t* )malloc(sizeof(db_lnode_t));
		if(new_node==NULL){
			return -ENOMEM;
		}
		new_node->data=new_node_data;
		new_node->prev=new_node->next=NULL;
		if(num>0 && num<=(*list_head)->list_size){
			current=(*list_head)->head;
			while(counter<num){
				counter++;
				current=current->next;
			}
			if(counter==1){
				(*list_head)->head->prev=new_node;
				new_node->next=(*list_head)->head;
				(*list_head)->head=new_node;
				(*list_head)->list_size++;
				return 0;
			}
			current->next->prev=new_node;
			new_node->prev=current->prev;
			current->prev=new_node;
			new_node->next=current;
			(*list_head)->list_size++;
			return 0;
		}else{
			free(new_node);
			return -EINVAL;
		}
	}else{
		if(num!=0){
			return -EINVAL;
		}
		(*list_head)->head->data=new_node_data;
		(*list_head)->list_size++;
		return 0;
	}
}

int db_list_insert_after(db_list_t** list_head ,int num ,void* new_node_data){
	int counter=0;
	db_lnode_t* current;
	db_lnode_t* new_node;
	if((*list_head)==NULL || list_head==NULL){
		norco_LOGE("list_head == NULL");
		return -EINVAL;
	}
	if((*list_head)->list_size!=0){
		new_node=(db_lnode_t* )malloc(sizeof(db_lnode_t));
		if(new_node==NULL){
			norco_LOGE("new_node == NULL");
			return -ENOMEM;
		}
		new_node->data=new_node_data;
		new_node->next=new_node->prev=NULL;
		if(num>0&&num<=(*list_head)->list_size){
			current=(*list_head)->head;
			// sqlite3_field_t *sqlite_field_tem1= (sqlite3_field_t *)new_node_data;
			// norco_LOGD("插入数据:%s", sqlite_field_tem1->s_field_name);
			// sqlite3_field_t *sqlite_field_tem2= (sqlite3_field_t *)(current->data);
			// norco_LOGD("当前链表数据2:%s", sqlite_field_tem2->s_field_name);
			while(counter!=num-1){
				counter++;
				current=current->next;
			}

			// sqlite3_field_t *sqlite_field_tem3= (sqlite3_field_t *)(current->data);
			// norco_LOGD("当前链表数据3:%s\n", sqlite_field_tem3->s_field_name);

			if(current->next==NULL){
				(*list_head)->tail=new_node;
				current->next=new_node;
				new_node->prev=current;
				(*list_head)->list_size++;
				return 0;
			}
			new_node->prev=current;
			new_node->next=current->next;
			current->next->prev=new_node;
			current->next=new_node;
			(*list_head)->list_size++;
			return 0;
		}else{
			free(new_node);
			norco_LOGE("free new_node, num did not match");
			return -EINVAL;
		}
	}else{
		if(num!=0){
			norco_LOGE("num != 0");
			return -EINVAL;
		}
		(*list_head)->head->data=new_node_data;
		(*list_head)->list_size++;
		return 0;
	}
}

int db_lnode_flush(db_list_t* list_head ,int num ,void* new_node_data){
	int counter=0;
	db_lnode_t* current;
	if(list_head==NULL || num<0 || num>list_head->list_size){
		return -EINVAL;
	}
	current=list_head->head;
	while(counter!=num-1){
		current=current->next;
		counter++;
	}
	current->data=new_node_data;

	return 0;
}

int db_list_delete(db_list_t** list_head ,int num){
	int counter=1;
	db_lnode_t* current;
	db_lnode_t* tmp;
	if((*list_head)==NULL||list_head==NULL){
		return -EINVAL;
	}
	current=(*list_head)->head;
	while(counter<num){
		counter++;
		current=current->next;
	}
	if(counter==1){
		tmp=(*list_head)->head;
		(*list_head)->head=(*list_head)->head->next;
		free(tmp);
		(*list_head)->head->prev=NULL;
		(*list_head)->list_size--;
		return 0;
	}
	if((*list_head)->list_size==counter){
		tmp=(*list_head)->tail;
		(*list_head)->tail=(*list_head)->tail->prev;
		free(tmp);
		(*list_head)->tail->next=NULL;
		(*list_head)->list_size--;
		return 0;
	}
	tmp=current;
	current->next->prev=current->prev;
	current->prev->next=current->next;
	free(tmp);
	(*list_head)->list_size--;

	return 0;
}

int db_list_destory(db_list_t* list_head){
	db_lnode_t* current;
	db_lnode_t* pos;
	if(list_head==NULL){
		return -EINVAL;
	}
	current=pos=list_head->head;
	if(list_head->list_size!=0){
		for(int i=0;i<=list_head->list_size;i++){

			/* 将数据区分配的内存free掉 */
			if(current->data != NULL)
			{
				free(current->data);
				current->data = NULL;
			}

			current=current->next;
			free(pos);
			pos=current;
		}
		free(list_head);
	}

	return 0;
}


void* db_list_visit(db_list_t** list_head ,int num){
	int counter;
	db_lnode_t* current=(*list_head)->head;
	if(num < 0 || num > (*list_head)->list_size){
		errno=EINVAL;
		return NULL;
	}
	for(counter=0; counter<num; counter++){
		current=current->next;
	}
	return current->data;
}

int db_list_travel(db_list_t* list_head ,void(*do_function)(void* , int)){
	if(list_head->list_size<0 || list_head==NULL){
		errno=EINVAL;
		return -EINVAL;
	}
	for(int i=0;i<list_head->list_size;i++){
		(*do_function)(db_list_visit( &list_head, i), i+1);
	}
}

int db_list_search(
		db_list_t** list_head,void* find_data ,int(*compare)(void* ,void* )){
	int counter=1;
	db_lnode_t* current;
	if((*list_head)==NULL||list_head==NULL){
		errno=EINVAL;
		return -EINVAL;
	}
	current=(*list_head)->head;
	while(compare(current->data,find_data)!=0 && current->next!=NULL){
		current=current->next;
		counter++;
	}
	if(current->next==NULL && compare(current->data,find_data)!=0)
	      return 0;
	return counter;
}
