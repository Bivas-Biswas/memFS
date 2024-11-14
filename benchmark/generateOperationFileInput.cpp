#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

std::string generateRandomString(int length)
{
    const std::string chars = "ab cdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    for (int i = 0; i < length; ++i)
    {
        result += chars[rand() % chars.size()];
    }
    return result;
}

int randomBetween(int m, int n)
{
    return m + rand() % (n - m + 1);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_name> <output_file_name>" << std::endl;
        return 1;
    }

    srand(time(0));

    std::vector<std::string> operations;
    std::ifstream infile(argv[1]);

    if (!infile.is_open())
    {
        std::cerr << "Error: Could not open " << argv[1] << "." << std::endl;
        return 1;
    }

    std::string filename;
    while (std::getline(infile, filename))
    {
        operations.push_back("create " + filename);
        operations.push_back("delete " + filename);
        operations.push_back("write " + filename + " " + "\"" + generateRandomString(randomBetween(500, 2000)) + "\"");
    }

    std::random_shuffle(operations.begin(), operations.end());
    std::random_shuffle(operations.begin(), operations.end());
    std::random_shuffle(operations.begin(), operations.end());

    std::ofstream outFile(argv[2]);
    for (const auto &op : operations)
    {
        outFile << op << std::endl;
    }

    outFile.close();
    std::cout << "Operations saved to '" << argv[2] << "'." << std::endl;

    return 0;
}
