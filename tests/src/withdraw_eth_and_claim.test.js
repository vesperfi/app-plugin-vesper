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
    "[Nano " +
      model.letter +
      "] Withdraw ETH from the veETH-DAI pool and claim rewards",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0xb02ec3c3e7dc11e4f4fe1bf167c7fb819f6f8bf86f87a109a1b5850b22a5fa7a
      const serializedTx = txFromEtherscan(
        "0x02f89001188451f4d5c08509641b7510831d80e094a89566489e932a2d334b9eff7884feb21a07d2b380a46d43aed80000000000000000000000000000000000000000000000140ec80fa7ee880000c080a09adb334c99567dfe3f53ba69831867cca5feca057f2ad212e8fa5b84133e7b87a04c6888d0fc106f43dfd394fbab7215df6c899b662c11c76ed0c7488e5d83ecfb"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 8 : 5;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_withdraw_eth_and_claim",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
