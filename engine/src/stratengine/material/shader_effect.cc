#include "stratengine/material/shader_effect.h"
#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <iostream>
// cppcheck-suppress unknownMacro
STRATE_INNER_NAMESPACE(material)

bgfx::ProgramHandle ShaderEffect::GetProgram() const { return program_; }

void ShaderEffect::AddStage(const std::string& shader_path, const std::string& type) {
    bx::FileReader reader;
    bx::open(&reader, shader_path.c_str());
    uint32_t size = (uint32_t)bx::getSize(&reader);
    const bgfx::Memory* mem = bgfx::alloc(size + 1);
    bx::Error err;
    bx::read(&reader, mem->data, size, &err);
    bx::close(&reader);
    mem->data[mem->size - 1] = '\0';

    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    stages_.push_back({handle, type});
}

void ShaderEffect::Build() {
    if (stages_.size() == 2) {
        program_ = bgfx::createProgram(stages_[0].shader_handle, stages_[1].shader_handle, true);
    } else if (stages_.size() == 1) {
        program_ = bgfx::createProgram(stages_[0].shader_handle, true);
    } else {
        // Handle other cases
        std::cerr << "Unsupported number of shader stages" << std::endl;
    }
}


// void ShaderEffect::Build() {
//     std::vector<bgfx::ShaderHandle> handles;
//     for (const auto& stage : stages_) {
//         handles.push_back(stage.shader_handle);
//     }
//     program_ = bgfx::createProgram(handles.data(), static_cast<uint16_t>(handles.size()), true);
// }

// void ShaderEffect::AddStage(const std::string& shader_path, const std::string& type) {
//     bx::FileReader reader;
//     bx::open(&reader, shader_path.c_str());
//     uint32_t size = (uint32_t)bx::getSize(&reader);
//     const bgfx::Memory* mem = bgfx::alloc(size + 1);
//     bx::read(&reader, mem->data, size);
//     bx::close(&reader);
//     mem->data[mem->size - 1] = '\0';

//     bgfx::ShaderHandle handle = bgfx::createShader(mem);
//     stages_.push_back({handle, type});
// }

// void ShaderEffect::Build() {
//     if (stages_.size() == 2) {
//         program_ = bgfx::createProgram(stages_[0].shader_handle, stages_[1].shader_handle, true);
//     } else if (stages_.size() == 1) {
//         program_ = bgfx::createProgram(stages_[0].shader_handle, true);
//     } else if (stages_.size() == 3) {
//         program_ = bgfx::createProgram(stages_[0].shader_handle, stages_[1].shader_handle, stages_[2].shader_handle, true);
//     } else {
//         // Handle other cases
//         std::cerr << "Unsupported number of shader stages" << std::endl;
//     }
// }

STRATE_INNER_END_NAMESPACE