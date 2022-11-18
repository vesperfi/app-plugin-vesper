const axios = require("axios");

function byChainAndBirthBlock(a, b) {
  if (a.chainId !== b.chainId) {
    return a.chainId - b.chainId;
  }
  return b.birthblock - a.birthblock;
}

function toBytesArray(address) {
  return address
    .slice(2)
    .toLowerCase()
    .match(/../g)
    .map((byte) => `0x${byte}`)
    .join(",");
}

function toPluginFormat(pool) {
  return [
    pool.chainId,
    `{${toBytesArray(pool.address)}}`,
    `"${pool.symbol.slice(0, 11)}"`,
    `"${pool.asset.symbol.slice(0, 11)}"`,
    pool.asset.decimals,
    `{${toBytesArray(pool.rewardsContractAddress || "0x00")}}`,
  ].join(",");
}

const vesperApiUrls = [
  "https://api.vesper.finance/pools?stages=prod+orbit+beta",
];

Promise.all(vesperApiUrls.map((url) => axios.get(url))).then(function (
  responses
) {
  const pools = responses
    .map((response) => response.data)
    .flat()
    .sort(byChainAndBirthBlock);

  console.log(`// Auto-generated on ${new Date().toISOString()}`);
  console.log('#include "vesper_plugin.h"');
  console.log(`#define NUM_VESPER_POOLS ${pools.length}`);
  console.log("const pool_metadata_t VESPER_POOLS[NUM_VESPER_POOLS] = {");
  pools.map(toPluginFormat).forEach((pool) => console.log(`{${pool}},`));
  console.log("};");
});
