
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

int main()
{
    // edit with UTF-8
    // check-language-support -a
    // sudo apt-get -y install language-pack-pl
    std::string polish_sentence = "zażółć gęślą jaźń";
    std::cout << polish_sentence << "\n";

    std::string upper{polish_sentence};
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [](auto ch) { return toupper(ch); });
    std::cout << upper << "\n";

    auto old_loc = setlocale(LC_CTYPE, nullptr);
    if (setlocale(LC_CTYPE, "pl_PL.UTF-8") != nullptr) {
        std::cout << setlocale(LC_CTYPE, nullptr) << "\n";
        std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [](auto ch) { return toupper(ch); });
        std::cout << upper << "\n";
        setlocale(LC_CTYPE, old_loc);
    }

    auto polish_locale = newlocale(LC_CTYPE, "pl_PL.UTF-8", nullptr);
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [&polish_locale](auto ch) { return toupper_l(ch, polish_locale); });
    std::cout << upper << "\n";

    int ch, ch2, count = 0;
    std::stringstream ss{polish_sentence};
    while (true) {
        ss.read((char*)&ch, 1);
        if (!ss.good())
            break;
        if ((ch & 0xf0) == 0xc0) {
            ss.read((char*)&ch2, 1);
            ch += ch2 * 0x100;
            printf("-%x-", ch);
            ch = std::use_facet<std::ctype<wchar_t>>(std::locale("pl_PL.UTF-8")).toupper(ch);
            printf(">%x-", ch);
            std::cout << (char)ch;
        } else
            std::cout << std::use_facet<std::ctype<char>>(std::locale("pl_PL.UTF-8")).toupper(ch);
        ++count;
    }

    std::cout << "\nstd::size(): " << polish_sentence.size() << ", characters count: " << count << "\n";
    freelocale(polish_locale);
}
