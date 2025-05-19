#include "Sub.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

// Static member definitions
map<string, int> Sub::nameToId;
map<int, Sub*> Sub::idToSub;
vector<int> Sub::runtimeStack;
vector<int> Sub::display;
vector<int> Sub::activeSubprogramIds;
int Sub::nextId = 0;
bool Sub::firstCallMade = false;

Sub::Sub(string name, Sub* parent, int args, int locals){

    // Set the subprograms information 
    this->name = name;
    this->staticParent = parent;
    this->numArgs = args;
    this->numLocals = locals;

    // Determine the static depth of the subprogram 
    staticDepth = (parent == nullptr) ? 0 : parent->staticDepth + 1;

    // Prevents two subprograms from having the same name 
    if (nameToId.count(name)) {
        throw runtime_error("Duplicate subprogram name: " + name);
    }

    // Set the subprogram ID 
    id = nextId++;
    nameToId[name] = id;
    idToSub[id] = this;

    // Add new subprogram to the list of children of its parent 
    if (parent != nullptr) {
        parent->addChild(this);
    }
}

void Sub::addChild(Sub* child) {
    staticChildren.push_back(child);
}

// The necessary get methods  
const string& Sub::getName() const { return name; }
int Sub::getDepth() const { return staticDepth; }
int Sub::getNumArgs() const { return numArgs; }
int Sub::getNumLocals() const { return numLocals; }
const vector<Sub*>& Sub::getChildren() const { return staticChildren; }
Sub* Sub::getParent() const { return staticParent; }

bool Sub::isVisibleFrom(Sub* caller) {
    Sub* current = this->staticParent;
    while (current != nullptr) {
        if (current == caller) return true;
        current = current->staticParent;
    }
    return false;
}

void Sub::call_() {
    cout << "Calling: " << name << endl;

    // Evaluate scope 
    if (!firstCallMade) {
        if (name != "Main") {
            cout << "Error: First call must be to 'Main'.\n";
            return;
        }
        firstCallMade = true;
    }
    else {
        if (activeSubprogramIds.empty()) {
            cout << "Error: No active subprograms on the stack.\n";
            return;
        }

        Sub* caller = idToSub[activeSubprogramIds.back()];
        if (!this->isVisibleFrom(caller)) {
            cout << "Error: Static scoping violated - " << name
                << " is not visible from " << caller->getName() << endl;
            return;
        }
    }

    // Manage the display
    int oldDisplayIndex = -1;
    if (display.size() > getDepth()) {
        oldDisplayIndex = display[getDepth()];
    }
    else {
        display.resize(getDepth() + 1, -1);
    }

    // Prepare and push activation record
    int frameStart = runtimeStack.size();
    int dynamicLink = runtimeStack.empty() ? -1 : frameStart - 1;

    runtimeStack.push_back(id);                  // subprogram ID
    runtimeStack.push_back(oldDisplayIndex);     // saved display link
    runtimeStack.push_back(dynamicLink);         // dynamic link

    for (int i = 0; i < numArgs; ++i)
        runtimeStack.push_back(0);               // arguments
    for (int i = 0; i < numLocals; ++i)
        runtimeStack.push_back(0);               // locals

    // Update display entry
    display[getDepth()] = frameStart;

    // Track subprogram on the logical call stack
    activeSubprogramIds.push_back(id);

    printStackAndDisplay();
}



void Sub::return_() {
    cout << "Returning from: " << name << endl;

    if (runtimeStack.empty() || activeSubprogramIds.empty()) {
        cout << "Error: No active subprogram to return from.\n";
        return;
    }

    // Remove this subprogram's activation record
    int frameStart = display[getDepth()];
    if (frameStart == -1) {
        cout << "Error: Display entry for " << name << " is invalid.\n";
        return;
    }

    int recordSize = 3 + numArgs + numLocals;
    runtimeStack.erase(runtimeStack.begin() + frameStart,
        runtimeStack.begin() + frameStart + recordSize);

    // Restore display
    display[getDepth()] = (frameStart < runtimeStack.size())
        ? runtimeStack[frameStart + 1]
        : -1;

    // Remove from active subprogram tracking
    activeSubprogramIds.pop_back();

    printStackAndDisplay();
}



void Sub::printStackAndDisplay() {
    vector<string> stackLines;
    vector<string> displayLines;

    // Print the stack 
    int i = 0;
    while (i < runtimeStack.size()) {
        int sid = runtimeStack[i];
        Sub* sub = idToSub[sid];
        int args = sub->getNumArgs();
        int locals = sub->getNumLocals();
        int frameSize = 3 + args + locals;

        stackLines.push_back("[" + to_string(i) + "] Subprogram ID: " + to_string(sid) + " (" + sub->getName() + ")");
        stackLines.push_back("[" + to_string(i + 1) + "] Saved display link: " + to_string(runtimeStack[i + 1]));
        stackLines.push_back("[" + to_string(i + 2) + "] Dynamic link: " + to_string(runtimeStack[i + 2]));

        for (int j = 0; j < args; ++j) {
            int idx = i + 3 + j;
            stackLines.push_back("[" + to_string(idx) + "] Arg " + to_string(j) + ": " + to_string(runtimeStack[idx]));
        }

        for (int j = 0; j < locals; ++j) {
            int idx = i + 3 + args + j;
            stackLines.push_back("[" + to_string(idx) + "] Local " + to_string(j) + ": " + to_string(runtimeStack[idx]));
        }

        stackLines.push_back("");
        i += frameSize;
    }

    // Print the display 
    for (int d = 0; d < display.size(); ++d) {
        if (display[d] != -1) {
            int sid = runtimeStack[display[d]];
            displayLines.push_back("Level " + to_string(d) + ": " +
                idToSub[sid]->getName() + " (stack pos: " + to_string(display[d]) + ")");
        }
        else {
            displayLines.push_back("Level " + to_string(d) + ": (empty)");
        }
    }

    while (displayLines.size() < stackLines.size())
        displayLines.push_back("");

    // Formatting 
    cout << left;
    cout << "\n--- Runtime Stack and Display (Bottom index 0 at bottom) ---\n";
    cout << setw(35) << "Runtime Stack" << "Display" << endl;
    cout << string(80, '-') << endl;

    for (int idx = (int)stackLines.size() - 1; idx >= 0; --idx) {
        cout << setw(35) << stackLines[idx] << displayLines[idx] << endl;
    }

    cout << string(80, '-') << "\n" << endl;
}