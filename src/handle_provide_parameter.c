#include "vesper_plugin.h"

static void handle_deposit_and_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    switch (context->selectorIndex) {
        case DEPOSIT:
        case DEPOSIT_AND_CLAIM:
        case WITHDRAW:
        case WITHDRAW_AND_CLAIM:
        case WITHDRAW_ETH:
        case WITHDRAW_ETH_AND_CLAIM:
            handle_deposit_and_withdraw(msg, context);
            break;
        // TODO add more methods
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}