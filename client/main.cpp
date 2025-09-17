#include "../httplib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>

// Códigos ANSI para cores (pastéis)
#define RESET "\033[0m"
#define BLUE "\033[38;2;100;180;255m"   // Azul pastel
#define YELLOW "\033[38;2;255;245;157m" // Amarelo pastel
#define RED "\033[38;2;255;158;158m"    // Vermelho pastel
#define BOLD "\033[1m"

void printStatus(const std::string& msg) {
    std::cout << YELLOW << BOLD << "[STATUS] " << RESET << msg << std::endl;
}

void printError(const std::string& msg) {
    std::cerr << RED << BOLD << "[ERRO] " << RESET << msg << std::endl;
}

void printMenu() {
    std::cout << BLUE;
    std::cout << "   ___   ___   ___   ___   ___   ___   ___   ___   ___\n";
    std::cout << "  /\\  \\ /\\  \\ /\\  \\ /\\  \\ /\\  \\ /\\  \\ /\\  \\ /\\  \\ /\\  \\\n";
    std::cout << " /::\\  \\\\:\\  \\\\:\\  \\\\:\\  \\\\:\\  \\\\:\\  \\\\:\\  \\\\:\\  \\\\:\\  \\\n";
    std::cout << "/::\\:\\__\\\\:\\__\\\\:\\__\\\\:\\__\\\\:\\__\\\\:\\__\\\\:\\__\\\\:\\__\\\\:\\__\\\n";
    std::cout << "\\/\\::/  //:/  //:/  //:/  //:/  //:/  //:/  //:/  //:/  /\n";
    std::cout << "  /:/  //:/  //:/  //:/  //:/  //:/  //:/  //:/  //:/  /\n";
    std::cout << "  \\/__/ \\/__/ \\/__/ \\/__/ \\/__/ \\/__/ \\/__/ \\/__/ \\/__/\n";
    std::cout << "              A R A R A R Q U I V O S\n";
    std::cout << "                   M  E  N  U\n" << RESET;

    std::cout << "\n1 - Enviar arquivo .txt para o AraraMestre\n";
    std::cout << "0 - Sair\n";
    std::cout << "Escolha uma opção: ";
}

int main() {
    int op = -1;
    while (op != 0) {
        printMenu();
        std::cin >> op;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (op == 1) {
            std::string filename;
            std::cout << BLUE << "\nDigite o caminho do arquivo .txt para enviar ao AraraMestre: " << RESET;
            std::getline(std::cin, filename);

            printStatus("Lendo arquivo...");
            std::ifstream file(filename);
            if (!file) {
                printError("Não foi possível abrir o arquivo: " + filename);
                continue;
            }

            std::stringstream buffer;
            buffer << file.rdbuf(); 

            printStatus("Enviando arquivo para o AraraMestre...");
            httplib::Client cli("http://localhost:8080"); // Porta do mestre
            auto res = cli.Post("/process", buffer.str(), "text/plain");

            if (res && res->status == 200) {
                printStatus("Resposta recebida do AraraMestre!");
                std::cout << BLUE << BOLD << res->body << RESET << std::endl;
            } else {
                printError("Erro na comunicação com o AraraMestre.\nVerifique se o mestre está rodando e tente novamente.");
            }
        } else if (op == 0) {
            printStatus("Saindo do AraraArquivos. Até logo!");
        } else {
            printError("Opção inválida. Tente novamente.");
        }
    }

    return 0;
}
