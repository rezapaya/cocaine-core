//
// Copyright (C) 2011-2012 Rim Zaidullin <creator@bash.org.ru>
//
// Licensed under the BSD 2-Clause License (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef _COCAINE_DEALER_HTTP_HOSTS_FETCHER_HPP_INCLUDED_
#define _COCAINE_DEALER_HTTP_HOSTS_FETCHER_HPP_INCLUDED_

#include <memory>
#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <curl/curl.h>

#include "cocaine/dealer/core/host_info.hpp"
#include "cocaine/dealer/core/service_info.hpp"
#include "cocaine/dealer/utils/refresher.hpp"

namespace cocaine {
namespace dealer {

class http_hosts_fetcher : private boost::noncopyable  {
public:
	http_hosts_fetcher(service_info_t service_info, boost::uint32_t interval = default_fetch_interval);
	virtual ~http_hosts_fetcher();
	
	void start();
	void stop();
	
	// passes list of hosts of specific service to callback
	void set_callback(boost::function<void(std::vector<host_info_t>&, service_info_t)> callback);

	static const int default_fetch_interval = 1;

private:
	void interval_func();
	static int curl_writer(char* data, size_t size, size_t nmemb, std::string* buffer_in);

private:
	CURL* curl_;
	boost::function<void(std::vector<host_info_t>&, service_info_t)> callback_;
	service_info_t service_info_;
	boost::uint32_t interval_;
	std::auto_ptr<refresher> refresher_;
};

} // namespace dealer
} // namespace cocaine

#endif // _COCAINE_DEALER_CURL_HOSTS_FETCHER_HPP_INCLUDED_