#include "Adapter1.h"

#include <iostream>
#include <thread>

Adapter1::Adapter1(SimpleDBus::Connection* conn, std::string path) : _conn(conn), _path(path), _discovering(false) {}

Adapter1::~Adapter1() {}

void Adapter1::add_option(std::string option_name, SimpleDBus::Holder value) {
    if (option_name == "Discovering") {
        _discovering = value.get_boolean();
    }
}
void Adapter1::remove_option(std::string option_name) {}

void Adapter1::StartDiscovery() {
    if (!_discovering) {
        auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, "org.bluez.Adapter1", "StartDiscovery");
        _conn->send_with_reply_and_block(msg);
    }
}
void Adapter1::StopDiscovery() {
    if (_discovering) {
        auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, "org.bluez.Adapter1", "StopDiscovery");
        _conn->send_with_reply_and_block(msg);

        // NOTE: It might take a few seconds until the peripheral reports that is has actually stopped discovering.
    }
}

SimpleDBus::Holder Adapter1::GetDiscoveryFilters() {
    // std::cout << "org.bluez.Adapter1 GetDiscoveryFilters" << std::endl;
    auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, "org.bluez.Adapter1", "GetDiscoveryFilters");
    SimpleDBus::Message reply_msg = _conn->send_with_reply_and_block(msg);
    SimpleDBus::Holder discovery_filters = reply_msg.extract();
    // std::cout << discovery_filters.represent() << std::endl;
    return discovery_filters;
}

void Adapter1::SetDiscoveryFilter(SimpleDBus::Holder properties) {
    // std::cout << "org.bluez.Adapter1 SetDiscoveryFilters" << std::endl;
    auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, "org.bluez.Adapter1", "SetDiscoveryFilter");
    msg.append_argument(properties, "a{sv}");
    _conn->send_with_reply_and_block(msg);
}

bool Adapter1::is_discovering() { return _discovering; }
