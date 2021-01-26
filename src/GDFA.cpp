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
        }
    }
    #ifdef TEST_DFA
    std::cout<<"**************** DFA"<<std::endl;
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

size_t findp(std::vector<size_t>& p, size_t x) { return x==p[x]?x:p[x]=findp(p, p[x]); }

template<class T> void clear_swap(T& x){ T().swap(x); }

void DFA::Minimize()
{
    clear_swap(state_count);
    std::vector<state> nodes(this->nodes.size());
    std::unordered_map<state, size_t> index;
    size_t i = 0, start_index = -1;
    for(auto& node: this->nodes) index[node] = i, nodes[i++] = node;
    std::vector<size_t> p(nodes.size());
    std::vector<std::vector<size_t>> end_G;
    std::vector<std::vector<size_t>> G;
    i = 0;
    for(i = 0; i<nodes.size(); i++){
        state& node = nodes[i];
        if(node == this->start) start_index = i;
        
        if(dynamic_cast<EndElem*>(node.get())){
            if(state_count.find(node->node_type) == state_count.end()){
                state_count[node->node_type] = end_G.size();
                end_G.push_back({i});
            }
            else end_G[state_count[node->node_type]].push_back(i);
            p[i] = i;
        }
        else{
            if(G.size() == 0) G.push_back({i}), p[i] = i;
            else G[0].push_back(i), p[i] = G[0][0]; // p[G[0][0]] == G[0][0]
        }
    }
    std::vector<bool> visable(char_table.size(), true);

    while(true){
        bool change_flag = false;
        // if(!visable[c_i++])
        for(size_t c_i = 0; c_i<char_table.size(); c_i++) if(visable[c_i++]) {
            char c = char_table[c_i];
            std::vector<std::vector<size_t>> temp_G;
            bool visable_flag = false;
            for(size_t i=0;i<G.size();i++){
                if(G[i].size() == 1) {
                    temp_G.push_back(G[i]);
                    continue;
                }
                std::vector<std::vector<size_t>> div_G;
                std::vector<std::vector<size_t>> div_end_G;
                std::unordered_map<int, size_t> map_to_div;
                for(size_t j=0;j<G[i].size();j++){
                    state& node = nodes[G[i][j]];
                    auto ptr = this->from_to.find(path{node, CharElem::c(c)});
                    if(ptr == this->from_to.end()) {
                        if(map_to_div.find(-1) == map_to_div.end()){
                            map_to_div[-1] = div_G.size();
                            div_G.push_back({G[i][j]});
                        }
                        else div_G[map_to_div[-1]].push_back(G[i][j]);
                        continue;
                    }
                    visable_flag = true;
                    state& to = ptr->second;
                    if(dynamic_cast<EndElem*>(to.get())){
                        if(map_to_div.find(-index[to]-2) == map_to_div.end()){  // solve the conflict with -1
                            map_to_div[-index[to]-2] = div_end_G.size();
                            div_end_G.push_back({G[i][j]});
                        }
                        else div_end_G[map_to_div[-index[to]-2]].push_back(G[i][j]);
                    }
                    else{
                        size_t f_to = findp(p, index[to]);
                        if(map_to_div.find(f_to) == map_to_div.end()){
                            map_to_div[f_to] = div_G.size();
                            div_G.push_back({G[i][j]});
                        }
                        else div_G[map_to_div[f_to]].push_back(G[i][j]);
                    }
                }
                temp_G.insert(temp_G.end(), div_G.begin(), div_G.end());
                temp_G.insert(temp_G.end(), div_end_G.begin(), div_end_G.end());
                clear_swap(div_G);
                clear_swap(div_end_G);
                clear_swap(map_to_div);
            }
            if(visable_flag == false) visable[c_i] = false;
            if(G.size() < temp_G.size()) change_flag = true;
            G.swap(temp_G);
            for(size_t i=0;i<G.size();i++){
                for(size_t j=0;j<G[i].size();j++) p[G[i][j]] = i;
            }
            clear_swap(temp_G);
        }
        if(!change_flag) break;
    }
    clear_swap(index);
    std::vector<state> new_nodes;
    bool start_flag = false, had_found = false;
    for(i = 0; i < end_G.size(); i++){
        for(size_t j = 0; !had_found&&j<end_G[i].size(); j++) if(end_G[i][j] == start_index) start_flag = had_found = true;
        if(start_flag){
            #ifdef TEST
            // "start" is used for me to find out the start node
            // however, which is not need for subsequent algorithm
            this->start = CreateNode('e', "start"+nodes[end_G[i][0]]->node_type);
            #else
            this->start = CreateNode('e', std::move(nodes[end_G[i][0]]->node_type));
            #endif
            new_nodes.push_back(this->start);
            start_flag = false;
        }
        else new_nodes.push_back(CreateNode('e', std::move(nodes[end_G[i][0]]->node_type)));
        for(size_t j = 0; j<end_G[i].size(); j++) index[nodes[end_G[i][j]]] = new_nodes.size() - 1;
    }
    state_count["mDFA"] = 0;
    int* counter = &state_count["mDFA"];
    for(i = 0; i < G.size(); i++){
        for(size_t j = 0; !had_found&&j<G[i].size(); j++) if(G[i][j] == start_index) start_flag = had_found = true;
        if(start_flag){
            this->start = CreateNode('s', "mDFA"), (*counter)++;
            new_nodes.push_back(this->start);
            start_flag = false;
        }
        else {new_nodes.push_back(CreateNode('n', "mDFA", (*counter)++));}
        for(size_t j = 0; j<G[i].size(); j++) index[nodes[G[i][j]]] = new_nodes.size() - 1;
    }
    std::map<path, state> new_from_to;
    for(i = 0; i<G.size(); i++){
        for(size_t j=0; j<G[i].size(); j++){
            for(const char c: char_table){
                auto ptr = this->from_to.find(path{nodes[G[i][j]], CharElem::c(c)});
                if(ptr == this->from_to.end()) continue;
                state& node = ptr->second;
                new_from_to.emplace(path{new_nodes[index[nodes[G[i][j]]]], CharElem::c(c)}, new_nodes[index[node]]);
            }
        }
    }
    for(i = 0; i<end_G.size(); i++){
        for(size_t j=0; j<end_G[i].size(); j++){
            for(const char c: char_table){
                auto ptr = this->from_to.find(path{nodes[end_G[i][j]], CharElem::c(c)});
                if(ptr == this->from_to.end()) continue;
                state& node = ptr->second;
                new_from_to.emplace(path{new_nodes[index[nodes[end_G[i][j]]]], CharElem::c(c)}, new_nodes[index[node]]);
            }
        }
    }
    this->from_to.swap(new_from_to);
    clear_swap(this->nodes);
    this->nodes.insert(new_nodes.begin(), new_nodes.end());
    
    #ifdef TEST_mDFA
    std::cout<<"**************** mDFA"<<std::endl;
    int cnt = 0;
    for(auto it=this->from_to.begin();it!=this->from_to.end();it++){
        ++cnt;
        std::cout<<(it->first).first->val()<<" =>\n";
            std::cout<<"  "<<(it->first).second<<" => "<<it->second->val()<<std::endl;
        std::cout<<std::endl;
    }
    std::cout<<cnt<<" edges.\n";
    std::cout<<this->nodes.size()<<" nodes.\n";
    #endif
}