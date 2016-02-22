//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 5             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: April 6, 2015      *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:           Load inventory data from a file and print the data as a table. Tables will   *
//*                     include the data sorted as is originally, descending by quantity, descending *
//*                     by selling price, and ascending by inventory number.                         *
//*                                                                                                  *
//*  USER DEFINED                                                                                    *
//*    MODULES     :    Header..........................Prints the output heading                    *
//*                     Footer..........................Prints the output footer                     *
//*                     PageBreak.......................Inserts a page break into the output file    *
//*                     LoadInventory...................Loads inventory data from file into the      *
//*                                                     program                                      *
//*                     SortByInventoryNumber...........Sorts inventory data by inventory number     *
//*                                                     in ascending order                           *
//*                     SortByPriceDescending...........Sorts inventory data by selling price in     *
//*                                                     in desending order                           *
//*                     SortByQuantity..................Sorts inventory data by quantity in          *
//*                                                     descending order                             *
//*                     PrintListHeadings...............Prints data headings to the output file      *
//*                     PrintList.......................Prints inventory data as a table             *
//*                                                                                                  *
//****************************************************************************************************

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PAGEWIDTH 95;       // Set constant page width
#define PAGEHEIGHT 76;      // Set constant page height
#define MAXRECORDS 50;      // Set maximum number of records

static int linesUsed;       // Line counter for output

using namespace std;

// --------------------------------------------------------------------------------------------------
// ITEM STRUCT
struct Item {
    int     id;             // Inventory number
    char    descrip[26];    // Item description
    int     quantity;       // Quantity on hand
    int     reorder;        // Reorder number
    float   cost;           // Cost of item
    float   price;          // Selling price
};

//******************************************* MAIN ***************************************************

    // Main functions
void Header(ofstream &);
void Footer(ofstream &);
void PageBreak(ofstream &outfile, int lines);
void LoadInventory(ifstream &, Item[], int &);
void SortByInventoryNumber(Item[], int, int);
void SortByPriceDescending(Item[], int);
void SortByQuantity(Item[], int);
void PrintListHeadings(ofstream &);
void PrintList(ofstream &, Item[], int);

int main() {
    
    int maxRec = MAXRECORDS;    // Maximum number of recordsfor inventory
    int recCount = 0;           // Number of records in inventory
    Item inventory[maxRec];     // Inventory
    
        // Get input file
    ifstream inputFile("data5.txt", ios::in);
        // Get output file
    ofstream outputFile("output.txt", ios::out);
        // Initialize line count
    linesUsed = 0;

        // Load inventory data from input fle
    LoadInventory(inputFile, inventory, recCount);
    
        // Print output header
    Header(outputFile);
    linesUsed+=4;   // Update line count
    
        // Print the original list
        // Print headings
    outputFile << "The Original Inventory Array: \n\n";
    linesUsed+=2;   // Update line count
    PrintListHeadings(outputFile);
    linesUsed+=3;   // Update line count
        // Print current list
    PrintList(outputFile, inventory, recCount);
    outputFile << endl;
    linesUsed+=recCount+1;   // Update line count
        // Insert page break
    PageBreak(outputFile, linesUsed);
    linesUsed = 0;   // Update line count
    
        // Print list sorted by quantity descending
        // Print headings
    outputFile << "The Inventory Array sorted in descending order \n";
    outputFile << "according to the quantity on hand using the Exchange Sort: \n\n";
    linesUsed+=3;   // Update line count
    PrintListHeadings(outputFile);
    linesUsed+=3;   // Update line count
        // Sort inventory
    SortByQuantity(inventory, recCount);
        // Print inventory
    PrintList(outputFile, inventory, recCount);
    outputFile << endl;
    linesUsed+=recCount+1;   // Update line count
        // Insert page break
    PageBreak(outputFile, linesUsed);
    linesUsed = 0;   // Update line count

        // Print list sorted by selling price descending
        // Print headings
    outputFile << "The Inventory Array sorted in descending order \n";
    outputFile << "according to the selling price using the Shell Sort: \n\n";
    linesUsed+=3;   // Update line count
    PrintListHeadings(outputFile);
    linesUsed+=3;   // Update line count
        // Sort inventory
    SortByPriceDescending(inventory, recCount);
        // Print inventory
    PrintList(outputFile, inventory, recCount);
    outputFile << endl;
    linesUsed+=recCount+1;   // Update line count
        // Insert page break
    PageBreak(outputFile, linesUsed);
    linesUsed = 0;   // Update line count
    
        // Print list sorted by number ascending
        // Print headings
    outputFile << "The Inventory Array sorted in ascending order \n";
    outputFile << "according to the inventory number using the Quick Sort: \n\n";
    linesUsed+=3;   // Update line count
    PrintListHeadings(outputFile);
    linesUsed+=3;   // Update line count
        // Sort inventory
    SortByInventoryNumber(inventory, 0, recCount-1);
        // Print inventory
    PrintList(outputFile, inventory, recCount);
    outputFile << endl;
    linesUsed+=recCount+1;   // Update line count
        // Insert page break
    PageBreak(outputFile, linesUsed);
    linesUsed = 0;   // Update line count
    
        // Print output footer
    Footer(outputFile);
    
        // Return to operating system
    return 0;
}
//****************************************************************************************************

//****************************************************************************************************
void LoadInventory(ifstream &in, Item list[], int &itemCount) {
    
    // Receives - A reference to the input file, an array for storing inventory data,
    //              and a reference to the record counter
    // Task - Loads data from the input file into the storage array
    // Returns - The inventory array is loaded accordingly, and the item count is updated
    
    Item    newItem;        // New item struct
    int     newId;          // Inventory number
    char    newDescrip[26]; // Item description
    int     newQuantity;    // Quantity on hand
    int     newReorder;     // Reorder number
    float   newCost;        // Cost of item
    float   newPrice;       // Selling price
    
        // Get the first item inventory number from data file
    in >> newId;
    
        // Process data until a negative inventory number is found
    while ( newId > 0 ) {
            // Read in item data from file
        in >> ws;
        in.get(newDescrip, 26);
        in >> ws >> ws;
        in >> newQuantity;
        in >> newReorder;
        in >> newCost;
        in >> newPrice;
            // Assign data to a new item
        newItem.id = newId;
        strcpy(newItem.descrip, newDescrip);
        newItem.quantity = newQuantity;
        newItem.reorder = newReorder;
        newItem.cost = newCost;
        newItem.price = newPrice;
            // Add new item to the list
        list[itemCount] = newItem;
            // Update item count
        itemCount++;
            // Get the next inventory number
        in >> newId;
    }
    
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void SortByInventoryNumber(Item list[], int first, int last) {
    
    // Receives - An array of inventory data, the starting index, and the final index of the
    //              data to be sorted
    // Task - Sorts the records by inventory number in ascending order, using Quick Sort
    // Returns - The inventory is sorted acordingly
    
    int FIRST = first;         // First item in the list
    int LAST = last;           // Last item in the list
    Item temp;                 // Temporary item placeholder

        // Initialize index markers
    int F = FIRST;             // Index marking the left incrementing scanner
    int L = LAST;              // Index marking the right decrementing scanner
    int M = ((LAST+FIRST)/2);  // Middle element in the list
    int P = list[M].id;        // Pivot value

        // Process list until the first index passes the last index
    while ( F <= L ) {
            // Find item in first part of list where the value is not less than the pivot point
        while ( list[F].id < P ) {
            F++;
        }
            // Find item in second part of list where the value is not greater than the pivot point
        while ( list[L].id > P ) {
            L--;
        }
            // If the first list index is still smaller than the last list index, swap and continue
        if ( F <= L ) {
                // Swap item at index F with the item at index L
            temp = list[F];
            list[F] = list[L];
            list[L] = temp;
                // Increment F and decrement L
            F++;
            L--;
        }
        // Keep going until first index exceeds the last index
    }
        // If the rightmost index marker has not reached the beginning of the list,
        //   partition left subarray for further sorting
    if ( L > FIRST ) {
        SortByInventoryNumber(list, FIRST, L);
    }
        // If the leftmost index marker has not reached the end of the list,
        //   partition right subarray for further sorting
    if ( F < LAST ) {
        SortByInventoryNumber(list, F, LAST);
    }
    
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void SortByPriceDescending(Item list[], int count) {
    
    // Receives - An array of inventory data, and the number of records on file
    // Task - Sorts the records by selling price in descending order, using the Shell Sort
    // Returns - The inventory is sorted acordingly
    
    int SortStages = 3;         // Number of stages in the sorting process
    int KValues[] = {7,3,1};    // List of k-values to use in the sorting process
    int stage;                  // Current stage in the sorting process
    int k;                      // Current k-value
    int unsorted;               // Index marking the beginning of unsorted elements
    int max;                    // Index of the maximum value found so far
    Item temp;
    
        // Process each stage of the Shell Sort
    for (stage = 0; stage < SortStages; stage++) {
            // Set k value according to current stage
        k = KValues[stage];
            // Initialize max so far
        max = 0;
            // Each group of elements (defined by k-value) is a "set"
            // Process each set
        for (int set = 0; set < k; set++) {
                // Use the Exchange Sort to sort the elements in the current set
                // Process entire set
            for (unsorted = set; unsorted < count; unsorted+=k) {
                    // Reset max value to the starting point of unsorted list
                max = unsorted;
                    // Search unsorted values for the max
                for (int i = unsorted; i < count; i+=k ) {
                        // When the max is found, replace current max
                    if (list[i].price > list[max].price) {
                        max = i;
                    }
                }
                // Swap the largest unsorted item with the head of unsorted values
                temp = list[unsorted];
                list[unsorted] = list[max];
                list[max] = temp;
            } // Continue until all values in set are sorted
        } // Continue until all sets in the stage are sorted
    } // End of stage
    
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void SortByQuantity(Item list[], int count) {
    
    // Receives - An array of inventory data, and the number of records on file
    // Task - Sorts the records by quantity in descending order, using the Exchange Sort
    // Returns - The inventory is sorted acordingly
    
    int unsortedInd;    // Index marking the beginning of unsorted values
    int ind;            // Index scanning through unsorted values
    int max;            // Index of largest value found so far
    Item temp;          // Temporary item placeholder
    
        // Initialize index markers
    unsortedInd = 0;
    max = 0;
    
        // Process list until all items are sorted
    while ( unsortedInd < (count-1) ) {
            // Scan through each unsorted item
        for (ind = (count-1); ind > unsortedInd; ind--) {
                // If current item has value larger than current max
            if ( list[ind].quantity > list[max].quantity ) {
                    // Set this element as the max so far
                max = ind;
            }
        }
            // Swap the largest unsorted item with the head of unsorted values
        temp = list[unsortedInd];
        list[unsortedInd] = list[max];
        list[max] = temp;
            // Increment the size of the sorted half
        unsortedInd++;
            // Reset max
        max = unsortedInd;
    }
    
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void PrintListHeadings(ofstream &outfile) {
    
    // Receives - A reference to the output file
    // Task - Prints the inventory data headings to the output file
    // Returns - The output file is updated accordingly
    
    outfile << left;
    outfile << setw(14) << "Inventory" << setw(25) << "Item" << setw(14) << "Quantity";
    outfile << setw(14) << "Reorder" << setw(14) << "Cost of" << setw(14) << "Selling" << "\n";
    outfile << setw(14) << "Number" << setw(25) << "Description" << setw(14) << "on hand";
    outfile << setw(14) << "Number" << setw(14) << "Item" << setw(14) << "Price" << "\n";
    outfile << setw(14) << "---------" << setw(25) << "----------------------";
    outfile << setw(14) << "---------" << setw(14) << "---------" << setw(14) << "---------";
    outfile << setw(14) << "---------" << "\n";
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void PrintList(ofstream &outfile, Item list[], int count) {
    
    // Receives - A reference to the output file, an array of inventory data, and the number of
    //              records on file
    // Task - Prints the data to a table in the output file
    // Returns - The output file is updated accordingly
    
    outfile << left;
    for (int i = 0; i < count; i++) {
        outfile << setw(14) << list[i].id << setw(25) << list[i].descrip;
        outfile << setw(14) << list[i].quantity << setw(14) << list[i].reorder;
        outfile << setw(14) << list[i].cost << setw(14) << list[i].price << "\n";
    }
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void PageBreak(ofstream &outfile, int lines) {
    
    // Receives - A reference to the output file and the number of lines printed so far
    // Task - Prints a page break
    // Returns - The output file is updated accordingly
    
        // Print a newline for every line on the page not used
    int max = PAGEHEIGHT;
    for (int i = 0; i < (max - lines); i++) {
        outfile << "\n";
    }
    return;
}
//****************************************************************************************************

//****************************************** HEADER **************************************************
void Header(ofstream &Outfile)
{
    // Receives - the output file
    // Task - Prints the output preamble
    // Returns - Nothing
    Outfile << setw(40) << "Katelyn Schaffer";
    Outfile << setw(17) << "CSC 36000";
    Outfile << setw(15) << "Section 11" << endl;
    Outfile << setw(40) << "Spring 2015";
    Outfile << setw(20) << "Assignment #5" <<endl;
    Outfile << setw(35) << "-----------------------------------";
    Outfile <<             "----------------------";
    Outfile << setw(35) << "-----------------------------------\n\n";
    
    return;
}
//****************************************************************************************************

//******************************************* FOOTER *************************************************
void Footer(ofstream &Outfile)
{
    // Receives - the output file
    // Task - Prints the output salutation
    // Returns - Nothing
    Outfile << endl;
    Outfile << setw(35) << " --------------------------------- " << endl;
    Outfile << setw(35) << "|      END OF PROGRAM OUTPUT      |" << endl;
    Outfile << setw(35) << " --------------------------------- " << endl;
    
    return;
}
//****************************************************************************************************