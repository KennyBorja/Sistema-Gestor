#include "../include/Frame.h"

/* Jose Paredes */
// Constructor
Frame::Frame(int frameID)
    : frameID(frameID), pinCount(0), pinned(false){
}
// Getter for frameID
int Frame::getframeID() {
    return frameID;
}

// Getter for page
Page& Frame::getPage() {
    return page;
}

// Setter for page
void Frame::setPage(Page page) {
    this->page = page;
}

// Getter for dirty flag
bool Frame::isDirty() const {
    std::queue<bool> copy = dirty_flag; // Hacemos una copia de la cola para iterar
    while (!copy.empty()) {
        if (copy.front() == true) {
            return true; // Si encontramos un 'true', devolvemos 'true'
        }
        copy.pop(); // Pasamos al siguiente elemento
    }
    return false; // Si no encontramos ningún 'true', devolvemos 'false'
}

// Setter for dirty flag
void Frame::setDirtyFlag(bool dirtyF) {
    dirty_flag.push(dirtyF); // Añadimos el valor a la cola
}

void Frame::freeDirtyFlag() {
    if (!dirty_flag.empty()) {
        dirty_flag.pop(); // Eliminamos el primer elemento de la cola
        std::cout << "POP";
    }
}

// Getter for pinCount
int Frame::getPinCount() {
    return pinCount;
}

// Increment pinCount
void Frame::incrementPinCount() {
    pinCount++;
}

// Decrement pinCount
void Frame::decrementPinCount() {
    pinCount--;
}

void Frame::setPinned(bool pin) {
    pinned = pin;
}

bool Frame::isPinned() {
    return pinned;
}

vector<string> Frame::extractFixedLengthStrings() {
    return page.extractFixedLengthStrings();
}