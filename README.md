# app-plugin-vesper

This repo contains the Vesper plugin for Ledger Nano S, S Plus and X devices.

It allows users to safely interact with the Vesper smart contracts by parsing the transaction data and displaying its content in a human readable way.

## Development

See the Ledger [plugin guide](https://developers.ledger.com/docs/dapp/nano-plugin/overview/) in order to better understand the flow and the context for plugins.

### Clone repositories and update files

Locally clone the Ethereum SDK:

```sh
git clone https://github.com/LedgerHQ/ethereum-plugin-sdk
```

Create a `plugin-dev` folder and clone the Ledger `app-ethereum` and `plugin-tools`.

```sh
mkdir plugin_dev
cd plugin_dev
git clone https://github.com/LedgerHQ/app-ethereum
git clone https://github.com/LedgerHQ/plugin-tools
```

Update the `.gitmodules` file in `app-ethereum` to clone the Ethereum SDK over HTTP:

```diff
-   path = ethereum-plugin-sdk
-   url = git@github.com:LedgerHQ/ethereum-plugin-sdk.git
+   path = ethereum-plugin-sdk
+   url = https://github.com/LedgerHQ/ethereum-plugin-sdk
```

Update the `docker-compose` file in `plugin-tools` to adapt it to this repo folder structure:

```diff
     image: ghcr.io/ledgerhq/ledger-app-builder/ledger-app-builder:latest
     volumes:
       - ..:/plugin_dev
+      - ../..:/plugin_dev/app-plugin-vesper
     working_dir: /plugin_dev
```

### Run tests

To run the tests, start the container first:

```sh
cd plugin-dev/plugin-tools
./start.sh
```

Then, within the container, compile the app locally for testing:

```sh
cd app-plugin-vesper/tests
./build_local_test_elfs.sh
```

Finally, back outside of the container and in the `tests` folder, install the test dependencies and run the tests:

```sh
cd tests
yarn install
yarn test
```

After making any change to the source code, recompile the app within the container and re-run the tests outside.
