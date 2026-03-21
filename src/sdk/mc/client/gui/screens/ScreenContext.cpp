#include "ScreenContext.hpp"

Tessellator *ScreenContext::tessellator() {
    return hat::member_at<Tessellator*>(this, AddressResolver::get("ScreenContext::tessellator"));
}