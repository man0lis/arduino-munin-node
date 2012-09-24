#include <Ethernet.h>
#include <list>
#include <string>

class MuninPluginField {
    public:
        MuninPluginField();
        virtual std::string getValue() = 0;
        void addConfig(std::string,std::string);
    private:
        std::list<std::pair<std::string,std::string>> config;
};

class MuninPlugin {
    public:
        MuninPlugin(std::string);
        void addField(MuninPluginField*);
        void addConfig(std::string,std::string);
        std::string name;
        void fetch(EthernetClient*);
        void config(EthernetClient*);
    private:
        std::list<MuninPluginField*> fields;
        std::list<std::pair<std::string,std::string>> config;
};

class MuninNode {
    public:
        MuninNode(std::string,int);
        void run();
        void addPlugin(MuninPlugin*);
    private:
        EthernetServer server;
        std::list<MuninPlugin*> plugins;
        std::string hostname;
        bool connected;
        bool waitFor(EthernetClient*,char);
        int timeout;
        char command;
        bool ready;
        bool parsed;
        std::string parse;
        void listPlugins(EthernetClient*);
        bool fetchPlugin(EthernetClient*);
        bool fetchPluginConfig(EthernetClient*);
        MuninPlugin* findPlugin(std::string);
};
