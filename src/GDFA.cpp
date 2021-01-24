#include "GDFA.h"
#include "GNFA.h"
#include <set>
#include <queue>
#include <map>
#include "config.h"

#ifdef TEST
#include <iostream>
#endif

using namespace GDFA;

std::pair<bool, LexType> DFA::FindISubsetEps(std::set<state>& I, const NFA& nfa)
{
    bool is_end = false;
    LexType end_type = "";
    std::queue<state> q;
    for(auto& s: I) q.emplace(s);
    while(!q.empty()){
        state u = q.front(); q.pop();
        auto all = nfa.from_to.equal_range(path{u, EpsElem::e()});
        for(auto it = all.first; it!=all.second; it++){
            if(I.find(it->second) == I.end()){
                I.insert(it->second), q.emplace(it->second);
                if(dynamic_cast<EndElem*>(it->second.get())){
                    is_end = true;
                    end_type = it->second->node_type;
                }
            }
        }
    }
    return {is_end, end_type};
}

std::pair<bool, LexType> DFA::FindISubset(std::set<state>& Ip, std::set<state>& I, const std::shared_ptr<REE>& c, const NFA& nfa)
{
    bool is_end = false;
    LexType end_type = "";
    std::queue<state> q;
    for(auto& s: Ip) q.emplace(s);
    while(!q.empty()){
        state u = q.front(); q.pop();
        auto all = nfa.from_to.equal_range(path{u, c->val()});
        for(auto it = all.first; it!=all.second; it++){
            if(I.find(it->second) == I.end()){
                I.insert(it->second);//, q.emplace(it->second);
                if(dynamic_cast<EndElem*>(it->second.get())) {
                    is_end = true;
                    end_type = it->second->node_type;
                }
            }
        }
    }
    auto [t1, t2] = FindISubsetEps(I, nfa);
    if(is_end) return {is_end, end_type};
    else return {t1, t2};
}

void DFA::NFAToDFA(NFA& nfa)
{
    state_count["DFA"] = 0;
    int *counter = &state_count["DFA"];
    std::set<state> I0;
    I0.insert(nfa.start);
    auto [is_end, end_type] = FindISubsetEps(I0, nfa);
    if(is_end) start = std::make_shared<EndElem>(end_type, state_count[end_type]++);
    else start = std::make_shared<StartElem>("DFA");
    this->nodes.emplace(start);

    std::map<std::set<state>, state> sI;
    sI.emplace(I0, start);
    std::queue<std::pair<std::set<state>, state>> q;
    q.emplace(I0, start);
    while(!q.empty()){
        auto [I0, from] = q.front();
        q.pop();
        for(const char c: char_table){
            std::set<state> I1 = std::set<state>();
            state to;
            auto cE = CreateElem(c, 1);
            auto [is_end, end_type] = FindISubset(I0, I1, cE, nfa);
            if(I1.size() == 0) continue;
            if(sI.find(I1) != sI.end()){
                this->from_to.emplace(path{from, cE->val()}, sI[I1]);
                continue;
            }
            if(is_end) to = std::make_shared<EndElem>(end_type, state_count[end_type]++);
            else to = std::make_shared<NodeElem>("DFA", (*counter)++);
            sI.emplace(I1, to);
            this->from_to.emplace(path{from, cE->val()}, to);
            q.emplace(I1, to);
            this->nodes.emplace(to);
            #ifdef TEST_DFAx
            std::cout<<std::endl<<from->val()<<std::endl;
            for(auto ii=I1.begin();ii!=I1.end();ii++){
                std::cout<<(*ii)->val()<<" ";
            }
            std::cout<<std::endl;
            #endif
        }
    }
    #ifdef TEST_DFA
    int cnt = 0;
    for(auto it=this->from_to.begin();it!=this->from_to.end();it++){
        ++cnt;
        std::cout<<(it->first).first->val()<<" =>\n";
            std::cout<<"  "<<(it->first).second<<" => "<<it->second->val()<<std::endl;
        std::cout<<std::endl;
    }
    std::cout<<cnt<<" edges.\n";
    std::cout<<nodes.size()<<" nodes.\n";
    #endif
}

void DFA::Minimize()
{
    state_count["mDFA"] = 0;
    int* counter = &state_count["mDFA"];
    std::vector<std::set<state>> G;         // nodes during minimization
    // std::vector<size_t> Gp;
    // auto emplace = [&G, &Gp](std::set<state>& g, size_t gp) {
    //     G.emplace_back(g);
    //     Gp.emplace_back(gp);
    // };
    // auto pop = [&G, &Gp]() {
    //     G.pop_back();
    //     Gp.pop_back();
    // };
    // emplace(std::set<state>(), 0);
    // emplace(std::set<state>(), 1);
    G.emplace_back(std::set<state>());
    G.emplace_back(std::set<state>());
    std::queue<std::set<state>> q;          // use to traversal
    std::map<state, std::set<state>> p;              // union-find sets
    for(auto& node: this->nodes){
        if(dynamic_cast<EndElem*>(node.get())){
            G[0].emplace(node);
            p[node] = G[0];
        }
        else{
            G[1].emplace(node);
            p[node] = G[1];
        }
    }
    if(G[1].size() == 0) G.pop_back();
    int xxx = 0;
    while(true){
        // std::cout<<(xxx++)<<std::endl;

        bool has_change = false;
        for(const char c: char_table){
            // std::cout<<c<<std::endl;
            auto cE = CreateElem(c, 1);
            std::vector<std::set<state>> temp_G;
            for(auto& g: G){
                if(g.size() == 1) temp_G.emplace_back(g);
                std::map<std::set<state>, size_t> map_to_next_G;
                int first = temp_G.size();
                temp_G.emplace_back(std::set<state>());
                for(auto it=g.begin(); it!=g.end(); it++){
                    auto to_it = this->from_to.find(path{*it, cE->val()});
                    if(to_it == this->from_to.end()){
                        temp_G[first].emplace(*it);
                        continue;
                    }
                    if(map_to_next_G.find(p[to_it->second]) == map_to_next_G.end()){
                        map_to_next_G[p[to_it->second]] = temp_G.size();
                        temp_G.push_back(std::set<state>({*it}));
                    }
                    else temp_G[map_to_next_G[p[to_it->second]]].emplace(*it);
                }
                if(temp_G[first].size() == 0) temp_G.erase(temp_G.begin()+first);
            }
            if(G.size() < temp_G.size()) has_change = true;
            G = temp_G;
            p.clear();
            for(auto& g: G) for(auto& st: g) p[st] = g;
        }
        if(!has_change) break;
    }

    std::map<path, state> new_from_to;
    std::vector<state> mDFANode(G.size());
    std::map<state, size_t> m;
    size_t index = 0;
    for(auto& g: G){
        bool is_end = false;
        bool is_start = false;
        for(auto& st: g){
            if(dynamic_cast<EndElem*>(st.get())) is_end = true;
            if(st == start) is_start = true;
        }
        state G0;
        if(is_start){
            if(is_end) mDFANode[index] = CreateNode('e', "mDFAstart", (*counter)++);
            else mDFANode[index] = CreateNode('s', "mDFA"), (*counter)++;
            start = mDFANode[index];
        }else{
            if(is_end) mDFANode[index] = CreateNode('e', "mDFA", (*counter)++);
            else mDFANode[index] = CreateNode('n', "mDFA", (*counter)++);
        }
        for(auto& st: g) m[st] = index;
        index++;
    }
    index = 0;
    for(auto& g: G){
        for(auto& st: g){
            for(const char c: char_table){
                auto cE = CreateElem(c, 1);
                auto to_it = this->from_to.find(path{st, cE->val()});
                if(to_it == this->from_to.end()) continue;
                new_from_to.emplace(path{mDFANode[index], cE->val()}, mDFANode[m[to_it->second]]);
            }
        }
        index++;
    }
    this->from_to = new_from_to;
    // this->nodes = mDFANode;
    #ifdef TEST_mDFA
    int cnt = 0;
    for(auto it=this->from_to.begin();it!=this->from_to.end();it++){
        ++cnt;
        std::cout<<(it->first).first->val()<<" =>\n";
            std::cout<<"  "<<(it->first).second<<" => "<<it->second->val()<<std::endl;
        std::cout<<std::endl;
    }
    std::cout<<cnt<<" edges.\n";
    std::cout<<nodes.size()<<" nodes.\n";
    #endif
}