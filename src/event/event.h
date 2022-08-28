#ifndef EVENT_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <map>

class EventHandler
{
	std::map<int, bool> mKeyPressed;	

	void handleWinResize(GLFWwindow *win, int newWidth, int newHeight);
	void handleKeyInput(GLFWwindow *win, int key, int scancode, int action, int mods);	
	EventHandler();
public:
	void setCallback(GLFWwindow *win);
	bool keyPressed(int key);

	static EventHandler* get();
};

#define EVENT_H
#endif
