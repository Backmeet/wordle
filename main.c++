#include <iostream>
#include <optional>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <array>
#define colour(text, r, g, b) ("\x1b[38;2;" #r ";" #g ";" #b "m" + std::string(text) + "\x1b[0m")
#define colourBg(text, r, g, b) ("\x1b[48;2;" #r ";" #g ";" #b "m" + std::string(1, text) + "\x1b[0m")
#define noEndColour(r, g, b) ("\x1b[38;2;" #r ";" #g ";" #b "m")
#define noEndColourBg(r, g, b) ("\x1b[48;2;" #r ";" #g ";" #b "m")
#define bgColour "\x1b[48;2;30;50;80m"


std::vector<int> inStringAts(char find, const std::string& s) {
    std::vector<int> indices;
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == find) {
            indices.push_back(i);
        }
    }
    return indices;
}

std::string genDelta(std::string guess, std::string target) {
    std::string out;

    for (int i = 0; i != guess.length(); i++) {
        char letter = guess[i];
        std::vector<int> foundAts = inStringAts(letter, target);
        bool existsInTarget = !foundAts.empty();
        bool correctPos = false;

        for (int idx : foundAts) {
            if (idx == i) {
                correctPos = true;
                break;
            }
        }

        if (correctPos) {
            out += colourBg(letter, 0, 255, 0);     // green
        } else if (existsInTarget) {
            out += colourBg(letter, 0, 0, 255);     // blue
        } else {
            out += colourBg(letter, 255, 0, 0);     // red
        }
    }

    return out + bgColour;
}

inline void removeChars(int number) {
    for (int i = 0; i < number; ++i)
        std::cout << '\b' << ' ' << '\b';
}

inline void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void sizedInput(std::istream& in, std::string& out, size_t neededSize) {
    out.clear();
    char c;
    std::string temp;

    while (true) {
        if (!in.get(c)) break; // EOF

        if (c == '\n') {
            if (out.size() == neededSize) {
                return;
            }

            temp = "You need to input exactly " + std::to_string(neededSize) + " number of chars!";
            std::cout << temp;
            sleep(1000);

            // remove error message
            removeChars(temp.length());
            std::cout << "\033[A" << "\033[" << std::to_string(out.size()) << "C";
            for (int i = 0; i != out.size(); i++) {
                std::cout << "\b \b";
            }
            
            out.clear();
            continue;
        }

        out.push_back(c);

    }
    return;
}

void wordle(std::string info, std::string target) {
    std::cout << info;
    std::cout << "The length of the target word is " << target.length() << "\n";
    std::string ask;
    sizedInput(std::cin, ask, target.length());
    std::cout << genDelta(ask, target);
    if (ask == target) {std::cout << "\nYou got it on the first try! good job!";}
    std::cout << "\n";
    for (int trys = 1; ask != target; trys++) {
        sizedInput(std::cin, ask, target.length());
        std::cout << genDelta(ask, target);
        if (ask == target) {std::cout << "\nYou got it! It only took you " << std::to_string(trys) << " trys";}
        std::cout << "\n";
    }
}
/*
you are : let the games begin    [0]
so dum! : wait what? -you prb    [1]
jk so   : HA get fooled -me      [2]
aries   : who dat? -you          [3]
femboy  : theirs more?           [4]
akana   : what was that last one [5]
siri    : what MORE?             [6]
ship    : ok what now?           [7]
*/

const static std::array<std::array<std::string, 2>, 8> infoAndWords = {{
    {"you are", "let the games begin"},
    {"so dum!", "wait what? -you prb (hint !@#$%^&*()\\/+-=`~ are all allowed!"},
    {"jk so", "Ha get fooled -me"},
    {"aries", "who dat? -you"},
    {"femboy", "theirs more?"},
    {"akana", "what was that last one"},
    {"siri", "what MORE?"},
    {"ship", "ok what now"}
}};

int main() {
    std::cout << bgColour;
    for (std::array<std::string, 2> infoandword : infoAndWords) {
        wordle(infoandword[1] + "\n", infoandword[0]);
    }
    std::cout << noEndColourBg(0, 0, 0);
}
