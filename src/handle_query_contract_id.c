#include "vesper_plugin.h"

void set_msg_name(ethQueryContractID_t *msg, char *text) {
    strlcpy(msg->name, text, msg->nameLength);
}

void set_msg_version(ethQueryContractID_t *msg, char *text) {
    strlcpy(msg->version, text, msg->versionLength);
}

// Sets the first screen to display.
void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    const context_t *context = (const context_t *) msg->pluginContext;

    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    set_msg_name(msg, PLUGIN_NAME);

    switch (context->selectorIndex) {
        case CLAIM_REWARD:
            set_msg_version(msg, "Claim rewards");
            break;
        case DEPOSIT:
        case DEPOSIT_ETH:
            set_msg_version(msg, "Deposit");
            break;
        case DEPOSIT_AND_CLAIM:
        case DEPOSIT_ETH_AND_CLAIM:
            set_msg_version(msg, "Deposit and claim");
            break;
        case SIMPLE_MIGRATE:
        case SIMPLE_MIGRATE_WITH_PERMIT:
            set_msg_version(msg, "Transfer");
            break;
        case WITHDRAW:
            set_msg_version(msg, "Withdraw");
            break;
        case WITHDRAW_ETH:
            set_msg_version(msg, "Withdraw ETH");
            break;
        case WITHDRAW_AND_CLAIM:
            set_msg_version(msg, "Withdraw and claim");
            break;
        case WITHDRAW_ETH_AND_CLAIM:
            set_msg_version(msg, "Withdraw ETH and claim");
            break;
        case CLAIM_REWARDS:
            set_msg_version(msg, "Claim VSP");
            break;
        case LOCK:
            set_msg_version(msg, "Lock VSP");
            break;
        case MINT:
            set_msg_version(msg, "Mint VSP");
            break;
        case UNLOCK:
            set_msg_version(msg, "Unlock VSP");
            break;
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}