//************************************* PROGRAM IDENTIFICATION ***************************************
//*                                                                                                  *
//*     PROGRAM FILE NAME: main.cpp               ASSIGNMENT #: 4             GRADE: _____           *
//*                                                                                                  *
//*     PROGRAM AUTHOR:     ______________________________                                           *
//*                                Katelyn Schaffer                                                  *
//*                                                                                                  *
//*     COURSE #: CSC 36000 11                                          DUE DATE: March 23, 2015     *
//*                                                                                                  *
//****************************************************************************************************

//*************************************** PROGRAM DESCRIPTION ****************************************
//*                                                                                                  *
//*  PROCESS:         This program is designed to read in passenger information and assign           *
//*                   passengers to their seats according to their requests and the seating rules.   *
//*  USER DEFINED                                                                                    *
//*    MODULES     :   void Header..........................Prints the output heading                *
//*                    void Footer..........................Prints the output footer                 *
//*                    void PageBreak.......................Inserts a page break                     *
//*                    void ConversionHeader................Prints conversion display headers        *
//*                    void EvaluationHeader................Prints evaluation display headers        *
//*                    void AnswerLabel.....................Prints answer label to output            *
//*                    void printProcess....................Prints step in conversion process or     *
//*                                                         evaluation process (overloaded)          *
//*                    void clearArray......................Removes all data from an array           *
//*                    bool isOperator......................Returns true if a given character is     *
//*                                                         an operator                              *
//*                    bool isParen.........................Returns true if a given character is     *
//*                                                         a parenthesis                            *
//*                    int getPriority......................Returns operator priority of a character *
//*                    int charEvaluate.....................Evaluates an expression with two given   *
//*                                                         integers and a character representing    *
//*                                                         an operator                              *
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
// NODE STRUCT
struct Node {
    char value;             // Character value holder
    int intVal;             // Integer value holder
    Node *next;             // The following node in the stack
};
/* *************************************** STACK CLASS ******************************************** */
class Stack {
public:
    Stack() { top = NULL; } // Stack constructor
    void Push(Node);        // Places a node on top of the stack
    void Pop(Node & );      // Retrieves and removes the top node from the stack
    Node *GetTop(int &);    // Returns reference to top-most node on the stack
    bool IsEmpty();         // Returns true if the stack is empty; returns false otherwise
    bool IsFull();          // Returns true if the stack is full (out of memory); false otherwise
private:
    Node *top;              // The top-most node on the stack
};
// --------------------------------------------------------------------------------------------------
// IsEmpty - Returns true if the stack is empty; returns false otherwise
bool Stack::IsEmpty() {
    return ( top == NULL );
}
// --------------------------------------------------------------------------------------------------
// IsFull - Returns true if the stack is full (out of memory); returns false otherwise
bool Stack::IsFull() {
        // Create a test node to test for memory
    Node  *p;
    p = new Node;
        // If out of memory, return true
    if (p == NULL) {
        delete p;
        cout << "Out of memory. " << endl;
        return true;
    }
    return false;
}
// --------------------------------------------------------------------------------------------------
// GetTop - Returns reference to top node
Node *Stack::GetTop(int &stat) {
        // If the stack is empty, return null and print error to console
    if ( IsEmpty() ) {
        cout << "Stack is empty. " << endl;
            // Update status indicator - error
        stat = -1;
        return NULL;
    }
        // Otherwise return top node
    else {
        return top;
    }
}
// --------------------------------------------------------------------------------------------------
// Push - Places new node on top of stack
void Stack::Push(Node newData) {
    
    // Receives - A node to add to the stack
    // Task - Places new node on top of the stack
    // Returns - The wait time is updated
    
        // If the stack is full, print error message to console
    if ( IsFull () ) {
        cout << " Add operation failed! " << endl;
        return;
    }
        // Create new node
    Node *p;
        // Reserve space for new node
    p = new Node;

        // Assign new data to new node
    p -> value = newData.value;
    p -> intVal = newData.intVal;
    p -> next = top;
    
        // Place the new node on top of the stack
    top = p;
    
    return;
}
// --------------------------------------------------------------------------------------------------
// Pop - Retrieves and removes the top node from the stack
void Stack::Pop(Node &dataOut) {
    
    // Receives - A reference to the data node to save to
    // Task - Saves top node data to a node, and removes top node from stack
    // Returns - The top node data is returned to the save node, and the stack
    //              is updated accordingly
    
        // Free space for new node
    Node *p ;
        // Make sure the stack is not empty
    if ( IsEmpty() ) {
            // If so, abort mission, print error and return
        cout << " Stack is empty. " << endl;
        cout << " Delete Operation Failed. " << endl;
        delete p;
        return;
    }
        // Save top node data for retrieval
    dataOut.value = top->value;
    dataOut.intVal = top->intVal;
        // Adjust top of stack
    p = top;
    top = top -> next;
        // Delete top node from the stack
    //delete p;
    return;
}
//****************************************************************************************************

//******************************************* MAIN ***************************************************

    // Main function prototypes
void Header(ofstream &);
void Footer(ofstream &);
void PageBreak(ofstream &, int);
void ConversionHeader(ofstream &);
void EvaluationHeader(ofstream &);
void AnswerLabel(ofstream &);
void printProcess(ofstream &, char[], int, Stack);
void printProcess(ofstream &, char[], int, char[], int, Stack);
void clearArray(char[], int);
bool isOperator(char);
bool isParen(char);
int getPriority(char);
int charEvaluate(int, char, int);

int main() {
    
    bool quit;              // Whether or not the program is finished running
    bool done;              // Whether or not the POP process is finished

    char infix[31];         // The infix expression
    char postfix[31];       // The postfix expression
    char expression[31];    // The original expression
    
    Stack opStack;          // The stack used for conversion
    Stack evalStack;        // The stack used for evaluation
    Node newNode;           // A new data node
    Node opNode;            // A data node
    
    int pfLength;           // Current length of the postfix expression
    int pfcount;            // Total count of postfix expression
    int ifLength;           // Current length of infix expression
    int ifcount;            // Total count of infix expression
    
    int num1;               // A number placeholder 1
    int num2;               // A number placeholder 2
    
        // Get input file
    ifstream inputFile("stack_in.txt", ios::in);
        // Get output file
    ofstream outputFile("output.txt", ios::out);
        // Initialize line count
    linesUsed = 0;
    
        // Print the output header
    Header(outputFile);
    linesUsed+=4;   // Update line count accordingly
    
        // Initialize quit flag to false
    quit = false;
        // Initialize data character
    char c = ' ';
    
        // Run program until all data is processed
    while (!quit) {
        
            // Initialize expression arrays and counters
        ifLength = 0;
        pfLength = 0;
        ifcount  = 0;
        pfcount  = 0;
        clearArray(infix, 31);
        clearArray(postfix, 31);
        clearArray(expression, 31);
        
            // Get first expression from file, reading character by character
        for ( c = inputFile.get(); c != '\n' && c != '\r' ; c = inputFile.get() ) {
                // Stop reading and quit program if the sentinel is reached
            if (c == 'X') { quit = true; break; }
                // Append character to infix notation
            infix[ifLength] = c;
            ifLength++;     // Update counters accordingly
            ifcount++;
        }
            // Finish reading expression
        inputFile >> ws;
        
            // If the quit signal has not been given yet, process expression
        if (!quit) {
            
                // Print conversion display headers
            ConversionHeader(outputFile);
            linesUsed+=3;   // Update line count accordingly
                // Print conversion status so far
            printProcess(outputFile, infix, ifLength, postfix, pfLength, opStack);
            linesUsed++;    // Update line count accordingly
            
                // Save backup copy of infix expression
            for (int i=0; i < ifLength; i++) {
                expression[i] = infix[i];
            }
            
                // Convert INFIX to POSTFIX
                // Process the infix expression character by character
            for (int i = 0; i < ifLength; i++) {
                
                    // Get character from infix expression
                c = infix[i];
                    // Erase character from infix expression (for printing)
                infix[i] = ' ';
                ifcount--;  // Update counter
                
                    // If the character is a digit
                if ( isdigit(c) ) {
                        // Append digit to postfix expression
                    postfix[pfLength] = c;
                    pfLength++; // Update counters accordingly
                    pfcount++;
                }
                    // Otherwise, process character as necessary...
                else {
                        // Save character to new node
                    newNode.value = c;
                    
                        // If the character is a left parenthesis
                    if ( c == '(' ) {
                            // Push new node to the stack
                        opStack.Push(newNode);
                    }
                        // If the character is a right parenthesis...
                    else if ( c == ')' ) {
                            // Pop operators from stack until left parenthesis is found
                        do {
                                // Get next operator from stack
                            opStack.Pop(opNode);
                                // If it's not a left parenthesis, it to postfix epression
                            if (opNode.value != '(') {
                                postfix[pfLength] = opNode.value;
                                pfLength++; // Update counters accordingly
                                pfcount++;
                            }
                            // Reapeat if left parenthesis isn't found
                        } while (opNode.value != '(');
                    }   // Finished handling right parenthsis
                    
                        // If the character is an operator...
                    else if ( isOperator(c) ) {
                            // If the stack is empty, push operator to the stack
                        if (opStack.IsEmpty()) {
                            opStack.Push(newNode);
                        }
                            // Otherwise deal with the operator according
                            //  to the others in the stack
                        else {
                                // Initialize "done" flag. We'll be done when the new
                                //  operator is pushed
                            done = false;
                                // Pop operators from the operator stack until we find the
                                // proper place to insert the new operator
                            do {
                                    // Grab the top-most node from the operator stack
                                opStack.Pop(opNode);
                                
                                    // If a left parenthesis is encountered...
                                if ( isParen(opNode.value) ) {
                                        // Push former stack item back to the stack
                                    opStack.Push(opNode);
                                        // Push new node onto the stack
                                    opStack.Push(newNode);
                                        // We're done here
                                    done = true;
                                }
                                    // If the priority of the item from the stack is LESS THAN
                                    // the priority of the new operator...
                                else if ( getPriority(opNode.value) < getPriority(c) ) {
                                        // Push former stack item back to the stack
                                    opStack.Push(opNode);
                                        // Push new node onto the stack
                                    opStack.Push(newNode);
                                        // We're done here
                                    done = true;
                                }
                                    // If the stack is empty (and we're not done yet)
                                if ( opStack.IsEmpty() && !done ) {
                                        // Push the popped node operator onto the postfix expression
                                    postfix[pfLength] = opNode.value;
                                    pfLength++; // Update counters accordingly
                                    pfcount++;
                                        // Push new node (from the INFIX expression) onto the stack
                                    opStack.Push(newNode);
                                        // Now we're done
                                    done = true;
                                }
                                    // If the priority of the item from the stack is GREATER THAN or
                                    // EQUAL to that of the newest operator (and we're not done yet)
                                if ( (getPriority(opNode.value) >= getPriority(newNode.value)
                                        && !done ) )
                                {
                                        // Place the popped operator onto the postfix expression
                                    postfix[pfLength] = opNode.value;
                                    pfLength++; // Update counters accordingly
                                    pfcount++;
                                }
                                // Repeat until done
                            } while (!done);
                        }
                    } // Finished dealing with operator
                } // Finished dealing with non-digit
                
                    // Print progress so far - Infix, Postfix, and Stack
                printProcess(outputFile, infix, ifcount, postfix, pfcount, opStack);
                linesUsed++;    // Increment line count accordingly
                
            }   // Finished processing infix
                // Now pop operators from the stack to the postfix until the stack is empty
            while (!opStack.IsEmpty()) {
                opStack.Pop(opNode);
                    // Push the popped node operator onto the postfix expression
                postfix[pfLength] = opNode.value;
                pfLength++; // Update counters accordingly
                pfcount++;
                    // Print progress so far - Infix, Postfix, and Stack
                printProcess(outputFile, infix, ifcount, postfix, pfcount, opStack);
                linesUsed++;    // Update line count accordingly
            }
            // FINISHED CONVERTING
            
            // NOW EVALUATE
            
                // Print evaluation display headers
            EvaluationHeader(outputFile);
            linesUsed+=4;   // Update line count accordingly
                // Print evaluation progress so far
            printProcess(outputFile, postfix, pfcount, evalStack);
            linesUsed++;    // Update line count accordingly
            
                // Process postfix expression character by character
            for (int i=0; i < pfLength; i++) {
                    // Get character from postfix expression
                c = postfix[i];
                    // Erase character from postfix expression (for printing)
                postfix[i] = ' ';
                pfcount--;  // Update counter
                
                    // If the character is an operand
                if ( isdigit(c) ) {
                        // Convert to integer
                    num1 = c - '0';
                        // Save operand to node as an ineger and push it to the stack
                    opNode.intVal = num1;
                    evalStack.Push(opNode);
                }
                    // If the character is an operator
                else if ( isOperator(c) ) {
                        // Pop the stack to get number 1
                    evalStack.Pop(opNode);
                    num1 = opNode.intVal;
                        // Pop the stack again for number 2
                    evalStack.Pop(opNode);
                    num2 = opNode.intVal;
                        // Evaluate two number with operator, and push to stack
                    opNode.intVal = charEvaluate(num2, c, num1);;
                    evalStack.Push(opNode);
                }
                    // Print progress so far
                printProcess(outputFile, postfix, pfcount, evalStack);
                linesUsed++;    // Update line count accordingly
            }
                // Print answer label
            AnswerLabel(outputFile);
            linesUsed++;    // Update line count accordinly
            
                // Pop the answer from the stack and print it with the original expression
            evalStack.Pop(opNode);
            outputFile << expression << " = ";
            outputFile << opNode.intVal << endl;
            linesUsed++;    // Update line count accordingly
            
                // Insert page break
            PageBreak(outputFile, linesUsed);
            linesUsed = 0;  // Reset line count
            
        }   // Finished processing expression
    }   // Loop until quit
    
        // Print output footer
    Footer(outputFile);
}
//****************************************************************************************************

//****************************************************************************************************
bool isOperator(char c) {
    
    // Receives - A character
    // Task - Determines if the given character is an operator
    // Returns - Returns true if the character is a valid operator; returns false otherwise
    
    return ( c == '+' || c == '-' || c == '*' || c == '/' );
}
//****************************************************************************************************

//****************************************************************************************************
bool isParen(char c) {
    
    // Receives - A character
    // Task - Determines if the given character is a parenthesis
    // Returns - Returns true if the character is a parenthesis; returns false otherwise
    
    return ( c == '(' || c == ')' );
}
//****************************************************************************************************

//****************************************************************************************************
int getPriority(char c) {
    
    // Receives - A character representing an operator
    // Task - Determines the priority of the operator character with comparison to other operators
    // Returns - The priority value of the operator
    
    switch (c) {
        case '-':       // Return a priority of 1 for - and +
        case '+':
            return 1;
            break;
        case '/':       // Return a priority of 2 for / and *
        case '*':
            return 2;
            break;
        default:        // In any other case, make note of error
            cout << "\nError: getPriority(): Invalid data: " << c << "\n";
            break;
    }
        // Return a negative value in case of error
    return -1;
}
//****************************************************************************************************

//****************************************************************************************************
void clearArray(char array[], int size) {
    
    // Receives - The array to be cleared and the size of the array
    // Task - Deletes all values from the array
    // Returns - The array is updated accordingly
    
        // For every index in the array, erase the data
    for (int i=0; i < size; i++) {
        array[i] = '\0';
    }
    
    return;
}
//****************************************************************************************************

//****************************************************************************************************
int charEvaluate(int x, char op, int y) {
    
    // Receives - A first number x, a character representing an operator, and a second number y
    // Task - Evaluates the expression in the format x [op] y, where [op] is the given
    //          operator used accordingly
    // Returns - The resulting value of the expression
    
        // Determine how to evaluate expression based on operator character
    switch (op) {
        case '-':
            return x-y;
            break;
        case '+':
            return x+y;
            break;
        case '/':
            return x/y;
            break;
        case '*':
            return x*y;
            break;
        default:    // Report an error if the operator character is not valid
            cout << "Error: charEvaluate: Invalid operator";
            break;
    }
        // Return -999 if there is an error
    return -999;
}
//****************************************************************************************************

//****************************************************************************************************
void ConversionHeader(ofstream &outfile) {
    
    // Receives - A reference to the output file
    // Task - Prints the conversion display headers
    // Returns - The output file is updated accordingly
    
    outfile << "                                        CONVERSION DISPLAY\n\n";
    outfile << "         Infix Expression";
    outfile << "          POSTFIX Expression";
    outfile << "        Stack Contents (Top to Bottom)";
    outfile << endl;
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void EvaluationHeader(ofstream &outfile) {
    
    // Receives - A reference to the output file
    // Task - Prints the evaluation display headers
    // Returns - The output file is updated accordingly
    
    outfile << "\n                                        EVALUATION DISPLAY\n\n";
    outfile << "                 POSTFIX Expression";
    outfile << "                          Stack Contents (Top to Bottom)";
    outfile << endl;
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void AnswerLabel(ofstream &outfile) {
    
    // Receives - A reference to the output file
    // Task - Prints the answer label
    // Returns - The output file is updated accordingly
    
    outfile << "\n    ORIGINAL EXPRESSION AND THE ANSWER:   ";
    return;
}
//****************************************************************************************************

//****************************************************************************************************
void printProcess(ofstream &outfile, char post[], int postLength, Stack evaluation) {
    
    // Receives - A reference to the output file, the postfix array, the length of the array,
    //              and the evaluation stack
    // Task - Prints the current step in the evaluation process
    // Returns - The output file is updated accordingly
    
        // Temporary node
    Node tempNode;
        // Print "Empty' if the expression is currently empty
    if (postLength == 0)    { outfile << right << setw(35) << "Empty"; }
        // Print the expression otherwise
    else                    { outfile << right << setw(35) << post; }
    
    outfile << "                          ";
        // Print "Empty" if the stack is currently empty
    if (evaluation.IsEmpty()) { outfile << "Empty"; }
        // Print all the stack values otherwise
    else {
        while (!evaluation.IsEmpty()) {
            evaluation.Pop(tempNode);
            outfile << tempNode.intVal << " ";
        }
    }
    outfile << endl;
    return;
}
// --------------------------------------------------------------------------------------------------
void printProcess(ofstream &outfile, char in[], int inLength,
                  char post[], int postLength, Stack stack) {
    
    // Receives - A reference to the output file, the infix array, the length of the infix,
    //              expression, the postfix array, the length of the postfix express, and the
    //              conversion stack
    // Task - Prints the current step in the converstion process
    // Returns - The output file is updated accordingly
    
        // Temporary node
    Node tempNode;
        // Print "Empty' if the expression is currently empty
    if (inLength == 0)      {outfile << right << setw(25) << "Empty";}
        // Otherwise, print the expression
    else                    {outfile << right << setw(25) << in;}

    outfile << "          ";
        // Print "Empty' if the expression is currently empty
    if (postLength == 0)    {outfile << left  << setw(25) << "Empty";}
        // Otherwise print the expression
    else                    {outfile << left  << setw(25) << post;}

    outfile << " ";
        // Print "Empty" if the stack is currently empty
    if (stack.IsEmpty())    { outfile << "Empty"; }
        // Otherwise, print all current stack values
    else {
        while (!stack.IsEmpty()) {
            stack.Pop(tempNode);
            outfile << tempNode.value << " ";
        }
    }
    outfile << endl;
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
    Outfile << setw(20) << "Assignment #4" <<endl;
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