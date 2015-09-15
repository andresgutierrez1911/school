//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 2             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: Feb 25, 2015       *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:       The purpose of this program is to read in a list of requested transactions and   *
//*                 data to be recorded. The data is stored as a customer mailing list.              *
//*                                                                                                  *
//*  USER DEFINED                                                                                    *
//*    MODULES     :    Header..........................Prints the output heading                    *
//*                     Footer..........................Prints the output footer                     *
//*                     ReadInput.......................Processes the instructions listed in the     *
//*                                                         input file                               *
//*                     CustomerList Functions..........(Documented under LIST CLASS)                *
//*                                                                                                  *
//****************************************************************************************************

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PAGEWIDTH 95;       // Set constant page width
#define PAGEHEIGHT 76;      // Set constant page height
int LINESUSED = 0;          // Initialize line count

using namespace std;

// --------------------------------------------------------------------------------------------------
// CUSTOMER TYPE STRUCT
struct Customer {
        // Customer data
    char firstName[13], lastName[13], address[21], city[21], state[3], zip[6];
    int data;
    int status;
        // Pointer to next customer in list
    Customer *next;
};

/* ***************************************** LIST CLASS ******************************************* */
class CustomerList {
public:
    CustomerList();                                 // CustomerList constructor
    bool NodeExists(char[], char[]);                // Tests for the existence of a specified
                                                    //      node in the list
    void InsertNode(Customer);                      // Receives a node of type Customer and
                                                    //      inserts it into the list
    void ChangeNode(char[], char[], int, char[]);   // Updates a field within a specified node
    void PrintList(ofstream &);                     // Prints the record data in the list
    void PrintListHead(ofstream &);                 // Prints the list header
    void RecordError(char, char[], char[]);         // Records error to the list error log
    void PrintErrorLog(ofstream &);                 // Prints the list error log
    void ClearErrorLog();                           // Clears the list error log
    Customer DeleteNode (char[], char[]);           // Deletes a node from the list
private:
    Customer *listStart;                            // Starting point of the list
    string errorLog;                                // Error log
    int errorCount;                                 // Number of errors in log
};

// --------------------------------------------------------------------------------------------------
// CustomerList constructor
CustomerList::CustomerList() {
        // Initialize starting point
    listStart = NULL;
}

// --------------------------------------------------------------------------------------------------
bool CustomerList::NodeExists(char fname[], char lname[]) {
    
    // Receives - The first and last name of queried customer record
    // Task - Searches for the specified record in the list
    // Returns - Returns TRUE if the record is found; Returns FALSE otherwise
    
        // Initialize search pointers
    Customer *PreviousPtr, *CurrentPtr;
    PreviousPtr = NULL;
    CurrentPtr = listStart;
    
        // Search list until a match is found or list is exhausted
    while ( (CurrentPtr != NULL) &&
           !(( strcmp(CurrentPtr->lastName,lname)==0)&&(strcmp(CurrentPtr->firstName,fname)==0))
           ) {
            // Step to the next node
        PreviousPtr = CurrentPtr;
        CurrentPtr = CurrentPtr -> next;
    }
        // If a match was found, return TRUE
        // If the list exhausted without finding a match, return FALSE
    return (CurrentPtr != NULL);
}

// --------------------------------------------------------------------------------------------------
void CustomerList::InsertNode(Customer newCustomer) {
    
    // Receives - A structure of type Customer
    // Task - Attemps to insert new customer record into the customer list
    // Returns - If successful, the list is updated accordingly
    
        // Tests for the node is being placed alphabetically correctly
    bool alphaTest = true;
    
        // Create search pointers
    Customer *newNode, *PreviousPtr, *CurrentPtr;
        // Reserve space for new node
    newNode = new (Customer);
        // Insert data if there is memory available
    if ( newNode != NULL) {
        
            // Copy new customer data to new node
        strcpy(newNode->firstName, newCustomer.firstName);
        strcpy(newNode->lastName, newCustomer.lastName);
        strcpy(newNode->address, newCustomer.address);
        strcpy(newNode->city, newCustomer.city);
        strcpy(newNode->state, newCustomer.state);
        strcpy(newNode->zip, newCustomer.zip);
        
            // Initialize search pointers
        PreviousPtr = NULL;
        CurrentPtr = listStart;
        
            // If list is currently empty, insert item into the first place
        if (CurrentPtr == NULL) {
                // The new item will point to NULL
            newNode -> next = NULL;
                // Set new item as the list starting point
            listStart = newNode;
            
            // The item has been inserted
        }
            // Otherwise, find where in the list to insert the node
        else {
                // Search through list for the location of where to insert the new node
                // (Search until list is exhausted or correct placement is found)
            while ( (CurrentPtr != NULL) && alphaTest )
            {
                    // If a node with an alphabetically larger last name is found,
                    // we have found the location to insert the node
                if (strcmp(CurrentPtr->lastName, newNode->lastName) > 0) {
                        // Flag alphaTest to abort search
                    alphaTest = false;
                }
                    // If a matching last name is found, but the first name is
                    // alphabetically larger, then the location has been found
                else if ( (strcmp(CurrentPtr->lastName, newNode->lastName) == 0)
                       && (strcmp(CurrentPtr->firstName, newNode->firstName) > 0) )
                {
                        // Flag alphaTest to abort search
                    alphaTest = false;
                }
                    // Otherwise, continue searching
                else {
                        // Step to next node
                    PreviousPtr = CurrentPtr;
                    CurrentPtr = CurrentPtr -> next;
                }
            }
                // If the new node belongs at the beginning of the list
            if (PreviousPtr == NULL) {
                    // Insert new node into the first place of the list
                newNode->next = listStart;
                listStart = newNode;
            }
                // Otherwise, insert the node between the appropriate neighbors
            else {
                PreviousPtr -> next = newNode;
                newNode -> next  = CurrentPtr;
            }
        }
    }   // Finished inserting node
    
        // If there is no free memory, print an error message
    else
    {
            // Print error message to console
        cout << "Error! Out of memory!";
    }
    return;
}

// --------------------------------------------------------------------------------------------------
void CustomerList::ChangeNode(char fname[], char lname[], int field, char newVal[]) {
    
    // Receives - The first and last name of a specified customer record, a specfified
    //              field to change, and the new value for said field
    // Task - Applies change of new value to the specified field in the specified record
    // Returns - The specified node is edited accordingly
    
        // Create temporary node placeholder
    Customer movingNode;
    
        // Initialize search pointers
    Customer *PreviousPtr, *CurrentPtr;
    PreviousPtr = NULL;
    CurrentPtr = listStart;
    
        // Search list until a match is found or list is exhausted
    while ( (CurrentPtr != NULL)
           && ( strcmp(CurrentPtr->lastName, lname) != 0 )
           && ( strcmp(CurrentPtr->firstName, fname) != 0 ) ) {
        PreviousPtr = CurrentPtr;
        CurrentPtr = CurrentPtr -> next;
    }
        // If the node was found in the list, apply change
    if (CurrentPtr != NULL) {
        
            // Apply change according to the specified field
        switch (field) {
            case 1:
                    // Change first name and readjust position in list
                strcpy(CurrentPtr->firstName, newVal);
                movingNode = DeleteNode(newVal, lname);
                InsertNode(movingNode);
                break;
            case 2:
                    // Change last name and readjust position in list
                strcpy(CurrentPtr->lastName, newVal);
                movingNode = DeleteNode(fname, newVal);
                InsertNode(movingNode);
                break;
            case 3:
                    // Change the address
                strcpy(CurrentPtr->address, newVal);
                break;
            case 4:
                    // Change the city
                strcpy(CurrentPtr->city, newVal);
                break;
            case 5:
                    // Change the state
                strcpy(CurrentPtr->state, newVal);
                break;
            case 6:
                    // Change the zip code
                strcpy(CurrentPtr->zip, newVal);
                break;
            default:
                    // If the specified field is not valid, print error message
                cout << "Error: Change error - Unknown field." << endl;
                break;
        }
    }
        // If the node was not found in the list, print a error message
    else {
        // Error
        cout << "Error: An error ocurred while atempting to change node." << endl;
    }
    return;
}

// --------------------------------------------------------------------------------------------------
Customer CustomerList::DeleteNode (char fname[], char lname[]) {
    
    // Receives - The first and last name of customer to be deleted
    // Task - Attempts to remove specified record from the list
    // Returns - If successful, the list is updated, and the data from the deleted node is returned
    
        // Create necessary pointers
    Customer *tempPtr, *PreviousPtr, *CurrentPtr;
    
        // If the node to be deleted is the first node, just adjust starting point
    if ( (lname  == listStart->lastName) && (fname  == listStart->firstName) )
    {
            // Assign 2nd node to the starting point exclude the first node
        tempPtr = listStart;
        listStart = listStart -> next;
        
            // Deletion successful; Return data from deleted node
        return *tempPtr;
    }
        // Otherwise, find the node to delete
    else
    {
            // Initialize search pointers
        PreviousPtr = listStart;
        CurrentPtr = listStart -> next;
        
            //  Search list until the node to delete is found (or the list exhausts)
        while ( (CurrentPtr != NULL)
               && ( strcmp(CurrentPtr->lastName, lname) != 0 )
               && ( strcmp(CurrentPtr->firstName, fname) != 0 ) )
        {
                // Step to the next node
            PreviousPtr = CurrentPtr;
            CurrentPtr = CurrentPtr -> next;
        }
            // If the node is found, remove it from the list
        if  (CurrentPtr != NULL)
        {
                // Readjust list to exclude deleted node
            tempPtr = CurrentPtr;
            PreviousPtr->next = CurrentPtr->next;
            tempPtr->next = NULL;
            
                // Deletion successful; Return data from deleted node
            return *tempPtr;
        }
    }
        // If the node was not found, return indication of failure
    tempPtr->status = -1;
    return *tempPtr;
}

// --------------------------------------------------------------------------------------------------
void CustomerList::PrintList(ofstream &outfile) {
    
    // Receives - The output file
    // Task - Prints list of data records to the output file
    // Returns - The output file is updated
    
        // Create temporary record pointer
    Customer  *CurrentPtr;
        // Initialize a current pointer to the start of the linked list
    CurrentPtr = listStart;
        // Check if the list is empty
    if ( CurrentPtr  == NULL )
    {
        // If so, print a message saying so
        outfile  << "The list is empty." << endl;
        return;
    }
        // Process the list
    while ( CurrentPtr != NULL )
    {
            //  Print the data for a node
        outfile << CurrentPtr->lastName << " ";
        outfile << CurrentPtr->firstName << " ";
        outfile << CurrentPtr->address << "  ";
        outfile << CurrentPtr->city << "  ";
        outfile << CurrentPtr->state << "     ";
        outfile << CurrentPtr->zip;
        outfile << endl;
            // Update line count accordingly
        LINESUSED++;
            //  Move to the next NODE
        CurrentPtr = CurrentPtr->next;
    }
    outfile << endl;
    LINESUSED++;    // Increment line count
    return;
}

// --------------------------------------------------------------------------------------------------
void CustomerList::PrintListHead(ofstream &outfile) {
    
    // Receives - The output file
    // Task - Prints list heading to the output file
    // Returns - The output file is updated
    
        // Print list header
    outfile << "                                ";
    outfile << "MAILING LIST" << endl;
    outfile << "Last Name    ";
    outfile << "First Name   ";
    outfile << "Address               ";
    outfile << "City         ";
    outfile << "State   ";
    outfile << "Zip " << endl;
    outfile << "===========================================================================";
    outfile << endl;
    return;
}

// --------------------------------------------------------------------------------------------------
void CustomerList::RecordError(char mode, char fname[], char lname[]) {
    
    // Receives - The error type (mode) and the first and last name of offending record
    // Task - Creates an error report for specified error and appends it to the list error log
    // Returns - The error log for the customer list is updated
    
        // Initialize new error message
    string newError = "\n";
    
        // Determine error type and compose new message
    switch (mode) {
        case 'A':   // Add error
            newError+="* ";
            newError+=fname;
            newError+=" ";
            newError+=lname;
            newError+=" is already in the list. Attempt to add duplicate record failed!\n";
            break;
        case 'C':   // Change error
            newError+="* Record of ";
            newError+=fname;
            newError+=lname;
            newError+=" not found. Attempt to change record failed!\n";
            break;
        case 'D':   // Delete error
            newError+="* Record of ";
            newError+=fname;
            newError+=lname;
            newError+=" not found. Attempt to delete record failed!\n";
            break;
        default:    // Error error: Invalid error
            cout << "Error: An error occured while recording an error.";
            break;
    }
        // Append new error message to the list error log
    errorLog+=newError;
    
    return;
}

// --------------------------------------------------------------------------------------------------
void CustomerList::PrintErrorLog(ofstream &outfile) {
    
    // Receives - The output file
    // Task - Print error log to the output file
    // Returns - The error log is printed to the output file
    
        // Print the error log to the output file
    outfile << errorLog;
    return;
}

// --------------------------------------------------------------------------------------------------
void CustomerList::ClearErrorLog() {
    
    // Receives - Nothing
    // Task - Clears the error log for the list
    // Returns - The local error log is cleared
    
        // Clear the error log
    errorLog = " ";
    return;
}

/* *************************************** END LIST CLASS ***************************************** */

//******************************************* MAIN ***************************************************

    // Main function prototypes
void Header(ofstream &);
void Footer(ofstream &);
void ReadInput(ifstream &, ofstream &);

int main() {
    
        // Get input file
    ifstream inputFile("data2V6.txt", ios::in);
        // Get output file
    ofstream outputFile("output.txt", ios::out);
    
        // Process input file
    ReadInput(inputFile, outputFile);
    
        // Print output footer
    Footer(outputFile);
    
        // Close data files
    inputFile.close();
    outputFile.close();
    
        // Return to operating system
    return 0;
}
//****************************************************************************************************

//************************************* FUNCTION READINPUT *******************************************
void ReadInput(ifstream &infile, ofstream &outfile)
{
    // Receives - The input file and the output file
    // Task - Reads input and processes transactions accordingly
    // Returns - The input instructions are carried out, and the output file is printed
    
        // Create local variables
    char    act;                        // action command
    char    fname[13];                  // first name
    char    lname[13];                  // last name
    char    nname[13];                  // new name
    char    address[21];                // customer address
    char    city[21], state[3], zip[6]; // customer address pt2
    int     field = 0;                  // field number
    
    int maxLines = PAGEHEIGHT;          // max number of lines for an output page

    Customer *newGuy;                   // new customer
    Customer tempCustomer;              // temporary customer placeholder
    
        // Create a list for all customer records
    CustomerList list;
    
        // Read the first action command from input file
    act = infile.get();
    infile >> ws;
    
        // Read action commands from file until the QUIT command
    while (act != 'Q') {
            // Process action accordingly
        switch (act) {
                
                // ----- ADD -----
            case 'A':
                    // Create a new customer
                newGuy = new (Customer);
                
                    // Read in customer data from file
                infile >> ws;
                infile.get(fname, 13);
                infile.get(lname, 13);
                infile >> ws;
                infile.get(address, 21);
                infile >> ws;
                infile.get(city, 13);
                infile >> ws;
                infile.get(state, 3);
                infile >> ws;
                infile.get(zip, 6);
                
                    // Assign customer data to new customer
                strcpy(newGuy->firstName, fname);
                strcpy(newGuy->lastName, lname);
                strcpy(newGuy->address, address);
                strcpy(newGuy->city, city);
                strcpy(newGuy->state, state);
                strcpy(newGuy->zip, zip);
                
                    // If the new node is not a duplicate, add it to the list
                if ( !(list.NodeExists(fname, lname)) ) {
                        // Insert new node into the list
                    list.InsertNode(*newGuy);
                }
                    // In case of duplicate, record error message
                else {
                    // Error: DUPLICATE ENTRY
                    list.RecordError(act, fname, lname);
                    LINESUSED+=2;   // Increment output line count accordingly
                }
                    // Finished add
                break;
                
                // ----- DELETE -----
            case 'D':
                    // Read in record data
                infile >> ws;
                infile.get(fname, 13);
                infile >> ws;
                infile.get(lname, 13);
                
                    // If the customer exists, delete it
                if (list.NodeExists(fname, lname)) {
                        // Remove customer from the list of records
                    tempCustomer = list.DeleteNode(fname, lname);
                }
                    // If the customer is not in the list, record an error
                else {
                        // Error: RECORD NOT FOUND
                    list.RecordError(act, fname, lname);
                    LINESUSED+=2;   // Increment output line count accordingly
                }
                    // Finished delete
                break;
                
                // ----- CHANGE -----
            case 'C':
                
                char f; // temporary field char indicator
                
                    // Read in record data
                infile >> ws;
                infile.get(fname, 13);
                infile >> ws;
                infile.get(lname, 13);
                
                    // Read in the specified field (as single char)
                infile.get(f);
                    // Convert field to int by subtracting 48, the acii value of 0
                atoi(&f);
                field = (f - 48);
                
                
                    // If the specified customer is on record, proceed with change
                if (list.NodeExists(fname, lname)) {

                    // Based on field number, apply changes to records
                    switch (field) {
                            
                        // Change first or last name
                        case 1:
                        case 2:
                                // Get new first name or last name from file
                            infile.get(nname, 13);
                                // Apply change to customer name
                            list.ChangeNode(fname, lname, field, nname);
                            break;
                            
                        // Change address
                        case 3:
                                // Get new address from file
                            infile.get(address, 21);
                                // Apply change to customer address
                            list.ChangeNode(fname, lname, field, address);
                            break;
                            
                        // Change city
                        case 4:
                                // Get new city name from file
                            infile.get(city, 13);
                                // Apply change to customer city
                            list.ChangeNode(fname, lname, field, city);
                            break;
                            
                        // Change state
                        case 5:
                                // Get new state name from file
                            infile.get(state, 3);
                                // Apply change to customer state
                            list.ChangeNode(fname, lname, field, state);
                            break;
                            
                        // Change zip code
                        case 6:
                                // Get new zip code from file
                            infile.get(zip, 6);
                                // Apply change to customer zip code
                            list.ChangeNode(fname, lname, field, zip);
                            break;
                            
                        // If invalid field is specified, display error message
                        default:
                            // Display error
                            cout << "Error: Invalid field specification.";
                            break;
                            
                    } // End switch
                }
                // If the specified customer is not found, record error
                else {
                        // Error: RECORD NOT FOUND
                    list.RecordError(act, fname, lname);
                    LINESUSED+=2;   // Increment output line count accordingly
                    
                        // Also finish reading through line in input file.
                        // Read characters according to the field specified.
                    switch (field) {
                        case 1:
                        case 2: infile.get(nname, 13);
                            break;
                        case 3: infile.get(address, 21);
                            break;
                        case 4: infile.get(city, 13);
                            break;
                        case 5: infile.get(state, 3);
                            break;
                        case 6: infile.get(zip, 6);
                            break;
                        default:
                                // Display error: invalid field
                            cout << "Error: Invalid field specification.";
                            break;
                    } // Finish reading through line
                }
                    // Finished change
                break;
                
                // ----- PRINT -----
            case 'P':
                    // Print output header
                Header(outfile);
                LINESUSED+=4;               // Increment line count accordingly
                    // Print list heading
                list.PrintListHead(outfile);
                LINESUSED+=3;               // Increment line count accordingly
                    // Print mailing list
                list.PrintList(outfile);
                    // Print error log
                list.PrintErrorLog(outfile);
                    // Clear error log
                list.ClearErrorLog();
                    // Insert page break
                for (int i=0; i < (maxLines - LINESUSED); i++) {
                    outfile << endl;
                }
                    // Reset line count
                LINESUSED = 0;
                break;
                
                // In case of error (invalid action)
            default:
                    // Print error message to console
                cout << "Error: Invalid action specification." << endl;
                break;
        } // Finish processing action
        
            // Read the next action from file
        infile >> ws;
        act = infile.get();
    }
        // Finish processing input file
    return;
}
//****************************************************************************************************

//*********************************************** HEADER *********************************************
void Header(ofstream &Outfile)
{
    // Receives - The output file
    // Task - Prints the output preamble
    // Returns - Nothing
    
    Outfile << setw(30) << "Katelyn Schaffer";
    Outfile << setw(17) << "CSC 36000";
    Outfile << setw(15) << "Section 11" << endl;
    Outfile << setw(30) << "Spring 2015";
    Outfile << setw(20) << "Assignment #2" <<endl;
    Outfile << setw(35) << "-----------------------------------";
    Outfile << setw(35) << "-----------------------------------\n\n";
    
    return;
}
//****************************************************************************************************

//************************************************ FOOTER ********************************************
void Footer(ofstream &Outfile)
{
    // Receives - The output file
    // Task - Prints the output salutation
    // Returns - Nothing
    
    Outfile << endl;
    Outfile << setw(35) << " --------------------------------- " << endl;
    Outfile << setw(35) << "|      END OF PROGRAM OUTPUT      |" << endl;
    Outfile << setw(35) << " --------------------------------- " << endl;
    
    return;
}
//****************************************************************************************************