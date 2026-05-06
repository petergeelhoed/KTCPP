#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <optional>

// Struct to hold one music track entry
struct Track
{
    std::string artist;
    std::string album;
    std::string track;
};

// C++20 helper: getline that returns std::optional<std::string>
std::optional<std::string> getline_opt(std::istream& is)
{
    std::string line;
    if (std::getline(is, line))
    {
        return line;
    }
    else
    {
        return std::nullopt;
    }
}

int main()
{
    const std::string filename = "tracks.csv";
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << '\n';
        return 1;
    }

    std::vector<Track> tracks;
    while (auto line_opt = getline_opt(file))
    {
        std::istringstream ss(*line_opt);
        auto artist_opt = getline_opt(ss);
        auto album_opt = getline_opt(ss);
        auto track_opt = getline_opt(ss);

        if (artist_opt && album_opt && track_opt)
        {
            tracks.push_back(Track{*artist_opt, *album_opt, *track_opt});
        }
        else
        {
            std::cerr << "Skipping malformed line: " << *line_opt << '\n';
        }
    }

    // Example: print parsed data
    for (const auto& t : tracks)
    {
        std::cout << "Artist: " << t.artist << ", Album: " << t.album
                  << ", Track: " << t.track << '\n';
    }

    return 0;
}
