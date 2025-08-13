#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

using namespace std;

const int MAX_VENTAS = 1000;

struct Empleado {
    char nombre[50];
    int edad;
    int codigoIdentificacion;
};

struct Venta {
    int   fecha;           
    int   codigoVendedor;  
    int   codigoProducto;
    double monto;
};

// Comprueba la existencia de vendedores.

bool vendedorExiste(int codigo, const char* archivoVendedores = "vendedores.dat") {
    ifstream in(archivoVendedores, ios::binary);
    if (!in) return false;

    Empleado e;
    while (in.read(reinterpret_cast<char*>(&e), sizeof(Empleado))) {
        if (e.codigoIdentificacion == codigo) return true;
    }
    return false;
}

// Guarda todas las ventas en un archivo.

bool guardarVenta(const Venta & v, const char* archivoVentas = "ventas_diarias.dat") {
    ofstream out(archivoVentas, ios::binary | ios::app);
    if (!out) {
        cerr << "No se pudo abrir " << archivoVentas << ".\n";
        return false;
    }
    out.write(reinterpret_cast<const char*>(&v), sizeof(Venta));
    return static_cast<bool>(out);
}

int main() {
    int fecha;
    cout << "Ingrese la fecha (AAAAMMDD): ";
    if (!(cin >> fecha)) {
        cerr << "Fecha invalida.\n";
        return 1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int ventasCargadas = 0;

    while (ventasCargadas < MAX_VENTAS) {

        int codVend;
        cout << "Codigo de vendedor (0 para terminar): ";

        if (!(cin >> codVend)) {
            cerr << "Entrada invalida.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (codVend == 0) break;

        if (!vendedorExiste(codVend)) {
            cout << "El vendedor no encontrado.\n";
            continue;
        }

        int codProd;
        cout << "Codigo de producto: ";

        if (!(cin >> codProd)) {
            cerr << "Entrada invalida.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        double monto;
        cout << "Monto de la venta: ";

        if (!(cin >> monto)) {
            cerr << "Entrada invalida.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Venta v{fecha, codVend, codProd, monto};
        if (!guardarVenta(v)) {
            cerr << "Error al guardar la venta.\n";
            break;
        }

        ventasCargadas++;

        char seguir;
        cout << "¿Cargar otra venta? (S/N): ";
        cin >> seguir;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (seguir != 's' && seguir != 'S') break;
    }

    cout << "Ventas guardadas para " << fecha << ": " << ventasCargadas << "\n";
    return 0;
}
