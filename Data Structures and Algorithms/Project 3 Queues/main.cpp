//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 3             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: Mar 9, 2015        *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:         This program is designed to read in passenger information and assign           *
//*                   passengers to their seats according to their requests and the seating rules.   *
//*  USER DEFINED                                                                                    *
//*    MODULES     :   Header..........................Prints the output heading                     *
//*                    Footer..........................Prints the output closing                     *
//*                    LoadCustomers...................Loads customer data from file into a queue    *
//*                    PrintHeaders....................Prints table headers to the output file       *
//*                    PrintResults....................Prints program results to the output file     *
//*                                                                                                  *
//*                    [CheckoutQueue Functions].......[Documented under QUEUE CLASS]                *
//*                                                                                                  *
//****************************************************************************************************

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PAGEWIDTH 95;       // Set constant page width
#define PAGEHEIGHT 76;      // Set constant page height

using namespace std;

// --------------------------------------------------------------------------------------------------
// CUSTOMER STRUCT
struct Customer {
    char name[25];          // Customer full name
    int arrivalTime;        // Time of customer arrival
    int processingTime;     // Time required for processing customer
    bool queued;            // Whether or not the customer has been assigned to a checkout line
    Customer *next;         // The following customer in the queue
};

/* **************************************** QUEUE CLASS ******************************************* */
class CheckoutQueue {
public:
    CheckoutQueue();                    // CheckoutQueue constructor
    Customer *First();                  // Returns the first node in the queue
    Customer *Last();                   // Returns the last node in the queue
    int GetCount();                     // Returns the number of elements in queue
    int GetWait();                      // Returns the wait time for the queue
    bool IsEmpty();                     // Returns true if the queue is empty; false otherwise
    bool IsFull();                      // Returns true if there is no memory left; false otherwise
    void IncWait(int);                  // Increases the wait time by a given value
    void DecWait(int);                  // Decreases the wait time by a given value
    void Add(Customer, int &);          // Adds a customer node the end of the queue
    void Remove(Customer &, int &);     // Removes a customer node from the front of the queue
private:
    int count;                          // The number of nodes in the queue
    int wait;                           // The wait time for the queue
    Customer *front;                    // Points to the frontmost node in the queue
    Customer *rear;                     // Points to the last node in the queue
};
// --------------------------------------------------------------------------------------------------
// CustomerList constructor
CheckoutQueue::CheckoutQueue() {
        // Initialize checkout queue variables
    count = 0;
    wait = 0;
    front = NULL;
    rear = NULL;
}
// --------------------------------------------------------------------------------------------------
// First - Returns the address of the first element in queue
Customer *CheckoutQueue::First() {
        // Return NULL if the queue is empty
    if ( IsEmpty() ) {
            // Print error message to console
        cout << "Error: Attempted to access element from an empty queue." << endl;
        return NULL;
    }
        // Otherwise, return the first element
    return front;
}
// --------------------------------------------------------------------------------------------------
// Last - Returns the address of the last element in queue
Customer *CheckoutQueue::Last() {
        // Return NULL if the queue is empty
    if ( IsEmpty() )
    {
            // Print error message to console
        cout << "Error: Attempted to access element from an empty queue." << endl;
        return NULL;
    }
        // Otherwise, return the last element
    return rear;
}
// --------------------------------------------------------------------------------------------------
// GetCount - Returns the number of nodes in the queue
int CheckoutQueue::GetCount() {
    return count;
}
// --------------------------------------------------------------------------------------------------
// GetWait - Returns the wait time for the queue
int CheckoutQueue::GetWait() {
    return wait;
}
// --------------------------------------------------------------------------------------------------
// IsEmpty
bool CheckoutQueue::IsEmpty() {
    
    // Returns true if the queue is empty; returns false otherwise
    
    return ( count == 0 );
}
// --------------------------------------------------------------------------------------------------
// IsFull
bool CheckoutQueue::IsFull() {
    
    // Returns true if there is no more memory to expand a queue; return false otherwise
    
        // Create a test node
    Customer  *test;
    test = new (Customer);
        // If a node cannot be created, the queue is full
    if (test == NULL) {
        delete test;
        cout << "Error: Out of Memory." << endl;
        return true;
    }
        // Otherwise the queue is not full
    return false;
}
// --------------------------------------------------------------------------------------------------
// IncWait

void CheckoutQueue::IncWait(int increment) {
    
    // Receives - A value to increment the wait time by
    // Task - Increase the queue wait time by the given value
    // Returns - The wait time is updated
    
    wait = (wait + increment);
}
// --------------------------------------------------------------------------------------------------
// DecWait
void CheckoutQueue::DecWait(int decrement) {
    
    // Receives - A value to decrement the wait time by
    // Task - Decrease the queue wait time by the given value
    // Returns - The wait time is updated
    
    wait = (wait - decrement);
}
// --------------------------------------------------------------------------------------------------
// Add
void CheckoutQueue::Add(Customer NewData, int &stat) {
    
    // Receives - Customer data and a status indicator
    // Task - Add the customer data to the checkout queue
    // Returns - The checkout queue is updated accordingly, and the status is changed
    //           if necessary
    
    stat = 1;                       // Initialize status
    Customer *NewCustomer;          // Reserve space for new customer node
    NewCustomer = new (Customer);
        // Make sure the queue is not full
    if ( IsFull () ) {
            // Indicate an error if so
        stat = 0;
        return;
    }
        // Copy customer data to the new node
    NewCustomer->arrivalTime = NewData.arrivalTime;
    NewCustomer->processingTime = NewData.processingTime;
    strcpy(NewCustomer->name, NewData.name);
    NewCustomer -> next = NULL;
        // If the queue is empty, append new node to the front
    if (front == NULL) {
        front = NewCustomer;
    }
        // Otherwise, append new node to the rear of the queue
    else {
        rear->next = NewCustomer;
    }
        // Append new customer node
    rear = NewCustomer;
        // Increment number of nodes in the queue
    count++;
    return;
}
// --------------------------------------------------------------------------------------------------
// Remove
void CheckoutQueue::Remove(Customer &ToDelete, int &stat) {
    
    // Receives - Reference to customer node and a status indicator
    // Task - Remove the front element from the list
    // Returns - The removed node is saved in the node reference parameter, and the
    //              status indicator is updated if necessary
    
    stat = 1;           // Initialize status indicator
    Customer *temp;     // Reserve space for temporary node
        // Make sure the queue is not empty
    if ( IsEmpty() ) {
            // If so, indicate an error
        stat = 0;
        return;
    }
        // Save removed node data for passing back
    ToDelete.arrivalTime = front->arrivalTime;
    ToDelete.processingTime = front->processingTime;
    strcpy(ToDelete.name, front->name);
        // Remove first node from the queue
    temp = front;
    front = front->next;
    delete temp;
        // Update the number of nodes in the queue
    count--;
    return;
}
/* ************************************** END QUEUE CLASS ***************************************** */

//******************************************* MAIN ***************************************************

    // Main function prototypes
void Header(ofstream &);
void Footer(ofstream &);
void LoadCustomers(ifstream &, CheckoutQueue &, int);
void PrintHeaders(ofstream &);
void PrintResults(ofstream &, CheckoutQueue, CheckoutQueue);

int main() {
    
    int stat = 1;                                       // Program status indicator
    int currentTime = 0;                                // Store time keeper
    
    Customer currentCustomer;                           // Currently processed customer
    Customer departingCustomer;                         // Customer that is leaving
    
    CheckoutQueue arrivalList;                          // List of customers arrived
    CheckoutQueue departureList;                        // List of customers departed
    CheckoutQueue line1;                                // Checkout line #1
    CheckoutQueue line2;                                // Checkout line #2
    CheckoutQueue line3;                                // Checkout line #3
    CheckoutQueue checkout[3] = {line1, line2, line3};  // Array of checkout lines 1-3
    int shortest;                                       // Index of shortest checkout line
    
        // Get input file
    ifstream inputFile("queue_in.txt", ios::in);
        // Get output file
    ofstream outputFile("output.txt", ios::out);
    
        // Load customer data from the input file to the arrival list
    LoadCustomers(inputFile, arrivalList, stat);
    
        // Initialize clock
    currentTime = 0;
    
        // Process customers until all have left
    while ( departureList.GetCount() < arrivalList.GetCount() ) {
        
            // QUEUE CUSTOMERS
            // If a customer is currently arriving
        if ( arrivalList.First()->arrivalTime == currentTime ) {
                // Remove customer from the arrival queue, receiving customer data
            arrivalList.Remove(currentCustomer, stat);
            
                // Find shortest wait of all 3 checkout lines (or an empty queue)
            shortest = 0;                   // Initialize shortest line index so far
            for (int i=0; i < 3 ; i++) {    // Compare all checkout line wait times
                if (checkout[i].GetWait() < checkout[shortest].GetWait()) {
                    shortest = i;
                }
                // Note: If current checkout wait time is EQUAL to the shortest
                //       wait time, it will still be disregarded because lines with
                //       lower numerical value have higher priority
            } // The shortest wait has been found
            
                // Mark customer as queued
            currentCustomer.queued = true;
                // Add customer to the shortest checkout line queue
            checkout[shortest].Add(currentCustomer, stat);
                // Increment wait time of the checkout line accordingly
            checkout[shortest].IncWait(currentCustomer.processingTime);
                // Append customer to the end of the arrival list
            arrivalList.Add(currentCustomer, stat);
        } // FINISHED QUEUEING CUSTOMER
        
            // UPDATE CHECKOUT LINES FROM LINE 1 TO LINE 3
        for (int i=0; i < 3 ; i++) {
                // REMOVE FINISHED CUSTOMERS
                // If the line is not empty, check for finished customers
            if (!checkout[i].IsEmpty()) {
                    // If a customer is done being processed, remove from line
                if ( checkout[i].First()->processingTime <= 0 ) {
                        // Remove customer from checkout line
                    checkout[i].Remove(departingCustomer, stat);
                        // Place customer in the list of departed customers
                    departureList.Add(departingCustomer, stat);
                    
                    //if ( strcmp( departingCustomer.name , "Freddie Freeloader       ") == 0 )
                        //cout << "Miles Davis has left the building." << endl;
                }
            }
                // PROCESS CUSTOMER AT THE FRONT
                // If the line is still not empty, process customer at the head
            if (!checkout[i].IsEmpty()) {
                    // Update processing time of front customer
                checkout[i].First()->processingTime--;
                    // Update total processing time for checkout line
                checkout[i].DecWait(1);
            }
        } // FINISHED UPDATING CHECKOUT LINES
        
            // Update clock
        currentTime++;
        
    }
        // Finished processing all customers
    
        // PRINT RESULTS
        // Print output heading
    Header(outputFile);
        // Print output table headers
    PrintHeaders(outputFile);
        // Print results
    PrintResults(outputFile, arrivalList, departureList);
        // Print output closing
    Footer(outputFile);
    
        // Close data files
    inputFile.close();
    outputFile.close();
    
        // Return to operating system
    return 0;
}

//****************************************************************************************************

//****************************************************************************************************
void LoadCustomers(ifstream &inputFile, CheckoutQueue &queue, int stat) {
    
    // Receives - Reference to an input file, reference to a queue, and a status indicator
    // Task - Load data from the input file into the queue
    // Returns - The queue is updated accordingly, and the status is updated if necessary
    
    int atime, ptime;       // Arrival time and processing time
    char customerName[26];  // Full name of a customer
    Customer newCustomer;   // A new customer node
    
        // Process input file until the end is reached
    for ( inputFile >> atime ; atime != -99; inputFile >> atime ) {
            // Read in customer data
        inputFile >> ws;
        inputFile.get(customerName, 26);
        inputFile >> ptime;
            // Save data to a new customer node
        strcpy(newCustomer.name, customerName);
        newCustomer.arrivalTime = atime;
        newCustomer.processingTime = ptime;
        newCustomer.queued = false;
            // Add customer node to the arrival queue
        queue.Add(newCustomer, stat);
    }
}
//****************************************************************************************************

//****************************************************************************************************
void PrintHeaders(ofstream &outfile) {
    
    // Receives - Reference to the output file
    // Task - Print table headers to the output file
    // Returns - The output file is updated accordingly

    outfile << "The order of customer arrival is:      The order of customer departure is:";
    outfile << endl;
    outfile << "---------------------------------      -----------------------------------";
    outfile << endl;
}
//****************************************************************************************************

//****************************************************************************************************
void PrintResults(ofstream &outfile, CheckoutQueue arrivals, CheckoutQueue departures) {
    
    // Receives - Reference to the output file, a queue of arrivals and a queue of departures
    // Task - Print the arrival queue and departure queue as side-by-side lists
    // Returns - The data is printed to the output file
    
    int stat = 1;       // Initialize the status indicator
    int numRecords;     // Number of records to be printed from each list
    Customer customer;  // A customer node
    
        // Initialize number of records to print, based on the length of the first list
    numRecords = arrivals.GetCount();
    
        // Print both lists side-by-side
    for ( int i = 0 ; i < numRecords ; i++ ) {
            // Get and print a value from list 1
        arrivals.Remove(customer, stat);
        outfile << customer.name;
            // Print spacing and divider
        outfile << "         |    ";
            // Get and print a value from list 2
        departures.Remove(customer, stat);
        outfile << customer.name;
            // Drop down a line
        outfile << endl;
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
    Outfile << setw(30) << "Katelyn Schaffer";
    Outfile << setw(17) << "CSC 36000";
    Outfile << setw(15) << "Section 11" << endl;
    Outfile << setw(30) << "Spring 2015";
    Outfile << setw(20) << "Assignment #3" <<endl;
    Outfile << setw(35) << "-----------------------------------";
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