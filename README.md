# `tetter`

A simple, convenient single-header library that simplifies type transformations on a list of types (like std::tuple) and calling template functions per type.

Compatible with C++11, but significantly more convenient to use with C++20. It does not use any std headers.

# Examples:

### Functions:

```cpp
using t0 = tetter<bool, int, char, long, double>;
using t1 = typename t0::template slice<1, 3>;
using t2 = typename t1::pop_back::reverse;

static_assert(std::is_same<t1, tetter<int, char, long>>::value);
static_assert(std::is_same<t2, tetter<char, int>>::value);
```

### Iterators:

```cpp
using t = tetter<bool, long long, int, char, double>;

#if Cpp20
using t_map = typename t::template map_l<[]<typename t>() -> const t* {}>;
using t_sort = typename t::template sort_l<[]<typename lt, typename rt>() { return sizeof(lt) < sizeof(rt); }>;
static constexpr bool t_value = t::template value_l<[]<std::size_t i>() { return (i % 64) == 128; }>::any;
#endif // Cpp20

#if Cpp11
template <typename t, std::size_t>
struct maping
{
	using type = const t*;
};

template <typename lt, typename rt>
struct sorting
{
	static constexpr bool value = sizeof(lt) < sizeof(rt);
};

template <typename, std::size_t i>
struct valueing
{
	static constexpr bool value = (i % 64) == 128;
};

using t_map = typename t::template map<maping>;
using t_sort = typename t::template sort<sorting>;
static constexpr bool t_value = t::template value<valueing>::any;
#endif // Cpp11

static_assert(std::is_same<t_map, tetter<const bool*, const long long*, const int*, const char*, const double*>>::value);
static_assert(std::is_same<t_sort, tetter<bool, char, int, long long, double>>::value);
static_assert(t_value == false);
```

### Runtime-Iterators:

```cpp
#if Cpp20
template <tetter_c x>
std::size_t total_size(std::size_t biger_then)
{
	return x::invoke_sum([]<typename t>(std::size_t m) {
        std::cout << sizeof(t) << '\n';
        return sizeof(t) > m ? sizeof(t) : m;
    }, multiply_by);
}
#endif // Cpp20

#if Cpp11
template <typename t, std::size_t i>
struct total_size_impl
{
	static std::size_t call(std::size_t m)
	{
        std::cout << sizeof(t) << '\n';
        return sizeof(t) > m ? sizeof(t) : m;
	}
};

template <typename x>
std::size_t total_size(std::size_t multiply_by)
{
    static_assert(is_tetter<x>::value);
	return x::template call_sum<suming>(multiply_by);
}
#endif // Cpp11

using t = tetter<bool, long long, int, short, double>;

int main()
{
	assert(total_size<t>(3) == ((0) + 8 + 4 + (0) + 8);
}
```

### Generating tetter:

```cpp
static_assert(std::is_same<tetter_from<std::tuple<int, bool>>, tetter<int, bool>>::value);
static_assert(std::is_same<tetter_sequence<4>, tetter<__tse<0>, __tse<1>, __tse<2>, __tse<3>>>::value);

struct example_class
{
    int function(bool&& v) const;
};

static_assert(std::is_same<tetter_from_args<decltype(&example_class::function)>, tetter<const example_class&, bool&&>>::value);
```

# Simplified source code:

> [!NOTE]
>
> Any `using XXX = /* tetter<...> */;` will not throw out of bound error, but will usually just return an empty `tetter<>`.
>
> eg.: `typename tetter<a, b, c>::template slice<3, 2137>` -> `tetter<>`

> [!NOTE]
> By passing rvalue-ref args to `tetter<...>::call` or `tetter<...>::call_XXX`, only the last call will get rvalue-ref args.
>
> Exception is first argument of the `tetter<...>::call_pipe`, it always can be rvalue-ref;
>
> eg.: `tetter<a, b, c>::invoke([]<typename t>(auto&& arg){ print(is_rvalue_ref<decltype(arg)>); }, std::move(variable))` -> `0` `0` `1`

```cpp
template <typename... ts>
struct tetter
{
	static constexpr size_t count;

	// (for empty tetter<> ::front and ::back will not be defined)
	using front = /* type */;
	using back = /* type */;

	template <typename t>
	using front_or = /* type */;
	template <typename t>
	using back_or = /* type */;

	template <typename... types>
	using push_front = /* tetter<...> */; 
	template <typename... types>
	using push_back = /* tetter<...>*/;

	template <size_t index, typename... types>
	using insert = /* tetter<...> */;

	template <size_t index, size_t amount = 1>
	using remove = /* tetter<...> */;

	template <typename... generic_types>
	using join = /* tetter<...> */;

	using reverse = /* tetter<...> */;

	template <size_t amount>
	using pop_front_n = /* tetter<...> */;
	template <size_t amount>
	using pop_back_n = /* tetter<...> */;

	using pop_front = /* tetter<...> */;
	using pop_back = /* tetter<...> */;

	template <size_t index>
	using get = /* type */;

	template <size_t index, size_t count>
	using slice = /* tetter<...> */;;

	template <template <typename...> typename to_type>
	using cast = /* type<...> */;

	template <template <typename...> typename... types_to_concat> 
	using concat = /* tetter<...> */;

	// concat any concatable type
	using concat_a = /* tetter<...> */;

	// a, a, b, b, c, c
	template <size_t times>
	using repeat = /* tetter<...> */;

	// types will be multiplied so tetter<ts...>::count * times == /* result */::count
	template <size_t times>
	using repeat_m = /* tetter<...> */;

	// a, b, c, a, b, c
	template <size_t times>
	using copy = /* tetter<...> */;

	// types will be multiplied so tetter<...>::count * times == /* result */::count
	template <size_t times>
	using copy_m = /* tetter<...> */;

	///
	//	struct find_result
	//	{
	//		static constexpr bool value; // success
	//		static constexpr size_t index; // if failed to find then find_result::index == tetter<...>::count
	//	}
	///
	// iterator must have static constexpr bool ::value
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator>
	using find = /* find_result */;

	// wrapper simplifying usage of find
	template <typename... types_to_find>
	using find_t = /* find_result */;

	// iterator must have static constexpr bool ::value
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator>
	using filter = /* tetter<...> */;

	// wrapper simplifying usage of filter
	template <typename... types_to_remove>
	using filter_t = /* tetter<...> */;

	// wrapper simplifying usage of filter
	template <typename... types_to_leave>
	using filter_t_l = /* tetter<...> */;

	// iterator must have static constexpr bool ::value
	template <template <typename left_type, typename right_type> typename sorter>
	using sort = /* tetter<...> */;

	// iterator must have declared type ::type
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator>
	using map = /* tetter<...> */;

	///
	//	struct value_result
	//	{
	//		// for bool-like
	//		static constexpr /* auto */ all;
	// 		static constexpr /* auto */ any;
	//		static constexpr /* auto */ none;
	//
	//		// for int-like
	//		static constexpr /* auto */ sum;
	//		static constexpr /* auto */ avg;
	// 		static constexpr /* auto */ max;
	//		static constexpr /* auto */ min;
	//	}
	///
	// iterator must have static constexpr bool-like&int-like (operators: &&, ||, !, +, /, <, >)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator>
	using value = /* value_result */;

	// iterator must have static constexpr bool-like (operators: &&, ||, !)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator>
	using value_b = /* value_result(only bool-like) */;

	// iterator must have static constexpr int-like (operators: +, /, <, >)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator>
	using value_i = /* value_result(only int-like) */;

	static constexpr bool has_duplicats;
	using remove_duplicats = /* tetter<...> */;

	// eg. will produce: it<a, 0>(args...); it<b, 1>(args...); it<c, 2>(args...)
	// iterator must have static ::call function or non-static operator() (C++23: or static operator())
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr void call(args&&... _args);

	// eg. will produce: it<c, 2>(it<b, 1>(it<a, 0>(inital_value, args...), args...), args...)
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which takes result of previous call as first argument
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename inital_type, typename... args>
	static constexpr /* auto */ call_pipe(inital_type&& inital_value = inital_type{}, args&&... _args);

	// eg. will produce: it<a, 0>(args...) && it<b, 1>(args...) && it<c, 2>(args...)
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns bool-like (operators: &&, ||, !)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_all(args&&... _args)

	// eg. will produce: it<a, 0>(args...) || it<b, 1>(args...) || it<c, 2>(args...)
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns bool-like (operators: &&, ||, !)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_any(args&&... _args)

	// eg. will produce: !(it<a, 0>(args...) || it<b, 1>(args...) || it<c, 2>(args...))
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns bool-like (operators: &&, ||, !)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_none(args&&... _args)

	// eg. will produce: it<a, 0>(args...) + it<b, 1>(args...) + it<c, 2>(args...)
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns int-like (operators: +, /, <, >)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_sum(args&&... _args)
	
	// eg. will produce: (it<a, 0>(args...) + it<b, 1>(args...) + it<c, 2>(args...)) / tetter<a, b, c>::count
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns int-like (operators: +, /, <, >)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_avg(args&&... _args);

	// eg. will produce: min(min(it<a, 0>(args...), it<b, 1>(args...)), it<c, 2>(args...))
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns int-like (operators: +, /, <, >)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_min(args&&... _args);

	// eg. will produce: max(max(it<a, 0>(args...), it<b, 1>(args...)), it<c, 2>(args...))
	// iterator must have static ::call function or non-static operator() (C++23: or static operator()) which returns int-like (operators: +, /, <, >)
	template <template <typename current_type, size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	static constexpr /* auto */ call_max(args&&... _args);

    // lambda_iterator does not need to have all the template arguments that iterator must have, 
    // so the following lambdas are valid:
    //  []<typename t>() { /* ... */ }
    //  []<size_t i>() { /* ... */ }
    //  []<typename t, size_t i>() { /* ... */ }
    //  []<typename t, typename... all_types>() { /* ... */ }
    //  []<size_t i, typename... all_types>() { /* ... */ }
    //  []<typename t, size_t i, typename... all_types>() { /* ... */ }

	// (C++20)
    // usefull for casting to complex types
    // eg.: template <size_t c, typename... ts> struct complex_type; using casted_type = typename /* tetter<...> */::template cast_l<[]<typename... ts>() -> complex_type<2137, ts...> {}>;
	template <auto lambda_caster>
	using cast_l = /* return_type */;

	// (C++20)
	// lambda_iterator must return boolean
	template <auto lambda_iterator>
	using find_l = /* find_result */;

	// (C++20)
	// lambda_iterator must return boolean
	template <auto lambda_iterator>
	using filter_l = /* tetter<...> */;

	// (C++20)
	// lambda_iterator must return boolean
	template <auto lambda_sorter>
	using sort_l = /* tetter<...> */;

	// (C++20)
	// types will be maped to return type of lambda_iterator (so lambda dont need return any value)
	template <auto lambda_iterator>
	using map_l = /* tetter<...> */;

	// (C++20)
	// lambda_iterator must return bool-like&int-like (operators: &&, ||, !, +, /, <, >)
	template <auto lambda_iterator>
	using value_l = /* value_result */;

	// (C++20)
	// lambda_iterator must return bool-like (operators: &&, ||, !)
	template <auto lambda_iterator>
	using value_b_l = /* value_result(only bool-like) */;

	// (C++20)
	// lambda_iterator must return int-like (operators: +, /, <, >)
	template <auto lambda_iterator>
	using value_i_l = /* value_result(only int-like) */;

	// (C++20)
	// wrapper for call
	template <typename lambda, typename... args>
	static constexpr void invoke(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_pipe
	template <typename lambda, typename inital_type, typename... args>
	static constexpr auto invoke_pipe(lambda&& _lambda, inital_type&& inital_value = inital_type{}, args&&... _args);

	// (C++20)
	// wrapper for call_all
	template <typename lambda, typename... args>
	static constexpr auto invoke_all(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_any
	template <typename lambda, typename... args>
	static constexpr auto invoke_any(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_none
	template <typename lambda, typename... args>
	static constexpr auto invoke_none(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_sum
	template <typename lambda, typename... args>
	static constexpr auto invoke_sum(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_avg
	template <typename lambda, typename... args>
	constexpr auto invoke_avg(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_min
	template <typename lambda, typename... args>
	static constexpr auto invoke_min(lambda&& _lambda, args&&... _args);

	// (C++20)
	// wrapper for call_max
	template <typename lambda, typename... args>
	static constexpr auto invoke_max(lambda&& _lambda, args&&... _args);
};


// eg.: tetter_from<std::tuple<int, char&>> -> tetter<int, char&>
template <typename t>
using tetter_from = /* tetter<...> */;

// eg.: void function(void* a, const int& b); tetter_from_args<decltype(&function)> -> tetter<void*, const int&>
// also works with methods (decltype(*this) is also included)
template <typename fn>
using tetter_from_args = /* tetter<...> */;

template <size_t n, template <size_t> typename sequence_type = /* struct { static constexpr size_t value; } */>
using tetter_sequence = /* tetter<sequence_type<i>, ...> */;


// will evaluate to true for any tetter<...>
template <typename t>
struct is_tetter
{
	static constexpr bool value;
};

// (C++14)
// wrapper for is_tetter::value
template <typename t>
inline static constexpr bool is_tetter_v;

// (C++20)
// wrapper for is_tetter_v
template <typename t>
concept tetter_c;
```
