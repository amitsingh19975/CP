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

using iii = tuple<ll, ll, ll>;

iii parse_number(vector<string> const& grid, ll i, ll j) {
    ll n = grid.size();
    ll m = grid[0].size();
    ll res = 0;

    auto old_j = j;
    auto start = j;

    while (i >= 0 && i < n && j >= 0 && j < m) {
        auto const c = grid[i][j];
        if (!isdigit(c)) {
            break;
        }
        j -= 1;
    }

    start = j + 1;

    j = old_j + 1;
    
    while (i >= 0 && i < n && j >= 0 && j < m) {
        auto const c = grid[i][j];
        if (!isdigit(c)) {
            break;
        }
        j += 1;
    }

    auto end = j - 1;

    auto n_start = amt_min(start, end);
    auto n_end = amt_max(start, end);

    rep(k, n_start, n_end + 1) {
        res *= 10;
        res += grid[i][k] - '0';
    }
    return iii(res, n_start, n_end);
}

struct hash_iii {
    size_t operator()(iii const& p) const {
        auto const [a, b, c] = p;
        return a ^ b ^ c;
    }
};

ll count_part_number_part_1(vector<string> const& grid, ll i, ll j) {
    ll n = grid.size();
    ll m = grid[0].size();
    ll res = 0;
    // Four directions including diagonals
    static constexpr std::array dir = { mp(-1, 1), mp(-1, 0), mp(-1, -1), mp(0, -1), mp(1, -1), mp(1, 0), mp(1, 1), mp(0, 1) };
    auto memo = unordered_set<iii, hash_iii>{};

    crep(d, dir) {
        auto const [di, dj] = d;
        ll ci = i + di;
        ll cj = j + dj;
        if (ci >= 0 && ci < n && cj >= 0 && cj < m) {
            auto const c = grid[ci][cj];
            if (isdigit(c)) {
                auto temp = parse_number(grid, ci, cj);
                auto const [num, start, end] = temp;
                auto const key = iii(ci, start, end);
                if (memo.count(key) == 0) {
                    // debug(temp, ci, di, dj, cj, i, j);
                    memo.insert(key);
                    res += num;
                }
            }
        }
    }

    
    return res;
}

ll solve_part_1(){
	vector<string> grid;
    string line;
    while(getline(cin, line)){
        grid.pb(line);
    }
    // debug(grid);
    ll n = grid.size();
    ll m = grid[0].size();
    ll res = 0;
    rep(i, n) {
        rep(j, m) {
            auto const c = grid[i][j];
            if (!isdigit(c) && c != '.') {
                // debug(i, j, c);
                res += count_part_number_part_1(grid, i, j);
            }
        }
    }
	return res;
}

ll count_part_number_part_2(vector<string> const& grid, ll i, ll j) {
    ll n = grid.size();
    ll m = grid[0].size();
    // Four directions including diagonals
    static constexpr std::array dir = { mp(-1, 1), mp(-1, 0), mp(-1, -1), mp(0, -1), mp(1, -1), mp(1, 0), mp(1, 1), mp(0, 1) };
    auto memo = unordered_set<iii, hash_iii>{};
    auto star_mul = 1;
    auto star_count = 0;

    crep(d, dir) {
        auto const [di, dj] = d;
        ll ci = i + di;
        ll cj = j + dj;
        if (ci >= 0 && ci < n && cj >= 0 && cj < m) {
            auto const c = grid[ci][cj];
            if (isdigit(c)) {
                auto temp = parse_number(grid, ci, cj);
                auto const [num, start, end] = temp;
                auto const key = iii(ci, start, end);
                if (memo.count(key) == 0) {
                    // debug(temp, ci, di, dj, cj, i, j);
                    memo.insert(key);
                    star_mul *= num;
                    star_count += 1;
                }
            }
        }
    }

    
    return star_count == 2 ? star_mul : 0;
}

ll solve_part_2(){
	vector<string> grid;
    string line;
    while(getline(cin, line)){
        grid.pb(line);
    }
    // debug(grid);
    ll n = grid.size();
    ll m = grid[0].size();
    ll res = 0;
    rep(i, n) {
        rep(j, m) {
            auto const c = grid[i][j];
            if (c == '*') {
                // debug(i, j, c);
                res += count_part_number_part_2(grid, i, j);
            }
        }
    }
	return res;
}

int main() {
	init();
	
	init_in_out_stream();
	prt(solve_part_2());
	return 0;
}

