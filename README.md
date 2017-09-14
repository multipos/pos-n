# PoS-N

## About

PoS-N not a coin. It is a blockchain with many coins. It is **PURELY EXPERIMENTAL** and may no currencies on the chain may have any value whatsoever. The developer makes no promises to develop any infrastructure except a seed node and block crawler. Mine these currencies if you want to participate in this experiment.In this sense, it is very similar to bitcoin, in that the only value at the time of launch is what knowledge may come of the experiment.

If you want to mine coins on this chain only to dump them as soon as they hit an exchange **YOU WILL HAVE TO DO THE WORK TO GET THE COINS YOU MINE LISTED**. The goal of the experiment is **NOT** to make money for miners. Period. Miners need to form communities around their coins and do the work to make money. Remember: It is an experiment, and the only thing I will attempt to do is to make sure mining works, is fair, and that PoS will work.

So what is the experiment? The experiment is to find out the optimal interest rate for a proof of stake coin. Miners will mine the chain, and mine a different random currency with a different stake rate. Staking begins after the mining period.  The interest rates of the various coins in the random rotation follow the Fibonacci sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597.

Because Every block miners (scrypt) will randomly mine one of many currencies, I don't know how pools will react or properly distribute currencies according to mining power.  I anticipate some confusion and perhaps errors with distribution from pools, so mine to pools at your own risk, and please do not blame pool operators for any problems in distribution. It will most likely not be their fault. Pools may or may not want to bother trying to adapt pool software to this chain. Whether they do or not has the same effect on everyone mining, so the process will be still be fair.

Mining is a flat reward for the currencies so that the last block will earn the same as the first block. Currencies in the random rotation are awarded 9375 coins per block. All currencies in the random rotation will have 63,000,000 money supplies at the end of mining.

I had to pick a currency for the GUI (which I needed for so-called "fair launch", this currency has the ticker of PSN, named after the chain. It is merely a matter of convenience and because it has an inflation rate of 1%, being the lowest number of the Fibonacci sequence. If you want to mine only this currency or any currency, set the *mineonly* flag to PSN in the config file. Even if you don't set the flag, you will randomly mine PSN.

My goal in choosing PSN for the GUI was not to play favorites, but to have a default currency for noobs who want to look at a GUI wallet. I also guessed that 1% interest rate may not be a great incentive for adoption, and that choosing this single currency for the GUI for launch would give it a significant advantage over other currencies.  The GUI should support as many currencies as needed in time, or specialized GUIs could be created for each currency if communities evolve around them.

All currencies are equals by the CLI client, which is what most experienced miners can use the CLI to create addresses and send coins. Instructions will be posted in time, or users can consult the *help* output of the CLI.

This coin is experimental, but in observation of good practice **DOES** have a fair launch period of one hour, or 300 blocks, of 1 coin per block. The regular PoW block reward is 9375 coins. Also in observation of fair launch practices, a windows wallet is available (necessitating having a default currency in the GUI).


## Mining

This coin can be mined with standard miners:

```./minerd -a scrypt -u RPCUSERNAME -p RPCPASSWORD -o 127.0.0.1:50414```


## Stats

* Tickers of Random Rotation: PSN, P03 ... P16
* Algo: scrypt
* PoW Block Times: 12 Seconds
* PoS Block Times: 2 Minutes
* Max PoW for Random Rotation: 63,000,000
* Block Reward: 9375
* PoW Duration: 14 days
* Fair Launch Duration: 300 Blocks (1 hr), 
* Fair Launch Block Reward: 1 coin
* PoS Rewards (First 3 years):  1, 2, 3, 5 ... 1597%
* PoS Rewards (After 3 years): Lower of initial interest rate or 4%
* Peer-to-Peer Port: 29861 (testnet 29862)
* RPC Port: 50414 (testnet 50415)
* Net Magic: 0xef, 0xff, 0xfb, 0xed (testnet 0xf9, 0xbb, 0xfd, 0xff)
