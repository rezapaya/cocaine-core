/*
    Copyright (c) 2011-2013 Andrey Sibiryov <me@kobology.ru>
    Copyright (c) 2011-2013 Other contributors as noted in the AUTHORS file.

    This file is part of Cocaine.

    Cocaine is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Cocaine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cocaine/dispatch.hpp"

#include "cocaine/context.hpp"
#include "cocaine/logging.hpp"

#include "cocaine/rpc/message.hpp"

using namespace cocaine;
using namespace cocaine::io;

dispatch_t::dispatch_t(context_t& context, const std::string& name):
    m_log(new logging::log_t(context, name)),
    m_name(name)
{ }

dispatch_t::~dispatch_t() {
    // Empty.
}

std::shared_ptr<dispatch_t>
dispatch_t::invoke(const io::message_t& message, const api::stream_ptr_t& upstream) const {
    slot_map_t::mapped_type slot;

    {
        std::lock_guard<std::mutex> guard(m_mutex);

        auto it = m_slots.find(message.id());

        if(it == m_slots.end()) {
            COCAINE_LOG_WARNING(m_log, "dropping an unknown type %d: %s message", message.id(), message.args());

            // TODO: COCAINE-82 changes this to a 'client' error category.
            throw cocaine::error_t("unknown message type");
        }

        // NOTE: The slot pointer is copied here so that the handling code could unregister the slot
        // via dispatch_t::forget() without pulling the object from underneath itself.
        slot = it->second;
    }

    COCAINE_LOG_DEBUG(m_log, "processing type %d message using slot '%s'", message.id(), slot->name());

    try {
        return (*slot)(message.args(), upstream);
    } catch(const std::exception& e) {
        COCAINE_LOG_ERROR(m_log, "unable to process type %d message using slot '%s' - %s", message.id(), slot->name(), e.what());

        // TODO: COCAINE-82 changes this to rethrow with a 'server' error category.
        throw;
    }
}

std::string
dispatch_t::name() const {
    return m_name;
}
