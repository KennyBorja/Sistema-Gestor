#ifndef RWHELPER_H
#define RWHELPER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class RWhelper {

public:

static string determinarTipoDato(const string &valor)
{
    stringstream ss(valor);
    char c;
    int numInt;
    double numDouble;

    // Intenta convertir el valor a diferentes tipos de datos
    if ((ss >> numInt) && (ss.eof()))
    {
        return "int"; // Si se puede convertir a int, es un número entero
    }
    else if ((ss.clear(), ss.seekg(0), ss >> numDouble) && (ss.eof()))
    {
        return "double"; // Si se puede convertir a double, es un número de punto flotante
    }
    else if ((ss.clear(), ss.seekg(0), ss >> c) && ss.eof() || valor.size() == 1)
    {
        return "char"; // Si se puede convertir a char, es un carácter
    }
    else
    {
        return "string"; // Si no se puede convertir a ninguno de los anteriores, es una cadena
    }
}

static int MayorLongitudCol(const string &nombreArchivo, int posicionColumna)
{

    ifstream archivo(nombreArchivo);
    string linea;
    int maxCaracteres = 0;

    getline(archivo, linea);

    while (getline(archivo, linea))
    {
        istringstream ss(linea);
        string campo;
        int indice = 0;
        while (getline(ss, campo, ';'))
        {
            if (indice == posicionColumna)
            {
                maxCaracteres = max(maxCaracteres, static_cast<int>(campo.length()));
                cout << campo << endl;
                break;
            }
            indice++;
        }
    }

    archivo.close();
    return maxCaracteres;
}

static int sumaTercerasComponentes(const std::string &linea)
{
    istringstream ss(linea);
    string token;

    int suma = 0;
    while (getline(ss, token, '#'))
    {
        // Ignorar el nombre de la columna
        getline(ss, token, '#');
        // Ignorar el tipo de dato
        getline(ss, token, '#');
        // Sumar el valor de la tercera componente (convertida a entero)
        suma += stoi(token);
    }
    return suma;
}

static int sectorxbloque()
{
    ifstream archivo2("caracteristicasDisco.txt");
        string caracteristicas;
        string linea;
        getline(archivo2, caracteristicas);
        stringstream ss2(caracteristicas);
        getline(ss2, linea, '-');//obtener linea de sectores por bloque
        getline(ss2, linea, '-');//obtener linea de sectores por bloque
        int sectoresXBloque = stoi(linea);

        return sectoresXBloque;
}

static int pesoSector()
{
    ifstream archivo2("caracteristicasDisco.txt");
        string caracteristicas;
        string linea;
        getline(archivo2, caracteristicas);
        stringstream ss2(caracteristicas);
        getline(ss2, linea, '-');//obtener linea de sectores por bloque
        int peso = stoi(linea);
        return peso;
}

static string obtenerPalabraDespuesDeBarra(int numBarra, const string& linea) {
    istringstream ss(linea);
    string token;

    // Ignorar los primeros tokens separados por '|'
    for (int i = 0; i < numBarra; ++i) {
        getline(ss, token, '|');
    }

    // Leer y devolver el siguiente token
    if (getline(ss, token, '|')) {
        return token;
    } else {
        return ""; // Si no hay suficientes tokens, retornar cadena vacía
    }
}

static void imprimirContenidoArchivo(const string& rutaArchivo) {
    ifstream archivo(rutaArchivo);
    string linea;

    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo " << rutaArchivo << endl;
        return;
    }

    while (getline(archivo, linea)) {
        cout << linea << endl;
    }

    archivo.close();
}



};


#endif