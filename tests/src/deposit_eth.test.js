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
    "[Nano " + model.letter + "] Deposit ETH into the vETH pool",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0xf7e314ee9d50664293193692b8fc73a8637ec59cc7b7016d1b30a5b1319fec45
      const serializedTx = txFromEtherscan(
        "0x02f87701048459682f0085022c1c85ab8304771f94103cc17c2b1586e5cd9bad308690bcd0bbe54d5e8711c8ac8ee2ead584d0e30db0c001a0bf33f59a1642d258bddbf398ef7291f018cc1e2389bf454b04ea867f8b1af78aa00466ed66e58dad452227e05d28a1f8d61575e724f47fd1caa89f0b242b67e563"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 9 : 5;
      await sim.navigateAndCompareSnapshots(".", model.name + "_deposit_eth", [
        right_clicks,
        0,
      ]);

      await tx;
    })
  );
});
