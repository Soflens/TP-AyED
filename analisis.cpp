#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

struct Empleado {
    char nombre[50];
    int edad;
    int codigoIdentificacion;
    int sucursal; 
};

struct Venta {
    int codigoIdentificacion; 
    int codigoProducto;
    int cantidad;
    float precioUnitario;
};

int main() {

    // Con el codigoIdentificacion se puede obtener la informacón necesaria del vendedor

    map<int, Empleado> vendedores;

    ifstream archVend("vendedores.dat", ios::binary);
    if (!archVend) {
        cerr << "Error al abrir venddores.dat" << endl;
        return 1;
    }
    Empleado emp;
    while (archVend.read((char*)&emp, sizeof(Empleado))) {
        vendedores[emp.codigoIdentificacion] = emp;
    }
    archVend.close();

    // Variables auxiliares para contener valores totales

    map<int, float> ventasPorVendedor; 
    map<int, float> ventasPorSucursal; 
    map<int, int> productosVendidos;  

    ifstream archVentas("ventas_diarias.dat", ios::binary);
    if (!archVentas) {
        cerr << "Error al abrir ventas_diarias.dat" << endl;
        return 1;
    }

    Venta venta;
    while (archVentas.read((char*)&venta, sizeof(Venta))) {
        float totalVenta = venta.cantidad * venta.precioUnitario;
        int codigoVend = venta.codigoIdentificacion;

        ventasPorVendedor[codigoVend] += totalVenta;

        if (vendedores.count(codigoVend)) {
            int suc = vendedores[codigoVend].sucursal;
            ventasPorSucursal[suc] += totalVenta;
        }

        productosVendidos[venta.codigoProducto] += venta.cantidad;
    }
    archVentas.close();

    // Vendedor que más dinero generó 

    int mejorVendedor = -1;
    float maxVentasVend = 0;
    for (auto& p : ventasPorVendedor) {
        if (p.second > maxVentasVend) {
            maxVentasVend = p.second;
            mejorVendedor = p.first;
        }
    }

    cout << "Resultados de Análisis\n\n";
    if (mejorVendedor != -1) {
        cout << "Vendedor que más dinero generó:\n";
        cout << "Nombre: " << vendedores[mejorVendedor].nombre << endl;
        cout << "Montototal: $" << maxVentasVend << "\n\n";
    }

    // Sucursal que más dinero generó

    int mejorSucursal = -1;
    float maxVentasSuc = 0;
    for (auto& p : ventasPorSucursal) {
        if (p.second > maxVentasSuc) {
            maxVentasSuc = p.second;
            mejorSucursal = p.first;
        }
    }

    if (mejorSucursal != -1) {
        cout << "Sucursal que más dinero generó:\n";
        cout << "Número de sucursal: " << mejorSucursal << endl;
        cout << "Montototal: $" << maxVentasSuc << "\n\n";
    }

    //Ranking de productos más vendidos
    
    vector<pair<int, int>> ranking(productosVendidos.begin(), productosVendidos.end());
    sort(ranking.begin(), ranking.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    cout << "Ranking de productos mas vendidos (por cantidad total):\n";
    for (auto& p : ranking) {
        cout << "Producto " << p.first << " - Cantidad: " << p.second << endl;
    }

    return 0;
}

