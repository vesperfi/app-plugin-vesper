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
    "[Nano " + model.letter + "] Withdraw ETH from the vETH pool",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0x1004d0426b8e83a36d3a02bebea81cfe4eb5146df8867f8255c4678d8ad3a307
      const serializedTx = txFromEtherscan(
        "0x02f890012f8459682f008504d6e0b7448305fc0594103cc17c2b1586e5cd9bad308690bcd0bbe54d5e80a4f14210a600000000000000000000000000000000000000000000000036871d5acd2b99b6c080a056440fdd939e118945cac1134b98ba6f0ef69988da4c2119f2235a853eb2c78da0194469ca3d567b755190e53114b3a2c057f8d22287400944a813c528b8261677"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 9 : 5;
      await sim.navigateAndCompareSnapshots(".", model.name + "_withdraw_eth", [
        right_clicks,
        0,
      ]);

      await tx;
    })
  );
});
