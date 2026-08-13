#include "ShaderUniforms.h"
#include <vector>
#include "../../Utils/Logger/Logger.h"

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
                LOG("WARNING::UNIFORM_LOCATION_NOT_FOUND of name: %s\n While cache invalidating, probably new shader program was not a copy from the previous one", name.c_str());
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
            LOG("WARNING::UNIFORM_LOCATION_NOT_FOUND of name: %s", name.c_str());
        }
        return location;
    }
    return it->second;
}

bool ShaderUniforms::IsProgramSet() const {
    return program != 0;
}
