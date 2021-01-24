#include "Data.h"
#include <cstdio>
#include <iostream>
#include "config.h"

namespace GloData {

    std::unordered_map<LexType, int> state_count;
    
    std::string char_table;

    std::shared_ptr<Element<std::string>> CreateElem(char c, bool is_char)
    {
        if(is_char) return std::make_shared<CharElem>(c);
        switch (c)
        {
        case '*': return std::make_shared<ClosureElem>(); break;
        case '+': return std::make_shared<PlusElem>(); break;
        case '|': return std::make_shared<OrConElem>(); break;
        case '?': return std::make_shared<NonGreedyElem>(); break;
        // case '.': return std::make_shared<DotElem>(); break;
        case 'c': return std::make_shared<ConnectElem>(); break;
        case 'e': return std::make_shared<EpsElem>(); break;
        
        default: break;
        }
        return nullptr;
    }
    std::shared_ptr<NodeElem> CreateNode(char c, std::string&& type, int id)
    {
        switch (c)
        {
        case 's': return std::make_shared<StartElem>(type); break;
        case 'e': return std::make_shared<EndElem>(type, id); break;
        case 'n': return std::make_shared<NodeElem>(type, id); break;
        default: break;
        }
        return nullptr;
    }

    bool GetCharTable(const char* path)
    {
        FILE* file = freopen(path, "r", stdin);
        if(file == NULL){
            for(char c=32; c < 126; c++) char_table.push_back(c);
            // char_table.push_back(' ');
            return 1;
        }
        std::string c;
        while(std::cin>>c) char_table += c;
        fclose(file);
        return 0;
    }
};