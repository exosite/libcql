/*
  Copyright (c) 2012 Matthew Stump

  This file is part of libcql.

  libcql is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  libcql is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "libcql/cql_vector_stream.hpp"
#include "libcql/cql_serialization.hpp"
#include "libcql/internal/cql_defines.hpp"

#include "libcql/internal/cql_message_event_impl.hpp"


cql::cql_message_event_impl_t::cql_message_event_impl_t() :
    _buffer(new std::vector<cql_byte_t>(0)),
    _event_type(CQL_EVENT_TYPE_UNKOWN),
    _topology_change(CQL_EVENT_TOPOLOGY_UNKNOWN),
    _schema_change(CQL_EVENT_SCHEMA_UNKNOWN),
    _status_change(CQL_EVENT_STATUS_UNKNOWN),
    _port(0)
{}

cql::cql_message_event_impl_t::cql_message_event_impl_t(size_t size) :
    _buffer(new std::vector<cql_byte_t>(size)),
    _event_type(CQL_EVENT_TYPE_UNKOWN),
    _topology_change(CQL_EVENT_TOPOLOGY_UNKNOWN),
    _schema_change(CQL_EVENT_SCHEMA_UNKNOWN),
    _status_change(CQL_EVENT_STATUS_UNKNOWN),
    _port(0)
{}

cql::cql_message_buffer_t
cql::cql_message_event_impl_t::buffer()
{
    return _buffer;
}

cql::cql_opcode_enum
cql::cql_message_event_impl_t::opcode() const
{
    return CQL_OPCODE_EVENT;
}

std::string
cql::cql_message_event_impl_t::str() const
{
    return "EVENT";
}

cql::cql_event_enum
cql::cql_message_event_impl_t::event_type() const
{
    return _event_type;
}

cql::cql_event_schema_enum
cql::cql_message_event_impl_t::schema_change() const
{
    return _schema_change;
}

cql::cql_event_topology_enum
cql::cql_message_event_impl_t::topology_change() const
{
    return _topology_change;
}

cql::cql_event_status_enum
cql::cql_message_event_impl_t::status_change() const
{
    return _status_change;
}

const std::string&
cql::cql_message_event_impl_t::keyspace() const
{
    return _keyspace;
}

const std::string&
cql::cql_message_event_impl_t::column_family() const
{
    return _column_family;
}

const std::string&
cql::cql_message_event_impl_t::ip() const
{
    return _ip;
}

cql::cql_int_t
cql::cql_message_event_impl_t::port() const
{
    return _port;
}

cql::cql_int_t
cql::cql_message_event_impl_t::size() const
{
    return _buffer->size();
}

bool
cql::cql_message_event_impl_t::consume(cql::cql_error_t* err)
{
    _ip = "";
    _port = 0;
    _keyspace = "";
    _column_family = "";
    _event_type = CQL_EVENT_TYPE_UNKOWN;
    _topology_change = CQL_EVENT_TOPOLOGY_UNKNOWN;
    _status_change = CQL_EVENT_STATUS_UNKNOWN;
    _schema_change = CQL_EVENT_SCHEMA_UNKNOWN;

    cql::vector_stream_t buffer(*_buffer);
    std::istream stream(&buffer);

    std::string event_type;
    cql::decode_string(stream, event_type);

    if (event_type == CQL_EVENT_TOPOLOGY_CHANGE) {
        _event_type = CQL_EVENT_TYPE_TOPOLOGY;

        std::string change;
        cql::decode_string(stream, change);
        cql::decode_inet(stream, _ip, _port);

        if (change == CQL_EVENT_TOPOLOGY_CHANGE_NEW) {
            _topology_change = CQL_EVENT_TOPOLOGY_ADD_NODE;
        }
        else if (change == CQL_EVENT_TOPOLOGY_CHANGE_REMOVE) {
            _topology_change = CQL_EVENT_TOPOLOGY_REMOVE_NODE;
        }
        else {
            _topology_change = CQL_EVENT_TOPOLOGY_UNKNOWN;
        }
    }
    else if (event_type == CQL_EVENT_STATUS_CHANGE) {
        _event_type = CQL_EVENT_TYPE_STATUS;

        std::string change;
        cql::decode_string(stream, change);
        cql::decode_inet(stream, _ip, _port);

        if (change == CQL_EVENT_STATUS_CHANGE_UP) {
            _status_change = CQL_EVENT_STATUS_UP;
        }
        else if (change == CQL_EVENT_STATUS_CHANGE_DOWN) {
            _status_change = CQL_EVENT_STATUS_DOWN;
        }
        else {
            _status_change = CQL_EVENT_STATUS_UNKNOWN;
        }
    }
    else if (event_type == CQL_EVENT_SCHEMA_CHANGE) {
        _event_type = CQL_EVENT_TYPE_SCHEMA;

        std::string change;
        cql::decode_string(stream, change);
        cql::decode_string(stream, _keyspace);
        cql::decode_string(stream, _column_family);

        if (change == CQL_EVENT_SCHEMA_CHANGE_CREATED) {
            _schema_change = CQL_EVENT_SCHEMA_CREATED;
        }
        else if (change == CQL_EVENT_SCHEMA_CHANGE_DROPPED) {
            _schema_change = CQL_EVENT_SCHEMA_DROPPED;
        }
        else if (change == CQL_EVENT_SCHEMA_CHANGE_UPDATED) {
            _schema_change = CQL_EVENT_SCHEMA_UPDATED;
        }
        else {
            _schema_change = CQL_EVENT_SCHEMA_UNKNOWN;
        }
    }

    return true;
}

bool
cql::cql_message_event_impl_t::prepare(cql::cql_error_t* err)
{
    cql::vector_stream_t buffer(*_buffer);
    std::ostream stream(&buffer);


    return true;
}
