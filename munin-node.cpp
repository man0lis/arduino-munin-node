#include "munin-node.h"

MuninNode::MuninNode(std::string hostname, int port)
    : server(port), hostname(hostname), connected(false), timeout(0), command(0)
{
    server.begin();
}

bool MuninNode::waitFor(EthernetClient* client, char waitForIt) {
    while (client->available() > 0) {
        char c = client->read();
        if (c == waitForIt) {
            return true;
        }
    }
    return false;
}

void MuninNode::listPlugins(EthernetClient* client) {
    std::list<MuninPlugin*>::iterator it;
    for (it = plugins.begin(); it != plugins.end(); ++it) {
        if (it != plugins.begin()) {
            client.print(" ");
        }
        client.print( (*it)->name );
    }
    client.println("");
}

MuninPlugin* MuninNode::findPlugin(std::string plugin) {
    std::list<MuninPlugin*>::iterator it;
    for (it = plugins.begin(); it != plugins.end(); ++it) {
        if ((*it)->name == plugin) {
            return *it;
        }
    }
    return NULL;
}

bool MuninNode::fetchPlugin(EthernetClient* client) {
    while(client.available() > 0) {
        char c = client.read();
        if (c != ' ' && c != '\n') {
            parse += c;
        }
        else {
            //TODO: ignore rest of line after space
            MuninPlugin *plugin = findPlugin(parse);
            if (plugin) {
                plugin->fetch(client);
            }
            else {
                client.println("#Unknown service");
            }
            client.println(".");
            return true;
        }
    }
    return false;
}

bool MuninNode::fetchPluginConfig(EthernetClient* client) {
    while(client.available() > 0) {
        char c = client.read();
        if (c != ' ' && c != '\n') {
            parse += c;
        }
        else {
            //TODO: ignore rest of line after space
            MuninPlugin *plugin = findPlugin(parse);
            if (plugin) {
                plugin->config(client);
            }
            else {
                client.println("#Unknown service");
            }
            client.println(".");
            return true;
        }
    }
    return false;
}

void MuninNode::run() {
    EthernetClient client = server.available();

    if (client) {
        if (!connected) {
            client.flush();    
            client.println("#munin node at " + hostname); 
            connected = true;
            timeout = mills();
        } 

        if (client.available() > 0) {
            timeout = mills();  
            if(!command) {
                command = client.read();
                ready = false;
                parsed = true;
                parse = "";
            }
            switch(command) {
                case 'c':
                case 'f':
                    ready = waitFor(&client, ' ');
                    break;
                
                case 'l':
                case 'n':
                case 'v':
                case 'q':
                default:
                    ready = waitFor(&client, '\n');
            }
            if (ready) {
                switch(command) {
                    case 'c':
                        parsed = fetchPluginConfig(&client);
                        break;
                        
                    case 'f':
                        parsed = fetchPlugin(&client);
                        break;

                    case 'l':
                        listPlugins(&client);
                        break;

                    case 'n':
                        client.println(hostname);
                        client.println(".");
                        break;

                    case 'v':
                        client.println("#munin node on " + hostname + "version: 0.1 (arduino-munin-node)");
                        break;

                    case 'q':
                        connected = false;
                        client.stop();
                        break;

                    default:
                        client.println("# Unknown command. Try list, nodes, config, fetch, version or quit")
                }
                if(parsed) {
                    command = 0;
                }
            }

        }
        if(timout > mills()+30000) {
            connected = false;
            client.stop();
        }
    }
    else {
        connected = false;
    }
}


