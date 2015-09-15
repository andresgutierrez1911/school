  ___  _  _        _    _     __  __                                   
 | __|| |(_) __ _ | |_ | |_  |  \/  | __ _  _ _   __ _  __ _  ___  _ _ 
 | _| | || |/ _` || ' \|  _| | |\/| |/ _` || ' \ / _` |/ _` |/ -_)| '_|
 |_|  |_||_|\__, ||_||_|\__| |_|  |_|\__,_||_||_|\__,_|\__, |\___||_|       
            |___/                                      |___/           



Project 2

Flight Manager 2.1

Written by Katie Schaffer

Developed on Windows using Microsoft Visual Studio 2012

Developed for Windows 7

Tested in Windows 7
10-21-2014

* This project is essentially Project 1, except a database is used
instead of the simulated database. Project 1 instructions are included *


> NOTE: For this project, I thought I'd try using Microsoft Visual 
Studio for once. That was clearly a horrible mistake, and I apologize.
It won't happen again.


> COMPILING

How to compile using Microsoft Visual Studio 2012 on Windows 7:
1) Start up Visual Studio 2012
2) Create a new project (File > New > Project), and select
   "Visual C++" (under "Other Languages" on the left), and finally,
   select "Empty Project."
3) Go to Project > "Add Existing Item". Select main.cpp to add the 
   source code to the project.
4) Make adjustment for MySql (to deal with all of the errors) as 
   the professor described in class.
5) Again. I apologize for Visual Studio.


> RUNNING

1) Navigate to "Visual Studio 2012"\Projects\[Name of Project]\Debug 
   where [Name of Project] is the name of the project where this
   program was built.
2) Type [Name of Project].exe and press ENTER.


> KNOWN ISSUES

- It is possible that some typos or other commands that deviate from 
syntax may cause the program to freeze, in which case the user must 
type ^C to quit.
 
- Character arrays are used, despite the instructions to not use them (all serving the purpose of strings.)



> ABOUT

To complete this assignment, I mainly needed to replace all of the file-searching logic with MySql queries. The program needed to prompt for MySql user credentials upon startup, and tables needed to be created upon logging in. From there, most functions could be simplified by incorporating MySql queries to store and retrieve information.