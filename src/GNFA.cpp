#include "GNFA.h"
#include <stack>
#include <memory>
#include <set>
#include "config.h"
#include <iostream>

#ifdef TEST
#include <assert.h>
#endif

using namespace GNFA;

bool GNFA::CheckRE(std::string re)
{
    if(re.size() < 1) return true;
    // remove escape characters
    for (size_t i = 0; i < re.size();){
        if (re[i] == '\\'){
            if(i == re.size()-1) return false;
            re.erase(i, 1);
            re[i] = '!'; // replaced by a relatedless character
        }
        else i++;
    }
    // check multi-expression *
    if(re[0] == '*') return false;
    bool check_flag = false;
    for (size_t i = 0; i < re.size(); i++) {
        if(re[i] == '[') check_flag = true;
        else if(re[i] == ']') check_flag = false;
        else if(re[i] == '*'){
            if(check_flag) return false;
            if(re[i - 1] == '*' || re[i - 1] == '-' || re[i - 1] == '(') return false;
        }
    }
    // check ()/[]
    if(check_flag) return false;
    std::stack<char> s;
    for (size_t i = 0; i < re.size(); i++)
    {
        if(re[i] == '[' || re[i] == '(') s.push(re[i]);
        else if(re[i] == ']' || re[i] == ')'){
            if (s.empty()) return false;
            if ((re[i] == ')' && s.top() == '(') || (re[i] == ']' && s.top() == '['))
                s.pop();
            else return false;
        }
    }
    // check ellipsis expression (e.g. a-z)
    if(re[0] == '-' || re[re.size() - 1] == '-') return false;
    auto rangable = [](char c) -> bool {
        return (c>='0' && c<='9') || (c>='a' && c<='z') || (c>='A' && c<='Z');
    };
    for (size_t i = 1; i < re.size() - 1; i++) if (re[i] == '-') {
        if (!rangable(re[i-1]) || !rangable(re[i+1])) return false;
    }
    return s.empty();
}

NFA::NFA(LexType _type, std::string _re)
{
    SetRE(_re);
    SetREType(_type);
    start = std::make_shared<StartElem>();
    end = std::make_shared<EndElem>();
    GenNFA();
}

// TODO extend . to all characters in char_table
void GNFA::NormalizeRE(std::string& re)
{
    std::string temp = "";
    auto tostr = [](const char c) -> std::string { return std::string(1, c); };
    for (size_t i = 0; i < re.size(); i++) {
        if (re[i] == '\\') {
            temp = temp + tostr(re[i]) + tostr(re[i+1]);
            i++;   
        }
        else {
            if(re[i] != '[') temp = temp + tostr(re[i]);
            else {
                i++;
                temp = temp + tostr('(');
                bool flag = false;
                while (i<re.size()) {
                    if (re[i] == ']') break;
                    else if (re[i] == '\\') {
                        if (flag) temp = temp + tostr('|');
                        temp = temp + tostr(re[i]) + tostr(re[i+1]);
                        i++;   
                    }
                    else if (re[i] == '-') {
                        for (char c = re[i - 1] + 1; c < re[i + 1]; c++) {
                            temp = temp + tostr('|');
                            temp = temp + c;
                        }
                    }
                    else {
                        if (flag) temp = temp + tostr('|');
                        temp = temp + tostr(re[i]);
                    }
                    flag = true;
                    i++;
                }
                temp = temp + tostr(')');
            }
        }
    }
    re = temp;
}

int priority(char c)
{
    switch (c)
    {
    case '*': return 1; break;
    case '?': return 1; break;
    case '+': return 1; break;
    // case '(': return 0; break;
    case '|': return 0; break;
    default: break;
    }
    return -1;
}

inline bool IsSpecial(char c)
{
    return c == '(' || c == ')' || c == '*' || c == '+' || c == '?' || c == '|' /*|| c == '.'*/;
}

void NFA::StrToREE()
{
    std::stack<char> stk;
    std::stack<int> cnt;
    cnt.push(0);
    for(size_t i=0;i<str.size();i++){
        if(IsSpecial(str[i])){
            /*if(str[i] == '.') re_nodes.emplace_back(std::make_shared<DotElem>()), cnt.top()++;
            else*/
            if(str[i] == ')'){
                while(!stk.empty() && stk.top() != '('){
                    re_nodes.emplace_back(CreateElem(stk.top()));
                    if(dynamic_cast<ConnectElem*>(re_nodes.back().get())) cnt.top()--;
                    stk.pop();
                }
                #ifdef TEST
                assert(!stk.empty());
                assert(!cnt.empty());
                #else
                if(!stk.empty())
                #endif
                  stk.pop();
                
                while(cnt.top()>1) re_nodes.push_back(std::make_shared<ConnectElem>()), cnt.top()--;
                // re_nodes.emplace_back(std::make_shared<ConnectElem>());
                cnt.pop();
            }
            else if(priority(str[i]) == 1) {
                re_nodes.emplace_back(CreateElem(str[i]));
            }
            else if(priority(str[i]) == 0) stk.push(str[i]);
            else{
                while(!stk.empty() && stk.top() != '('){
                    re_nodes.emplace_back(CreateElem(stk.top()));
                    if(dynamic_cast<ConnectElem*>(re_nodes.back().get())) cnt.top()--;
                    stk.pop();
                }
                cnt.top()++;
                cnt.push(0);
                stk.push(str[i]);
            }
        }
        else{
            if(str[i] == '\\') i++;
            #ifdef TEST
            assert(i<str.size());
            #else
            if(i<str.size())
            #endif
              re_nodes.emplace_back(std::make_shared<CharElem>(str[i]));
            cnt.top()++;
            while(!stk.empty() && stk.top() != '('){
                re_nodes.emplace_back(CreateElem(stk.top()));
                if(dynamic_cast<ConnectElem*>(re_nodes.back().get())) cnt.top()--;
                stk.pop();
            }
        }
    }
    while(cnt.top()>1) re_nodes.push_back(std::make_shared<ConnectElem>()), cnt.top()--;
#ifdef TEST_NFA
    std::cout<<std::endl;
    for (auto node: re_nodes)
    {
        std::cout<<node->type()<<" ";
    }
    std::cout<<std::endl;
#endif
}

void NFA::SetRE(std::string _str)
{
    NormalizeRE(_str);
    this->str = _str;
    StrToREE();
}

void NFA::SetREType(LexType _type) noexcept
{
    this->type = _type;
}
// from nfas to final nfa, ret is the return value
void GNFA::UnionNFAs(FinalNFA &ret, std::vector<NFA> &nfas)
{
    std::set<LexType> vis;
    for(auto& nfa: nfas){
        ret.from_to.insert(nfa.from_to.begin(), nfa.from_to.end());
        ret.from_to.emplace(path{ret.start, EpsElem::e()}, nfa.start);
        if(vis.find(nfa.type) == vis.end()){
            vis.insert(nfa.type);
            ret.end.emplace_back(std::make_shared<EndElem>(nfa.type));
        }
        ret.from_to.emplace(path{nfa.end, EpsElem::e()}, std::make_shared<EndElem>(nfa.type));
    }
    
    #ifdef TEST_NFA
    for(auto it=ret.from_to.begin();it!=ret.from_to.end();){
        std::cout<<(it->first).first.get()->val()<<" =>\n";
        auto se = (it->first).second;
        auto t = ret.from_to.equal_range(it->first);
        for(it=t.first;it!=t.second;it++){
            // if(dynamic_cast<EndElem*>(&(it->second)))
            //     std::cout<<"  "<<se<<" => "<<static_cast<EndElem*>(&(it->second))->val()<<std::endl;
            std::cout<<"  "<<(it->first).second<<" => "<<it->second.get()->val()<<std::endl;
        }
        std::cout<<std::endl;
    }
    #endif
}

void NFA::AddEdge(const path p, const state s)
{
    this->from_to.emplace(p, s);
}
void NFA::GenNFA()
{
    int *from = nullptr;
    if(state_count.find(this->type) == state_count.end()){
        state_count[this->type] = 0;
    }
    from = &state_count[this->type];
    auto GetState = [this](int x) -> state {
        return std::make_shared<NodeElem>(this->type, x);
    };
    // this->nodes.push_back(this->type + std::to_string(*from));
    // RGenNFA(this->re, from);
    std::stack<SubNFA> stk;

    for(auto& node: this->re_nodes){
        REE* p = node.get();
        SubNFA temp;
        if(dynamic_cast<CharElem*>(p)){
            temp = SubNFA(GetState(*from), GetState(1+(*from)));
            (*from) += 2;
            AddEdge(path{temp.start, static_cast<CharElem*>(p)->val()}, temp.end);
        }
        else{
            #ifdef TEST
            assert(!stk.empty());
            #else
            if(stk.empty()) continue;
            #endif
            temp = stk.top();
            stk.pop();
            if(dynamic_cast<ConnectElem*>(p)){
                #ifdef TEST
                assert(!stk.empty());
                #else
                if(stk.empty()) continue;
                #endif
                SubNFA temp2 = stk.top();
                stk.pop();
                if(dynamic_cast<OrConElem*>(p)){
                    state new_start = GetState((*from)++);
                    state new_end = GetState((*from)++);
                    AddEdge(path{new_start, EpsElem::e()}, temp.start);
                    AddEdge(path{new_start, EpsElem::e()}, temp2.start);
                    AddEdge(path{temp.end, EpsElem::e()}, new_end);
                    AddEdge(path{temp2.end, EpsElem::e()}, new_end);
                    temp = SubNFA(new_start, new_end);
                }
                else{
                    AddEdge(path{temp2.end, EpsElem::e()}, temp.start);
                    temp = SubNFA(temp2.start, temp.end);
                }
            }
            else if(dynamic_cast<ClosureElem*>(p)){
                if(dynamic_cast<PlusElem*>(p)){
                    state new_start = GetState((*from)++);
                    state new_end = GetState((*from)++);
                    AddEdge(path{new_start, EpsElem::e()}, temp.start);
                    AddEdge(path{temp.end, EpsElem::e()}, new_end);
                    AddEdge(path{temp.end, EpsElem::e()}, temp.start);
                    temp = SubNFA(new_start, new_end);
                }
                else{
                    state new_start = GetState((*from)++);
                    state middle = GetState((*from)++);
                    state new_end = GetState((*from)++);
                    AddEdge(path{new_start, EpsElem::e()}, middle);
                    AddEdge(path{middle, EpsElem::e()}, temp.start);
                    AddEdge(path{temp.end, EpsElem::e()}, middle);
                    AddEdge(path{middle, EpsElem::e()}, new_end);
                    temp = SubNFA(new_start, new_end);
                }
            }
            else if(dynamic_cast<NonGreedyElem*>(p)){
                state new_start = GetState((*from)++);
                state new_end = GetState((*from)++);
                AddEdge(path{new_start, EpsElem::e()}, temp.start);
                AddEdge(path{temp.end, EpsElem::e()}, new_end);
                AddEdge(path{new_start, EpsElem::e()}, new_end);
                temp = SubNFA(new_start, new_end);
            }
        }
        stk.push(temp);
    }
    #ifdef TEST_NFA
    assert(stk.size() == 1);
    this->start = stk.top().start;
    this->end = stk.top().end;
    for(auto it=this->from_to.begin();it!=this->from_to.end();){
        std::cout<<(it->first).first.get()->val()<<" =>"<<std::endl;
        auto ret = from_to.equal_range(it->first);
        auto se = (it->first).second;
        for(it = ret.first; it != ret.second; ++it)
            std::cout<<"  "<<se<<" => "<<(*it).second.get()->val()<<std::endl;
        std::cout<<std::endl;
    }
    std::cout<<this->start.get()->val()<<" =>"<<this->end.get()->val()<<std::endl;
    #else
    if(stk.empty()) return;
    this->start = stk.top().start;
    this->end = stk.top().end;
    #endif
}

bool GetNFA(GNFA::FinalNFA& ret)
{
    FILE* file = freopen(REPATH, "r", stdin);
    if(file == NULL) { // file open failed.
        std::cerr<<RENAME<<" can not open.\n";
        return 1;
    }
    int n;
    std::cin>>n;
    std::map<std::string, LexType> type_check;
    std::string re, re_type;
    for(int i=0;i<n;i++) {
        std::cin>>re_type;
        if(type_check.find(re_type) == type_check.end()) {
            type_check[re_type] = re_type;
            state_count[re_type] = 0;
        }
    }
    int err_cnt = 0;
    std::vector<GNFA::NFA> nfas;
    do getline(std::cin, re); while(re!="%");                    // just for robustness against RE.txt
    while(std::getline(std::cin, re)) {
        size_t space_index = re.find(' ');
        re_type = re.substr(0, space_index);
        re = re.substr(space_index + 1);
        if(type_check.find(re_type) == type_check.end()){
            std::cerr<<"Type "<<re_type<<" is not allowed!\n";
            err_cnt++;
        }
        else if(!GNFA::CheckRE(re)){
            std::cerr<<re<<" syntax error!\n";
            err_cnt++;
        }
        else {
            GNFA::NFA temp(type_check[re_type], re);
            #ifdef TEST_INPUT
            cout<<temp.type<<" "<<temp.str<<endl;
            #endif
            nfas.emplace_back(temp);
        }
    }
    fclose(file);
    if(nfas.size()<1) {
        std::cerr<<"There is no correct regular expression!\nPlease check the file RE.txt.\n";
        return 1;
    }
    std::cout<<nfas.size()<<" sub-NFAs are gotten."<<std::endl;
    std::cout<<err_cnt<<" inputs are error."<<std::endl;
    GNFA::UnionNFAs(ret, nfas);
    state_count.clear();
    return 0;
}
