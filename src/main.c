/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "os.h"
#include "cx.h"

#include "vesper_plugin.h"

// List of selectors supported by this plugin.
// Pools calls:
static const uint32_t CLAIM_REWARD_SELECTOR = 0xd279c191;
static const uint32_t DEPOSIT_SELECTOR = 0xb6b55f25;
static const uint32_t DEPOSIT_AND_CLAIM_SELECTOR = 0xa941a90e;
static const uint32_t DEPOSIT_ETH_SELECTOR = 0xd0e30db0;
static const uint32_t DEPOSIT_ETH_AND_CLAIM_SELECTOR = 0xc9d7f672;
static const uint32_t SIMPLE_MIGRATE_SELECTOR = 0xd24612e9;
static const uint32_t SIMPLE_MIGRATE_WITH_PERMIT_SELECTOR = 0xc3acd6b6;
static const uint32_t WITHDRAW_SELECTOR = 0x2e1a7d4d;
static const uint32_t WITHDRAW_AND_CLAIM_SELECTOR = 0x448a1047;
static const uint32_t WITHDRAW_ETH_SELECTOR = 0xf14210a6;
static const uint32_t WITHDRAW_ETH_AND_CLAIM_SELECTOR = 0x6d43aed8;
// VSP locking/bonding calls:
static const uint32_t CLAIM_REWARDS_SELECTOR = 0xef5cfb8c;
static const uint32_t LOCK_SELECTOR = 0x1338736f;
static const uint32_t MINT_SELECTOR = 0x156e29f6;
static const uint32_t UNLOCK_SELECTOR = 0x6b41674c;

// Array of all the different Vesper selectors. Make sure this follows the same order as the
// enum defined in `vesper_plugin.h`
const uint32_t VESPER_SELECTORS[NUM_SELECTORS] = {CLAIM_REWARD_SELECTOR,
                                                  DEPOSIT_SELECTOR,
                                                  DEPOSIT_AND_CLAIM_SELECTOR,
                                                  DEPOSIT_ETH_SELECTOR,
                                                  DEPOSIT_ETH_AND_CLAIM_SELECTOR,
                                                  SIMPLE_MIGRATE_SELECTOR,
                                                  SIMPLE_MIGRATE_WITH_PERMIT_SELECTOR,
                                                  WITHDRAW_SELECTOR,
                                                  WITHDRAW_AND_CLAIM_SELECTOR,
                                                  WITHDRAW_ETH_SELECTOR,
                                                  WITHDRAW_ETH_AND_CLAIM_SELECTOR,
                                                  CLAIM_REWARDS_SELECTOR,
                                                  LOCK_SELECTOR,
                                                  MINT_SELECTOR,
                                                  UNLOCK_SELECTOR};

// Function to dispatch calls from the ethereum app.
void dispatch_plugin_calls(int message, void *parameters) {
    switch (message) {
        case ETH_PLUGIN_INIT_CONTRACT:
            handle_init_contract(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_PARAMETER:
            handle_provide_parameter(parameters);
            break;
        case ETH_PLUGIN_FINALIZE:
            handle_finalize(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_INFO:
            handle_provide_token(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_ID:
            handle_query_contract_id(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            handle_query_contract_ui(parameters);
            break;
        default:
            PRINTF("Unhandled message %d\n", message);
            break;
    }
}

void handle_query_ui_exception(unsigned int *args) {
    switch (args[0]) {
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            ((ethQueryContractUI_t *) args[1])->result = ETH_PLUGIN_RESULT_ERROR;
            break;
        default:
            break;
    }
}

// Calls the ethereum app.
void call_app_ethereum() {
    unsigned int libcall_params[3];
    libcall_params[0] = (unsigned int) "Ethereum";
    libcall_params[1] = 0x100;
    libcall_params[2] = RUN_APPLICATION;
    os_lib_call((unsigned int *) &libcall_params);
}

// Weird low-level black magic. No need to edit this.
__attribute__((section(".boot"))) int main(int arg0) {
    // Exit critical section
    __asm volatile("cpsie i");

    // Ensure exception will work as planned
    os_boot();

    // Try catch block. Please read the docs for more information on how to use those!
    BEGIN_TRY {
        TRY {
            // Low-level black magic.
            check_api_level(CX_COMPAT_APILEVEL);

            // Check if we are called from the dashboard.
            if (!arg0) {
                // Called from dashboard, launch Ethereum app
                call_app_ethereum();
                return 0;
            } else {
                // Not called from dashboard: called from the ethereum app!
                const unsigned int *args = (const unsigned int *) arg0;

                // If `ETH_PLUGIN_CHECK_PRESENCE` is set, this means the caller is just trying to
                // know whether this app exists or not. We can skip `dispatch_plugin_calls`.
                if (args[0] != ETH_PLUGIN_CHECK_PRESENCE) {
                    dispatch_plugin_calls(args[0], (void *) args[1]);
                }
            }
        }
        CATCH_OTHER(e) {
            switch (e) {
                // These exceptions are only generated on handle_query_contract_ui()
                case 0x6502:
                case EXCEPTION_OVERFLOW:
                    handle_query_ui_exception((unsigned int *) arg0);
                    break;
                default:
                    break;
            }
            PRINTF("Exception 0x%x caught\n", e);
        }
        FINALLY {
            // Call `os_lib_end`, go back to the ethereum app.
            os_lib_end();
        }
    }
    END_TRY;

    // Will not get reached.
    return 0;
}
