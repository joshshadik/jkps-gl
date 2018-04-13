#include "gltfModel.h"

#include <iostream>

using namespace jkps::gl;

static std::string GetFilePathExtension(const std::string &FileName) {
	if (FileName.find_last_of(".") != std::string::npos)
		return FileName.substr(FileName.find_last_of(".") + 1);
	return "";
}

GLTFModel::GLTFModel(const tinygltf::Model & model)
{


	for (const auto& mesh : model.meshes)
	{
		for (const auto& primitive : mesh.primitives)
		{
			Geometry::VertexData vBuffers;

			for (const auto& attribute : primitive.attributes)
			{
				const tinygltf::Accessor& accessor = model.accessors[attribute.second];
				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				uint32_t elementSize = tinygltf::GetTypeSizeInBytes(accessor.type);
				uint32_t floatSize = bufferView.byteLength / sizeof(float);
				std::vector<float> vData(floatSize);

				memcpy(vData.data(), buffer.data.data() + bufferView.byteOffset + accessor.byteOffset, bufferView.byteLength);

				VertexLayout layout(
					{ VertexLayout::VertexAttribute(vBuffers.size(), elementSize / sizeof(float), 0) },
					elementSize
				);

				vBuffers.push_back(std::make_pair( vData, layout));
			}

			const tinygltf::Accessor& idxAccessor = model.accessors[primitive.indices];
			const tinygltf::BufferView& idxBufferView = model.bufferViews[idxAccessor.bufferView];
			const tinygltf::Buffer& idxBuffer = model.buffers[idxBufferView.buffer];
		}
	}
	for (auto material : model.materials)
	{
	}
}

std::shared_ptr<GLTFModel> GLTFModel::loadFromFile(const std::string && filename)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF gltf_ctx;

	std::string err;
	std::string ext = GetFilePathExtension(filename);

	bool ret = false;
	if (ext.compare("glb") == 0) {
		std::cout << "Reading binary glTF" << std::endl;
		// assume binary glTF.
		ret = gltf_ctx.LoadBinaryFromFile(&model, &err, filename.c_str());
	}
	else {
		std::cout << "Reading ASCII glTF" << std::endl;
		// assume ascii glTF.
		ret = gltf_ctx.LoadASCIIFromFile(&model, &err, filename.c_str());
	}

	if (!err.empty()) {
		printf("Err: %s\n", err.c_str());
	}

	if (!ret) {
		printf("Failed to parse glTF\n");
		return nullptr;
	}

	return std::make_shared<GLTFModel>(model);
}
