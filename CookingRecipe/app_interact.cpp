#include "DataStructures.hpp"
#include <array>

// Coduri ANSI pentru culori
const std::string red = "\033[1;31m";
const std::string yellow = "\033[1;33m";
const std::string green = "\033[1;32m";
const std::string reset = "\033[0m";

static constexpr std::array<const char*, 3> supported_arguments{
	"search", "view", "update_ingredient"
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
	case 0: // search
		if (argc != 3)
		{
			std::cout << red << "Invalid number of arguments for search command!" << reset << '\n';
			return -1;
		}
		break;

	case 1: // view
		if (argc != 3)
		{
			std::cout << red << "Invalid number of arguments for view command!" << reset << '\n';
			return -1;
		}
		break;

	case 2: // update_ingredient
		if (argc != 5)
		{
			std::cout << red << "Invalid number of arguments for update_ingredient command!" << reset << '\n';
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
		std::vector<Recipe> matches;

		switch (found)
		{
		case 0:
			matches = book.deepSearchRecipes(argv[2]);
			std::cout << yellow << "Found " << matches.size() << " match(es):" << reset << '\n';
			book.displayRecipes(matches);
			break;

		case 1:
			book.displayRecipes({ *book.searchRecipe(argv[2]) });
			break;

		case 2:
			unsigned int index = std::stoul(std::string(argv[3]));
			book.update_ingredient(argv[2], index, argv[4]);
			std::cout << green << "Ingredient updated successfully." << reset << '\n';
			break;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout << red << "Error occurred: " << ex.what() << reset << '\n';
		return -1;
	}
}
