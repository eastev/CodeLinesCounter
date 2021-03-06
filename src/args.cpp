/*
 * SPDX-License-Identifier: GPL-3.0
 * Copyright © eastev <eastevrud31@gmail.com>
 */ 

/*
 * Simple-loc arguments parser
 */

#include <string>
#include <iostream>
#include <filesystem>

#include <args.hpp>
#include <output.hpp>
#include <parser.hpp>

#define CONFIG "etc/simple-loc/langs.conf"

void option_f(loc::parser& parser, std::vector<std::string>& argv)
{
    std::vector<std::string>::iterator iterator;
    iterator = std::find(argv.begin(), argv.end(), "-d");

    if(iterator != argv.end())
        parser.files_only(*(std::next(iterator)));
    else
    {
        iterator = std::find(argv.begin(), argv.end(), "--directory");

        if(iterator != argv.end())
            parser.files_only(*(std::next(iterator)));
        else
            parser.files_only();
    }
}

void option_l(loc::parser& parser, std::vector<std::string>& argv, int& i)
{
    std::vector<std::string>::iterator iterator;
    iterator = std::find(argv.begin(), argv.end(), "-d");

    if(iterator != argv.end())
        parser.language(argv[++i], *(std::next(iterator)));
    else
    {
        iterator = std::find(argv.begin(), argv.end(), "--directory");

        if(iterator != argv.end())
            parser.language(argv[++i], *(std::next(iterator)));
        else
            parser.language(argv[++i]);
    }
}

/* Parse arguments and call specified functions */
int loc::args::parse(int argc, std::vector<std::string>& argv)
{
    output::name();

    std::string conf_path = CONFIG; 

    #ifndef __local_dev__
        conf_path = "/" + conf_path;
    #endif

    std::string arg;
    loc::parser parser(conf_path);

    if(argc == 1)
    {
        std::string cdir = ".";

        parser.dir(cdir);
    }
    else
    {
        for(int i = 1; i < argc; i++)
        {
            arg = std::string(argv[i]);

            /* Show help */
            if(arg == "-h" || arg == "--help")
            {
                output::help();

                return 0;
            }
            else if(arg == "-f" || arg == "--files-only")
            {
                option_f(parser, argv);

                break;
            }
            else if(arg == "-d" || arg == "--directory")
            {
                parser.dir(argv[++i]);

                break;
            }
            else if(arg == "-l" || arg == "--language")
            {
                option_l(parser, argv, i);

                break;
            }
            else
            {
                printf("\033[0;31mError\033[0m: No option %s\n", arg.c_str());
                output::tail();

                return 1;
            }
        }
    }

    output::head();

    for(size_t i = 0; i < parser.langs.size(); i++)
    {
        output::lang(parser.langs[i].get_name(), parser.langs[i].files, 
                        parser.langs[i].code);
    }

    output::tail();

    return 0;
}
