#include "../include/BufferManager.h"

#include "../include/Frame.h"
#include "../include/Page.h"

// configura LRU (Least Recently Used) con el número total de frames.

BufferManager::BufferManager(int numFrames, char policy) {
    
    this->numFrames = numFrames;
    cout<<"BufferManager creado"<<endl;
    cout<<"Numero de frames: "<<numFrames<<endl;
    this->bpool = BufferPool(numFrames);
    cout<<"BufferPool creado"<<endl;
    this->policy = policy;
    
}



void BufferManager::loadPageFromDisk(Page page, char _mode) {
    bool mode = _mode == 'W' ? true : false;
    cout << "Cargando pagina " << page.getPageId() << " al buffer pool" << endl;
    if (bpool.isPageLoaded(page.getPageId())) {
        cout << "La pagina ya esta cargada" << endl;
        if (mode) {
            bpool.getFrame(bpool.getFrameId(page.getPageId())).setDirtyFlag(true);
        }else {bpool.getFrame(bpool.getFrameId(page.getPageId())).setDirtyFlag(false);}
        bpool.modifyPinInExistingFrame(page.getPageId(), 'i');
        bpool.incrementHistory();
        bpool.printTableFrame();
        return;
    }

    if (policy == 'L') {
        bpool.LRU();
    } else if (policy == 'C') {
        bpool.Clock();
    } else {
        cout << "Politica de reemplazo no valida" << endl;
        return;
    }

    int idFree = bpool.findFreeFrame();
    Frame tempFrame(idFree);
    tempFrame.setDirtyFlag(mode);
    tempFrame.setPage(page);
    tempFrame.incrementPinCount();

    bpool.setPageInFrame(idFree, page.getPageId(), tempFrame);
    bpool.incrementHistory();
    bpool.printTableFrame();
}




void BufferManager::killProcess(int pageID) {
    bpool.modifyPinInExistingFrame(pageID, 'k');
    // bpool.incrementHistory();
    bpool.printTableFrame();
}

void BufferManager::printTableFrame() {
    bpool.printTableFrame();
}

void BufferManager::pinPage(int pageID) {
    bpool.modifyPinInExistingFrame(pageID, 'p');
}

void BufferManager::unpinPage(int pageID) {
    bpool.modifyPinInExistingFrame(pageID, 'u');
}

bool BufferManager::setRecord(int pageID, string record) {
    
    Frame& frame = bpool.getFrame(bpool.getFrameId(pageID));
    
    if(!frame.isDirty()) {
        bpool.setDirty(pageID);
    }

    bool success = frame.getPage().addRecordInContent(record);

     if (!success) {
        return false;
    } else {
        std::cout << "Registro añadido exitosamente a la página con ID " << pageID << "." << std::endl;
        return true;
    }
}

void BufferManager::setContentInFreeList(int pageID, string record) {
    Frame& frame = bpool.getFrame(bpool.getFrameId(pageID));
    if(frame.getPinCount() == 0) {
        bpool.modifyPinInExistingFrame(pageID, 'i');
    }
    frame.getPage().SetRecordtInEmptyPoscition(record);
}

void BufferManager::printAtribsPage(int pageID) {
    Frame& frame = bpool.getFrame(bpool.getFrameId(pageID));
    frame.getPage().imprimirPage();
}

bool BufferManager::hayEspaciosVacios(int pageID) {
    Frame& frame = bpool.getFrame(bpool.getFrameId(pageID));
    return frame.getPage().isPosicionesVaciasEmpty();
}

Page BufferManager::rescuePage(int PageID)
{
    Frame &frame = bpool.getFrame(bpool.getFrameId(PageID));
    return frame.getPage();
}

Page BufferManager::rescueAndCleanPage(int PageID) {
    Frame &frame = bpool.getFrame(bpool.getFrameId(PageID));
    Page page = frame.getPage();
    page.cleanPage();
    return page;
}



bool BufferManager::isPageDir(int pageID) {
    return bpool.isFrameDirty(pageID);
}

void BufferManager::deleteReg(int BlockID, string target, string itemToSearch, string schema){
    Frame &frame = bpool.getFrame(bpool.getFrameId(BlockID));
    int was =frame.getPage().DeleteTarjetRecord(target, itemToSearch, schema);
    if(was!=-1){
        bpool.setDirty(BlockID);
    }
    if (was == -1){
        cout<<"Registro no encontrado"<<endl;
    }
    
}

void BufferManager::updateHeader(int pageID){
    Frame &frame = bpool.getFrame(bpool.getFrameId(pageID));
    frame.getPage().updateHeader();
}


bool BufferManager::isBlockloaded(int pageId){
    return bpool.isPageLoaded(pageId);
}


vector <int> BufferManager::getPagesId(){
    return bpool.getPagesId();
}

vector<string> BufferManager::extractFixedLengthStrings(int pageID) {
    vector<string> result;
    Frame &frame = bpool.getFrame(bpool.getFrameId(pageID));
    result = frame.getPage().extractFixedLengthStrings();

    return result;
}

int BufferManager::getEmptyPosition(int pageID){
    Frame &frame = bpool.getFrame(bpool.getFrameId(pageID));
    return frame.getPage().getEmptyPosition();

    
}
int BufferManager::getnumReg(int pageID){
    Frame &frame = bpool.getFrame(bpool.getFrameId(pageID));
    return frame.getPage().getNumRegistros();
}