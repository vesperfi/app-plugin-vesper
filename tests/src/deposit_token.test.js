import "core-js/stable";
import "regenerator-runtime/runtime";
import {
  waitForAppScreen,
  zemu,
  nano_models,
  txFromEtherscan,
} from "./test.fixture";

nano_models.forEach(function (model) {
  test(
    "[Nano " + model.letter + "] Deposit WBTC into the vWBTC pool",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0xe8671f63531da777e07807598820b33905755e8ffccb68cf773a4fc0c8ca2a9e
      const serializedTx = txFromEtherscan(
        "0x02f8900116843c847a38851401fc93fc83051821944b2e76ebbc9f2923d83f5fbde695d8733db1a17b80a4b6b55f2500000000000000000000000000000000000000000000000000000000004f13d1c001a0c0ed17ba1ac8e18dcc12a85f8eedb68510e69424b0eac681b5d09cb3ab80f5caa04741fae690547d531f6553edfd99de8c28925422ebe4fb87fdaae9e3b0cd77ee"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 7 : 5;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_deposit_token",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
