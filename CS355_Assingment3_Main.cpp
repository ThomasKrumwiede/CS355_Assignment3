#include "Sub.h"
#include <iostream>

using namespace std;

int main() {
    //Create the Subprograms 
    Sub* main_ = new Sub("Main", nullptr, 0, 2);
    Sub* A = new Sub("A", main_, 1, 3);
    Sub* B = new Sub("B", main_, 2, 2);
    Sub* C = new Sub("C", A, 0, 1);
    Sub* D = new Sub("D", C, 1, 3);
    // Start Test 

    cout << "Test 1: \n"; 
    // Call the main funtion  
    main_->call_();
    
    // This will test the creation of nested subprograms
    A->call_();
    C->call_();
    D->call_();

    D->return_();
    C->return_();
    A->return_();

   

    // This shows that two sibling programs cant be called at the same time
    cout << "\n \n \n Test 2: \n";
    A->call_();
    B->call_();
    C->call_(); 

    C->return_();
    A->return_();

    
    // Test to make sure subprogram C cant be called when not in scope 
    cout << "\n \n \n Test 3: \n";
    B->call_();

    C->call_();  

    B->return_();

    // Return main funciton 
    main_->return_();

    return 0;
}