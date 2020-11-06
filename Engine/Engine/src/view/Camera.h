#ifndef CAMERA_H
#define CAMERA_H
#include "GL/glew.h"
#include "../math/Mat4.h"
#include "../controllers/CameraController.h"

/*
* Usage example:
* 
* Camera camera(view, projection);
* 
* // Maybe add a camera controller
* camera.addCameraController(cameraController);
* 
* // Maybe update view
* camera.setView(newView);
* 
* // Finally update the camera
* camera.update();
*/
class Camera {

private:
	Mat4 view;
	Mat4 projection;
	GLuint vbo;
	// TODO Add ubo and UBO_BP


public:
	Camera(const Mat4& view, const Mat4& projection, const GLuint uboBp);

	// TODO Maybe reconsider this name
	void update();
	
	// Sets the view matrix
	void setView(const Mat4& view);

	// Sets the projection matrix
	void setProjection(const Mat4& projection);

	// Adds the FreeCameraController
	void addCameraController(CameraController& cameraController);
};

#endif
