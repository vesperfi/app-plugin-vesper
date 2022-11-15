#include "vesper_plugin.h"

void set_msg_title(ethQueryContractUI_t *msg, const char *text) {
    strlcpy(msg->title, text, msg->titleLength);
}

void set_msg_msg(ethQueryContractUI_t *msg, const char *text) {
    strlcpy(msg->msg, text, msg->msgLength);
}

static void set_pool_name_ui(ethQueryContractUI_t *msg, context_t *context) {
    set_msg_title(msg, "Pool");
    set_msg_msg(msg, context->pool_metadata->pool_ticker);
}

static void set_token_amount_ui(ethQueryContractUI_t *msg, context_t *context) {
    set_msg_title(msg, "Amount");
    amountToString(context->amount,
                   sizeof(context->amount),
                   context->pool_metadata->decimals,
                   context->pool_metadata->token_ticker,
                   msg->msg,
                   msg->msgLength);
}

static void set_eth_amount_ui(ethQueryContractUI_t *msg, context_t *context) {
    set_msg_title(msg, "Amount");
    amountToString(msg->pluginSharedRO->txContent->value.value,
                   msg->pluginSharedRO->txContent->value.length,
                   context->pool_metadata->decimals,
                   context->pool_metadata->token_ticker,
                   msg->msg,
                   msg->msgLength);
}

static void set_v_token_amount_ui(ethQueryContractUI_t *msg, context_t *context) {
    set_msg_title(msg, "Amount");
    amountToString(context->amount,
                   sizeof(context->amount),
                   18,
                   context->pool_metadata->pool_ticker,
                   msg->msg,
                   msg->msgLength);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    switch (msg->screenIndex) {
        case 0:
            switch (context->selectorIndex) {
                case DEPOSIT:
                case DEPOSIT_ETH:
                case WITHDRAW:
                    set_pool_name_ui(msg, context);
                    break;
                // Keep this
                default:
                    PRINTF("Received an invalid selectorIndex (0): %d\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        case 1:
            switch (context->selectorIndex) {
                case DEPOSIT:
                    set_token_amount_ui(msg, context);
                    break;
                case DEPOSIT_ETH:
                    set_eth_amount_ui(msg, context);
                    break;
                case WITHDRAW:
                    set_v_token_amount_ui(msg, context);
                    break;
                // Keep this
                default:
                    PRINTF("Received an invalid selectorIndex (1): %d\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex: %d\n", msg->screenIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}
