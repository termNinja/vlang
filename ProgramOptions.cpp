/*
 * ProgramOptions.cpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "ProgramOptions.hpp"

namespace opt = boost::program_options;

namespace vlang {
namespace util {

bool ProgramOptions::is_init = false;

ProgramOptions& ProgramOptions::get() {
    static ProgramOptions instance;
    return instance;
}

const std::vector<std::string>& ProgramOptions::input() const {
    return m_vm["input-file"].as< std::vector<std::string> >();
}

std::string ProgramOptions::first_input_file() const {
    return m_vm["input-file"].as< std::vector<std::string> >()[0];
}

bool ProgramOptions::contains_input_files() const {
    if (m_vm.count("input-file"))
        return true;
    else return false;
}

void ProgramOptions::set_input(opt::variables_map vm) {
    m_vm = vm;
}

const boost::program_options::variables_map& ProgramOptions::var_map() const {
    return m_vm;
}

std::string ProgramOptions::output_path() const {
    return m_vm["output"].as<std::string>();
}

void ProgramOptions::init(int argc, char** argv) {
    if (ProgramOptions::get().is_init) {
        std::cerr << "Warning! Detected multiple init of ProgramOptions!" << std::endl;
        return;
    }

    opt::options_description desc("All options");
    desc.add_options()
        ("help", "produce help message")
        //("optimization", opt::value<std::string>()->default_value(""), "optimization level")
        ("input-file,i", opt::value<std::vector<std::string> >(), "input .vala file")
        ("output,o", opt::value<std::string>()->default_value("a.out"), " executable output path and name")
    ;

    // Let's make any given unspecified argument as input file
    opt::positional_options_description po_desc;
    po_desc.add("input-file", -1);

    opt::variables_map vm;

    // TODO: Handle the cases when we are passed a dumb argument.
    // It seems LLVM has turned off exceptions which I need in order to acomplish this.
    opt::store(opt::command_line_parser(argc, argv).options(desc).positional(po_desc).run(), vm);

    opt::notify(vm);
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }

    ProgramOptions::get().is_init = true;
    ProgramOptions::get().set_input(vm);
}

} // ;util
} // ;vlang
