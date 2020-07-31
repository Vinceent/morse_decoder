#ifndef MORSEDECODER_H
#define MORSEDECODER_H
#include<set>
#include<string>
#include<QVector>

class MorseDecoder
{
public:
    MorseDecoder(const std::set<std::string>& t, const std::string& s);
    QVector<std::string>& result();
    static const std::set<std::string> default_morse_table;
private:
    void decode(size_t starting_pos = 0);

    std::set<std::string> m_table;
    std::string m_str;
    QVector<QVector<std::string>> valid_steps;
    size_t code_limit;
};


#endif // MORSEDECODER_H
