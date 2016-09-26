/*
 * ProgramOptions.hpp
 * Copyright (C) 2016 Nemanja Mićović <nmicovic@outlook.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PROGRAMOPTIONS_HPP
#define PROGRAMOPTIONS_HPP

#include <boost/program_options.hpp>
#include <boost/program_options/errors.hpp>
#include <vector>
#include <string>
#include <iostream>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
namespace vlang {
namespace util {
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/// -------------------------------------------------------------------------------------
/// \brief Singleton class that contains compiler options.
/// -------------------------------------------------------------------------------------
class ProgramOptions {
public:
    /// \brief Initializes ProgramOptions from given cmd args.
    static void init(int argc, char** argv);

    /// \brief Gives the reference to the Program Options object.
    static ProgramOptions& get();

    /// \brief Gives a vector of strings that are paths to input files.
    const std::vector<std::string>& input() const;

    /// \brief Gives the parameter map.
    const boost::program_options::variables_map& var_map() const;

    /// \brief Returns true if input files were provided.
    /// Make sure to call this function in order to check
    /// were there are arguments supplied.
    ///
    /// TODO: Make this less risky maybe?
    bool contains_input_files() const;

    /// \brief Emmits the parsed source code to stdout.
    bool emit_source() const;

    /// \brief Returns the first input file given in program options.
    std::string first_input_file() const;

    /// \brief Returns true if vlang dumps colored code.
    bool syntax_highlight() const;

    /// \brief Returns the executable path and name
    std::string output_path() const;

    /// \brief Returns a string which shows the state of program options.
    std::string show_state() const;

    /// \brief Returns true if compiler should emit llvm code.
    bool emit_llvm() const;

    /// \brief Done for testing, to be removed.
    void write_llvm_to_bitcode() const;

private:
    ProgramOptions() {}
    ProgramOptions(const ProgramOptions&) = delete;
    ProgramOptions(const ProgramOptions&&) = delete;
    ProgramOptions& operator=(const ProgramOptions&) = delete;
    ProgramOptions& operator=(const ProgramOptions&&) = delete;

    void set_input(boost::program_options::variables_map vm);

    boost::program_options::variables_map m_vm;

    static bool is_init;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
} // ;util 
} // ;vlang
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#endif /* !PROGRAMOPTIONS_HPP */
