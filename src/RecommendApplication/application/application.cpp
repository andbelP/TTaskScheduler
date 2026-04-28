#include <expected>
#include <string>

#include "application.hpp"

std::expected<void, std::string> Application::Run(){
    return std::expected<void, std::string>{};
}