#include <httplib.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <future>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

// Função segura para conversão de string para int
int safe_stoi(const std::string &str) {
    try {
        if (str.empty()) return 0;
        return std::stoi(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: resposta invalida do escravo: '" << str << "'" << std::endl;
        return 0;
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: numero fora do range: '" << str << "'" << std::endl;
        return 0;
    }
}

bool check_slave_health(const std::string &url) {
    httplib::Client cli(url.c_str());
    cli.set_connection_timeout(5, 0);  // 5 segundos timeout
    cli.set_read_timeout(5, 0);        // 5 segundos timeout
    auto res = cli.Get("/health");
    return res && res->status == 200;
}

std::string call_slave(const std::string &url, const std::string &endpoint, const std::string &text) {
    httplib::Client cli(url.c_str());
    cli.set_connection_timeout(10, 0);  // 10 segundos timeout para conexão
    cli.set_read_timeout(10, 0);        // 10 segundos timeout para leitura
    auto res = cli.Post(endpoint.c_str(), text, "text/plain");
    if (res && res->status == 200) return res->body;
    return "0";
}

int main() {
    httplib::Server svr;

    // Endpoint de health check para o mestre
    svr.Get("/health", [](const httplib::Request&, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    svr.Post("/process", [](const httplib::Request &req, httplib::Response &res) {
        std::string text = req.body;

        // Verifica saúde dos escravos antes de enviar requisições
        bool slave1_healthy = check_slave_health("http://slave1:5001");
        bool slave2_healthy = check_slave_health("http://slave2:5002");

        if (!slave1_healthy || !slave2_healthy) {
            json error_result;
            error_result["error"] = "Um ou mais escravos nao estao disponiveis";
            error_result["slave1_status"] = slave1_healthy ? "OK" : "UNAVAILABLE";
            error_result["slave2_status"] = slave2_healthy ? "OK" : "UNAVAILABLE";
            res.status = 503; // Service Unavailable
            res.set_content(error_result.dump(), "application/json");
            return;
        }

        // Dispara threads para escravos
        auto f1 = std::async(std::launch::async, call_slave, "http://slave1:5001", "/letras", text);
        auto f2 = std::async(std::launch::async, call_slave, "http://slave2:5002", "/numeros", text);

        // Espera resultados
        std::string letras = f1.get();
        std::string numeros = f2.get();

        // Resposta JSON
        json result;
        result["letras"] = safe_stoi(letras);
        result["numeros"] = safe_stoi(numeros);

        res.set_content(result.dump(), "application/json");
    });

    std::cout << "Mestre rodando na porta 8080..." << std::endl;
    
    // Verifica status dos escravos na inicialização
    std::cout << "Verificando status dos escravos..." << std::endl;
    bool slave1_status = check_slave_health("http://slave1:5001");
    bool slave2_status = check_slave_health("http://slave2:5002");
    
    std::cout << "Slave1 (http://slave1:5001): " << (slave1_status ? "ONLINE" : "OFFLINE") << std::endl;
    std::cout << "Slave2 (http://slave2:5002): " << (slave2_status ? "ONLINE" : "OFFLINE") << std::endl;
    
    if (slave1_status && slave2_status) {
        std::cout << "Todos os escravos estao online! Sistema pronto." << std::endl;
    } else {
        std::cout << "AVISO: Alguns escravos estao offline. O sistema pode nao funcionar corretamente." << std::endl;
    }

    svr.listen("0.0.0.0", 8080);
}
