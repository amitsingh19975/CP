#include<iostream>
#include<unordered_map>
#include<vector>
#include<unordered_set>
#include<set>
#include<list>
#include<array>
#include<map>
#include<memory>
#include<tuple>
#include<cmath>
#include<algorithm>
#include<numeric>
#include<utility>
#include<cstring>
#include<cstdarg>
#include<queue>
#include<stack>
#include<sstream>

#define rep4(VAR,BEGIN,END,STEP) for( auto VAR = ll(BEGIN); VAR < ll(END); VAR += ll(STEP) )
#define rep3(VAR,BEGIN,END) for( auto VAR = ll(BEGIN); VAR < ll(END); VAR += ll(1) )
#define rep2(VAR,END) for( auto VAR = 0ll; VAR < ll(END); VAR += ll(1) )

#define repr4(VAR,END,BEGIN,STEP) for( auto VAR = ll(END) - 1; VAR >= ll(BEGIN); VAR -= ll(STEP) )
#define repr3(VAR,END,BEGIN) for( auto VAR = ll(END) - 1; VAR >= ll(BEGIN); VAR -= ll(1) )
#define repr2(VAR,END) for( auto VAR = ll(END) - 1; VAR >= 0; VAR -= ll(1) )

#define GET_REP( _0, _1, _2, _3,  NAME,...) NAME
#define rep(...) GET_REP( __VA_ARGS__, rep4, rep3, rep2 ) (__VA_ARGS__)
#define repr(...) GET_REP( __VA_ARGS__, repr4, repr3, repr2 ) (__VA_ARGS__)
#define crep(VAR,CONTAINER) for( auto const& VAR : CONTAINER )  

#define fst(p) std::get<0>(p)
#define snd(p) std::get<1>(p)
#define trd(p) std::get<2>(p)

#define rng(CONTAINER) CONTAINER.begin(), CONTAINER.end()
#define init() std::ios_base::sync_with_stdio(false); std::cin.tie(NULL)
#define clear_buff() std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')

#define mp std::make_pair
#define pb push_back
#define def(TYPE, ...) TYPE __VA_ARGS__; get_input(__VA_ARGS__)

[[maybe_unused]] static auto sep = '\n';

template<typename... Args>
void prt_impl(Args&&... args){
	if constexpr(sizeof...(args) == 0){
		std::cout<<'\n';
	} else {
		((std::cout<<args, (sep == '\0' ? std::cout : std::cout << sep)), ...);
	}
}

template<typename... Args>
void get_input(Args&... args){
	((std::cin>>args), ...);
}

#define prt(...) prt_impl(__VA_ARGS__);

using ll = long long;
using vi = std::vector<ll>;
using vvi = std::vector<vi>;
using ii = std::pair<ll,ll>;
using vii = std::vector<ii>;
using vvii = std::vector<vii>;

template<typename U, typename V>
auto amt_max(U const& lhs, V const& rhs){
    return std::max( lhs, static_cast<U>(rhs) );
}

template<typename U, typename V>
auto amt_min(U const& lhs, V const& rhs){
    return std::min( lhs, static_cast<U>(rhs) );
}

template<typename T, typename U, typename... Args>
auto amt_max(T const& f, U const& s, Args&&... args){
	return amt_max(f, amt_max(s, std::forward<Args>(args)...));
}

template<typename T, typename U, typename... Args>
auto amt_min(T const& f, U const& s, Args&&... args){
	return amt_min(f, amt_min(s, std::forward<Args>(args)...));
}

[[maybe_unused]] static constexpr auto pinf = std::numeric_limits<ll>::max();
[[maybe_unused]] static constexpr auto ninf = std::numeric_limits<ll>::min();

#ifdef DEBUG
	void init_in_out_stream(){
		#ifdef FILE_IN
			freopen(FILE_IN,"r",stdin);
		#endif
		// #ifdef FILE_OUT
		// 	freopen(FILE_OUT,"w",stdout);
		// #endif
	}
#else
	void init_in_out_stream(){}
#endif

#ifdef DEBUG
	#include <debug.hpp>
#else
	#define debug_s(OS, ...)
	#define debug(...)
	#define matrix_w(V, R, C)
	#define col_vec_w(V, C)
	#define row_vec_w(V, R)
	#define assert(COND, MSG)
	#define assert_cmp(CMP, LEFT, RIGHT, MSG)
	#define assert_eq(LEFT, RIGHT, MSG)
	#define assert_neq(LEFT, RIGHT, MSG)
	#define assert_lt(LEFT, RIGHT, MSG)
	#define assert_lte(LEFT, RIGHT, MSG)
	#define assert_gt(LEFT, RIGHT, MSG)
	#define assert_gte(LEFT, RIGHT, MSG)
#endif

using namespace std;

struct Game {
    ll red{};
    ll green{};
    ll blue{};

    friend ostream& operator<<(ostream& os, const Game& game) {
        os << "red: " << game.red << " green: " << game.green << " blue: " << game.blue;
        return os;
    }
};

vector<Game> parse_game(string line) {
    vector<Game> games;
    auto colon_pos = line.find(':');
    auto g_set = line.substr(colon_pos + 2);
    stringstream sets(g_set);
    g_set.clear();
    string set, color;

    while (getline(sets, g_set, ';')) {
        stringstream ss(g_set);
        Game game;
        while (getline(ss, set, ',')) {
            stringstream item(set);
            ll val;
            item >> val;
            item >> color;
            switch (color[0]) {
                case 'r':
                    game.red = val;
                    break;
                case 'g':
                    game.green = val;
                    break;
                case 'b':
                    game.blue = val;
                    break;
            }
        }
        games.push_back(game);
    }
    return games;
}

ll solve_part_1(){
	vector<vector<Game>> games;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) break;
        auto game = parse_game(std::move(line));
        games.push_back(std::move(game));
    }
    Game g_cond{ .red = 12, .green = 13, .blue = 14 };
    
    ll max_score = 0;
    rep(i, games.size()) {
        auto const& game = games[i];
        bool is_valid = true;
        crep(g, game) {
            if (g.red > g_cond.red || g.green > g_cond.green || g.blue > g_cond.blue) {
                is_valid = false;
                break;
            }
        }
        if (is_valid) {
            max_score += i + 1;
        }
    }
	return max_score;
}

ll solve_part_2(){
	vector<vector<Game>> games;
    string line;
    while (getline(cin, line)) {
        if (line.empty()) break;
        auto game = parse_game(std::move(line));
        games.push_back(std::move(game));
    }
    
    ll max_score = 0;
    rep(i, games.size()) {
        auto const& game = games[i];

        auto min_cubes = Game{
            .red = 0,
            .green = 0,
            .blue = 0
        };

        crep(g, game) {
            min_cubes.red = amt_max(min_cubes.red, g.red);
            min_cubes.green = amt_max(min_cubes.green, g.green);
            min_cubes.blue = amt_max(min_cubes.blue, g.blue);
        }

        auto m = (min_cubes.red * min_cubes.green * min_cubes.blue);
        max_score += m;
    }
	return max_score;
}

int main() {
	init();
	
	init_in_out_stream();

	prt(solve_part_2());
	return 0;
}

