// CS3243 Operating Systems
// Summer 2014 
// Project 3: Forks and Threads
// Matthew Tinney and Shade Alabsa
// Date: 10 June 2014
// File: partthree.cpp

#include "partthree_h"

int getLenth(struct node* head) {
	struct node* current = head;
	int count = 0;

	while (current != NULL) {
		count++;
		current = current->next;
	}

	return count;
}
boolean insertAtEnd(struct node* head, int data) {
	struct node* current = head;
	boolean success = false;

	struct node* temp;
	temp = malloc(sizeof(struct node));
	temp->data = data;
	temp->next = NULL;

	while (current != NULL) {
		current = current->next;
	}

	current->next = element;

	if (current->next == element && element->next == NULL) {
		success = true;
	}

	return success;
}

boolean insertAtBeginning(struct node** headRef, int data) {
	boolean success = false;
	struct node* temp;

	temp = malloc(sizeof(struct node));
	temp->data = data;
	temp->next = *headRef;
	*headRef = temp;

	if (*headRef == temp && *headRef->next != NULL) {
		success = true;
	}

	return success;
}

boolean remove(struct node* head, struct node* element) {
	struct node* current = head;
	boolean success = false;

	while (current->next != element) {
		current = current->next;
	}

	current->next = element->next;

	if (current->next == element->next){
		success = true;
		//delete element
	}

	return success;
}