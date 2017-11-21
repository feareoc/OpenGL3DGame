#pragma once

#include <GL\glew.h>
#include <glm\mat4x4.hpp>

#include <unordered_map>

#include "Entity.h"
#include "EntityShader.h"
#include "TexturedModel.h"

class EntityRenderer
{
private:
	EntityShader m_shader;

	void bindTexturedModel(const TexturedModel& model)
	{
		m_shader.setShineVariables(model.texture.reflectivity, model.texture.shineDamper);
		glBindVertexArray(model.model.getID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.texture.getID());
	}

	void unbindTexturedModel()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}

	void prepareInstance(const Entity& entity)
	{
		glm::mat4 model = entity.getModelMatrix();
		m_shader.setModelMatrix(entity.getModelMatrix());
	}
public:
	EntityRenderer(const EntityShader& shader) : m_shader(shader)
	{ 
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void prepare()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void render(std::unordered_map<TexturedModel, std::vector<Entity>>& entities)
	{
		for (auto& t : entities)
		{
			bindTexturedModel(t.first);
			for (Entity e : t.second)
			{
				prepareInstance(e);
				glDrawElements(GL_TRIANGLES, t.first.model.numVertices, GL_UNSIGNED_INT, nullptr);
			}
			unbindTexturedModel();
		}
	}

	void stop()
	{
		glDisable(GL_DEPTH_TEST);
	}
};