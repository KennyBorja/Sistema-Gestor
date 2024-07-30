#ifndef REGISTROFIJO_H
#define REGISTROFIJO_H

//#include "RWhelper.h"
#include "RWBLOQUE.h"
#include "RWesquema.h"


using namespace std;

class RegistroFijo {

public:
   static void GuardarRegistro(const string &registro, int totalPeso, const string &nombreTabla)
{
    
    string Bloque =RWBloque::bloqueConEspacio(totalPeso,nombreTabla );
    
    if(Bloque==""){
        cout<<"disco lleno"<<endl;
    }

    if (Bloque != "")
    {
        stringstream ss(Bloque);
        string parte;

        getline(ss, parte, '-');
        string nombreBloque=parte;
        string rutaBloque="disk/bloques/" + nombreBloque + ".txt";

        getline(ss, nombreBloque, '-');
        string sector=parte;

      
        
        //RWBloque::ActualizarCabeceraBloque(rutaBloque,totalPeso,nombreTabla );
        //RWBloque::(totalPeso,nombreTabla );
        //RWBloque::(totalPeso,nombreTabla );


        ofstream archivo2("disk/" + Bloque + nombreTabla + ".txt", ios::app);
        //cout<<"disk/" + RUTA + nombreTabla + ".txt";
        archivo2 << registro << endl;
        archivo2.close();
    }
}

    static string guardarRegDesdeTeclado(string nombre)
{
    string fila = RWesquema::extraerEsquema(nombre);
    int totalPeso = RWhelper::sumaTercerasComponentes(fila);
    cout << "el peso total es: " << totalPeso << endl;

    if (fila.empty())
    {
        cout << "El esquema '" << nombre << "' no fue encontrado." << endl;
        return "";
    }
    else
    {

        istringstream ss(fila);
        string parte;
        string guardar;
        string dato;
        string nombreColumna;
        string peso;

        while (getline(ss, parte, '#'))
        {
            // obtener el nombre de la columna
            nombreColumna = parte;

            // pasar el tipo de dato
            getline(ss, parte, '#');

            // obtener el peso
            getline(ss, parte, '#');
            peso = parte;
            int longitudDeseada = std::stoi(peso);

            cout << "Ingrese " << nombreColumna << ": " << endl;
            cin >> dato;

            int caracteresActuales = dato.length();

            if (caracteresActuales < longitudDeseada)
            {
                dato.append(longitudDeseada - caracteresActuales, ' ');
            }

            if (dato.length() > longitudDeseada)
            {
                dato = dato.substr(0, longitudDeseada);
            }

            guardar += dato;
        }
        return guardar;
    }
}

    static std::string guardarRegDesdeCsv(std::ifstream& archivo, const std::string& esquema) {
        std::string fila = esquema;
        
        if (fila.empty()) {
            std::cout << "El esquema no existe" << std::endl;
            return "";
        }

        int totalPeso = RWhelper::sumaTercerasComponentes(fila);
        int peso;

        std::string linea;
        std::string parte;
        std::string guardar;
        std::string dato;

        std::string totalGuardar;

        std::istringstream ssEsq(fila);

            std::getline(archivo, linea);
            std::istringstream ss(linea);
            std::string campo;

            while (std::getline(ss, campo, ';')) {
                // obtener el nombre de la columna
                std::getline(ssEsq, parte, '#');
                std::getline(ssEsq, parte, '#');
                std::getline(ssEsq, parte, '#');

                peso = std::stoi(parte);

                int longitudDeseada = peso;
                int caracteresActuales = campo.length();

                if (caracteresActuales < longitudDeseada) {
                    campo.append(longitudDeseada - caracteresActuales, ' ');
                }

                if (caracteresActuales > longitudDeseada) {
                    campo = campo.substr(0, longitudDeseada);
                }

                guardar += campo;
            }
            
            ssEsq.clear();  // Limpiar el estado del stringstream
            ssEsq.seekg(0, std::ios::beg); // Establecer el puntero de lectura al principio

            totalGuardar += guardar;
            guardar.clear();  // Limpiar guardar para el próximo registro
            
        return totalGuardar;
    }

    
    static void mostrarTabla(string nombreEsquema)
{

    string nombreArch = nombreEsquema + ".txt";

    string esquema = RWesquema::extraerEsquema(nombreEsquema);

    if (esquema.empty())
    {
        cout << "El archivo '" << nombreEsquema << ".txt' no se pudo abrir." << endl;
        return;
    }
    else
    {

        string token;
        size_t pos = 0;
        int contador = 0;

        while ((pos = esquema.find('#', pos)) != string::npos)
        {
            if (contador % 3 == 0)
            {
                token = esquema.substr(0, pos);
                cout << token << "\t"; // Imprimir solo los elementos en posiciones impares seguidos de una tabulación
            }
            esquema.erase(0, pos + 1); // Eliminar el token y el delimitador de la cadena
            contador++;                // Cambiar entre índices impares y pares en cada iteración
            pos = 0;                   // reiniciar la pocicion
        }
        cout << endl;
        // mostrar tabla cambiando # por tabulaciones

        ifstream file("bloques.txt");
        string linea;
        

        string ruta;
        string pesoBloq;
        int pesoSector=RWhelper::pesoSector();
        int sectXbloq = RWhelper::sectorxbloque();
        int pesoRestante;
        

        while (getline(file, linea))
        {
            istringstream ss(linea);
            getline(ss, pesoBloq, '|');//tamaño bloque
            if(stoi(pesoBloq)<(pesoSector*sectXbloq))
            {
                string tabla=RWhelper::obtenerPalabraDespuesDeBarra(2+sectXbloq, linea);
                if(tabla==nombreEsquema)
                {
                    getline(ss, pesoBloq, '|');//nombre bloque

                    for(int i=0;i<sectXbloq;i++)
                    {
                        string ruta;
                         getline(ss, pesoBloq, '|');//bloques requeridos
                         string espaciorestanteSector;
                         stringstream ss2(pesoBloq);
                         getline(ss2, espaciorestanteSector, '-');

                         if(stoi(espaciorestanteSector)<pesoSector)
                         {
                            ruta+="disk/";
                            ruta+=RWBloque::obtenerRuta(pesoBloq);
                            ruta+=nombreArch;
                            RWhelper::imprimirContenidoArchivo(ruta);
                         }



                    }

                }
                else break;

            }


        }
    }
}

    void consultaSelect(string nombreTabla) {
    string resultado;
    string condiciones;

    string esquema = RWesquema::extraerEsquema(nombreTabla);
    string copiaesquema=esquema;

    if (esquema.empty()) {
        cout << "El archivo '" << nombreTabla << ".txt' no se pudo abrir." << endl;
        return;
    }


    size_t pos = 0;
    int contador = 0;
    string token;
    cout<<"ingrese la condicion"<<endl;
    string condicion;
    cin>>condicion;

    size_t posicionOperador = condicion.find_first_of("=<>");
    string columna = condicion.substr(0, posicionOperador);
    string operador = condicion.substr(posicionOperador, 1);
    string valor = condicion.substr(posicionOperador + 1);


    bool encontrado = false;

    while ((pos = esquema.find('#', pos)) != string::npos) {
        
        token = esquema.substr(0, pos);
        cout << token << "\t"; // Imprimir solo los elementos en posiciones impares seguidos de una tabulación
        esquema.erase(0, pos+1); // Eliminar el token y el delimitador de la cadena
        contador++; // Cambiar entre índices impares y pares en cada iteración
        pos=0; //reiniciar la pocicion
        if (token == columna) {
            encontrado=true;
        
            break;
        }

    }

    if(esquema==columna){
        contador++;
        encontrado=true;
        cout<<"ultimontrado";
    }
    
    if (encontrado==false){
        cout<<"no se encontro";
        return;
    }
       

    contador=(contador+1)/2;
    cout<<contador;
    

    string nombreArchivo = nombreTabla + ".txt";
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo '" << nombreArchivo << "'." << endl;
        return;
    }

    string linea;
    string token2;
    ofstream archivo2(columna + ".txt", ios::app);
    bool Cumple = false;
    while (getline(archivo, linea)) {
         size_t poss = 0;
        string verificar=linea;

        for(int i = 0; i < contador; i++) {
            poss = verificar.find('#', poss);
            token2 = verificar.substr(0, poss);
            verificar.erase(0, poss+1);
            poss=0;
        }

        cout<<token2<<endl;
       
        bool cumpleCondiciones = true;
        
            

            if (operador == "=") {
                if (token2 != valor) {
                    cumpleCondiciones = false;
                    
                   
                }
            } else if (operador == "<") {
                if (token2 >= valor) {
                    cumpleCondiciones = false;
                    
                }
            } else if (operador == ">") {
                if (token2 <= valor) {
                    cumpleCondiciones = false;
                    
                }
            }
        
        

        if (cumpleCondiciones) {
            archivo2 << linea << endl;
            Cumple = true;

            
        } 
    }

    if (Cumple == true) {

        
        bool esta=RWesquema::verificaresquema(columna);
        if(esta==false){
            ofstream archivo3("esquemas.txt", ios::app);
            archivo3 << columna<<'#' <<copiaesquema <<endl;
        }
    }

    archivo.close();
}

    static int nameToInteger(const std::string &name) {
        int result = 0;
        for (char c : name) {
            result = result * 256 + static_cast<unsigned char>(c);
        }
        return result;
    }

   static std::string integerToName(unsigned long long value) {
        std::string name;
        while (value > 0) {
            char c = static_cast<char>(value % 256);
            name = c + name;
            value /= 256;
        }
        return name;
    }



    static string extraerKey(string nombreTabla,string record,int pos)
    {
        
        string esquema = RWesquema::extraerEsquema(nombreTabla);
        istringstream ss(esquema);
        string item;
        
        int posSUma=0;
        int size=0;
            for(int i=0;i<pos;i++)
            {
                getline(ss, item, '#');
                getline(ss, item, '#');
                getline(ss, item, '#');

                if (i==pos-1)
                {
                    size=stoi(item);
                    break;
                }
                posSUma=stoi(item)+posSUma;
            }
        return record.substr(posSUma,size);
    }
    
};




#endif