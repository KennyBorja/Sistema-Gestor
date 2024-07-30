#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../include/BufferPool.h"
#include "../include/Frame.h"
#include "../include/diskSpaceManager.h"

using namespace std;

BufferPool::BufferPool() {
    this->numFrames = 0;
    this->capacity = 2048;
    this->clock_pointer = 0;
}

BufferPool::BufferPool(int numFrames) {
    this->numFrames = numFrames;
    this->capacity = 2048;
    this->clock_pointer = 0; // Inicializar el puntero del reloj
    cout<<"BufferPool"<<endl;
    int cantidadFrames = numFrames;
    for (int i = 0; i < numFrames; i++) {
        cout<<i<<endl;
        Frame frame(i);
        this->frames.push_back(frame);
        page_table[i] = -1;
        use_bits.push_back(false); // Inicializar los bits de uso
        cout<<i<<endl;

    }
    cout<<"BufferPool creado"<<endl;
}

vector<Frame> &BufferPool::getFrames() { return frames; }

Frame &BufferPool::getFrame(int frameID) {
    for (int i = 0; i < frames.size(); i++) {
        if (frames[i].getframeID() == frameID) {
            return frames[i];
        }
    }
    throw std::runtime_error("Frame not found");
}

void BufferPool::modifyPinInExistingFrame(int pageID, char flag) {
    for (int i = 0; i < frames.size(); i++) {
        if (page_table[frames[i].getframeID()] == pageID) {
            if (flag == 'i') {
                frames[i].incrementPinCount();
            } else if (flag == 'k' && frames[i].getPinCount() > 0) {
                frames[i].decrementPinCount();
                frames[i].freeDirtyFlag();
            } else if (flag == 'p') {
                frames[i].setPinned(true);
            } else if (flag == 'u') {
                frames[i].setPinned(false);
            } else {
                cout << "Flag no valido" << endl;
                
            }
        }
    }
}

void BufferPool::setPage(Frame &frame, int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
        frames[frameID] = frame;
        page_table[frameID] = 1;
    } else {
        cout << "SetP: Frame ID fuera de rango" << endl;
        return;
    }
    cout << "\nPagina [" << frameID << "] cargada correctamente." << endl;
}

void BufferPool::printPage(int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
        if (page_table[frameID] == 1) {
            Frame &frame = frames[frameID];
            cout << "Frame ID: " << frameID << endl;
            cout << "Dirty Flag: " << "(frame.isDirty() ? \" true \" : \" false \")" << endl;
            cout << "Pin Count: " << "frame.getPinCount()" << endl;
            cout << "Page Capacity: " << "frame.getPage().getPageCapacity()" << endl;
            cout << "Page Content: " << "frame.getPage().getPageContent() " << endl;
        } else {
            cout << "Frame ID: " << frameID << " no contiene una página." << endl;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

void BufferPool::addRecord(int frameID, string record) {
    if (frameID >= 0 && frameID < numFrames) {
        if (page_table[frameID] == 1) {
            Frame &frame = frames[frameID];
            // frame.getPage().addRecord(record);
            cout << "Registro agregado correctamente." << endl;
        } else {
            cout << "Frame ID: " << frameID << " no contiene una página." << endl;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

void BufferPool::deleteRecord(int frameID, string record) {
    if (frameID >= 0 && frameID < numFrames) {
        if (page_table[frameID] == 1) {
            Frame &frame = frames[frameID];
            // frame.getPage().deleteRecord(record);
            cout << "Registro eliminado correctamente." << endl;
        } else {
            cout << "Frame ID: " << frameID << " no contiene una página." << endl;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

void BufferPool::pageIsDirty(int pageID) { //Jose Paredes : No se utilizo
    if (pageID >= 0 && pageID < numFrames) {
        if (page_table[pageID] == 1) {
            Frame &frame = frames[pageID];
            // return frame.isDirty();
            return;
        } else {
            cout << "Frame ID: " << pageID << " no contiene una página." << endl;
            return;
        }
    } else {
        cout << "Frame ID fuera de rango." << endl;
        return;
    }
}

void BufferPool::freeFrame(int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
        frames[frameID] = Frame(frameID);
        page_table[frameID] = -1;
        cout << "Frame [" << frameID << "] liberado correctamente." << endl;
    } else {
        cout << "Frame ID fuera de rango." << endl;
    }
}

int BufferPool::findFreeFrame() {
    for (auto it = page_table.begin(); it != page_table.end(); ++it) {
        if (it->second == -1) 
        {   
            use_bits[it->first] = true; // Marcar el bit de uso
            return it->first;
        }
    }

    /*Si retorna -2 es porque todos los frames ya tienen cargado una pagina*/
    return -2;
}

void BufferPool::setPageInFrame(int frameID, int pageID, Frame &frame) {
    if (frameID >= 0 && frameID < numFrames) {
        frames[frameID] = frame;
        page_table[frameID] = pageID;
    } else {
        cout << "SetP: Frame ID fuera de rango" << endl;
        return;
    }

    cout << "\nPagina [" << frameID << "] cargada Correctamente\n"
         << endl;
    //setHistory(pageID); // Guardar el historial de la página
}

bool BufferPool::isPageLoaded(int pageID) {
    for (const auto &pair : page_table) {
        if (pair.second == pageID) {
            use_bits[pair.first] = true;
            return true;
        }
    }
    return false;
}

int BufferPool::getFrameId(int pageID) {
    for (const auto &pair : page_table) {
        if (pair.second == pageID) {
            return pair.first;
        }
    }
    return -1;
}

void BufferPool::printTableFrame() {
    cout << setw(10) << "Frame Id" << "\t"
         << setw(10) << "Page Id" << "\t"
         << setw(10) << "Dirty Bit" << "\t"
         << setw(10) << "Pin Count" << "\t"
         << setw(10) << "Last Used" <<"\t" 
         << setw(10) << "bit" <<"\t"
         << setw(10) << "pinned" << endl;
         

    for (int i = 0; i < frames.size(); i++) {
        cout << setw(10) << frames[i].getframeID() << "\t"
             << setw(10) << frames[i].getPage().getPageId() << "\t"
             << setw(10) << (frames[i].isDirty() ? "Yes" : "No") << "\t"
             << setw(10) << frames[i].getPinCount() << "\t"
             << setw(10) << history[frames[i].getframeID()] << "\t"
             << setw(10) << use_bits[i] << "\t"
             << setw(10) << frames[i].isPinned() << endl;
    }

    
}

void BufferPool::setHistory(int pageID) {
    history[pageID] = 0;
}

void BufferPool::incrementHistory() {
    for (auto &pair : page_table) {
        if (pair.second != -1) history[pair.first]++;
    }
}



void BufferPool::LRU() {
    cout << "LRU" << endl;
    /*
    En el unordered_map history, se guardará el frameID y el tiempo en el que fue accedido. Atraves de las veces que se llame en general con +1.
    Ahora se buscara cual frame tiene mas tiempo en estar. Luego se revisara si el pin count esta en cero y ahi recien se llamara la funcion freeFrame.
    */
    int max = 0;
    int lastUsedFrame = 0;
    for (auto &pair : history) {
        if (pair.second > max) {
            max = pair.second;
            lastUsedFrame = pair.first;
        }
    }
    if (frames[lastUsedFrame].getPinCount() == 0) {
        history[lastUsedFrame] = 0;
        freeFrame(lastUsedFrame);
    } else {
        cout << "No se puede liberar el frame " << lastUsedFrame << " porque tiene un pin count mayor a 0" << endl;
    }
}

void BufferPool::Clock() {
    int iterations = 0; // Contador de iteraciones

    while (iterations < numFrames*2) { // Limitar el número de iteraciones a numFrames
        cout << "----entrada---"<<clock_pointer<< "------------" << endl;

        if (frames[clock_pointer].isPinned() == true) {
            // Si el frame está pinneado, poner el bit de uso a 1 y avanzar el puntero
            use_bits[clock_pointer] = true;
             clock_pointer = (clock_pointer + 1) % numFrames;
            continue;
        }
        
        if (!use_bits[clock_pointer] && frames[clock_pointer].getPinCount() == 0) {
            // Si el bit de uso es 0 y el pin count es 0, reemplazar este frame
            freeFrame(clock_pointer);
            clock_pointer = (clock_pointer + 1) % numFrames;
            return;
        } else {
            cout<<use_bits[clock_pointer]<<endl;
            // Si el bit de uso es 1, ponerlo a 0 y avanzar el puntero
            use_bits[clock_pointer] = false;
            clock_pointer = (clock_pointer + 1) % numFrames;
        }

        iterations++; // Incrementar el contador de iteraciones
    }

    // Si se inspeccionaron todos los frames y no se encontró ninguno para reemplazar
    if (iterations >= numFrames) {
        while(true){
            if(frames[clock_pointer].isPinned() == true){
                clock_pointer = (clock_pointer + 1) % numFrames;
                continue;
            }
             char response;
                cout << "El frame " << clock_pointer << " tiene un pin count mayor a 0. ¿Desea liberarlo? (y/n): ";
                cin>>response;
                

                if(response == 'y')
                {
                    diskSpaceManager disk;
                    disk.savePageInBlock(frames[clock_pointer].getPage(),1);

                      while(frames[clock_pointer]. getPinCount() > 0) //AGREGUE ESTO
                        freeFrameQueue(clock_pointer);  //AGRUEGUE ESTO


                    freeFrame(clock_pointer);
                    clock_pointer = (clock_pointer + 1) % numFrames;
                    return;
                }
                else
                {
                    clock_pointer = (clock_pointer + 1) % numFrames;
                    continue;
                }

        }
    }
    
    cout << "--salida-----"<<clock_pointer<< "------------" << endl;
}


bool BufferPool::isFrameDirty(int pageID)
{
    for (int i = 0; i < frames.size(); i++)
    {
        if (page_table[frames[i].getframeID()] == pageID)
        {
            return frames[i].isDirty();
        }
    }
}

void BufferPool::setDirty(int pageID)
{
    for (int i = 0; i < frames.size(); i++)
    {
        if (page_table[frames[i].getframeID()] == pageID)
        {
            frames[i].setDirtyFlag(true);
        }
    }
}


void BufferPool::freeFrameQueue(int frameID) {
    if (frameID >= 0 && frameID < numFrames) {
       
        frames[frameID].freeDirtyFlag();
        frames[frameID].decrementPinCount();
        cout << "PinCount Frame " << frameID << "es:" <<frames[frameID].getPinCount() << endl;
        cout << "Dirty bit Frame " << frameID << "es:" <<frames[frameID].isDirty ()<< endl;

    }
    else
    {
        cout << "Frame ID fuera de rango." << endl;
    }
}


int BufferPool::getPincount(int pageID)
{
    for (int i = 0; i < frames.size(); i++)
    {
        if (page_table[frames[i].getframeID()] == pageID)
        {
            return frames[i].getPinCount();
        }
    }
}

vector<int> BufferPool::getPagesId()
{
    vector<int> pagesId;
    for (int i = 0; i < frames.size(); i++)
    {
        if (page_table[frames[i].getframeID()] != -1)
        {
            pagesId.push_back(page_table[frames[i].getframeID()]);
        }
    }
    return pagesId;
}