#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Struct to hold one music track entry
struct Track
{
    std::string artist;
    std::string album;
    std::string track;
};

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
    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string artist, album, track;

        // Extract CSV fields (simple comma-separated parsing)
        if (std::getline(ss, artist, ',') && std::getline(ss, album, ',') &&
            std::getline(ss, track))
        {

            tracks.push_back(Track{artist, album, track});
        }
        else
        {
            std::cerr << "Skipping malformed line: " << line << '\n';
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
