#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

// Constantes Importantes

const int empleadosPorSucursal = 10;      
const int sucursalesMaximo = 5;           
const int empleadosMaximos = empleadosPorSucursal * sucursalesMaximo;

// Agregamos un variable más para facilitar la programación de la parte 3 = int sucursal

struct Empleado {
    char nombre[50];
    int edad;
    int codigoIdentificacion;
    int sucursal; 
};

struct Sucursal {
    Empleado empleados[empleadosPorSucursal];
    int cantEmpleados;
};

bool codigoDuplicado(Sucursal sucursales[], int totalSucursales, int codigo) {
    for (int i = 0; i < totalSucursales; i++) {
        for (int j = 0; j < sucursales[i].cantEmpleados; j++) {
            if (sucursales[i].empleados[j].codigoIdentificacion == codigo) {
                return true;
            }
        }
    }
    return false;
}

void guardarEmpleadosEnArchivo(Sucursal sucursales[], int totalSucursales, const char* nombreArchivo) {
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }

    for (int i = 0; i < totalSucursales; i++) {
        for (int j = 0; j < sucursales[i].cantEmpleados; j++) {
            Empleado& emp = sucursales[i].empleados[j];
            archivo.write((char*)&emp, sizeof(Empleado));
        }
    }

    archivo.close();
    cout << "Datos guardados exitosamente en " << nombreArchivo << endl;
}

void mostrarEmpleadosDesdeArchivo(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo para lectura." << endl;
        return;
    }

    Empleado emp;
    int contador = 1;
    while (archivo.read((char*)&emp, sizeof(Empleado))) {
        cout << "Empleado " << contador++ << ":\n";
        cout << "Nombre: " << emp.nombre << endl;
        cout << "Edad: " << emp.edad << endl;
        cout << "Código de identificación: " << emp.codigoIdentificacion << endl;
        cout << "Sucursal: " << emp.sucursal << endl;
        cout << endl;
    }

    archivo.close();
}

int main() {
    Sucursal sucursales[sucursalesMaximo];
    for (int i = 0; i < sucursalesMaximo; i++) {
        sucursales[i].cantEmpleados = 0;
    }

    int numEmpleadosTotal = 0;

    while (numEmpleadosTotal < empleadosMaximos) {
        cout << "Ingrese el número de sucursal donde irá el empleado " << numEmpleadosTotal + 1
             << " (1-" << sucursalesMaximo << "): ";
        int sucursal;
        cin >> sucursal;
        cin.ignore();

        if (sucursal < 1 || sucursal > sucursalesMaximo) {
            cout << "Número de sucursal inválido.\n";
            continue;
        }

        if (sucursales[sucursal - 1].cantEmpleados >= empleadosPorSucursal) {
            cout << "Esa sucursal ya alcanzó su capacidad máxima de "
                 << empleadosPorSucursal << " empleados.\n";
            continue;
        }

        cout << "Ingrese el nombre del empleado: ";
        char nombre[50];
        cin.getline(nombre, 50);

        cout << "Edad del empleado: ";
        int edad;
        cin >> edad;
        cin.ignore();

        cout << "Código de identificación (ej. 123456): ";
        int codigoIdentificacion;
        cin >> codigoIdentificacion;
        cin.ignore();

        if (codigoDuplicado(sucursales, sucursalesMaximo, codigoIdentificacion)) {
            cout << "Error: Ya existe un empleado con ese código de identificación.\n";
            continue;
        }

        int aux = sucursales[sucursal - 1].cantEmpleados;
        strcpy(sucursales[sucursal - 1].empleados[aux].nombre, nombre);
        sucursales[sucursal - 1].empleados[aux].edad = edad;
        sucursales[sucursal - 1].empleados[aux].codigoIdentificacion = codigoIdentificacion;
        sucursales[sucursal - 1].empleados[aux].sucursal = sucursal; 

        sucursales[sucursal - 1].cantEmpleados++;
        numEmpleadosTotal++;

        cout << "¿Desea agregar otro empleado? (S/N): ";
        char respuesta;
        cin >> respuesta;
        cin.ignore();

        if (respuesta != 's' && respuesta != 'S') {
            break;
        }
    }

    guardarEmpleadosEnArchivo(sucursales, sucursalesMaximo, "vendedores.dat");

    cout << "\n--- Mostrando empleados desde archivo ---\n\n";
    mostrarEmpleadosDesdeArchivo("vendedores.dat");

    return 0;
}
