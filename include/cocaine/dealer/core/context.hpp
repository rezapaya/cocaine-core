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

#ifndef _COCAINE_DEALER_CONTEXT_HPP_INCLUDED_
#define _COCAINE_DEALER_CONTEXT_HPP_INCLUDED_

#include <string>
#include <map>
#include <memory>

#include <zmq.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include "cocaine/dealer/core/configuration.hpp"
#include "cocaine/dealer/core/statistics_collector.hpp"
#include "cocaine/dealer/utils/smart_logger.hpp"
#include "cocaine/dealer/storage/eblob_storage.hpp"

namespace cocaine {
namespace dealer {

class context_t : private boost::noncopyable {
public:
	explicit context_t(const std::string& config_path = "");
	virtual ~context_t();

	boost::shared_ptr<base_logger> logger();
	boost::shared_ptr<configuration> config();
	boost::shared_ptr<zmq::context_t> zmq_context();
	boost::shared_ptr<statistics_collector> stats();
	boost::shared_ptr<eblob_storage> storage();

private:
	boost::shared_ptr<zmq::context_t> zmq_context_;
	boost::shared_ptr<base_logger> logger_;
	boost::shared_ptr<configuration> config_;
	boost::shared_ptr<statistics_collector> stats_;
	boost::shared_ptr<eblob_storage> storage_;
};

} // namespace dealer
} // namespace cocaine

#endif // _COCAINE_DEALER_CONTEXT_HPP_INCLUDED_
