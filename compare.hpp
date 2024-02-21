#pragma once

#if !defined(__cpp_impl_three_way_comparison) || __cpp_impl_three_way_comparison < 201907L
namespace GP {
namespace impl {

using __cmp_type = signed char;
enum struct __Ord: __cmp_type {
	less = -1,
	equivalent = 0, // not used
	greater = 1,
};

enum struct __Ncmp: __cmp_type {
	unordered = 2,
};

struct __unspec {
	constexpr __unspec(__unspec *) noexcept {}
};

class partial_ordering {
private:
	__cmp_type value;

public:
	static const partial_ordering less;
	static const partial_ordering greater;
	static const partial_ordering equivalent;
	static const partial_ordering unordered;

private:
	constexpr partial_ordering(__Ord v) noexcept : value(__cmp_type(v)) {}
	constexpr partial_ordering(__Ncmp v) noexcept : value(__cmp_type(v)) {}

friend constexpr bool operator<(partial_ordering, __unspec) noexcept;
friend constexpr bool operator>(partial_ordering, __unspec) noexcept;
friend constexpr bool operator<=(partial_ordering, __unspec) noexcept;
friend constexpr bool operator>=(partial_ordering, __unspec) noexcept;
friend constexpr bool operator==(partial_ordering, __unspec) noexcept;
friend constexpr bool operator==(partial_ordering, partial_ordering) noexcept;
friend constexpr bool operator<(__unspec, partial_ordering) noexcept;
friend constexpr bool operator>(__unspec, partial_ordering) noexcept;
friend constexpr bool operator<=(__unspec, partial_ordering) noexcept;
friend constexpr bool operator>=(__unspec, partial_ordering) noexcept;
};

inline constexpr partial_ordering partial_ordering::less(__Ord::less);
inline constexpr partial_ordering partial_ordering::greater(__Ord::greater);
inline constexpr partial_ordering partial_ordering::equivalent(__Ord::equivalent);
inline constexpr partial_ordering partial_ordering::unordered(__Ncmp::unordered);

inline constexpr bool
operator<(partial_ordering o1, __unspec) noexcept {
	// less       < 0: true
	// greater    < 0: false
	// equivalent < 0: false
	// unordered  < 0: false
	return -1 == o1.value;
}

inline constexpr bool
operator>(partial_ordering o1, __unspec) noexcept {
	// less       > 0: false
	// greater    > 0: true
	// equivalent > 0: false
	// unordered  > 0: false
	return 1 == o1.value;
}

inline constexpr bool
operator<=(partial_ordering o1, __unspec) noexcept {
	// less       <= 0: true
	// greater    <= 0: false
	// equivalent <= 0: true
	// unordered  <= 0: false
	return o1.value <= 0;
}

inline constexpr bool
operator>=(partial_ordering o1, __unspec) noexcept {
	// less       >= 0: false
	// greater    >= 0: true
	// equivalent >= 0: true
	// unordered  >= 0: false
	return __cmp_type(o1.value & 1) == o1.value;
}

inline constexpr bool
operator==(partial_ordering o1, __unspec) noexcept {
	// less       == 0: false
	// greater    == 0: false
	// equivalent == 0: true
	// unordered  == 0: false
	return 0 == o1.value;
}

inline constexpr bool
operator==(partial_ordering o1, partial_ordering o2) noexcept {
	return o1.value == o2.value;
}

inline constexpr bool
operator<(__unspec, partial_ordering o2) noexcept {
	// 0 < less:       false
	// 0 < greater:    true
	// 0 < equivalent: false
	// 0 < unordered:  false
	return 1 == o2.value;
}

inline constexpr bool
operator>(__unspec, partial_ordering o2) noexcept {
	// 0 > less:       true
	// 0 > greater:    false
	// 0 > equivalent: false
	// 0 > unordered:  false
	return -1 == o2.value;
}

inline constexpr bool
operator<=(__unspec, partial_ordering o2) noexcept {
	// 0 <= less:       false
	// 0 <= greater:    true
	// 0 <= equivalent: true
	// 0 <= unordered:  false
	return __cmp_type(o2.value & 1) == o2.value;
}

inline constexpr bool
operator>=(__unspec, partial_ordering o2) noexcept {
	// 0 >= less:       true
	// 0 >= greater:    false
	// 0 >= equivalent: true
	// 0 >= unordered:  false
	return o2.value <= 0;
}

}
}
#endif
