#include "gltfModel.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"

#include <iostream>

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

GLTFModel::GLTFModel(std::unique_ptr<tinygltf::Model> model, std::shared_ptr<ShaderProgram> overrideShader)
	: _model(std::move(model))
{
	// todo - import materials
	auto mat = std::make_shared<Material>(overrideShader);
	_materials.push_back(mat); 

	for (const auto& mesh : _model->meshes)
	{
		//for (const auto& primitive : mesh.primitives)
		{
			// todo : more than 1 primitive
			const auto& primitive = mesh.primitives[0];

			Geometry::VertexData vBuffers;
			Geometry::IndexData iBuffer;

			for (const auto& attribute : primitive.attributes)
			{
				const tinygltf::Accessor& accessor = _model->accessors[attribute.second];
				const tinygltf::BufferView& bufferView = _model->bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = _model->buffers[bufferView.buffer];

				uint32_t elementSize = tinygltf::GetTypeSizeInBytes(accessor.type);
				std::vector<float> vData;

				//memcpy(vData.data(), buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, accessor.count * elementSize * sizeof(float));

				std::copy_n(reinterpret_cast<const float*>(buffer.data.data() + bufferView.byteOffset + accessor.byteOffset), accessor.count * elementSize, std::back_inserter(vData));

				VertexLayout layout(
					{ VertexLayout::VertexAttribute(Shader::StandardAttrLocations[attribute.first], elementSize, 0) },
					elementSize * sizeof(float)
				);

				vBuffers.push_back(std::make_pair( vData, layout));
			}

			const tinygltf::Accessor& idxAccessor = _model->accessors[primitive.indices];
			const tinygltf::BufferView& idxBufferView = _model->bufferViews[idxAccessor.bufferView];
			const tinygltf::Buffer& idxBuffer = _model->buffers[idxBufferView.buffer];
			
			const unsigned char* start = idxBuffer.data.data() + idxBufferView.byteOffset + idxAccessor.byteOffset;
			if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE) 
			{
				std::copy_n(start, idxAccessor.count, std::back_inserter(iBuffer));
			}
			else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) 
			{
				std::copy_n(reinterpret_cast<const unsigned short*>(start), idxAccessor.count, std::back_inserter(iBuffer));
			}
			else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) 
			{
				std::copy_n(reinterpret_cast<const unsigned int*>(start), idxAccessor.count, std::back_inserter(iBuffer));
			}

			auto geo = std::make_shared<Geometry>(vBuffers, iBuffer);
			_geometries.push_back(geo);

			auto m = std::make_shared<Mesh>(geo, mat);
			_meshes.push_back(m);
		}


	}

	const auto& scene = _model->scenes[_model->defaultScene];
	
	for (const auto nId : scene.nodes) // todo : more than default scene
	{
		const auto& node = _model->nodes[nId];
		
		importNode(node);
	}
}

std::shared_ptr<GLTFModel> GLTFModel::loadFromFile(const std::string && filename, std::shared_ptr<ShaderProgram> overrideShader)
{
	auto model = std::make_unique<tinygltf::Model>();
	tinygltf::TinyGLTF gltf_ctx;

	std::string err;
	std::string ext = GetFilePathExtension(filename);

	bool ret = false;
	if (ext.compare("glb") == 0) {
		std::cout << "Reading binary glTF" << std::endl;
		// assume binary glTF.
		ret = gltf_ctx.LoadBinaryFromFile(model.get(), &err, filename.c_str());
	}
	else {
		std::cout << "Reading ASCII glTF" << std::endl;
		// assume ascii glTF.
		ret = gltf_ctx.LoadASCIIFromFile(model.get(), &err, filename.c_str());
	}

	if (!err.empty()) {
		printf("Err: %s\n", err.c_str());
	}

	if (!ret) {
		printf("Failed to parse glTF\n");
		return nullptr;
	}

	return std::make_shared<GLTFModel>(std::move(model), overrideShader);
}

void jkps::gl::GLTFModel::render(std::shared_ptr<MaterialUniformBlock> ubo, const size_t modelOffset)
{
	const auto& scene = _model->scenes[_model->defaultScene];
	for (auto id : scene.nodes)
	{
		renderTreeFromNode(id, glm::mat4(), ubo, modelOffset);
	}
}

void jkps::gl::GLTFModel::importNode(const tinygltf::Node & node)
{

	for (const auto nId : node.children)
	{
		const auto& n = _model->nodes[nId];
		importNode(n);
	}
}

void jkps::gl::GLTFModel::renderTreeFromNode(int nId, const glm::mat4& parentMtx, std::shared_ptr<MaterialUniformBlock> ubo, const size_t modelOffset)
{
	const auto& node = _model->nodes[nId];

	glm::mat4 mtx;

	if (node.matrix.size() == 16)
	{
		std::copy_n(node.matrix.data(), 16, reinterpret_cast<float*>(&mtx));
	}

	mtx = parentMtx; // *mtx;

	if (node.mesh >= 0)
	{
		//ubo->setValue(&mtx, modelOffset, sizeof(mtx));
		//ubo->uploadData();
		//ubo->bind(0);
		_meshes[node.mesh]->render();
	}

	for (const auto id : node.children)
	{
		renderTreeFromNode(id, mtx, ubo, modelOffset);
	}
}
