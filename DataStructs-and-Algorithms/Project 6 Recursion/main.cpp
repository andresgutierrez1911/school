//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 6             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: April 13, 2015     *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:           This program takes a set of integer values as specified in the #define       *
//*                     statements and calculates their factorial values. Results and stats are      *
//*                     printed to the output file.                                                  *
//*                                                                                                  *
//*  USER DEFINED                                                                                    *
//*    MODULES     :    IterativeFactorial..................Calculates the factorial value of a      *
//*                                                         given integer (iterative method)         *
//*                     RecursiveFactorial..................Calculates the factorial value of a      *
//*                                                         given integer (recursive method)         *
//*                     PrintStats..........................Prints the current counter stats in      *
//*                                                         factorial process                        *
//*                     Header..............................Prints the output header                 *
//*                     Footer..............................Prints the output footer                 *
//*                                                                                                  *
//****************************************************************************************************

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PAGEWIDTH 95;           // Set constant page width
#define PAGEHEIGHT 76;          // Set constant page height

#define FIRST 0;                // Set input starting point
#define LAST  9;                // Set input ending point

static int linesUsed;           // Line counter for output
static int functionCalls;       // Function call counter
static int assignmentOpps;      // Assignment operation counter
static int multiplicationOpps;  // Multiplication operation counter

using namespace std;

//******************************************* MAIN ***************************************************

    // Main function prototypes
int IterativeFactorial(int);
int RecursiveFactorial(int);
void PrintStats(ofstream &);
void PageBreak(ofstream &, int);
void Header(ofstream &);
void Footer(ofstream &);

int main() {
    
        // Get output file
    ofstream outputFile("output.txt", ios::out);
    
        // Initialize line counter and max lines
    linesUsed = 0;
    int maxlines = PAGEHEIGHT;
    
        // Initialize call and operation counters
    functionCalls = 0;
    assignmentOpps = 0;
    multiplicationOpps = 0;
    
        // Set starting and ending point for integer input
    int start = FIRST;
    int end = LAST;
    
        // Print output heading
    Header(outputFile);
    linesUsed+=4;   // Increment line count accordingly
    
        // Process integers from start to end
    for (int i = start; i <= end ; i++) {
        
            // Check for end of page
        if ( (maxlines-linesUsed) < 16 ) {
                // Insert page break
            PageBreak(outputFile, linesUsed);
                // Reset line count
            linesUsed = 0;
        }
        
            // Reset call and operation counters
        functionCalls = 0;
        assignmentOpps = 0;
        multiplicationOpps = 0;
        
            // Print current number
        outputFile << "The number is: " << i << "\n\n";
        
            // Print factorial iterative method stats
        outputFile << "The statistics for the iterative method of finding factorial values";
        outputFile << "\n";
        outputFile << "\tThe value of " << i << "! is " << IterativeFactorial(i) << "\n";
            // Print counter stats
        PrintStats(outputFile);
        outputFile << endl;
        
            // Reset call and operation counters
        functionCalls = 0;
        assignmentOpps = 0;
        multiplicationOpps = 0;

            // Print factorial recursive method stats
        outputFile << "The statistics for the recursive method of finding factorial values";
        outputFile << "\n";
        outputFile << "\tThe value of " << i << "! is " << RecursiveFactorial(i) << "\n";
            // Print counter stats
        PrintStats(outputFile);
        outputFile << endl;
        
        outputFile << "************************************************************" << "\n\n";
        
            // Increment line count accordingly
        linesUsed+=16;
    }

        // Check for end of page
    if ( (maxlines-linesUsed) < 4 ) {
            // Insert page break
        PageBreak(outputFile, linesUsed);
            // Reset line count
        linesUsed = 0;
    }
    
        // Print output footer
    Footer(outputFile);
    
        // Return to operating system
    return 0;
}
//******************************************** END MAIN **********************************************

//****************************************************************************************************
int IterativeFactorial(int x) {
    
    // Receives - An integer value of which to calculate the factorial value
    // Task - Calculates the factorial value of the given integer (interative method)
    // Returns - The factorial value of the given integer
    
        // Record function call
    functionCalls++;
        // Set factorial to default 1
    int factorial = 1;
        // Increment assignment counter
    assignmentOpps++;
    
        // If necessary, continue calculating factorial until finished
    while ( x > 0 ) {
            // Perform step in calculating factorial
        factorial = factorial * x;
        multiplicationOpps++;   // Increment multiplication counter
        assignmentOpps++;       // Increment assignment counter
        x--;                    // Decrement number for next step in process
    }
    
        // When finished, return the result
    return factorial;
}
//****************************************************************************************************

//****************************************************************************************************
int RecursiveFactorial(int x) {
    
    // Receives - An integer value of which to calculate the factorial value
    // Task - Calculates the factorial value of the given integer (recursive method)
    // Returns - The factorial value of the given integer
    
        // Record function call
    functionCalls++;
        // If the number is greater than 0, continue calculating factorial
    if ( x > 0 ) {
            // Increment counter for multiplication (which is about to occur)
        multiplicationOpps++;
            // Further calculate factorial value
        return x * RecursiveFactorial(x-1);
    }
        // Otherwise, return 1
    else {
        return 1;
    }
}
//****************************************************************************************************

//****************************************************************************************************
void PrintStats(ofstream &outfile) {

    // Receives - A reference to the output file; stat variables exist globally
    // Task - Prints the stat/counter values in the factorial process
    // Returns - The output file is updated accordingly
    
    outfile << "\tThe number of function calls is " << functionCalls << "\n";
    outfile << "\tThe number of assignment operations is " << assignmentOpps << "\n";
    outfile << "\tThe number of multiplication operations is "<< multiplicationOpps<<"\n";
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
    Outfile << setw(20) << "Assignment #6" <<endl;
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