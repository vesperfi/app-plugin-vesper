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
      "] Deposit WBTC into the veWBTC-DAI pool and claim rewards",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0x964b9daaa269140241101bf219d89bfe4a6f47dd6e925c388859bcb9453914d9
      const serializedTx = txFromEtherscan(
        "0x02f892018216128459682f00850b86503cc083112b1e947a74b6d3a07d3249ea2fbb58e47f0daf6d6a2ebf80a4a941a90e0000000000000000000000000000000000000000000000000000000283baec00c080a0a98d764b9c333084e4186002eb3a85950db0a72934211aa55fff18bad8ee0102a07c914552d5f74d6cf09c40c0bf5c87020cb8dd9e731699621a5b4e95f1de412a"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 6 : 5;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_deposit_token_and_claim",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
