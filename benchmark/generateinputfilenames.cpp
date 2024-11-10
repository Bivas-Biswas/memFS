#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <string>

std::string generateRandomString(int length) {
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += chars[rand() % chars.size()];
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <number_of_files> <output_file_name>" << std::endl;
        return 1;
    }

    int numFiles = std::stoi(argv[1]);
    std::string outputFile = argv[2];

    srand(time(0));

    std::set<std::string> filenames;
    
    while (filenames.size() < numFiles) {
        filenames.insert("file_" + generateRandomString(15) + ".txt");
    }

    std::ofstream outFile(outputFile);
    for (const auto& filename : filenames) {
        outFile << filename << std::endl;
    }

    outFile.close();
    std::cout << numFiles << " unique filenames saved to '" << outputFile << "'." << std::endl;

    return 0;
}
