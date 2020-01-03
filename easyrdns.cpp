/*
 * Copyright (C) 2019 Paul Ciarlo <paul.ciarlo@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "easyrdns.hpp"

#include <cstring>

easyrdns::EasyResolver::EasyResolver() : _lasterr(none)
{

}

easyrdns::EasyResolver::~EasyResolver()
{

}

std::optional<struct in_addr> easyrdns::EasyResolver::try_string_to_v4addr(const std::string &straddr)
{
    struct in_addr inaddr;
    int res = inet_pton(AF_INET, straddr.c_str(), &inaddr);
    if (res == 1) {
        return std::make_optional(std::move(inaddr));
    } else {
        _lasterr = idk;
        return std::nullopt;
    }
}

std::optional<struct in6_addr> easyrdns::EasyResolver::try_string_to_v6addr(const std::string &straddr)
{
    struct in6_addr inaddr;
    int res = inet_pton(AF_INET6, straddr.c_str(), &inaddr);
    if (res == 1) {
        return std::make_optional(std::move(inaddr));
    } else {
        _lasterr = idk;
        return std::nullopt;
    }
}

std::optional<std::string> easyrdns::EasyResolver::easy_ip_to_ptr(const std::string &straddr)
{
    int res;
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

    std::optional<struct in_addr> v4addr = try_string_to_v4addr(straddr);
    if (v4addr) {
        struct sockaddr_in sa = {
            AF_INET,
            0, //.sin_port
            v4addr.value(), // .sin_addr
            {0},
        };
        res = getnameinfo((const struct sockaddr*)&sa, sizeof(sa), hbuf, sizeof(hbuf), nullptr, 0, NI_NAMEREQD);
        if (res == 0) {
            return std::make_optional(hbuf);
        } else {
            _lasterr = idk;
            return std::nullopt;
        }
    }
    std::optional<struct in6_addr> v6addr = try_string_to_v6addr(straddr);
    if (v6addr) {
        struct sockaddr_in6 sa = {
            AF_INET6,
            0, //.sin6_port
            0, // .sin6_flowinfo
            v6addr.value(), // .sin6_addr
            0, // .sin6_scope_id,
        };
        res = getnameinfo((const struct sockaddr*)&sa, sizeof(sa), hbuf, sizeof(hbuf), nullptr, 0, NI_NAMEREQD);
        if (res == 0) {
            return std::make_optional(hbuf);
        } else {
            _lasterr = idk;
            return std::nullopt;
        }
    }
    _lasterr = idk;
    return std::nullopt;
}

NOMANGLE easyrdns_status_t easyrdns_ip_to_ptr(const char *ip_str, char *ptr_buf, size_t ptr_buf_len)
{
    easyrdns::EasyResolver res;
    std::optional<std::string> maybeptr = res.easy_ip_to_ptr(ip_str);
    if (maybeptr) {
        strncpy(ptr_buf, maybeptr.value().c_str(), ptr_buf_len);
        return valid;
    } else {
        *ptr_buf = '\0';
        return invalid;
    }
}
