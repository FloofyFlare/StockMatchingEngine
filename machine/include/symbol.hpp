#pragma once
#include <cstdint>
#include <string_view>
#include <unordered_map>

using SymbolId = uint64_t;
static constexpr SymbolId INVALID_SYMBOL_ID = UINT64_MAX;

struct StringHash {
    using is_transparent = void;
    size_t operator()(std::string_view sv) const {
        return std::hash<std::string_view>{}(sv);
    }
};

struct StringEqual {
    using is_transparent = void;
    bool operator()(std::string_view a, std::string_view b) const {
        return a == b;
    }
};

class SymbolTable {
private:
    std::unordered_map<std::string, SymbolId, StringHash, StringEqual> symbol_map;
    SymbolId next_id = 0;

    
public:
    SymbolId getOrCreateId(std::string_view name){
        auto it = symbol_map.find(name);
        if (it != symbol_map.end()) return it->second;
        SymbolId id = next_id++;
        symbol_map.emplace(name, id);
        return id;
    }
    SymbolId lookup(std::string_view name) const {
        auto it = symbol_map.find(name);
        if (it != symbol_map.end()) return it->second;
        return INVALID_SYMBOL_ID;
    }
};