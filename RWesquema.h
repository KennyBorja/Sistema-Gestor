#ifndef RWESQUEMA_H
#define RWESQUEMA_H

#include "RWhelper.h"


using namespace std;

class RWesquema
{

public:

    static bool verificaresquema(string esquema)
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

    static string extraerEsquema(const string &nombreTabla)
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

    static void IngresarEsquema(string &nombre)
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

    static void GuardarEsquemaDesdeArchivo(string &nombreTabla)
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

#endif