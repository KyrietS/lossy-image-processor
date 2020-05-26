#include <CLI/CLI11.hpp>
#include <iostream>
#include <string>
#include "LIPCoder.hpp"

void printStats(Statistics stats);

int main(int argc, char** argv)
{
    CLI::App app;

    // All flags and options.
    bool encode{ false };
    bool decode{ false };
    std::string path_in;
    std::string path_out;
    bool print_stats{ false };

    // encode | decode option group.
    auto action = app.add_option_group("Action type", "What action on a file should be performed.");
    action->add_flag("-e,--encode", encode, "Encode a file");
    action->add_flag("-d,--decode", decode, "Decode a file");
    action->require_option(1);
    action->required(true);

    // input_file option.
    app.add_option("source", path_in, "Path to a file you want to encode or decode.")
        ->required(true)
        ->check(CLI::ExistingFile);

    // output_file option.
    app.add_option("dest", path_out, "Path to a result file.")
        ->required(true);

    // print stats after encoding process
    app.add_flag("-s,--stats", print_stats, "Print stats during and after encoding process.");

    app.get_formatter()->label("Positionals", "Files");
    app.get_formatter()->label("TEXT", "PATH");
    app.get_formatter()->label("TEXT:FILE", "PATH");


    // Parse input parameters.
    CLI11_PARSE(app, argc, argv);

    // File 'path_out' should not exist at this point.
    //if (std::filesystem::exists(path_out))
    //{
    //    std::cerr << "dest: Path already exists: " << path_out << std::endl;
    //    std::cerr << "Run with --help for more information." << std::endl;
    //    return -1;
    //}

    // The meat.
    LIPCoder coder(print_stats);
    Statistics stats;

    if (encode) {
        stats = coder.encode(path_in, path_out);
    }
    else if (decode) {
        stats = coder.decode(path_in, path_out);
    }

    if (print_stats)
        printStats(stats);

    return 0;
}

void printStats(Statistics stats)
{
    // TODO: Print stats
}