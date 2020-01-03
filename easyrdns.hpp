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

#ifndef _github_paulyc_easyrdns_hpp_
#define _github_paulyc_easyrdns_hpp_

#include "easyrdns.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>
#include <optional>

namespace easyrdns {

class EasyResolver
{
public:
    enum Error
    {
        none,
        idk,
    };

    EasyResolver();
    ~EasyResolver();

    std::optional<struct in_addr> try_string_to_v4addr(const std::string &straddr);
    std::optional<struct in6_addr> try_string_to_v6addr(const std::string &straddr);
    std::optional<std::string> easy_ip_to_ptr(const std::string &straddr);

    Error get_last_error() const { return _lasterr; }
private:
    //struct addrinfo _hints;
    Error _lasterr;
};

} // namespace easyrdns

#endif // _github_paulyc_easyrdns_hpp_
