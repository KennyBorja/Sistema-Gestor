#include "../include/diskSpaceManager.h"


bool diskSpaceManager::BuscarBloque(int id) {
    ifstream archivo("bloques.txt");
    string linea;
    
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string parte;
        int idBloque;
        int espacio;
        getline(ss, parte, '|');
        idBloque = stoi(parte);
        getline(ss, parte, '|');
        espacio = stoi(parte);
        if (idBloque == id) {
            page.setPageId(idBloque);
            page.setSize(espacio);
            SetPageContent(idBloque);
            return true;
        }
    }
    return false;
}

void diskSpaceManager::savePageInBlock(Page page, int mode) {
    page.updateHeader();
    RWBloque::ActualizarBloques(page.getPageId(), page.getSize(), mode);
    RWBloque::ActualizarBloque(page.getPageId(),page.getName(),page.getCabeceraSpaciosPage(),page.getContent());
    RWBloque::ActializarSectores(page.getName(),page.getContent(),page.getTamanio(),page.getNumRegistros());
    if (mode==1){
        writeStateBlockReserved(page.getPageId());
    }
    
}


void diskSpaceManager::SetPageContent(int id) {
    ifstream archivo("disk//bloques//" + to_string(id) + ".txt");
    string linea;

    if (getline(archivo, linea)) {
        page.setName(linea);
        if (getline(archivo, linea)) {
            page.setCabeceraSpaciosPage(linea);

            if (getline(archivo, linea)) {
                page.setContent(linea);
            } else {
                page.setContent(""); // Asigna un valor por defecto si no hay tercera línea
            }
        } else {
            page.setCabeceraSpaciosPage(""); // Asigna un valor por defecto si no hay segunda línea
            page.setContent(""); // Asigna un valor por defecto si no hay tercera línea
        }
    } else {
        page.setContent(""); // Asigna un valor por defecto si no hay primera línea
        page.setCabeceraSpaciosPage(""); // Asigna un valor por defecto si no hay segunda línea
        page.setName(""); // Asigna un valor por defecto si no hay tercera línea
    }
}

void diskSpaceManager::printActualPage() {
    page.imprimirPage();
}

Page diskSpaceManager::getPage() {
    return page;
}

void diskSpaceManager::setPage(Page page) {
    this->page = page;
}

void diskSpaceManager::showBlock(int blockID) {
    BuscarBloque(blockID);
    printActualPage();
}


string diskSpaceManager::getpathF(string record) {
    ifstream file("bloques.txt");
    string line;
    string path;

    while (getline(file, line)) {
        // Encontrar la primera aparición de "|"
        size_t pos1 = line.find("|");
        if (pos1 == string::npos) continue;

        // Extraer el primer valor
        path = line.substr(0, pos1);
        

        // Encontrar la segunda aparición de "|"
        size_t pos2 = line.find("|", pos1 + 1);
        if (pos2 == string::npos) continue;

        // Extraer el segundo valor
        string capacityStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int capacity = stoi(capacityStr);

        // Extraer el tercer valor
        string ocupado = line.substr(pos2 + 1);
        int estado = stoi(ocupado);

        int size=record.size();
        if((estado==0)&& size<capacity){
            return path;
        }
    }

    file.close();
    return "";
}

string diskSpaceManager::getpathFecha(string record) {
    ifstream file("bloques.txt");
    string line;
    string path;

    while (getline(file, line)) {
        // Encontrar la primera aparición de "|"
        size_t pos1 = line.find("|");
        if (pos1 == string::npos) continue;

        // Extraer el primer valor
        path = line.substr(0, pos1);
        

        // Encontrar la segunda aparición de "|"
        size_t pos2 = line.find("|", pos1 + 1);
        if (pos2 == string::npos) continue;

        // Extraer el segundo valor
        string capacityStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int capacity = stoi(capacityStr);


        int size=record.size();
        if(size<capacity){
            return path;
        }
    }

    file.close();
    return "";
}


void diskSpaceManager::writeStateBlockReserved(int blockID)
{
    ifstream file("bloques.txt");
    ofstream temp("temp.txt");
    string line;
    string path;
    while (getline(file, line)) {
        // Encontrar la primera aparición de "|"
        size_t pos1 = line.find("|");
        if (pos1 == string::npos) continue;

        // Extraer el primer valor
        path = line.substr(0, pos1);
        

        // Encontrar la segunda aparición de "|"
        size_t pos2 = line.find("|", pos1 + 1);
        if (pos2 == string::npos) continue;

        // Extraer el segundo valor
        string capacityStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int capacity = stoi(capacityStr);

        // Extraer el tercer valor
        string ocupado = line.substr(pos2 + 1);
        int estado = stoi(ocupado);

        if(estado==0){
            if(blockID==stoi(path)){
                temp<<path<<"|"<<capacity<<"|"<<"1"<<endl;
            }else{
                temp<<line<<endl;
            }
        }else{
            temp<<line<<endl;
        }
    }

    file.close();
    temp.close();
    remove("bloques.txt");
    rename("temp.txt","bloques.txt");
    
}




