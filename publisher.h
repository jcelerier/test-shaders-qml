#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <QObject>
#include <QString>
#include <QtQml>
#include <coppa/ossia/device/minuit_listening_local_device.hpp>
#include <coppa/map.hpp>

#include "transmitter.h"
using namespace coppa;
using namespace coppa::ossia;

// This one supports a single listener
class my_device
{
    public:
        using map_type = coppa::basic_map<ParameterMapType<coppa::ossia::Parameter>>;
        using parent_t = my_device;

        my_device(
                map_type& map,
                std::string name,
                unsigned int in_port,
                std::string out_ip,
                unsigned int out_port):
            m_map{map},
            m_server{in_port, [&] (const auto& m, const auto& ip)
        {
            coppa::ossia::minuit_message_handler<minuit_listening_local_behaviour>::on_messageReceived(*this, m_map, m, ip);
        }},
            client{out_ip, int(out_port)},
            nameTable{name}
        {
            m_server.run();
        }

        auto name() const
        { return nameTable.get_device_name(); }

        auto& map() const
        { return m_map; }

        template<typename String, typename Arg>
        void update(param_t<String> path, Arg&& val)
        {
            auto map_it = m_map.update(path, std::forward<Arg>(val));
            if(map_it != m_map.end())
            {
                auto res = *map_it;
                on_value_changed(res);

                auto it = client.listened.find(path);
                if(it != client.listened.end())
                {
                    // A:listen /WhereToListen:attribute value (each time the attribute change if the listening is turned on)
                    auto addr = nameTable.get_action(minuit_action::ListenReply);
                    thread_local std::stringstream stream;

                    stream << path << ":" << to_minuit_attribute_text(minuit_attribute::Value);
                    auto final_path = stream.str();
                    client.sender.send(
                                string_view(addr),
                                string_view(final_path),
                                static_cast<const Value&>(res));
                }

            }
        }

        Nano::Signal<void(const Parameter&)> on_value_changed;

    private:
        map_type& m_map;
        coppa::osc::receiver m_server;

    public:
        listener client;
        coppa::osc::sender& sender = client.sender;
        minuit_name_table nameTable;

};
class Publisher : public Transmitter
{
        Q_OBJECT

    public:
        explicit Publisher(int, int);
        Q_INVOKABLE void relayChanges(const QPointF&);
        Q_INVOKABLE void clicked(const QPointF&);
        Q_INVOKABLE void released(const QPointF&);
        void init();

    signals:
        void blurChanged(double value);
        void clear();

    private:

        basic_map<ParameterMapType<Parameter>> m_base_map;
        //locked_map<basic_map<ParameterMapType<Parameter>>> m_map{m_base_map};
        my_device m_dev;
};

#endif // PUBLISHER_H
