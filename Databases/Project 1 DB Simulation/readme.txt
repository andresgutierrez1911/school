  ___  _  _        _    _     __  __                                   
 | __|| |(_) __ _ | |_ | |_  |  \/  | __ _  _ _   __ _  __ _  ___  _ _ 
 | _| | || |/ _` || ' \|  _| | |\/| |/ _` || ' \ / _` |/ _` |/ -_)| '_|
 |_|  |_||_|\__, ||_||_|\__| |_|  |_|\__,_||_||_|\__,_|\__, |\___||_|  
            |___/                                      |___/           

Flight Manager 2.0
Written by Katie Schaffer
Developed on a Mac, using Xcode
Developed for Mac OS and Linux
Tested in Mac OS 10.9.4 and Debian 7.6.0
9-16-2014

> COMPILING

  Mac & Linux:
  1. Open a terminal window
  2. Navigate into the Flight Manager root folder
  3. Type the following command:
     g++ main.cpp -o FlightManager
     
     (Or replace FlightManager with your preferred name.)


> RUNNING

  Mac & Linux:
  1. Open a terminal window
  2. Navigate into the Flight Manager folder
  3. Type ./FlightManager_mac
     (Or replace FlightManager_mac with the name of your compiled version.)


> USAGE

(Refer to project1.pdf for the full instructions/purpose of this project.)

Basic command examples:

Add a city to the database: a c stl st. louis
Add an airline to the database: a a twa trans world airlines
Add a flight to the database: a f twa stl jfk 119
List cities: l c
List airlines: l a
List flights: l f
Find a flight from STL to JFK: f stl jfk 0
Quit: q


> KNOWN ISSUES
  
. Typos or other commands that deviate from syntax may cause the program to freeze, in which case the user must type ^C to quit.
. Character arrays are used (all serving the purpose of strings.)


> DEVELOPMENT LOG

I began approaching the problem by writing a really basic outline of the program's overall structure:

Repeat until quit
  Prompt for input
    Receive input
    Do something, based on input
      If add
	[ADD]
      If list  
	[LIST]
      If find
	[FIND]

...And the main functions:

[ADD]
1. Update local arrays with data from data file
2. Add new data to local arrays
3. Update data file with local data
   (This involved saving all local data to a new file, and then replacing the old file.)

[LIST]
1. Update local arrays with data from data file
2. Process local data
   For each element
    If (the element is of the queried type)
     Print element info to console

[FIND]
1. Update local arrays with data from data file
2. Process local list of flights
   For each flight
    If (the departure city matches that of the query)..
     And If of the destination city also matches the query..
       The flight is a match

9-8-2014
Flight Manager 1.0 was written. It served all the purposes necessary, but it relied on arrays. In order to keep the data file updated, the local arrays were updated before every command was executed. Immediately after changes were made to local arrays, the data file would be completely re-written.

9-12-2014 — 9-16-2014
I began re-writing the program in a way that did not use the system of local arrays. Lots of unnecessary code was removed, and every function was either deleted or re-written. However, the program's original basic structure was still in place. The biggest difference is in the way data is processed. Instead of looping through arrays of organized structures, the program must scan through the data file itself several times to find a queried piece of data.