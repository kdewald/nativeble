#include "BluezService.h"

#include <algorithm>

BluezService::BluezService() : conn(DBUS_BUS_SYSTEM), object_manager(&conn, "org.bluez", "/") {
    object_manager.InterfacesAdded = [&](std::string path, SimpleDBus::Holder options) { add_path(path, options); };
    object_manager.InterfacesRemoved = [&](std::string path, SimpleDBus::Holder options) {
        remove_path(path, options);
    };
}

BluezService::~BluezService() {
    // std::cout << "Destroying BluezService" << std::endl;
    conn.remove_match("type='signal',sender='org.bluez'");
}

void BluezService::init() {
    conn.init();
    object_manager.GetManagedObjects(true);  // Feed the objects via callback.

    conn.add_match("type='signal',sender='org.bluez'");
}

void BluezService::run_async() {
    conn.read_write();
    SimpleDBus::Message message = conn.pop_message();
    while (message.is_valid()) {
        switch (message.get_type()) {
            case SimpleDBus::MessageType::SIGNAL:
                process_received_signal(message);
                break;
            default:
                break;
        }
        message = conn.pop_message();
    }
}

void BluezService::process_received_signal(SimpleDBus::Message& message) {
    std::string path = message.get_path();

    if (path == "/org/freedesktop/DBus") return;

    if (object_manager.process_received_signal(message)) return;

    for (auto& [path, adapter] : adapters) {
        if (adapter->process_received_signal(message)) return;
    }
}

void BluezService::add_path(std::string path, SimpleDBus::Holder options) {
    int path_elements = std::count(path.begin(), path.end(), '/');

    switch (path_elements) {
        case 2:
            agent.reset(new BluezAgent(path, options));
            break;
        case 3:
            adapters.emplace(std::make_pair(path, new BluezAdapter(&conn, path, options)));
            break;
        default:
            // Propagate the paths downwards until someone claims it.
            for (auto& [adapter_path, adapter] : adapters) {
                if (adapter->add_path(path, options)) return;
            }
            break;
    }
}

void BluezService::remove_path(std::string path, SimpleDBus::Holder options) {
    int path_elements = std::count(path.begin(), path.end(), '/');

    switch (path_elements) {
        case 2:
            break;
        case 3:
            adapters.erase(path);
            break;
        default:
            // Propagate the paths downwards until someone claims it.
            for (auto& [adapter_path, adapter] : adapters) {
                if (adapter->remove_path(path, options)) return;
            }
            break;
    }
}

std::shared_ptr<BluezAdapter> BluezService::get_first_adapter() {
    std::shared_ptr<BluezAdapter> return_value = nullptr;
    if (!adapters.empty()) {
        return_value = adapters.begin()->second;
    }

    return return_value;
}

std::shared_ptr<BluezAdapter> BluezService::get_adapter(std::string adapter_name) {
    std::shared_ptr<BluezAdapter> return_value = nullptr;
    std::string expected_path = "/org/bluez/" + adapter_name;

    for (auto& [adapter_path, adapter] : adapters) {
        if (adapter_path == expected_path) {
            return_value = adapter;
            break;
        }
    }
    return return_value;
}
