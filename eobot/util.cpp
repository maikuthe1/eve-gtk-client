#include "util.hpp"

#include <algorithm>

int path_length(int x1, int y1, int x2, int y2)
{
    int dx = std::abs(x1 - x2);
    int dy = std::abs(y1 - y2);

    return dx + dy;
}

std::vector<std::string> Args(std::string str)
{
    std::vector<std::string> args;
    std::string word;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if((str[i] == ' ' || i == str.length() - 1))
        {
            if(str[i] != ' ')
                word += str[i];
            if(!word.empty())
                args.push_back(word);
            word.clear();
        }
        else if(str[i] != ' ')
        {
            word += str[i];
        }
    }

    return args;
}



std::vector<std::string> explode2(const std::string &str,  std::string &delimiter)
{
    std::vector<std::string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng==0)
        return arr;//no change

    int i=0;
    int k=0;
    while( i<strleng )
    {
        int j=0;
        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
            j++;
        if (j==delleng)//found delimiter
        {
            arr.push_back(  str.substr(k, i-k) );
            i+=delleng;
            k=i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(  str.substr(k, i-k) );
    return arr;
}
std::string Lowercase(std::string str)
{
    std::string ret = str;

    std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);

    return ret;
}