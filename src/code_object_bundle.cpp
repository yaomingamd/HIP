#include "../include/hip/hcc_detail/code_object_bundle.hpp"

#include <hsa/hsa.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

// CREATORS
hip_impl::Bundled_code_header::Bundled_code_header(const vector<char>& x)
    : Bundled_code_header{x.cbegin(), x.cend()} {}

hip_impl::Bundled_code_header::Bundled_code_header(
    const void* p) {  // This is a pretty terrible interface, useful only because
    // hipLoadModuleData is so poorly specified (for no fault of its own).
    if (!p) return;

    if (!valid(*static_cast<const Bundled_code_header*>(p))) return;
    auto ph = static_cast<const Header_*>(p);

    size_t sz = sizeof(Header_) + ph->bundle_cnt_ * sizeof(Bundled_code::Header);
    auto pb = static_cast<const char*>(p) + sizeof(Header_);
    auto n = ph->bundle_cnt_;
    while (n--) {
        sz += reinterpret_cast<const Bundled_code::Header*>(pb)->bundle_sz;
        pb += sizeof(Bundled_code::Header);
    }

    read(static_cast<const char*>(p), static_cast<const char*>(p) + sz, *this);
}
