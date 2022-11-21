#include "vesper_plugin.h"
#include "vesper_metadata.h"

const pool_metadata_t *find_pool_metadata(const uint8_t *(*get_pool_prop)(const pool_metadata_t *),
                                          uint8_t *address) {
    uint8_t i;
    for (i = 0; i < NUM_VESPER_POOLS; i++) {
        const pool_metadata_t *current = &VESPER_POOLS[i];
        if (memcmp(get_pool_prop(current), address, ADDRESS_LENGTH) == 0) {
            return current;
        }
    }
    return NULL;
}

const uint8_t *get_pool_address(const pool_metadata_t *pool) {
    return pool->address;
}

const uint8_t *get_pool_rewards_address(const pool_metadata_t *pool) {
    return pool->rewards;
}

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    uint8_t *destination = msg->pluginSharedRO->txContent->destination;
    bool pool_data_found = false;

    switch (context->selectorIndex) {
        case DEPOSIT:
        case DEPOSIT_AND_CLAIM:
        case DEPOSIT_ETH:
        case DEPOSIT_ETH_AND_CLAIM:
        case WITHDRAW:
        case WITHDRAW_AND_CLAIM:
        case WITHDRAW_ETH:
        case WITHDRAW_ETH_AND_CLAIM:
            context->pool_metadata = find_pool_metadata(get_pool_address, destination);
            pool_data_found = context->pool_metadata != NULL;
            break;
        case CLAIM_REWARD:
            context->pool_metadata = find_pool_metadata(get_pool_rewards_address, destination);
            pool_data_found = context->pool_metadata != NULL;
            break;
        case SIMPLE_MIGRATE:
        case SIMPLE_MIGRATE_WITH_PERMIT:
            context->pool_metadata = find_pool_metadata(get_pool_address, context->pool_from);
            context->pool_metadata_to = find_pool_metadata(get_pool_address, context->pool_to);
            pool_data_found = context->pool_metadata != NULL && context->pool_metadata_to != NULL;
            break;
    }

    if (!pool_data_found) {
        PRINTF("Pool data not found\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    switch (context->selectorIndex) {
        case CLAIM_REWARD:
            msg->numScreens = 1;
            break;
        case DEPOSIT:
        case DEPOSIT_AND_CLAIM:
        case DEPOSIT_ETH:
        case DEPOSIT_ETH_AND_CLAIM:
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
