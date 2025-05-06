#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

void pausa(int milisegundos) {
    sleep_for(milliseconds(milisegundos));
}

string decimalABinario(int n, int longitud) {
    string bin = "";
    while (n > 0) {
        bin = (char)('0' + (n % 2)) + bin;
        n /= 2;
    }
    while (bin.length() < longitud) {
        bin = '0' + bin;
    }
    return bin;
}

void sumadorCompleto(int a, int b, int acarreoEntrada, int& suma, int& acarreoSalida) {
    suma = a ^ b ^ acarreoEntrada;
    acarreoSalida = (a & b) | (acarreoEntrada & (a ^ b));
}

bool propagar(const string& A, const string& B, int inicio, int fin) {
    bool p = true;
    cout << "  Grupo de " << inicio << " a " << fin << " (posición original):\n";
    pausa(500);
    for (int i = inicio; i <= fin; ++i) {
        int ai = A[i] - '0';
        int bi = B[i] - '0';
        int xor_result = ai ^ bi;
        cout << "    A[" << i << "] = " << ai << ", B[" << i << "] = " << bi
             << ", A^B = " << xor_result << "\n";
        p = p && (xor_result == 1);
        pausa(300);
    }
    cout << "  Resultado P = " << p << " → " << (p ? "Propaga el acarreo" : "NO propaga") << "\n";
    pausa(800);
    return p;
}

string sumaSaltoAcarreo(string A, string B, int K) {
    int n = A.length();
    reverse(A.begin(), A.end());
    reverse(B.begin(), B.end());

    string suma(n, '0');
    vector<bool> p_bloques;

    cout << "\n=== Fase 1: Detectar propagación de grupos ===\n";
    pausa(700);
    for (int i = 0; i < n; i += K) {
        int fin = min(i + K - 1, n - 1);
        bool p = propagar(A, B, i, fin);
        p_bloques.push_back(p);
    }

    cout << "\n=== Fase 2: Sumar con salto de acarreo ===\n";
    pausa(700);
    int acarreo = 0;
    for (int i = 0; i < n; i += K) {
        int acarreoGrupo = acarreo;
        int fin = min(i + K - 1, n - 1);

        cout << "\nSumando grupo " << i / K << " (bits " << i << " a " << fin << "):\n";
        cout << "  C_in = " << acarreoGrupo << "\n";
        pausa(500);

        for (int j = i; j <= fin; ++j) {
            int a = A[j] - '0';
            int b = B[j] - '0';
            int sumaBit, acarreoBit;
            sumadorCompleto(a, b, acarreoGrupo, sumaBit, acarreoBit);
            suma[j] = sumaBit + '0';
            cout << "    A[" << j << "] = " << a << ", B[" << j << "] = " << b
                 << " → suma = " << sumaBit << ", C_out = " << acarreoBit << "\n";
            acarreoGrupo = acarreoBit;
            pausa(300);
        }

        string parcial = suma.substr(i, fin - i + 1);
        cout << "  Resultado del grupo: " << parcial << "\n";

        if (i + K < n && p_bloques[i / K]) {
            cout << "  → El grupo propaga el acarreo, se mantiene: C_out = " << acarreo << "\n";
        } else {
            acarreo = acarreoGrupo;
            cout << "  → El grupo NO propaga, nuevo C_out = " << acarreo << "\n";
        }
        pausa(800);
    }

    if (acarreo > 0) {
        suma += '1';
        cout << "\nAcarreo final agregado → Se añade '1' al final.\n";
    }

    reverse(suma.begin(), suma.end());
    cout << "\nResultado final: " << suma << "\n";
    return suma;
}

int main() {
    int num1, num2;
    int K;
    cout << "Ingrese el primer número (decimal): ";
    cin >> num1;
    cout << "Ingrese el segundo número (decimal): ";
    cin >> num2;
    cout << "Ingrese el tamaño del grupo K: ";
    cin >> K;

    int max_val = max(num1, num2);
    int longitud = 0;
    while ((1 << longitud) <= max_val) longitud++;
    longitud = max(longitud, K); // Asegurar al menos tamaño K

    string A = decimalABinario(num1, longitud);
    string B = decimalABinario(num2, longitud);

    cout << "\nNúmero 1 en binario: " << A << " (decimal " << num1 << ")\n";
    cout << "Número 2 en binario: " << B << " (decimal " << num2 << ")\n";

    string resultado = sumaSaltoAcarreo(A, B, K);
    cout << "\n>>> Resultado final: " << resultado << "\n";

    return 0;
}
