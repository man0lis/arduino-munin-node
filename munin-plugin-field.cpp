#include "munin-node.h"

MuninPluginField::::MuninPluginField(std::string plugin_name)
    : name(plugin_name)
{
}

void MuninPluginField::addConfig(std::string key,std::string value) {
    config.push_back(std::pair<std::string,std::string>(key,value));
}

void MuninPluginField::fetch(EthernetClient* client) {
    client.println(name + ".value " + getValue());
}

void MuninPluginField::config(EthernetClient* client) {
    std::list<std::pair<std::string,std::string>>::iterator conf_it;
    for(conf_it = config.begin(); conf_it != config.end(); ++conf_it) {
        client.println(name + "." + (*conf_it)->first + " " + (*conf_it)->second);
    }
}

std::string MuninPluginField::getValue(EthernetClient* client) {

}
