#include "Device1.h"

#include "simpledbus/base/Logger.h"

const std::string Device1::_interface_name = "org.bluez.Device1";

Device1::Device1(SimpleDBus::Connection* conn, std::string path)
    : _conn(conn), _path(path), _address(""), _name(""), _connected(false), _services_resolved(false) {}

Device1::~Device1() {}

void Device1::add_option(std::string option_name, SimpleDBus::Holder value) {
    if (option_name == "Address") {
        _address = value.get_string();
    } else if (option_name == "Name") {
        _name = value.get_string();
    } else if (option_name == "Alias") {
        _alias = value.get_string();
    } else if (option_name == "RSSI") {
        _rssi = value.get_int16();
    } else if (option_name == "Connected") {
        _connected = value.get_boolean();
        if (_connected && OnConnected) {
            LOG_F(VERBOSE_0, "%s -> OnConnected", _path.c_str());
            OnConnected();
        } else if (!_connected && OnDisconnected) {
            LOG_F(VERBOSE_0, "%s -> OnDisconnected", _path.c_str());
            OnDisconnected();
        }
    } else if (option_name == "ServicesResolved") {
        _services_resolved = value.get_boolean();
        if (_services_resolved && OnServicesResolved) {
            LOG_F(VERBOSE_0, "%s -> OnServicesResolved", _path.c_str());
            OnServicesResolved();
        }
    }
    // TODO: Add ManufacturerData
}

void Device1::remove_option(std::string option_name) {}

void Device1::Connect() {
    if (!_connected) {
        // Only attempt connection if disconnected.
        LOG_F(DEBUG, "%s -> Connect", _path.c_str());
        auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, _interface_name, "Connect");
        _conn->send_with_reply_and_block(msg);
    } else {
        LOG_F(WARN, "%s is already connected...", _path.c_str());
        // If already connected, run callbacks to emulate a successful connection.
        if (OnConnected) {
            LOG_F(VERBOSE_0, "%s -> OnConnected (fake)", _path.c_str());
            OnConnected();
        }
        if (_services_resolved && OnServicesResolved) {
            LOG_F(VERBOSE_0, "%s -> OnServicesResolved (fake)", _path.c_str());
            OnServicesResolved();
        }
    }
}

void Device1::Disconnect() {
    if (_connected) {
        // Only attempt disconnection if connected.
        LOG_F(DEBUG, "%s -> Disconnect", _path.c_str());
        auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, _interface_name, "Disconnect");
        _conn->send_with_reply_and_block(msg);
    } else {
        LOG_F(WARN, "%s is already disconnected...", _path.c_str());
        // If already disconnected, run callbacks to emulate a successful disconnection.
        if (OnDisconnected) {
            LOG_F(VERBOSE_0, "%s -> OnDisconnected (fake)", _path.c_str());
            OnDisconnected();
        }
    }
}

int16_t Device1::get_rssi() { return _rssi; }

std::string Device1::get_name() { return _name; }

std::string Device1::get_alias() { return _alias; }

std::string Device1::get_address() { return _address; }

bool Device1::is_connected() { return _connected; }