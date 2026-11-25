#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Movie {
    std::string id;
    std::string title;
    std::string genres;
};

struct Rating {
    std::string userId;
    std::string movieId;
    std::string rating;
    std::string timestamp;
};

std::vector<std::string> parseCsvRow(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"' && quoted && i + 1 < line.size() && line[i + 1] == '"') {
            field += '"';
            ++i;
        } else if (c == '"') {
            quoted = !quoted;
        } else if (c == ',' && !quoted) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }

    fields.push_back(field);
    return fields;
}

int main(int argc, char* argv[]) {
    std::filesystem::path dataDirectory =
        argc > 1 ? argv[1] : "data/ml-latest-small";

    std::vector<Movie> movies;
    std::vector<Rating> ratings;

    std::ifstream moviesFile(dataDirectory / "movies.csv");
    if (!moviesFile) {
        std::cerr << "Could not open " << dataDirectory / "movies.csv" << '\n';
        return 1;
    }

    std::string line;
    std::getline(moviesFile, line); // Header
    while (std::getline(moviesFile, line)) {
        std::vector<std::string> fields = parseCsvRow(line);
        if (fields.size() >= 3) {
            movies.push_back({fields[0], fields[1], fields[2]});
        }
    }

    std::ifstream ratingsFile(dataDirectory / "ratings.csv");
    if (!ratingsFile) {
        std::cerr << "Could not open " << dataDirectory / "ratings.csv" << '\n';
        return 1;
    }

    std::getline(ratingsFile, line); // Header
    while (std::getline(ratingsFile, line)) {
        std::vector<std::string> fields = parseCsvRow(line);
        if (fields.size() >= 4) {
            ratings.push_back({fields[0], fields[1], fields[2], fields[3]});
        }
    }

    std::cout << "Loaded " << movies.size() << " movies\n";
    std::cout << "Loaded " << ratings.size() << " ratings\n";

    if (!movies.empty()) {
        std::cout << "\nFirst movie:\n";
        std::cout << movies.front().id << " | " << movies.front().title
                  << " | " << movies.front().genres << '\n';
    }

    return 0;
}
