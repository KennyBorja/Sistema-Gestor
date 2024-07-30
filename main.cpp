#include <cstdlib>
#include <windows.h>

#include "regFijos.h"
#include "RWesquema.h"

#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>

#include "include/BufferManager.h"
#include "include/page.h"
#include "include/diskSpaceManager.h"
#include "include/BPlusTree.h"

using namespace std;

//lee y extrae rutas
class FileManager {
public:

        vector<string> bloquePesoAcargar(int pesoRegistro, string nombreSquema) {
            ifstream archivo("bloques.txt");
            string linea;
            vector<string> result(3); // Inicializamos un vector de tamaño 3

            while (getline(archivo, linea)) {
                stringstream ss(linea);
                string parte;
                string nombreBloque;
                int pesoBloque;
                string esquemaEnBloque;

                // Leer nombre del bloque
                if (getline(ss, parte, '|')) {
                    nombreBloque = parte;
                } else {
                    continue;
                }

                // Leer peso del bloque
                if (getline(ss, parte, '|')) {
                    pesoBloque = stoi(parte);
                } else {
                    continue;
                }

                // Leer esquema en bloque (si existe)
                if (getline(ss, parte, '|')) {
                    esquemaEnBloque = parte;
                } else {
                    esquemaEnBloque = "";
                }

                // Verificar condiciones
                if ((esquemaEnBloque == nombreSquema || esquemaEnBloque == "") && pesoBloque >= pesoRegistro) {
                    result[0] = nombreBloque;
                    result[1] = to_string(pesoBloque);
                    result[2] = esquemaEnBloque;
                    return result;
                }
            }

            // Si no se encontró ningún bloque que cumpla las condiciones, retornar un vector vacío
        return vector<string>();
    }

         bool verificaresquema(string esquema)
    {
        ifstream archivo("esquemas.txt");
        string linea;
        size_t pos = 0;
        string token;
        while (getline(archivo, linea))
        {

            pos = linea.find('#', pos);
            token = linea.substr(0, pos);
            pos = 0; // reiniciar la pocicion
            cout << endl;
            if (token == esquema)
            {
                return true;
            }
        }
        return false;
    }

        string extraerEsquema(const string &nombreTabla)
        {
            string fila;
            ifstream archivoExistente("esquemas.txt");

            if (!archivoExistente.is_open())
            {
                cerr << "Error al abrir el archivo 'esquemas.txt'." << endl;
                return fila;
            }

            string linea;
            while (getline(archivoExistente, linea))
            {
                size_t indice = linea.find('#');
                if (indice != string::npos)
                {
                    string nombreTablaExistente = linea.substr(0, indice);
                    if (nombreTablaExistente == nombreTabla)
                    {
                        // Extraer los tipos de datos de la línea
                        stringstream ss(linea.substr(indice + 1)); // Saltar el nombre de la tabla
                        getline(ss, fila);                         // Leer toda la línea desde el stringstream
                        break;                                     // Salir del bucle una vez que se ha encontrado la línea
                    }
                }
            }

            archivoExistente.close();
            return fila;
        }

        void IngresarEsquema(string &nombre)
        {
            string nombreColumna;
            string tipoDato;
            string peso;

            std::ofstream archivo("esquemas.txt", std::ios::app); // Abre en modo append

            if (archivo.is_open())
            {
                int continua = 1;
                std::string esquema;
                esquema = nombre;

                while (continua == 1)
                {
                    cout << "Ingrese el nombre de la columna: ";
                    cin >> nombreColumna;

                    cout << "Ingrese el tipo: ";
                    cin >> tipoDato;

                    cout << "Ingrese cantidad: ";
                    cin >> peso;

                    // Concatenar los datos de fila
                    esquema += "#" + nombreColumna + "#" + tipoDato + "#" + peso;

                    cout << "¿Desea agregar otra columna? (1 = sí, 0 = no): ";
                    cin >> continua;

                    while (continua != 0 && continua != 1)
                    {
                        cout << "Por favor, ingrese 1 si desea continuar o 0 si desea salir: ";
                        cin.clear();
                        cin >> continua;
                    }
                }
                archivo << esquema << std::endl;

                archivo.close();
            }
            else
            {
                std::cerr << "Error al abrir el archivo 'esquema.txt'" << std::endl;
            }
        }

        void GuardarEsquemaDesdeArchivo(string &nombreTabla)
        {
            string tipoDato;
            string resultado = nombreTabla.substr(0, nombreTabla.size() - 4);

            ifstream archivo(nombreTabla);

            if (!archivo.is_open())
            {
                cerr << "Error al abrir el archivo '" << nombreTabla << "'." << endl;
                return; // Devolver un string vacío si no se puede abrir el archivo
            }

            ofstream archivoSalida("esquemas.txt", ios::app);

            string linea;
            getline(archivo, linea);
            istringstream ss(linea);
            string base;
            int count = 0;
            int max = 0;

            getline(archivo, linea);
            istringstream sss(linea);
            string base2;

            while (getline(ss, base, ';') && getline(sss, base2, ';'))
            {
                resultado += "#" + base;
                tipoDato = RWhelper::determinarTipoDato(base2);

                //obtener la longitud maxima de cada columna
                max = RWhelper::MayorLongitudCol(nombreTabla, count);
                if (tipoDato == "int" && max < 100000000)
                {
                    resultado += "#int#8";
                }
                else
                {
                    resultado += "#" + tipoDato + "#" + to_string(max);
                }
                count++;
            }

            archivo.close();
            archivoSalida << resultado << '\n';
            archivoSalida.close();
}

        
};

//crea disco carpetas y txt
class SimuladorDisco
{
public:
    int num_platos;
    int num_caras;
    int num_pistas;
    int num_sectores;
    int tamanioSector;
    int sectxBloque;

    SimuladorDisco()
        : num_platos(3), num_caras(2), num_pistas(4), num_sectores(4), tamanioSector(512), sectxBloque(4)
    {
        long long tamDiskoBytes = num_platos * num_caras * num_pistas * num_sectores * tamanioSector;
        double tamDiskokB = static_cast<double>(tamDiskoBytes) / 1024; // Convertir bytes a kilobytes

        cout << "Tamaño total del disco: " << tamDiskokB << " kB" << std::endl;
        guardarCaracteristicasDisco();
        crearCarpetas();
        bloque();
        mostrarCaracteristicasDisco();

    }

    SimuladorDisco(int platos, int caras, int pistas, int sectores, int tamSector, int sectoresBloque)
        : num_platos(platos), num_caras(caras), num_pistas(pistas), num_sectores(sectores), tamanioSector(tamSector), sectxBloque(sectoresBloque)
    {
        long long tamDiskoBytes = num_platos * num_caras * num_pistas * num_sectores * tamanioSector;
        double tamDiskokB = static_cast<double>(tamDiskoBytes) / 1024; // Convertir bytes a kilobytes

        cout << "Tamaño total del disco: " << tamDiskokB << " kB" << std::endl;
        guardarCaracteristicasDisco();
        crearCarpetas();
        bloque();
        mostrarCaracteristicasDisco();
        cout<<"terminancoo el disco-----------------------------------";
    }

    void crearCarpetas()
    {
        string nombredisko = "disk";

        if (existeCarpeta(nombredisko))
        {
            cout << "La carpeta '" << nombredisko << "' ya existe." << endl;
            return; // Salir de la función si la carpeta ya existe
        }

        crearCarpeta(nombredisko);
        for (int plato = 1; plato <= num_platos; ++plato)
        {
            string nombreCarpetaPlato = nombredisko + "\\Plato" + std::to_string(plato);
            crearCarpeta(nombreCarpetaPlato);

            for (int cara = 1; cara <= num_caras; ++cara)
            {
                string nombreCarpetaCara = nombreCarpetaPlato + "\\Cara" + std::to_string(cara);
                crearCarpeta(nombreCarpetaCara);

                for (int pista = 1; pista <= num_pistas; ++pista)
                {
                    string nombreCarpetaPista = nombreCarpetaCara + "\\Pista" + std::to_string(pista);
                    crearCarpeta(nombreCarpetaPista);

                    for (int sector = 1; sector <= num_sectores; ++sector)
                    {
                        string nombreArchivoSector = nombreCarpetaPista + "\\Sector" + std::to_string(sector) + ".txt";
                        crearArchivo(nombreArchivoSector);
                    }
                }
            }
        }
    }

    void crearCarpeta(const std::string &nombre)
    {
        if (CreateDirectoryA(nombre.c_str(), NULL) != 0)
        {
            // std::cout << "Carpeta '" << nombre << "' creada correctamente." << std::endl;
        }
        else
        {
            DWORD error = GetLastError();
            if (error == ERROR_ALREADY_EXISTS)
            {
                std::cout << "La carpeta '" << nombre << "' ya existe." << std::endl;
            }
            else
            {
                std::cerr << "Error al crear la carpeta '" << nombre << "'. Código de error: " << error << std::endl;
            }
        }
    }

    void crearArchivo(const std::string &nombre)
    {
        ofstream archivo(nombre);
        if (archivo.is_open())
        {
            archivo << string(tamanioSector, '0'); // Escribir 1024 '0' para simular el tamaño del sector
            archivo.close();
        }
        else
        {
            cerr << "Error al crear el archivo '" << nombre << "'" << endl;
        }
    }

    bool existeCarpeta(const string &nombre)
    {
        DWORD atributos = GetFileAttributesA(nombre.c_str());
        return (atributos != INVALID_FILE_ATTRIBUTES && (atributos & FILE_ATTRIBUTE_DIRECTORY));
    }

    void caracteristicasDisco()
    {
        ofstream archivo("caracteristicasDisco.txt");
        string linea = to_string(tamanioSector) + "-" + to_string(sectxBloque);
        linea += "-" + to_string(num_platos) + "-" + to_string(num_caras) + "-" + to_string(num_pistas) + "-" + to_string(num_sectores);
        archivo << linea;
        archivo.close();
    }
    
    void bloque()
    {
        string carpetaBloques = "disk\\bloques";

        if (existeCarpeta(carpetaBloques))
        {
            cout << "La carpeta 'bloques' ya existe." << endl;
            return;
        }

        crearCarpeta(carpetaBloques);

        ifstream verificarArchivo("bloques.txt");
        if (verificarArchivo.good())
        {
            cout << "El archivo 'bloques.txt' ya existe." << endl;
            return;
        }

        ofstream archivoBloques("bloques.txt");
        string bloque_actual;
        int contador_bloque = 0;
        int suma_bloque = 0;
        int b = 0;

        for (int sector = 1; sector <=num_sectores; ++sector)
        {
            for (int pista = 1; pista <= num_pistas; ++pista)
            {
                for (int cara = 1; cara <=num_caras; ++cara)
                {
                    for (int plato = 1; plato <=num_platos; ++plato)
                    {
                        string linea = to_string(plato) + "-" + to_string(cara) + "-" + to_string(pista) + "-" + to_string(sector);
                        bloque_actual += linea + "|";
                        suma_bloque += tamanioSector;
                        contador_bloque++;

                        if (contador_bloque == sectxBloque)
                        {
                            string nombreBloque = carpetaBloques + "\\" + to_string(b) + ".txt";
                            archivoBloques << to_string(b) + "|" + to_string(suma_bloque) + "|"+to_string(0)+"\n";

                            ofstream archivoBloque(nombreBloque);
                            if (archivoBloque.is_open())
                            {
                                archivoBloque <<  bloque_actual.substr(0, bloque_actual.length() - 1) << endl;
                                archivoBloque.close();
                            }

                            bloque_actual = "";
                            suma_bloque = 0;
                            contador_bloque = 0;
                            b++;
                        }
                    }
                }
            }
        }

        if (!bloque_actual.empty())
        {
            string nombreBloque = carpetaBloques + "\\bloque" + to_string(b) + ".txt";
            archivoBloques << "bloque" + to_string(b) + "|" + to_string(suma_bloque) + "\n";

            ofstream archivoBloque(nombreBloque);
            if (archivoBloque.is_open())
            {
                archivoBloque << to_string(suma_bloque) + "|" + bloque_actual.substr(0, bloque_actual.length() - 1) << endl;
                archivoBloque.close();
            }
        }

        caracteristicasDisco();
        archivoBloques.close();
    }

    void guardarCaracteristicasDisco()
    {
        ofstream archivo("caracteristicasDisco.txt");
        string linea = to_string(tamanioSector) + "-" + to_string(sectxBloque);
        linea += "-" + to_string(num_platos) + "-" + to_string(num_caras) + "-" + to_string(num_pistas) + "-" + to_string(num_sectores);
        archivo << linea;
        archivo.close();
    }

   static void mostrarCaracteristicasDisco()
    {
        ifstream archivo("caracteristicasDisco.txt");
        string linea;
        stringstream ss;
        getline(archivo, linea);
        ss << linea;
        int tamanioSector, sectxBloque, num_platos, num_caras, num_pistas, num_sectores;
        char delim = '-';
        string token;

        getline(ss, token, delim);
            tamanioSector = stoi(token);
        getline(ss, token, delim);
            sectxBloque = stoi(token);
        getline(ss, token, delim);
            num_platos = stoi(token);
        getline(ss, token, delim);
            num_caras = stoi(token);
        getline(ss, token, delim);
            num_pistas = stoi(token);
        getline(ss, token, delim);
            num_sectores = stoi(token);
        
        cout<<"Tamaño del sector: "<<tamanioSector<<"/t";
        cout<<"Sectores por bloque: "<<sectxBloque<<"/t";
        cout<<"Número de platos: "<<num_platos<<"/t";
        cout<<"Número de caras: "<<num_caras<<"/t";
        cout<<"Número de pistas: "<<num_pistas<<"/t";
        cout<<"Número de sectores: "<<num_sectores<<"/t";
        archivo.close();
    }

};



class DatabaseManagementSystem {
private:
    
    
    FileManager fileManager;

    BufferManager BfManger;
    diskSpaceManager diskManager;
    BPlusTree tree;



public:

   DatabaseManagementSystem(int frames = 4, char policy = 'C') 
        : BfManger(frames, policy), fileManager(), diskManager(), tree(3) {
    }


      void addRegistrosBloque(string nombreDoc , int cantidad, int mode) {
        std::ifstream archivoStream(nombreDoc);

        if (!archivoStream.is_open()) {
            std::cerr << "Error abriendo el archivo " << nombreDoc << std::endl;
            return;
        }

        std::string nombreEsq = nombreDoc.substr(0, nombreDoc.size() - 4);
        std::string esquema = RWesquema::extraerEsquema(nombreEsq);

        // Saltar la primera línea del archivo CSV (cabecera)
        std::string linea;
        std::getline(archivoStream, linea);

        int registrosInsertados = 0;

        while (registrosInsertados < cantidad) {
            std::string reg = RegistroFijo::guardarRegDesdeCsv(archivoStream, esquema);//obtener registros y avanzar
            if(mode==1){
                ingresarRegistro(reg, nombreEsq);   //ingresar registro y cargar bloque
            }
            if (mode==2){
                ingresarRegistroXFecha(reg, nombreEsq);   //ingresar registro y cargar bloque
            }

            if (mode ==3){
                ingresarRegistrosTitanic(reg, nombreEsq);
            }

            registrosInsertados++;
        }

        archivoStream.close();

        if (registrosInsertados < cantidad) {
            std::cout << "Se añadieron solo " << registrosInsertados << " registros de " << cantidad << " solicitados." << std::endl;
        } else {
            std::cout << "Se añadieron los " << cantidad << " registros solicitados." << std::endl;
        }
       
    }


    void ingresarRegistro(string record, string nombreTabla){
        
        int key; //TODO PONER FUNCION PARA EXTRER KEY INT
        int pos =1;
        key=stoi(RegistroFijo::extraerKey(nombreTabla, record,pos));
        //cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<key<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
        string path;
        path=tree.search(key);
        
        //caso en que no se encuentra en arbol

        if(path=="empty"||path=="Nfound"){ //no esta asignar bloque y cargar y meter
            path=diskManager.getpathF(record); //obtener la ruta
            tree.insert(key,path);//insertar en arbol
            tree.printLeafPaths(); //imoprimir arbol
            diskManager.BuscarBloque(stoi(path)); //seteo page 
            BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer
            diskManager.writeStateBlockReserved(stoi(path)); //escribo que el bloque ya esta ocupado
            BfManger.printAtribsPage(stoi(path)); //imprimo el contenido
            //introducir registro
            if (!record.empty() && BfManger.setRecord(stoi(path), record)) {
                std::cout << "Registro añadido correctamente" << std::endl;
            }

        }

        //caso que si esta en arbol
        else{

            //si esta cargado
            if(BfManger.isBlockloaded(stoi(path))){

                if(!BfManger.hayEspaciosVacios(stoi(path))){
                    
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }
                else if (!record.empty() && BfManger.setRecord(stoi(path), record)) {
                    BfManger.updateHeader(stoi(path));
                    std::cout << "Registro añadido correctamente" << std::endl;
                } else {
                    std::cout << "No se pudo añadir el registro, página llena o error." << std::endl;
                }
            }
            //si no esta cargado
            else {
               
                tree.insert(key,path);
                diskManager.BuscarBloque(stoi(path)); //seteo page 
                BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer
                BfManger.setRecord(stoi(path), record);//añado registro
                BfManger.printAtribsPage(stoi(path)); //imprimo el contenido
            }
        }
    }

    
    void ingresarRegistroXFecha(string record, string nombreTabla)
    {
        
        int key; //TODO PONER FUNCION PARA EXTRER KEY INT por dia
        string diaKey=RegistroFijo::extraerKey(nombreTabla, record,4);

        key=stoi(diaKey.substr(0,2));
        string path;
        path=tree.search(key);
        
        //caso en que no se encuentra en arbol

        if(path=="empty"||path=="Nfound"){ //no esta asignar bloque y cargar y meter
            path=diskManager.getpathFecha(record); //obtener la ruta

            if(BfManger.isBlockloaded(stoi(path))){//si el bloque esta cargado

                if(!BfManger.hayEspaciosVacios(stoi(path))){ //si hay espacios vacios
                   
                    tree.insert(key,path);
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }
                else if (BfManger.setRecord(stoi(path), record)) {
                    BfManger.updateHeader(stoi(path));
                    //int pos = BfManger.getnumReg(stoi(path));
                    //string pathConUbicacion=path+"-"+to_string(pos);
                    tree.insert(key,path);//insertar en arbol
                    std::cout << "Registro añadido correctamente" << std::endl;
                    return;
                } else {
                    savePage(stoi(path));
                    //path=diskManager.getpathFecha(record); //obtener la nueva ruta
                    ingresarRegistroXFecha(record,nombreTabla);
                    return;

                }
            }

            else{   //si el bloque no esta cargado
                tree.insert(key,path);
                diskManager.BuscarBloque(stoi(path)); //seteo page 
                BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer
                
                if(!BfManger.hayEspaciosVacios(stoi(path))){ //si hay espacios vacios
                    tree.insert(key,path);
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }
                else if (BfManger.setRecord(stoi(path), record)) { //caso contrario
                    BfManger.updateHeader(stoi(path));
                    tree.insert(key,path);//insertar en arbol
                    std::cout << "Registro añadido correctamente" << std::endl;
                    return;
                }

            }
            
        }

        //caso que si esta en arbol
        else{

            vector<string> paths= tree.getPathsForKey(key);
            path=paths.back();

            //si esta cargado
            if(BfManger.isBlockloaded(stoi(path))){

                if(!BfManger.hayEspaciosVacios(stoi(path))){
                    
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }

                else if (!record.empty() && BfManger.setRecord(stoi(path), record)) {
                    BfManger.updateHeader(stoi(path));
                    std::cout << "Registro añadido correctamente" << std::endl;
                } else {
                    savePage(stoi(path));
                    path=diskManager.getpathFecha(record); //obtener la ruta
                    tree.insert(key,path);//insertar en arbol
                    ingresarRegistroXFecha(record,nombreTabla);
                    return;

                }
            }

            //si no esta cargado
            else {
                
                diskManager.BuscarBloque(stoi(path)); //seteo page 
                BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer

                if(!BfManger.hayEspaciosVacios(stoi(path))){
                    
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }

                else if (!record.empty() && BfManger.setRecord(stoi(path), record)) {
                    BfManger.updateHeader(stoi(path));
                    std::cout << "Registro añadido correctamente" << std::endl;
                    return;
                } else {
                    savePage(stoi(path));
                    path=diskManager.getpathFecha(record); //obtener la ruta
                    tree.insert(key,path);//insertar en arbol
                    ingresarRegistroXFecha(record,nombreTabla);
                    return;

                }
            }
            
        }

    }


    void ingresarRegistrosTitanic(string record, string nombreTabla){
         string key;

    

        string dato=RegistroFijo::extraerKey(nombreTabla, record,4);
        string datoToConvert=dato.substr(0,3);

        int integerRepresentation = RegistroFijo::nameToInteger(datoToConvert);


        key=tree.search(integerRepresentation); //todo extrermo la ruta 0-1

        if(key=="empty"||key=="Nfound"){ //no esta asignar bloque y cargar y meter
            
            string path=diskManager.getpathFecha(record); //obtener la ruta
            string bloque=path;
            cout<<"path<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<path<<endl;

            //si esta cargado
            if(BfManger.isBlockloaded(stoi(path))){

                if(!BfManger.hayEspaciosVacios(stoi(path))){
                    
                    int ubucacion=BfManger.getEmptyPosition(stoi(path));
                    bloque+="-"+to_string(ubucacion);
                    tree.insert(integerRepresentation,bloque);
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }

                else if (BfManger.setRecord(stoi(path), record)) {
                    int numReg=BfManger.getnumReg(stoi(path));
                    bloque+="-"+to_string(numReg);
                    tree.insert(integerRepresentation,bloque);
                    BfManger.updateHeader(stoi(path));
                    std::cout << "Registro añadido correctamente" << std::endl;
                } else {
                    savePage(stoi(path));
                    path=diskManager.getpathFecha(record); //obtener la ruta
                    ingresarRegistrosTitanic(record,nombreTabla);
                    return;

                }
            }

            //si no esta cargado
            else {
                
                diskManager.BuscarBloque(stoi(path)); //seteo page 
                BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer

                if(!BfManger.hayEspaciosVacios(stoi(path))){
                    int ubucacion=BfManger.getEmptyPosition(stoi(path));
                    bloque+="-"+to_string(ubucacion);
                    tree.insert(integerRepresentation,bloque);
                    BfManger.setContentInFreeList(stoi(path), record);
                    BfManger.updateHeader(stoi(path));
                }

                else if (!record.empty() && BfManger.setRecord(stoi(path), record)) {
                    int numReg=BfManger.getnumReg(stoi(path));
                    bloque+="-"+to_string(numReg);
                    tree.insert(integerRepresentation,bloque);
                    std::cout << "Registro añadido correctamente" << std::endl;
                    return;
                } 
            }
        }

        else{
            cout<<"registro ya existe"<<endl;
            return;
        }

    }

  
    void deleteReg(string userID, string postID, string esquema){ 
        
        int key=stoi(userID);
        string path= tree.search(key);

        //si no esta el ususario en el arbol
        if(path=="empty"||path=="Nfound"){ 
            cout<<"Usuario no registra"<<endl;
            return;
        }

        //si esta en el arbol
        else{
             string schema = RWesquema::extraerEsquema(esquema);

            //si esta cargado buffer
            if(BfManger.isBlockloaded(stoi(path))){
                
                BfManger.deleteReg(stoi(path) , postID, "postID", schema);
                BfManger.updateHeader(stoi(path));
                BfManger.printAtribsPage(stoi(path));
            }
            //si no esta cargado buffer
            else {
                diskManager.BuscarBloque(stoi(path)); //seteo page 
                BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer
                BfManger.deleteReg(stoi(path) , postID, "postID", schema);
                BfManger.updateHeader(stoi(path));
                BfManger.printAtribsPage(stoi(path)); //imprimo el contenido
               
            }
        }



    }

     void eliminarUsuario(string userID)
    {
        int key = stoi(userID);
        string path = tree.search(key);

        if (path == "empty" || path == "Nfound")
        {
            cout << "Usuario no encontrado" << endl;
            return;
        }

        if (BfManger.isBlockloaded(stoi(path)))
        {
            BfManger.killProcess(stoi(path));
            Page page = BfManger.rescueAndCleanPage(stoi(path));
            diskManager.savePageInBlock(page, 0);
            tree.deleteKey(key);
        }
        else
        {
            diskManager.BuscarBloque(stoi(path)); //seteo page 
            BfManger.loadPageFromDisk(diskManager.getPage(), 'W'); //le doy la page al buffer
            Page page = BfManger.rescueAndCleanPage(stoi(path));
            diskManager.savePageInBlock(page, 0);
            BfManger.killProcess(stoi(path));
            tree.deleteKey(key);
        }
    }



    void mostrarUsuario(string userID)
    {
        int key = stoi(userID);
        string path = tree.search(key);

        if (path == "empty" || path == "Nfound")
        {
            cout << "Usuario no encontrado" << endl;
            return;
        }

        if (BfManger.isBlockloaded(stoi(path)))
        {
            vector<string> registros = BfManger.extractFixedLengthStrings(stoi(path));
            for (int i = 0; i < registros.size(); i++)
            {
                cout << registros[i] << endl;
            }
        }
        else
        {
            diskManager.BuscarBloque(stoi(path)); //seteo page 
            BfManger.loadPageFromDisk(diskManager.getPage(), 'R'); //le doy la page al buffer
            vector<string> registros = BfManger.extractFixedLengthStrings(stoi(path));
            for (int i = 0; i < registros.size(); i++)
            {
                cout << registros[i] << endl;
            }
        }

        
    }

    void mostrarUsuariosTitac(int userID){

        cout<<"<<<<<<<<<<<<<<<mostrando usuarios<<<<<<<<<<<<<<<<<<<<<"<<userID<<endl;
        
        string path=tree.search(userID);


        if(path=="empty"||path=="Nfound"){
            cout<<"Usuario no encontrado"<<endl;
            return;
        }

        string bloque=path.substr(0,path.find("-"));
        int block=stoi(bloque);
        
        string numreg=path.substr(path.find("-")+1,path.size());
        int ubicacion=stoi(numreg);


        if(BfManger.isBlockloaded(stoi(path))){
            vector<string> registros = BfManger.extractFixedLengthStrings(block);
            cout<<registros[ubicacion-1]<<endl;
        }
        else{
            diskManager.BuscarBloque(stoi(path)); //seteo page 
            BfManger.loadPageFromDisk(diskManager.getPage(), 'R'); //le doy la page al buffer
            vector<string> registros = BfManger.extractFixedLengthStrings(block);
            cout<<registros[ubicacion]<<endl;
            
        }
    }

    void MostrarPage(int pageID) {
        BfManger.printAtribsPage(pageID);
    }

    void savePage(int pageID){
        if(BfManger.isPageDir(pageID)){

            Page page = BfManger.rescuePage(pageID);
            diskManager.savePageInBlock(page,1);
            BfManger.killProcess(pageID);
        }
    }

    void showBloackAndPage(int pageID){
        MostrarPage(pageID);
        diskManager.showBlock(pageID);
    }


    void opcionesBufferManager(){
        int opcion;
        do
        {
            std::cout << "\n--- Buffer Manager ---" << std::endl;
            std::cout << "1. pin pagina" << std::endl;
            std::cout << "2. unpin pagina" << std::endl;
            std::cout << "3. destruir procceso" << std::endl;
            std::cout << "4. mostrar frames" << std::endl;
            std::cout << "5. Mostrar página y bloque" << std::endl;
            std::cout << "6. Eliminar registro" << std::endl;
            std::cout << "7. Salir" << std::endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1:{
                cout<<"id pagina: ";
                int id;
                cin>>id;
                BfManger.pinPage(id);
                break;
            }
                
            case 2:{
                cout<<"id pagina: ";
                int id2;
                cin>>id2;
                BfManger.unpinPage(id2);
                break;

            }
            
            case 3:{
                 cout<<"id pagina: ";
                int id3;
                cin >> id3;
                BfManger.killProcess(id3);
                break;
            }
               
            case 4:{
                BfManger.printTableFrame();
                break;
            }
                
                
            case 5:
                
                break;
            case 6:
                
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente." << std::endl;
            }
        } while (opcion != 7);
    
    }

    
    void guardarTodasLasPaginas()
    {
        vector<int> paginasDirties = BfManger.getPagesId();
        for (int i = 0; i < paginasDirties.size(); i++)
        {
            savePage(paginasDirties[i]);
            tree.saveTree("bplustree.txt");
        }

    }

//todo opciones arbol--------------------------------------------------

    void mostrarArbol(){
        tree.printTree();
        cout<<"paths"<<endl;
        tree.printLeafPaths();
    }

    void guardarArbol(){
        tree.saveTree("bplustree.txt");
        cout<<"arbol guardado"<<endl;
    }

    void recuperarArbol(){
        
            try {
                tree.loadTree("bplustree.txt");
            }
            catch (const std::exception &e) {
                std::cerr << "Error al cargar el arbol B+ desde el archivo 'bplustree.dat': " << e.what() << std::endl;
            }
    }

   
    
};


class Menu
{
public:

    DatabaseManagementSystem pruev;
    
    Menu() {
    }

    void mostrar()
    {

        int opcion;
        do
        {
            std::cout << "\n--- Menú ---" << std::endl;
            std::cout << "1. Agregar esquema" << std::endl;
            std::cout << "2. opciones post(useID)" << std::endl;
            std::cout << "3. ingresar post(Date)" << std::endl;
            std::cout << "4. opciones titanic(UserId)" << std::endl;
            std::cout << "5. opciones del B+ TREE" << std::endl;
            std::cout << "6. guardar page" << std::endl;
            std::cout << "7. Mostrar page" << std::endl;
            std::cout << "8. gestionar disco" << std::endl;
            std::cout << "9. guardar todas las paginas" << std::endl;
            std::cout << "10. Salir" << std::endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1:
                agregarEsquema();
                break;
            case 2:
                opcionesPost();
                break;
            case 3:
                ingresarRegistroXFecha();
                break;
            case 4:
                opcionesTitanic();
                break;
            case 5:
                opcionesArbol();
                break;

            case 6:
                guardarPage();
                break;
            case 7:
                mostrarPage();
                break;
                
            case 8:
                crearDisco();
                break;
            
            case 9:
                guardarTodasPaginas();
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente." << std::endl;
            }
        } while (opcion != 10);
    }

    void agregarEsquema()
    {
        system("cls");
        std::string nombreTabla;
        std::cout << "Ingrese el nombre de la tabla: ";
        std::cin >> nombreTabla;
        RWesquema::IngresarEsquema(nombreTabla);
    }

    void opcionesPost(){
        int opcion;
        do
        {
            std::cout << "\n--- Post ---" << std::endl;
            std::cout << "1. Ingresar varios post desde archivo" << std::endl;
            std::cout << "2. Ingresar post teclado" << std::endl;
            std::cout << "3. Mostrar post por usuario" << std::endl;
            std::cout << "4. Eliminar post" << std::endl;
            std::cout << "5. Eliminar Usuario" << std::endl;
            std::cout << "6. Salir" << std::endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1:
                añadirRegistrosCSV();
                break;
            case 2:
                CargarBloque();
                break;
            case 3:
                mostrarUSUARIO();
                break;
            case 4:
                borrarRegistro();
                break;
            case 5:
                eliminarUsuario();
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente." << std::endl;
            }
        } while (opcion != 6);
    }

    void pinUnpinKill()
    {
        system("cls");
        pruev.opcionesBufferManager();
        
    }

    void agregarDatosPorTeclado()
    {
        std::string nombreTabla;
        std::cout << "Ingrese el nombre de la tabla: ";
        std::cin >> nombreTabla;
        RegistroFijo::guardarRegDesdeTeclado(nombreTabla);
    }

    void CargarBloque()
    {
        
        cout<<"Ingrese la tabla: ";
        string tabla;
        cin>>tabla;

        string reg = RegistroFijo::guardarRegDesdeTeclado(tabla);
        
        pruev.ingresarRegistro(reg, tabla);
       //pruev.ingresarRegistroFijosDesdeCsv(nombreTabla,2);
    }

    void añadirRegistrosCSV()
    {
        system("cls");
        int cantidad;
        std::string nombreTabla="post.txt";
        cout<<"Cuantos registros desea añadir: ";
        cin>>cantidad;
        pruev.addRegistrosBloque(nombreTabla, cantidad,1);
    }

    void guardarPage()
    {
        system("cls");
        int pageID;
        std::cout << "Ingrese la pagina a guardar: ";
        std::cin >> pageID;
       pruev.savePage(pageID);
    }

    void mostrarPage()
    {
        system("cls");
        int pageID;
        std::cout << "Ingrese la pagina a mostrar: ";
        std::cin >> pageID;
        pruev.MostrarPage(pageID);
    }

    void crearDisco()
    {
        int caso;
        std::cout << "1. Disco predefinido" << std::endl;
        std::cout << "2. Crear disco por teclado" << std::endl;
        std::cout << "3. mostrar caracteristicas del disco" << std::endl;
        std::cin >> caso;
        system("cls");

        if (caso == 1)
        {
            SimuladorDisco disco;
            cout<<"--------------------------------------------------------------";
            return;
        }
        if (caso == 2)
        {
            int num_platos, num_caras, num_pistas, num_sectores, tamanioSector, sectxBloque;

            cout << "Ingrese el número de platos: "; cin >> num_platos;
            cout << "Ingrese el número de caras por plato: "; cin >> num_caras;
            cout << "Ingrese el número de pistas por cara: "; cin >> num_pistas;
            cout << "Ingrese el número de sectores por pista: "; cin >> num_sectores;
            cout << "Ingrese el tamaño del sector (en bytes): ";cin >> tamanioSector;
            cout << "Ingrese el número de sectores por bloque: ";cin >> sectxBloque;

            // Crear el objeto SimuladorDisco con los valores ingresados
            SimuladorDisco disco(num_platos, num_caras, num_pistas, num_sectores, tamanioSector, sectxBloque);

        }
        if (caso == 3)
        {
            SimuladorDisco::mostrarCaracteristicasDisco();
        }
    }

    void borrarRegistro()
    {
        string userID, postID;
        cout << "Ingrese el ID del usuario: ";
        cin >> userID;
        cout << "Ingrese el ID del registro: ";
        cin >> postID;
        pruev.deleteReg(userID, postID,"post");
    }

    void mostrarUSUARIO()
    {
        string userID;
        cout << "Ingrese el ID del usuario: ";
        cin >> userID;
        pruev.mostrarUsuario(userID);
    }

    void eliminarUsuario()
    {
        string userID;
        cout << "Ingrese el ID del usuario: ";
        cin >> userID;
        pruev.eliminarUsuario(userID);
    }
    void ingresarRegistroXFecha()
    {
        system("cls");
        int cantidad;
        std::string nombreTabla="post.txt";
        cout<<"Cuantos registros desea añadir: ";
        cin>>cantidad;
        pruev.addRegistrosBloque(nombreTabla, cantidad,2);
    }
// todo opciones titanic >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 

    void ingresarUsuariosTitanic(){
        std::string nombreTabla="Titanic.csv";
        int cantidad;
        cout<<"Cuantos registros desea añadir: ";
        cin>>cantidad;
        pruev.addRegistrosBloque(nombreTabla, cantidad,3);
    }

    void mostrarUsuarioTitanic(){
        string userID;
        cout << "Ingrese el ID del usuario: ";
        cin >> userID;
        userID=userID.substr(0,3);
        int var = RegistroFijo::nameToInteger(userID);
        pruev.mostrarUsuariosTitac(var);
    }


    void opcionesTitanic(){
        int opcion;
        do {
            std::cout << "\n--- Titanic ---" << std::endl;
            std::cout << "1. Ingresar varios usuarios desde archivo" << std::endl;
            std::cout << "2. Mostrar usuario" << std::endl;
            std::cout << "3. Salir" << std::endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1:
                ingresarUsuariosTitanic();
                break;
            case 2:
                mostrarUsuarioTitanic();
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente." << std::endl;
            }
        } while (opcion != 3);

    }

// todo sub menu  arbol b+>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    void mostrarArbol(){
        pruev.mostrarArbol();
    }
    void guardarArbol(){
        pruev.guardarArbol();
    }
    void recuperarArbol(){
        pruev.recuperarArbol();
    }
    void opcionesArbol(){
        int opcion;
        do
        {
            std::cout << "\n--- Arbol B+ ---" << std::endl;
            std::cout << "1. Mostrar arbol" << std::endl;
            std::cout << "2. Guardar arbol" << std::endl;
            std::cout << "3. Recuperar arbol" << std::endl;
            std::cout << "4. Salir" << std::endl;
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion)
            {
            case 1:
                mostrarArbol();
                break;
            case 2:
                guardarArbol();
                break;
            case 3:
                recuperarArbol();
                break;
            default:
                std::cout << "\nOpción inválida. Intente nuevamente." << std::endl;
            }
        } while (opcion != 4);
    }

    void guardarTodasPaginas(){
        void guardarTodasLasPaginas();
        cout<<"todas las paginas guardadas";
    }


};



int main() {
    
  Menu menu;
  menu.mostrar();
    
    return 0;
}