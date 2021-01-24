/* Generate the NFA from regular expression
*  code by Miawua
*  2020/11/27
*/
#ifndef __GNFA_H__
#define __GNFA_H__

#include <vector>
#include <string>
#include "Data.h"
#include <map>
#include <memory>

namespace GNFA{
    using namespace GloData;
    using state = std::shared_ptr<NodeElem>;
    using REE   = Element<std::string>;
    // using path  = std::pair<std::string, state>; // REE->val to state
    using path  = std::pair<state, std::string>; // REE->val to state

    struct SubNFA{
        state start;
        state end;
        SubNFA() noexcept { ; }
        SubNFA(state _s, state _e) noexcept
        :start(_s), end(_e) { ; }
    };

    class NFA{
        void AddEdge(const path, const state);
    public:
        std::string str;                // corresponding regular expression
        std::vector<std::shared_ptr<REE>> re_nodes;
        LexType type;                   // statement type
        // std::vector<state> nodes;       // state nodes
        std::multimap<path, state> from_to; // from path.state to state when a std::string comes
        state start;
        state end;

        NFA(LexType _type, std::string _str);
        ~NFA() = default;

        // void print();
        void SetRE(std::string);
        void SetREType(LexType) noexcept;
        // void RGenNFA(std::string, int*);     // generation by recursion
        void GenNFA();
        void StrToREE();
    };
    class FinalNFA{
    public:
        std::multimap<path, state> from_to;
        std::shared_ptr<StartElem> start;
        std::vector<std::shared_ptr<EndElem>> end;
        FinalNFA() noexcept {
            start = std::make_shared<StartElem>();
        }
    };
    void NormalizeRE(std::string&);                     // Normalize the re for just having |()*
    bool CheckRE(std::string);                          // Check the re for syntax
    void UnionNFAs(FinalNFA&, std::vector<NFA>&);       // Union all subNFA to the final NFA

};

bool GetNFA(GNFA::FinalNFA&);

#endif