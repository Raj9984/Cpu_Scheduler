#define CROW_MAIN
#include "crow_all.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

void runSchedulingAlgorithm() {
    system("completeScheduling.exe");
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)([](){
        return crow::response("<html><body>"
                              "<h1>Upload CPU Scheduling Input File</h1>"
                              "<form action='/upload' method='post' enctype='multipart/form-data'>"
                              "<input type='file' name='file'>"
                              "<input type='submit' value='Upload'>"
                              "</form></body></html>");
    });

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        auto file = req.get_part("file");
        if (!file) {
            return crow::response(400, "No file uploaded");
        }

        // Save the uploaded file to input.txt
        std::ofstream ofs("input.txt", std::ios::binary);
        ofs.write(file->body.data(), file->body.size());
        ofs.close();

        // Run the scheduling algorithm
        runSchedulingAlgorithm();

        // Read the output file and return its contents
        std::ifstream ifs("output.txt");
        std::stringstream buffer;
        buffer << ifs.rdbuf();

        return crow::response(buffer.str());
    });

    app.port(8080).multithreaded().run();
}
