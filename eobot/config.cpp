#include "config.hpp"

#include <fstream>
#include <cstdio>
#include <string.h>

Config::Config()
{

}

Config::Config(std::string filename)
{
    this->Load(filename);
}

bool Config::Load(std::string filename)
{
    std::ifstream file(filename);
    if(!file.is_open())
    {
        //printf("Config: Could not load file '%s'", filename.c_str());
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while(std::getline(file, line))
    {
        lines.push_back(line);
    }
    file.close();

    for(unsigned int i = 0; i < lines.size(); ++i)
    {
        line = lines[i];

        std::size_t equation_pos = line.find_first_of('=');
        if(equation_pos == std::string::npos)
        {
            printf("Config: syntax error: Could not find a \'=\'. (Could be a section header)");
            continue;
        }

        std::string key = line.substr(0, equation_pos);

        std::string value = line.substr(equation_pos + 1, line.size()/* maybe - 1 */);
        //printf("Config: %s-%s\n", key.c_str(), value.c_str());
        this->entries.push_back(Entry(key, value));
    }

    return true;
}

void Config::Save(std::string filename)
{
    return; //Don't execute - Needs to be updated to correct INI format
    std::ofstream file(filename, std::ios::trunc);
    if(!file.is_open())
    {
        printf("Config: Could not open file");
        return;
    }
    else if(!file.good())
    {
        printf("Data stream error");
        return;
    }

    for(unsigned int i = 0; i < this->entries.size(); ++i)
    {
        std::string data = '[' + this->entries[i].key + '=' + this->entries[i].value + ']' + '\n';
        //const char *cdata = data.c_str();
        //file.write(cdata, (unsigned)strlen(cdata));

    }
    file.close();
}

Config::Entry Config::GetEntry(std::string key)
{
    for(unsigned int i = 0; i < this->entries.size(); ++i)
    {
        if(this->entries[i].key == key)
        {
            return this->entries[i];
        }
    }

    return Entry("", "");
}

std::string Config::GetValue(std::string key)
{
    Entry entry = this->GetEntry(key);

    return entry.value;
}
