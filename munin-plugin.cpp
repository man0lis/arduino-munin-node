#include "munin-node.h"

MuninPlugin::MuninPlugin(std::string plugin_name)
    : name(plugin_name)
{
}

void MuninPlugin::addField(MuninPluginField* field) {
    fields.push_back(field);
}

void MuninPlugin::addConfig(std::string key,std::string value) {
    config.push_back(std::pair<std::string,std::string>(key,value));
}

void MuninPlugin::fetch(EthernetClient* client) {
    std::list<MuninPluginField>::iterator it;
    for(it = fields.begin(); it != fields.end(); ++it) {
        (*it)->fetch(client);
    }
    client.println(".");
}

void MuninPlugin::config(EthernetClient* client) {
    std::list<std::pair<std::string,std::string>>::iterator conf_it;
    for(conf_it = config.begin(); conf_it != config.end(); ++conf_it) {
        client.println((*conf_it)->first + " " + (*conf_it)->second);
    }

    std::list<MuninPluginField>::iterator field_it;
    for(field_it = fields.begin(); field_it != fields.end(); ++field_it) {
        (*field_it)->config(client);
    }
    client.println(".");
}
