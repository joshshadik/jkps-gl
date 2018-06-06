#include "app.h"

#include <vector>

#include <json.hpp>

#include <glm/gtc/matrix_transform.hpp>

void App::init()
{
    printf("app version %f \n", 0.01f);
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
    _quadGeo = Geometry(vertexData, quadVertexIndices);

    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -2.15f));
    auto projection = glm::perspective(1.2f, 1.0f, 0.1f, 100.0f);

    MaterialUniformBlock::Descriptor globalDescriptor{
        {{"view", 0}, {"projection", sizeof(glm::mat4)}},
        sizeof(glm::mat4) * 2
    };

    _globalUniformBlock = MaterialUniformBlock(globalDescriptor);
    _globalUniformBlock.setValue("view", &view, sizeof(glm::mat4));
    _globalUniformBlock.setValue("projection", &projection, sizeof(glm::mat4));

    _globalUniformBlock.uploadData();

    _globalUniformBlock.bind(0);


    Shader::loadFromFile(&vs, "resources/shaders/standard.vs", Shader::Vertex); //std::make_shared<Shader>(vsSrc, Shader::Vertex);
    Shader::loadFromFile(&fs, "resources/shaders/standard.fs", Shader::Fragment); //std::make_shared<Shader>(fsSrc, Shader::Fragment);

    program = ShaderProgram(&vs, &fs);
    material = Material(&program);

    boxMesh = Mesh(&_quadGeo, &material);

    _modelRot = glm::quat_cast(glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));
    _modelScale = glm::vec3(0.001f);

    _modelMtx =  glm::scale(glm::mat4_cast(_modelRot), _modelScale);

    _gltfModel = std::make_unique<GLTFModel>();
    GLTFModel::loadFromFile(_gltfModel.get(), "resources/models/venus_de_milo/scene.gltf", &program);
    _gltfModel->setMatrix( _modelMtx );

    //_gltfModel.setMatrix(glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));

    Shader::loadFromFile(&composeVS, "resources/shaders/compose.vs", Shader::Vertex);
    Shader::loadFromFile(&composeFS, "resources/shaders/compose.fs", Shader::Fragment);

    composeProgram = ShaderProgram(&composeVS, &composeFS);
    _composeMaterial = Material(&composeProgram);
    _composeMesh = Mesh(&_quadGeo, &_composeMaterial);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);


}

void App::render()
{
    _screenBuffer.bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _gltfModel->render();

    //boxMesh->render(glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, -1.0)));

    Framebuffer::bindDefault(_screenSize);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _composeMesh.render();

//#ifdef _DEBUG
//    GLenum er = glGetError();
//    if (er != 0)
//    {
//        printf("error: %d \n", er);
//    }
//#endif
}

void App::update(double dt)
{
    if (_controls->buttonHeld(Controls::MOUSE_LEFT))
    {
        float rotAmount = _controls->deltaMousePos().x * 90.0f * (float)dt;
        _modelRot = glm::angleAxis(rotAmount, glm::vec3(0.0, 1.0, 0.0)) * _modelRot;
        
        _modelMtx = glm::scale(glm::mat4_cast(_modelRot), _modelScale);

        _gltfModel->setMatrix(_modelMtx);
    }
}

void App::setControls(Controls * controls)
{
    _controls = controls;
}

void App::resize(const glm::ivec2 & size)
{
    _screenSize = size;

    auto projection = glm::perspective(1.2f, size.x / (float)size.y, 0.1f, 100.0f);
    _globalUniformBlock.setValue("projection", &projection, sizeof(glm::mat4));
    _globalUniformBlock.uploadData();

    _colorScreenTextures.clear();
    _colorScreenTextures.push_back(Texture( size, GL_RGBA8, GL_RGBA));
    _colorScreenTextures.push_back(Texture( size, GL_RGBA32F, GL_RGBA, GL_FLOAT));
    _colorScreenTextures.push_back(Texture(size, GL_RGBA8, GL_RGBA));
    _colorScreenTextures.push_back(Texture(size, GL_RGBA8, GL_RGBA));

    _depthTexture = Texture( size, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);

    _screenBuffer = Framebuffer(&_colorScreenTextures, &_depthTexture, size);

    _composeMaterial.setUniform(_composeMaterial.getUniformLocation("uColorTex"), &_colorScreenTextures[0]);
    _composeMaterial.setUniform(_composeMaterial.getUniformLocation("uPositionTex"), &_colorScreenTextures[1]);
    _composeMaterial.setUniform(_composeMaterial.getUniformLocation("uNormalTex"), &_colorScreenTextures[2]);
    _composeMaterial.setUniform(_composeMaterial.getUniformLocation("uMetalRoughOccTex"), &_colorScreenTextures[3]);

    _composeMaterial.setUniform(_composeMaterial.getUniformLocation("uLightDirection"), glm::normalize(glm::vec3(0.0, -1.0, -1.0)));

    Framebuffer::bindDefault();
}
