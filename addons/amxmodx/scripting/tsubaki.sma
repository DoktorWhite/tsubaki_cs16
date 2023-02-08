#define DEBUG

#include <tsubaki/tsubaki_lib>
#include <tsubaki/tsubaki_skills>
#include <tsubaki/tsubaki_debug>

public plugin_precache() {
    PrecacheTsubakiStuff();		//tsubaki_method_lib
    RegisterTsubakiEvents();	//tsubaki_lib
}

public plugin_init() {
    server_print("Server on");

    RegisterTBKDebugMenuAndCommand();
    RegisterTsubakiEntityStuffs();

    register_concmd("rest", "ServerRestart", ADMIN_ALL);
}

public plugin_end() {

}

public client_putinserver(client) {
    #if defined DEBUG
        gbPlyDeveloper[client] = true;
    #endif
    //TsubakiRegisterPlayerEvent(client);         //tsubaki_lib
}

public CS_InternalCommand(client, const command[])
{
    static const block_command[][] = {"kill", "buy", "autobuy", "rebuy"};

    for(new i=0; i<sizeof(block_command); i++)
    {
        if(equal(block_command[i], command))
            return PLUGIN_HANDLED;
    }
    return PLUGIN_CONTINUE;
}

public ServerRestart(client) {
    if(client!=0 && !gbPlyDeveloper[client])
        return PLUGIN_CONTINUE;

    server_cmd("restart");
    return PLUGIN_HANDLED;
}