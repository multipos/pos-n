// Copyright 2016 PoS-N Coin Devs

#ifndef PSN_COLORS_H
#define PSN_COLORS_H

#include <vector>
#include <map>
#include <string>
#include <limits>

#include <stdint.h>
#include <assert.h>
#include <stdio.h>

// define to 1 for building testnet, 0 for main net
#define TESTNET_BUILD 0

// define to 1 to allow rpc to send multiple outputs to same address
#define ALLOW_DUPLICATE_DESTINATIONS 0

// N_COLORS includes COLOR_P00
#define N_COLORS 18
#define N_COLOR_BYTES 1

// proof model
#define PURE_POW 0
#define PURE_POS 1
#define MIXED_POW_POS 2
#define PROOF_MODEL PURE_POS

// does reward depend on age of coin?
#define COINAGE_DEPENDENT_POS 0

// for GetBalance, GetStake in the wallet model
#define N_GUI_OVERVIEW_COLORS 1


// this is a multiple of the MIN_TX_FEE
#define VERY_HIGH_FEE 1000

// Main Net (1) PUBKEY, (2) SCRIPT & Test Net (3) PUBKEY, (4) SCRIPT
#define N_VERSIONS 4


extern bool fTestNet;

//////////////////////////////////////////////////////////////////////

// forks


// networking
extern unsigned short const TOR_PORT;

extern unsigned short const P2P_PORT;
extern unsigned short const P2P_PORT_TESTNET;

extern unsigned short const DEFAULT_PROXY;
extern unsigned short const DEFAULT_PROXY_TESTNET;

// rpc
extern unsigned short const RPC_PORT;
extern unsigned short const RPC_PORT_TESTNET;

// pchMessageStart
extern unsigned char pchMessageStart[4];
extern unsigned char pchMessageStartTestnet[4];


// extern const int N_COLORS;
// extern const int N_COLOR_BYTES;
extern const int64_t BASE_COIN;
extern const int64_t BASE_CENT;

extern const int BASE_FEE_EXPONENT;
extern const int64_t BASE_COIN;
extern const int64_t BASE_CENT;
extern const int BASE_FEE_EXPONENT;
// extern const bool COINAGE_DEPENDENT_POS;
extern std::string ADDRESS_DELIMETER;

// Complete enum of currencies
// These are the indices used throughout the code
typedef enum {
        PSN_COLOR_P00 = 0, PSN_COLOR_P01, PSN_COLOR_P02,
        PSN_COLOR_P03, PSN_COLOR_P04, PSN_COLOR_P05,
        PSN_COLOR_P06, PSN_COLOR_P07, PSN_COLOR_P08,
        PSN_COLOR_P09, PSN_COLOR_P10, PSN_COLOR_P11,
        PSN_COLOR_P12, PSN_COLOR_P13, PSN_COLOR_P14,
        PSN_COLOR_P15, PSN_COLOR_P16, PSN_COLOR_P17,
        PSN_COLOR_END
} PSN_COLOR ;

typedef enum {
        MAIN_PUBKEY_IDX = 0,
        MAIN_SCRIPT_IDX,
        TEST_PUBKEY_IDX,
        TEST_SCRIPT_IDX
} ADDRESS_VERSION_INDEX ;


extern const int DEFAULT_COLOR;

extern const int DEFAULT_STAKE_COLOR;

extern const int COLOR_UNITS[N_COLORS];

extern const int PSN_COLOR_UNIH;
extern const int PSN_COLOR_PREM;
extern const int PSN_COLOR_DPST;

extern const int aGuiOverviewColors[N_GUI_OVERVIEW_COLORS];
extern std::vector<int> GUI_OVERVIEW_COLORS;

extern const int DIGITS[N_COLORS];
extern const int DECIMALS[N_COLORS];


#if COINAGE_DEPENDENT_POS
extern const int64_t STAKE_MULTIPLIER[N_COLORS];
extern const int64_t MAX_MINT_PROOF_OF_STAKE[N_COLORS];
#else
extern const int64_t BASE_POS_APR[N_COLORS];
extern const int64_t HYPERINFLATION_LIMIT[N_COLORS];
#endif

extern const int64_t MAX_MONEY[N_COLORS];

extern const int FEE_COLOR[N_COLORS];
extern const int64_t MIN_TX_FEE[N_COLORS];
extern const int64_t MIN_RELAY_TX_FEE[N_COLORS];
extern const int64_t COMMENT_FEE_PER_CHAR[N_COLORS];

extern const int64_t OP_RET_FEE_PER_CHAR[N_COLORS];

extern const bool TRANSFERABLE[N_COLORS];

extern const bool SCAVENGABLE[N_COLORS];

extern const int64_t MIN_TXOUT_AMOUNT[N_COLORS];

extern const int64_t MIN_INPUT_VALUE[N_COLORS];

extern const int64_t STAKE_COMBINE_THRESHOLD[N_COLORS];

extern const int MINT_COLOR[N_COLORS];

extern const char *COLOR_TICKER[N_COLORS];
extern const char *COLOR_NAME[N_COLORS];

extern const unsigned char aColorID[N_VERSIONS][N_COLORS][N_COLOR_BYTES];

extern std::vector<std::vector<std::vector<unsigned char> > > COLOR_ID;

extern const int64_t PRIORITY_MULTIPLIER[N_COLORS];

extern const int64_t WEIGHT_MULTIPLIER[N_COLORS];

extern const int64_t POW_SUBSIDY[N_COLORS];

extern std::vector<std::map <std::vector <unsigned char>, int > > MAPS_COLOR_ID;

bool GetColorFromTicker(const std::string &ticker, int &nColorIn);

bool GetTickerFromColor(int nColor, std::string &ticker);

bool CheckColor(int nColor);

int64_t GetCoin(int nColor);
int64_t GetCent(int nColor);

bool CanStake(int nStakeColor);

int GetStakeMinConfirmations(int nColor);

int IsTransferrable(int nColor);

bool SplitQualifiedAddress(const std::string &qualAddress,
                              std::string &address, int &nColor, bool fDebug);

// add b58 compatible bytes of n to end of vch, little byte first
bool AppendColorBytes(int n, std::vector<unsigned char> &vch);

bool ValueMapAllPositive(const std::map<int, int64_t> &mapNet);
bool ValueMapAllZero(const std::map<int, int64_t> &mapNet);

// Returns effectively mapCredit - mapDebit
//   much like vectors would be subtracted.
void FillNets(const std::map<int, int64_t> &mapDebit,
              const std::map<int, int64_t> &mapCredit,
              std::map<int, int64_t> &mapNet);

// minting
int64_t GetTargetSpacing(int nHeight, bool fProofOfStake);
int GetLastFairLaunchBlock();
#if PROOF_MODEL == PURE_POS
int GetLastPoWBlock(int64_t nTime);
int GetFirstPoSBlock(int64_t nTime);
#endif

int64_t GetMaxPoWMint(int nColor);
int64_t GetMaxPoWSubsidy(int nColor, const int64_t nReward);



#endif  // PSN_COLORS_H
