#include "event.h"
#include <glad/glad.h>
#include "../GL/gl-func.h"

EventHandler::EventHandler()
{ }

void EventHandler::handleWinResize(GLFWwindow *win, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glUniform2f(glGetUniformLocation(getActiveProgram(), "uScreenDimensions"),
				(float)newWidth, (float)newHeight);
}

void EventHandler::handleKeyInput(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS)
		mKeyPressed[key] = true;
	else if(action == GLFW_RELEASE)
		mKeyPressed[key] = false;
}

bool EventHandler::keyPressed(int key)
{
	return mKeyPressed[key];
}

void EventHandler::setCallback(GLFWwindow *win)
{
	glfwSetWindowUserPointer(win, this);

	auto windowResize = [](GLFWwindow *window, int newWidth, int newHeight)
	{
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->
			handleWinResize(window, newWidth, newHeight);
	};
	glfwSetWindowSizeCallback(win, windowResize);
	
	auto keyInputFunc = [](GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		static_cast<EventHandler*>(glfwGetWindowUserPointer(window))->
			handleKeyInput(window, key, scancode, action, mods);
	};
	glfwSetKeyCallback(win, keyInputFunc);
}

EventHandler* EventHandler::get()
{
	static EventHandler* event = new EventHandler;
	return event;
}
