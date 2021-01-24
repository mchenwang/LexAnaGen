#ifndef __GLEXA_H__
#define __GLEXA_H__

#include "GDFA.h"

namespace GLexAnalyzer{
    using namespace GDFA;

    bool Generate(DFA dfa, const char* save_path);
};

#endif