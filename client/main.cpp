#include <httplib.h>
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    // Lê arquivo de texto
    std::ifstream file("entrada.txt");
    if (!file) {
        std::cerr << "Erro ao abrir arquivo.\n";
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Cliente HTTP -> Mestre
    httplib::Client cli("http://localhost:8080"); // Porta do mestre
    auto res = cli.Post("/process", buffer.str(), "text/plain");

    if (res && res->status == 200) {
        std::cout << "Resposta do Mestre:\n" << res->body << std::endl;
    } 
    else {
        std::cerr << "Erro na comunicação com Mestre.\n";
    }

    return 0;
}
