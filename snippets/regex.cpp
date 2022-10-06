    static const auto scalarMinRegex = std::regex{R"([^\w]__fun__([\w]+))"};
    static constinit const char minExpression[] = "------- __fun__765++++__fun__baba+++";
    for (auto iter = std::cregex_iterator(minExpression, minExpression + sizeof(minExpression), scalarMinRegex);
         iter != std::cregex_iterator{}; ++iter) {
            std::cout << iter->str(1) << '\n'; 
    }

    std::regex rgx{"(a*)(b+)?(c*)"};
    std::cmatch match;
    assert(std::regex_match("aaaccc", match, rgx));
    assert(match.size() == 4);
    assert(match[2].matched == false && match.str(2).empty());
    assert(match[3].matched && match.str(3) == "ccc");
    assert(match.str(2022).empty());