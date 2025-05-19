# CS355_Assignment3
This is code for assignment 3 for CS355 programing languages

In this problem you will make a class that simulates the run-time stack maintenance using the display approach as discussed in class. Write a console application in Visual Studio  2019 C++. Make a class Sub, representing subprograms in a statically scoped programming language that supports nested subprograms. A Sub object has instance variables for a name, a pointer to its static parent object, a collection (initially empty) of pointers to static children, a number of arguments, a number of local variables, and a static depth. There are no other instance variables. Assume that the only data type in this language is integer, and subprograms do not return a value. If the static parent's pointer is NULL, this Sub object does not have a static parent.

Supply a constructor with 4 arguments: name (a string), a pointer to static parent, number of arguments, and number of local variables. The constructor will initialize the instance variables corresponding to its arguments, will calculate and initialize the static depth, and will add a pointer to its object in the collection of its static parent's children pointers. 

Supply methods call_ and return_ without arguments, which will maintain a run-time stack and a display, both implemented as a class-level variables of class Sub. Implement them as arrays or vectors of integers (not of Sub objects). The bottom of the stack should be at position 0. In the display, the static link to the subprogram with static depth 0 should be at position 0.
•	Method call_ will check if this sub's name is visible from the subprogram with the activation record currently at the top of the runtime stack, and if so, will place an activation record for the called subprogram as the new top of the runtime stack and will reflect this in the display.  Otherwise this call is an error. 
•	Method return_ will remove the activation record of this subprogram from the runtime stack and will reflect this in the display.
•	If the static scoping rules are violated, show meaningful precise error messages, providing all available information.

Supply any necessary get methods.

Each activation record should contain in this order, from bottom (smallest position number) to top (largest position number):
•	an integer encoding of the subprogram name (the name is a string, but your stack is of integers, so you will need to map the names to integers).. 
•	saved display link: the position in the runtime stack (index) that was stored in the display position corresponding to this call, before the call. You need to save it, in order to be able to restore it after the call, in case this call changed it in the display. 
•	dynamic link: the position in the runtime stack (index) of the last element of the activation record of the caller (the old stack top). A Sub object with name main_ is always the first called (otherwise there is an error), and the dynamic link of the first activation record on the stack is minus one (-1). 
•	one integer element for each argument
•	one integer element for each local variable
The main function of your program should initialize empty runtime stack and display, declare a number of Sub objects, and provide a sufficient number of suitable calls of their call_ and return_ methods to demonstrate in full the rules of static scoping visibility and thus thoroughly test the class Sub and the maintenance of the stack and the display according to the static scoping rules. Above each test, please include a comment explaining the reason for the test.

In your class Sub supply a convenient way to print on the screen the activation records currently on the stack in a readable format, resembling the runtime stack and the display vertically next to each other, with their tops towards the top and their bottoms at the bottom of the printout. Number each line with the position number (index) of the corresponding stack or display cell, so that it is easy to check the static links from the display and the dynamic links from the stack.  For each activation record print (bottom to top, the subprogram name at the bottom, and the local variables at the top of the activation record printout):

•	“Sub” followed by the subprogram name (not the integer encoding), 
•	“Saved Link: “ followed by the saved link as an integer position in the stack (index)
•	“Dynamic Link: “ followed by the dynamic link as an integer position in the stack (index) 
•	For each argument print “Argument” followed by the sequence number of this argument
•	For each local variable print “Local Var” followed by the sequence number of this variable.

Inside the display cells print the currently stored link to the bottom of the corresponding activation record on the stack (an integer index to the runtime stack) and the name of the subprogram to which activation record it links.
