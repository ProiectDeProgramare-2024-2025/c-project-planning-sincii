#ifndef _DATASTRUCTURED_HPP_
#define _DATASTRUCTURED_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <fstream>

struct Recipe
{
	std::string name;
	std::string description;
	std::vector<std::string> ingredients;
};

Recipe make_recipe(const std::string name, const std::string& description, const std::string& ingredients)
{
	Recipe result;
	result.name = name;
	result.description = description;

	std::stringstream ss(ingredients);
	std::string ingredient;

	while (std::getline(ss, ingredient, ','))
	{
		result.ingredients.push_back(ingredient);
	}

	return result;
}

class FileManager
{
public:
	static std::vector<Recipe> loadFromFile(const std::string& filename)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			return {};
		}

		std::vector<Recipe> result;
		std::string name;
		std::string description;
		std::string ingredients;

		while (std::getline(file, name))
		{
			bool valid_input = std::getline(file, description) && std::getline(file, ingredients);
			if (!valid_input)
			{
				throw std::runtime_error("Recipe list file is corrupt!");
			}

			result.push_back(make_recipe(name, description, ingredients));
		}

		return result;
	}

	static void saveToFile(const std::string& filename, const std::vector<Recipe>& recipes)
	{
		std::ofstream file(filename);

		for (const Recipe& recipe : recipes)
		{
			file << recipe.name << '\n';
			file << recipe.description << '\n';

			if (!recipe.ingredients.empty())
			{
				auto it = recipe.ingredients.begin();

				file << *it;
				++it;
				while (it != recipe.ingredients.end())
				{
					file << ',' << *it;
					++it;
				}
				file << '\n';
			}
		}
	}
};

class RecipeBook
{
public:
	constexpr static const char* FILE_NAME = "recipes.txt";

	RecipeBook()
	{
		recipes = FileManager::loadFromFile(FILE_NAME);
	}

	void addRecipe(const Recipe& recipe)
	{
		recipes.push_back(recipe);
		FileManager::saveToFile(FILE_NAME, recipes);
	}

	void deleteRecipe(const std::string& name)
	{
		auto found = searchRecipe(name);
		recipes.erase(found);
		FileManager::saveToFile(FILE_NAME, recipes);
	}

	void editRecipe(const std::string& name, const std::string& new_description,
		const std::string& new_ingredients)
	{
		Recipe& recipe = *searchRecipe(name);
		recipe = make_recipe(recipe.name, new_description, new_ingredients);
		FileManager::saveToFile(FILE_NAME, recipes);
	}

	void displayBook() const
	{
		displayRecipes(recipes);
	}

	void displayRecipes(const std::vector<Recipe>& list) const
	{
		const std::string reset = "\033[0m";
		const std::string yellow = "\033[1;33m";
		const std::string cyan = "\033[1;36m";
		const std::string green = "\033[1;32m";
		const std::string gray = "\033[0;37m";

		for (const Recipe& recipe : list)
		{
			std::cout << gray << " =- " << yellow << recipe.name << gray << " -= \n\n" << reset;
			std::cout << cyan << "Description: '" << recipe.description << "'\n\n" << reset;

			for (const std::string& ingredient : recipe.ingredients)
			{
				std::cout << green << " > " << ingredient << "\n" << reset;
			}
			std::cout << '\n';
		}
	}

	std::vector<Recipe>::iterator searchRecipe(const std::string& name)
	{
		auto found = std::find_if(recipes.begin(), recipes.end(),
			[&](const Recipe& recipe)
			{
				return recipe.name == name;
			});

		if (found == recipes.end())
		{
			throw std::runtime_error("Recipe to be deleted not found!");
		}

		return found;
	}

	std::vector<Recipe> deepSearchRecipes(const std::string& key) const
	{
		std::vector<Recipe> result;

		std::copy_if(recipes.begin(), recipes.end(), std::back_inserter(result),
			[&](const Recipe& recipe)
			{
				if (recipe.name.find(key) != std::string::npos)
				{
					return true;
				}

				for (const std::string& ingredient : recipe.ingredients)
				{
					if (ingredient.find(key) != std::string::npos)
					{
						return true;
					}
				}

				return false;
			});

		return result;
	}

	void update_ingredient(const std::string& name,
		unsigned int index, const std::string& new_ingredient)
	{
		Recipe& recipe = *searchRecipe(name);
		recipe.ingredients.at(index) = new_ingredient;
		FileManager::saveToFile(FILE_NAME, recipes);
	}

private:
	std::vector<Recipe> recipes;
};

#endif // !_DATASTRUCTURED_HPP_

