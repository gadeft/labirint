#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    std::cout << "argc: " << argc << std::endl;
    std::cout << "argv: ";

    for(int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }

// Declare the supported options.

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("filename", po::value<std::string>(), "set the name of the file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("filename")) {
        std::cout << "The filename was set to " 
     << vm["filename"].as<std::string>() << ".\n";
    } else {
        std::cout << "The filename was not set.\n";
    }

    return 0;
}
