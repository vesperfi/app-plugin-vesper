#include "vesper_plugin.h"
#include "vesper_metadata.h"

const pool_metadata_t *find_pool_metadata(uint8_t *pool_address) {
    uint8_t i;
    for (i = 0; i < NUM_VESPER_POOLS; i++) {
        const pool_metadata_t *current = &VESPER_POOLS[i];
        if (memcmp(current->address, pool_address, ADDRESS_LENGTH) == 0) {
            return current;
        }
    }
    return NULL;
}

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    uint8_t *pool_address = msg->pluginSharedRO->txContent->destination;
    context->pool_metadata = find_pool_metadata(pool_address);

    if (!context->pool_metadata) {
        PRINTF("Pool not found\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    switch (context->selectorIndex) {
        case CLAIM_REWARD:
        case UNLOCK:
            msg->numScreens = 1;
            break;
        case DEPOSIT:
        case DEPOSIT_AND_CLAIM:
        case DEPOSIT_ETH:
        case DEPOSIT_ETH_AND_CLAIM:
        case LOCK:
        case MINT:
        case WITHDRAW:
        case WITHDRAW_AND_CLAIM:
        case WITHDRAW_ETH:
        case WITHDRAW_ETH_AND_CLAIM:
            msg->numScreens = 2;
            break;
        case SIMPLE_MIGRATE:
        case SIMPLE_MIGRATE_WITH_PERMIT:
            msg->numScreens = 3;
            break;
        default:
            msg->numScreens = 0;
    }

    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}
