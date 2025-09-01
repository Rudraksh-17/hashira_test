#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <cmath>
#include <string>
#include "json.hpp"

// For convenience
using json = nlohmann::json;

// Helper function to format the polynomial string nicely
void printPolynomial(double a, double b, double c) {
    std::cout << "  Polynomial: ";
    // Print x^2 term (since a=1, we can just print "x^2")
    std::cout << "x^2";

    // Print x term (b)
    if (b != 0) {
        if (b < 0) {
            std::cout << " - " << std::abs(b) << "x";
        } else {
            std::cout << " + " << b << "x";
        }
    }

    // Print constant term (c)
    if (c != 0) {
        if (c < 0) {
            std::cout << " - " << std::abs(c);
        } else {
            std::cout << " + " << c;
        }
    }
    std::cout << " = 0" << std::endl;
}

int main() {
    std::ifstream file_input("roots.json");
    if (!file_input.is_open()) {
        std::cerr << "Error: Could not open roots.json" << std::endl;
        return 1;
    }

    json data;
    try {
        data = json::parse(file_input);
    } catch (json::parse_error &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return 1;
    }

    if (!data.is_array()) {
        std::cerr << "Error: JSON root must be an array of objects." << std::endl;
        return 1;
    }

    int idx = 1;
    for (const auto& entry : data) {
        std::cout << "--- Processing Entry " << idx << " ---" << std::endl;
        std::string name = entry.value("name", "Unnamed");
        std::cout << "  Name: " << name << std::endl;

        std::vector<double> roots;
        try {
            roots = entry.at("roots").get<std::vector<double>>();
        } catch (json::exception &e) {
            std::cerr << "  Error: Could not extract roots. " << e.what() << std::endl;
            idx++;
            continue;
        }

        // Validate that this is a quadratic equation
        if (roots.size() != 2) {
            std::cerr << "  Error: Expected 2 roots for a quadratic equation, but found " << roots.size() << "." << std::endl;
            idx++;
            continue;
        }

        // --- Logic for Quadratic Equation ---
        const double r1 = roots[0];
        const double r2 = roots[1];

        // We assume a monic polynomial, so a = 1
        const double a = 1.0;
        
        // Sum of roots = -b/a  => b = -a * (r1 + r2)
        const double b = -a * (r1 + r2);

        // Product of roots = c/a => c = a * (r1 * r2)
        const double c = a * (r1 * r2);

        std::cout << "  Roots: [" << r1 << ", " << r2 << "]" << std::endl;
        std::cout << "  Coefficients (a=1): a=" << a << ", b=" << b << ", c=" << c << std::endl;
        
        printPolynomial(a, b, c);
        
        idx++;
    }

    return 0;
}