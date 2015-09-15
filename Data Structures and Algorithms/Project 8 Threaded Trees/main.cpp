//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 8             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: May 4, 2015        *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:       This program reads in an input file of commands, processing each command as      *
//*                 necessary. Tasks include Inserting item data, Deleting items from database,      *
//*                 Updating data according to transactions, and Printing data in database.          *
//*                 Task results and printed data are printed to the output file.                    *
//*                                                                                                  *
//*  USER DEFINED                                                                                    *
//*    MODULES     :        Header......................Prints the output header                     *
//*                         Footer......................Prints the output footer                     *
//*                         PageBreak...................Prints a page break                          *
//*                         PrintHeadings...............Prints the item chart headings               *
//*                         InsertDivider...............Prints a divider to output file              *
//*                         [Tree functions]............[See Tree class]                             *
//*                                                                                                  *
//****************************************************************************************************

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PAGEWIDTH 95;       // Set constant page width
#define PAGEHEIGHT 76;      // Set constant page height

static int linesUsed;       // Line counter for output

using namespace std;

// --------------------------------------------------------------------------------------------------
// Item strut
struct Item {
    char id[6];         // Item ID code
    char name[21];      // Item description
    int onHand;         // Quantity on hand
    int onOrder;        // Quanitiy on order
    
    int thread;         // Indicates whether or not the right pointer is a thread
    Item *leftPtr;      // Left pointer
    Item *rightPtr;     // Right pointer
};
// --------------------------------------------------------------------------------------------------

//******************************************* TREE ***************************************************
class Tree {
public:
    Tree();                                         // Constructor
    int  GetCount() { return count; }               // Returns the number of nodes in tree
    Item *GetRoot() { return root; }                // Returns root node
    Item *GetItemById(char[]);                      // Returns specific item according to ID
    Item *Search(char[], Item*);                    // Searches by ID for a specific item
    bool ItemExists(char[]);                        // Returns whether or not a specific item exists
    void Insert(Item);                              // Inserts an item node into the tree
    void Delete(char[]);                            // Removes item from tree, returning the address
    void PrintItem(ofstream &, char[]);             // Prints item data to file
    void PrintAll(ofstream &, Item*);               // Prints all nodes in tree from specified root
private:
    int  count;                                     // Number of nodes in tree
    Item *root;                                     // Root node of the tree
};
// --------------------------------------------------------------------------------------------------
// Tree constructor
Tree::Tree() {
    root = NULL;        // Initialize root to NULL
    count = 0;          // Initialize node count to 0
}
// --------------------------------------------------------------------------------------------------
// GetItemById
Item* Tree::GetItemById(char query[]) {
    
    // Receives - An item ID
    // Task - Gets the item specified by ID
    // Returns - A pointer to the item specified by ID
    
    Item *requestedItem;    // Define item pointer
    requestedItem = NULL;   // Initialize item pointer to NULL
    
        // Find the specified item, starting search at root
    requestedItem = Search(query, root);
    
        // If the item is found, return pointer to item
    if (requestedItem != NULL)
        return requestedItem;
        // Otherwise, print error message to console, and return NULL
    else  {
        cout << "Error: Requested item not found." << endl;
        return requestedItem;
    }
}
// --------------------------------------------------------------------------------------------------
// Search
Item* Tree::Search(char query[], Item *Root) {
    
    // Receives - The ID of item queried, and a pointer to the root of tree/subtree to search
    // Task - Searches tree InOrder for the item with the queried ID
    // Returns - The status indicator is updated and a pointer to the found item is returned
    
    
    Item *queriedItem;      // Define pointer to queried item
    queriedItem = NULL;     // Initialize quieried item pointer to NULL
    
    int RightThread;        // Thread indicator
    Item *CurrPtr;          // Current pointer traversing the tree
    
        // Initialize current pointer at the root
    CurrPtr = Root;
    
        // If the tree is empty, return NULL
    if (CurrPtr == NULL) {
        return queriedItem;
    }
        // Move to the leftmost node in the tree
    while ( CurrPtr->leftPtr != NULL) {
        CurrPtr  = CurrPtr->leftPtr;
    }
        // Process all nodes in order
    while (CurrPtr != NULL)
    {
            // If this item id matches that of the query, the queried item is found
        if ( strcmp(CurrPtr->id, query) == 0 ) {
            queriedItem = CurrPtr;
            return queriedItem;     // Return the item
        }
        
            // Get thread status and move to the right pointer
        RightThread = CurrPtr->thread;
        CurrPtr = CurrPtr->rightPtr;
            // If there is no right thread, move to any remaining left children
        if ( ( CurrPtr != NULL )  && ( RightThread == 0) ) {
                // Process left children until finished
            while ( CurrPtr->leftPtr != NULL) {
                CurrPtr = CurrPtr->leftPtr;
            }
        }
    }   // Finished processing all nodes in tree
    
    return queriedItem;
};
// --------------------------------------------------------------------------------------------------
// ItemExists
bool Tree::ItemExists(char query[]) {
    
    // Receives - An item ID
    // Task - Determine whether or not the item specified by ID exists in the database
    // Returns - True if the item exists; False otherwise
    
    Item *searched;     // Define item pointer
    searched = NULL;    // Initialize item pointer to NULL
        // Search for specified item, starting at tree root
    searched = Search(query, root);
        // Return true if the item is found; return false otherwise
    if (searched != NULL)  return true;
    else                   return false;
}

// --------------------------------------------------------------------------------------------------
// Insert
void Tree::Insert(Item newData) {
    
    // Receives - An item node containing data to insert as a new item node
    // Task - Inserts new item as a new node to the tree
    // Returns - The tree is updated accordingly
    
    bool inserted = false;              // Initialize inserted flag (node not inserted yet)
    Item *newItem, *current;            // Pointers to new item node and current item node
    newItem = new Item;                 // New item node placeholder
    
        // Test for sufficient memory
    if (newItem != NULL) {
            // Load new item data into a new item node
        strcpy(newItem->id, newData.id);
        strcpy(newItem->name, newData.name);
        newItem->onHand = newData.onHand;
        newItem->onOrder = newData.onOrder;
            // Initialize new item pointers to NULL and no thread
        newItem->leftPtr = NULL;
        newItem->rightPtr = NULL;
        newItem->thread = 0;
        
            // Point to the root of the tree
        current = root;
            // Search for location to insert new item node (until the node is inserted)
        while (inserted == false) {
                // If there is no tree yet, place the new node at the root
            if (current == NULL) {
                root = newItem;
                inserted = true;    // The node is inserted
            }
                // Otherwise, keep searching tree for a location to insert new node
            else {
                    // If current node ID is larger than or equal to that of the new item
                if (strcmp(current->id, newItem->id) >= 0) {
                        // If current node has a left child, SEARCH LEFT SUBTREE
                    if (current->leftPtr != NULL) {
                        current = current->leftPtr; // (Now currently pointing to left child)
                    }
                        // Otherwise, insert new node into left child location
                    else {
                        newItem->rightPtr = current;  // Add thread to new item
                        newItem->thread = 1;          // Indicate the existence of thread
                        current->leftPtr = newItem;   // Insert new item into tree
                        inserted = true;              // The item is now inserted
                    }
                }
                    // If current node ID value is smaller than that of the new item
                else {
                        // If the current node has a right child, SEARCH RIGHT SUBTREE
                    if ( (current->rightPtr != NULL) && (current->thread != 1) ) {
                        current = current->rightPtr; // (Now currently pointing to right child)
                    }
                        // Otherwise, insert new node into right child location
                    else {
                        newItem->rightPtr = current->rightPtr; // Add thread to new item
                        newItem->thread = 1;                   // Indicate thread on new item
                        current->thread = 0;                   // Remove thread from parent
                        current->rightPtr = newItem;           // Insert new item into tree
                        inserted = true;                       // The item is now inserted
                    }
                }
            } // Finished processing currently-searched node
        } // Finished search for insert location
        
        count++; // Increment tree node count
        
    } // Finished insert process
    
        // In case of memory error, print error message to console
    else cout << "Error: Out of memory!" << endl;
    
    return;
};
// --------------------------------------------------------------------------------------------------
// Delete
void Tree::Delete(char id[]) {
    
    // Receives - The ID of a specific item in the tree
    // Task - Remove speified item from the tree
    // Returns - The tree is updated accordingly
  
    bool found = false; // Indicates whether or not deletion node is found
        // Define pointers to deletion node, parent, and search pointers
    Item *delnode, *parnode, *node1, *node2, *node3, *sPtr;
    
    sPtr = root;        // Initialize start pointer
    delnode = sPtr;     // Initialize deletion node pointer at the start
    parnode = NULL;     // Initialize parent node pointer to NULL
    
        // Find the node to be deleted and its parent
    while ( (found == false) && (delnode != NULL) && (delnode->thread != 1) ) {
            // If the node to be deleted is found, indicate as such
        if ( strcmp(id, delnode->id) == 0 )
            found = true;
            // Otherwise continue traversing the tree towards the delnode location
        else
        {
            parnode = delnode;                      // Keep track of the parent node
            if (strcmp(id, delnode->id) < 0 ) {
                delnode = delnode->leftPtr;         // Move left
            }
            else {
                delnode = delnode->rightPtr;        // Move right
            }
        }
    }   // Finished searching for node to delete and its parent
    
        // Now readjust the tree for node deletion
    
    // -------------------------------------------------- CASE 1 : Node is not in tree
        // If the node has not been found, print error to console and return
    if ( ( strcmp(id, delnode->id) != 0 ) && (found == false) ) {
        cout << "Error: Node is not in the tree. Delete failed." << endl;
        return;
    }
    // -------------------------------------------------- CASE 2 : Node has no children
        // If the node has no children, just remove it
    if  ( ( delnode->leftPtr == NULL ) &&
        ( ( (delnode->rightPtr == NULL) || (delnode->thread == 1) ) ) ) {
        if (parnode == NULL) {
            sPtr = NULL; // In case of error, reset start pointer
        }
        else {
                // If the node to delete is a left child, remove it
            if (parnode->leftPtr == delnode)
                parnode->leftPtr = NULL;
                // If the node to delete is a right child, link parent to its right thread
            else {
                parnode->rightPtr = delnode->rightPtr;
                parnode->thread = 1;    // Indicate that the parent has a thread now
            }
        }
            // Deletion complete
        return;
    }
    // -------------------------------------------------- CASE 3 : Node has one right child
    if  ( ( delnode->leftPtr == NULL ) &&
        ( ( (delnode->rightPtr != NULL) || (delnode->thread == 0) ) ) ) {
        if (parnode == NULL) {
            sPtr = delnode->rightPtr; // In case of error, reset start pointer
        }
            // Patch parent with the deleted node's right pointer
        else {
            if ( parnode->leftPtr == delnode) // Deleted node is a LEFT CHILD
                parnode->leftPtr = delnode->rightPtr;
            else                              // Deleted node is a RIGHT CHILD
                parnode->rightPtr = delnode->rightPtr;
        }
            // Deletion complete
        return;
    }
    // -------------------------------------------------- CASE 4 : Node has one left child
    if ( ( delnode->leftPtr != NULL )  &&
       ( ( delnode->rightPtr == NULL)  ||  (delnode->thread == 1) ) ) {
        if (parnode == NULL) {
            sPtr = delnode->rightPtr; // In case of error, reset start pointer
        }
            // Patch parent with the deleted node's left pointer
        else {
            if (parnode->leftPtr == delnode) // Delete node is a LEFT CHILD
                parnode->leftPtr = delnode->leftPtr;
            else   	              		     // Delete node is a RIGHT CHILD
                parnode->rightPtr = delnode->leftPtr;
        }
        node1 = delnode->leftPtr;
        while ( (node1->rightPtr != NULL) && (node1->thread !=1) ) {
               node1 = node1->rightPtr;
        }
        node1->rightPtr = delnode->rightPtr;
            // Deletion complate
        return;
    }
    // -------------------------------------------------- CASE 5 : Node has 2 children
    if ( ( delnode->leftPtr != NULL ) &&
         ( delnode->rightPtr != NULL ) && (delnode->thread == 0) ) {
            // Initialize pointers for searching
        node1 = delnode;
        node2 = delnode->leftPtr;
        node3 = delnode->leftPtr;
            // Find the rightmost child in the left sub-tree of deleted node
        while ( (node3 != NULL) && (node3->thread != 1) ) {
            node2 = node3;
            node3 = node3->rightPtr;
        }
        if ( parnode == NULL ) { // In case of error, reset start pointer
            sPtr = node3;
        }
        else {
        // - - - - - - - - - - - - - - SUBCASE : Deleted node is a left child
                // Patch parent with the node
            if (parnode->leftPtr == delnode) {
                parnode->leftPtr = node3;
            }
        // - - - - - - - - - - - - - - SUBCASE : Deleted node is a right child
                // Patch parent with the node
            else {
                parnode->rightPtr = node3;
            }
        }
        // - - - - - - - - - - - - - - SUBCASE : L child of delnode has a right subtree
            // Patch nodes where necessary
        if ( ( node3->leftPtr != NULL) && (node2 != node3) ) {
            node2->rightPtr = node3->leftPtr;
            node3->leftPtr = delnode->leftPtr;
            node3->rightPtr = delnode->rightPtr;
            node3->thread = 0;  // Indicate there is no thread
        }
        // - - - - - - - - - - - - - - SUBCASE : L child of delnode has a left subtree
            // Patch nodes where necessary
        else if (node2 == node3) {
            node3->rightPtr = delnode->rightPtr;
            node3->thread = 0;  // Indicate there is no thread
        }
        // - - - - - - - - - - - - - - SUBCASE : L child of delnode has a left and right subtrees
            // Patch nodes where necessary
        else if (node2->rightPtr != NULL) {
            node2->rightPtr = node3;
            node2->thread = 1;  // Indicate there is a thread
            node3->leftPtr = delnode->leftPtr;
            node3->rightPtr = delnode->rightPtr;
            node3->thread = 0;  // Indicate there is no thread
        }
    }
        // Deletion complete
    return;
};
// --------------------------------------------------------------------------------------------------
// PrintItem
void Tree::PrintItem(ofstream &Outfile, char id[]) {
    
    // Receives - A reference to the output file and the ID specifying the item to print
    // Task - Prints the item data to chart
    // Returns - The output file is updated accordingly
    
    Item *item;                 // Define item pointer
    item = GetItemById(id);     // Point to specified item
    
        // If the item exists, print data to output file
    if (item != NULL) {
        Outfile << left;    // Align left
        Outfile << setw(12) << item->id;
        Outfile << setw(24) << item->name;
        Outfile << right;   // Align right
        Outfile << setw(6) << item->onHand;
        Outfile << setw(6) << " ";
        Outfile << setw(6) << item->onOrder;
        Outfile << endl;
    }
        // Of the item does not exist, print error message to console
    else {
        cout << "ERROR: PrintItem - Item not found." << endl;
    }
    
    return;
}
// --------------------------------------------------------------------------------------------------
// PrintAll
void Tree::PrintAll(ofstream &outfile, Item *Root) {
    
    // Receives - A reference to the output file and pointer to the starting root of tree
    // Task - Prints all item data from nodes starting at specified root
    // Returns - The output file is updated accordingly
    
    int RightThread;    // Thread indicator
    Item *CurrPtr;      // Current pointer traversing the tree
    
        // Initialize current pointer at the root
    CurrPtr = Root;

        // Move to the leftmost node in the tree
    while ( CurrPtr->leftPtr != NULL) {
        CurrPtr  = CurrPtr->leftPtr;
    }
        // Process all nodes in order
    while (CurrPtr != NULL)
    {
            // Print current root node data
        outfile << left;
        outfile << setw(12) << CurrPtr->id;
        outfile << setw(24) << CurrPtr->name;
        outfile << right;   // Align right
        outfile << setw(6) << CurrPtr->onHand;
        outfile << setw(6) << " ";
        outfile << setw(6) << CurrPtr->onOrder;
        outfile << endl;
        
            // Get thread status and move to the right pointer
        RightThread = CurrPtr->thread;
        CurrPtr = CurrPtr->rightPtr;
            // If there is no right thread, move to any remaining left children
        if ( ( CurrPtr != NULL )  && ( RightThread == 0) ) {
                // Process left children until finished
            while ( CurrPtr->leftPtr != NULL) {
                CurrPtr = CurrPtr->leftPtr;
            }
        }
    }   // Finished processing all nodes in tree
    
    return;
};

//****************************************************************************************************

//******************************************* MAIN ***************************************************

// Main functions
void Header(ofstream &);                            // Prints the output header
void Footer(ofstream &);                            // Prints the output footer
void PageBreak(ofstream &, int);                    // Inserts a page break
void PrintHeadings(ofstream &);                     // Prints the item chart headings
void InsertDivider(ofstream &);                     // Inserts a divider

int main() {
        // Get input and output files
    ifstream inputFile("thread_in.txt", ios::in);   // Input file
    ofstream outputFile("output.txt", ios::out);    // Output file
    
        // Create processing variables
    Tree    ItemTree;                               // Tree to hold item database
    Item    newItem;                                // New item placeholder
    Item*   currentItem;                            // Item pointer
    char    command;                                // Command signifier
    
        // Item data
    char    id[6];                                  // Item ID
    char    name[26];                               // Item name/description
    int     onHand;                                 // Quantity of item on hand
    int     onOrder;                                // Quantity of item on order
    int     quantity;                               // Multi-purpose quantity variable
    
        // Initialize line count
    linesUsed = 0;
    
        // Initialize new node child pointers to NULL
    newItem.leftPtr = NULL;
    newItem.rightPtr = NULL;
    
        // Print output header
    Header(outputFile);
    linesUsed+=4;   // Update line count
    
    
        // Process input file, completing tasks accordingly
    do {
            // Read command from data file
        inputFile >> command;
        inputFile >> ws;
        
            // Process command accordingly
        switch (command) {
                
                // "Quit" command       ------------------------------------------------------------
            case 'Q':                                           // QUIT
                    // Finish processing all commands
                break;
                // "Insert" command     ------------------------------------------------------------
            case 'I':                                           // INSERT
                    // Read item data
                inputFile.get(id, 6);
                inputFile >> ws;
                inputFile.get(name, 26);
                inputFile >> ws;
                inputFile >> onHand;
                inputFile >> onOrder;
                inputFile >> ws;
                    // Add data to new item node
                strcpy(newItem.id, id);
                strcpy(newItem.name, name);
                newItem.onHand = onHand;
                newItem.onOrder = onOrder;
                
                    // Make sure item is not a duplicate
                if ( !ItemTree.ItemExists(id) ) {
                        // Insert new item node into tree
                    ItemTree.Insert(newItem);
                        // Print update
                    outputFile << "Item ID Number " << id;
                    outputFile << " successfully entered into database." << endl;
                }
                    // In case of duplicate, print error
                else {
                    outputFile << "ERROR --- ";
                    outputFile << "Attempt to insert a duplicate item (" << id << ") ";
                    outputFile << "into the database." << endl;
                }
                    // Insert output divider
                InsertDivider(outputFile);
                linesUsed+=2;   // Update line count
                    // Finished command
                break;
                
                // "Delete" command     ------------------------------------------------------------
            case 'D':                                           // DELETE
                    // Read deletion request data
                inputFile.get(id, 6);
                inputFile >> ws;
                inputFile.get(name, 26);
                inputFile >> ws;
                
                    // If item is in the database, delete it
                if ( ItemTree.ItemExists(id) ) {
                        // Remove item from the item tree
                    ItemTree.Delete(id);
                        // Print update
                    outputFile << "Item ID Number " << id;
                    outputFile << " successfully deleted from database." << endl;
                }
                    // If the item is not in the database, print error
                else {
                    outputFile << "ERROR --- ";
                    outputFile << "Attempt to delete an item (" << id << ") ";
                    outputFile << "not in the database list." << endl;
                }
                    // Insert output divider
                InsertDivider(outputFile);
                linesUsed+=2;   // Update line count
                    // Finished command
                break;

                // Sales transaction    ------------------------------------------------------------
            case 'S':                                           // SALES TRANSACTION
                    // Read transaction info from file
                inputFile.get(id, 6);
                inputFile >> ws;
                inputFile >> quantity;
                
                    // Make sure item is in database
                if (ItemTree.ItemExists(id)) {
                        // Point to the item to update
                    currentItem =  ItemTree.GetItemById(id);
                        // Make sure current quantity on hand is sufficient
                    if (currentItem->onHand >= quantity) {
                            // Update item info
                        currentItem->onHand-=quantity;
                            // Print update
                        outputFile << "Quantity on hand for item " << id;
                        outputFile << " successfully updated." << endl;
                    }
                        // If there is not enough of the item on hand, print error
                    else {
                        outputFile << "ERROR --- ";
                        outputFile << "Attempt to update an item (" << id << ") ";
                        outputFile << "with insuffient quanitity on hand." << endl;
                    }
                } // Transaction complete
                    // If the item is not in database, print error
                else {
                    outputFile << "ERROR --- ";
                    outputFile << "Item " << id << " not on database. ";
                    outputFile << "Data not updated." << endl;
                }
                    // Insert output divider
                InsertDivider(outputFile);
                linesUsed+=2;   // Update line count
                    // Finished command
                break;
                
                // Restock transaction  ------------------------------------------------------------
            case 'O':                                           // RESTOCK TRANSACTION
                    // Read transaction info from file
                inputFile.get(id, 6);
                inputFile >> ws;
                inputFile >> quantity;
                
                    // Make sure item is in database
                if (ItemTree.ItemExists(id)) {
                        // Get the item to update
                    currentItem =  ItemTree.GetItemById(id);
                        // Update item info
                    currentItem->onHand+=quantity;
                        // Print update
                    outputFile << "Quantity on hand for item " << id;
                    outputFile << " successfully updated." << endl;
                }
                    // If the item is not in database, print error
                else {
                    outputFile << "ERROR --- ";
                    outputFile << "Item " << id << " not on database. ";
                    outputFile << "Data not updated." << endl;
                }
                    // Insert output divider
                InsertDivider(outputFile);
                linesUsed+=2;   // Update line count
                    // Finished command
                break;
                
                // Receival of shipment ------------------------------------------------------------
            case 'R':                                           // RECEIVAL OF SHIPMENT
                    // Read transaction info from file
                inputFile.get(id, 6);
                inputFile >> ws;
                inputFile >> quantity;
                
                    // Make sure item is in database
                if (ItemTree.ItemExists(id)) {
                        // Get the item to update
                    currentItem =  ItemTree.GetItemById(id);
                        // Transfer quantity from on-order to on-hand
                    currentItem->onOrder-=quantity;
                    currentItem->onHand+=quantity;
                        // Print updates
                    outputFile << "Quantity on order for item " << id;
                    outputFile << " successfully updated." << endl;
                    InsertDivider(outputFile);  // Insert divider between updates
                    outputFile << "Quantity on hand for item " << id;
                    outputFile << " successfully updated." << endl;
                    linesUsed+=4;   // Update line count
                }
                    // If the item is not in database, print error
                else {
                    outputFile << "ERROR --- ";
                    outputFile << "Item " << id << " not on database. ";
                    outputFile << "Data not updated." << endl;
                }
                    // Insert output divider
                InsertDivider(outputFile);
                
                    // Finished command
                break;
                // Print comment        ------------------------------------------------------------
            case 'P':                                           // PRINT
                    // Read print type from file
                inputFile >> command;
                inputFile >> ws;
                    // Complete print task based on type
                switch (command) {
                    case 'E':   // Print ENTIRE database
                            // Insert page break
                        PageBreak(outputFile, linesUsed);
                        linesUsed = 0;  // Reset line count
                            // Print item heading
                        PrintHeadings(outputFile);
                        linesUsed+=3;   // Update line count
                            // Print entire tree (from root)
                        ItemTree.PrintAll(outputFile, ItemTree.GetRoot());
                        linesUsed+=ItemTree.GetCount();    // Update line count
                            // Insert page break
                        PageBreak(outputFile, linesUsed);
                        linesUsed = 0;  // Reset line count
                            // Finished print
                        break;
                    case 'N':   // Print INDIVIDUAL item
                        inputFile.get(id, 6);
                        inputFile >> ws;
                            // Make sure item exists
                        if (ItemTree.ItemExists(id)) {
                                // Print item heading
                            PrintHeadings(outputFile);
                            linesUsed+=3;   // Update line count
                                // Print item data to output file
                            ItemTree.PrintItem(outputFile, id);
                            linesUsed+=ItemTree.GetCount(); // Update line count
                                // Insert divider
                            InsertDivider(outputFile);
                            linesUsed++;    // Update line count
                        }
                            // If item does not exist, print error
                        else {
                            outputFile << "ERROR --- ";
                            outputFile << "Attempt to print an item (" << id << ") ";
                            outputFile << "not in the database list." << endl;
                                // Insert divider
                            InsertDivider(outputFile);
                            linesUsed+=2;    // Update line count
                        }
                            // Finished print
                        break;
                    default:    // In case of unknown print type or syntax error, print error
                        cout << "ERROR: Unknown print type." << endl;
                        break;
                }
                    // Finished command
                break;
                // Unknown command      ------------------------------------------------------------
            default:                                            // ERROR
                cout << "ERROR: Unknown command." << endl;
                break;
        }
        // Continue processing until the quit command
    } while ( command != 'Q');
    
        // Print output footer
    Footer(outputFile);
    
        // Return to operating system
    return 0;
}
//****************************************************************************************************

//****************************************************************************************************
void InsertDivider(ofstream &Outfile) {
    
    // Receives - A reference to the output file
    // Task - Prints a divider of 60 dashes (-)
    // Returns - The output file is updated accordingly
    
    Outfile << "------------------------------";
    Outfile << "------------------------------\n";
    
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void PrintHeadings(ofstream &Outfile) {
    
    // Receives - A reference to the output file
    // Task - Prints headings for item chart
    // Returns - The output file is updated accordingly
    
        // Align left and print headings
    Outfile << left;
    Outfile << setw(12) << "Item";
    Outfile << setw(24) << "Item";
    Outfile << setw(12) << "Quantity";
    Outfile << setw(12) << "Quantity";
    Outfile << endl;
    Outfile << setw(12) << "ID Number";
    Outfile << setw(24) << "Description";
    Outfile << setw(12) << "On Hand";
    Outfile << setw(12) << "On Order";
    Outfile << endl;
    InsertDivider(Outfile); // Insert a divider
    
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
    Outfile << setw(20) << "Assignment #8" <<endl;
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