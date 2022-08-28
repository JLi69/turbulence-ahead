#include "app.h"

int main()
{
	App* app = App::get();
	app->init();	
	app->run();
}
