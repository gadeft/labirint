#include <iostream>
#include <vector>
#include <set>
#include <fstream>

using field = std::vector<std::vector<char>>;
using point = std::pair<int, int>;
using set_of_points = std::set<point>;

std::istream& operator>>(std::istream& in, field& labirint);
std::ostream& operator<<(std::ostream& out, field& labirint);
bool is_path_exist(const field& labirint, const point& curr, set_of_points& visited);
std::vector<point> neighbors(const field& labirint, const point& p);
char get_value(const field& labirint, const point& p);
point find_point(const field& labirint, char symbol);

const char begin = 'A';
const char end = 'B';
const char wall = 'X';
const char corridor = ' ';

int main()
{
    try
    {
        field labirint;

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("filename", po::value<std::string>(), "set the name of the file")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

        if (vm.count("help")) 
        {
            std::cout << desc << "\n";
            throw std::runtime_error("Help requested");
        }

        if (vm.count("filename")) 
        {
            std::cout << "The filename was set to " 
         << vm["filename"].as<std::string>() << ".\n";

            std::fstream in_file(vm["filename"].as<std::string>());
            if (!in_file.open())
                throw std::runtime_error("Unable to open the file");
            in_file >> labirint;
        } 
        else 
        {
            std::cout << "The filename was not set.\n";

            std::cin >> labirint;
        }

        set_of_points visited;

        point start = find_point(labirint, begin);
        std::cout << labirint << std::endl;

        std::cout << (
                is_path_exist(labirint, start, visited) ? 
                "Path exists" :
                "Path does not exist"
                ) << std::endl;

        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
}

point find_point(const field& labirint, char symbol)
{
    for(int row = 0; row < labirint.size(); row++)
        for (int col = 0; col < labirint[row].size(); col++)
            if (labirint[row][col] == symbol)
                return {row, col};

    throw std::runtime_error("Character " + std::string(1, symbol) + " does not exist");
}

char get_value(const field& labirint, const point& p)
{
    return labirint[p.first][p.second];
}

std::vector<point> neighbors(const field& labirint, const point& p)
{
    std::vector<point> result;

    if (p.first > 0)
        result.push_back({p.first - 1, p.second});
    if (p.second < labirint[0].size() - 1)
        result.push_back({p.first, p.second + 1});
    if (p.first < labirint.size() -  1)
        result.push_back({p.first + 1, p.second});
    if (p.second > 0)
        result.push_back({p.first, p.second - 1});

    return std::move(result);
}

bool is_path_exist(const field& labirint, const point& curr, set_of_points& visited)
{
    if (get_value(labirint, curr) == end)
        return true;
    if (get_value(labirint, curr) == wall)
        return false;
    if (visited.count(curr))
        return false;
    visited.insert(curr);

    for (const auto& neighbor: neighbors(labirint, curr))
    {
        if (is_path_exist(labirint, neighbor, visited))
            return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& out, field& labirint)
{
    for (auto it = labirint.begin(); it != labirint.end(); it++)
    {
        for (auto sub_it = it->begin(); sub_it != it->end(); sub_it++)
        {
            out << *sub_it;
        }

        out << std::endl;
    }
    out << "Size: " << labirint.size() << std::endl;

    return out;
}

std::istream& operator>>(std::istream& in, field& labirint)
{
    char symbol;
    unsigned current_line = 0;
    unsigned width;
    std::vector<char> a;
    bool exists_A = false;
    bool exists_B = false;
    while(in.get(symbol))
    {
        if (symbol == '\n')
        {
            if (a.size() == 0)
                break;

            if (current_line == 0)
                width = a.size();

            if (a.size() != width)
                throw std::runtime_error("Provided field has wrong dimenssions");
            
            labirint.push_back(a);
            a.clear();

            current_line++;
        }
        else if (symbol == ' ' || symbol == 'X' || symbol == 'A' || symbol == 'B')
        {
            if (symbol == 'A')
                if (!exists_A)
                    exists_A = true;
                else
                    throw std::runtime_error("Duplicate A at line " + std::to_string(current_line + 1));

            if (symbol == 'B')
                if (!exists_B)
                    exists_B = true;
                else
                    throw std::runtime_error("Duplicate B at line " + std::to_string(current_line + 1));
            
            a.push_back(symbol);
        }
        else
            throw std::runtime_error("Invalid character at line " + std::to_string(current_line + 1));
    }

    if (!exists_A || !exists_B)
        throw std::runtime_error("Labirint must contain both A and B");

    return in;
}
