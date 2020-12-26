#include "Device1.h"

#include <iostream>

Device1::Device1(SimpleDBus::Connection* conn, std::string path)
    : _conn(conn), _path(path), _address(""), _name(""), _connected(false), _services_resolved(false) {
    // std::cout << "Creating org.bluez.Device1: " << path << std::endl;
}

Device1::~Device1() {}

void Device1::add_option(std::string option_name, SimpleDBus::Holder value) {
    // std::cout << "\t" << option_name << std::endl;

    if (option_name == "Address") {
        _address = value.get_string();
    } else if (option_name == "Name") {
        _name = value.get_string();
    } else if (option_name == "Connected") {
        _connected = value.get_boolean();
        if (_connected && OnConnected) {
            OnConnected();
        } else if (!_connected && OnDisconnected) {
            OnDisconnected();
        }
    } else if (option_name == "ServicesResolved") {
        _services_resolved = value.get_boolean();
        if (_services_resolved && OnServicesResolved) {
            OnServicesResolved();
        }
    }
}

void Device1::remove_option(std::string option_name) {}

void Device1::Connect() {
    if (!_connected) {
        // Only attempt connection if disconnected.
        auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, "org.bluez.Device1", "Connect");
        _conn->send_with_reply_and_block(msg);
    } else {
        // If already connected, run callbacks to emulate a successful connection.
        if (OnConnected) {
            OnConnected();
        }
        if (_services_resolved && OnServicesResolved) {
            OnServicesResolved();
        }
    }
}

void Device1::Disconnect() {
    if (_connected) {
        // Only attempt disconnection if connected.
        auto msg = SimpleDBus::Message::create_method_call("org.bluez", _path, "org.bluez.Device1", "Disconnect");
        _conn->send_with_reply_and_block(msg);
    } else {
        // If already disconnected, run callbacks to emulate a successful disconnection.
        if (OnDisconnected) {
            OnDisconnected();
        }
    }
}

std::string Device1::get_name() { return _name; }

std::string Device1::get_address() { return _address; }

bool Device1::is_connected() { return _connected; }