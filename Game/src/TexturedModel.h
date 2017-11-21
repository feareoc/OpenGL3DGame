#pragma once

#include "Model.h"
#include "Texture.h"

class TexturedModel
{
public:
	Model model;
	Texture texture;
public:
	TexturedModel(const Model& model, const Texture& texture) : model(model), texture(texture)
	{}

	Model getModel() const
	{
		return model;
	}

	Texture getTexture() const
	{
		return texture;
	}

	bool operator==(const TexturedModel& other) const
	{
		return (model.getID() == other.model.getID() && texture.getID() == other.texture.getID());
	}
};

//TODO: Make the key in the hash map pointers instead, easier to compare, no need for custom functions
namespace std
{
	template<>
	struct hash<TexturedModel>
	{
		std::size_t operator()(const TexturedModel& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<unsigned int>()(k.model.getID()) ^ (hash<unsigned int>()(k.texture.getID()) << 1)) >> 1);
		}
	};
}