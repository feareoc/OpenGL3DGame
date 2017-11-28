#pragma once

#include "Loader.h"
#include "RawModel.h"
#include "WaterShader.h"
#include "WaterTile.h"
#include "WaterFrameBuffers.h"
#include "Camera.h"

class WaterRenderer
{
private:
	const float WAVE_SPEED = 0.03f;
	RawModel m_quad;
	WaterShader m_shader;
	WaterFrameBuffers& m_fbos;
	Texture m_dudvTex;
	float moveFactor;
public:
	WaterRenderer(WaterFrameBuffers& fbos) : m_quad(RawModel({ -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f }, 2)), m_fbos(fbos), m_dudvTex(Loader::loadTexture2D("waterDUDV.png"))
	{ }

	void render(const std::vector<WaterTile>& water, const Camera& camera)
	{
		prepareRender(camera);
		for (const WaterTile& tile : water)
		{
			m_shader.setModelMatrix(tile.getModelMatrix());
			glDrawArrays(GL_TRIANGLES, 0, m_quad.getVertexCount());
		}
		unbind();
	}

	void prepareRender(const Camera& camera)
	{
		m_shader.use();
		m_shader.setViewMatrix(camera);
		m_shader.setProjectionMatrix(camera);
		moveFactor += WAVE_SPEED * 0.016f;
		moveFactor = ::fmod(moveFactor, 1.0f);
		m_shader.setMoveFactor(moveFactor);
		m_quad.bind();
		glEnableVertexAttribArray(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbos.getReflectionTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_fbos.getRefractionTexture());
		glActiveTexture(GL_TEXTURE2);
		m_dudvTex.bind();
	}

	void unbind()
	{
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_shader.stop();
	}
};