/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <signal.h>
#include "open62541.h"

UA_Boolean running = true;
static void stopHandler(int sig) {
    running = false;
}

int main(void) {
    signal(SIGINT,  stopHandler);
    signal(SIGTERM, stopHandler);

    UA_ServerConfig *config = UA_ServerConfig_new_default();
    UA_Server *server = UA_Server_new(config);

    /* Create a rudimentary objectType
     *
     * Type:
     * + MamalType
     *  v- Class  = "mamalia"
     *  v- Species
     *  o- Abilities
     *      v- MakeSound
     *      v- Breathe = True
     *  + DogType
     *      v- Species = "Canis"
     *      v- Name
     *      o- Abilities
     *          v- MakeSound = "Wuff"
     *           v- FetchNewPaper
     */

    UA_StatusCode retval;
    UA_ObjectTypeAttributes otAttr = UA_ObjectTypeAttributes_default;
    otAttr.description = UA_LOCALIZEDTEXT("en-US", "A mamal");
    otAttr.displayName = UA_LOCALIZEDTEXT("en-US", "MamalType");
    UA_Server_addObjectTypeNode(server, UA_NODEID_NUMERIC(1, 10000),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "MamalType"), otAttr, NULL, NULL);

    UA_VariableAttributes vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "This mamals class");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "Class");
    UA_String classVar = UA_STRING("mamalia");
    UA_Variant_setScalar(&vAttr.value, &classVar, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 10001),
                              UA_NODEID_NUMERIC(1, 10000),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "Class"), UA_NODEID_NULL,
                              vAttr, NULL, NULL);

    vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "This mamals species");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "Species");
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 10002),
                              UA_NODEID_NUMERIC(1, 10000),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "Species"), UA_NODEID_NULL,
                              vAttr, NULL, NULL);

    otAttr = UA_ObjectTypeAttributes_default;
    otAttr.description = UA_LOCALIZEDTEXT("en-US", "A dog, subtype of mamal");
    otAttr.displayName = UA_LOCALIZEDTEXT("en-US", "DogType");
    UA_Server_addObjectTypeNode(server, UA_NODEID_NUMERIC(1, 20000),
                                UA_NODEID_NUMERIC(1, 10000),
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                UA_QUALIFIEDNAME(1, "DogType"), otAttr, NULL, NULL);

    vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "This dogs species");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "Species");
    UA_String defaultSpecies = UA_STRING("Canis");
    UA_Variant_setScalar(&vAttr.value, &defaultSpecies, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 20001),
                              UA_NODEID_NUMERIC(1, 20000),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "Species"), UA_NODEID_NULL,
                              vAttr, NULL, NULL);

    vAttr = UA_VariableAttributes_default;
    vAttr.description =  UA_LOCALIZEDTEXT("en-US", "This dogs name");
    vAttr.displayName =  UA_LOCALIZEDTEXT("en-US", "Name");
    UA_String defaultName = UA_STRING("unnamed dog");
    UA_Variant_setScalar(&vAttr.value, &defaultName, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 20002),
                              UA_NODEID_NUMERIC(1, 20000),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
                              UA_QUALIFIEDNAME(1, "Name"), UA_NODEID_NULL,
                              vAttr, NULL, NULL);

    /* Instatiate a dog named bello:
     * (O) Objects
     *   + O Bello <DogType>
     *     + Age
     *     + Name
     */

    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.description = UA_LOCALIZEDTEXT("en-US", "A dog named Bello");
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Bello");
    UA_Server_addObjectNode(server, UA_NODEID_NUMERIC(1, 0),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Bello"), UA_NODEID_NUMERIC(1, 20000),
                            oAttr, NULL, NULL);

    oAttr = UA_ObjectAttributes_default;
    oAttr.description = UA_LOCALIZEDTEXT("en-US", "Another dog");
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Dog2");
    UA_Server_addObjectNode(server, UA_NODEID_NUMERIC(1, 0),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Dog2"), UA_NODEID_NUMERIC(1, 20000),
                            oAttr, NULL, NULL);

    oAttr = UA_ObjectAttributes_default;
    oAttr.description = UA_LOCALIZEDTEXT("en-US", "A mamal");
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Mamal1");
    UA_Server_addObjectNode(server, UA_NODEID_NUMERIC(1, 0),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "Mamal1"), UA_NODEID_NUMERIC(1, 10000),
                            oAttr, NULL, NULL);

    /* Run the server */
    retval = UA_Server_run(server, &running);
    UA_Server_delete(server);
    UA_ServerConfig_delete(config);
    return (int)retval;
}
