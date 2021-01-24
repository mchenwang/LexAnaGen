#include <cstdio>
#include <iostream>
#include "config.h"
#include "Data.h"
#include "GNFA.h"
#include "GDFA.h"
#include "GLexA.h"

using namespace std;
using namespace GloData;

// bool GetNFA(GNFA::FinalNFA& ret)
// {
//     FILE* file = freopen(REPATH, "r", stdin);
//     if(file == NULL) { // file open failed.
//         std::cerr<<RENAME<<" can not open.\n";
//         return 1;
//     }
//     int n;
//     std::cin>>n;
//     std::map<std::string, LexType> type_check;
//     std::string re, re_type;
//     for(int i=0;i<n;i++) {
//         std::cin>>re_type;
//         if(type_check.find(re_type) == type_check.end()) {
//             type_check[re_type] = re_type;
//             state_count[re_type] = 0;
//         }
//     }
//     int err_cnt = 0;
//     std::vector<GNFA::NFA> nfas;
//     do getline(std::cin, re); while(re!="%");                    // just for robustness against RE.txt
//     while(std::cin>>re_type>>re) {
//         if(type_check.find(re_type) == type_check.end()){
//             std::cerr<<"Type "<<re_type<<" is not allowed!\n";
//             err_cnt++;
//         }
//         else if(!GNFA::CheckRE(re)){
//             std::cerr<<re<<" syntax error!\n";
//             err_cnt++;
//         }
//         else {
//             GNFA::NFA temp(type_check[re_type], re);
//             #ifdef TEST_INPUT
//             cout<<temp.type<<" "<<temp.str<<endl;
//             #endif
//             nfas.emplace_back(temp);
//         }
//     }
//     fclose(file);
//     if(nfas.size()<1) {
//         std::cerr<<"There is no correct regular expression!\nPlease check the file RE.txt.\n";
//         return 1;
//     }
//     std::cout<<nfas.size()<<" sub-NFAs are gotten."<<std::endl;
//     std::cout<<err_cnt<<" inputs are error."<<std::endl;
//     GNFA::UnionNFAs(ret, nfas);
//     state_count.clear();
//     return 0;
// }

int main()
{
    if(GetCharTable(CHARTABLE)) cout<<"Use default characters table.\n";
    bool flag = false;
    GNFA::FinalNFA fnfa;
    flag = GetNFA(fnfa);
    if(flag) return 1;
    cout<<"Get target NFA."<<endl;
    GDFA::DFA dfa;
    dfa.NFAToDFA(fnfa);
    // dfa.Minimize();
    flag = GLexAnalyzer::Generate(dfa, "../target/ana.cpp");
    freopen("CON","w",stdout);
    if(flag) cout<<"finish."<<endl;
    else cerr<<"error in generate."<<endl;
    return 0;
}