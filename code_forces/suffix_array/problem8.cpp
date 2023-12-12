// https://codeforces.com/edu/course/2/lesson/2/5/practice/contest/269656/problem/C
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

void radix_sort(vi& p, vi const& cs) {
	auto const n = ll(p.size());
	vi cnt(n);
	rep(i, n) ++cnt[cs[i]];

	vi pos(n);
	pos[0] = 0;
	rep(i, 1, n) pos[i] = pos[i - 1] + cnt[i - 1];

	vi np(n);
	rep(i, n) {
		auto const idx = pos[cs[p[i]]]++;
		np[idx] = p[i];
	}
	p = std::move(np);
}

void build_suffix_array(string const& s, vi& p, vi& cs) {
	auto const n = ll(s.size());
	p.resize(n);
	cs.resize(n);

	{
		vii a(n);
		rep(i, n) a[i] = mp(s[i], i);
		sort(rng(a));
		rep(i, n) p[i] = snd(a[i]);

		cs[p[0]] = 0;
		rep(i, 1, n) cs[p[i]] = cs[p[i - 1]] + (fst(a[i]) != fst(a[i - 1]));
	}

	ll k = 0;
	vi ncs(n);

	while((1 << k) < n) {
		auto const shift = 1 << k;
		rep(i, n) p[i] = (p[i] - shift + n) % n;

		radix_sort(p, cs);

		ncs[p[0]] = 0;
		rep(i, 1, n) {
			auto const cp = mp(cs[p[i]], cs[(p[i] + shift) % n]);
			auto const& pp = mp(cs[p[i - 1]], cs[(p[i - 1] + shift) % n]);
			ncs[p[i]] = ncs[p[i - 1]] + (cp != pp);
		}
		std::swap(cs, ncs);
		++k;
	}
}

ll match_str(string const& s, ll l1, ll r1, ll l2, ll r2) {
	auto const r = amt_min(r1, r2);
	return std::strncmp(s.data() + l1, s.data() + l2, r);
}

ll bs(string const& s, vi const& p, ll l, ll r) {
	ll n = ll(s.size()) - 1;
	ll low = 0;
	ll high = n;
	while (low <= high) {
		auto mid = low + (high - low) / 2;
		auto const res = match_str(s, p[mid], n, l, r - l + 1);
		// debug(l, r, s.substr(p[mid], n - p[mid]), s.substr(l, r - l + 1), res);
		if(res >= 0) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}
	return low;
}

void solve(string const& s, ll n){
	vi p, cs;
	build_suffix_array(s, p, cs);
	// $
	// a$
	// aba$
	// abacaba$
	// acaba$
	// ba$
	// bacaba$
	// caba$
	crep(el, p) {
		cout << s.substr(el) << '\n';
	}

	vector<tuple<ll, ll, ll>> res(n, { -1, -1, -1 });
	rep(i, n) {
		def(ll, l, r);
		auto const pos = bs(s, p, l - 1, r - 1);
		// debug(l, r, pos);
		res[i] = { pos, l, r };
	}
	sort(rng(res));
	sep = '\0';
	crep(el, res) {
		prt(snd(el), ' ', trd(el), '\n');
	}
}

int main() {
	init();
	
	init_in_out_stream();

	def(ll, t);
	while(t--){
		def(string, s);
		s.push_back('$');
		clear_buff();
		def(ll, n);
		solve(s, n);
	}	
	return 0;
}

