#pragma once
#include <cstdlib>
#include <stdexcept>

namespace TX {
	/// <summary>
	/// constexpr string.
	/// https://github.com/boostcon/cppnow_presentations_2012/blob/master/wed/schurr_cpp11_tools_for_class_authors.pdf?raw=true
	/// </summary>
	class ConstString {
	private:
		const char* const p_;
		const std::size_t sz_;
	public:
		template<std::size_t N>
		constexpr ConstString(const char(&a)[N]) : // ctor
			p_(a), sz_(N - 1) {
		}
		constexpr char operator[](std::size_t n) const { // []
			return n < sz_ ? p_[n] :
				throw std::out_of_range("");
		}
		constexpr const char* c_str() const { return p_; }
		constexpr std::size_t size() const { return sz_; } // size()
	};
}
