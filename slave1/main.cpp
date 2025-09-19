#include <httplib.h>
#include <cctype>
#include <string>

int main() {
    httplib::Server svr;

    svr.Post("/letras", [](const httplib::Request &req, httplib::Response &res) {
        int count = 0;
        for (char c : req.body) {
            if (std::isalpha(static_cast<unsigned char>(c))) count++;
        }

        std::cout << "Contagem de letras concluída. Total: " << count << " letras encontradas." << std::endl;

        res.set_content(std::to_string(count), "text/plain");
    });

    svr.Get("/health", [](const httplib::Request&, httplib::Response &res) {
        res.set_content("OK", "text/plain");
    });

    svr.listen("0.0.0.0", 5001);
}
