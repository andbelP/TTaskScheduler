#pragma once

#include <expected>
#include <string>

class Application{

    int cnt_of_places_;

public:

    Application(int cnt_of_places) : cnt_of_places_(cnt_of_places) {}

    std::expected<void, std::string> Run();

};