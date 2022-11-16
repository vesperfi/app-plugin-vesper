import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx, nano_models } from "./test.fixture";
import { ethers } from "ethers";
import { parseEther, parseUnits } from "ethers/lib/utils";

const contractAddr = "0xa89566489e932a2d334b9eff7884feb21a07d2b3";
const pluginName = "vesper";
const testNetwork = "ethereum";
const abi_path =
  `../networks/${testNetwork}/${pluginName}/abis/` + contractAddr + ".json";
const abi = require(abi_path);

nano_models.forEach(function (model) {
  test(
    "[Nano " +
      model.letter +
      "] Deposit ETH into the veETH-DAI pool and claim rewards",
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);
      const { data } = await contract.populateTransaction.depositAndClaim();

      // Get the generic transaction template
      let unsignedTx = genericTx;
      unsignedTx.to = contractAddr;
      unsignedTx.data = data;
      unsignedTx.value = parseEther("0.1");

      // Create serializedTx and remove the "0x" prefix
      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

      // Wait for the application to actually load and parse the transaction
      await waitForAppScreen(sim);

      // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
      const right_clicks = model.letter === "S" ? 5 : 5;
      await sim.navigateAndCompareSnapshots(
        ".",
        model.name + "_deposit_eth_and_claim",
        [right_clicks, 0]
      );

      await tx;
    })
  );
});
