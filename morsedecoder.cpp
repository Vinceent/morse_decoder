#include "morsedecoder.h"
#include<algorithm>
MorseDecoder::MorseDecoder(const std::set<std::string>& t, const std::string& s)
{
    m_table = t;
    m_str = s;
    code_limit = std::max_element(t.cbegin(), t.cend(), [](const std::string& f, const std::string& s){return f.size()<s.size();})->size() +1;
    valid_steps.resize(s.size()+1);
}
std::vector<std::string>& MorseDecoder::result()
{
    decode();
    return valid_steps[0];
}

void MorseDecoder::decode(size_t starting_pos )
{
    auto it = m_str.begin() +starting_pos;
    for(size_t offset = 1; (offset!=code_limit)&&(m_str.end() - it >= offset); ++offset) {  //offset reprezents the size of the symbol we look for
        std::string letter(it, it+offset);
        if(m_table.find(letter)!=m_table.end()) {
            if(it+offset == m_str.end()) {
                valid_steps[starting_pos].push_back(letter);
                break;
            }
            if(valid_steps[starting_pos+offset].empty()) {      //if we have not computed the symbols starting from the end of letter
                decode(starting_pos+offset);
            }
            for(auto& precalc_str: valid_steps[starting_pos+offset]) {
                valid_steps[starting_pos].push_back(letter +' '+ precalc_str);
            }
        }
    }
}
