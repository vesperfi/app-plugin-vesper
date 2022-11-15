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
      "] Withdraw WBTC from the veWBTC-DAI pool and claim rewards",
    zemu(model, async (sim, eth) => {
      // Tx hash: 0x0158b83473db6f9b601d049d6b9ff7c9dcb967cefbf74d21ca0ac0279fbb86c9
      const serializedTx = txFromEtherscan(
        "0xf88a81d28507e498f300830a4330947a74b6d3a07d3249ea2fbb58e47f0daf6d6a2ebf80a4448a104700000000000000000000000000000000000000000000000001470ab6703bfe3226a0c990c3f922aaef9fa67c9a00831347ecabc14458cf00134691984a0a3fc897cba050be3bb292d6f036f3fcbbe88e332795a8fad4c55b00599d42553ca45828295c"
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 7 : 5;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_withdraw_token_and_claim",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
