const metadata = require("vesper-metadata");

// The deposit asset decimals should be present in vesper-metadata or read from the contracts.
const assetDecimals = {
  alUSD: 18,
  APE: 18,
  AVAX: 18,
  BNB: 18,
  BUSD: 18,
  DAI: 18,
  "DAI.e": 18,
  DPI: 18,
  ETH: 18,
  FEI: 18,
  FRAX: 18,
  LINK: 18,
  MATIC: 18,
  mUSD: 18,
  QI: 18,
  stETH: 18,
  UNI: 18,
  USDC: 6,
  "USDC.e": 6,
  USDT: 6,
  VSP: 18,
  WBTC: 8,
  "WBTC.e": 8,
  WETH: 18,
  "WETH.e": 18,
};

function toPluginFormat(pool) {
  const address = pool.address
    .slice(2)
    .toLowerCase()
    .match(/../g)
    .map((byte) => `0x${byte}`)
    .join(",");

  const decimals = assetDecimals[pool.asset];
  if (!decimals) {
    throw new Error(`No decimals found for ${pool.asset}`);
  }

  return [
    `{${address}}`,
    `"${pool.symbol.slice(0, 11)}"`,
    `"${pool.asset.slice(0, 11)}"`,
    decimals,
  ].join(",");
}

const pools = metadata.pools.filter((pool) => pool.stage != "retired");

console.log(`// Generated from vesper-metadata@${metadata.version}`);
console.log('#include "vesper_plugin.h"');
console.log(`#define NUM_VESPER_POOLS ${pools.length}`);
console.log("const pool_metadata_t VESPER_POOLS[NUM_VESPER_POOLS] = {");
pools.map(toPluginFormat).forEach((pool) => console.log(`{${pool}},`));
console.log("};");
