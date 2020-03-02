#ifndef MORSEDECODER_H
#define MORSEDECODER_H
#include<set>
#include<string>
#include<vector>

class MorseDecoder
{
public:
    MorseDecoder(const std::set<std::string>& t, const std::string& s);
    std::vector<std::string>& result();
private:
    void decode(size_t starting_pos = 0);

    std::set<std::string> m_table;
    std::string m_str;
    std::vector<std::vector<std::string>> valid_steps;
    size_t code_limit;
};

#endif // MORSEDECODER_H
