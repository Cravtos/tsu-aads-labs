#include "library.h"

int main()
{
    String str("aaabacbaaaaabaaaababbaaaaaaaab");
    String pat("aaabaaaab");

    for (size_t i = 0; i < pat.len(); i++) {
        std::cout << pat[i] << ' ';
    }

    std::cout << std::endl;

    ssize_t idx_in = str.kmp_search(pat);
    if (idx_in == -1)
    {
        std::cout << "There aren't \"" << pat << "\" in a \"" << str << "\"" << std::endl;
        return -1;
    }
    ssize_t idx_out = idx_in + pat.len();

    String result = str(0, idx_in) + str(idx_out, str.len());
    std::cout << result << std::endl;
}
