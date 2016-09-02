#include <Application.hpp>
#include <Utility.hpp>
#include <Environment.hpp>

#include <stdexcept>
#include <iostream>

int main()
{
    Stealth(); //hide console

	try
	{
        scanLibrary();

		Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
        std::cin.get();
	}
}
