#include <cstdio>
#include <iostream>
#include "config.h"
#include "Data.h"
#include "GNFA.h"
#include "GDFA.h"
#include "GLexA.h"

using namespace std;
using namespace GloData;

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
    dfa.Minimize();
    flag = GLexAnalyzer::Generate(dfa, ANAPATH);
    freopen("CON","w",stdout);
    if(flag) cout<<"finish."<<endl;
    else cerr<<"error in generate."<<endl;
    return 0;
}