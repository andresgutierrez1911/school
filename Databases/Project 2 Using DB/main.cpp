#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include <conio.h>

using namespace std;

string myget_passwd();
int AddCity(char[], char[], MYSQL*, MYSQL);
int AddAirline(char[], char[], MYSQL*, MYSQL);
int AddFlight(char[], char[], char[], int, MYSQL*, MYSQL);
int List(char, MYSQL*, MYSQL);
int FindFlight(char[], char[], int, MYSQL*, MYSQL);
int FindConnection(char[], char[], MYSQL*, MYSQL);
bool IsDuplicate(char, char[], MYSQL*, MYSQL);
string GetElemName(char, char[], MYSQL*, MYSQL);
int Save(char, char[], MYSQL*, MYSQL);
int Merge(char, char[], MYSQL*, MYSQL);
int Clear(MYSQL*, MYSQL);

string cities_T = "table_c";
string airlines_T = "table_a";
string flights_T = "table_f";

// myget_passwd() via Dr. Blythe
// Reads in password, printing *s to console
string myget_passwd()
{
	string passwd;
	for(;;)
	{
		char ch;
		ch=getch();				// get input char
		if (ch==13 || ch==10) 	// if done reading char
			break;           	// stop reading
		cout << '*';  // print * to console for every read char
		passwd+=ch;   // add read char to the password
	}
	cin.sync(); 	// clear cin buffer
	cout << endl;  	// drop down a line
	return passwd;
}

int main()
{
    bool quit = false;  // Quit signal
    char input;         // User's input
    char code[255];     // ID code placeholder
    char code2[255];    // ID code placeholder 2
	char code3[255];    // ID code placeholder 3
    int num;            // Simple number placeholder
	string inString;	// String of input

	MYSQL *conn,		// actual mysql connection
		mysql;			// local mysql data
	string db_host;		// mysql hostname
	string db_user;		// mysql userid
	string db_password;	// mysql password
	string db_name;		// mysql db name
	int status;			// status of mysql operations

	cout << "_____ FLIGHT MANAGER _____" << endl;

	// Initialize mysql
	cout << "[Initializing client DB subsystem] ... "; cout.flush();
	mysql_init(&mysql);
	cout << "[OK!]" << endl << endl;

	// Get mysql hostname
	cout << "MySQL database hostname (or IP adress):";
	cin >> db_host;
	// Get username
	cout << "MySQL database username:";
	cin >> db_user;
	// Get password
	cout << "MySQL database password:";
	db_password=myget_passwd();
	// Set db name to the username
	db_name= db_user;

	// ***** CREATE TABLES *****
	cout << "Connecting to remote DB ..."; 
	cout.flush();
	// Connect to server
	conn = mysql_real_connect(&mysql, 
		                    db_host.c_str(), db_user.c_str(), db_password.c_str(), db_name.c_str(),
				             0,0,0); 

	// If there is a problem making the connection
	if (conn==NULL) {
		// Print error message and exit program
		cout << mysql_error(&mysql) << endl;
		return 1; 
	}
	// Otherwise continue
	else
		cout << "Connected!" << endl << endl;

	// Create cities table
	string myQuery = "create table if not exists ";  
	myQuery += cities_T;
	myQuery += "(code char(100) NOT NULL, name char(100) NOT NULL, ";
	myQuery += "PRIMARY KEY (code) );";

	// Send the query
	status = mysql_query(conn, myQuery.c_str());

	// If error creating table
	if (status!=0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1; 
	}

	// Create airline table
	myQuery = "create table if not exists ";  
	myQuery += airlines_T;
	myQuery += "(code char(100) NOT NULL, name char(100) NOT NULL, ";
	myQuery += "PRIMARY KEY (code) );";

	// Send the query
	status = mysql_query(conn, myQuery.c_str());

	// If error creating table
	if (status!=0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1; 
	}

	// Create flight table
	myQuery = "create table if not exists ";  
	myQuery += flights_T;
	myQuery += "(airline char(100), departure char(100), destination char(100), cost integer, ";
	myQuery += "PRIMARY KEY (airline,departure,destination,cost), ";
	myQuery += "FOREIGN KEY (airline) REFERENCES ";
	myQuery += airlines_T;
	myQuery += "(code), ";
	myQuery += "FOREIGN KEY (departure) REFERENCES ";
	myQuery += cities_T;
	myQuery += "(code), ";
	myQuery += "FOREIGN KEY (destination) REFERENCES ";
	myQuery += cities_T;
	myQuery += "(code) );";

	// Send the query
	status = mysql_query(conn, myQuery.c_str());

	// If error creating table
	if (status!=0) {
		// Print error message and quit
		cout << mysql_error(&mysql) << endl;
		return 1; 
	}

	// ***** FINISH CREATING TABLES *****
    
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
                    // - ADD CITY -
                    case 'c':
					    // Scan through separator
						cin.get();
						// Get new city code
						cin.get(code, 255, ' ');
						// Scan through separator
						cin.get();
						// Get city name
						cin.get(code2, 255);
						
						// Add city to database
						status = AddCity(code,code2,conn,mysql);
						// In case of errors
						if (status!=0)
						{
							// Print error message to console
							cout << mysql_error(&mysql) << endl;
							// Quit the program
							return 1;  
						}
						break;
					// - ADD AIRLINE -
                    case 'a':
						// Scan through separator
						cin.get();
						// Get new airline code
						cin.get(code, 255, ' ');
						// Scan through separator
						cin.get();
						// Get airline name
						cin.get(code2, 255);
						
						// Add airline to database
                        status = AddAirline(code,code2,conn,mysql);
						// In case of errors
						if (status!=0)
						{
							// Print error message
							cout << mysql_error(&mysql) << endl;
							// Force quit
							return 1; 
						}
                        break;
                    // - ADD FLIGHT -
                    case 'f':
					    // Scan through separator
						cin.get();
						// Get airline code
						cin.get(code, 255, ' ');
						// Scan through separator
						cin.get();
						// Get departure city code
						cin.get(code2, 255, ' ');
						// Scan through separator
						cin.get();
						// Get destination city code
						cin.get(code3, 255, ' ');
						// Scan through separator
						cin.get();
						// Get flight price
						cin >> num;

						// Add flight to database
                        status = AddFlight(code,code2,code3,num,conn,mysql);
						// In case of errors
						if (status!=0)
						{
							// Print error message
							cout << mysql_error(&mysql) << endl;
							// Abort mission
							return 1; 
						}
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
                        
                        // Retrieve all cities from db and print them
                        status = List(input,conn,mysql);
                        
                        break;
                    // - LIST AIRLINES -
                    case 'a':
                        
                        // Retrieve all airlines from db and print them
                        status = List(input,conn,mysql);
                        
                        break;
                    // - LIST FLIGHTS -
                    case 'f':
                        
                        // Retrieve all flights from db and print them
                        status = List(input,conn,mysql);
                        
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
                status = FindFlight(code, code2, num, conn, mysql);
                
                break;
			// ----- SAVE -----
			case 's':
				// Receive specified table to save
                cin >> input;
				// Scan through separator
				cin.get();
				// Read specified filename
				cin.get(code, 255);
				// Save specified table
				status = Save(input, code, conn, mysql);
				// In case of error
				if (status!=0)
				{   // Print error message
					cout << mysql_error(&mysql) << endl;
					// Force quit
					return 1; 
				}
				// Otherwise print success message
				else {
					cout << "[Data saved successfully to \"";
					cout << code << "\"]" << endl;
				}
				break;
			// ----- MERGE -----
			case 'm':
				// Receive specified table to save
                cin >> input;
				// Scan through separator
				cin.get();
				// Read specified filename
				cin.get(code, 255);
				// Merge data into specified table
				status = Merge(input, code, conn, mysql);
				// In case of error
				if (status!=0)
				{   // Print error message
					cout << mysql_error(&mysql) << endl;
					// Force quit
					return 1; 
				}
				// Otherwise print success message
				else {
					cout << "[Data merged successfully from \"";
					cout << code << "\"]" << endl;
				}
				break;
			// ----- CLEAR -----
			case 'c':
				// Clear all data from tables
				Clear(conn,mysql);
				break;
            // ----- QUIT -----
            case 'q':
                // Signal the program to quit
                quit = true;
                cout << "[Goodbye.]";
                break;
            // ERROR
            default:
                cout << "[Invalid entry: \"" << input << "\"]";
                cout << endl;
                break;
        }
        
    } // End program

	// Close connection
	mysql_close(conn);

    return 0;
}

/* **************************************************************** */

//************************* FUNCTION ADDCITY *************************
int AddCity(char cityCode[], char cityName[], MYSQL *conn, MYSQL mysql) 
{
	// Function AddCity
	// Inserts new city row into mysql city table

	int status;	// status of operation
    
	// Make sure this is not a duplicate
	if (!IsDuplicate('c', cityCode, conn, mysql)) {
		// Add city to table if not duplicate
		// Write query
		string myQuery = "insert into ";  
		myQuery += cities_T;
		myQuery += " VALUES(\'";
		myQuery += cityCode;
		myQuery += "\', \'";
		myQuery += cityName;
		myQuery += "\');";
		// Send the query, attempting to add row to db
		status = mysql_query(conn, myQuery.c_str());
	}
	// In case of duplicate
	else {
		// Don't do anything
		status = 0;
	}

	return status;
}
//*********************** END FUNCTION ADDCITY ***********************

//*********************** FUNCTION ADDAIRLINE ************************
int AddAirline(char airlineCode[], char airlineName[], 
			   MYSQL *conn, MYSQL mysql) 
{
	// Function AddAirline
	// Inserts new airline row into mysql city table

	int status;	// status of operation

	// Make sure this is not a duplicate
	if (!IsDuplicate('a', airlineCode, conn, mysql)) {
		// Add airline to table if not a duplicate
		// Write query
		string myQuery = "insert into ";  
		myQuery += airlines_T;
		myQuery += " VALUES(\'";
		myQuery += airlineCode;
		myQuery += "\', \'";
		myQuery += airlineName;
		myQuery += "\');";
		// Send the query, attempting to add row to db
		status = mysql_query(conn, myQuery.c_str());
	}
	// In case of duplicate
	else {
		// Don't do anything
		status = 0;
	}

	return status;
}
//********************* END FUNCTION ADDAIRLINE **********************

//************************ FUNCTION ADDFLIGHT ************************
int AddFlight(char code[], char dep[], char dest[], int price, 
				MYSQL *conn, MYSQL mysql)
{
	// Function AddFlight
	// Inserts new flight row into mysql city table
    
    int status;	// status of operation
    
	// Write the query
	string myQuery = "insert into ";  
	myQuery += flights_T;
	myQuery += " VALUES(\'";
	myQuery += code;
	myQuery += "\', \'";
	myQuery += dep;
	myQuery += "\', \'";
	myQuery += dest;
	myQuery += "\',";
	myQuery += std::to_string(price);
	myQuery += ");";

	// Send the query, attempting to add row to db
	status = mysql_query(conn, myQuery.c_str());
	
	return status;
}
//********************* END OF FUNCTION ADDFLIGHT ********************

//************************ FUNCTION DUPLICATE ************************
bool IsDuplicate(char type, char code[], MYSQL *conn, MYSQL mysql)
{
    // Function IsDuplicate
    // Detetects if an element is already in a db table

	MYSQL_RES *res; // mysql query results
	MYSQL_ROW row;  // a row of a table from results
	string myQuery;	// mysql query
	int status;		// status of mysql search

	// Begin search query
	myQuery = "select code from ";
	
	// If adding a new city
	if (type == 'c') {
		// Search city table for duplicate
		myQuery += cities_T;
	}
	// If adding a new airline
	else if (type == 'a') {
		// Search airline table for duplicate
		myQuery += airlines_T;
	}
	// If invalid type specified
	else {
		// Error message
		cout << "Error: Invalid element type specified.";
	}

	// Finish query
	myQuery += " where code=\"";
	myQuery += code;
	myQuery += "\" ;";

	// Send query, attempting to search table for duplicates
	status = mysql_query(conn, myQuery.c_str());

	// In case of error
	if (status!=0)
	{
		// Print error message to console
		cout << "Warning: An error occured while checking for duplicates." << endl;
		cout << mysql_error(&mysql) << endl;
	}

	// get the query result(s)
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);

	// clean up the query
	mysql_free_result(res);

	// If any results are found
	if (row!=NULL) {
		// A duplicate has been found
		return true;
	}
    // Otherwise return false because no duplicates are found
    return false;
}
//********************* END OF FUNCTION DUPLICATE ********************

//*************************** FUNCTION LIST **************************
int List(char query, MYSQL *conn, MYSQL mysql)
{
    // Function List
    // Lists row data from a db table
    
	MYSQL_RES *res; // mysql query results
	MYSQL_ROW row;  // a row of a table from results
	string myQuery;	// mysql query
	int status;		// status of operation
	int resultCount = 0;	// Result counter

	// Begin query
	myQuery = "select * from ";

    // Select appropriate table
	switch (query) {
    // Print flight element (city)
    case 'c':
		// Select from city table
		myQuery += cities_T;
		break;
	case 'a':
		// Select from airline table
		myQuery += airlines_T;
		break;
    // Print flight
    case 'f':
		// Select from flight table
		myQuery += flights_T;
		break;
    // Error
    default:
		cout << "[Error: Invalid query.]" << endl;
		break;
	} // End selecting table

	// End query
	myQuery += ";";

	// Send query, attempting to search for all data in table
	status = mysql_query(conn, myQuery.c_str());
	// Get results
	res = mysql_store_result(conn);

	// Go through each row of table
	for(row=mysql_fetch_row(res); row!=NULL; 
		row=mysql_fetch_row(res)) {
		resultCount++; // Increment result counter
		// If flight
		if (query == 'f') {
			// Print row data to console
			cout << GetElemName('a', row[0], conn, mysql) << ": ";
			cout << GetElemName('c', row[1], conn, mysql) << " -> ";
			cout << GetElemName('c', row[2], conn, mysql) << " ";
			cout << "$" << row[3] << endl;
		}
		// If airline of city
		else {
			// Print row data to console
			cout << row[0] << " \t" << row[1] << endl;
		}
	}

	// If there are no results
	if (resultCount == 0) {
		// State so
		cout << "[No results.]" << endl;
	}

	// clean up the query
	mysql_free_result(res);
   
    return status;
}
//************************ END OF FUNCTION LIST **********************

//********************** FUNCTION FINDFLIGHT *************************
int FindFlight(char queryDepart[], char queryDest[],
                int queryConnections, MYSQL *conn, MYSQL mysql)
{
    // Function FindFlight
    // Finds and lists flights based on departure and
    // destination queried
    
	MYSQL_RES *res; // mysql query results
	MYSQL_ROW row;  // a row of a table from results
	string myQuery;	// mysql query
	int status;		// status of operation
	int resultCount = 0;	// Result counter

	// If requested number of connections is 0
	if (queryConnections == 0) {
		// Write query to find flights
		myQuery = "select * from ";
		myQuery += flights_T;
		myQuery += " where departure=\"";
		myQuery += queryDepart;
		myQuery += "\" AND destination=\"";
		myQuery += queryDest;
		myQuery += "\";";
		// Send query
		status = mysql_query(conn, myQuery.c_str());
		// If error occurs
		if (status!=0) {
			// Print error message and abort mission
			cout << mysql_error(&mysql) << endl;
			return status;
		}

		// Get the query result(s)
		res = mysql_store_result(conn);

		// Go through each row in results
		for(row=mysql_fetch_row(res); row!=NULL; 
			row=mysql_fetch_row(res)) {
			// Increment result counter
			resultCount++;
			// Print result
			cout << row[1] << " -> " << row[2] << " : ";
			cout << row[0] << " $" << row[3] << endl;
		}

		// If there are no results
		if (resultCount == 0) {
			// State so
			cout << "[No results.]" << endl;
		}

		// Clean up the query
		mysql_free_result(res);
	}
	// If the requested number of connections is 1
	else if (queryConnections == 1) {
		// Find flights and connections
		status = FindConnection(queryDepart, queryDest, conn, mysql);
	}
	else {
		// ERROR
		cout << "[Error.]";
		return 1;
	}

    return status;
}
//******************* END OF FUNCTION FINDFLIGHT *********************

//******************* FUNCTION FINDCONNECTION ************************
int FindConnection(char queryDepart[], char queryDest[], 
				   MYSQL *conn, MYSQL mysql)
{
	MYSQL_RES *res, *res2;	// mysql query results
	MYSQL_ROW row, row2;	// a row of a table from results
	string myQuery;			// mysql query
	int status;				// status of operation
	int resultCount = 0;	// result counter

	// Write query to find flight with connections
	myQuery = "select * from ";
	myQuery += flights_T;
	myQuery += " as t ";
	myQuery += "where departure=\"";
	myQuery += queryDepart;
	myQuery += "\" and exists(";
	myQuery += "select * from ";
	myQuery += flights_T;
	myQuery += " where departure=t.destination and destination =\"";
	myQuery += queryDest;
	myQuery += "\");";

	// Send query
	status = mysql_query(conn, myQuery.c_str());

	// If an error occurs
	if (status!=0) {
		// Print error message and abort mission
		cout << mysql_error(&mysql) << endl;
		return status;
	}

	// Get the query results
	res = mysql_store_result(conn);

	// Go through each result
	for(row=mysql_fetch_row(res); row!=NULL; 
		row=mysql_fetch_row(res)) {
		// Find all the connections from this flight 
		// to the destination
		myQuery = "select * from ";
		myQuery += flights_T;
		myQuery += " AS t ";
		myQuery += "where departure=\"";
		myQuery += row[2]; // The first flight's destination
		myQuery += "\" AND destination=\"";
		myQuery += queryDest; // The goal destination
		myQuery += "\";";

		// Get results
		status = mysql_query(conn, myQuery.c_str());
		// Handle possible errors
		if (status!=0) {
			// Print error message
			cout << mysql_error(&mysql) << endl;
			return status;  // Force quit
		}
		// Get result of this query
		res2 = mysql_store_result(conn);

		// Go through each connection found
		for(row2=mysql_fetch_row(res2); row2!=NULL; 
			row2=mysql_fetch_row(res2)) {
			// Increment result counter
			resultCount++;
			// Print flights
			cout << row[1] << " -> " << row[2] << " : " << row[0]
				<< " $" << row[3] << " ; ";
			cout << row2[1] << " -> " << row2[2] << " : " << row2[0]
				<< " $" << row2[3] << " ; ";
			cout << "For a total cost of: $";
			cout << ( std::atoi(row[3]) + std::atoi(row2[3]) ) ;
			cout << endl;
		}
	}

	// If there are no results
	if (resultCount == 0) {
		// State so
		cout << "[No results.]" << endl;
	}
	else {
		// clean up the query
		mysql_free_result(res2);
	}

	// clean up the query
	mysql_free_result(res);
   
    return status;
}
//**************** END OF FUNCTION FINDCONNECTION ********************

//************************ FUNCTION GETELEMNAME **********************
string GetElemName(char type, char code[], MYSQL *conn, MYSQL mysql)
{
    // Function GetElemName
    // Receives a city or airline code and returns the full name
    
	MYSQL_RES *res; // mysql query results
	MYSQL_ROW row;  // a row of a table from results
	string myQuery;	// mysql query
	int status;		// status of operation
	string name = "";	// full name of element

	// Begind query
	myQuery = "select name from ";

    // Select appropriate table
	switch (type) {
    // Print flight element (city)
    case 'c':
		// Select from city table
		myQuery += cities_T;
		break;
	case 'a':
		// Select from airline table
		myQuery += airlines_T;
		break;
    // Error
    default:
		cout << "[Error: Invalid query.]" << endl;
		break;
	} // End selecting table

	myQuery += " where code=\"";
	myQuery += code;
	myQuery += "\";";

	status = mysql_query(conn, myQuery.c_str());

	// get the query result(s)
	res = mysql_store_result(conn);
	row=mysql_fetch_row(res);

	// If a corresponding name is found in the database
	if (row!=NULL) {
		// The resulting name is the name of the element queried
		name += row[0];
	}
	// If no item is found in the database
	else {
		// Set the name to a place-holding error message
		name += "[Error]";
	}

	// clean up the query
	mysql_free_result(res);
    
    return name;
}
//********************* END OF FUNCTION GETELEMNAME ******************

//*************************** FUNCTION SAVE **************************
int Save(char table, char filename[], MYSQL *conn, MYSQL mysql) 
{
	// Function Save
	// Writes all data from a mysql table to a file

	// Data file
    ofstream file(filename, ios::app);

	MYSQL_RES *res; // mysql query results
	MYSQL_ROW row;  // a row of a table from results
	string myQuery;	// mysql query
	int status = 0; // status of operation

	// Begin query
	myQuery = "select * from ";

    // Select appropriate table
	switch (table) {
    // Print flight element (city)
    case 'c':
		// Select from city table
		myQuery += cities_T;
		break;
	case 'a':
		// Select from airline table
		myQuery += airlines_T;
		break;
    // Print flight
    case 'f':
		// Select from flight table
		myQuery += flights_T;
		break;
    // Error
    default:
		cout << "[Error: Invalid query.]" << endl;
		break;
	} // End selecting table

	// Finish query
	myQuery += ";";

	// Send the query
	status = mysql_query(conn, myQuery.c_str());

	// Get query results
	res = mysql_store_result(conn);

	// Go through each row of results
	for(row=mysql_fetch_row(res); row!=NULL; 
		row=mysql_fetch_row(res)) {
		// Print row data (for flight)
		if (table == 'f') {
			file << row[0] << " " << row[1] << " " << row[2] 
			<< " " << row[3] << endl;
		}
		// Print row data (for city or airline)
		else {
			file << row[0] << " " << row[1] << endl;
		}
	}

	// Clean up the query
	mysql_free_result(res);
   
	// Close the file
	file.close();

    return status;
}
//************************* END OF FUNCTION SAVE *********************

//**************************** FUNCTION MERGE ************************
int Merge(char table, char filename[], MYSQL *conn, MYSQL mysql) 
{
	// Function Merge
	// Merges a list of data from a data file to a mysql table

    // Input file
    ifstream file(filename, ios::in);

	string myQuery;	// mysql query
	int status = 0;	// status of operation

	string in;      // string of data
    char data[255]; // character array of data
	char name[255]; // character array for name
    char dep[255];  // Departure city code
    char dest[255]; // Desination city code
    float price;    // Price value

	// Cycle through data file
    while (file.get(data, 255, ' ') != NULL) {
		// Read in data from file
		switch(table) {
		// Read airlines
		case 'a':
			// Get element code
			//file.get(data, 255, ' ');
			//cout << data << " ";
			// Skip separator
			file.get();
			// Get element name
			file.get(name, 255);
			//cout << name << endl;
			// Skip separator
			//file.get();
			// Skip whitespace
			file >> ws;

			// Add airline to the database
			AddAirline(data,name,conn,mysql);

			break;
		// Read cities
		case 'c':
			// Get element code
			//file.get(data, 255, ' ');
			//cout << data << " ";
			// Skip separator
			file.get();
			// Get element name
			file.get(name, 255);
			//cout << name << endl;
			// Skip separator
			//file.get();
			// Skip whitespace
			file >> ws;

			// Add city to database
			AddCity(data,name,conn,mysql);

			break;
		// Read flights
		case 'f':
			// Read flight code
			//file.get(data, 255, ' ');
			//cout << data << " ";
			// Skip separator
			file.get();
			// Read departure city code
			file.get(dep, 255, ' ');
			//cout << dep << " ";
			// Skip separator
			file.get();
			// Read destination city cody
			file.get(dest, 255, ' ');
			//cout << dest << " ";
			// Skip separator
			//file.get();
			// Read price
			file >> price;
			//cout << " $" << price << endl;
			// Skip whitespace
			file >> ws;

			// Add flight to the database
			AddFlight(data,dep,dest,price,conn,mysql);

			break;
		// Error
		default:
			// Print error message
			cout << "[Syntax error: Invalid table specification]";
			break;
		}
	}

	// Close the file
	file.close();

	return status;
}
//************************* END OF FUNCTION MERGE *********************

//************************** FUNCTION CLEAR ***************************
int Clear(MYSQL *conn, MYSQL mysql)
{
	// Function Clear
	// Removes all data from cities, airlines, and flights tables

	int status = 0;	// status of operation
	string myQuery;	// mysql query

	// Clear data from flight table
	myQuery = "delete from ";  
	myQuery += flights_T;
	myQuery += ";";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	// In case of error
	if (status!=0) {   
		// Print error message and alter status
		cout << mysql_error(&mysql) << endl;
		status = 1;
	}

	// Clear data from city table
	myQuery = "delete from ";  
	myQuery += cities_T;
	myQuery += ";";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	// In case of error
	if (status!=0) {   
		// Print error message and alter status
		cout << mysql_error(&mysql) << endl;
		status = 1;
	}

	// Clear data from airline table
	myQuery = "delete from ";  
	myQuery += airlines_T;
	myQuery += ";";
	// Send the query
	status = mysql_query(conn, myQuery.c_str());
	// In case of error
	if (status!=0) {   
		// Print error message and alter status
		cout << mysql_error(&mysql) << endl;
		status = 1;
	}

	// If operation completed successfully
	if (status == 0) {
		// State so
		cout << "[Successfully cleared the database.]" << endl;
	}

	return status;
}
//************************** FUNCTION CLEAR ***************************