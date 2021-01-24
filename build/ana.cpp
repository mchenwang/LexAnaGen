#include <iostream>
#include <string>
using namespace std;
using state = int;
using type = string;

state transform(state now, const char character){
    switch (now)
    {
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        if(character == '=') return 31;
        break;
    case 0:
        if(character == '"') return 5;
        if(character == '#') return 14;
        if(character == '&') return 2;
        if(character == '\'') return 1;
        if(character == '(') return 8;
        if(character == ')') return 6;
        if(character == '*') return 3;
        if(character == '+') return 4;
        if(character == ',') return 7;
        if(character == '-') return 10;
        if(character == '/') return 9;
        if(character == '0') return 13;
        if(character == '1') return 11;
        if(character == ';') return 12;
        if(character == '<') return 16;
        if(character == '=') return 15;
        if(character == '>') return 30;
        if(character == 'A') return 32;
        if(character == 'B') return 17;
        if(character == '_') return 18;
        if(character == 'a') return 19;
        if(character == 'b') return 28;
        if(character == 'i') return 27;
        if(character == 'm') return 24;
        if(character == 'r') return 22;
        if(character == '{') return 29;
        if(character == '}') return 20;
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    case 10:
        if(character == '=') return 25;
        break;
    case 11:
        if(character == '0') return 13;
        if(character == '1') return 11;
        break;
    case 12:
        break;
    case 13:
        if(character == '0') return 13;
        if(character == '1') return 11;
        break;
    case 14:
        break;
    case 15:
        if(character == '=') return 21;
        break;
    case 16:
        break;
    case 17:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 18:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 19:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 20:
        break;
    case 21:
        break;
    case 22:
        if(character == 'e') return 38;
        break;
    case 23:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 24:
        if(character == 'a') return 39;
        break;
    case 25:
        break;
    case 26:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 27:
        if(character == 'n') return 36;
        break;
    case 28:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 29:
        break;
    case 30:
        break;
    case 31:
        break;
    case 32:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 33:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 34:
        break;
    case 35:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 36:
        if(character == 'c') return 37;
        if(character == 't') return 34;
        break;
    case 37:
        if(character == 'l') return 46;
        break;
    case 38:
        if(character == 't') return 47;
        break;
    case 39:
        if(character == 'i') return 41;
        break;
    case 40:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 41:
        if(character == 'n') return 48;
        break;
    case 42:
        if(character == 'd') return 51;
        break;
    case 43:
        if(character == 'n') return 50;
        break;
    case 44:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 45:
        if(character == '0') return 23;
        if(character == '1') return 26;
        if(character == 'A') return 33;
        if(character == 'B') return 35;
        if(character == '_') return 40;
        if(character == 'a') return 45;
        if(character == 'b') return 44;
        break;
    case 46:
        if(character == 'u') return 42;
        break;
    case 47:
        if(character == 'u') return 49;
        break;
    case 48:
        break;
    case 49:
        if(character == 'r') return 43;
        break;
    case 50:
        break;
    case 51:
        if(character == 'e') return 52;
        break;
    case 52:
        break;
    }
    return -1;
}
type getType(state now){
    switch (now)
    {
    case 1:
        return "SYMBOL";
    case 2:
        return "OPERATOR";
    case 3:
        return "OPERATOR";
    case 4:
        return "OPERATOR";
    case 0:
        return "NUMBER";
    case 5:
        return "SYMBOL";
    case 6:
        return "SEPARATOR";
    case 7:
        return "SEPARATOR";
    case 8:
        return "SEPARATOR";
    case 9:
        return "OPERATOR";
    case 10:
        return "OPERATOR";
    case 11:
        return "NUMBER";
    case 12:
        return "SEPARATOR";
    case 13:
        return "NUMBER";
    case 14:
        return "SYMBOL";
    case 15:
        return "OPERATOR";
    case 16:
        return "SYMBOL";
    case 17:
        return "ID";
    case 18:
        return "ID";
    case 19:
        return "ID";
    case 20:
        return "SEPARATOR";
    case 21:
        return "OPERATOR";
    case 23:
        return "ID";
    case 25:
        return "OPERATOR";
    case 26:
        return "ID";
    case 28:
        return "ID";
    case 29:
        return "SEPARATOR";
    case 30:
        return "SYMBOL";
    case 31:
        return "OPERATOR";
    case 32:
        return "ID";
    case 33:
        return "ID";
    case 34:
        return "KEYWORD";
    case 35:
        return "ID";
    case 40:
        return "ID";
    case 44:
        return "ID";
    case 45:
        return "ID";
    case 48:
        return "KEYWORD";
    case 50:
        return "KEYWORD";
    case 52:
        return "KEYWORD";
    }
    return std::string();
}
int main(int argc, char* argv[]) {
    if(argc < 2){
        cout<<"missing program file to be analyzed.\n";
        return 1;
    }
    if(argc > 2) cout<<"there are redundant parameters."<<endl;
    FILE* file = freopen(argv[1], "r", stdin);
    if(file == NULL){
        cerr<<"can not open "<<argv[1]<<"\n";
        return 1;
    }
    string line;
    short is_annotation = 0;
    while(getline(cin, line)) {
        int len = line.size();
        int i = 0;
        if(is_annotation == 2) {
            bool an_is_end = false;
            while(i<len-1) {
                if(line[i]=='*'&&line[i+1]=='/') {
                    an_is_end = true;
                    break;
                }
                i++;
            }
            if(!an_is_end) continue;
            i+=2;
        }
        is_annotation = 0;
        state now = 0;
        state last = -1;
        int start = i;
        while(i<len-1){
            if(line[i]=='/'){
                if(line[i+1]=='/') break;
                int temp = i;
                if(line[i+1]=='*') is_annotation = 2, i+=2;
                while(i<len-1){
                    if(line[i]=='*'&&line[i+1]=='/'){
                        is_annotation = 0;
                        break;
                    }
                    i++;
                }
                if(is_annotation){
                    i=temp;
                    break;
                }
                if(last == -1){
                    cerr<<line.substr(start, i-start+1)<<" error"<<endl;
                    return 1;
                }
                else{
                    cout<<line.substr(start, temp-start)<<"0"<<getType(last)<<endl;
                    now = 0;
                    last = -1;
                    i+=2;
                    start = i;
                }
            }
            now = transform(now, line[i]);
            if(now == -1){
                if(last == -1){
                    cerr<<line.substr(start, i-start+1)<<" error"<<endl;
                    return 1;
                }
                else{
                    cout<<line.substr(start, i-start)<<" "<<getType(last)<<endl;
                    now = 0;
                    last = -1;
                    start = i;
                }
            }
            else {
                if(!getType(now).empty()) last = now;
                i++;
            }
        }
        if(is_annotation == 0 && i<len) now = transform(now, line[i++]);
        if(now == 0) continue;
        if(now == -1 || getType(now).empty()){
            if(last == -1){
                cerr<<line.substr(start, i-start)<<" error"<<endl;
                return 1;
            }
            else{
                cout<<line.substr(start, i-start-1)<<" "<<getType(last)<<endl;
                now = transform(0, line[i-1]);
                start = i-1;
            }
            if(now == -1 || getType(now).empty()){
                cerr<<line.substr(start, i-start)<<" error"<<endl;
                return 1;
            }
        }
        if(i<=len) cout<<line.substr(start, i-start)<<" "<<getType(now)<<endl;
    }
    fclose(file);
    return 0;
}
