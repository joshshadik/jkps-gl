#include "gltfModel.h"

using namespace jkps::gl;

bool loadImageData(tinygltf::Image*, std::string*, int, int, const unsigned char*, int, void*) {
    /* Bypass tinygltf image loading and load the image on demand in image2D instead. */
    return true;
}

static std::string GetFilePathExtension(const std::string &FileName) {
    if (FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".") + 1);
    return "";
}

int getTextureIndex(const tinygltf::Material& material, tinygltf::ExtensionMap::const_iterator pbrExt, const std::string& valueName, const std::string& pbrName)
{
    int texIndex = -1;

    auto diffuseIt = material.values.find(valueName);
    if (diffuseIt != material.values.end())
    {
        texIndex = diffuseIt->second.TextureIndex();
    }
    else
    {
        auto additionalIt = material.additionalValues.find(valueName);
        if (additionalIt != material.additionalValues.end())
        {
            texIndex = additionalIt->second.TextureIndex();
        }
        else if (pbrExt != material.extensions.end())
        {
            auto diffObj = pbrExt->second.Get(pbrName);
            if (diffObj.Type() && diffObj.IsObject())
            {
                texIndex = diffObj.Get("index").Get<int>();
            }
        }
    }

    return texIndex;
}

GLTFModel::GLTFModel(tinygltf::Model&& model, ShaderProgram* overrideShader)
    : _model(std::move(model))
{
    for (const auto& image : _model.images)
    {
        GLuint format, layout;
        switch (image.component)
        {
        case 3:
            format = GL_RGB8;
            layout = GL_RGB;
            break;

        case 4:
            format = GL_RGBA8;
            layout = GL_RGBA;
            break;
        }
        auto tex = Texture(image.image, glm::ivec2(image.width, image.height), format, layout);
        _textures.push_back(std::move(tex));
    }

    for (const auto& material : _model.materials)
    {
        auto mat = Material(overrideShader);
        auto pbrExt = material.extensions.find("KHR_materials_pbrSpecularGlossiness");

        int diffIndex = getTextureIndex(material, pbrExt, "baseColorTexture", "diffuseTexture");
        auto loc = mat.getUniformLocation("uDiffuse");

        if (diffIndex >= 0)
        {        
            mat.setUniform(loc, &_textures[diffIndex]);
        }
        else
        {
            std::vector<uint8_t> texData;
            glm::ivec2 size(512, 512);
            texData.assign(size.x * size.y * 3, 255);
            auto tex = Texture(texData, size, GL_RGB8, GL_RGB);
            _textures.push_back(std::move(tex));
            mat.setUniform(loc, &_textures.back());
        }

        int normalIndex = getTextureIndex(material, pbrExt, "normalTexture", "normalTexture");
        auto nLoc = mat.getUniformLocation("uNormalTex");

        if (normalIndex >= 0)
        {
            mat.setUniform(nLoc, &(_textures[normalIndex]));
        }


        int occIndex = getTextureIndex(material, pbrExt, "occlusionTexture", "occlusionTexture");
        auto oLoc = mat.getUniformLocation("uOcclusionTex");

        if (occIndex >= 0)
        {
            mat.setUniform(oLoc, &_textures[occIndex]);
        }

        glm::vec4 diffuseFactorColor{ 1.0f, 1.0f, 1.0f, 1.0f };

        static const std::string diffuseFactorKey = "diffuseFactor";
        static const std::string metallicFactorKey = "metallicFactor";
        static const std::string roughnessFactorKey = "roughnessFactor";

        static const MaterialUniformBlock::Descriptor pbrDescriptor {
            { { diffuseFactorKey, 0 },{ metallicFactorKey, sizeof(glm::vec4) },{ roughnessFactorKey, sizeof(glm::vec4) + sizeof(float) } },
            sizeof(glm::vec4) + sizeof(float) * 4
        };


        auto ubo = MaterialUniformBlock(pbrDescriptor);

        if (pbrExt != material.extensions.end())
        {          
            auto diffFactor = pbrExt->second.Get(diffuseFactorKey);
            if (diffFactor.Type())
            {
                auto arrValues = diffFactor.Get<tinygltf::Value::Array>();

                for (int i = 0; i < arrValues.size(); ++i)
                {
                    diffuseFactorColor[i] = (float)arrValues.at(i).Get<int>();
                }
            }
        }
        else
        {
            printf("looking for baseColorFactor \n");
            auto diffuseIt = material.values.find("baseColorFactor");
            if (diffuseIt != material.values.end())
            {
                printf("found baseColorFactor \n");
                auto diffFactor = diffuseIt->second.ColorFactor();

                for (int i = 0; i < 4; ++i)
                {
                    diffuseFactorColor[i] = (float)diffFactor[i];
                }
            }
        }

        printf("diffuse color factor: %f, %f, %f, %f \n", diffuseFactorColor[0], diffuseFactorColor[1], diffuseFactorColor[2], diffuseFactorColor[3]);

        ubo.setValue(diffuseFactorKey, &diffuseFactorColor, sizeof(diffuseFactorColor));
        ubo.uploadData();

        _ubos.push_back(std::move(ubo));
        GLint pbrLoc = overrideShader->getUniformBlockLocation("PBR");
        printf("pbr loc: %d \n", pbrLoc);
        mat.addUniformBlock(1, pbrLoc, &_ubos.back());

        auto alphaMode = material.additionalValues.find("alphaMode");
        if (alphaMode != material.additionalValues.end())
        {
            std::string mode = alphaMode->second.string_value;

            if (mode == "BLEND")
            {
                mat.setBlended(true);
                mat.setBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
        }

        _materials.push_back(std::move(mat));
    }

    for (const auto& mesh : _model.meshes)
    {
        MeshGroup meshGroup;

        for (const auto& primitive : mesh.primitives)
        {
            Geometry::VertexData vBuffers;
            Geometry::IndexData iBuffer;

            for (const auto& attribute : primitive.attributes)
            {
                GLuint attrLoc;
                if (Shader::getStandardAttribute(attribute.first, attrLoc))
                {
                    const auto& accessor = _model.accessors[attribute.second];
                    const auto& bufferView = _model.bufferViews[accessor.bufferView];
                    const auto& buffer = _model.buffers[bufferView.buffer];

                    uint32_t elementSize = tinygltf::GetTypeSizeInBytes(accessor.type);
                    std::vector<float> vData;

                    std::copy_n(reinterpret_cast<const float*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset), accessor.count * elementSize, std::back_inserter(vData));

                    VertexLayout layout{
                    { VertexLayout::VertexAttribute(attrLoc, elementSize, 0) },
                        (uint16_t)(elementSize * sizeof(float))
                    };

                    vBuffers.push_back(std::make_pair(vData, layout));
                }
            }

            const auto& idxAccessor = _model.accessors[primitive.indices];
            const auto& idxBufferView = _model.bufferViews[idxAccessor.bufferView];
            const auto& idxBuffer = _model.buffers[idxBufferView.buffer];

            const uint8_t* start = idxBuffer.data.data() + idxBufferView.byteOffset + idxAccessor.byteOffset;
            if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE)
            {
                std::copy_n(start, idxAccessor.count, std::back_inserter(iBuffer));
            }
            else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
            {
                std::copy_n(reinterpret_cast<const uint16_t*>(start), idxAccessor.count, std::back_inserter(iBuffer));
            }
            else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
            {
                std::copy_n(reinterpret_cast<const uint32_t*>(start), idxAccessor.count, std::back_inserter(iBuffer));
            }

            auto geo = Geometry(vBuffers, iBuffer);
            _geometries.push_back(std::move(geo));

            auto m = Mesh( &_geometries.back(), &_materials[primitive.material]);
            meshGroup.push_back(std::move(m));
        }
    
        _meshGroups.push_back(std::move(meshGroup));
    }

    const auto& scene = _model.scenes[_model.defaultScene];

    for (const auto nId : scene.nodes) // todo : more than default scene
    {
        const auto& node = _model.nodes[nId];

        importNode(node);
    }
}

bool GLTFModel::loadFromFile(GLTFModel* gltfModel, const std::string && filename, ShaderProgram* overrideShader)
{
    auto model = tinygltf::Model();
    tinygltf::TinyGLTF gltf_ctx;

    std::string err;
    std::string ext = GetFilePathExtension(filename);

    bool ret = false;
    if (ext.compare("glb") == 0) {
        printf("Reading binary glTF\n");
        // assume binary glTF.
        ret = gltf_ctx.LoadBinaryFromFile(&model, &err, filename.c_str());
    }
    else {
        printf("Reading ASCII glTF\n");
        // assume ascii glTF.
        ret = gltf_ctx.LoadASCIIFromFile(&model, &err, filename.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
        return false;
    }

    *gltfModel = std::move(GLTFModel(std::move(model), overrideShader));
    return true;
}

void jkps::gl::GLTFModel::render()
{
    const auto& scene = _model.scenes[_model.defaultScene];
    for (auto id : scene.nodes)
    {
        renderTreeFromNode(id, _matrix);
    }
}

void jkps::gl::GLTFModel::importNode(const tinygltf::Node & node)
{

    for (const auto nId : node.children)
    {
        const auto& n = _model.nodes[nId];
        importNode(n);
    }
}

void jkps::gl::GLTFModel::renderTreeFromNode(int nId, const glm::mat4& parentMtx)
{
    const auto& node = _model.nodes[nId];

    glm::mat4 mtx;

    if (node.matrix.size() == 16)
    {
        std::copy_n(node.matrix.data(), 16, reinterpret_cast<float*>(&mtx));
    }

    mtx = parentMtx * mtx;

    if (node.mesh >= 0)
    {
        for (auto& mesh : _meshGroups[node.mesh])
        {
            mesh.render(mtx);
        }
    }

    for (const auto id : node.children)
    {
        renderTreeFromNode(id, mtx);
    }
}
