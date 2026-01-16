/* tetter - https://github.com/Maslowian/tetter */

/* 
MIT License

Copyright (c) 2025-2026 Piotr Ginalski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef _tetter_hpp
#define _tetter_hpp

#define tetter_version_major 1
#define tetter_version_minor 0
#define tetter_version_patch 0

#define _tetter_check(x, v) (defined(__ ## x) ? (__ ## x >= v) : (__cplusplus >= v)) 

#ifndef _tetter_decltype
#if _tetter_check(cpp_decltype_auto, 201304L)
#define _tetter_decltype(...) decltype(auto) 
#else
#define _tetter_decltype(...) decltype(__VA_ARGS__)
#endif
#endif

#ifndef _tetter_variable_templates
#if _tetter_check(cpp_variable_templates, 201304L)
#define _tetter_variable_templates true
#else
#define _tetter_variable_templates false
#endif
#endif

#ifndef _tetter_generic_lambdas
#if _tetter_check(cpp_generic_lambdas, 201707L)
#define _tetter_generic_lambdas true
#else
#define _tetter_generic_lambdas false
#endif
#endif

#ifndef _tetter_concepts
#if _tetter_check(cpp_concepts, 201907L)
#define _tetter_concepts true
#else
#define _tetter_concepts false
#endif
#endif

#ifndef _tetter_unevaluated_lambda
#if _tetter_check(cplusplus, 202002L)
#define _tetter_unevaluated_lambda true
#else
#define _tetter_unevaluated_lambda false 
#endif
#endif

#ifndef _tetter_static_call_operator
#if _tetter_check(cpp_static_call_operator, 202207L)
#define _tetter_static_call_operator true
#else
#define _tetter_static_call_operator false
#endif
#endif

namespace _tetter { namespace {

template <bool v, typename tt, typename ft>
struct _conditional;

template <typename tt, typename ft>
struct _conditional<false, tt, ft>
{
	using type = ft;
};

template <typename tt, typename ft>
struct _conditional<true, tt, ft>
{
	using type = tt;
};

template <typename ft, typename st>
struct _is_same
{
	static constexpr bool value = false;
};

template <typename t>
struct _is_same<t, t>
{
	static constexpr bool value = true;
};

using _size_t = decltype(sizeof(0)); 

template <typename t>
t&& _declval() noexcept { static_assert(false); }

template <typename... ts>
struct _tetter
{
	template <template <typename...> typename t>
	using cast = t<ts...>;
};

template <typename lt, typename rt>
struct _join_tetter;

template <typename... lts, typename... rts>
struct _join_tetter<_tetter<lts...>, _tetter<rts...>>
{
	using type = _tetter<lts..., rts...>;
};

template<typename... Ts>
struct _enable_impl { using type = void; };
 
template <typename... ts>
using _enable_t = typename _enable_impl<ts...>::type;

template <typename... ts>
struct _false
{
	static constexpr bool value = false;
};

template <typename t>
struct _clean
{
	using type = t;
};

template <typename t>
struct _clean<const t>
{
	using type = t;
};

template <typename t>
struct _clean<volatile t>
{
	using type = t;
};

template <typename t>
struct _clean<t&>
{
	using type = t;
};

template <typename t>
struct _clean<const t&>
{
	using type = t;
};

template <typename t>
struct _clean<volatile t&>
{
	using type = t;
};

template <typename t>
struct _clean<t&&>
{
	using type = t;
};

template <typename t>
struct _clean<const t&&>
{
	using type = t;
};

template <typename t>
struct _clean<volatile t&&>
{
	using type = t;
};

template <typename... ts>
struct _front_impl;

template <typename t, typename... ts>
struct _front_impl<t, ts...>
{
	using type = t;
};

template <typename... ts>
struct _back_impl;

template <typename t, typename st, typename... ts>
struct _back_impl<t, st, ts...>
{
	using type = typename _back_impl<st, ts...>::type;
};

template <typename t>
struct _back_impl<t>
{
	using type = t;
};

template <_size_t i, typename... ts>
struct _pop_front_impl;

template <_size_t i, typename t, typename... ts>
struct _pop_front_impl<i, t, ts...> 
{
	using type = typename _pop_front_impl<i-1, ts...>::type;
};

template <typename t, typename... ts>
struct _pop_front_impl<0, t, ts...> 
{
	using type = _tetter<t, ts...>;
};

template <_size_t i>
struct _pop_front_impl<i> 
{
	using type = _tetter<>;
};

template <_size_t i, typename tis, typename tos>
struct _pop_back_impl
{
	using type = void;
};

template <_size_t i, typename ti, typename... tis, typename... tos>
struct _pop_back_impl<i, _tetter<ti, tis...>, _tetter<tos...>>
{
	using type = typename _conditional<sizeof...(tos) + 1 == i, _tetter<tos..., ti>, typename _pop_back_impl<i, _tetter<tis...>, _tetter<tos..., ti>>::type>::type;
};

template <_size_t i, typename... ts>
struct _get_impl;

template <_size_t i, typename t, typename... ts>
struct _get_impl<i, t, ts...>
{
	using type = typename _get_impl<i-1, ts...>::type;
};

template <typename t, typename... ts>
struct _get_impl<0, t, ts...>
{
	using type = t;
};

template <_size_t i>
struct _get_impl<i>
{
	static_assert(false, "out of bounds");
};

template <_size_t i, _size_t c, typename tis, typename tos>
struct _slice_impl;

template <_size_t i, _size_t c, typename ti, typename... tis, typename... tos>
struct _slice_impl<i, c, _tetter<ti, tis...>, _tetter<tos...>>
{
	using type = typename _slice_impl<i-1, c, _tetter<tis...>, _tetter<tos...>>::type;
};

template <_size_t c, typename ti, typename... tis, typename... tos>
struct _slice_impl<0, c, _tetter<ti, tis...>, _tetter<tos...>>
{
	using type = typename _slice_impl<0, c-1, _tetter<tis...>, _tetter<tos..., ti>>::type;
};

template <typename ti, typename... tis, typename... tos>
struct _slice_impl<0, 0, _tetter<ti, tis...>, _tetter<tos...>>
{
	using type = _tetter<tos...>;
};

template <_size_t i, _size_t c, typename... tos>
struct _slice_impl<i, c, _tetter<>, _tetter<tos...>>
{
	using type = _tetter<tos...>;
};

template <typename tli, typename tlo>
struct _reverse_impl;

template <typename tli, typename... tlis, typename... tlos>
struct _reverse_impl<_tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _reverse_impl<_tetter<tlis...>, _tetter<tli, tlos...>>::type;
};

template <typename... tlos>
struct _reverse_impl<_tetter<>, _tetter<tlos...>>
{
	using type = _tetter<tlos...>;
};

template <_size_t i, typename tl, typename tlo>
struct _copy_impl;

template <_size_t i, typename... tls, typename... tlos>
struct _copy_impl<i, _tetter<tls...>, _tetter<tlos...>>
{
	using type = typename _copy_impl<i-1, _tetter<tls...>, _tetter<tlos..., tls...>>::type;
};

template <typename... tls, typename... tlos>
struct _copy_impl<0, _tetter<tls...>, _tetter<tlos...>>
{
	using type = _tetter<tlos...>;
};

template <_size_t i, typename t, typename tlo>
struct _repeat_impl_impl;

template <_size_t i, typename t, typename... tlo>
struct _repeat_impl_impl<i, t, _tetter<tlo...>>
{
	using type = typename _repeat_impl_impl<i-1, t, _tetter<tlo..., t>>::type;
};

template <typename t, typename... tlo>
struct _repeat_impl_impl<0, t, _tetter<tlo...>>
{
	using type = _tetter<tlo...>;
};

template <_size_t i, typename tli, typename tlo>
struct _repeat_impl;

template <_size_t i, typename tli, typename... tlis, typename... tlos>
struct _repeat_impl<i, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _repeat_impl<i, _tetter<tlis...>, typename _repeat_impl_impl<i, tli, _tetter<tlos...>>::type>::type;
};

template <_size_t i, typename... tlos>
struct _repeat_impl<i, _tetter<>, _tetter<tlos...>>
{
	using type = _tetter<tlos...>;
};

template <template <typename, _size_t, typename...> typename vt, typename t, _size_t i, typename ats, typename enable>
struct _wrapper_impl
{
	using type = vt<t, i>;
};

template <template <typename, _size_t, typename...> typename vt, typename t, _size_t i, typename... ts>
struct _wrapper_impl<vt, t, i, _tetter<ts...>, _enable_t<vt<t, i, ts...>>>
{
	using type = vt<t, i, ts...>;
};

template <template <typename, _size_t, typename...> typename vt>
struct _wrapper
{
	template <typename t, _size_t i, typename ats>
	using type = typename _wrapper_impl<vt, t, i, ats, _enable_t<>>::type;
};

template <template <typename, typename> typename vt>
struct _sort_wrapper
{
	template <typename lt, typename rt>
	using type = vt<lt, rt>;
};

template <template <typename> typename vt>
struct _action_wrapper
{
	template <typename t>
	using type = vt<t>;
};

template <template <typename> typename vt>
struct _condition_wrapper
{
	template <typename t>
	using type = vt<t>;
};

template <_size_t i, typename w, typename rts, typename ats, typename ots>
struct _type_impl;

template <_size_t i, typename w, typename rt, typename... rts, typename ats, typename... ots>
struct _type_impl<i, w, _tetter<rt, rts...>, ats, _tetter<ots...>>
{
	using type = typename _type_impl<i+1, w, _tetter<rts...>, ats, _tetter<ots..., typename w::template type<rt, i, ats>::type>>::type;
};

template <_size_t i, typename w, typename ats, typename... ots>
struct _type_impl<i, w, _tetter<>, ats, _tetter<ots...>>
{
	using type = _tetter<ots...>; 
};

template <_size_t i, typename w, typename rts, typename ats>
struct _bool_value_impl;

template <_size_t i, typename w, typename rt, typename srt, typename... rts, typename ats>
struct _bool_value_impl<i, w, _tetter<rt, srt, rts...>, ats>
{
	static constexpr auto all = w::template type<rt, i, ats>::value && _bool_value_impl<i+1, w, _tetter<srt, rts...>, ats>::all;
	static constexpr auto any = w::template type<rt, i, ats>::value || _bool_value_impl<i+1, w, _tetter<srt, rts...>, ats>::any;
};

template <_size_t i, typename w, typename rt, typename ats>
struct _bool_value_impl<i, w, _tetter<rt>, ats>
{
	static constexpr auto all = w::template type<rt, i, ats>::value;
	static constexpr auto any = w::template type<rt, i, ats>::value;
};

template <_size_t i, typename w, typename ats>
struct _bool_value_impl<i, w, _tetter<>, ats>
{
	static constexpr auto all = true;
	static constexpr auto any = false;
};

template <_size_t i, typename w, typename rts, typename ats>
struct _int_value_impl;

template <_size_t i, typename w, typename rt, typename srt, typename... rts, typename ats>
struct _int_value_impl<i, w, _tetter<rt, srt, rts...>, ats>
{
	static constexpr auto sum = w::template type<rt, i, ats>::value + _int_value_impl<i+1, w, _tetter<srt, rts...>, ats>::sum;
	static constexpr auto min = w::template type<rt, i, ats>::value < _int_value_impl<i+1, w, _tetter<srt, rts...>, ats>::min ? w::template type<rt, i, ats>::value : _int_value_impl<i+1, w, _tetter<srt, rts...>, ats>::min;
	static constexpr auto max = w::template type<rt, i, ats>::value > _int_value_impl<i+1, w, _tetter<srt, rts...>, ats>::max ? w::template type<rt, i, ats>::value : _int_value_impl<i+1, w, _tetter<srt, rts...>, ats>::max;
};

template <_size_t i, typename w, typename rt, typename ats>
struct _int_value_impl<i, w, _tetter<rt>, ats>
{
	static constexpr auto sum = w::template type<rt, i, ats>::value;
	static constexpr auto min = w::template type<rt, i, ats>::value;
	static constexpr auto max = w::template type<rt, i, ats>::value;
};

template <_size_t i, typename w, typename ats>
struct _int_value_impl<i, w, _tetter<>, ats>
{
	static constexpr auto sum = 0;
	static constexpr auto min = 0;
	static constexpr auto max = 0;
};

template <_size_t i, typename w, typename rts, typename ats>
struct _find_impl;

template <_size_t i, typename w, typename rt, typename... rts, typename ats>
struct _find_impl<i, w, _tetter<rt, rts...>, ats> 
{
private:
	static constexpr bool found = w::template type<rt, i, ats>::value;
	using next = _find_impl<i+1, w, _tetter<rts...>, ats>;

public:
	static constexpr _size_t index = found ? i : next::index;
	using type = typename _conditional<found, rt, typename next::type>::type;
};

template <_size_t i, typename w, typename ats>
struct _find_impl<i, w, _tetter<>, ats>
{
	static constexpr _size_t index = i;
	using type = void;
};

template <_size_t i, typename w, typename tli, typename tlo, typename ats>
struct _filter_impl;

template <_size_t i, typename w, typename tli, typename... tlis, typename... tlos, typename ats>
struct _filter_impl<i, w, _tetter<tli, tlis...>, _tetter<tlos...>, ats>
{
	using type = typename _filter_impl<i+1, w, _tetter<tlis...>, typename _conditional<w::template type<tli, i, ats>::value, _tetter<tlos..., tli>, _tetter<tlos...>>::type, ats>::type;
};

template <_size_t i, typename w, typename... tlos, typename ats>
struct _filter_impl<i, w, _tetter<>, _tetter<tlos...>, ats>
{
	using type = _tetter<tlos...>;
};

template <typename w, typename t, typename tli, typename tlo>
struct _sort_impl_impl;

template <typename w, typename t, typename tli, typename... tlis, typename... tlos>
struct _sort_impl_impl<w, t, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _conditional<w::template type<t, tli>::value, _tetter<tlos..., t, tli, tlis...>, typename _sort_impl_impl<w, t, _tetter<tlis...>, _tetter<tlos..., tli>>::type>::type;
};

template <typename w, typename t, typename... tlos>
struct _sort_impl_impl<w, t, _tetter<>, _tetter<tlos...>>
{
	using type = _tetter<tlos..., t>;
};
template <typename w, typename tli, typename tlo>
struct _sort_impl;

template <typename w, typename tli, typename... tlis, typename... tlos> 
struct _sort_impl<w, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _sort_impl<w, _tetter<tlis...>, typename _sort_impl_impl<w, tli, _tetter<tlos...>, _tetter<>>::type>::type;
};

template <typename w, typename... tlos> 
struct _sort_impl<w, _tetter<>, _tetter<tlos...>>
{
	using type = _tetter<tlos...>;
};

template <typename w, typename t, _size_t i, typename ats, typename enable, typename... args>
struct _invoke_impl;

template <typename w, typename t, _size_t i, typename ats, typename... args>
struct _invoke_impl<w, t, i, ats, _enable_t<decltype(typename w::template type<t, i, ats>{}(_declval<args>()...))>, args...>
{
	inline static constexpr _tetter_decltype(typename w::template type<t, i, ats>{}(_declval<args>()...)) call(args&&... _args)
	{
		return typename w::template type<t, i, ats>{}(static_cast<args&&>(_args)...);
	}
};

#if _tetter_static_call_operator
template <typename w, typename t, _size_t i, typename ats, typename enable, typename... args>
struct _invoke_static_impl
{
	inline static constexpr _tetter_decltype(_invoke_impl<w, t, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call(args&&... _args)
	{
		return _invoke_impl<w, t, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...);
	}
};

template <typename w, typename t, _size_t i, typename ats, typename... args>
struct _invoke_static_impl<w, t, i, ats, _enable_t<decltype(w::template type<t, i, ats>::operator()(_declval<args>()...))>, args...>
{
	inline static constexpr _tetter_decltype(w::template type<t, i, ats>::operator()(_declval<args>()...)) call(args&&... _args)
	{
		return w::template type<t, i, ats>::operator()(static_cast<args&&>(_args)...);
	}
};
#endif

template <typename w, typename t, _size_t i, typename ats, typename enable, typename... args>
struct _invoke
{
#if _tetter_static_call_operator
	inline static constexpr _tetter_decltype(_invoke_static_impl<w, t, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call(args&&... _args)
	{
		return _invoke_static_impl<w, t, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...);
	}
#else
	inline static constexpr _tetter_decltype(_invoke_impl<w, t, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call(args&&... _args)
	{
		return _invoke_impl<w, t, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...);
	}
#endif
};

template <typename w, typename t, _size_t i, typename ats, typename... args>
struct _invoke<w, t, i, ats, _enable_t<decltype(w::template type<t, i, ats>::call(_declval<args>()...))>, args...>
{
	inline static constexpr _tetter_decltype(w::template type<t, i, ats>::call(_declval<args>()...)) call(args&&... _args)
	{
		return w::template type<t, i, ats>::call(static_cast<args&&>(_args)...);
	}
};

template <_size_t i, typename w, typename rts, typename ats, typename... args>
struct _call_impl;

template <_size_t i, typename w, typename rt, typename srt, typename... rts, typename ats, typename... args>
struct _call_impl<i, w, _tetter<rt, srt, rts...>, ats, args...>
{
	inline static constexpr void call(args&&... _args)
	{
		_invoke<w, rt, i, ats, _enable_t<>, args&...>::call(static_cast<args&>(_args)...); 
		_call_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call(static_cast<args&&>(_args)...);
	}
};

template <_size_t i, typename w, typename rt, typename ats, typename... args>
struct _call_impl<i, w, _tetter<rt>, ats, args...>
{
	inline static constexpr void call(args&&... _args)
	{
		_invoke<w, rt, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...); 
	}
};

template <_size_t i, typename w, typename ats, typename... args>
struct _call_impl<i, w, _tetter<>, ats, args...>
{
	inline static constexpr void call(args&&... _args)
	{}
};

template <_size_t i, typename w, typename rts, typename it, typename ats, typename... args>
struct _call_pipe_impl;

template <_size_t i, typename w, typename rt, typename srt, typename... rts, typename it, typename ats, typename... args>
struct _call_pipe_impl<i, w, _tetter<rt, srt, rts...>, it, ats, args...>
{
	inline static constexpr _tetter_decltype(_call_pipe_impl<i+1, w, _tetter<srt, rts...>, decltype(_invoke<w, rt, i, ats, _enable_t<>, it, args&...>::call(_declval<it>(), _declval<args&>()...)), ats, args...>::call_pipe(
	                                                                              _declval<decltype(_invoke<w, rt, i, ats, _enable_t<>, it, args&...>::call(_declval<it>(), _declval<args&>()...))>(), _declval<args>()...))
		call_pipe(it&& _it, args&&... _args)
	{
		return _call_pipe_impl<i+1, w, _tetter<srt, rts...>, decltype(_invoke<w, rt, i, ats, _enable_t<>, it, args&...>::call(_declval<it>(), _declval<args&>()...)), ats, args...>::call_pipe(
		                                                              _invoke<w, rt, i, ats, _enable_t<>, it, args&...>::call(static_cast<it&&>(_it), static_cast<args&>(_args)...), static_cast<args&&>(_args)...);
	}
};

template <_size_t i, typename w, typename rt, typename it, typename ats, typename... args>
struct _call_pipe_impl<i, w, _tetter<rt>, it, ats, args...>
{
	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, it, args...>::call(_declval<it>(), _declval<args>()...)) call_pipe(it&& _it, args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, it, args...>::call(static_cast<it&&>(_it), static_cast<args&&>(_args)...); 
	}
};

template <_size_t i, typename w, typename it, typename ats, typename... args>
struct _call_pipe_impl<i, w, _tetter<>, it, ats, args...>
{
	inline static constexpr it&& call_pipe(it&& _it, args&&... _args)
	{
		return static_cast<it&&>(_it);
	}
};

template <_size_t i, typename w, typename rts, typename ats, typename... args>
struct _call_bool_impl;

template <_size_t i, typename w, typename rt, typename srt, typename... rts, typename ats, typename... args>
struct _call_bool_impl<i, w, _tetter<rt, srt, rts...>, ats, args...>
{
	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args&...>::call(_declval<args&>()...) && 
		_call_bool_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_all(_declval<args>()...)) call_all(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args&...>::call(static_cast<args&>(_args)...) && 
			_call_bool_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_all(static_cast<args&&>(_args)...);
	}

	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args&...>::call(_declval<args&>()...) || 
		_call_bool_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_any(_declval<args>()...)) call_any(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args&...>::call(static_cast<args&>(_args)...) || 
			_call_bool_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_any(static_cast<args&&>(_args)...);
	}
};

template <_size_t i, typename w, typename rt, typename ats, typename... args>
struct _call_bool_impl<i, w, _tetter<rt>, ats, args...>
{
	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call_all(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...); 
	}

	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call_any(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...); 
	}
};

template <_size_t i, typename w, typename ats, typename... args>
struct _call_bool_impl<i, w, _tetter<>, ats, args...>
{
	inline static constexpr bool call_all(args&&... _args)
	{
		return true;
	}

	inline static constexpr bool call_any(args&&... _args)
	{
		return false;
	}
};

template <_size_t i, typename w, typename rts, typename ats, typename... args>
struct _call_int_impl;

template <_size_t i, typename w, typename rt, typename srt, typename... rts, typename ats, typename... args>
struct _call_int_impl<i, w, _tetter<rt, srt, rts...>, ats, args...>
{
	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args&...>::call(_declval<args&>()...) + 
		_call_int_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_sum(_declval<args>()...)) call_sum(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args&...>::call(static_cast<args&>(_args)...) +
			_call_int_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_sum(static_cast<args&&>(_args)...);
	}

	inline static constexpr decltype(_invoke<w, rt, i, ats, _enable_t<>, args&...>::call(_declval<args&>()...)) call_min(args&&... _args)
	{
		auto this_value = _invoke<w, rt, i, ats, _enable_t<>, args&...>::call(static_cast<args&>(_args)...);
		auto other_value = _call_int_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_min(static_cast<args&&>(_args)...);

		if (this_value < other_value)
			return this_value;
		else
			return other_value;
	}

	inline static constexpr decltype(_invoke<w, rt, i, ats, _enable_t<>, args&...>::call(_declval<args&>()...)) call_max(args&&... _args)
	{
		auto this_value = _invoke<w, rt, i, ats, _enable_t<>, args&...>::call(static_cast<args&>(_args)...);
		auto other_value = _call_int_impl<i+1, w, _tetter<srt, rts...>, ats, args...>::call_max(static_cast<args&&>(_args)...);

		if (this_value > other_value)
			return this_value;
		else
			return other_value;
	}
};

template <_size_t i, typename w, typename rt, typename ats, typename... args>
struct _call_int_impl<i, w, _tetter<rt>, ats, args...>
{
	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call_sum(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...); 
	}

	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call_min(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...); 
	}

	inline static constexpr _tetter_decltype(_invoke<w, rt, i, ats, _enable_t<>, args...>::call(_declval<args>()...)) call_max(args&&... _args)
	{
		return _invoke<w, rt, i, ats, _enable_t<>, args...>::call(static_cast<args&&>(_args)...); 
	}
};

template <_size_t i, typename w, typename ats, typename... args>
struct _call_int_impl<i, w, _tetter<>, ats, args...>
{
	inline static constexpr int call_sum(args&&... _args)
	{
		return 0; 
	}

	inline static constexpr int call_min(args&&... _args)
	{
		return 0; 
	}

	inline static constexpr int call_max(args&&... _args)
	{
		return 0; 
	}
};

template <typename w, typename enable, typename... ts>
struct _try_type_impl
{
	static_assert(_false<w>::value, "to use tetter's ::map<>, iterator must have ::type");
};

template <typename w, typename... ts>
struct _try_type_impl<w, _enable_t<typename _type_impl<0, w, _tetter<ts...>, _tetter<ts...>, _tetter<>>::type>, ts...>
{
	using type = typename _type_impl<0, w, _tetter<ts...>, _tetter<ts...>, _tetter<>>::type;
};

template <typename w, typename enable, typename... ts>
struct _try_bool_value_impl
{
	static_assert(_false<w>::value, "to use tetter's ::value<>::all, ::value<>::any and ::value<>::none, iterator must have ::value (of type bool or anything with operators: &&, || and !)");
};

template <typename w, typename... ts>
struct _try_bool_value_impl<w, _enable_t<decltype(_bool_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::all),
                                        decltype(!_bool_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::any)>, ts...>
{
	static constexpr auto all = _bool_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::all;
	static constexpr auto any = _bool_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::any;
	static constexpr auto none = !any;
};

template <typename w, typename enable, typename... ts>
struct _try_int_value_impl
{
	static_assert(_false<w>::value, "to use tetter's ::value<>::sum, ::value<>::avg, ::value<>::min and ::value<>::max, iterator must have ::value (of type int or anything with operators: +, /, < and >)");
};

template <typename w, typename... ts>
struct _try_int_value_impl<w, _enable_t<decltype(_int_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::sum / _declval<_size_t>()),
                                        decltype(_int_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::min),
                                        decltype(_int_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::max)>, ts...>
{
	static constexpr auto sum = _int_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::sum;
	static constexpr auto avg = sum / static_cast<_size_t>(sizeof...(ts) > 0 ? sizeof...(ts) : 1);
	static constexpr auto min = _int_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::min;
	static constexpr auto max = _int_value_impl<0, w, _tetter<ts...>, _tetter<ts...>>::max;
};

template <typename w, typename enable, typename... ts>
struct _try_value_impl : public _try_bool_value_impl<w, _enable_t<>, ts...>, public _try_int_value_impl<w, _enable_t<>, ts...> {};

template <typename w, typename enable, typename... ts>
struct _try_find_impl
{
	static_assert(_false<w>::value, "to use tetter's ::find, iterator must have ::value (of type bool or anything boollike)");
};

template <typename w, typename... ts>
struct _try_find_impl<w, _enable_t<decltype(_find_impl<0, w, _tetter<ts...>, _tetter<ts...>>::index)>, ts...>
{
private:
	using result = _find_impl<0, w, _tetter<ts...>, _tetter<ts...>>;

public:
	static constexpr _size_t index = result::index;
	static constexpr bool value = index != sizeof...(ts);
	using type = typename result::type;
};

template <typename w, typename enable, typename... ts>
struct _try_filter_impl
{
	static_assert(_false<w>::value, "to use tetter's ::filter, iterator must have ::value (of type bool or anything boollike)");
};

template <typename w, typename... ts>
struct _try_filter_impl<w, _enable_t<typename _filter_impl<0, w, _tetter<ts...>, _tetter<>, _tetter<ts...>>::type>, ts...>
{
	using type = typename _filter_impl<0, w, _tetter<ts...>, _tetter<>, _tetter<ts...>>::type;
};

template <typename w, typename enable, typename... ts>
struct _try_sort_impl
{
	static_assert(_false<w>::value, "to use tetter's ::sort, sorter must have ::value (of type bool or anything boollike)");
};

template <typename w, typename... ts>
struct _try_sort_impl<w, _enable_t<typename _sort_impl<w, _tetter<ts...>, _tetter<>>::type>, ts...>
{
	using type = typename _sort_impl<w, _tetter<ts...>, _tetter<>>::type;
};

template <typename w, typename args, typename enable, typename... ts>
struct _try_call_impl
{
	static_assert(_false<w>::value, "to use tetter's ::call, iterator must have ::call or .operator() (with default constructor)");
};

template <typename w, typename... args, typename... ts>
struct _try_call_impl<w, _tetter<args...>, _enable_t<decltype(_call_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call(_declval<args>()...))>, ts...>
{
	inline static constexpr void call(args&&... _args)
	{
		_call_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call(static_cast<args&&>(_args)...);
	}
};

template <typename w, typename it, typename args, typename enable, typename... ts>
struct _try_call_pipe_impl
{
	static_assert(_false<w>::value, "to use tetter's ::call_pipe, iterator must have ::call or .operator() (with default constructor)");
};

template <typename w, typename it, typename... args, typename... ts>
struct _try_call_pipe_impl<w, it, _tetter<args...>, _enable_t<decltype(_call_pipe_impl<0, w, _tetter<ts...>, it, _tetter<ts...>, args...>::call_pipe(_declval<it>(), _declval<args>()...))>, ts...>
{
	inline static constexpr _tetter_decltype(_call_pipe_impl<0, w, _tetter<ts...>, it, _tetter<ts...>, args...>::call_pipe(_declval<it>(), _declval<args>()...)) call_pipe(it&& _iv, args&&... _args)
	{
		return _call_pipe_impl<0, w, _tetter<ts...>, it, _tetter<ts...>, args...>::call_pipe(static_cast<it&&>(_iv), static_cast<args&&>(_args)...);
	}
};

template <typename w, typename args, typename enable, typename... ts>
struct _try_call_bool_impl
{
	static_assert(_false<w>::value, "to use tetter's ::call_all, ::call_any and ::call_none, iterator must have ::call or .operator() (with default constructor) which returns bool or anything with operators: &&, || and !");
};

template <typename w, typename... args, typename... ts>
struct _try_call_bool_impl<w, _tetter<args...>, _enable_t<decltype(_call_bool_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_all(_declval<args>()...)),
                                                          decltype(!_call_bool_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_any(_declval<args>()...))>, ts...>
{
	inline static constexpr _tetter_decltype(_call_bool_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_all(_declval<args>()...)) call_all(args&&... _args)
	{
		return _call_bool_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_all(static_cast<args&&>(_args)...);
	}

	inline static constexpr _tetter_decltype(_call_bool_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_any(_declval<args>()...)) call_any(args&&... _args)
	{
		return _call_bool_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_any(static_cast<args&&>(_args)...);
	}

	inline static constexpr _tetter_decltype(!call_any(_declval<args>()...)) call_none(args&&... _args)
	{
		return !call_any(static_cast<args&&>(_args)...);
	}
};

template <typename w, typename args, typename enable, typename... ts>
struct _try_call_int_impl
{
	static_assert(_false<w>::value, "to use tetter's ::call_sum, ::call_avg, ::call_min and ::call_min, iterator must have ::call or .operator() (with default constructor) which returns int or anything with operators: +, /, < and >");
};

template <typename w, typename... args, typename... ts>
struct _try_call_int_impl<w, _tetter<args...>, _enable_t<decltype(_call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_sum(_declval<args>()...) / _declval<_size_t>()),
                                                         decltype(_call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_min(_declval<args>()...)),
														 decltype(_call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_max(_declval<args>()...))>, ts...>
{
	inline static constexpr _tetter_decltype(_call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_sum(_declval<args>()...)) call_sum(args&&... _args)
	{
		return _call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_sum(static_cast<args&&>(_args)...);
	}

	inline static constexpr _tetter_decltype(call_sum(_declval<args>()...) / _declval<_size_t>()) call_avg(args&&... _args)
	{
		return call_sum(static_cast<args&&>(_args)...) / static_cast<_size_t>(sizeof...(ts) > 0 ? sizeof...(ts) : 1);
	}

	inline static constexpr _tetter_decltype(_call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_min(_declval<args>()...)) call_min(args&&... _args)
	{
		return _call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_min(static_cast<args&&>(_args)...);
	}

	inline static constexpr _tetter_decltype(_call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_max(_declval<args>()...)) call_max(args&&... _args)
	{
		return _call_int_impl<0, w, _tetter<ts...>, _tetter<ts...>, args...>::call_max(static_cast<args&&>(_args)...);
	}
};

template <bool invert, typename... ts>
struct _is_any_of
{
	template <typename t, _size_t i, typename ats>
	struct type
	{
		template <typename _t, _size_t>
		struct iterator_impl
		{
			static constexpr bool value = _is_same<_t, t>::value;
		};

		static constexpr bool value = invert ^ _try_value_impl<_wrapper<iterator_impl>, _enable_t<>, ts...>::any;
	};
};

/* template <typename w, typename ts>
struct _duplicats_impl_impl;

template <typename w, typename... ts>
struct _duplicats_impl_impl<w, _tetter<ts...>>
{
	static constexpr bool value = _try_find_impl<w, _enable_t<>, ts...>::value;
};

template <typename t, _size_t i, typename... ts>
struct _duplicats_impl
{
	static constexpr bool value = !_duplicats_impl_impl<_is_any_of<false, t>, typename _pop_front_impl<i+1, ts...>::type>::value;  
}; */

template <typename t, typename ts>
struct _join_impl_impl
{
	static_assert(_false<t>::value, "invalid use, tetter's ::join<t> requires t to have template args");
};

template <template <typename...> typename t, typename... jts, typename... ts>
struct _join_impl_impl<t<jts...>, _tetter<ts...>>
{
	using type = _tetter<ts..., jts...>;
};

template <typename gts, typename ts>
struct _join_impl;

template <typename gt, typename... gts, typename... ts>
struct _join_impl<_tetter<gt, gts...>, _tetter<ts...>>
{
	using type = typename _join_impl<_tetter<gts...>, typename _join_impl_impl<gt, _tetter<ts...>>::type>::type;
};

template <typename... ts>
struct _join_impl<_tetter<>, _tetter<ts...>>
{
	using type = _tetter<ts...>;
};

template <typename t, template <typename...> typename... cts>
struct _concat_impl_impl;

template <typename... ts, template <typename...> typename ct, template <typename...> typename... cts>
struct _concat_impl_impl<ct<ts...>, ct, cts...>
{
	using type = _tetter<ts...>;
};

template <typename t, template <typename...> typename ct, template <typename...> typename... cts>
struct _concat_impl_impl<t, ct, cts...>
{
	using type = typename _concat_impl_impl<t, cts...>::type;
};

template <typename t>
struct _concat_impl_impl<t>
{
	using type = t;
};

template <typename tli, typename... tlos>
struct _concat_impl_impl_impl
{
	using type = _tetter<tlos..., tli>;
};

template <typename... tlis, typename... tlos>
struct _concat_impl_impl_impl<_tetter<tlis...>, tlos...>
{
	using type = _tetter<tlos..., tlis...>;
};

template <typename tli, typename tlo, template <typename...> typename... cts>
struct _concat_impl;

template <typename tli, typename... tlis, typename... tlos, template <typename...> typename... cts>
struct _concat_impl<_tetter<tli, tlis...>, _tetter<tlos...>, cts...>
{
	using type = typename _concat_impl<_tetter<tlis...>, typename _concat_impl_impl_impl<typename _concat_impl_impl<tli, cts...>::type, tlos...>::type, cts...>::type;
};

template <typename... tlos, template <typename...> typename... cts>
struct _concat_impl<_tetter<>, _tetter<tlos...>, cts...>
{
	using type = _tetter<tlos...>;
};

template <typename t>
struct _concat_any_impl_impl
{
	using type = t;
};

template <template <typename...> typename ct, typename... ts>
struct _concat_any_impl_impl<ct<ts...>>
{
	using type = _tetter<ts...>;
};

template <typename tli, typename tlo>
struct _concat_any_impl;

template <typename tli, typename... tlis, typename... tlos>
struct _concat_any_impl<_tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _concat_any_impl<_tetter<tlis...>, typename _concat_impl_impl_impl<typename _concat_any_impl_impl<tli>::type, tlos...>::type>::type;
};

template <typename... tlos>
struct _concat_any_impl<_tetter<>, _tetter<tlos...>>
{
	using type = _tetter<tlos...>;
};

template <_size_t i, typename tli, typename tlo, typename... ts>
struct _insert_impl;

template <_size_t i, typename tli, typename... tlis, typename... tlos, typename... ts>
struct _insert_impl<i, _tetter<tli, tlis...>, _tetter<tlos...>, ts...>
{
	using type = typename _insert_impl<i-1, _tetter<tlis...>, _tetter<tlos..., tli>, ts...>::type;
};

template <_size_t i, typename... tlos, typename... ts>
struct _insert_impl<i, _tetter<>, _tetter<tlos...>, ts...>
{
	using type = _tetter<tlos..., ts...>;
};

template <typename tli, typename... tlis, typename... tlos, typename... ts>
struct _insert_impl<0, _tetter<tli, tlis...>, _tetter<tlos...>, ts...>
{
	using type = _tetter<tlos..., ts..., tli, tlis...>;
};

template <typename... tlos, typename... ts>
struct _insert_impl<0, _tetter<>, _tetter<tlos...>, ts...>
{
	using type = _tetter<tlos..., ts...>;
};

template <_size_t i, _size_t c, typename tli, typename tlo>
struct _remove_impl;

template <_size_t i, _size_t c, typename tli, typename... tlis, typename... tlos>
struct _remove_impl<i, c, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _remove_impl<i-1, c, _tetter<tlis...>, _tetter<tlos..., tli>>::type;
};

template <_size_t c, typename tli, typename... tlis, typename... tlos>
struct _remove_impl<0, c, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _join_tetter<typename _pop_front_impl<c, tli, tlis...>::type, _tetter<tlos...>>::type;
};

template <_size_t i, _size_t c, typename... tlos>
struct _remove_impl<i, c, _tetter<>, _tetter<tlos...>>
{
	using type = _tetter<tlos...>;
};

template <_size_t i, typename t, typename tli, typename tlo>
struct _replace_impl;

template <_size_t i, typename t, typename tli, typename... tlis, typename... tlos>
struct _replace_impl<i, t, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = typename _replace_impl<i-1, t, _tetter<tlis...>, _tetter<tlos..., tli>>::type;
};

template <typename t, typename tli, typename... tlis, typename... tlos>
struct _replace_impl<0, t, _tetter<tli, tlis...>, _tetter<tlos...>>
{
	using type = _tetter<tlos..., t, tlis...>;
};

template <_size_t i, typename t, typename... tlos>
struct _replace_impl<i, t, _tetter<>, _tetter<tlos...>>
{
	static_assert(_false<t>::value, "out of bounds");
};

#if _tetter_generic_lambdas || _tetter_unevaluated_lambda

template <typename lt, typename t, _size_t i, typename ats, typename enable, typename... args>
struct _lambda_invoke_impl
{
	static_assert(_false<lt>::value, "lambda iterator must have operator()<t, i, optional_all_types...> or operator()<t, optional_all_types...> or operator<i, optional_all_types...>()");
};

template <typename lt, typename t, _size_t i, typename ats, typename... args>
struct _lambda_invoke_impl<lt, t, i, ats, _enable_t<decltype(_declval<lt>().template operator()<t>(_declval<args>()...))>, args...> 
{
	using result_type = decltype(_declval<lt>().template operator()<t>(_declval<args>()...));

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return static_cast<lt&&>(l).template operator()<t>(static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename t, _size_t i, typename ats, typename... args>
struct _lambda_invoke_impl<lt, t, i, ats, _enable_t<decltype(_declval<lt>().template operator()<i>(_declval<args>()...))>, args...> 
{
	using result_type = decltype(_declval<lt>().template operator()<i>(_declval<args>()...));

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return static_cast<lt&&>(l).template operator()<i>(static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename t, _size_t i, typename ats, typename... args>
struct _lambda_invoke_impl<lt, t, i, ats, _enable_t<decltype(_declval<lt>().template operator()<t, i>(_declval<args>()...))>, args...> 
{
	using result_type = decltype(_declval<lt>().template operator()<t, i>(_declval<args>()...));

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return static_cast<lt&&>(l).template operator()<t, i>(static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename t, _size_t i, typename ats, typename enable, typename... args>
struct _lambda_invoke
{
	using result_type = typename _lambda_invoke_impl<lt, t, i, ats, _enable_t<>, args...>::result_type;

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return _lambda_invoke_impl<lt, t, i, ats, _enable_t<>, args...>::call(static_cast<lt&&>(l), static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename t, _size_t i, typename... ats, typename... args>
struct _lambda_invoke<lt, t, i, _tetter<ats...>, _enable_t<decltype(_declval<lt>().template operator()<t, ats...>(_declval<args>()...))>, args...> 
{
	using result_type = decltype(_declval<lt>().template operator()<t, ats...>(_declval<args>()...));

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return static_cast<lt&&>(l).template operator()<t, ats...>(static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename t, _size_t i, typename... ats, typename... args>
struct _lambda_invoke<lt, t, i, _tetter<ats...>, _enable_t<decltype(_declval<lt>().template operator()<i, ats...>(_declval<args>()...))>, args...> 
{
	using result_type = decltype(_declval<lt>().template operator()<i, ats...>(_declval<args>()...));

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return static_cast<lt&&>(l).template operator()<i, ats...>(static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename t, _size_t i, typename... ats, typename... args>
struct _lambda_invoke<lt, t, i, _tetter<ats...>, _enable_t<decltype(_declval<lt>().template operator()<t, i, ats...>(_declval<args>()...))>, args...> 
{
	using result_type = decltype(_declval<lt>().template operator()<t, i, ats...>(_declval<args>()...));

	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return static_cast<lt&&>(l).template operator()<t, i, ats...>(static_cast<args&&>(_args)...);
	}
};

template <typename lt, typename lft, typename rit, typename enable>
struct _lambda_invoke_sort
{
	static_assert(_false<lt>::value, "lambda sorter must have operator()<lt, rt>");
};

template <typename lt, typename lft, typename rit>
struct _lambda_invoke_sort<lt, lft, rit, _enable_t<decltype(_declval<lt>().template operator()<lft, rit>())>>
{
	using result_type = decltype(_declval<lt>().template operator()<lft, rit>());

	inline static constexpr auto call(lt&& l)
	{
		return static_cast<lt&&>(l).template operator()<lft, rit>();
	}
};

#endif

#if _tetter_generic_lambdas

template <typename t, _size_t i, typename... ts>
struct _lambda_impl
{
	template <typename lt, typename... args>
	inline static constexpr auto call(lt&& l, args&&... _args)
	{
		return _lambda_invoke<lt, t, i, _tetter<ts...>, _enable_t<>, args...>::call(static_cast<lt&&>(l), static_cast<args&&>(_args)...);
	}
};

template <typename t, _size_t i, typename... ts>
struct _lambda_pipe_impl
{
	template <typename ft, typename lt, typename... args>
	inline static constexpr auto call(ft&& f, lt&& l, args&&... _args)
	{
		return _lambda_invoke<lt, t, i, _tetter<ts...>, _enable_t<>, ft, args...>::call(static_cast<lt&&>(l), static_cast<ft&&>(f), static_cast<args&&>(_args)...);
	}
};

#endif

#if _tetter_unevaluated_lambda

template <auto lambda>
struct _lambda_v_wrapper
{
	template <typename t, _size_t i, typename ats>
	struct type
	{
		static constexpr auto value = _lambda_invoke<const decltype(lambda)&, t, i, ats, _enable_t<>>::call(lambda);
	};
};

template <auto lambda>
struct _lambda_t_wrapper
{
	template <typename t, _size_t i, typename ats>
	struct type_impl
	{
		using type = _lambda_invoke<const decltype(lambda)&, t, i, ats, _enable_t<>>::result_type;
	};

	template <typename t, _size_t i, typename ats>
	using type = type_impl<t, i, ats>;
};

template <auto lambda>
struct _lambda_sort_wrapper
{
	template <typename lt, typename rt>
	struct type
	{
		static constexpr auto value = _lambda_invoke_sort<const decltype(lambda)&, lt, rt, _enable_t<>>::call(lambda);
	};
};

#endif

template <typename t>
struct _from_impl
{
	static_assert(_false<t>::value, "tetter_from requires generic type");
};

template <template <typename...> typename t, typename... ts>
struct _from_impl<t<ts...>>
{
	using type = _tetter<ts...>;
};

template <typename t, typename enable>
struct _from_args_impl
{
	static_assert(_false<t>::value, "tetter_from_args requires callable type");
};

template <typename t, typename... ts>
struct _from_args_impl<t(*)(ts...), _enable_t<>>
{
	using type = _tetter<ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...), _enable_t<>>
{
	using type = _tetter<ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) const, _enable_t<>>
{
	using type = _tetter<const ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) volatile, _enable_t<>>
{
	using type = _tetter<volatile ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) const volatile, _enable_t<>>
{
	using type = _tetter<const volatile ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) &, _enable_t<>>
{
	using type = _tetter<ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) const &, _enable_t<>>
{
	using type = _tetter<const ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) volatile &, _enable_t<>>
{
	using type = _tetter<volatile ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) const volatile &, _enable_t<>>
{
	using type = _tetter<const volatile ct&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) &&, _enable_t<>>
{
	using type = _tetter<ct&&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) const &&, _enable_t<>>
{
	using type = _tetter<const ct&&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) volatile &&, _enable_t<>>
{
	using type = _tetter<volatile ct&&, ts...>; 
};

template <typename t, typename ct, typename... ts>
struct _from_args_impl<t(ct::*)(ts...) const volatile &&, _enable_t<>>
{
	using type = _tetter<const volatile ct&&, ts...>; 
};

template <typename t>
struct _from_args_impl<t, _enable_t<decltype(&t::operator())>>
{
	using type = typename _from_args_impl<decltype(&t::operator()), _enable_t<>>::type; 
};

template <_size_t i> 
struct _size_t_sequence
{
	static constexpr _size_t value = i;
};

template <_size_t i, template <_size_t> typename t, typename ts> 
struct _tetter_sequence_impl;

template <_size_t i, template <_size_t> typename t, typename... ts> 
struct _tetter_sequence_impl<i, t, _tetter<ts...>>
{
	using type = typename _tetter_sequence_impl<i-1, t, _tetter<t<i-1>, ts...>>::type; 
};

template <template <_size_t> typename t, typename... ts> 
struct _tetter_sequence_impl<0, t, _tetter<ts...>>
{
	using type = _tetter<ts...>; 
};

template <typename... ts>
struct _tetter_impl
{
	using front = typename _front_impl<ts...>::type;
	using back = typename _back_impl<ts...>::type;
};

template <>
struct _tetter_impl<>
{
};

} } // _tetter

template <typename... ts>
struct tetter : public _tetter::_tetter_impl<ts...>
{
	static constexpr _tetter::_size_t count = sizeof...(ts);

	template <typename t>
	using front_or = typename _tetter::_front_impl<ts..., t>::type;
	template <typename t>
	using back_or = typename _tetter::_back_impl<t, ts...>::type;

	template <typename... types>
	using push_front = tetter<types..., ts...>; 
	template <typename... types>
	using push_back = tetter<ts..., types...>;

	template <_tetter::_size_t index, typename... types>
	using insert = typename _tetter::_insert_impl<index, _tetter::_tetter<ts...>, _tetter::_tetter<>, types...>::type::template cast<tetter>;

	template <_tetter::_size_t index, _tetter::_size_t amount = 1>
	using remove = typename _tetter::_remove_impl<index, amount, _tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>;

	template <_tetter::_size_t index, typename type>
	using replace = typename _tetter::_replace_impl<index, type, _tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>;

	template <typename... generic_types>
	using join = typename _tetter::_join_impl<_tetter::_tetter<generic_types...>, _tetter::_tetter<ts...>>::type::template cast<tetter>;

	using reverse = typename _tetter::_reverse_impl<_tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>;

	template <_tetter::_size_t amount>
	using pop_front_n = typename _tetter::_pop_front_impl<amount, ts...>::type::template cast<tetter>;
	template <_tetter::_size_t amount>
	using pop_back_n = typename _tetter::_conditional<(count > amount), typename _tetter::_pop_back_impl<count - amount, _tetter::_tetter<ts...>, _tetter::_tetter<>>::type, _tetter::_tetter<>>::type::template cast<tetter>;

	using pop_front = pop_front_n<1>;
	using pop_back = pop_back_n<1>;

	template <_tetter::_size_t index>
	using get = typename _tetter::_get_impl<index, ts...>::type;

	template <_tetter::_size_t index, _tetter::_size_t count>
	using slice = typename _tetter::_slice_impl<index, count, _tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>;

	template <template <typename...> typename to_type>
	using cast = to_type<ts...>;

	template <template <typename...> typename... types_to_concat> 
	using concat = typename _tetter::_concat_impl<_tetter::_tetter<ts...>, _tetter::_tetter<>, types_to_concat...>::type::template cast<tetter>;

	using concat_a = typename _tetter::_concat_any_impl<_tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>;

	// a, a, b, b, c, c
	template <_tetter::_size_t times>
	using repeat = typename _tetter::_repeat_impl<times + 1, _tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>;

	template <_tetter::_size_t times>
	using repeat_m = typename _tetter::_conditional<(times > 0), typename _tetter::_repeat_impl<times == 0 ? 1 : times, _tetter::_tetter<ts...>, _tetter::_tetter<>>::type::template cast<tetter>, tetter<>>::type;

	// a, b, c, a, b, c
	template <_tetter::_size_t times>
	using copy = typename _tetter::_copy_impl<times, _tetter::_tetter<ts...>, _tetter::_tetter<ts...>>::type::template cast<tetter>;

	template <_tetter::_size_t times>
	using copy_m = typename _tetter::_conditional<(times > 0), typename _tetter::_copy_impl<times == 0 ? 0 : times - 1, _tetter::_tetter<ts...>, _tetter::_tetter<ts...>>::type::template cast<tetter>, tetter<>>::type;

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator>
	using find = _tetter::_try_find_impl<_tetter::_wrapper<iterator>, _tetter::_enable_t<>, ts...>;

	template <typename... types_to_find>
	using find_t = _tetter::_try_find_impl<_tetter::_is_any_of<false, types_to_find...>, _tetter::_enable_t<>, ts...>;

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator>
	using filter = typename _tetter::_try_filter_impl<_tetter::_wrapper<iterator>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <typename... types_to_remove>
	using filter_t = typename _tetter::_try_filter_impl<_tetter::_is_any_of<true, types_to_remove...>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <typename... types_to_leave>
	using filter_t_l = typename _tetter::_try_filter_impl<_tetter::_is_any_of<false, types_to_leave...>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <template <typename left_type, typename right_type> typename sorter>
	using sort = typename _tetter::_try_sort_impl<_tetter::_sort_wrapper<sorter>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator>
	using map = typename _tetter::_try_type_impl<_tetter::_wrapper<iterator>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	/* TODO:
	template <_tetter::_size_t times, template<typename _tetter> action>
	using do_n = type_list<...>

	// tetter<...>::do_n_l<8, []<typename t>() -> t::map_l<[]<typename t, size_t>() -> t* {}> {}>;

	template <template<typename _tetter> action, template<typename _tetter> condition = until_type_changed>
	using do_until = type_list<...>
	
	// tetter<...>::do_until_l<[]<typename t>() -> t::concat_a {}>;
	*/

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator>
	using value = _tetter::_try_value_impl<_tetter::_wrapper<iterator>, _tetter::_enable_t<>, ts...>;

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator>
	using value_b = _tetter::_try_bool_value_impl<_tetter::_wrapper<iterator>, _tetter::_enable_t<>, ts...>;

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator>
	using value_i = _tetter::_try_int_value_impl<_tetter::_wrapper<iterator>, _tetter::_enable_t<>, ts...>;

	// TODO: too slow 
	// static constexpr bool has_duplicats = !value<_tetter::_duplicats_impl>::all;
	// using remove_duplicats = typename reverse::template filter<_tetter::_duplicats_impl>::reverse;

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr void call(args&&... _args)
	{
		_tetter::_try_call_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call(static_cast<args&&>(_args)...);
	}

	// it<c, 2>(it<b, 1>(it<a, 0>(inital_value, args...), args...), args...)
	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename inital_type, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_pipe_impl<_tetter::_wrapper<iterator>, inital_type, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_pipe(_tetter::_declval<inital_type>(), _tetter::_declval<args>()...))
		call_pipe(inital_type&& inital_value = inital_type{}, args&&... _args)
	{
		return _tetter::_try_call_pipe_impl<_tetter::_wrapper<iterator>, inital_type, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_pipe(static_cast<inital_type&&>(inital_value), static_cast<args&&>(_args)...);
	}

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_bool_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_all(_tetter::_declval<args>()...))
		call_all(args&&... _args)
	{
		return _tetter::_try_call_bool_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_all(static_cast<args&&>(_args)...);
	}

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_bool_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_any(_tetter::_declval<args>()...))
		call_any(args&&... _args)
	{
		return _tetter::_try_call_bool_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_any(static_cast<args&&>(_args)...);
	}

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_bool_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_none(_tetter::_declval<args>()...)) 
		call_none(args&&... _args)
	{
		return _tetter::_try_call_bool_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_none(static_cast<args&&>(_args)...);
	}

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_sum(_tetter::_declval<args>()...))
		call_sum(args&&... _args)
	{
		return _tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_sum(static_cast<args&&>(_args)...);
	}
	
	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_avg(_tetter::_declval<args>()...))
		call_avg(args&&... _args)
	{
		return _tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_avg(static_cast<args&&>(_args)...);
	}

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_min(_tetter::_declval<args>()...))
		call_min(args&&... _args)
	{
		return _tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_min(static_cast<args&&>(_args)...);
	}

	template <template <typename current_type, _tetter::_size_t current_index, typename... optional_all_types> typename iterator, typename... args>
	inline static constexpr _tetter_decltype(_tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_max(_tetter::_declval<args>()...))
		call_max(args&&... _args)
	{
		return _tetter::_try_call_int_impl<_tetter::_wrapper<iterator>, _tetter::_tetter<args...>, _tetter::_enable_t<>, ts...>::call_max(static_cast<args&&>(_args)...);
	}

#if _tetter_unevaluated_lambda // (C++20)
	template <auto lambda_caster>
	using cast_l = decltype(lambda_caster.template operator()<ts...>());

	template <auto lambda_iterator>
	using find_l = _tetter::_try_find_impl<_tetter::_lambda_v_wrapper<lambda_iterator>, _tetter::_enable_t<>, ts...>;

	template <auto lambda_iterator>
	using filter_l = typename _tetter::_try_filter_impl<_tetter::_lambda_v_wrapper<lambda_iterator>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <auto lambda_sorter>
	using sort_l = typename _tetter::_try_sort_impl<_tetter::_lambda_sort_wrapper<lambda_sorter>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <auto lambda_iterator>
	using map_l = typename _tetter::_try_type_impl<_tetter::_lambda_t_wrapper<lambda_iterator>, _tetter::_enable_t<>, ts...>::type::template cast<tetter>;

	template <auto lambda_iterator>
	using value_l = _tetter::_try_value_impl<_tetter::_lambda_v_wrapper<lambda_iterator>, _tetter::_enable_t<>, ts...>;

	template <auto lambda_iterator>
	using value_b_l = _tetter::_try_bool_value_impl<_tetter::_lambda_v_wrapper<lambda_iterator>, _tetter::_enable_t<>, ts...>;

	template <auto lambda_iterator>
	using value_i_l = _tetter::_try_int_value_impl<_tetter::_lambda_v_wrapper<lambda_iterator>, _tetter::_enable_t<>, ts...>;
#endif

#if _tetter_generic_lambdas // (C++20)
	template <typename lambda, typename... args>
	inline static constexpr void invoke(lambda&& _lambda, args&&... _args)
	{
		call<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename inital_type, typename... args>
	inline static constexpr auto invoke_pipe(lambda&& _lambda, inital_type&& inital_value = inital_type{}, args&&... _args)
	{
		return call_pipe<_tetter::_lambda_pipe_impl>(static_cast<inital_type&&>(inital_value), static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_all(lambda&& _lambda, args&&... _args)
	{
		return call_all<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_any(lambda&& _lambda, args&&... _args)
	{
		return call_any<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_none(lambda&& _lambda, args&&... _args)
	{
		return call_none<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_sum(lambda&& _lambda, args&&... _args)
	{
		return call_sum<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_avg(lambda&& _lambda, args&&... _args)
	{
		return call_avg<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_min(lambda&& _lambda, args&&... _args)
	{
		return call_min<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}

	template <typename lambda, typename... args>
	inline static constexpr auto invoke_max(lambda&& _lambda, args&&... _args)
	{
		return call_max<_tetter::_lambda_impl>(static_cast<lambda&&>(_lambda), static_cast<args&&>(_args)...);
	}
#endif
};

template <typename t>
using tetter_from = typename _tetter::_from_impl<typename _tetter::_clean<t>::type>::type::template cast<tetter>;

template <typename function>
using tetter_from_args = typename _tetter::_from_args_impl<typename _tetter::_clean<function>::type, _tetter::_enable_t<>>::type::template cast<tetter>;

template <_tetter::_size_t n, template <_tetter::_size_t> typename sequence_type = _tetter::_size_t_sequence>
using tetter_sequence = typename _tetter::_tetter_sequence_impl<n, sequence_type, _tetter::_tetter<>>::type::template cast<tetter>;

template <typename t>
struct is_tetter
{
	static constexpr bool value = false;
};

template <typename... ts>
struct is_tetter<tetter<ts...>>
{
	static constexpr bool value = true;
};

#if _tetter_variable_templates // (C++14)
template <typename t>
inline static constexpr bool is_tetter_v = is_tetter<t>::value;
#endif

#if _tetter_concepts // (C++20)
template <typename t>
concept tetter_c = is_tetter_v<t>;
#endif

#endif
