/*
Student Name: Mingyue Chen
Class section:6777
Instructor: Chien-Liang Liu
Due date: 03/17/2017
Brief description of file contents: Write a C++ program that emulates the operating system's responsibility of allocating
memory to certain programs.
*/
#include"pa2.h"
int main(const int argc, const char **argv) {
	//argc is argument count, argv is an array of c_strings
	std::string *cppArgs = new std::string[argc];
	for (int i = 0; i != argc; ++i) {
		cppArgs[i] = std::string(argv[i]);
	}
	//utilize cppArgs
	if (argc == 2 && cppArgs[1] == "worst") {
		std::cout << "Using worst fit algorithm" << std::endl;
	}
	else if (argc == 2 && cppArgs[1] == "best") {
		std::cout << "Using best fit algorithm" << std::endl;
	}
	else {
		std::cout << "Please enter \"worst\" or \"best\" to start testing this program." << std::endl << std::endl;
		return 1;
	}
	//Create two linked lists -- free list and used list
	LinkedList *freeList = new LinkedList("Free");
	LinkedList *UsedList = new LinkedList;

	//Display the menu
	std::cout << '\n' << "    1. Add program" << '\n' << "    2. Kill program" << '\n';
	std::cout << "    3. Fragmentation" << '\n' << "    4. Print memory" << '\n' << "    5. Exit" << '\n' << '\n';
	//Create some variables
	std::string name;
	int choice=0, size = 0, usedPage;
	double tempChoice, tempSize;
	bool go = true;

	while (go) {
		//Prompt the user to choose what they want to do (add a program, kill a program, calculate fragments, print memory, or exit)
		std::cout << "choice - ";
		std::cin >> tempChoice;
		//Check if the input is a string	
		if (!std::cin) {
			//Erase any string in the buffer 
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			std::cout << '\n' << "Stop Entering Text." << std::endl << std::endl;
			continue;
		}

		//Check if the choice is out of range
		if (tempChoice < 1 || tempChoice >5) {
			std::cout << '\n' << "Please enter an integer between 1 and 5." << '\n' << '\n';
			//Erase any string in the buffer 
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			continue;
		}
		//Check if the choice is a decimal
		if ((int)tempChoice != tempChoice) {
			std::cout << '\n' << "Sorry, you cannot enter a decimal for your program size." << std::endl << '\n';
			//Erase any string in the buffer 
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			continue;
		}

		//Assign a valid input to "choice"
		choice = tempChoice;
		//Add a program
		if (choice == 1) {
			//Prompt the user to enter the program name
			std::cout << "Program name - ";
			std::cin >> name;
			//Prompt the user to enter the Program size (KB)
			std::cout << "Program size (KB) - ";
			std::cin >> tempSize;
			//Check if the size is a string	
			if (!std::cin) {
				//Erase any string in the buffer 
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				std::cout << '\n' << "Stop Entering Text." << std::endl << std::endl;
				continue;
			}
		
			//Check if the size is positive
			if (tempSize <= 0) {
				std::cout << '\n' << "Sorry, your size should be greater than 0." << '\n' << '\n';
				continue;
			}	
			//Check if the program size is a decimal
			if ((int)tempSize != tempSize) {
				std::cout << '\n' << "Sorry, you cannot enter a decimal for your program size." << std::endl << '\n';
				//Erase any string in the buffer 
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				continue;
			}
			//Assign the valid input to "size"
			size = tempSize;
			//Calculate how many pages are used
			usedPage = (size + 3) / 4;
			//Check if there is enough memory for the program 
			if (usedPage > 128) {
				std::cout << '\n' << "Error, Not enough memory for Program " << name << '\n' << '\n';
				continue;
			}
			else {
				//Using worst fit algorithm
				if (cppArgs[1] == "worst") {
					addWorstFreeList(usedPage, UsedList, freeList, name);
				}
				//Using best fit algorithm
				else if (cppArgs[1] == "best") {
					addBestFreeList(usedPage, UsedList, freeList, name);
				}
			}
		}
		//Kill a program
		else if (choice == 2) {
			//Prompt the user to enter the program name
			std::cout << "Program name - ";
			std::cin >> name;
			killProgram(UsedList, freeList, name);
			mergeProgram(freeList);
		}
		//Calculate fragments
		else if (choice == 3) {
			calculation(freeList);
		}
		//Print memory
		else if (choice == 4) {
			std::cout << '\n';
			PrintMemory(UsedList, freeList);
		}
		//Exit
		else if (choice == 5) {
			go = false;
		}
	}
	return 0;
}

/****************************************************************************************************************
 *                                           addWorstFreeList                                                    *
 * This function uses the page used, the used list, the free list, and the program name passed to it             *
 * as arguments to add programs to the used list and the free list by using the worst-fit strategy. This         *
 * function also checks if the program to be added is already running or if the free memory is enough for        *
 * the program. Corresponding messages will be printed out to tell the user whether the program is added or not. *
 *****************************************************************************************************************/
void addWorstFreeList(int pageUsed, LinkedList* usedList, LinkedList* Freelist, std::string programName) {
	//"prev", "curr", "maxNode", and "maxNodePrev" will be used as pointers for the free list
	Node* prev = Freelist->getHead();
	Node* curr = prev->next;
	Node* maxNode = NULL;
	Node* maxNodePrev = NULL;
	//"prevUsed" and "currUsed" will be used as pointers for the used list
	Node *prevUsed = usedList->getHead();
	Node *currUsed = prevUsed->next;
	//Create some variables
	int max = 0;
	int UsedNodesInUsed = 0;
	int UsedNodesInFree = 0;
	//Check if the program is already running
	while (currUsed != NULL) {
		if (currUsed->name == programName) {
			std::cout << '\n' << "Error, Program " << programName << " already running." << std::endl << std::endl;
			return;
		}
		prevUsed = currUsed;
		currUsed = currUsed->next;
	}
	//Look for the largest available block for the program
	while (curr != NULL) {
		if (curr->name == "Free") {
			if (curr->size > max && pageUsed <= curr->size) {
				max = curr->size;
				maxNode = curr;
				maxNodePrev = prev;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	//Print out a message when there is not enough memory for the program
	if (max == 0) {
		std::cout << '\n' << "Error, Not enough memory for Program " << programName << std::endl << std::endl;
	}
	//There is enough memory for the program
	else {
		//Create a new node
		Node* newNode = new Node;
		newNode->name = "Used";
		newNode->size = pageUsed;
		//Insert the new node between "maxNode" and "maxNodePrev" and change the size of the node which "maxNode" points to
		maxNode->size = maxNode->size - pageUsed;
		maxNodePrev->next = newNode;
		newNode->next = maxNode;
		//Make "prev" and "curr" point to the head and the first node of the free list, respectively
		prev = Freelist->getHead();
		curr = prev->next;
		//Count the number of used nodes before the newly inserted node in the free list
		while (curr != newNode) {
			if (curr->name == "Used") {
				UsedNodesInFree++;
			}
			prev = curr;
			curr = curr->next;
		}
		//Make "prevUsed" and "currUsed" point to the head and the first node (or NULL if the used list is empty) of the used list, respectively
		prevUsed = usedList->getHead();
		currUsed = prevUsed->next;
		//Find the right position of the new node in the used list
		while (currUsed != NULL) {
			if (UsedNodesInUsed == UsedNodesInFree) {
				break;
			}
			prevUsed = currUsed;
			currUsed = currUsed->next;
			UsedNodesInUsed++;
		}
		//Insert a new node in the used list
		if (UsedNodesInUsed == UsedNodesInFree) {
			Node *newNodeInUsed = new Node;
			newNodeInUsed->size = pageUsed;
			newNodeInUsed->name = programName;
			prevUsed->next = newNodeInUsed;
			newNodeInUsed->next = currUsed;
		}
		//Print out a message if the program is added successfully
		std::cout << '\n' << "Program " << programName << " added successfully: " << pageUsed << " page(s) used." << '\n' << '\n';
	}
}

/****************************************************************************************************************
 *                                           addBesttFreeList                                                    *
 * This function uses the page used, the used list, the free list, and the program name passed to it             *
 * as arguments to add programs to the used list and the free list by using the best-fit strategy. This          *
 * function also checks if the program to be added is already running or if the free memory is enough for        *
 * the program. Corresponding messages will be printed out to tell the user whether the program is added or not. *
 *****************************************************************************************************************/
void addBestFreeList(int pageUsed, LinkedList* usedList, LinkedList* freeList, std::string programName) {
	//"prev", "curr", "minNode", and "minNodePrev" will be used as pointers for the free list
	Node* prev = freeList->getHead();
	Node* curr = prev->next;
	Node* minNode = NULL;
	Node* minNodePrev = NULL;
	//"prevUsed" and "currUsed" will be used as pointers for the used list
	Node *prevUsed = usedList->getHead();
	Node *currUsed = prevUsed->next;
	//Create some variables
	int min = 33;
	int UsedNodesInUsed = 0;
	int UsedNodesInFree = 0;
	//Check if the program is already running
	while (currUsed != NULL) {
		if (currUsed->name == programName) {
			std::cout << '\n' << "Error, Program " << programName << " already running." << std::endl << std::endl;
			return;
		}
		prevUsed = currUsed;
		currUsed = currUsed->next;
	}
	//Look for the smallest available block for the program
	while (curr != NULL) {
		if (curr->name == "Free") {
			if ((curr->size < min) && (pageUsed <= curr->size)) {
				min = curr->size;
				minNode = curr;
				minNodePrev = prev;
			}
		}
		prev = curr;
		curr = curr->next;
	}
	//Print out a message when there is not enough memory for the program
	if (min == 33) {
		std::cout << '\n' << "Error, Not enough memory for Program " << programName << std::endl << '\n';
	}
	//There is enough memory for the program
	else {
		//Create a new node
		Node* newNode = new Node;
		newNode->name = "Used";
		newNode->size = pageUsed;
		//Insert the new node between "minNode" and "minNodePrev" and change the size of the node which "minNode" points to
		minNode->size = minNode->size - pageUsed;
		minNodePrev->next = newNode;
		newNode->next = minNode;
		//Make "prev" and "curr" point to the head and the first node of the free list, respectively
		prev = freeList->getHead();
		curr = prev->next;
		//Count the number of used nodes before the newly inserted node in the free list
		while (curr != newNode) {
			if (curr->name == "Used") {
				UsedNodesInFree++;
			}
			prev = curr;
			curr = curr->next;
		}
		//Make "prevUsed" and "currUsed" point to the head and the first node (or NULL if the used list is empty) of the used list, respectively
		prevUsed = usedList->getHead();
		currUsed = prevUsed->next;
		//Find the right position of the new node in the used list
		while (currUsed != NULL) {
			if (UsedNodesInUsed == UsedNodesInFree) {
				break;
			}
			prevUsed = currUsed;
			currUsed = currUsed->next;
			UsedNodesInUsed++;
		}
		//Insert a new node in the used list
		if (UsedNodesInUsed == UsedNodesInFree) {
			Node *newNodeInUsed = new Node;
			newNodeInUsed->size = pageUsed;
			newNodeInUsed->name = programName;
			prevUsed->next = newNodeInUsed;
			newNodeInUsed->next = currUsed;
		}
		//Print out a message if the program is added successfully
		std::cout << '\n' << "Program " << programName << " added successfully: " << pageUsed << " page(s) used." << '\n' << '\n';
	}
}

/************************************************************************************************
 *                                          KillProgram                                         *
 *This function uses the used list, the free list, and the program name passed to it as         *
 *arguments to kill a program. This function also checks if the program to be deleted exists.   *
 *Corresponding messages will be printed out to tell the user if the program is deleted or not. *
 ************************************************************************************************/
void killProgram(LinkedList* usedList, LinkedList* freeList, std::string programName) {
	//Create some variables
	int pageUsed = -1, usedNodesInUsed = 0, usedNodesInFree = 0;
	//"prevUsed" and "currUsed" are used as pointers for the used list. "prev" and "curr" are used as pointers for the free list.
	Node* prevUsed = usedList->getHead();
	Node* currUsed = prevUsed->next;
	Node* prev = freeList->getHead();
	Node* curr = prev->next;
	//Delete the program in the used list
	while (currUsed != NULL) {
		if (currUsed->name == programName) {
			//Delete the program
			prevUsed->next = currUsed->next;
			pageUsed = currUsed->size;
			delete currUsed;
			break;
		}
		prevUsed = currUsed;
		currUsed = currUsed->next;
	}
	//Check if the program exists 
	if (pageUsed == -1) {
		std::cout << '\n' << "The program you want to delete does not exist." << std::endl << '\n';
		return;
	}
	//The program that the user wants to delete exists
	else {
		//Create a node "checkUsed" to find the the number of used nodes before the deleted program in the used list
		Node *checkUsed = usedList->getHead();
		//Make "prevUsed" point to the node right after the deleted program in the used list
		prevUsed = prevUsed->next;
		checkUsed = checkUsed->next;
		//Count the number of used nodes before the deleted program in the used list
		while (checkUsed != prevUsed) {
			usedNodesInUsed++;
			checkUsed = checkUsed->next;
		}
		//In the free list, find the used node whose corresponding node in the used list was deleted and mark the used node as "Free"
		while (curr != NULL) {
			if ((usedNodesInUsed == usedNodesInFree) && (curr->name != "Free")) {
				curr->name = "Free";
				break;
			}
			else if (curr->name == "Used") {
				usedNodesInFree++;
			}
			prev = curr;
			curr = curr->next;
		}
		//Print out a message if the program is killed successfully
		std::cout << '\n' << "Program " << programName << " successfully killed, " << pageUsed << " page(s) reclaimed." << '\n' << '\n';
	}
}

/*************************************************************************************
 *                              mergeProgram                                         *
 *This function uses the free list passed to it as arguments to combine free memory. *
 *************************************************************************************/
void mergeProgram(LinkedList* Freelist) {
	//"prev" and "curr" are used as pointers for the free list.
	Node* prev = Freelist->getHead();
	Node* curr = prev->next;
	//Merge free memory
	while (curr != NULL) {
		if (prev->name == "Free" && curr->name == "Free") {
			//Create a new pointer
			Node* checkCurr = NULL;
			prev->next = curr->next;
			prev->size = prev->size + curr->size;
			//"checkCurr" points to the node to be deleted
			checkCurr = curr;
			curr = prev;
			delete checkCurr;
		}
		prev = curr;
		curr = curr->next;
	}
}

/******************************************************************************************************
 *                                           PrintMemory                                              *
 * This function uses the used list and the free list passed to it as arguments to print out memory.  *
 ******************************************************************************************************/
void PrintMemory(LinkedList* usedList, LinkedList* freeList) {
	//"prev" and "curr" are used as pointers for the free list. "prevUsed" and "currUsed" are used as pointers for the used list.
	Node* prev = freeList->getHead();
	Node* curr = prev->next; //the used list can be empty, so currUsed can be null
	Node* prevUsed = usedList->getHead();
	Node* currUsed = prevUsed->next;
	//"counter" is created to move one line forward when the name of the program or "Free" are printed out eight times
	int counter = 0;
	while (curr != NULL) {
		if (curr->name == "Used") {
			//If the name in the free list is "Used", then the program will go to the used list to print out the name of the program
			for (int i = 1; i <= currUsed->size; i++) {
				std::cout << currUsed->name << "\t";
				counter++;
				//move one line forward 
				if (counter >= 8) {
					std::cout << std::endl;
					counter -= 8;
				}
			}
			prevUsed = currUsed;
			currUsed = currUsed->next;
		}
		else if (curr->name == "Free") {
			//If the name in the free list is "Free", then the program will print out "Free"
			for (int i = 1; i <= curr->size; i++) {
				std::cout << curr->name << "\t";
				counter++;
				//move one line forward
				if (counter >= 8) {
					std::cout << std::endl;
					counter -= 8;
				}
			}
		}
		prev = curr;
		curr = curr->next;
	}
	//move one line forward
	std::cout << '\n';
}

/*******************************************************************************************************
 *                                            calculation                                              *
 * This function uses the free list passed to it as arguments to calculate the fragments in memory.    *
 *******************************************************************************************************/
void calculation(LinkedList* freeList) {
	//"prev" and "curr" are used as pointers for the free list.
	Node* prev = freeList->getHead();
	Node* curr = prev->next;
	int fragments = 0;
	//Calculate the number of nodes marked as "Free" whose size is greater than 0 in the free list, which is equal to the number of fragments.
	while (curr != NULL) {
		if (curr->name == "Free" && curr->size != 0) {
			fragments++;
		}
		prev = curr;
		curr = curr->next;
	}
	//Print out a message which tells the user the number of fragments
	std::cout << '\n' << "There are " << fragments << " fragment(s)." << std::endl << std::endl;
}