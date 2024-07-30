#ifndef RWBLOQUE_H
#define RWBLOQUE_H
#include <vector>

#include "RWhelper.h"

using namespace std;

class RWBloque
{
public:


    
static void RestaeUbicacionBloque(int numBloque, int numSector, int pesoRegistro, const string& nombretabla) {
    ifstream archivo("bloques.txt");
    ofstream archivoTemporal("temp.txt");
    string linea;
    int lineaActual = -1;

    while(getline(archivo, linea)) {
        lineaActual++;

        if(lineaActual == numBloque) {
            istringstream iss(linea);
            string nuevaCadena;

            // Restar el peso del registro al bloque
            getline(iss, linea, '|');
            int pesoBloque = stoi(linea);
            pesoBloque -= pesoRegistro;
            nuevaCadena += to_string(pesoBloque) + "|";

            // Agregar el nombre del bloque
            getline(iss, linea, '|');
            nuevaCadena += linea + "|";

            int sector = 0;
            while(getline(iss, linea, '|')) { // Buscar los sectores
                sector++;
                if (sector == numSector) {
                    size_t pos = linea.find('-');
                    string parte1 = linea.substr(0, pos); // Parte antes del '-'
                    int pesoSector = stoi(parte1) - pesoRegistro;
                    string parte2 = linea.substr(pos + 1); // Parte después del '-'
                    nuevaCadena += to_string(pesoSector) + "-" + parte2 + "|";
                } else {
                    nuevaCadena += linea + "|";
                }
            }

            // Verificar si el nombre de la tabla ya está presente
            if (nuevaCadena.find(nombretabla) == string::npos) {
                nuevaCadena += nombretabla;
            } else {
                // Si el nombre de la tabla ya está presente, no agregar el separador '|'
                nuevaCadena.pop_back(); // Eliminar el último '|' agregado
            }

            archivoTemporal << nuevaCadena << endl; // Escribir la línea modificada en el archivo temporal
        } else {
            archivoTemporal << linea << endl; // Escribir la línea sin modificar en el archivo temporal
        }
    }

    archivo.close();
    archivoTemporal.close();
    std::remove("bloques.txt"); // Pasar la cadena literal directamente
    std::rename("temp.txt", "bloques.txt"); // Pasar las cadenas literales directamente
}

static string bloqueConEspacio(int pesoRegistro, string const &nombreTabla)
    {
        ifstream archivo("bloques.txt");
        string linea;

        //obtener cantidad de bloques
        ifstream archivo2("caracteristicasDisco.txt");
        string caracteristicas;
        getline(archivo2, caracteristicas);
        stringstream ss2(caracteristicas);
        getline(ss2, linea, '-');//obtener linea de sectores por bloque
        int pesoSector = stoi(linea);
        getline(ss2, linea, '-');//obtener linea de sectores por bloque
        int sectoresXBloque = stoi(linea);
        
        string nombreBloque, pesoDisponible, ActualTabla;

        while (getline(archivo, linea))
        {
            istringstream ss(linea);
            string token;

            getline(ss, token, '|');//nombrebloque
            nombreBloque = token;
            getline(ss, token, '|');//obtener espacio disponible
            pesoDisponible = token;
            getline(ss, token, '|');//hay datos registrados
            ActualTabla = token;


            if (stoi(pesoDisponible) == sectoresXBloque*pesoSector)
            {
                archivo.close();
                return nombreBloque;
            }


            if ((stoi(pesoDisponible) > pesoRegistro)&&(ActualTabla==nombreTabla))
            {
                if (verificarSector(nombreBloque, pesoRegistro)==false)
                {
                    //cout<<"Espacio insuficiente cccccccccccccccccccccccccccccccc";
                    continue;
                }
                archivo.close();
                return nombreBloque;
            }
        }
        return "";
    }

    static bool verificarSector(string  nombreBloque, int peso)
    {
        ifstream archivo("disk/bloques/"+nombreBloque+".txt");
       //cout<< "disk/bloques/"+nombreBloque+".txt";
        string linea;
        getline(archivo, linea);
        stringstream ss(linea);
        string token,token1;

        while(getline(ss, token, '|'))
        {
            stringstream ss2(token);
            getline(ss2, token1, '-');
            if (stoi(token1) > peso)
            {
                archivo.close();
                return true;
            }
        }

        archivo.close();
        return false;

    }

    static string obtenerRuta(string &linea)
    {
        string ruta;
        istringstream ss(linea);
        // omitir peso y bloque
        getline(ss, linea, '-');

        // obtener ruta
        getline(ss, linea, '-');
        ruta += "plato" + linea + "/";
        getline(ss, linea, '-');
        ruta += "cara" + linea + "/";
        getline(ss, linea, '-');
        ruta += "pista" + linea + "/";
        getline(ss, linea, '-');
        ruta += "sector" + linea + "/";
        return ruta;

    }

    static int capacidadBloque(const string &linea)
    {
        istringstream ss(linea);
        string pesoActual;
        getline(ss, pesoActual, '-');
        return stoi(pesoActual);
    }

    

    static void ActualizarBloques(int Bloque,int totalPeso, int mode )
    {
        ifstream archivo("bloques.txt");
        ofstream archivoTemporal("temp.txt");
        string linea;
        while (getline(archivo, linea))
        {
            istringstream ss(linea);
            string token;
            string nuevaLinea;
            getline(ss, token, '|');
            if (stoi(token) == Bloque)
            {
                nuevaLinea+=token+"|";
                nuevaLinea += to_string(totalPeso);
                nuevaLinea+="|"+to_string(mode);
                archivoTemporal << nuevaLinea << endl;
            }
            else
            {
                archivoTemporal << linea << endl;
            }
        }

        archivo.close();
        archivoTemporal.close();
        std::remove("bloques.txt");
        std::rename("temp.txt", "bloques.txt");

    }


    static void ActualizarBloque(int blockID, string Ruta, string Cabecera, string content)
{
     // Definir la ruta del archivo en la subcarpeta "disk/bloques/"
    string rutaBloques = "disk/bloques/" + to_string(blockID) + ".txt";

    // Abrir el archivo en modo de escritura truncada
    ofstream archivoSalida(rutaBloques, ios::trunc);
    if (!archivoSalida.is_open()) {
        cerr << "No se pudo abrir el archivo para escribir: " << rutaBloques << endl;
        return;
    }
    // Escribir los nuevos valores al archivo
    archivoSalida << Ruta << endl;
    archivoSalida << Cabecera << endl;
    archivoSalida << content << endl;
    archivoSalida.close();
}



    static void ActializarSectores(const std::string& rutas, const std::string& content, int tamanioreg, int numreg)
    {
        vector <string> rutasVector;
        istringstream ss(rutas);
        string token;
        while (getline(ss, token, '|'))
        {
            rutasVector.push_back(token);
        }

        string path = "disk/";
        int cantSector = rutasVector.size();

        int regXsector = numreg/cantSector;
        int resto=numreg%rutasVector.size();
        int nosobrepasarResto=0;
        int pos=0;

        for(int i = 0; i < cantSector; i++)
        {
            
            istringstream ss(rutasVector[i]);
            string token;
            getline(ss, token, '-');
            path += "plato" + token + "/";
            getline(ss, token, '-');
            path += "cara" + token + "/";
            getline(ss, token, '-');
            path += "pista" + token + "/";
            getline(ss, token, '-');
            path += "sector" + token + ".txt";


            std::ofstream archivo(path, std::ios_base::trunc);


            if(regXsector==0&&resto>0)
            {
                
                    string registro = content.substr(pos,regXsector * tamanioreg);
                    archivo << registro << std::endl;
                    pos+=regXsector * tamanioreg;
                    resto--;
                
            }

            else if(regXsector>0)
            {
                if(resto>0)
                {
                    string registro = content.substr(pos,regXsector * tamanioreg+tamanioreg);
                    archivo << registro << std::endl;
                    pos+=regXsector * tamanioreg+tamanioreg;
                    resto--;
                }
                else
                {
                    string registro = content.substr(pos,regXsector * tamanioreg);
                    archivo << registro << std::endl;
                    pos+=regXsector * tamanioreg;
                }
            }


            else if (resto==0){
                break;
            }

            archivo.close();
            path = "disk/";
        }
    }

};

#endif // UTILIDADESARCHIVO_H
