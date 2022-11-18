#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

// Name of the plugin.
#define PLUGIN_NAME "Vesper"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
typedef enum {
    DEPOSIT = 0,
    DEPOSIT_AND_CLAIM,
    DEPOSIT_ETH,
    DEPOSIT_ETH_AND_CLAIM,
    WITHDRAW,
    WITHDRAW_AND_CLAIM,
    WITHDRAW_ETH,
    WITHDRAW_ETH_AND_CLAIM,
    CLAIM_REWARD,
    SIMPLE_MIGRATE,
    SIMPLE_MIGRATE_WITH_PERMIT,
} selector_t;

// Enumeration used to parse the smart contract data.
typedef enum {
    AMOUNT = 0,
    POOL_FROM,
    POOL_TO,
    UNEXPECTED_PARAMETER,
} parameter;

// Number of selectors defined in this plugin. Should match the enum `selector_t`.
#define NUM_SELECTORS 15

extern const uint32_t VESPER_SELECTORS[NUM_SELECTORS];

typedef struct pool_metadata_t {
    uint8_t chain_id;                   // Pool chain id
    uint8_t address[ADDRESS_LENGTH];    // Pool address
    char pool_ticker[MAX_TICKER_LEN];   // Pool ticker
    char token_ticker[MAX_TICKER_LEN];  // Deposit token ticker
    uint8_t decimals;                   // Deposit token decimals
    uint8_t rewards[ADDRESS_LENGTH];    // Rewards contract address
} pool_metadata_t;

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_t {
    // For display.
    const pool_metadata_t *pool_metadata;
    uint8_t amount[INT256_LENGTH];

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);