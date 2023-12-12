#ifndef __AMT_DEBUG_HPP__
#define __AMT_DEBUG_HPP__

#include <iostream>
#include <tuple>
#include <vector>
#include <type_traits>
#include <iomanip>

namespace amt {
    struct StringView {
        constexpr static unsigned npos = static_cast<unsigned>(-1);
        char const* str;
        unsigned len;

        static StringView from(char const* str) {
            unsigned len{ 0 };
            while (str[len] != '\0') ++len;
            return { str, len };
        }
        
        static StringView from(std::string const& str) {
            return { str.data(), static_cast<unsigned>(str.size()) };
        }

        StringView substr(unsigned start, unsigned end = npos) const {
            return { str + start, end == npos ? len - start : end - start };
        }

        friend std::ostream& operator<<(std::ostream& os, StringView sv) {
            for (unsigned i = 0; i < sv.len; ++i) {
                os << sv.str[i];
            }
            return os;
        }
    };

    namespace internal {
        struct Location {
            StringView file;
            unsigned line;

            friend std::ostream& operator<<(std::ostream& os, Location const& loc) {
                return os << loc.file << ":" << loc.line;
            }

            Location(char const* file = __FILE__, unsigned line = __LINE__)
                : file(StringView::from(file)), line(line)
            {}
        };

        std::ostream& set_escape_code(std::ostream& os, char const* code) {
            return os << "\x1b[" << code << "m";
        }

        std::ostream& reset_escape_code(std::ostream& os) {
            return os << "\x1b[0m";
        }

        void assert_impl(char const* prefix, bool cond, StringView msg, Location loc) {
            if (!cond) {
                set_escape_code(std::cerr, "1;91");
                std::cerr << prefix;
                reset_escape_code(std::cerr);
                std::cerr << msg << " in "<< loc << std::endl;
                std::abort();
            }
        }

        template<typename T>
        struct is_array : std::is_array<T> {};
        
        template<typename T, typename A>
        struct is_array<std::vector<T, A>> : std::true_type {};

        #ifdef _LIBCPP_ARRAY
            template<typename T, std::size_t N>
            struct is_array<std::array<T, N>> : std::true_type {};
        #endif

        template<typename T>
        struct is_map : std::false_type {};

        #ifdef _LIBCPP_MAP
            template<typename K, typename V, typename C, typename A>
            struct is_map<std::map<K, V, C, A>> : std::true_type {};
        #endif

        #ifdef _LIBCPP_UNORDERED_MAP
            template<typename K, typename V, typename C, typename A>
            struct is_map<std::unordered_map<K, V, C, A>> : std::true_type {};
        #endif

        template<typename T>
        struct is_set : std::false_type {};

        #ifdef _LIBCPP_SET
            template<typename K, typename C, typename A>
            struct is_set<std::set<K, C, A>> : std::true_type {};
        #endif

        #ifdef _LIBCPP_UNORDERED_SET
            template<typename K, typename C, typename A>
            struct is_set<std::unordered_set<K, C, A>> : std::true_type {};
        #endif

        template<typename T>
        struct is_string : std::false_type {};

        #ifdef _LIBCPP_STRING
            template<typename C, typename T, typename A>
            struct is_string<std::basic_string<C, T, A>> : std::true_type {};
        #endif

        template<>
        struct is_string<char const*> : std::true_type {};

        template<>
        struct is_string<char*> : std::true_type {};

        template<typename T>
        struct is_pair : std::false_type {};

        template<typename T, typename U>
        struct is_pair<std::pair<T, U>> : std::true_type {};

        template<typename T>
        struct is_tuple : std::false_type {};

        template<typename... Args>
        struct is_tuple<std::tuple<Args...>> : std::true_type {};

        template<typename Fn, typename... Args, std::size_t... Is>
        constexpr void tuple_for_impl(std::tuple<Args...> const& ts, Fn&& fn, std::index_sequence<Is...>) noexcept {
            (fn(std::get<Is>(ts), Is), ...);
        }
    }

    template<typename Fn, typename... Args>
    constexpr void tuple_for(std::tuple<Args...> const& ts, Fn&& fn) noexcept {
        internal::tuple_for_impl(ts, std::forward<Fn>(fn), std::make_index_sequence<sizeof...(Args)>());
    }


    template<typename S>
    void assert(bool cond, S const& msg, char const* cond_str = nullptr, internal::Location loc = {}) {
        if (cond_str == nullptr) {
            internal::assert_impl("Assertion failed: ", cond, StringView::from(msg), loc);
        } else {
            std::string message = std::string("(") + std::string(cond_str) + "): " + std::string(msg);
            internal::assert_impl("Assertion failed: ", cond, StringView::from(message), loc);
        }
    }

    enum class Cmp {
        Eq,
        NotEq,
        Less,
        LessEq,
        Greater,
        GreaterEq,
    };

    std::string to_string(Cmp cmp) {
        switch (cmp) {
            case Cmp::Eq: return "==";
            case Cmp::NotEq: return "!=";
            case Cmp::Less: return "<";
            case Cmp::LessEq: return "<=";
            case Cmp::Greater: return ">";
            case Cmp::GreaterEq: return ">=";
        }
    }

    constexpr Cmp to_opposite(Cmp cmp) noexcept {
        switch (cmp) {
            case Cmp::Eq: return Cmp::NotEq;
            case Cmp::NotEq: return Cmp::Eq;
            case Cmp::Less: return Cmp::GreaterEq;
            case Cmp::LessEq: return Cmp::Greater;
            case Cmp::Greater: return Cmp::LessEq;
            case Cmp::GreaterEq: return Cmp::Less;
        }
    }
    
    template<typename T, typename U, typename S>
    void assert(Cmp cmp, T const& left, U const& right, char const* left_str, char const* right_str, S const& msg, internal::Location loc = {}) {
        auto const sym = to_string(to_opposite(cmp));
        std::string message = std::string("(") + std::string(left_str) + " " + sym + " " + std::string(right_str) + "): " + std::string(msg);
        switch(cmp) {
            case Cmp::Eq: internal::assert_impl("Equality assertion failed: ", (left == right), StringView::from(message), loc); break;
            case Cmp::NotEq: internal::assert_impl("Inequality assertion failed: ", (left != right), StringView::from(message), loc); break;
            case Cmp::Less: internal::assert_impl("Less assertion failed: ", (left < right), StringView::from(message), loc); break;
            case Cmp::LessEq: internal::assert_impl("Less or equal assertion failed: ", (left <= right), StringView::from(message), loc); break;
            case Cmp::Greater: internal::assert_impl("Greater assertion failed: ", (left > right), StringView::from(message), loc); break;
            case Cmp::GreaterEq: internal::assert_impl("Greater or equal assertion failed: ", (left >= right), StringView::from(message), loc); break;
        }
    }

    template<typename T>
    struct MatrixWrapper {
        std::vector<T> const& data;
        unsigned rows;
        unsigned cols;

        MatrixWrapper(std::vector<T> const& data, unsigned rows, unsigned cols = 1u)
            : data(data)
            , rows(rows)
            , cols(cols)
        {
            if constexpr (internal::is_array<std::decay_t<T>>::value) {
                using value_type = std::decay_t<decltype(data[0][0])>;
                static_assert(std::is_arithmetic_v<value_type>, "MatrixWrapper: T is not arithmetic type");
                assert(rows == data.size(), "MatrixWrapper: rows != data.size()", nullptr, internal::Location(__FILE__, __LINE__));
                for (auto const& v : data) {
                    assert(cols == v.size(), "MatrixWrapper: cols != v.size()", nullptr, internal::Location(__FILE__, __LINE__));
                }
            } else {
                using value_type = std::decay_t<decltype(data[0])>;
                static_assert(std::is_arithmetic_v<value_type>, "MatrixWrapper: T is not arithmetic type");
                assert(rows * cols == data.size(), "MatrixWrapper: rows * cols != data.size()", nullptr, internal::Location(__FILE__, __LINE__));
            }
        }
    };

    struct LevelGuard {
        std::size_t level{0};
        std::size_t indent{2};   

        constexpr auto guard() noexcept {
            return level++;
        }

        std::string space() const noexcept {
            return std::string(indent * level, ' ');
        }

        ~LevelGuard() noexcept {
            --level;
        }
    };

    template<typename A>
    std::ostream& print_vec(std::ostream& os, A const& v, LevelGuard lg, std::size_t newline_indent_len = 0) {
        auto const sp = lg.space();
        lg.guard();

        auto const newline_indent = std::string(newline_indent_len, ' ');
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "3;92");
            os << "(Size: " << v.size() << ") ";
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "1");

        os << "[\n" << sp << newline_indent;
        for (auto i = 0ul; i < v.size(); ++i) {
            auto const& el = v[i];
            os << sp;
            internal::reset_escape_code(os);
            internal::set_escape_code(os, "3;2");
            os << "[" << i << "]";
            internal::reset_escape_code(os);
            os << " = ";
            internal::set_escape_code(os, "1");
            print(os, el, lg, newline_indent_len);
            if (i + 1 < v.size()) os << ",\n" << newline_indent << sp;
        }
        return os << "\n" << sp << newline_indent << "]";
    }

    template<typename T>
    std::ostream& print_map(std::ostream& os, T const& v, LevelGuard lg, std::size_t newline_indent_len = 0) {
        auto const sp = lg.space();
        lg.guard();

        auto const newline_indent = std::string(newline_indent_len, ' ');
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "3;92");
            os << "(Size: " << v.size() << ") ";
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "1");

        os << "{\n" << sp << newline_indent;
        auto i = 0ul;
        for (auto const& [k, el] : v) {
            os << sp;
            print(os, k, lg, newline_indent_len);
            os << ": ";
            print(os, el, lg, newline_indent_len);
            if (i + 1 < v.size()) os << ",\n" << newline_indent << sp;
            ++i;
        }
        return os << "\n" << sp << newline_indent << "}";
    }

    template<typename T>
    std::ostream& print_set(std::ostream& os, T const& v, LevelGuard lg, std::size_t newline_indent_len = 0) {
        auto const sp = lg.space();
        lg.guard();

        auto const newline_indent = std::string(newline_indent_len, ' ');
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "3;92");
            os << "(Size: " << v.size() << ") ";
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "1");

        os << "{\n" << sp << newline_indent;
        auto i = 0ul;
        for (auto const& el : v) {
            os << sp;
            internal::reset_escape_code(os);
            internal::set_escape_code(os, "3;2");
            os << "[" << i << "]";
            internal::reset_escape_code(os);
            os << " = ";
            internal::set_escape_code(os, "1");
            print(os, el, lg, newline_indent_len);
            if (i + 1 < v.size()) os << ",\n" << newline_indent << sp;
            ++i;
        }
        return os << "\n" << sp << newline_indent << "}";
    }

    template<typename T, typename U>
    std::ostream& print_pair(std::ostream& os, std::pair<T, U> const& v, LevelGuard lg, std::size_t newline_indent_len = 0) {
        os << "(";
        print(os, v.first, lg, newline_indent_len);
        os << ", "; 
        print(os, v.second, lg, newline_indent_len);
        return os << ")";
    }

    template<typename... Args>
    std::ostream& print_tuple(std::ostream& os, std::tuple<Args...> const& v, LevelGuard lg, std::size_t newline_indent_len = 0) {
        os << "(";
        tuple_for(v, [size=sizeof...(Args), lg, newline_indent_len, &os](auto const& el, std::size_t i) {
            print(os, el, lg, newline_indent_len);
            if (i + 1 < size) os << ", ";
        });
        return os << ")";
    }

    template<typename T>
    std::ostream& print(std::ostream& os, T const& v, LevelGuard lg, std::size_t newline_indent_len = 0) {
        using v_type = std::decay_t<T>;
        if constexpr (internal::is_array<v_type>::value) {
            return print_vec(os, v, lg, newline_indent_len);
        } else if constexpr(internal::is_map<v_type>::value) {
            return print_map(os, v, lg, newline_indent_len);
        } else if constexpr(internal::is_string<v_type>::value) {
            return os << '"' << v << '"';
        } else if constexpr(internal::is_pair<v_type>::value) {
            return print_pair(os, v, lg, newline_indent_len);
        } else if constexpr(internal::is_tuple<v_type>::value) {
            return print_tuple(os, v, lg, newline_indent_len);
        } else if constexpr (internal::is_set<v_type>::value) {
            return print_set(os, v, lg, newline_indent_len);
        } else {
            return os << v;
        }
    }

    template<typename T>
    std::ostream& print(std::ostream& os, MatrixWrapper<T> const& m, LevelGuard lg, std::size_t newline_indent_len = 0) {
        if constexpr (!internal::is_array<std::decay_t<T>>::value) {
            return print_flat_matrix(os, m, lg, newline_indent_len);
        } else {
            return print_matrix(os, m, lg, newline_indent_len);
        }
    }
    
    template<typename T>
    std::ostream& print_flat_matrix(std::ostream& os, MatrixWrapper<T> const& m, LevelGuard lg, std::size_t newline_indent_len = 0) {
        lg.guard();
        auto const sp = lg.space();

        auto const is_matrix = m.cols > 1 && m.rows > 1;
        auto const is_col_vec = m.cols == 1 && m.rows > 1;
        auto const is_row_vec = m.cols > 1 && m.rows == 1;
        auto const is_scalar = m.cols == 1 && m.rows == 1;

        auto const newline_indent = std::string(newline_indent_len, ' ');
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "3;92");
            if (is_matrix) os << "(Rows: " << m.rows << ", Cols: " << m.cols << ") ";
            else if (is_col_vec) os << "(Cols: " << m.cols << ") ";
            else if (is_row_vec) os << "(Rows: " << m.rows << ") ";
            else os << "(Size: " << m.data.size() << ") ";
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "1");

        auto const max_num = std::max_element(m.data.begin(), m.data.end());
        auto const max_num_len = std::to_string(*max_num).size();

        os << "[";
        if (is_matrix || is_col_vec) {
            os << "\n" << sp << newline_indent;
        }

        for (auto i = 0ul; i < m.rows; ++i) {
            if (!is_row_vec && !is_scalar) {
                os << sp;
            }
            for (auto j = 0ul; j < m.cols; ++j) {
                auto const el = m.data[i * m.cols + j];
                os << std::setw(max_num_len) << el;
                if (j + 1 < m.cols) {
                    os << ", ";
                }
            }
            if (i + 1 < m.rows) {
                os << ",\n" << sp << newline_indent;
            }
        }

        if (is_matrix || is_col_vec) {
            os << '\n' << sp << newline_indent;
        }
        os << "]" << std::endl;

        return os;
    }
    
    template<typename T>
    std::ostream& print_matrix(std::ostream& os, MatrixWrapper<T> const& m, LevelGuard lg, std::size_t newline_indent_len = 0) {
        lg.guard();
        auto const sp = lg.space();

        auto const is_matrix = m.cols > 1 && m.rows > 1;
        auto const is_col_vec = m.cols == 1 && m.rows > 1;
        auto const is_row_vec = m.cols > 1 && m.rows == 1;
        auto const is_scalar = m.cols == 1 && m.rows == 1;

        auto const newline_indent = std::string(newline_indent_len, ' ');
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "3;92");
            if (is_matrix) os << "(Rows: " << m.rows << ", Cols: " << m.cols << ") ";
            else if (is_col_vec) os << "(Cols: " << m.cols << ") ";
            else if (is_row_vec) os << "(Rows: " << m.rows << ") ";
            else os << "(Size: " << m.data.size() << ") ";
        internal::reset_escape_code(os);
        internal::set_escape_code(os, "1");

        auto max_num = 0;
        for (auto const& v : m.data) {
            for (auto const& el : v) {
                max_num = std::max(max_num, el);
            }
        }
        auto const max_num_len = std::to_string(max_num).size();

        os << "[";
        if (is_matrix || is_col_vec) {
            os << "\n" << sp << newline_indent;
        }

        for (auto i = 0ul; i < m.rows; ++i) {
            if (!is_row_vec && !is_scalar) {
                os << sp;
            }
            for (auto j = 0ul; j < m.cols; ++j) {
                auto const el = m.data[i][j];
                os << std::setw(max_num_len) << el;
                if (j + 1 < m.cols) {
                    os << ", ";
                }
            }
            if (i + 1 < m.rows) {
                os << ",\n" << sp << newline_indent;
            }
        }

        if (is_matrix || is_col_vec) {
            os << '\n' << sp << newline_indent;
        }
        os << "]" << std::endl;

        return os;
    }

} // namespace amt

template<typename T, typename A>
std::ostream& operator<<(std::ostream& os, std::vector<T, A> const& v) {
    return amt::print(os, v, amt::LevelGuard{});
}



namespace amt {
    struct Span {
        unsigned start;
        unsigned end;
    };

    // Lex C++ syntax
    std::vector<Span> parse_string(StringView sv) {
        std::vector<Span> ps;
        unsigned start = 0;
        for (unsigned i = 0; i < sv.len + 1; ++i) {
            auto const c = sv.str[i];
            switch (c) {
                case ' ': {
                    start = i + 1;
                    break;
                }
                case ',': case '\0': {
                    if (start == i) break;
                    ps.push_back({ start, i });
                    start = i + 1;
                    break;
                }
                case '{': case '(': {
                    unsigned depth = 1;
                    auto const open = c;
                    auto const close = c == '{' ? '}' : ')';
                    while (depth > 0) {
                        ++i;
                        assert(i < sv.len, "Unmatched parenthesis", nullptr, internal::Location(__FILE__, __LINE__));
                        if (sv.str[i] == open) ++depth;
                        else if (sv.str[i] == close) --depth;
                    }
                    ++i;
                    if (start == i) break;

                    ps.push_back({ start, i });
                    start = i + 1;
                    break;
                }
                case '"': {
                    ++i;
                    while (sv.str[i] != '"') {
                        assert(i < sv.len, "Unmatched quote", nullptr, internal::Location(__FILE__, __LINE__));
                        if (sv.str[i] == '\\') ++i;
                        ++i;
                    }
                    ++i;
                    if (start == i) break;
                    ps.push_back({ start, i });
                    start = i + 1;
                    break;
                }
                case '\'': {
                    ++i;
                    while (sv.str[i] != '\'') {
                        assert(i < sv.len, "Unmatched quote", nullptr, internal::Location(__FILE__, __LINE__));
                        if (sv.str[i] == '\\') ++i;
                        ++i;
                    }
                    if (start == i) break;
                    ps.push_back({ start, i });
                    start = i + 1;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        return ps;
    }

    namespace internal {

        template<typename... Args, std::size_t... Is>
        std::ostream& print_tuple_at(std::ostream& os, std::tuple<Args...> const& args, unsigned i, unsigned len, std::index_sequence<Is...>) {
            return ((Is == i ? ::amt::print(os, std::get<Is>(args), LevelGuard{}, len) : os),...);
        }

        template<typename... Args>
        void debug_internal(std::ostream& os, StringView sv, std::tuple<Args...> const& args) {
            std::vector<Span> ps = parse_string(sv);
            os << "debug {\n";
            auto i = 0ul;
            for (auto const& p : ps) {
                os << "  ";
                set_escape_code(os, "1;93");
                os << sv.substr(p.start, p.end);
                reset_escape_code(os);
                os << " = ";
                set_escape_code(os, "1");
                auto start_indent = std::max(p.end - p.start + 1, 6u);
                print_tuple_at(os, args, i, start_indent, std::make_index_sequence<sizeof...(Args)>());
                reset_escape_code(os);
                os << std::endl;
                ++i;
            }
            os << "} \n";
        }
    }

    // debug_impl("a, b, c", a, b, c);
    template<typename... Args>
    void debug_impl(std::ostream& os, char const* str, Args&&...args) {
        internal::debug_internal(os, StringView::from(str), std::forward_as_tuple(args...));
    }
}

#define debug_s(OS, ...) amt::debug_impl((OS), #__VA_ARGS__, __VA_ARGS__)
#define debug(...) debug_s(std::clog, __VA_ARGS__)
#define matrix_w(V, R, C) amt::MatrixWrapper((V), (R), (C))
#define col_vec_w(V, C) matrix_w((V), 1ul, (C))
#define row_vec_w(V, R) matrix_w((V), (R), 1ul)
#define assert(COND, MSG) amt::assert((COND), (MSG), #COND, amt::internal::Location(__FILE__, __LINE__))
#define assert_cmp(CMP, LEFT, RIGHT, MSG) amt::assert(amt::Cmp::CMP, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))
#define assert_eq(LEFT, RIGHT, MSG) amt::assert(amt::Cmp::Eq, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))
#define assert_neq(LEFT, RIGHT, MSG) amt::assert(amt::Cmp::NotEq, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))
#define assert_lt(LEFT, RIGHT, MSG) amt::assert(amt::Cmp::Less, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))
#define assert_lte(LEFT, RIGHT, MSG) amt::assert(amt::Cmp::LessEq, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))
#define assert_gt(LEFT, RIGHT, MSG) amt::assert(amt::Cmp::Greater, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))
#define assert_gte(LEFT, RIGHT, MSG) amt::assert(amt::Cmp::GreaterEq, (LEFT), (RIGHT), #LEFT, #RIGHT, (MSG), amt::internal::Location(__FILE__, __LINE__))

#endif // __AMT_DEBUG_HPP__