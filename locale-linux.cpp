
#include <string>
#include <iostream>
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
    freelocale(polish_locale);
}
