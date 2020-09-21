/**************************************************************/
/*                                                            */
/*   Program to demonstrate the implmentation of a simple     */
/*   linked list of numbers with a delete.					  */
/*                                                            */
/**************************************************************/

#define _CRT_SECURE_NO_WARNINGS 1
#define bool int
#define false 0
#define true (!false)

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Preprocessor Variable
#define SIZE 5 // maximum count that is passed when adding the car.

//Stucture template for data part of the linked list
struct data {
	int numOfOwners;
	float reserveAmount;
	char reg[20], carMake[20], carModel[10], color[10];
	bool reserve;

};

//Stucture template for one node
struct LinearNode {
	struct data* element;
	struct LinearNode* next;
};


// Function prototypes
void menu();
void addNewCar();
void sellACar(char*);
void reserveUnreserve();
void reserve();
void unReserve(char*);
void viewAllCar();
void viewSpecificCar(char*);
void exitSystem();

bool isEmpty();
bool isFull();
void getFromFile(FILE*);
void saveToFile(FILE*);

// Global Variables
struct LinearNode* list = NULL; //front of list

struct LinearNode* last = NULL; //pointer to last node in list

int count; 
/**************MAIN FUNCTION*******************/
int main() {

	FILE* fp;

	if ((fp = fopen("cars.dat", "rb")) == NULL) {
		//file DOES NOT exist so get user to input cars.
		printf("File does not exit\n\n");
		menu();
	}//end if
	else {
		getFromFile(fp);
		menu();
	}

	saveToFile(fp);

	getchar();
	getchar();
}
/*******************************************/

/**************MENU*******************/
void menu() {
	char carReg[20];
	int optionNumber;
	//basic menu system system that separates all method by pressing speficic number keys
	//to run any method of choice.
	do {
		printf("*******JY Car Dealer********\n\n");
		printf("1. Add a new car to the showroom.\n");
		printf("2. Sell a car from the showroom\n");
		printf("3. Reserve/Unreserve a car in the showroom\n");
		printf("4. View all cars in the showroom\n");
		printf("5. View a specific car in the showroom\n");
		printf("6. Exit System\n\n");

		printf("Please Input Option 1-6\n");
		scanf("%d", &optionNumber);

		switch (optionNumber) {
		case 1:
			addNewCar();
			break;
		case 2:
			sellACar(carReg);
			break;
		case 3:
			reserveUnreserve();
			break;
		case 4:
			viewAllCar();
			break;
		case 5:
			viewSpecificCar(carReg);
			break;
		case 6:
			exitSystem();
			break;
		}
	} while (optionNumber != 6);
}
/*******************************************/


/**************ADD A NEW CAR*******************/
void addNewCar() {
	char carReg[20];
	char make[20];
	char model[20];
	char color[10];
	int numOfOwners = 0;
	float deposit = 0;
	bool reserved = false; // setting the reserved = false as default so that everytime we sell a car in sellACar() method gives user an error unless the car is reserved.
	struct LinearNode* aNode;
	struct data* anElement;
	
	//Adding elements in the linked list.
	//isFull() - satisfies if the amount cars has reached its capacity, else continue adding more cars.
	if (isFull()) {
		printf("Reached max amount of cars!\n");
	}
	else {
		printf("\nEnter car register number: ");
		scanf("%s", carReg);

		printf("\nEnter car make: ");
		scanf("%s", make);

		printf("\nEnter car model: ");
		scanf("%s", model);

		printf("\nEnter car colour: ");
		scanf("%s", color);
		// Do-while loop to ensure that there can only be a 0 to 3 owners. 
		do {
			printf("\nEnter number of owners: ");
			scanf("%d", &numOfOwners);
			if (numOfOwners <= 3 && numOfOwners > 0) {
				anElement = (struct data*)malloc(sizeof(struct data));
				anElement->numOfOwners = numOfOwners;
			}
			else if (numOfOwners == 0) {
				printf("\n**This is a new car!**\n");
			}
			else //end else
				printf("\nMaximum owners is only 3!\n");
		} while (numOfOwners > 3 || numOfOwners < 0); //using character(s) other than integers will default the number of owners to 0.

	    //create space for new data element
		anElement = (struct data*)malloc(sizeof(struct data));

		// create space for new node
		aNode = (struct LinearNode*)malloc(sizeof(struct LinearNode));

		//add aCar to data node
		strcpy(anElement->reg, carReg);
		strcpy(anElement->carMake, make);
		strcpy(anElement->carModel, model);
		strcpy(anElement->color, color);
		anElement->numOfOwners = numOfOwners;
		anElement->reserve = reserved;
		anElement->reserveAmount = deposit;

		if (aNode == NULL)
			printf("Error - no space for the new node\n");
		else { // add data part to the node
			aNode->next = NULL;
			aNode->element = anElement;

			//add node to end of the list
			if (isEmpty())
			{
				list = aNode;
				last = aNode;
			}
			else {
				last->next = aNode;
				last = aNode;
			} //end else
		}//end else
		printf("\n**Successfully added to the showroom!**\n");
		count++;// keep track of counts after adding cars.
	}//end else
	getchar();
	getchar();
}
/*******************************************/

/**************SELL A CAR*******************/
void sellACar(char* carReg) {
	struct LinearNode* current, * previous;
	bool notFound = true;

	if (isEmpty())
		printf("\nError - there are no cars in the list\n");
	else {
		printf("\nPlease enter the reg number of the car to sell: ");
		scanf("%s", carReg);

		current = previous = list;

		while (notFound && current != NULL) {

			if (strcmp(current->element->reg, carReg) == 0)// using strcmp to compare the car reg that has been inputted in the list and initialize value to 0 for the boolean to work
				notFound = false;						   // and find the car reg whether if its in the list or not.
			else {
				previous = current; //searching through the list finding and comparing, if not found the first time.
				current = current->next;
			}//end else
		} //end while

		if (notFound)
			printf("\nError - there is not such car with the reg number: %s\n", carReg);
		else {
			if (current->element->reserve == false) //search through the struct data where reserve is default to false in addNewCar() method. if its true then it will be sold.
				printf("\nCar must be reserved before selling");
			else if (current == list) {
				list = list->next; //searches through the list if found sell.
				free(current);
				printf("\nCar with reg number %s is sold!\n", carReg);
			} //end else
			else {
				previous->next = current->next; //if not found in the first node move to next and if or sell.
				free(current);
				printf("\nCar with reg number %s is sold!\n", carReg);
			} //end else
		}//end else
	}//end else
	getchar();
	getchar();
}
/*******************************************/


/**************RESERVE OR UNRESERVE CAR*******************/
void reserveUnreserve() {
	int option;
	char reg[20];
	do {
		printf("1. Reserve a car\n");
		printf("2. Unreserve a car\n\n");
		scanf("%d", &option);

		switch (option) {
		case 1:
			reserve();
			break;

		case 2:
			unReserve(reg);
			break;
		}
	} while (option != 1 && option != 2);
}
/*******************************************/
void reserve() {
	char carReg[20];
	float amount;
	struct LinearNode* current, * previous;
	bool reserved = false;
	
	if (isEmpty())
		printf("\nError - there are no cars in the list\n");
	else {
		printf("*******Reservation Menu*********\n");
		printf("\nEnter reg number you want to reserve:");
		scanf("%s", carReg);

		current = list;
		while (current != NULL) {

			if (strcmp(current->element->reg, carReg) == 0) {
				//when an amount has been entered, it will update the reserveAmount. and set the reservation to true.
				printf("\nEnter deposit amount 500 - 1500 for the car to be reserved:\n");
				scanf("%f", &amount);
				if (amount >= 500 && amount <= 1500) {
					current->element->reserveAmount = amount;
					current->element->reserve = true;
					printf("Car has been reserved with the deposit amount: %5.2f\n", amount);
					break; //reason i put break is because it wouldnt let me continue after inputting the amount of deposit.
				}
				else {
					printf("\nInput the right amount\n"); //error - if the right amount is not met.
				}//end else
			}
			else {
				previous = current;
				current = current->next;
			}//end else
		}//end while
		printf("\nCar not found!");
	}//end else
	getchar();
	getchar();
}

void unReserve(char* carReg) {
	float deposit;
	struct LinearNode* current, * previous;
	bool reserved = true;

	//the same concept as reserving but in this case car found in the list will be set to false, making it unreserved and updating the list.
	if (isEmpty())
		printf("\nError - there are no cars in the list\n\n");
	else {
		printf("*******Unreserve A Car*********\n");
		printf("\nEnter reg number you want to unreserve:");
		scanf("%s", carReg);
		current = list;
		while (current != NULL) {
			if (strcmp(current->element->reg, carReg) == 0) {
				deposit = 0;//change the reserve amount/deposit back to default, since it has been unreserved.
				current->element->reserveAmount = deposit;
				current->element->reserve = false;
				printf("\nCar has been unreserved!\n");
				break;
			}
			else {
				previous = current;
				current = current->next;
			}//end else
		}//end while
		printf("\nCar not found!");
	}//end else
	getchar();
	getchar();
}

/**************DISPLAY ALL CAR IN SHOWROOM*******************/
void viewAllCar() {
	struct LinearNode* current;

	if (isEmpty())
		printf("\nError - there are no cars in the list\n\n");
	else {
		current = list;
		while (current != NULL) {
			printf("\n/**************ALL CARS IN SHOWROOM*******************/\n");
			printf("\nCar register number: %s\n", current->element->reg);
			printf("Car make: %s\n", current->element->carMake);
			printf("Car model: %s\n", current->element->carModel);
			printf("Car color: %s\n", current->element->color);
			printf("Number of owners: %d\n", current->element->numOfOwners);
			printf("Deposit Amount: %5.2f\n", current->element->reserveAmount);
			printf("Reserved: %s\n", current->element->reserve ? "Yes" : "No");
			current = current->next;
		} //end while
	}//end else
	getchar();
	getchar();
}
/*******************************************/

/**************DISPLAY A SPECIFIC CAR*******************/
void viewSpecificCar(char* carReg) {
	struct LinearNode* current, * previous;

	if (isEmpty())
		printf("\nError - there are no cars in the list\n");
	else {
		printf("Please input the register number to view all specified vehicle details\n");
		scanf("%s", carReg);
		current = list;
		while (current != NULL) {
			// strmcp searches through the list and if found will print out the details.
			if (strcmp(current->element->reg, carReg) == 0) {
				printf("\n******Car Details******\n");
				printf("Car register number: %s\n", current->element->reg);
				printf("Car make: %s\n", current->element->carMake);
				printf("Car model: %s\n", current->element->carModel);
				printf("Car color: %s\n", current->element->color);
				printf("Number of owners: %d\n", current->element->numOfOwners);
				printf("Deposit Amount: %5.2f\n", current->element->reserveAmount);
				printf("Reserved: %s\n", current->element->reserve ? "Yes" : "No");
				break; //break ends the infinite loop, because if the car was found then current wouldn't be updated and would be stuck.
			}
			else {
				previous = current;
				current = current->next;
			}
			printf("Car not found!\n");
		}
	}//end while
	getchar();
	getchar();
}
/*******************************************/
//boolean method to keep users from not entering >5 number of cars in the list.
bool isFull() {
	if (count == SIZE)
		return true;
	else
		return false;
 }


bool isEmpty() {
	if (list == NULL)
		return true;
	else
		return false;
}

/************SAVING FILES**************/
void saveToFile(FILE* fp) {
	struct LinearNode* current;
	
	fp = fopen("cars.dat", "wb");
	
	current = list;
	while (current != NULL) {
		fwrite(current->element, sizeof(struct data), 1, fp);
		current = current->next;
	}
	fclose(fp);
}

/************READING FILES**************/
void getFromFile(FILE* fp) {
	struct data* anElement;
	struct LinearNode* aNode;

	printf("Retrieving cars from the file...\n\n");
	//malloc to create anElement.
	anElement = (struct data*)malloc(sizeof(struct data));

	while (fread(anElement, sizeof(struct data), 1, fp) != NULL) {
		//call malloc to linear node.
		aNode = (struct LinearNode*)malloc(sizeof(struct LinearNode));
		
		//Connect node to the anElement.
		aNode->next = NULL;
		aNode->element = anElement;

		//rest of the add new element to linked list.
		//add node to end of the list
		if (isEmpty())
		{
			list = aNode;
			last = aNode;
		}
		else {
			last->next = aNode;
			last = aNode;
		} //end else
		//call malloc again to create anElement.
		anElement = (struct data*)malloc(sizeof(struct data));
	}//end else
	fclose(fp);
}


/**************EXIT SYSTEM*******************/
void exitSystem() {
	printf("\nSaving Files...\n\nGoodbye!");
}