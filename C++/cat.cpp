#include "cat.h"

Cat::Cat(std::string name) : name(name) {
    std::cout << "Constructor of Cat: " << name << std::endl;
}

Cat::~Cat() {
    std::cout << "Destructor of Cat: " << name << std::endl;
}