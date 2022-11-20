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

static void handle_simple_migrate(ethPluginProvideParameter_t *msg,
                                  context_t *context,
                                  bool permit) {
    switch (context->next_param) {
        case TOKEN_A:
            copy_address(context->pool_from, msg->parameter, sizeof(context->pool_from));
            context->next_param = TOKEN_B;
            break;
        case TOKEN_B:
            copy_address(context->pool_to, msg->parameter, sizeof(context->pool_to));
            context->next_param = permit ? FROM : AMOUNT;
            break;
        case FROM:
            context->next_param = TO;
            break;
        case TO:
            context->next_param = AMOUNT;
            break;
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

    switch (context->selectorIndex) {
        case DEPOSIT:
        case DEPOSIT_AND_CLAIM:
        case WITHDRAW:
        case WITHDRAW_AND_CLAIM:
        case WITHDRAW_ETH:
        case WITHDRAW_ETH_AND_CLAIM:
            handle_deposit_and_withdraw(msg, context);
            break;
        case CLAIM_REWARD:
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        case SIMPLE_MIGRATE:
            handle_simple_migrate(msg, context, false);
            break;
        case SIMPLE_MIGRATE_WITH_PERMIT:
            handle_simple_migrate(msg, context, true);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}