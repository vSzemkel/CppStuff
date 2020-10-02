
#include <io.h>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <algorithm>

int main()
{
    // edit with windows1250
    std::wstring polish_sentence = L"za¿ó³æ gêœl¹ jaŸñ";

    auto old_mode = _setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << polish_sentence << "\n";

    std::wstring upper{polish_sentence};
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [](auto ch) { return towupper(ch); });
    std::wcout << upper << "\n";

    auto old_loc = setlocale(LC_CTYPE, nullptr);
    if (setlocale(LC_CTYPE, "Polish_Poland.1250") != nullptr) {
        std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [](auto ch) { return towupper(ch); });
        std::wcout << upper << "\n";
        setlocale(LC_CTYPE, old_loc);
    }

    auto polish_locale = _create_locale(LC_CTYPE, "pl_PL");
    std::transform(std::begin(polish_sentence), std::end(polish_sentence), std::begin(upper), [polish_locale](auto ch) { return _towupper_l(ch, polish_locale); });
    std::wcout << upper << "\n";
    _free_locale(polish_locale);

    _setmode(_fileno(stdout), old_mode);
}
