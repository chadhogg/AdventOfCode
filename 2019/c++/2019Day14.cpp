/// \file 2019Day14.cpp
/// \author Chad Hogg
/// \brief My solution to https://adventofcode.com/2019/day/14.

#include <iostream>
#include <string>
#include <unordered_map>

#include "utilities.hpp"

struct Ingredient {
    unsigned int quantity;
    std::string type;
};

struct Recipe {
    std::vector<Ingredient> inputs;
    Ingredient output;
};

using Cookbook = std::unordered_map<std::string, Recipe>;

std::ostream & operator<< (std::ostream & out, Ingredient const& x) {
    out << x.quantity << " " << x.type;
    return out;
}

std::ostream & operator<< (std::ostream & out, Recipe const& r) {
    for (unsigned int index {0U}; index < r.inputs.size (); ++index) {
        out << r.inputs[index];
        if (index < r.inputs.size () - 1) {
            out << ", ";
        }
    }
    out << " => ";
    out << r.output;
    return out;
}

std::ostream & operator<< (std::ostream & out, Cookbook const& b) {
    for (std::pair<std::string const, Recipe> const& r : b) {
        out << r.second << "\n";
    }
    return out;
}

Ingredient parseIngredient (std::string const& str) {
    int quantity = std::atoi (str.substr (0, str.find (' ')).c_str ());
    std::string type = str.substr (str.find (' ') + 1);
    return {(unsigned int)quantity, type};
}

std::vector<Ingredient> parseInputs (std::string const& str) {
    std::vector<Ingredient> vec;
    std::size_t before = 0;
    std::size_t comma = str.find (',');
    while (comma != str.npos) {
        vec.push_back (parseIngredient (str.substr (before, comma - before)));
        before = comma + 2;
        comma = str.find (',', before);
    }
    vec.push_back (parseIngredient (str.substr (before)));
    return vec;
}

Cookbook getInput () {
    Cookbook cookbook;
    std::string line;
    while (std::getline (std::cin, line)) {
        std::string before = line.substr (0, line.find ('=') - 1);
        std::string after = line.substr (line.find ('>') + 2);
        Recipe recipe {parseInputs (before), parseIngredient (after)};
        cookbook.insert ({recipe.output.type, recipe});
    }
    return cookbook;
}

std::ostream & operator<< (std::ostream & out, std::unordered_map<std::string, unsigned long> const& map) {
    for (std::pair<std::string const, unsigned long> p : map) {
        out << p.second << " " << p.first << " ";
    }
    return out;
}

unsigned long oreToMakeFuel (Cookbook const& book) {
    std::unordered_map<std::string, unsigned long> needs;
    std::unordered_map<std::string, unsigned long> extras;
    unsigned long oreAmount {0UL};
    needs.insert ({"FUEL", 1UL});
    while (!needs.empty ()) {
        //std::cout << "Needs: " << needs << "\n";
        //std::cout << "Extras: " << extras << "\n";
        std::string neededType = needs.begin ()->first;
        unsigned long neededQuantity = needs.begin ()->second;
        needs.erase (needs.begin ());
        Recipe const& recipe = book.at (neededType);
        unsigned long times = neededQuantity / recipe.output.quantity + (neededQuantity % recipe.output.quantity == 0 ? 0 : 1);
        unsigned long surplus = times * recipe.output.quantity - neededQuantity;
        if (surplus > 0) {
            if (extras.count (neededType) == 0) {
                extras.insert ({neededType, surplus});
            }
            else {
                extras[neededType] += surplus;
            }
        }
        for (Ingredient const& component : recipe.inputs) {
            unsigned long needed = times * component.quantity;
            if (component.type == "ORE") {
                oreAmount += needed;
            }
            else {
                if (extras.count (component.type) == 1) {
                    if (extras.at (component.type) > needed) {
                        extras[component.type] = extras[component.type] - needed;
                        needed = 0;
                    }
                    else {
                        needed -= extras[component.type];
                        extras.erase (component.type);
                    }
                }
                if (needed > 0) {
                    if (needs.count (component.type) == 0) {
                        needs.insert ({component.type, needed});
                    }
                    else {
                        needs[component.type] += needed;
                    }
                }
            }
        }
    }
    return oreAmount;
}

unsigned long howMuchFuelFromTrillionOre (Cookbook const& book) {
    std::unordered_map<std::string, unsigned long> needs;
    std::unordered_map<std::string, unsigned long> extras;
    unsigned long oreAmount {0UL};
    unsigned long fuelMade {0UL};
    while (oreAmount <= 1'000'000'000'000) {
        //std::cout << "Made " << fuelMade << " fuel, used " << oreAmount << " ore.\n";
        unsigned long jump {1UL};
        if (oreAmount < 900'000'000'000) {
            jump = 1000UL;
            //std::cout << "Leaping by 1000\n";
        }
        else if (oreAmount < 999'900'000'000) {
            jump = 100UL;
            //std::cout << "Jumping by 100\n";
        }
        needs.insert ({"FUEL", jump});
        fuelMade += jump;
        while (!needs.empty ()) {
            //std::cout << "Needs: " << needs << "\n";
            //std::cout << "Extras: " << extras << "\n";
            std::string neededType = needs.begin ()->first;
            unsigned long neededQuantity = needs.begin ()->second;
            needs.erase (needs.begin ());
            Recipe const& recipe = book.at (neededType);
            unsigned long times = neededQuantity / recipe.output.quantity + (neededQuantity % recipe.output.quantity == 0 ? 0 : 1);
            unsigned long surplus = times * recipe.output.quantity - neededQuantity;
            if (surplus > 0) {
                if (extras.count (neededType) == 0) {
                    extras.insert ({neededType, surplus});
                }
                else {
                    extras[neededType] += surplus;
                }
            }
            for (Ingredient const& component : recipe.inputs) {
                unsigned long needed = times * component.quantity;
                if (component.type == "ORE") {
                    oreAmount += needed;
                }
                else {
                    if (extras.count (component.type) == 1) {
                        if (extras.at (component.type) > needed) {
                            extras[component.type] = extras[component.type] - needed;
                            needed = 0;
                        }
                        else {
                            needed -= extras[component.type];
                            extras.erase (component.type);
                        }
                    }
                    if (needed > 0) {
                        if (needs.count (component.type) == 0) {
                            needs.insert ({component.type, needed});
                        }
                        else {
                            needs[component.type] += needed;
                        }
                    }
                }
            }
        }
    }
    return fuelMade - 1;
}

int main () {
    Cookbook book = getInput ();
    //std::cout << book << "\n";
    std::cout << oreToMakeFuel (book) << "\n";
    std::cout << howMuchFuelFromTrillionOre (book) << "\n";
    return 0;
}
