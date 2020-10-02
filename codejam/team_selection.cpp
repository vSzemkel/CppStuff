
/*****************************************************************************/
/* Problem: As the football coach at your local school, you have been tasked */
/* with picking a team of exactly P students to represent your school. There */
/* are N students for you to pick from. The i-th student has a skill rating  */
/* Si, which is a positive integer indicating how skilled they are. You have */
/* decided that a team is fair if it has exactly P students on it and they   */
/* all have the same skill rating. That way, everyone plays as a team.       */
/* Initially, it might not be possible to pick a fair team, so you will give */
/* some of the students one-on-one coaching. It takes one hour of coaching   */
/* to increase the skill rating of any student by 1.                         */
/*                                                                           */
/* Solution by: vSzemkel, March 2020                                         */

// use pre-compiled headers
#include "pch.h"

// constant problem parameters
constexpr uint32_t students_count = 1000;
constexpr uint32_t team_size = 30;
constexpr uint8_t lowest_skill_level = 18;
constexpr uint8_t greatest_skill_level = 75;

// custom types
using groups_t = std::map<uint8_t, uint32_t>;
using groups_ci = groups_t::const_iterator;

// global variables
std::array<uint8_t,students_count> skills;
uint32_t team_level{0};
groups_t groups;

void generate_skills()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<> dist(lowest_skill_level, greatest_skill_level);

    std::generate(skills.begin(), skills.end(), [&]() { return dist(rng); });
}

void generate_groups()
{
    for (const auto& s : skills) {
        auto res = groups.try_emplace(s, 1);
        if (!res.second) res.first->second++;
    }
}

uint32_t compute_window_score(groups_ci low, groups_ci hi)
{
    --hi;
    if (hi->second >= team_size) return 0;

    const uint32_t window_skill_level = hi->first;
    uint32_t required_training{0};
    uint32_t required_players = team_size - hi->second;
    while (required_players > 0) {
        --hi;
        uint32_t players_taken;
        if (hi->second > required_players) {
            players_taken = required_players;
            required_players = 0;
        } else {
            players_taken = hi->second;
            required_players -= players_taken;
        }
        required_training += players_taken * (window_skill_level - hi->first);
    }

    return required_training;
}

uint32_t compute_training()
{
    static_assert(team_size <= students_count, "Too few students");
    // set initial window
    uint32_t window_size{0};
    auto low = groups.begin();
    auto hi = low;
    while (window_size < team_size) {
        window_size += hi->second;
        ++hi;
    }

    team_level = std::prev(hi)->first;
    uint32_t best_score = compute_window_score(low, hi);
    // slide the window
    while (hi != groups.end()) {
        window_size += hi->second;
        ++hi;
        while (window_size - low->second >= team_size) {
            window_size -= low->second;
            ++low;
        }
        auto window_score = compute_window_score(low, hi);
        if (window_score < best_score) {
            best_score = window_score;
            team_level = std::prev(hi)->first;
        }
        if (best_score == 0) break;
    }

    return best_score;
}

int main()
{
    generate_skills();
    for (const auto& s : skills)
        std::cout << (int)s << ", ";

    generate_groups();
    std::cout << "\n\n";
    for (const auto& g : groups)
        std::cout << g.second << " students on skill level " << (int)g.first << '\n';

    std::cout << "\n\nYou need " << compute_training() << " hours to train a team with " << team_level << "\n";
}
