#pragma once

#include "Player.h"

class Camera
{
public:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_fieldOfView;
	float m_nearPlane;
	float m_farPlane;

	Player& player;
	float distanceFromPlayer = 50;
	float angleAroundPlayer = 0;
public:
	void updateVectors()
	{
		m_front.x = glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
		m_front.y = glm::sin(glm::radians(m_yaw));
		m_front.z = glm::sin(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw));
		m_front = glm::normalize(m_front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));
	}
public:
	Camera(Player& player, const glm::vec3& position, float yaw = 0, float pitch = 0, float roll = 0, float fov = 45)
		: m_position(position),
		m_worldUp(glm::vec3(0, 1, 0)),
		m_yaw(yaw),
		m_pitch(pitch),
		m_roll(roll),
		m_fieldOfView(fov),
		m_nearPlane(0.1f),
		m_farPlane(4000.0f),
		player(player)
	{
		updateVectors();
	}

	glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(m_position, player.m_position, m_up);
	}

	glm::mat4 getProjectionMatrix(float aspectRatio) const
	{
		return glm::perspective(m_fieldOfView, aspectRatio, m_nearPlane, m_farPlane);
	}

	void rotate(float yaw, float pitch = 0.0f, float roll = 0.0f)
	{
		m_yaw += yaw;
		m_pitch += pitch;
		m_roll += roll;

		if (m_yaw > 89.0f)
		{
			m_yaw = 89.0f;
		}
		else if (m_yaw < 1.0f)
		{
			m_yaw = 1.0f;
		}
	}

	void move(float dx, float dy = 0, float dz = 0)
	{
		m_position.x += dx;
		m_position.y += dy;
		m_position.z += dz;
	}

	void moveFront(float xspeed, float yspeed, float zspeed)
	{
		m_position.x += m_front.x * xspeed;
		m_position.y += m_front.y * yspeed;
		m_position.z += m_front.z * zspeed;
	}

	void moveRight(float xspeed, float yspeed, float zspeed)
	{
		m_position.x += m_right.x * xspeed;
		m_position.y += m_right.y * yspeed;
		m_position.z += m_right.z * zspeed;
	}

	void moveUp(float xspeed, float yspeed, float zspeed)
	{
		m_position.x += m_up.x * xspeed;
		m_position.y += m_up.y * yspeed;
		m_position.z += m_up.z * zspeed;
	}

	void zoom(float value)
	{
		distanceFromPlayer += value;
	}

	void calcCamPos()
	{
		float theta = m_pitch - player.m_rotation.y;
		//
		m_front.x = glm::cos(glm::radians(theta)) * glm::cos(glm::radians(m_yaw));
		m_front.y = glm::sin(glm::radians(m_yaw));
		m_front.z = glm::sin(glm::radians(theta)) * glm::cos(glm::radians(m_yaw));
		m_front = glm::normalize(m_front);
		m_right = glm::normalize(glm::cross(m_front, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_front));
		m_position = player.m_position + m_front * distanceFromPlayer;
		//std::cout << "yaw: " << m_yaw << " x: " << m_position.x << " y: " << m_position.y << " z: " << m_position.z << std::endl;
	}
};