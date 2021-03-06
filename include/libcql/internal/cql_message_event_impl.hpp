/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libcql.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#ifndef CQL_MESSAGE_EVENT_IMPL_H_
#define CQL_MESSAGE_EVENT_IMPL_H_

#include <string>

#include "libcql/cql_event.hpp"
#include "libcql/internal/cql_message.hpp"

namespace cql {

    class cql_message_event_impl_t :
        public cql_event_t,
        public cql_message_t
    {
    public:

        cql_message_event_impl_t();

        cql::cql_opcode_enum
        opcode() const;

        std::string
        str() const;

        virtual cql_event_enum
        event_type() const;

        virtual cql_event_topology_enum
        topology_change() const;

        virtual cql_event_status_enum
        status_change() const;

        virtual cql_event_schema_enum
        schema_change() const;

        virtual const std::string&
        keyspace() const;

        virtual const std::string&
        column_family() const;

        virtual const std::string&
        ip() const;

        virtual cql_int_t
        port() const;

        bool
        consume(cql::cql_error_t* err);

        bool
        prepare(cql::cql_error_t* err);

    private:
        cql_event_enum            _event_type;
        cql_event_topology_enum   _topology_change;
        cql_event_schema_enum     _schema_change;
        cql_event_status_enum     _status_change;

        std::string               _ip;
        cql_int_t                 _port;
        std::string               _keyspace;
        std::string               _column_family;
    };

} // namespace cql

#endif // CQL_MESSAGE_EVENT_IMPL_H_
