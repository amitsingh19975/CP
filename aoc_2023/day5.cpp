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

struct Range {
    ll start{};
    ll end{};
    constexpr Range() = default;
    constexpr Range(ll start, ll len = 1)
        : start(start)
        , end(start + len)
    {}

    constexpr bool contains(ll value, ll def) const {
        auto v = value < 0 ? def : value;
        return v >= start && v < end;
    }

    constexpr bool contains(Range const& range) const {
        return range.start >= start && range.end <= end;
    }

    constexpr Range intersect(Range const& range) const {
        auto temp_start = amt_max(range.start, this->start);
        auto temp_end = amt_min(range.end, this->end);
        return Range(temp_start, temp_end - temp_start);
    }

    friend std::ostream& operator<<(std::ostream& is, Range const& range) {
        is << "Range{start: " << range.start << ", end: " << range.end << "}";
        return is;
    }

    constexpr auto operator<(ll const& rhs) const noexcept {
        return start < rhs;
    }
    
    constexpr auto operator<(Range const& rhs) const noexcept {
        return start < rhs.start;
    }

    constexpr auto size() const noexcept {
        return end - start;
    }

    constexpr auto is_valid() const noexcept {
        if (start == pinf || size() == 0) return false;
        return start >= 0 && end >= 0 && end > start;
    }
};

template <typename T>
struct Seed {
    T id{};
    T soil{-1};
    T fertilizer{-1};
    T water{-1};
    T light{-1};
    T temperature{-1};
    T humidity{-1};
    T location{-1};

    friend std::ostream& operator<<(std::ostream& is, Seed const& seed) {
        is << "Seed{id: " << seed.id << ", soil: " << seed.soil << ", fertilizer: " << seed.fertilizer
           << ", water: " << seed.water << ", light: " << seed.light << ", temperature: " << seed.temperature
           << ", humidity: " << seed.humidity << ", location: " << seed.location << "}";
        return is;
    }

    constexpr auto fix_soil() noexcept {
        soil = soil < 0 ? id : soil;
    }

    constexpr auto fix_fertilizer() noexcept {
        fertilizer = fertilizer < 0 ? soil : fertilizer;
    }

    constexpr auto fix_water() noexcept {
        water = water < 0 ? fertilizer : water;
    }

    constexpr auto fix_light() noexcept {
        light = light < 0 ? water : light;
    }

    constexpr auto fix_temperature() noexcept {
        temperature = temperature < 0 ? light : temperature;
    }

    constexpr auto fix_humidity() noexcept {
        humidity = humidity < 0 ? temperature : humidity;
    }

    constexpr auto fix_location() noexcept {
        location = location < 0 ? humidity : location;
    }
};

template<typename Fn>
auto parse_range(Fn&& fn) noexcept {
    string line;
    while (getline(cin, line)) {
        if (line.empty()) {
            break;
        }
        stringstream ss(line);
        ll ds, srs, len;
        ss >> ds >> srs >> len;
        auto dest_range = Range(ds, len);
        auto src_range = Range(srs, len);
        std::invoke(fn, dest_range, src_range);
    }
}

ll solve_part_1(){
    using seed_t = Seed<ll>;
	vector<seed_t> seeds;

    string line;
    while(getline(cin, line)) {
        if (line.empty()) {
            continue;
        }

        auto prefix_pos = line.find(':');
        auto prefix = line.substr(0, prefix_pos);
        auto suffix = line.substr(prefix_pos + 1);

        if (prefix == "seeds") {
            stringstream ss(suffix);
            while (ss) {
                if (!isdigit(ss.peek())) {
                    ss.ignore();
                    continue;
                }
                seed_t seed;
                ss >> seed.id;
                seeds.push_back(seed);
            }
        } else if (prefix == "seed-to-soil map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.id, 0)) {
                        seed.soil = dst.start + (seed.id - src.start);
                    }
                }
            });
            std::for_each(rng(seeds), [](auto& seed) { seed.fix_soil(); });
        } else if (prefix == "soil-to-fertilizer map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.soil, seed.id)) {
                        seed.fertilizer = dst.start + (seed.soil - src.start);
                    }
                }
            });
            for_each(rng(seeds), [](auto& seed) { seed.fix_fertilizer(); });
        } else if (prefix == "fertilizer-to-water map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.fertilizer, seed.soil)) {
                        seed.water = dst.start + (seed.fertilizer - src.start);
                    }
                }
            });
            for_each(rng(seeds), [](auto& seed) { seed.fix_water(); });
        } else if (prefix == "water-to-light map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.water, seed.fertilizer)) {
                        seed.light = dst.start + (seed.water - src.start);
                    }
                }
            });
            for_each(rng(seeds), [](auto& seed) { seed.fix_light(); });
        } else if (prefix == "light-to-temperature map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.light, seed.water)) {
                        seed.temperature = dst.start + (seed.light - src.start);
                    }
                }
            });
            for_each(rng(seeds), [](auto& seed) { seed.fix_temperature(); });
        } else if (prefix == "temperature-to-humidity map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.temperature, seed.light)) {
                        seed.humidity = dst.start + (seed.temperature - src.start);
                    }
                }
            });
            for_each(rng(seeds), [](auto& seed) { seed.fix_humidity(); });
        } else if (prefix == "humidity-to-location map") {
            parse_range([&seeds](auto dst, auto src) {
                for (auto& seed : seeds) {
                    if (src.contains(seed.humidity, seed.temperature)) {
                        seed.location = dst.start + (seed.humidity - src.start);
                    }
                }
            });
            for_each(rng(seeds), [](auto& seed) { seed.fix_location(); });
        }
    }

	return std::min_element(rng(seeds), [](auto const& lhs, auto const& rhs) {
        return lhs.location < rhs.location;
    })->location;
}

ii find_min_range(vector<pair<Range, Range>> const& ranges, Range const& range) {
    auto it = std::find_if(rng(ranges), [&range](auto const& r) {
        return fst(r).intersect(range).is_valid();
    });

    if (it == ranges.end()) {
        return mp(-1, -1);
    }


    auto [dst, src] = *it;

    auto intersect = dst.intersect(range);
    debug(range, dst, src, intersect, intersect.is_valid(), (it == ranges.end()));
    
    if (intersect.start > dst.start) {
        intersect.start = dst.start;
    }

    if (intersect.end < dst.end) {
        intersect.end = dst.end;
    }

    auto start = intersect.start;
    auto len = intersect.size();
    return mp(start, len);
}

ll solve_part_2(){
    using seed_t = Seed<Range>;
	vector<seed_t> seeds;

    string line;

    ll min = pinf;
    vector<pair<Range, Range>> seed_to_soil_ranges;
    vector<pair<Range, Range>> soil_to_fertilizer_ranges;
    vector<pair<Range, Range>> fertilizer_to_water_ranges;
    vector<pair<Range, Range>> water_to_light_ranges;
    vector<pair<Range, Range>> light_to_temperature_ranges;
    vector<pair<Range, Range>> temperature_to_humidity_ranges;
    vector<pair<Range, Range>> humidity_to_location_ranges;

    while(getline(cin, line)) {
        if (line.empty()) {
            continue;
        }

        auto prefix_pos = line.find(':');
        auto prefix = line.substr(0, prefix_pos);
        auto suffix = line.substr(prefix_pos + 1);

        if (prefix == "seeds") {
            stringstream ss(suffix);
            while (ss) {
                if (!isdigit(ss.peek())) {
                    ss.ignore();
                    continue;
                }
                ll start, len;
                ss >> start >> len;
                seed_t seed;
                seed.id = Range(start, len);
                seeds.push_back(seed);
            }
        } else if (prefix == "seed-to-soil map") {
            parse_range([&seed_to_soil_ranges](auto dst, auto src) {
                seed_to_soil_ranges.push_back({dst, src});
            });

        } else if (prefix == "soil-to-fertilizer map") {
            parse_range([&soil_to_fertilizer_ranges](auto dst, auto src) {
                soil_to_fertilizer_ranges.push_back({dst, src});
            });
        } else if (prefix == "fertilizer-to-water map") {
            parse_range([&fertilizer_to_water_ranges](auto dst, auto src) {
                fertilizer_to_water_ranges.push_back({dst, src});
            });
        } else if (prefix == "water-to-light map") {
            parse_range([&water_to_light_ranges](auto dst, auto src) {
                water_to_light_ranges.push_back({dst, src});
            });
        } else if (prefix == "light-to-temperature map") {
            parse_range([&light_to_temperature_ranges](auto dst, auto src) {
                light_to_temperature_ranges.push_back({dst, src});
            });
        } else if (prefix == "temperature-to-humidity map") {
            parse_range([&temperature_to_humidity_ranges](auto dst, auto src) {
                temperature_to_humidity_ranges.push_back({dst, src});
            });
        } else if (prefix == "humidity-to-location map") {
            parse_range([&humidity_to_location_ranges](auto dst, auto src) {
                humidity_to_location_ranges.push_back({dst, src});
            });
        }
    }

    sort(rng(humidity_to_location_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    sort(rng(temperature_to_humidity_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    sort(rng(light_to_temperature_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    sort(rng(water_to_light_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    sort(rng(fertilizer_to_water_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    sort(rng(soil_to_fertilizer_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    sort(rng(seed_to_soil_ranges), [](auto const& lhs, auto const& rhs) {
        return lhs.first < rhs.first;
    });

    auto min_location = humidity_to_location_ranges[0];

    array ranges = {
        temperature_to_humidity_ranges,
        light_to_temperature_ranges,
        water_to_light_ranges,
        fertilizer_to_water_ranges,
        soil_to_fertilizer_ranges,
        seed_to_soil_ranges
    };
    
    for (auto const& range: ranges) {
        auto [start, len] = find_min_range(range, min_location.first);
        auto first_el = range[0];
        
    }
    
    debug(min_location);

	return min;
}

int main() {
	init();
	
	init_in_out_stream();

	prt(solve_part_2());
	return 0;
}

