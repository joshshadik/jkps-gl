#include "app.h"

#include "resourceManager.h"
#include "primitives.h"

#include <vector>

#include <json.hpp>

#include <glm/gtc/matrix_transform.hpp>


void App::init()
{
    Primitives::init();
    _quadGeo = Primitives::quad();

    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    auto projection = glm::perspective(1.2f, 1.0f, 0.1f, 100.0f);

    MaterialUniformBlock::Descriptor globalDescriptor{
        {{"view", 0}, {"projection", sizeof(glm::mat4)}},
        sizeof(glm::mat4) * 2
    };

    _globalUniformBlock = ResourceManager::getNextUniformBlock();

    *_globalUniformBlock = MaterialUniformBlock(globalDescriptor);
    _globalUniformBlock->setValue("view", &view, sizeof(glm::mat4));
    _globalUniformBlock->setValue("projection", &projection, sizeof(glm::mat4));

    _globalUniformBlock->uploadData();

    _globalUniformBlock->bind(0);

    vs = ResourceManager::getNextShader();
    fs = ResourceManager::getNextShader();

    Shader::loadFromFile(vs, "resources/shaders/standard.vs", Shader::Vertex); //std::make_shared<Shader>(vsSrc, Shader::Vertex);
    Shader::loadFromFile(fs, "resources/shaders/standard.fs", Shader::Fragment); //std::make_shared<Shader>(fsSrc, Shader::Fragment);

    program = ResourceManager::getNextShaderProgram();
    *program = ShaderProgram(vs, fs);

    material = ResourceManager::getNextMaterial();
    *material = Material(program);

    boxMesh = ResourceManager::getNextMesh();
    *boxMesh = Mesh(_quadGeo, material);

    _modelPos = glm::vec3(0.0f, 2.0f, 0.0f);
    _modelRot = glm::quat_cast(glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));
    _modelScale = glm::vec3(0.001f);

    _modelMtx =  glm::scale(glm::translate(glm::mat4_cast(_modelRot), _modelPos), _modelScale);

    GLTFModel::loadFromFile(&_gltfModel, "resources/models/venus_de_milo/scene.gltf", program);
    _gltfModel.setMatrix( _modelMtx );

    //_gltfModel.setMatrix(glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f)));

    Shader* sprayUpdateVS = ResourceManager::getNextShader();
    Shader* sprayUpdateFS = ResourceManager::getNextShader();
    Shader* sprayRenderVS = ResourceManager::getNextShader();
    Shader* sprayRenderFS = ResourceManager::getNextShader();

    Shader::loadFromFile(sprayUpdateVS, "resources/shaders/screen.vs", Shader::Vertex);
    Shader::loadFromFile(sprayUpdateFS, "resources/shaders/spray/pos-vel.fs", Shader::Fragment);
    Shader::loadFromFile(sprayRenderVS, "resources/shaders/spray/particle.vs", Shader::Vertex);
    Shader::loadFromFile(sprayRenderFS, "resources/shaders/spray/particle.fs", Shader::Fragment);

    ShaderProgram* updateProgram = ResourceManager::getNextShaderProgram();
    ShaderProgram* sprayProgram = ResourceManager::getNextShaderProgram();
    Material* updateMaterial = ResourceManager::getNextMaterial();
    _sprayMaterial = ResourceManager::getNextMaterial();

    *updateProgram = ShaderProgram(sprayUpdateVS, sprayUpdateFS);
    *sprayProgram = ShaderProgram(sprayRenderVS, sprayRenderFS);
    *updateMaterial = Material(updateProgram);
    *_sprayMaterial = Material(sprayProgram);

    _sprayParticles.init(Primitives::quad(), _sprayMaterial, updateMaterial, 64);
    int diffLoc = _sprayMaterial->getUniformLocation("diffuseColor");
    _sprayMaterial->setUniform(diffLoc, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    

    _sprayParticles.lifetime(0.5f);
    _sprayParticles.direction(glm::vec3(1.0f, 1.0f, -1.0f));
    _sprayParticles.magnitude(20.0f);
    _sprayParticles.randomness(0.1f);
    _sprayParticles.origin(glm::vec3(-0.5f, 0.5f, -1.5f));


    composeVS = ResourceManager::getNextShader();
    composeFS = ResourceManager::getNextShader();

    Shader::loadFromFile(composeVS, "resources/shaders/compose.vs", Shader::Vertex);
    Shader::loadFromFile(composeFS, "resources/shaders/compose.fs", Shader::Fragment);

    composeProgram = ResourceManager::getNextShaderProgram();
    _composeMaterial = ResourceManager::getNextMaterial();
    _composeMesh = ResourceManager::getNextMesh();

    *composeProgram = ShaderProgram(composeVS, composeFS);
    *_composeMaterial = Material(composeProgram);
    *_composeMesh = Mesh(_quadGeo, _composeMaterial);

    for (int i = 0; i < 4; ++i)
    {
        _colorScreenTextures.push_back(ResourceManager::getNextTexture());
    }

    _depthTexture = ResourceManager::getNextTexture();

    _screenBuffer = ResourceManager::getNextFramebuffer();

    _composeMaterial->setUniform(_composeMaterial->getUniformLocation("uColorTex"), _colorScreenTextures[0]);
    _composeMaterial->setUniform(_composeMaterial->getUniformLocation("uPositionTex"), _colorScreenTextures[1]);
    _composeMaterial->setUniform(_composeMaterial->getUniformLocation("uNormalTex"), _colorScreenTextures[2]);
    _composeMaterial->setUniform(_composeMaterial->getUniformLocation("uMetalRoughOccTex"), _colorScreenTextures[3]);

    _composeMaterial->setUniform(_composeMaterial->getUniformLocation("uLightDirection"), glm::normalize(glm::vec3(0.0, -1.0, -1.0)));


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

#ifdef _DEBUG
    GLenum er = glGetError();
    if (er != 0)
    {
        printf("error: %d \n", er);
    }
#endif
}

void App::render(const glm::ivec4& viewport)
{
    _screenBuffer->bind();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _gltfModel.render();

    _sprayParticles.render();

    //boxMesh->render(glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, -1.0)));

    Framebuffer::bindDefaultVP(viewport);


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _composeMesh->render();

#ifdef _DEBUG
    GLenum er = glGetError();
    if (er != 0)
    {
        printf("error: %d \n", er);
    }
#endif
}

void App::update(double dt)
{
    if (_controls->buttonHeld(Controls::MOUSE_LEFT))
    {
        float rotAmount = _controls->deltaMousePos().x * 90.0f * (float)dt;
        _modelRot = glm::angleAxis(rotAmount, glm::vec3(0.0, 1.0, 0.0)) * _modelRot;
        
        _modelMtx = glm::scale(glm::translate(glm::mat4_cast(_modelRot), _modelPos), _modelScale);

        _gltfModel.setMatrix(_modelMtx);
    }

    _sprayParticles.update(dt);

#ifdef _DEBUG
    GLenum er = glGetError();
    if (er != 0)
    {
        printf("error: %d \n", er);
    }
#endif
}

void App::setControls(Controls * controls)
{
    _controls = controls;
}

void App::resize(const glm::ivec2 & size)
{
    _screenSize = size;

    auto projection = glm::perspective(1.2f, size.x / (float)size.y, 0.1f, 100.0f);
    _globalUniformBlock->setValue("projection", &projection, sizeof(glm::mat4));
    _globalUniformBlock->uploadData();

    *_colorScreenTextures[0] = Texture( size, GL_RGBA8, GL_RGBA);
    *_colorScreenTextures[1] = Texture( size, GL_RGBA32F, GL_RGBA, GL_FLOAT);
    *_colorScreenTextures[2] = Texture(size, GL_RGBA8, GL_RGBA);
    *_colorScreenTextures[3] = Texture(size, GL_RGBA8, GL_RGBA);

    *_depthTexture = Texture( size, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT);

    *_screenBuffer = Framebuffer(_colorScreenTextures, _depthTexture, size);

    Framebuffer::bindDefault();

     int loc = _sprayMaterial->getUniformLocation("screenSize");
    _sprayMaterial->setUniform(loc, glm::vec2(_screenSize));
}

void App::overrideViewProjection(const glm::mat4 & view, const glm::mat4 & projection)
{
    _gUniforms._view = view;
    _gUniforms._projection = projection;

    _globalUniformBlock->setValue("view", &view, sizeof(glm::mat4));
    _globalUniformBlock->setValue("projection", &projection, sizeof(glm::mat4));

    _globalUniformBlock->uploadData();
}
