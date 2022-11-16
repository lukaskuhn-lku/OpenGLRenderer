#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENTS
{
	CAMERA_UP,
	CAMERA_DOWN,
	CAMERA_LEFT,
	CAMERA_RIGHT
};


class Camera {
public: 
	//CAMERA
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	glm::mat4 view = glm::mat4(1.0f);

	Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp) {
		this->cameraPos = cameraPos;
		this->cameraFront = cameraFront;
		this->cameraUp = cameraUp;
		
		this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void moveView(float yaw, float pitch) {
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);

		this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void movePos(CAMERA_MOVEMENTS movement, float cameraSpeed) {
		switch (movement)
		{
		case CAMERA_UP:
			this->cameraPos += cameraSpeed * cameraFront;
			break;
		case CAMERA_DOWN:
			this->cameraPos -= cameraSpeed * cameraFront;
			break;
		case CAMERA_LEFT:
			this->cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;
		case CAMERA_RIGHT:
			this->cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;
		default:
			break;
		}

		this->view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}



};
	