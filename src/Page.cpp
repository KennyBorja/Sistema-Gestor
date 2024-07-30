#include "../include/Page.h"
#include <fstream>
#include <iostream>

Page::Page() : Size(0), Name(""), pageID(-1) {
}

Page::Page(int size, string name) : Size(size), Name(name) {
}

// todo:  metodos getter////////////////////////////////////////////////////////////

int Page::getPageId() {
  return this->pageID;
}

int Page::getSize() {
  return Size;
}

string Page::getName() {
  return Name;
}

string Page::getCabeceraSpaciosPage() {
  return cabeceraSpaciosPage;
}

string Page::getContent() {
  return content;
}

int Page::getNumRegistros() {
  return numRegistros;
}

int Page::getTamanio() {
  return tamanio;
}

// todo: metodos setter/////////////////////////////////////////////////////////////////////
void Page::setAllAttributes(int Size, string Name, int pageID, string content) {
  this->Size = Size;
  this->Name = Name;
  this->pageID = pageID;
  this->content = content;
}

void Page::setSize(int size) {
  Size = size;
}

void Page::setPageId(int pageId) {
  this->pageID = pageId;
}

void Page::setName(string name) {
  Name = name;
}

void Page::setCabeceraSpaciosPage(string input) {
  cabeceraSpaciosPage = input;
   if (input.empty()) {

            numRegistros = 0;
            tamanio = 0;
            return;
        }
        
        std::stringstream ss(input);
        std::string item;
        std::vector<int> tokens;
        
        while (std::getline(ss, item, '-')) {
            tokens.push_back(std::stoi(item));

        }

        if (tokens.size() >= 2) {
            numRegistros = tokens[0];
            tamanio = tokens[1];
        } else {
            cout<<"Error en la cabecera de la pagina"<<endl;
            return;
        }

        for (size_t i = 2; i < tokens.size(); ++i) {
            posicionesvacias.push_back(tokens[i]);
            cout<<"posiciones vacias"<<tokens[i]<<endl;
        }
}

void Page::setContent(string content) {
  this->content = content;
}

//! metodos para la pagina/////////////////////////////////////////////////////////


bool Page::deleteRecordInContent(int index) {
  
  posicionesvacias.push_back(index);
  numRegistros--;
  Size += tamanio;
  return true;
}


void Page::imprimirPage() {
  cout << "Page ID: " << pageID << endl;
  cout << "Ruta: " << Name << endl;
  cout << "Capacidad: " << Size << endl;
  cout << "Contenido: " << content << endl;
  cout << "Cabecera: " << cabeceraSpaciosPage << endl;
  for(auto i:posicionesvacias){
    cout<<"posiciones vacias"<<i<<endl;
  }
}

bool Page::isPosicionesVaciasEmpty() {
    
    return posicionesvacias.empty();
}

bool Page::addRecordInContent(string &record) {
  // Asumiendo que cada carácter cuenta como un byte
  int recordSize = record.size();
  
  if (recordSize <= Size) {
    
    numRegistros++;
    tamanio=recordSize;
    content+=record;
    Size -= recordSize;
    return true;
  }
  return false;
}


void Page::SetRecordtInEmptyPoscition(string record) {

    int pos=tamanio*posicionesvacias[0];
    content.replace(pos,record.size(),record);
    numRegistros++; 
    posicionesvacias.erase(posicionesvacias.begin());
}

void Page::updateHeader() {
    cabeceraSpaciosPage = std::to_string(numRegistros) + "-" + std::to_string(tamanio);
    for (size_t i = 0; i < posicionesvacias.size(); ++i) {
        cabeceraSpaciosPage += "-" + std::to_string(posicionesvacias[i]);
    }
}

int Page::DeleteTarjetRecord(string target, string itemToSearch, string schema) {
    stringstream ss(schema);
    string token;
    int position = 0;

    int pesoActual = 0;
    int sumaprev = 0;

    while(getline(ss, token, '#')) {
        
        if (token == itemToSearch) {
            
            getline(ss, token, '#' ); //tipo
            getline(ss, token, '#' ); //tamaño
            pesoActual=stoi(token);
            //cout<<"-----pesoactual---------"<<pesoActual<<"----------------------"<<endl;
            break;
        }
        getline(ss, token, '#' ); //tipo
        getline(ss, token, '#' ); //tamaño
        sumaprev=sumaprev+ stoi(token);
        
    }

    if(pesoActual==0){ //si no encuentra

      return -1;
    }

    vector<string> segments = extractFixedLengthStrings();
    
    int start = sumaprev+1;
    int end = sumaprev + pesoActual - 1;
    int posDelete=compareSubstrings(segments, target, start, end);
    if (posDelete!=-1){
      deleteRecordInContent(posDelete);
    }
    return posDelete;
}


vector<string> Page::extractFixedLengthStrings() {
    vector<string> result;

    for (size_t i = 0; i < content.length(); i += tamanio) {
        result.push_back(content.substr(i, tamanio));
    }

    return result;
}

int Page::compareSubstrings(const vector<string>& segments, const string& target, int start, int end) {
    int count=0;
    for (const string& segment : segments) {
          string substring = segment.substr(start - 1, end - start + 1);
          string cleanedSubstring = removeSpaces(substring);
          if (cleanedSubstring == target) {
            //cout<<"posicion a eliminar<<<<<<<<<<<<<<"<<count<<endl;
            return count;
          }
          count++;
    }
    return -1;
}

string Page::removeSpaces( string& str) {
    string noSpaces;
    noSpaces.reserve(str.size()); // Reservar espacio suficiente para mejorar el rendimiento
    for (char ch : str) {
        if (!isspace(ch)) {
            noSpaces += ch;
        }
    }
    return noSpaces;
}


void Page::cleanPage() {
    content.clear();
    numRegistros = 0;
    tamanio = 0;
    posicionesvacias.clear();
    Size = 2048;
}

int Page::getEmptyPosition() {
    int vacias=posicionesvacias[0];
    posicionesvacias.erase(posicionesvacias.begin());
    return vacias;
}