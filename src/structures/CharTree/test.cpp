#include "CharTree.h"

template <typename T>
void check(T result, T expected) {
    if (result != expected) {
        std::cout << "WA:\n\tres:\t" << result << "\n\texp:\t" << expected << "\n";
    } else {
        printf("OK\n");
    }
}

int main() {
    CharTree tree;
    check<bool>(tree.check_key  ("h"),      false);
    check<bool>(tree.check_key  ("hello"),  false);
    check<bool>(tree.check_key  ("world"),  false);
    check<bool>(tree.check_key  ("hel"),    false);
    check<bool>(tree.check_key  ("hellow"), false);
    check<bool>(tree.check_key  ("hello"),  false);
    check<bool>(tree.add_key    ("hello"),  true);
    check<bool>(tree.add_key    ("hello"),  false);
    check<bool>(tree.check_key  ("hell"),   false);
    check<bool>(tree.check_key  ("helloa"), false);
    check<bool>(tree.check_key  (""),       false);
    check<bool>(tree.add_key    (""),       false);
    check<bool>(tree.check_key  ("h"),      false);
    check<bool>(tree.add_key    ("h"),      true);
    check<bool>(tree.check_key  ("h"),      true);
    check<bool>(tree.add_key    ("helloa"), true);
    check<bool>(tree.add_key    ("world"),  true);
    check<bool>(tree.check_key  ("helloa"), true);
    check<bool>(tree.check_key  ("world"),  true);
    check<bool>(tree.check_key  ("w"),      false);
    check<bool>(tree.check_key  ("worls"),  false);
    check<bool>(tree.check_key  ("worle"),  false);
    tree.clear();
    check<bool>(tree.check_key  ("h"),      false);
    check<bool>(tree.check_key  ("hello"),  false);
    check<bool>(tree.check_key  ("world"),  false);
    check<bool>(tree.check_key  ("hel"),    false);
    check<bool>(tree.check_key  ("hellow"), false);
    check<bool>(tree.check_key  ("hello"),  false);
    check<bool>(tree.add_key    ("hello"),  true);
    check<bool>(tree.add_key    ("hello"),  false);
    check<bool>(tree.check_key  ("hell"),   false);
    check<bool>(tree.check_key  ("helloa"), false);
    check<bool>(tree.check_key  (""),       false);
    check<bool>(tree.add_key    (""),       false);
    check<bool>(tree.check_key  ("h"),      false);
    check<bool>(tree.add_key    ("h"),      true);
    check<bool>(tree.check_key  ("h"),      true);
    check<bool>(tree.add_key    ("helloa"), true);
    check<bool>(tree.add_key    ("world"),  true);
    check<bool>(tree.check_key  ("helloa"), true);
    check<bool>(tree.check_key  ("world"),  true);
    check<bool>(tree.check_key  ("w"),      false);
    check<bool>(tree.check_key  ("worls"),  false);
    check<bool>(tree.check_key  ("worle"),  false);
}