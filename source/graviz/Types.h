/**
 * Project   Graviz
 *
 * @file     Types.h
 * @author   Dmytro Sadovyi
 * @date     03.11.2016
 */

#pragma once

namespace NGraviz
{

enum class TSystemMode
{
    Default,
    InProcess,
    WaitForAnswer,
    Total        // wrong mode, just amount of modes
                 // add new type before it
};

enum class TProblemSolverType
{
    RealTime, // update answer each time, when we change input
    Straight, // update answer by query
    Interactive, // dialog with user
};

enum class TGravizCommand
{
    System,             // should be redirect to system
    RunSolver,          // start application
    RunSolverVisual,     // visual solver
    ParseSite,          // parse site to get data
    Test,               // test commands
    Tester,             // launch tester
    TerminateProcess,   // stop application
    Compile,            // compile source code
    ChangeDirectory,    // change working directory
    Python,
    Exit,               // Exit from system
    Unknown,            // unknown command name
    Empty,              // no command

    Total,              // amount of commands
                        // new commands must be specified before
};


} // namespace NGraviz
