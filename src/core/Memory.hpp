#pragma once
#include <libhat/access.hpp>

#define $field(type, name, offset) \
private: \
	__forceinline type& get_##name() { \
		return hat::member_at<type>(this, offset); \
	} \
	__forceinline void set_##name(const type& value) { \
		hat::member_at<type>(this, offset) = value; \
	} \
public: \
	__declspec(property(get = get_##name, put = set_##name)) type name;

namespace Memory {

}