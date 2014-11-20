#include "preprocessor.h"

#include "tokeniser.h"

namespace moss
{
    Preprocessor::Preprocessor()
    {
    }

    void Preprocessor::process_stream(std::istream &ss, std::ostream &os)
    {
        Tokeniser tokens(ss);

        while (tokens.has_tokens())
        {
            auto line = tokens.next_token_line();

            bool first = true;
            for (auto iter = line.begin(); iter != line.end(); ++iter)
            {
                if (first && (*iter)[0] == '#')
                {
                    process_preprocess_line(line);
                }
                else
                {
                    process_normal_line(line);
                }
            }
        }
    }

    void Preprocessor::process_preprocessor_line(const std::vector<std::string> &line)
    {
        if (line[0] == "#define")
        {
            if (line.size() < 2)
            {
                add_symbol(line[1], std::string(""));
            }
            else
            {
                add_symbol(line[1], line[2]);
            }
        }
    }

    void Preprocessor::process_normal_line(const std::vector<std::string> &line)
    {

    }
}
