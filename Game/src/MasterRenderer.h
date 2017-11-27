#pragma once

#include <glm\vec3.hpp>

#include <unordered_map>
#include "Entity.h"
#include "EntityRenderer.h"
#include "EntityShader.h"
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "TerrainShader.h"
#include "TexturedModel.h"
#include "SkyboxRenderer.h"

class MasterRenderer
{
private:
	EntityShader entityShader;
	EntityRenderer entityRenderer;

	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer;

	SkyboxRenderer skyboxRenderer;

	std::unordered_map<TexturedModel, std::vector<Entity>> entities;
	std::vector<Terrain> terrains;
	glm::vec3 m_skyColour;
public:
	MasterRenderer(const EntityRenderer& entityRenderer, const TerrainRenderer& terrainRenderer, const EntityShader& entityShader, const TerrainShader& terrainShader) : 
		entityRenderer(entityRenderer), 
		terrainRenderer(terrainRenderer), 
		entityShader(entityShader), 
		terrainShader(terrainShader),
		m_skyColour(glm::vec3(0.5f, 0.5f, 0.5f))
	{ }

	void render(const std::vector<Light>& lights, Camera& camera)
	{
		prepare();
		entityShader.use();
		entityShader.setSkyColour(m_skyColour.x, m_skyColour.y, m_skyColour.z);
		entityShader.setLight(lights);
		entityShader.setViewMatrix(camera.getViewMatrix());
		entityShader.setProjectionMatrix(camera.getProjectionMatrix());
		entityRenderer.render(entities);
		entityShader.stop();

		terrainShader.use();
		terrainShader.setSkyColour(m_skyColour.x, m_skyColour.y, m_skyColour.z);
		terrainShader.setLights(lights);
		terrainShader.setViewMatrix(camera.getViewMatrix());
		terrainShader.setProjectionMatrix(camera.getProjectionMatrix());
		terrainRenderer.render(terrains);
		terrainShader.stop();

		skyboxRenderer.render(camera, m_skyColour.x, m_skyColour.y, m_skyColour.z);
	}

	void prepare()
	{
		glClearColor(m_skyColour.x, m_skyColour.y, m_skyColour.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void enableCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	static void disableCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	void processEntity(const Entity& entity)
	{
		if (entities.find(entity.texturedModel) != entities.end())
		{
			entities.at(entity.texturedModel).push_back(entity);
		}
		else
		{
			entities.insert({ entity.texturedModel, std::vector<Entity>{ entity } });
		}
	}
	void processEntities(const std::vector<Entity>& entities)
	{
		for (Entity e : entities)
		{
			processEntity(e);
		}
	}

	void processTerrain(Terrain terrain)
	{
		terrains.push_back(terrain);
	}

	void processTerrains(const std::vector<Terrain>& terrains)
	{
		for (Terrain t : terrains)
		{
			processTerrain(t);
		}
	}

	void skyColour(float r, float g, float b)
	{
		m_skyColour = glm::vec3(r, g, b);
	}
};