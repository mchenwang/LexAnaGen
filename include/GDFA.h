/* Generate the DFA from NFA
*  code by Miawua
*/
#ifndef __GDFA_H__
#define __GDFA_H__

#include "Data.h"
#include <map>
#include <set>
#include <vector>
#include <GNFA.h>
#include <GDFA.h>

namespace GDFA{
    using namespace GloData;
    using REE   = Element<std::string>;
    using state = std::shared_ptr<NodeElem>;
    using path  = std::pair<state, std::string>;
    using NFA   = GNFA::FinalNFA;

    class DFA{
    public:
        std::map<path, state> from_to;
        state start;
        std::set<state> nodes;

        DFA()  = default;
        ~DFA() = default;
        void NFAToDFA(NFA&);
        std::pair<bool, LexType> FindISubset(std::set<state>&,std::set<state>&, const std::shared_ptr<REE>&, const NFA&);
        std::pair<bool, LexType> FindISubsetEps(std::set<state>&, const NFA&);

        void Minimize();
    };
};

#endif