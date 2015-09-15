//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 7             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: April 27, 2015     *
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
    
    Item *leftChild;    // Pointer to left child of item node
    Item *rightChild;   // Pointer to right child of item node
};
// --------------------------------------------------------------------------------------------------

//******************************************* TREE ***************************************************
class Tree {
public:
         Tree();                                    // Constructor
    int  GetCount() { return count; }               // Returns the number of nodes in tree
    Item *GetRoot() { return root; }                // Returns root node
    Item *GetItemById(char[]);                      // Returns specific item according to ID
    Item *Search(char[], int&, Item*);              // Searches by ID for a specific item
    bool ItemExists(char[]);                        // Returns whether or not a specific item exists
    void Insert(Item);                              // Inserts an item node into the tree
    void Delete(char[]);                            // Removes item from tree, returning the address
    void PatchParent(Item*, Item*, Item*);          // Readjusts tree after removal of node
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
    int stat = 0;           // Initialize search status to 0
    
    // Find the specified item, starting search at root
    requestedItem = Search(query, stat, root);
    
    // If the item is found, return pointer to item
    if (stat == 1)
        return requestedItem;
    // Otherwise, print error message to console, and return NULL
    else  {
        cout << "Error: Requested item not found." << endl;
        return requestedItem;
    }
}

// --------------------------------------------------------------------------------------------------
// Search
Item* Tree::Search(char query[], int &stat, Item *Root) {
    
    // Receives - The ID of item queried, an integer status indicator, and a pointer to the
    //              root of tree/subtree to search
    // Task - Searches tree InOrder for the item with the queried ID
    // Returns - The status indicator is updated and a pointer to the found item is returned
    
    Item *queriedItem;      // Define pointer to queried item
    queriedItem = NULL;     // Initialize quieried item pointer to NULL
    
    // Search tree from the specified root node if it exists
    if ( Root != NULL) {
        // Search left tree if the queried item is not found yet
        if (stat<1) queriedItem = Search(query, stat, Root->leftChild);
        
        // Check if current node id matches that of the query (if the item is not found yet)
        if (stat<1) if ( strcmp(Root->id, query) == 0 ) {
            // If so, the queried item is found
            stat = 1;
            queriedItem = Root;
            return queriedItem; // Return the item
        }
        // Search right tree if the queried item is not found yet
        if (stat<1) queriedItem = Search(query, stat, Root->rightChild);
    }
    // Return the item
    return queriedItem;
}
// --------------------------------------------------------------------------------------------------
// ItemExists
bool Tree::ItemExists(char query[]) {
    
    // Receives - An item ID
    // Task - Determine whether or not the item specified by ID exists in the database
    // Returns - True if the item exists; False otherwise
    
    Item *searched;     // Define item pointer
    searched = NULL;    // Initialize item pointer to NULL
    int stat = 0;       // Initialize search status to 0
    // Search for specified item, starting at tree root
    searched = Search(query, stat, root);
    // Return true if the item is found; return false otherwise
    if (stat == 1)  return true;
    else            return false;
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
            // Initialize new item pointers to NULL
        newItem->leftChild = NULL;
        newItem->rightChild = NULL;
        
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
                    if (current->leftChild != NULL) {
                        current = current->leftChild; // (Now currently pointing to left child)
                    }
                        // Otherwise, insert new node into left child location
                    else {
                        current->leftChild = newItem;   // Insert new item into tree
                        inserted = true;                // The item is now inserted
                    }
                }
                    // If current node ID value is smaller than that of the new item
                else {
                        // If the current node has a right child, SEARCH RIGHT SUBTREE
                    if (current->rightChild != NULL) {
                        current = current->rightChild; // (Now currently pointing to right child)
                    }
                        // Otherwise, insert new node into right child location
                    else {
                        current->rightChild = newItem;  // Insert new item into tree
                        inserted = true;                // The item is now inserted
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
    
        // Initialize node pointers
    Item *delnode, *parent, *node1, *node2, *node3;
        // Initialize "found" flag (The specified node has not been found yet)
    bool found = false;

    delnode = root;     // Point to the root of the tree
    parent = NULL;      // Initialize parent pointer to NULL
    
        // Search for specified node according to ID
    while ((found == false) && (delnode != NULL)) {
            // If a node's ID matches that of the query, the node has been found
        if ( strcmp(delnode->id, id) == 0 ) {
            found = true;
        }
            // Otherwise keep searching
        else {
                // Save place of parent node
            parent = delnode;
                // Search left subtree or right subtree as necessary
            if ( strcmp(delnode->id, id) > 0 )
                delnode = delnode->leftChild;  // Search left subtree if queried ID is larger
            else
                delnode = delnode->rightChild; // Otherwise search right subtree
        }
    }   // Continue searching until queried node is found
    
        // Now readjust the tree for node deletion
                                                             //  CASE 1 – Node is not in tree
        // If the node has not been found, print error to console
    if (found == false) {
        cout << "Error: Node is not in the tree. Delete failed." << endl;
    }
    else
    {
        if (delnode->leftChild == NULL) {
            if (delnode->rightChild == NULL)                 // CASE 2 – Node has NO children
            {
                    // Remove node from tree
                PatchParent(NULL, parent, delnode);
            }
            else                                             // CASE 3 – Node has ONE right child
            {
                    // Remove node from tree and patch with right child
                PatchParent(delnode->rightChild, parent, delnode);
            }
        } // Finished handling cases where there is no left child
        else
        {
            if(delnode->rightChild == NULL)                 // CASE 4 – Node has ONE left child
            {
                    // Remove node from tree and patch with left child
                PatchParent(delnode->leftChild, parent, delnode);
            }
            else {                                          // CASE 5 – Node has TWO children
                    // Prepare node pointers for search
                node1 = delnode;
                node2 = delnode->leftChild;
                node3 = node2->rightChild;
                    // Find rightmost node of left subtree to replace deleted node
                while(node3 != NULL) {
                    node1 = node2;
                    node2 = node3;
                    node3 = node3->rightChild;
                }
                if (node1 != delnode){
                    node1->rightChild = node2->leftChild;
                    node2->leftChild = delnode->leftChild;
                }
                    // Remove node and patch with rightmost node of left subtree
                node2->rightChild = delnode->rightChild;
                PatchParent(node2, parent, delnode);
            }   // Finished handling two-children case
        }   // Finished handling cases where there is a left child
        
        count--;    // Decrement tree node count
        
    }   // Finished readjusting tree for node deletion
    
    return;
}
// --------------------------------------------------------------------------------------------------
// PatchParent
void Tree::PatchParent(Item *child, Item *parent, Item *delnode) {
    
    // Receives - Pointer to a child Item node for patching, pointer to the parent node whose
    //              child is to be replaced, and pointer to the node being removed
    // Task - Replace deleted node with a new node patch
    // Returns - The tree is updated accordingly
    
        // If there is no parent, the child will patch as the root of the tree
    if (parent == NULL)
        root = child;
        // Otherwise, patch parent node's right or left child (according to the deleted node)
    else {
        if(delnode == parent->leftChild)
            parent->leftChild = child;      // Patch left child
        else
            parent->rightChild = child;     // Patch right child
    }
    return;
}
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
        Outfile << left;
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
void Tree::PrintAll(ofstream &Outfile, Item *Root) {
    
    // Receives - A reference to the output file and pointer to the starting root of tree
    // Task - Prints all item data from nodes starting at specified root
    // Returns - The output file is updated accordingly
    
        // Align left
    Outfile << left;
        // If current root node exists, print any subtrees and current node data
    if ( Root != NULL) {
            // Print left subtree
        PrintAll(Outfile, Root->leftChild);
        
            // Print current root node data
        Outfile << setw(12) << Root->id;
        Outfile << setw(24) << Root->name;
        Outfile << right;   // Align right
        Outfile << setw(6) << Root->onHand;
        Outfile << setw(6) << " ";
        Outfile << setw(6) << Root->onOrder;
        Outfile << endl;
        
            // Print right subtree
        PrintAll(Outfile, Root->rightChild);
    }
    
    return;
}

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
    ifstream inputFile("tree_in.txt", ios::in);     // Input file
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
    newItem.leftChild = NULL;
    newItem.rightChild = NULL;
    
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
    Outfile << setw(20) << "Assignment #7" <<endl;
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