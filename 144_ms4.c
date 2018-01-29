// -------------------------------------------
//  Name: Gilberto Wagner Miranda da Silva
//  Description: Final Assembly
// -------------------------------------------

/* -The Grocery Store Inventory System is able to print a detailed list of all items in the inventory, 
 search and display an item by its SKU number, checkout an item to be delivered to the shelf for sale, 
 add to stock items that are recently purchased for inventory, and add a new item to the inventory or update
 an already existing item. */


#include <stdio.h>
#define LINEAR 1
#define FORM 0
#define STOCK 1
#define CHECKOUT 0
#define MAX_ITEM_NO 500
#define MAX_QTY 999
#define SKU_MAX 999
#define SKU_MIN 100
#define DATAFILE "items.txt"

const double TAX = 0.13;

/*  MS1  */
// user interface tools function PROTOTYPES:
void welcome(void);
void printTitle(void);
void printFooter(double gTotal);
void flushKeyboard(void);
void pause(void);
int getInt(void);
int getIntLimited(int lowerLimit, int upperLimit);
double getDouble(void);
double getDoubleLimited(double lowerLimit, double upperLimit);

// application user interface function PROTOTYPES:
int yes(void);
int menu(void);
void GroceryInventorySystem(void);





struct Item {
	double price;
	int sku;
	int isTaxed;
	int quantity;
	int minQuantity;
	char name[21];
};



/*  MS2  */
//Function Prototypes MS2
double totalAfterTax(struct Item item);
int isLowQuantity(struct Item item);
struct Item itemEntry(int sku);
void displayItem(struct Item item, int linear);
void listItems(const struct Item item[], int NoOfItems);
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index);





/*  MS3  */
//Function Prototypes MS3
void search(const struct Item item[], int NoOfRecs);
void updateItem(struct Item* itemptr);
void addItem(struct Item item[], int *NoOfRecs, int sku);
void addOrUpdateItem(struct Item item[], int* NoOfRecs);
void adjustQuantity(struct Item item[], int NoOfRecs, int stock);





/*  MS4  */
void saveItem(struct Item item, FILE* dataFile);
int loadItem(struct Item* item, FILE* dataFile);
int saveItems(const struct Item item[], char fileName[], int NoOfRecs);
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr);

/*  Final Assembly  */



// ----------------------------------------
// FUNCTION DEFINITIONS
// ----------------------------------------

//Print header - Program name
void welcome(void)
{
	printf("---=== Grocery Inventory System ===---\n");
	printf("\n");
	return;
}



//Print header - Title
void printTitle(void)
{
	printf("Row |SKU| Name               | Price  |Taxed| Qty | Min |   Total    |Atn\n");
	printf("----+---+--------------------+--------+-----+-----+-----+------------|---\n");
	return;
}


//Print footer Without and with "Grand Total"
void printFooter(double gTotal)
{
	printf("--------------------------------------------------------+----------------\n");

	if (gTotal > 0)
	{
		printf("                                           Grand Total: |%12.2lf\n", gTotal);
	}
	return;
}



//flushkeyboard - clear keyboard until the user hit <ENTER>
void flushKeyboard(void)
{
	while (getchar() != '\n')
		;
}


//Pauses the execution of the application
void pause(void)
{

	printf("Press <ENTER> to continue...");
	while (getchar() != '\n')
		;
}


//Gets a valid interger from the keyboard and returns it
int getInt(void)
{
	int getInterger;
	char getCharacter = 'x';

	while (getCharacter != '\n')
	{
		scanf("%d%c", &getInterger, &getCharacter);
		if (getCharacter != '\n')
		{
			flushKeyboard();
			printf("Invalid integer, please try again: ");
		}
	}
	return getInterger;
}


//This function makes sure the interger is whithin a determined limit
int getIntLimited(int lowerLimit, int upperLimit)
{
	int tempInterger;

	tempInterger = getInt();

	while (tempInterger < lowerLimit || tempInterger > upperLimit)
	{
		printf("Invalid value, %d < value < %d: ", lowerLimit, upperLimit);
		tempInterger = getInt();
	}

	return tempInterger;
}


//Gets a valid doulbe from the keyboard and returns it
double getDouble(void)
{
	double getDouble;
	char getCharacter = 'x';

	while (getCharacter != '\n')
	{
		scanf("%lf%c", &getDouble, &getCharacter);
		if (getCharacter != '\n')
		{
			flushKeyboard();
			printf("Invalid number, please try again: ");
		}
	}
	return getDouble;
}


//This function makes sure the double entered is whithin a determined limit
double getDoubleLimited(double lowerLimit, double upperLimit)
{
	double tempDouble;

	tempDouble = getDouble();

	while (tempDouble < lowerLimit || tempDouble > upperLimit)
	{
		printf("Invalid value, %lf< value < %lf: ", lowerLimit, upperLimit);
		tempDouble = getDouble();
	}

	return tempDouble;

	return tempDouble;
}


//Receives a single character from the user: (Y) or (N)
int yes(void)
{
	int exitInput = 2;
	char enterChar;

	do
	{

		scanf("%c", &enterChar);
		flushKeyboard();

		if (enterChar == 'y' || enterChar == 'Y')
		{
			exitInput = 1;
		}
		else if (enterChar == 'n' || enterChar == 'N')
		{
			exitInput = 0;
		}
		else
		{
			printf("Only (Y)es or (N)o are acceptable: ");
			exitInput = 2;
		}
	} while (exitInput == 2);

	return exitInput;
}


//Menu prints the following options
int menu(void)
{
	int option;

	printf("1- List all items\n");
	printf("2- Search by SKU\n");
	printf("3- Checkout an item\n");
	printf("4- Stock an item\n");
	printf("5- Add new item or update item\n");
	printf("6- delete item\n");
	printf("7- Search by name\n");
	printf("0- Exit program\n");
	printf("> ");
	option = getIntLimited(0, 7);

	return option;
}


//Receives an Item and returns the total inventory price of the product
double totalAfterTax(struct Item item)
{
	double totalInvPrice;
	double total;

	if (item.isTaxed == 1)
	{
		total = (item.price * item.quantity);
		totalInvPrice = (total * TAX) + total;
	}
	else
	{
		totalInvPrice = (item.price * item.quantity);
	}
	return totalInvPrice;
}


//Receives an Item and returns true (1) if the Item quantity is less than Item 
//minimum quantity and false (0) otherwise
int isLowQuantity(struct Item item)
{
	int flag = 0;

	if (item.quantity <= item.minQuantity)
	{
		flag = 1;
	}
	return flag;
}


//This function receives an integer argument for sku and creates an Item and 
//sets its sku to the sku argument value.
struct Item itemEntry(int sku)
{
	struct Item itemEntry;

	printf("        SKU: %d\n", sku);
	itemEntry.sku = sku;
	printf("       Name: ");
	scanf("%20[^\n]", itemEntry.name);
	flushKeyboard();
	printf("      Price: ");
	itemEntry.price = getDoubleLimited(0.01, 1000.00);
	printf("   Quantity: ");
	itemEntry.quantity = getIntLimited(1, MAX_QTY);
	printf("Minimum Qty: ");
	itemEntry.minQuantity = getIntLimited(0, 100);
	printf("   Is Taxed: ");
	itemEntry.isTaxed = yes();

	return itemEntry;
}


//This function receives two arguments:  an Item and an integer flag  
//called linear and prints an Item on screen in two different formats 
//depending on the value of linear flag being true or false.
void displayItem(struct Item item, int linear)
{
	double totalPlusTaxes;
	int flag;


	totalPlusTaxes = totalAfterTax(item);

	if (linear == 1)
	{
		printf("|%d|", item.sku);
		printf("%-20s|", item.name);
		printf("%8.2lf|", item.price);
		if (item.isTaxed == 1)
		{
			printf("  Yes|");
		}
		else
		{
			printf("   No|");
		}
		printf("%4d |", item.quantity);
		printf("%4d |", item.minQuantity);
		if (item.quantity > item.minQuantity)
		{
			printf("%12.2lf|\n", totalPlusTaxes);
		}
		else
		{
			printf("%12.2lf|***\n", totalPlusTaxes);
		}
	}
	else
	{
		printf("        SKU: %d\n", item.sku);
		printf("       Name: %s\n", item.name);
		printf("      Price: %.2lf\n", item.price);
		printf("   Quantity: %d\n", item.quantity);
		printf("Minimum Qty: %d\n", item.minQuantity);
		if (item.isTaxed == 1)
		{
			printf("   Is Taxed: Yes\n");
		}
		else
		{
			printf("   Is Taxed: No\n");
		}
		flag = isLowQuantity(item);
		if (flag == 1)
		{
			printf("WARNING: Quantity low, please order ASAP!!!\n");
		}
	}
}



//This function receives a constant array of Items and their number and 
//prints the items in list with the grand total price at the end. 
void listItems(const struct Item item[], int noOfItems)
{
	int i;
	double gTotalProd = 0;
	int rowCounter = 1;
	double totalPlusTax;

	printTitle();

	for (i = 0; i < noOfItems; i++)
	{
		printf("%-4d", rowCounter++);
		displayItem(item[i], 1);
		totalPlusTax = totalAfterTax(item[i]);
		gTotalProd += totalPlusTax;
	}
	printFooter(gTotalProd);
}



//Receives a constant array of items and their number and also a SKU to look
//for in the item array.  The last argument is a pointer to an index.
//The target of this index pointer will be set to the index of the Item-element
//in which the sku is found, otherwise no action will be taken on index pointer. 
int locateItem(const struct Item item[], int NoOfRecs, int sku, int* index)
{
	int i;
	int flag = 0;

	for (i = 0; i < NoOfRecs; i++)
	{
		if (sku == item[i].sku)
		{
			*index = i;
			flag = 1;
		}
	}
	return flag;
}



// receives an array of items and its size and searches through the array for an Item
//with a specific sku that is received from the user. If found, 
//it will display the item in FORM format, otherwise it will print an error message.
void search(const struct Item item[], int NoOfRecs)
{
	int i;
	int getSKU;
	int flag;

	printf("Please enter the SKU: ");
	getSKU = getIntLimited(SKU_MIN, SKU_MAX);

	flag = locateItem(item, NoOfRecs, getSKU, &i);

	if (flag == 1)
	{
		displayItem(item[i], FORM);
	}
	else
	{
		printf("Item not found!\n");
	}
}



// The function receives the address of the Item to update 
// and modifies the fields of an item.
void updateItem(struct Item* itemptr)
{
	struct Item tempItem;
	int yesORno;

	printf("Enter new data:\n");
	tempItem = itemEntry(itemptr->sku);

	printf("Overwrite old data? (Y)es/(N)o: ");
	yesORno = yes();

	if (yesORno == 1)
	{
		printf("--== Updated! ==--\n");
		*itemptr = tempItem;
	}
	else
	{
		printf("--== Aborted! ==--\n");
	}
}



//If the item array is not full, this function will ask the user to enter the data
//for an Item (with the SKU received through the argument list) 
//and if the user confirms, it will add it to the array
//and add one to the target of NoOfRecs pointer.
void addItem(struct Item item[], int *NoOfRecs, int sku)
{
	struct Item tempItem;
	int yesORno;

	if (*NoOfRecs == MAX_ITEM_NO)
	{
		printf("Can not add new item; Storage Full!\n");
	}
	else
	{
		tempItem = itemEntry(sku);
		printf("Add Item? (Y)es/(N)o: ");
		yesORno = yes();
		if (yesORno == 1)
		{
			item[*NoOfRecs] = tempItem;
			*NoOfRecs += 1;
			printf("--== Added! ==--\n");
		}
		else
		{
			printf("--== Aborted! ==--\n");
		}
	}
}



//Receives an SKU from the user and updates or adds an Item in an array of Items 
//depending on the SKU being present in an item in the array or not.
void addOrUpdateItem(struct Item item[], int* NoOfRecs)
{
	int getSKU;
	int i;
	int flag;
	int yesORno;

	printf("Please enter the SKU: ");
	getSKU = getIntLimited(SKU_MIN, SKU_MAX);

	flag = locateItem(item, *NoOfRecs, getSKU, &i);

	if (flag == 1)
	{
		displayItem(item[i], FORM);
		printf("Item already exists, Update? (Y)es/(N)o: ");
		yesORno = yes();
		if (yesORno == 1)
		{
			updateItem(&item[i]);
		}
		else
		{
			printf("--== Aborted! ==--\n");
		}
	}
	else
	{
		addItem(item, &*NoOfRecs, getSKU);
	}
}



//Depending on the value of the stock argument being 
//STOCK or CHECKOUT, this function will increase or reduce the quantity 
//of the selected Item in the array by the value received from the user.
//If stocking, (adding to storage) this value can vary 
//between 0 to(MAX_QTY  item_quantity) and 
//if checking out(removing from storage) this value can vary 
//between 0 to item_quantity.
void adjustQuantity(struct Item item[], int NoOfRecs, int stock)
{
	int getSKU;
	int i;
	int flag;
	char stringStock[9] = "to stock";
	char checkout[12] = "to checkout";
	int temp;
	int adjustStock;

	printf("Please enter the SKU: ");
	getSKU = getIntLimited(SKU_MIN, SKU_MAX);

	flag = locateItem(item, NoOfRecs, getSKU, &i);

	if (flag == 0)
	{
		printf("SKU not found in storage!\n");

	}
	else
	{

		displayItem(item[i], FORM);
		if (stock == STOCK)
		{
			temp = MAX_QTY - (item[i].quantity);
			printf("Please enter the quantity %s; Maximum of %d or 0 to abort: ", stringStock, temp);
			adjustStock = getIntLimited(0, temp);
			if (adjustStock == 0)
			{
				printf("--== Aborted! ==--\n");
			}
			else
			{
				item[i].quantity += adjustStock;
				printf("--== Stocked! ==--\n");
			}
		}
		else
		{
			printf("Please enter the quantity %s; Maximum of %d or 0 to abort: ", checkout, item[i].quantity);
			adjustStock = getIntLimited(0, item[i].quantity);
			if (adjustStock == 0)
			{
				printf("--== Aborted! ==--\n");
			}
			else
			{
				item[i].quantity -= adjustStock;
				printf("--== Checked out! ==--\n");
			}
		}
		flag = isLowQuantity(*item);
		if (item[i].quantity <= item[i].minQuantity)
		{
			printf("Quantity is low, please reorder ASAP!!!\n");
		}
	}

}



//This function writes content of an Item, comma separated, in one line of a text file
//pointed by "dataFile" argument.
void saveItem(struct Item item, FILE* dataFile)
{
	
	fprintf(dataFile, "%d,%d,%d,%.2lf,%d,%s\n", item.sku, item.quantity, item.minQuantity, item.price, item.isTaxed, item.name);	

}



// This function reads all the fields of an Item from one line of a comma separated
//text file using fscanf  and stores them in the Item structure that is pointed 
// by the item pointer in the argument list. 
int loadItem(struct Item* item, FILE* dataFile)
{
	int rv;
	int returnValue = 0;

	rv = fscanf(dataFile, "%d,%d,%d,%lf,%d,%[^\n]\n", &item->sku, &item->quantity, &item->minQuantity, &item->price, &item->isTaxed, item->name);
	if (rv == 6)
	{
		returnValue = 1;
	}
	return returnValue;
}



// saveItems uses the saveItem function to write an entire array of Items into a file.
int saveItems(const struct Item item[], char fileName[], int NoOfRecs)
{
	FILE *fp = NULL;
	int rv = 0;
	int i;

	fp = fopen(fileName, "w");
	if (fp != NULL)
	{
		for (i = 0; i < NoOfRecs; i++)
		{
			saveItem(item[i], fp);
		}
		fclose(fp);
		rv = 1;
	}
	else
	{
		printf("Failed to open file!\n");
	}
	return rv;
}



//loadItems uses the loadItem function to read all the records saved in a file 
//into the item array and sets the target of the NoOfRecsPtr to the number 
//of Items read from the file. 
int loadItems(struct Item item[], char fileName[], int* NoOfRecsPtr)
{
	FILE *fp = NULL;
	int rv = 0;

	*NoOfRecsPtr = 0;

	fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		while (loadItem(&item[*NoOfRecsPtr], fp) == 1)
		{
			*NoOfRecsPtr += 1;
		}
		fclose(fp);
		rv = 1;
	}
	else
	{
		printf("Failed to open file!\n");
	}

	return rv;
}



//This function avoids redundancy in case 3, 4 and 5 of the GroceryInventorySystem function
void saveItemsRedundancy(struct Item item[], char fileName[], int NoOfRecs)
{
	int returnSaveItems = 0;
	
	returnSaveItems = saveItems(item, fileName, NoOfRecs);
	if (returnSaveItems == 0)
	{
		printf("could not update data file %s\n", DATAFILE);
	}
}


//Heart of the application, run the whole program
void GroceryInventorySystem(void)
{
	int option;
	int flag = 0;
	struct Item item[MAX_ITEM_NO];
	int noOfRecs = 0;
	int returnLoadItems;

	welcome();
	returnLoadItems = loadItems(item, DATAFILE, &noOfRecs);
	if (returnLoadItems == 0)
	{
		printf("Could not read from %s.\n", DATAFILE);
	}
	else
	{
		do
		{
			option = menu();
		switch (option)
			{
			case 1:
				listItems(item, noOfRecs);
				pause();
				break;

			case 2:
				search(item, noOfRecs);
				pause();
				break;

			case 3:
				adjustQuantity(item, noOfRecs, CHECKOUT);
				saveItemsRedundancy(item, DATAFILE, noOfRecs);
				pause();
				break;

			case 4:
				adjustQuantity(item, noOfRecs, STOCK);
				saveItemsRedundancy(item, DATAFILE, noOfRecs);
				pause();
				break;

			case 5:
				addOrUpdateItem(item, &noOfRecs);
				saveItemsRedundancy(item, DATAFILE, noOfRecs);
				pause();
				break;

			case 6:
				printf("Not implemented!\n");
				pause();
				break;

			case 7:
				printf("Not implemented!\n");
				pause();
				break;

			case 0:
				printf("Exit the program? (Y)es/(N)o: ");
				flag = yes();
				break;

			}
		} while (flag == 0);
	}
}


//main function
int main(void)
{
	GroceryInventorySystem();

	return 0;
}
