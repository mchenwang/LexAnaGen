/* Global data
*  code by Miawua
*/
#ifndef __DATA_H__
#define __DATA_H__

#include <string>
// #include <GNFA.h>
#include <unordered_map>
#include <memory>

namespace GloData{
    using LexType = std::string;
    template <class T>
    class Element {
    public:
        Element() = default;
        ~Element()= default;
        virtual T type() const = 0;
        inline virtual std::string val() const noexcept { return type(); }
    };

    class NodeElem : public Element<std::string> {
    public:
        NodeElem() = default;
        NodeElem(const std::string _nt, const int _i=-1) noexcept
        :node_type(_nt), id(_i) {;}
        std::string type() const noexcept { return "node"; }
        inline std::string val() const noexcept { return node_type + std::to_string(id); }
        std::string node_type;
        int id;
        // friend bool operator<(const std::shared_ptr<NodeElem>& x, const std::shared_ptr<NodeElem>& y)
        // {
        //     if(x->node_type == y->node_type) return x->id<y->id;
        //     else return x->node_type<y->node_type;
        //     // return x->val() < y->val();
        // }
        bool operator<(const NodeElem& x) const{
            return val() < x.val();
        }
    };

    class StartElem : public NodeElem {
    public:
        StartElem() = default;
        StartElem(const std::string _nt, const int _i=-1) noexcept
        :NodeElem(_nt, _i) {;}
        std::string type() const noexcept { return "start"; }
        // static std::string e() { return "start"; }
        inline std::string val() const noexcept override  { return node_type + type(); }
    };
    class EndElem : public NodeElem {
    public:
        EndElem() = default;
        EndElem(const std::string _nt, const int _i=-1) noexcept
        :NodeElem(_nt, _i) {;}
        std::string type() const noexcept { return "end"; }
        // static std::string e() { return "end"; }
        inline std::string val() const noexcept override  {
            return node_type + type() + (id == -1?"":std::to_string(id));
        }
    };

    class CharElem : public Element<std::string> {
    public:
        CharElem(char _v) noexcept
        :value(_v) { ; }
        std::string type() const noexcept { return std::string(1, value); }   // char
        // Override
        inline std::string val() const noexcept override  { return " "+type(); } // use to distinguish special characters
        static std::string c(char c) { return " "+std::string(1, c); }
        char value;
    };

    /*class DotElem : public Element<std::string> {
    public:
        DotElem() = default;
        std::string type() noexcept { return "."; }    // all characters
    };*/

    class EpsElem : public Element<std::string> {
    public:
        EpsElem() = default;
        std::string type() const noexcept { return "eps"; }
        static std::string e() { return "eps"; }
        inline std::string val() const noexcept { return "eps"; }
    };
    // binary op
    class ConnectElem : public Element<std::string> {
    public:
        ConnectElem() = default;
        std::string type() const noexcept { return "`"; }      // connect
    };
    // binary op
    class OrConElem : public ConnectElem {
    public:
        OrConElem() = default;
        // Override
        std::string type() const noexcept override { return "|"; }     // or
    };
    // unary op
    class ClosureElem : public Element<std::string> {
    public:
        ClosureElem() = default;
        std::string type() const noexcept { return "*"; }    // closure
    };
    // unary op
    class PlusElem : public ClosureElem {
    public:
        PlusElem() = default;
        // Override
        std::string type() const noexcept override { return "+"; }    // positive closure
    };
    // unary op
    class NonGreedyElem : public Element<std::string> {
    public:
        NonGreedyElem() = default;
        std::string type() const noexcept { return "?"; }     // non-greedy
    };

    // Factory
    std::shared_ptr<Element<std::string>> CreateElem(char c, bool is_char = false);
    std::shared_ptr<NodeElem> CreateNode(char, std::string&&, int id = -1);

    // Character table
    extern std::string char_table;
    // State counter
    extern std::unordered_map<GloData::LexType, int> state_count;

    bool GetCharTable(const char*);
};

#endif