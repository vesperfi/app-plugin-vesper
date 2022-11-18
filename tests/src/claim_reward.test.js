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
    "[Nano " + model.letter + "] Claim rewards from the veWBTC-DAI pool",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0x6c0d0285673ffeb208af9453f33d1de4ded3da15ccbeef2ac732504295100d96
      const serializedTx = txFromEtherscan(
        "0x02f8910182011b839896808502540be400832911c89492659c1293f74d76249a72601c9bf36bf6d738ca80a4d279c1910000000000000000000000000a00d785fb387daa35509af75dc8b8517b842a55c080a0f8deb13c2dd7548a3dff98f054343007589df49b1f91e2a980a365aea651c170a009089b7e59d32a4f04ad73f8878bbb860a6fea5c81384f95fc59e86820f2e589"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 4 : 4;
      await sim.navigateAndCompareSnapshots(".", model.name + "_claim_reward", [
        right_clicks,
        0,
      ]);

      await tx;
    })
  );
});
