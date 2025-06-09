#include "DataStructures.hpp"
#include <array>
#include <iostream>

// Culori ANSI
const std::string red = "\033[1;31m";
const std::string yellow = "\033[1;33m";
const std::string green = "\033[1;32m";
const std::string reset = "\033[0m";

static constexpr std::array<const char*, 4> supported_arguments{
	"add", "delete", "edit", "list"
};

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		std::cout << red << "No arguments passed!" << reset << '\n';
		return -1;
	}

	const std::string selection = argv[1];

	std::size_t found = std::find(supported_arguments.begin(),
		supported_arguments.end(), selection) - supported_arguments.begin();

	switch (found)
	{
	case 0: // add
		if (argc != 5)
		{
			std::cout << red << "Invalid number of arguments for 'add' command!" << reset << '\n';
			return -1;
		}
		break;

	case 1: // delete
		if (argc != 3)
		{
			std::cout << red << "Invalid number of arguments for 'delete' command!" << reset << '\n';
			return -1;
		}
		break;

	case 2: // edit
		if (argc != 5)
		{
			std::cout << red << "Invalid number of arguments for 'edit' command!" << reset << '\n';
			return -1;
		}
		break;

	case 3: // list
		if (argc != 2)
		{
			std::cout << red << "Invalid number of arguments for 'list' command!" << reset << '\n';
			return -1;
		}
		break;

	default:
		std::cout << red << "Unsupported argument!" << reset << '\n';
		return -1;
	}

	try
	{
		RecipeBook book;

		switch (found)
		{
		case 0:
			book.addRecipe(make_recipe(argv[2], argv[3], argv[4]));
			std::cout << green << "Recipe added successfully!" << reset << '\n';
			break;

		case 1:
			book.deleteRecipe(argv[2]);
			std::cout << green << "Recipe deleted successfully!" << reset << '\n';
			break;

		case 2:
			book.editRecipe(argv[2], argv[3], argv[4]);
			std::cout << green << "Recipe edited successfully!" << reset << '\n';
			break;

		case 3:
			std::cout << yellow << "Listing all recipes:\n" << reset;
			book.displayBook();
			break;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << red << "Error occurred: " << ex.what() << reset << '\n';
		return -1;
	}
}


