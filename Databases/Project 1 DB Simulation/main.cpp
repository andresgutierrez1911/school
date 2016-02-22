#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <string.h>

using namespace std;

void AddElem(char);
void AddFlight();
void List(char);
string GetElemName(char, char[]);
bool Duplicate(char, char[]);
void FindFlight(char[], char[], int);
void FindConnection(char[], char[], int, char[], char[], int& );

int main()
{
    bool quit = false;  // Quit signal
    char input;         // User's input
    char code[255];     // ID code placeholder
    char code2[255];    // ID code placeholder 2
    int num;            // Simple number placeholder
    
    cout << "_____ FLIGHT MANAGER _____" << endl;
    
    // Run program until user quits
    while (!quit) {
        // Prompt for user input
        cout << ">>> ";
        // Receive input
        cin >> input;
        // Choose action
        switch (input) {
            // ----- ADD ------
            case 'a':
                // Receive secondary input
                cin >> input;
                // Choose category to add
                switch (input) {
                    // - ADD FLIGHT ELEMENT (CITY OR AIRLINE) -
                    case 'c':
                    case 'a':
                        AddElem(input);
                        break;
                    // - ADD FLIGHT -
                    case 'f':
                        AddFlight();
                        break;
                    // Error
                    default:
                        cout << "[Syntax error]" << endl;
                        break;
                }
                break;
            // ----- LIST -----
            case 'l':
                // Receive secondary input
                cin >> input;
                // Choose category to list
                switch (input) {
                    // - LIST CITIES -
                    case 'c':
                        
                        // Scan list and print results
                        List(input);
                        
                        break;
                    // - LIST AIRLINES -
                    case 'a':
                        
                        // Scan list and print results
                        List(input);
                        
                        break;
                    // - LIST FLIGHTS -
                    case 'f':
                        
                        // Scan list and print results
                        List(input);
                        
                        break;
                    // Error
                    default:
                        cout << "[Syntax error]" << endl;
                        break;
                }
                break;
            // ----- FIND -----
            case 'f':
                // Scan through separator
                cin.get();
                // Get departure code
                cin.get(code, 255, ' ');
                // Scan through separator
                cin.get();
                // Get destination code
                cin.get(code2, 255, ' ');
                // Get number of connections
                cin >> num;
                
                // Print flights according to query
                FindFlight(code, code2, num);
                
                break;
            // ----- QUIT -----
            case 'q':
                // Signal the program to quit
                quit = true;
                cout << "[Goodbye.]";
                break;
            // ERROR
            default:
                cout << "I do not understand \"" << input << "\".";
                cout << endl;
                break;
        }
        
    } // End program
    return 0;
}

/* **************************************************************** */

//************************* FUNCTION ADDELEM *************************
void AddElem(char type)
{
    // Function AddElem
    // Adds a new element to the data file, based on info entered
    
    // Data file
    ofstream file("data.txt", ios::app);
    
    char code[255];     // Max 3-character code
    char name[255];     // Element name
    
    // Scan through separator
    cin.get();
    // Get new city code
    cin.get(code, 255, ' ');
    // Scan through separator
    cin.get();
    cin.get(name, 255);
    
    // Make sure this is not a duplicate
    if (!Duplicate(type, code)) {
        // Print element data to file
        file << type << "|";
        file << code << "|";
        file << name << "|";
        file << endl;
    }
    // Error if duplicate
    else {
        cout << "[That code value is already taken.]";
        cout << endl;
    }
    
    // Close the file
    file.close();

    
    return;
}
//********************** END OF FUNCTION ADDELEM *********************

//************************ FUNCTION ADDFLIGHT ************************
void AddFlight()
{
    // Function AddFlight
    // Adds a flight to the data file, based on info entered
    
    // Data file
    ofstream file("data.txt", ios::app);
    
    char code[255]; // Flight code
    char dep[255];  // Departure city code
    char dest[255]; // Desination city code
    float price;    // Price value
    
    // Scan through separator
    cin.get();
    // Get airline code
    cin.get(code, 255, ' ');
    // Scan through separator
    cin.get();
    // Get departure city code
    cin.get(dep, 255, ' ');
    // Scan through separator
    cin.get();
    // Get destination city code
    cin.get(dest, 255, ' ');
    // Scan through separator
    cin.get();
    // Get flight price
    cin >> price;
    
    // Print flight data to file
    file << 'f' << "|";
    file << code << "|";
    file << dep << "|";
    file << dest << "|";
    file << price;
    file << endl;

    // Close the file
    file.close();
    
    return;
}
//********************* END OF FUNCTION ADDFLIGHT ********************

//************************ FUNCTION DUPLICATE ************************
bool Duplicate(char query, char code[])
{
    // Function Duplicate
    // Detetects if an element is already in the data file
    
    // Input file
    ifstream file("data.txt", ios::in);
    
    string in;      // string of data
    char c;         // single character of data
    char data[255]; // character array of data
    
    // Cycle through data file
    while (file.get(c)) {
        // If current line is an element of an equal type to the query
        if (c == query) {
            // Skip separator
            file.get();
            // Get element code
            file.get(data, 255, '|');
            // Check if this code is identical to the one in query
            if (strcmp(data, code) == 0) {
                // If so, flag duplicate
                return true;
            }
            else {
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        // Current line is not equal to query
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    } // End cycle through data file
    
    // Close the file
    file.close();
    
    // Return false if no duplicates are found
    return false;
}
//********************* END OF FUNCTION DUPLICATE ********************

//*************************** FUNCTION LIST **************************
void List(char query)
{
    // Function List
    // Lists elements from the data file according to the
    // category queried
    
    // Input file
    ifstream file("data.txt", ios::in);
    
    string in;      // string of data
    char c;         // single character of data
    char data[255]; // character array of data
    int counter = 0;// Result counter
    
    char dep[255];  // Departure city code
    char dest[255]; // Desination city code
    float price;    // Price value

    
    // Cycle through data file
    while (file.get(c)) {
        // If current line is an element of an equal type to the query
        if (c == query) {
            // Output the element information to console
            switch (c) {
                // Print flight element (city)
                case 'c':
                // Print flight element (airline)
                case 'a':
                    // Skip separator
                    file.get();
                    // Get element code
                    file.get(data, 255, '|');
                    // Print element code to console
                    cout << data;
                    // Skip separator
                    file.get();
                    // Get element name
                    file.get(data, 255, '|');
                    // Print element name to console
                    cout << " " << data;
                    // Skip separator
                    file.get();
                    // Skip whitespace
                    file >> ws;
                    // Increment result counter
                    counter++;
                    break;
                // Print flight
                case 'f':
                    // Skip separator
                    file.get();
                    // Read flight code
                    file.get(data, 255, '|');
                    // Skip separator
                    file.get();
                    // Read departure city code
                    file.get(dep, 255, '|');
                    // Skip separator
                    file.get();
                    // Read destination city cody
                    file.get(dest, 255, '|');
                    // Skip separator
                    file.get();
                    // Read price
                    file >> price;
                    // Skip whitespace
                    file >> ws;
                    
                    // Print fight info to console
                    cout << GetElemName('a', data) << ": ";
                    cout << GetElemName('c', dep) << " -> ";
                    cout << GetElemName('c', dest) << " ";
                    cout << "$" << price;
                    
                    // Increment result counter
                    counter++;

                    break;
                // Error
                default:
                    cout << "[Error: Invalid query.]" << endl;
                    break;
            } // End printing element
            
            // New line to console
            cout << endl;
        }
        // Current line is not equal to query
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    } // End cycle through data file
    // If no result were found, print message to console
    if (counter == 0) {
        cout << "[No results.]" << endl;
    }
    // Close the file
    file.close();
    
    return;
}
//************************ END OF FUNCTION LIST **********************

//*************&&********* FUNCTION GETELEMNAME **********************
string GetElemName(char type, char code[])
{
    // Function GetElemName
    // Returns the full name of an element in the data file, based
    // on a queried ID code
    
    // Input file
    ifstream file("data.txt", ios::in);
    
    char c;             // data character placeholder
    char data[255];     // data placeholder
    string in;          // data string placeholder
    string name = "";   // name placeholder
    
    // Cycle through data file
    while (file.get(c)) {
        // If current line is an element of an equal type to the query
        if (c == type) {
            // Skip separator
            file.get();
            // Get code
            file.get(data, 255, '|');
            // See if code matches the code in query
            if (strcmp(data, code) == 0) {
                // If the code matches, get the full name
                // Skip separator
                file.get();
                // Get the full name value
                getline(file, name, '|');
                // DONE HERE!
                // Close the file
                file.close();
                // Return name
                return name;
            }
            // If not, skip the line
            else {
                getline(file, in);
                file >> ws;
            }
        }
        // Current line is not equal to query
        else {
            // Skip line
            getline(file, in);
            file >> ws;
        }
    } // End cycle through data file

    // Close the data file
    file.close();
    
    // Return an error if no item is found
    return "[Error: Unknown airline.]";
}
//********************* END OF FUNCTION GETELEMNAME ******************

//********************** FUNCTION FINDFLIGHT *************************
void FindFlight(char queryDepart[], char queryDest[],
                int queryConnections)
{
    // Function FindFlight
    // Finds and lists flights based on departure and
    // destination queried
    
    // Input file
    ifstream file("data.txt", ios::in);
    
    char c;             // Character of data from file
    string in;          // String of data from file
    int counter = 0;    // Result counter
    
    char airline[255];  // Airline code
    char dep[255];      // Departure city code
    char dest[255];     // Desination city code
    float price;        // Price value
    
    // Cycle through data file line by line
    while (file.get(c)) {
        // If current line is a flight
        if (c == 'f') {
            // Get the flight airline code
            file.get();
            file.get(airline, 255, '|');
            file.get();
            // Get the flight departure city code
            file.get(dep, 255, '|');
            file.get(); // (Skip separator)
            // See if this flight departure matches the query
            if (strcmp(dep, queryDepart) == 0) {
                // Find flights according to number of connections
                switch (queryConnections) {
                    // 0 connections
                    case 0:
                        // See if the flight's destination
                        // matches the query
                    {
                        // Get the flight destination city code
                        file.get(dest, 255, '|');
                        file.get(); // (Skip separator)
                        // Compare the flight destination to the the query
                        if (strcmp(dest, queryDest) == 0) {
                            // This flight matches the query!
                            // Get the flight price
                            file >> price;
                            // Print flight info to console
                            cout << dep << " -> " << dest;
                            cout << " : " << airline << " ";
                            cout << "$" << price;
                            cout << endl;
                            // Increment result counter
                            counter ++;
                        }
                        else {
                            // This flight doesn't match the query
                            // Skip line
                            getline(file, in);
                            file >> ws;
                        }
                    }
                        break;
                    // 1 connection
                    case 1:
                        // See if there is a flight from the first flight's
                        // destination to the requested destination
                    {
                        // Get the flight destination city code
                        file.get(dest, 255, '|');
                        file.get(); // (Skip separator)
                        // Get the flight price
                        file >> price;
                        
                        // Find flight from here to the queried destination
                        FindConnection(airline, dep, price, dest, queryDest, counter);
                    }
                        break;
                    // Error
                    default:
                    {
                        cout << "[Error: Unsupported value for flight connections.]";
                    }
                        break;
                }
            }
            // If not, then skip the line
            else {
                // This flight doesn't match the query
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        // If current line is not a flight
        else {
            // This flight doesn't match the query
            // Skip line
            getline(file, in);
            file >> ws;
        }
    } // End cycle through data file
    // Display message if no results were found
    if (counter < 1) {
        cout << "[No results.]" << endl;
    }
    
    // Close the data file
    file.close();
    
    return;
}
//******************* END OF FUNCTION FINDFLIGHT *********************

//******************* FUNCTION FINDCONNECTION ************************
void FindConnection(char firstFlight[], char firstDepart[],
                    int firstPrice, char queryDepart[],
                    char queryDest[], int &counter)
{
    // Function FindConnection
    // Finds and prints a second flight from a first flight's
    // destination to the wanted destination
    
    // Input file
    ifstream file("data.txt", ios::in);
    
    char c;             // Character of data from file
    string in;          // String of data from file
    
    char airline[255];  // Airline code
    char dep[255];      // Departure city code
    char dest[255];     // Desination city code
    float price;        // Price value
    
    // Cycle through data file line by line
    while (file.get(c)) {
        // If current line is a flight
        if (c == 'f') {
            // Get the flight airline code
            file.get();
            file.get(airline, 255, '|');
            file.get();
            // Get the flight departure city code
            file.get(dep, 255, '|');
            file.get(); // (Skip separator)
            // See if this flight departure matches the query
            if (strcmp(dep, queryDepart) == 0) {
                // See if the flight's destination
                // matches the query
                
                // Get the flight destination city code
                file.get(dest, 255, '|');
                file.get(); // (Skip separator)
                // Compare the flight destination to the the query
                if (strcmp(dest, queryDest) == 0) {
                    // This flight matches the query!
                    // Get the flight price
                    file >> price;
                    
                    // PRINT FLIGHT INFO TO CONSOLE
                    // Flight 1 info
                    cout << firstDepart << " -> " << queryDepart;
                    cout << " : " << firstFlight << " ";
                    cout << "$" << firstPrice;
                    cout << "; ";
                    // Flight 2 info
                    cout << dep << " -> " << dest;
                    cout << " : " << airline << " ";
                    cout << "$" << price;
                    // Total price
                    cout << ", for a total cost of $";
                    cout << (firstPrice + price);
                    cout << endl;
                    // Increment result counter
                    counter++;
                }
                else {
                    // This flight doesn't match the query
                    // Skip line
                    getline(file, in);
                    file >> ws;
                }
            }
            // If not, then skip the line
            else {
                // This flight doesn't match the query
                // Skip line
                getline(file, in);
                file >> ws;
            }
        }
        // If current line is not a flight
        else {
            // This flight doesn't match the query
            // Skip line
            getline(file, in);
            file >> ws;
        }
    } // End cycle through data file
    
    // Close the data file
    file.close();
    
    return;
}
//**************** END OF FUNCTION FINDCONNECTION ********************