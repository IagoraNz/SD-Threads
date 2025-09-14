#include <httplib.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <future>
#include <iostream>

using json = nlohmann::json;

std::string call_slave(const std::string &url, const std::string &endpoint, const std::string &text) {
    httplib::Client cli(url.c_str());
    auto res = cli.Post(endpoint.c_str(), text, "text/plain");
    if (res && res->status == 200) return res->body;
    return "0";
}

int main() {
    httplib::Server svr;

    svr.Post("/process", [](const httplib::Request &req, httplib::Response &res) {
        std::string text = req.body;

        // Dispara threads para escravos
        auto f1 = std::async(std::launch::async, call_slave, "http://slave1:5001", "/letras", text);
        auto f2 = std::async(std::launch::async, call_slave, "http://slave2:5002", "/numeros", text);

        // Espera resultados
        std::string letras = f1.get();
        std::string numeros = f2.get();

        // Resposta JSON
        json result;
        result["letras"] = std::stoi(letras);
        result["numeros"] = std::stoi(numeros);

        res.set_content(result.dump(), "application/json");
    });

    std::cout << "Mestre rodando na porta 8080...\n";
    svr.listen("0.0.0.0", 8080);
}
