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
      "] Migrate funds from the vETH pool to the vaETH pool",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0xbab9aa547e35ecbc59d762c1cd6698ab1f71311c293fcf4e9603844febfbccca
      const serializedTx = txFromEtherscan(
        "0x02f8d10118849502f9008506fc23ac00830d5f1e94ca5a5b83fc6ed8577102e0b3b19379683f33b34680b864d24612e9000000000000000000000000103cc17c2b1586e5cd9bad308690bcd0bbe54d5e000000000000000000000000d1c117319b3595fbc39b471ab1fd485629eb05f20000000000000000000000000000000000000000000000000dd380b833fafee2c001a015c5e662633f2f9989f346c175fbbb3be1bfdc8a6f098c0bb2e835a52dcd6688a05f1d064d1f739df8e60302561afee5b4716116bf6be7ced1e53517459d8dc07a"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 8 : 6;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_simple_migrate",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
