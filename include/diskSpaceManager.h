#ifndef DISKSPACEMANAGER_H
#define DISKSPACEMANAGER_H

#include <string>
#include <fstream>
#include <sstream>
#include "Page.h"  // Asegúrate de tener Page.h en el mismo directorio o ajusta el include según corresponda
#include "../RWBLOQUE.h"

using namespace std;

class diskSpaceManager {
public:
    Page page;

    bool BuscarBloque(int id);
    void savePageInBlock(Page page, int mode);
    void SetPageContent(int id);
    void printActualPage();
    Page getPage();
    void setPage(Page page);
    void showBlock(int BlockID);

    string getpathF(string record);     //OBTENER BLOQUE A INSERTAR
    string getpathFecha(string record);    //OBTENER BLOQUE A INSERTAR POR FECHA
    void writeStateBlockReserved(int blockID);    //ESCRIBIR QUE EL BLOQUE YA ESTA USADO POR UN USUARIO
    
};

#endif  // DISKSPACEMANAGER_H
