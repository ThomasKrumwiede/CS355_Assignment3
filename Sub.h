#ifndef SUB_H
#define SUB_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class Sub {
private:
    string name;
    Sub* staticParent;
    vector<Sub*> staticChildren;
    int numArgs;
    int numLocals;
    int staticDepth;
    int id; // Subprogram id number

    static map<string, int> nameToId; // Map the subprogram name to the id number 
    static map<int, Sub*> idToSub;
    static vector<int> runtimeStack; // Simulation of the runtime stack 
    static vector<int> display; // Simulation of the disply 
    static vector<int> activeSubprogramIds;
    static int nextId;
    static bool firstCallMade;

public:

    /*
    Constructor for Sub
    @param name is the name of the subprogram
    @param parent is a pointer to the subprograms parent
    @param args is the number of arguments the subprogram takes
    @param locals is the number of local variables the subprogram has
     */
    Sub(std::string name, Sub* parent, int args, int locals);

    /*
    Add a subprogram to the list of staticChildren for its parent
    @param child is the node that it going to be added to the static children list
    @return void
    */
    void addChild(Sub* child);

    // All of the get methods 
    const string& getName() const;
    int getDepth() const;
    int getNumArgs() const;
    int getNumLocals() const;
    const vector<Sub*>& getChildren() const;
    Sub* getParent() const;

    /*
    Helper fucntion to determine if a subprogram is following static scoping 
    @caller is a pointer to the subprogram that is being tested
    @return bool*/
    bool isVisibleFrom(Sub* caller);

    /*
    Call_ method simulates a new subprogam being called
    this method will also call the print method to print the activation records in the runtime stack 
    and will also print the display 
    @return void */
    void call_();

    /*
    return_ method will simulate a subprocram reaching its completed state and will termiate it 
    @return void*/
    void return_();

    /*
    this method will print the contents of the runtime stack and the disply\
    @return void*/
    static void printStackAndDisplay();
};

#endif // SUB_H