#include "GDFA.h"
#include "GLexA.h"
#include "Data.h"
#include "config.h"
#include <iostream>
#include <map>

using namespace GLexAnalyzer;

bool GLexAnalyzer::Generate(DFA dfa, const char* save_path){
    FILE* file = freopen(save_path, "w", stdout);
    if(file == NULL) return false;
    std::map<state, size_t> index;
    size_t i = 1;
    for(auto it = dfa.nodes.begin(); it!=dfa.nodes.end(); it++){
        state node = *it;
        if(node == dfa.start) index[node] = 0;
        else index[node] = i++;
    }

    std::cout<<"#include <iostream>"<<std::endl
             <<"#include <string>"<<std::endl
             <<"using namespace std;"<<std::endl
             <<"using state = int;"<<std::endl
             <<"using type = string;"<<std::endl<<std::endl;

    std::cout<<"state transform(state now, const char character){\n"
             <<"    switch (now)"<<std::endl
             <<"    {"<<std::endl;
    for(auto it = dfa.nodes.begin(); it!=dfa.nodes.end(); it++){
        state from = *it;
    std::cout<<"    case "<<index[from]<<":"<<std::endl;
        for(const char c: char_table){
            auto to_ptr = dfa.from_to.find(path{from, CharElem::c(c)});
            if(to_ptr == dfa.from_to.end()) continue;
            if(c == '\'')
    std::cout<<"        if(character == '\\"<<c<<"') return "<<index[to_ptr->second]<<";"<<std::endl;
            else
    std::cout<<"        if(character == '"<<c<<"') return "<<index[to_ptr->second]<<";"<<std::endl;
        }
    std::cout<<"        break;"<<std::endl;
    }
    std::cout<<"    }"<<std::endl
             <<"    return -1;"<<std::endl
             <<"}"<<std::endl;;

    std::cout<<"type getType(state now){\n"
             <<"    switch (now)"<<std::endl
             <<"    {"<<std::endl;
    for(auto it = dfa.nodes.begin(); it!=dfa.nodes.end(); it++){
        state node = *it;
        if(dynamic_cast<EndElem*>(node.get())){
    std::cout<<"    case "<<index[node]<<":"<<std::endl;
    std::cout<<"        return \""<<node->node_type<<"\";"<<std::endl;
        }
    }
    std::cout<<"    }"<<std::endl
             <<"    return std::string();"<<std::endl
             <<"}"<<std::endl<<std::endl;
    
    std::cout<<"int main(int argc, char* argv[]) {"<<std::endl
             <<"    if(argc < 2){"<<std::endl
             <<"        cout<<\"missing program file to be analyzed.\\n\";"<<std::endl
             <<"        return 1;"<<std::endl
             <<"    }"<<std::endl
             <<"    if(argc > 2) cout<<\"there are redundant parameters.\"<<endl;"<<std::endl
             <<"    FILE* file = freopen(argv[1], \"r\", stdin);"<<std::endl
             <<"    if(file == NULL){"<<std::endl
             <<"        cerr<<\"can not open \"<<argv[1]<<\"\\n\";"<<std::endl
             <<"        return 1;"<<std::endl
             <<"    }"<<std::endl
             <<"    string line;"<<std::endl
             <<"    short is_annotation = 0;"<<std::endl
             <<"    while(getline(cin, line)) {"<<std::endl
             <<"        int len = line.size();"<<std::endl
             <<"        int i = 0;"<<std::endl
             <<"        if(is_annotation == 2) {"<<std::endl
             <<"            while(i<len-1 && is_annotation) {"<<std::endl
             <<"                if(line[i]=='*'&&line[i+1]=='/') is_annotation = 0;"<<std::endl
             <<"                i++;"<<std::endl
             <<"            }"<<std::endl
             <<"            if(is_annotation) continue;"<<std::endl
             <<"            i++;"<<std::endl
             <<"        }"<<std::endl
             <<"        is_annotation = 0;"<<std::endl
             <<"        state now = 0;"<<std::endl
             <<"        state last = -1;"<<std::endl
             <<"        int start = i;"<<std::endl
             <<"        int last_index = i;"<<std::endl
             <<"        while(i<=len){"<<std::endl
             <<"            if(i == len){"<<std::endl
             <<"                if(now == 0) break;"<<std::endl
             <<"                if(now == -1 || getType(now).empty()){"<<std::endl
             <<"                    if(last != -1){"<<std::endl
             <<"                        cout<<line.substr(start, last_index-start+1)<<\" \"<<getType(last)<<endl;"<<std::endl
             <<"                        now = 0, last = -1;"<<std::endl
             <<"                        start = i = last_index+1;"<<std::endl
             <<"                        continue;"<<std::endl
             <<"                    }"<<std::endl
             <<"                    cerr<<line.substr(start, i-start)<<\" error\"<<endl;"<<std::endl
             <<"                    return 1;"<<std::endl
             <<"                }"<<std::endl
             <<"                cout<<line.substr(start, i-start)<<\" \"<<getType(now)<<endl;"<<std::endl
             <<"                break;"<<std::endl
             <<"            }"<<std::endl
             <<"            if(line[i]=='/' && i<len-1){"<<std::endl
             <<"                if(line[i+1] == '/' || line[i+1] == '*'){"<<std::endl
             <<"                    if(now == -1 || (now!=0 && getType(now).empty())){"<<std::endl
             <<"                        if(last != -1){"<<std::endl
             <<"                            cout<<line.substr(start, last_index-start+1)<<\" \"<<getType(last)<<endl;"<<std::endl
             <<"                            now = 0, last = -1;"<<std::endl
             <<"                            start = i = last_index+1;"<<std::endl
             <<"                            continue;"<<std::endl
             <<"                        }"<<std::endl
             <<"                        cerr<<line.substr(start, i-start)<<\" error\"<<endl;"<<std::endl
             <<"                        return 1;"<<std::endl
             <<"                    }"<<std::endl
             <<"                    if(!getType(now).empty()) cout<<line.substr(start, i-start)<<\" \"<<getType(now)<<endl;"<<std::endl
             <<"                    is_annotation = 1, now = 0;"<<std::endl
             <<"                    if(line[i+1] == '/') break;"<<std::endl
             <<"                    is_annotation = 2, i += 2;"<<std::endl
             <<"                    while(i<len-1 && is_annotation){"<<std::endl
             <<"                        if(line[i] == '*' && line[i+1] == '/') is_annotation = 0;"<<std::endl
             <<"                        i++;"<<std::endl
             <<"                    }"<<std::endl
             <<"                    if(is_annotation) break;"<<std::endl
             <<"                    start = ++i, last = -1;"<<std::endl
             <<"                }"<<std::endl
             <<"            }"<<std::endl
             <<"            now = transform(now, line[i]);"<<std::endl
             <<"            if(now == -1){"<<std::endl
             <<"                if(last != -1){"<<std::endl
             <<"                    cout<<line.substr(start, last_index-start+1)<<\" \"<<getType(last)<<endl;"<<std::endl
             <<"                    now = 0, last = -1;"<<std::endl
             <<"                    start = i = last_index+1;"<<std::endl
             <<"                    continue;"<<std::endl
             <<"                }"<<std::endl
             <<"                cerr<<line.substr(start, i-start+1)<<\" error\"<<endl;"<<std::endl
             <<"                return 1;"<<std::endl
             <<"            }"<<std::endl
             <<"            else {"<<std::endl
             <<"                if(!getType(now).empty()) last = now, last_index = i;"<<std::endl
             <<"                i++;"<<std::endl
             <<"            }"<<std::endl
             <<"        }"<<std::endl
             <<"    }"<<std::endl
             <<"    fclose(file);"<<std::endl
             <<"    return 0;"<<std::endl
             <<"}"<<std::endl;
    fclose(file);
    return true;
}