#ifndef FRAME_H
#define FRAME_H

#include "Page.h"
#include <queue>
#include <iostream>

class Frame {
   private:
    int frameID;
    queue<bool> dirty_flag;
    int pinCount;
    Page page;
    bool pinned;
    
   public:
    // Constructor
    Frame(int frameID);


    // Getters
    int getframeID();
    bool isDirty () const;
    int getPinCount();
    bool isPinned();
    

    Page& getPage();

    // Setters
    void setFrameID(int id);
    void setDirtyFlag(bool dirty);
    void freeDirtyFlag();
    void setPinCount(int pin);
    void setPage(Page pg);
    void incrementPinCount();
    void decrementPinCount();
    void setPinned(bool pin);
    
    vector<string>extractFixedLengthStrings();
};

#endif  // FRAME_H
