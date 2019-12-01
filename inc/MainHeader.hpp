#ifndef MAINHEADER_HPP
# define MAINHEADER_HPP

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <stack>
#include <map>

#ifndef EXCEPTION_HPP
# include "Exception.hpp"
#endif

#ifndef IOPERAND_HPP
# include "IOperand.hpp"
#endif

#ifndef AVM_HPP
# include "AVM.hpp"
#endif

bool					isNumber(std::string s);

static std::ofstream		outputFile;
static std::string			outputFilename = "outputFile";
static std::ofstream		logsFile;
static std::string			logsFilename = "logsFile";

// static std::map<std::string, void (AVM::*)(IOperand*)> dispatchTable  = {
// 	{"push", &AVM::cmd_push},
// 	{"pop", &AVM::cmd_pop},
// 	{"dump", &AVM::cmd_dump},
// 	{"assert", &AVM::cmd_assert},
// 	{"add", &AVM::cmd_add},
// 	{"sub", &AVM::cmd_sub},
// 	{"mul", &AVM::cmd_mul},
// 	{"div", &AVM::cmd_div},
// 	{"mod", &AVM::cmd_mod},
// 	{"print", &AVM::cmd_print},
// 	{"exit", &AVM::cmd_exit}
// };

static std::map<std::string, eOperandType> types = {
    {"int8", Int8},
    {"int16", Int16},
    {"int32", Int32},
    {"float", Float},
    {"double", Double}
};

#endif