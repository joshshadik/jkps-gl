#include "app.h"

#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

void App::init()
{
    std::vector<float> quadVertices{
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
    };

    std::vector<float> quadTexcoords{
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    std::vector<uint32_t> quadVertexIndices{
        0, 1, 2,
        0, 2, 3
    };

    VertexLayout vertLayout{
        { VertexLayout::VertexAttribute(Shader::StandardAttrLocations["POSITION"], 3, 0) },
        3 * sizeof(GLfloat)
    };
    VertexLayout texcoordLayout{
        { VertexLayout::VertexAttribute(Shader::StandardAttrLocations["TEXCOORD_0"], 2, 0)},
        2 * sizeof(GLfloat)
    };

    Geometry::VertexData vertexData = { std::make_pair(quadVertices, vertLayout), std::make_pair(quadTexcoords, texcoordLayout) };
    box = std::make_shared<Geometry>(vertexData, quadVertexIndices);

    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -2.15f));
    auto projection = glm::perspective(1.2f, 1.0f, 0.1f, 100.0f);

    MaterialUniformBlock::Descriptor globalDescriptor{
        {{"view", 0}, {"projection", sizeof(glm::mat4)}},
        sizeof(glm::mat4) * 2
    };

    _globalUniformBlock = std::make_shared<MaterialUniformBlock>(globalDescriptor);
    _globalUniformBlock->setValue("view", &view, sizeof(glm::mat4));
    _globalUniformBlock->setValue("projection", &projection, sizeof(glm::mat4));

    _globalUniformBlock->uploadData();

    //_globalUniformBlock = std::make_shared<MaterialUniformBlock>(&_gUniforms, sizeof(GlobalUniforms));

    _globalUniformBlock->bind(0);


    vs = Shader::loadFromFile("./resources/shaders/standard.vs", Shader::Vertex); //std::make_shared<Shader>(vsSrc, Shader::Vertex);
    fs = Shader::loadFromFile("./resources/shaders/standard.fs", Shader::Fragment); //std::make_shared<Shader>(fsSrc, Shader::Fragment);

    program = std::make_shared<ShaderProgram>(vs, fs);
    material = std::make_shared<Material>(program);

    boxMesh = std::make_shared<Mesh>(box, material);


    _gltfModel = GLTFModel::loadFromFile("./resources/models/venus_de_milo/scene.gltf", program);
    _gltfModel->setMatrix(
        glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::scale(glm::mat4(), glm::vec3(0.001f))
    );

    //_gltfModel->setMatrix(glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    GLenum er = glGetError();
    if (er != 0)
    {
        std::cout << "error: " << er << '\n';
    }
}

void App::render()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static float test = -1.0f;

    //test -= 0.01;

    //_gUniforms._model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, test));
    //_globalUniformBlock->uploadData();

    //boxMesh->render();

    _gltfModel->render();

#ifdef _DEBUG
    GLenum er = glGetError();
    if (er != 0)
    {
        std::cout << "gl error: " << er << '\n';
    }
#endif
}
