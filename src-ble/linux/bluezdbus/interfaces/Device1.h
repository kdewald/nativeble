#pragma once

#include "simpledbus/SimpleDBus.h"

#include <string>

class Device1 : public SimpleDBus::Interfaces::PropertyHandler {
  private:
    static const std::string _interface_name;

    SimpleDBus::Connection* _conn;
    std::string _path;

    std::string _name;
    std::string _address;
    bool _connected;
    bool _services_resolved;

    void add_option(std::string option_name, SimpleDBus::Holder value);
    void remove_option(std::string option_name);

  public:
    Device1(SimpleDBus::Connection* conn, std::string path);
    ~Device1();

    void Connect();
    void Disconnect();

    std::string get_name();
    std::string get_address();
    bool is_connected();

    std::function<void(void)> OnConnected;
    std::function<void(void)> OnDisconnected;
    std::function<void(void)> OnServicesResolved;
};