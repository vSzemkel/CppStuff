
#include <string>
#include <fcntl.h>
#include <iostream>
#include <algorithm>

int main()
{
    // edit with windows1250
    //std::wstring polish_sentence = L"za¿ó³æ gêœl¹ jaŸñ";
    std::string polish_sentence = "za\xc5\xbc\xc3\xb3\xc5\x82\xc4\x87 g\xc4\x99\xc5\x9bl\xc4\x85 ja\xc5\xba\xc5\x84";

    std::cout << polish_sentence << "\n";

    std::string upper{polish_sentence};
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [](auto ch) { return toupper(ch); });
    std::cout << upper << "\n";

    auto old_loc = setlocale(LC_CTYPE, "pl_PL.UTF-8");
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [](auto ch) { return toupper(ch); });
    std::cout << upper << "\n";
    setlocale(LC_CTYPE, old_loc);

    auto polish_locale = newlocale(LC_CTYPE, "pl_PL.UTF-8", nullptr);
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [polish_locale](auto ch) { return toupper_l(ch, polish_locale); });
    std::cout << upper << "\n";
    freelocale(polish_locale);
}
