#include "ShaderUniforms.h"
#include <iostream>

ShaderUniforms::ShaderUniforms() {
    locations.reserve(UNIFORM_CACHE_INITIAL_SIZE);
}

void ShaderUniforms::SetShaderProgram(GLuint program) {
    bool wasSet = IsProgramSet();
    this->program = program;
    if(!IsProgramSet()) {
        locations.clear();
    } else if(wasSet) {
        std::vector<std::string> toErase;
        for(const auto &[name, location] : locations) {
            GLint newLocation = glGetUniformLocation(program, name.c_str());
            if(newLocation != -1)
                locations[name] = newLocation;
            else {
                toErase.push_back(name);
                std::cout << "WARNING::UNIFORM_LOCATION_NOT_FOUND of name: " << name << "\n While cache invalidating, probably new shader program was not a copy from the previous one" << std::endl;
            }
        }
        for(const auto &name : toErase) {
            locations.erase(name);
        }
    }
}

GLint ShaderUniforms::GetUniformLocation(const std::string &name) const {
    auto it = locations.find(name);
    if(it == locations.end()) {
        GLint location = glGetUniformLocation(program, name.c_str());
        if(location != -1) {
            locations[name] = location;
        } else {
            std::cout << "WARNING::UNIFORM_LOCATION_NOT_FOUND of name: " << name << std::endl;
        }
        return location;
    }
    return it->second;
}

bool ShaderUniforms::IsProgramSet() const {
    return program != 0;
}
