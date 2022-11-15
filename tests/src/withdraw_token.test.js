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
    "[Nano " + model.letter + "] Withdraw WBTC from the vWBTC pool",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0x8e00e4b5c4db3cefccdd8eac93fcb50d3f4131d243db1722062989ae69a1d68b
      const serializedTx = txFromEtherscan(
        "0x02f89001308459682f00850511a5c6e183072354944b2e76ebbc9f2923d83f5fbde695d8733db1a17b80a42e1a7d4d000000000000000000000000000000000000000000000000004b4a7ec22d4345c080a0e62ec85d8a01e9dc55098123a2b60058ae006bc1885c3fd3f759fabb0395982ba01addaf073ed2b21d72c8556f640d11ae0d3c9dc9c58ffad3adf8cd807c802a8c"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 9 : 5;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_withdraw_token",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
