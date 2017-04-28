/*
Student Name: Mingyue Chen
Class section:6777
Instructor: Chien-Liang Liu
Due date: 03/17/2017
Brief description of file contents: Define two classes (Node and LinkedList) and some functions 
*/
#include <iostream>
#include <string>  
struct Node {
	int size;
	std::string name;
	Node *next;
	//Constuctors
	Node(std::string programName, int page) {
		name = programName;
		size = page;
		next = 0;
	}
	Node() {

	}
};
class LinkedList {
private:
	Node *head; //List head pointer
public:
	//Constructors
	LinkedList(std::string programName) {
		head = new Node("", 0);
		head->next = new Node("Free", 32);
	}
	LinkedList() {
		head = new Node("", 0);
	}
	//Create a function which can return head
	Node* getHead() { return head; }
};
//The following functions are defined in pa2.cpp
void addWorstFreeList(int pageUsed, LinkedList* Usedlist, LinkedList* Freelist, std::string programName);
void addBestFreeList(int usedPage, LinkedList* UsedList, LinkedList* freeList, std::string name);
void killProgram(LinkedList* usedList, LinkedList* freeList, std::string programName);
void mergeProgram(LinkedList* Freelist);
void PrintMemory(LinkedList* usedList, LinkedList* freeList);
void calculation(LinkedList* freeList);