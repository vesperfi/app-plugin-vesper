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
      "] Migrate funds from the vETH pool to the vaETH pool using EIP-2612",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0x765c9cfdc27a330d16abaa1920edbc96f8e3d2453057f9970d65995df758666b
      const serializedTx = txFromEtherscan(
        "0x02f901930181878459682f008507e8020ac5830e7d7294ca5a5b83fc6ed8577102e0b3b19379683f33b34680b90124c3acd6b6000000000000000000000000103cc17c2b1586e5cd9bad308690bcd0bbe54d5e000000000000000000000000d1c117319b3595fbc39b471ab1fd485629eb05f20000000000000000000000007fac097afebc44f33e9d597768c33d6988f686be000000000000000000000000ca5a5b83fc6ed8577102e0b3b19379683f33b34600000000000000000000000000000000000000000000000089d88980e813a3f6000000000000000000000000000000000000000000000000000000006340425a000000000000000000000000000000000000000000000000000000000000001b0c451c6b57480991460a15c6acc155ccb8d5238766730cb4b5b4c14a87cf031e59b29c43aa16e5ac8ae31e007dd7d161bbf729c5ad6e044c8f473a7c1da7452fc080a0b782dad19f5648301fa0243ad0d6b953dec1c047dc16bdbb95a04f7a6858aeeaa01a4b16efabc8cc5353d323cf65db30a72ce454aa675a1db08e0d1b2c42c0387d"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 10 : 6;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_simple_migrate_with_permit",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
