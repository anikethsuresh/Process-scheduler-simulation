/* Fill in your Name and GNumber in the following two comment fields
 * Name: Aniketh Suresh
 * GNumber:	G01172069
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"

int schedule_count(Process *list) {
	int numberOfProcesses = 0;
	if(list == NULL){
		return numberOfProcesses;
	}
	else{
		Process *current = list;
		while(current != NULL){
			numberOfProcesses++;
			current = current->next;
		}
	}
  return numberOfProcesses;
}

void schedule_insert(Process **list, Process *node) {
  if(node == NULL){
  	return;
  }
  if(*list == NULL){
  	*list = node;
  	return;
  }
  // Case : Node inserted is the new head
  if(node->pid < (*list)->pid){
  	node->next = *list;
  	*list = node;
  	return;
  }
  Process *current = *list;
  while(current->next != NULL){
  	if((node->pid > current->pid) && (node->pid < current->next->pid)){
  		node->next = current->next;
  		current->next = node;
  		return;
  	}
  	current = current->next;
  }
  // Case :Node is to be inserted at the last position
  current->next = node;
  return;
}

void schedule_terminate(Process *node) {
  node->next = NULL;	
  free(node);
  return;
}

Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {
	Process* newProcess = malloc(sizeof(malloc));
	if (newProcess==NULL){
		printf("Malloc FAILED\n");
		return NULL;
	}
	else{
		strcpy(newProcess->name, name);
		newProcess->pid = pid;
		newProcess->time_remaining = time_remaining;
		newProcess->time_last_run = time_last_run;
		newProcess->next = NULL;
	}
  	return newProcess;
}

void schedule_remove_process(Process **list, Process *node){
	if(node == NULL || *list == NULL){
		return;
	}
	// Case: Node to be deleted is the head of the list
	if(node->pid == (*list)->pid){
		*list = (*list)->next;
		return;
	}
	// Node to be deleted is at any other position
	Process *current = *list;
	while(current->next != NULL){
		if(current->next->pid == node->pid){
			Process *nodeToDelete = current->next;
			current->next = current->next->next;
			nodeToDelete->next = NULL;
			return;
		}
		current = current->next;
	}
}


Process *schedule_select(Process **list) {
	if((*list) == NULL){
		return NULL;
	}
	Process *lowestTimeRemainingProcess = *list;
	Process *current = *list;

	// Case: Look for the first node(process) which is starving and remove it
	while(current!=NULL){
		if((clock_get_time() - current->time_last_run) >= TIME_STARVATION){
			// Remove the current node (process)
			schedule_remove_process(list, current);
			return current;
		}
		current = current->next;
	}

	// Case: There is no process starving. Search for the process with the shortest
	// 		 time remaining
	current = (*list)->next;
	while(current!=NULL){
		if(current->time_remaining < lowestTimeRemainingProcess->time_remaining){
			lowestTimeRemainingProcess = current;
		}
		current = current->next;
	}
	schedule_remove_process(list, lowestTimeRemainingProcess);
	return lowestTimeRemainingProcess;
}