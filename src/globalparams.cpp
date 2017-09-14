// Copyright 2016 PoS-N Coin Devs

#include "globalparams.h"

//////////////////////////////////////////////////////////////////////
///
/// Forks
///
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///
/// Network Constants
///
//////////////////////////////////////////////////////////////////////

// network constants (used to be in netbase.cpp)

// random port number, not used much
unsigned short const TOR_PORT = 17718;

unsigned short const P2P_PORT = 29861;
unsigned short const P2P_PORT_TESTNET = 29862;

unsigned short const DEFAULT_PROXY = 9050;
unsigned short const DEFAULT_PROXY_TESTNET = 19050;

// rpc
unsigned short const RPC_PORT = 50414;
unsigned short const RPC_PORT_TESTNET = 50415;

// The message start string is designed to be unlikely to occur in normal data.
// The characters are rarely used upper ASCII, not valid as UTF-8, and produce
// a large 4-byte int at any alignment.
unsigned char pchMessageStart[4] = { 0xef, 0xff, 0xfb, 0xed };
unsigned char pchMessageStartTestnet[4] = { 0xf9, 0xbb, 0xfd, 0xff };


//////////////////////////////////////////////////////////////////////
///
/// Minting Constants
///
//////////////////////////////////////////////////////////////////////
// target spacings
static const unsigned int nTargetSpacingPoS = 120;  // 2 min/block
static const unsigned int nTargetSpacingPoW = 12;   // 5 block/min
static const unsigned int nTargetSpacingPoSTestNet = 20;  // 20 sec/plock
static const unsigned int nTargetSpacingPoWTestNet = 6;   // 6 sec/block
// block cutoffs
static const int LAST_FAIR_LAUNCH_BLOCK = 300;            // 60 min
static const int LAST_FAIR_LAUNCH_BLOCK_TESTNET = 30;     // 6 min


//////////////////////////////////////////////////////////////////////
///
/// PoS Constants
///
//////////////////////////////////////////////////////////////////////
#if PROOF_MODEL == PURE_POS
// must have a last PoW block if it is to be pure PoS
// optional overlap between PoW and PoS
int LAST_POW_BLOCK = 101100;  // 300 + 14 * 5 * 24 * 60 = 14 day
int FIRST_POS_BLOCK = 101101;  // LAST_POW_BLOCK + 1
static const int LAST_POW_BLOCK_TESTNET = 101100;
static const int FIRST_POS_BLOCK_TESTNET = 81;
#elif PROOF_MODEL == MIXED_POW_POS
//
#endif  // PROOF_MODEL


//////////////////////////////////////////////////////////////////////
///
/// Money Constants
///
//////////////////////////////////////////////////////////////////////

// make sure these are consistent with nStakeMinAge
// 8 hr at 2 min block times
static const int nStakeMinConfirmations = 240;
// 10 min at 2 min block times
static const int nStakeMinConfirmationsTestnet = 5;

// avoid counting zeros
const int64_t BASE_COIN = 100000000;
const int64_t BASE_CENT = 1000000;

// different currencies (colored coins) have different money supplies
// fees are charged in the currency of the transaction
// const int BASE_FEE_EXPONENT = 5;

// some systems will want to multiply coinage by an interest rate
// posncoin has a fixed and money supply dependent rewards
// const bool COINAGE_DEPENDENT_POS = false;

// for qualified addresses with currency as name
std::string ADDRESS_DELIMETER = "_";

// different currencies may have different divisibilities
const int64_t COIN[N_COLORS] = { 0, BASE_COIN, BASE_COIN,
                             BASE_COIN, BASE_COIN, BASE_COIN,
                             BASE_COIN, BASE_COIN, BASE_COIN,
                             BASE_COIN, BASE_COIN, BASE_COIN,
                             BASE_COIN, BASE_COIN, BASE_COIN,
                             BASE_COIN, BASE_COIN, BASE_COIN };

const int64_t CENT[N_COLORS] = { 0, BASE_CENT, BASE_CENT,
                             BASE_CENT, BASE_CENT, BASE_CENT,
                             BASE_CENT, BASE_CENT, BASE_CENT,
                             BASE_CENT, BASE_CENT, BASE_CENT,
                             BASE_CENT, BASE_CENT, BASE_CENT,
                             BASE_CENT, BASE_CENT, BASE_CENT };


// related to max supply
const int DIGITS[N_COLORS] = { 0, 10, 10, 10, 10, 10, 10, 10, 10,
                               0, 10, 10, 10, 10, 10, 10, 10, 10 };


// related to divisibility in GUI
// should be consistent with behavior due to COLOR_UNITS
const int DECIMALS[N_COLORS] = { 0, 4, 4, 4, 4, 4, 4, 4, 4,
                                 0, 4, 4, 4, 4, 4, 4, 4, 4 };

// this test is here to help with consistency
// but it could be that there could be a mix of coinage dependent and not
#if COINAGE_DEPENDENT_POS
// colored coins are generated based on relative preponderance
// these are in order of PSN_COLOR
// if a currency can't mint, then this value is not relevant
const int64_t STAKE_MULTIPLIER[N_COLORS] = { };

#else
// If a currency can't mint, then this value is not relevant.
// These are somewhat like markers, and are
//    used for calculations in GetProofOfStakeReward().
// These are best interpreted as annual interest rates in %.
// First coin is redundant placeholder, useful for consistency elsewhere.
// Fibonnacci sequence seemed to have the best properties for this experiment.
// Indexed by mint color (same as stake color for PoS-N)
const int64_t BASE_POS_APR[N_COLORS] = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55,
                                           89, 144, 233, 377, 610, 987, 1597 };
#endif

// IMPORTANT: make sure these values are in smallest divisible units
const int64_t MAX_MONEY[N_COLORS] = { 0,  BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000,
                        BASE_COIN * 2100000000, BASE_COIN * 2100000000 };

/**********************************************************************
 *  A Note on Deposit and Withdrawal Fees
 **********************************************************************
 * Both fees are taken in UNIH. That is, the network takes the fee
 * in UNIH, but deposits and withdrawals are different in structure
 * so the fees come from different places.
 * Deposits - deposit fees are deducted from the DPT outputs. This
 * is possible because the OUTPUTS = INPUTS - FEES, where the
 * units of the terms do not matter. Example: imagine a deposit
 * of 100 UNIH with a fee of 1 UNIH added. In this case
 *     - OUTPUTS = 100 DPT
 *     - INPUTS = 101 UNIH
 *     - FEES = 1 UNIH
 * A miner would then be able to add 1 UNIH to his fees.
 * Withdrawals - withdrawal fees are deducted from the UNIH outputs.
 * Withdrawals are a little more complicated because of interest,
 * so there is another term:
 *    OUTPUTS = INPUTS + INTEREST - FEES
 * Let's imagine that an individual deposited the 100 UNIH, and it
 * collected 11 UNIH in interest, and the transaction cost 1 UNIH
 * in fees. Here
 *      - OUTPUTS = 110 UNIH
 *      - INPUTS = 100 DPT
 *      - INTEREST = 11 UNIH
 *      - FEES = 1 UNIH
 * A miner would be able to add 1 UNIH to his fees.
 * Of course with proof of stake, no miners claim fees, so the
 * network just eats them.
 **********************************************************************/                 

// Fees are complicated. Each currency can be sent with a fee payable in it's
// FEE_COLOR. Most of the times the FEE_COLOR is the transaction currency itself.
// For example, the fee currency for PSN is PSN. However, delegating a fee currency
// is necessary for atomic currencies with a low coin count. They would cease to
// be useful very quickly.
const int FEE_COLOR[N_COLORS] = { (int) PSN_COLOR_P00,  (int) PSN_COLOR_P02,
                                  (int) PSN_COLOR_P02,  (int) PSN_COLOR_P03,
                                  (int) PSN_COLOR_P04,  (int) PSN_COLOR_P05,
                                  (int) PSN_COLOR_P06,  (int) PSN_COLOR_P07,
                                  (int) PSN_COLOR_P08,  (int) PSN_COLOR_P09,
                                  (int) PSN_COLOR_P10,  (int) PSN_COLOR_P11,
                                  (int) PSN_COLOR_P12,  (int) PSN_COLOR_P13,
                                  (int) PSN_COLOR_P14,  (int) PSN_COLOR_P15,
                                  (int) PSN_COLOR_P16,  (int) PSN_COLOR_P17 };


// MIN_TX_FEE, MIN_RELAY_TX_FEE, and OP_RET_FEE_PER_CHAR are in units of the **FEE_COLOR**

// $2 M cap -> ~$0.001 tx fee
// if cap grows to $20 B, then readjust
// min tx fees are *not* weighted by a priority multiplier
const int64_t MIN_TX_FEE[N_COLORS] = { 0, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100 };


const int64_t MIN_RELAY_TX_FEE[N_COLORS] = { 0, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100,
                15 * BASE_CENT / 100, 15 * BASE_CENT / 100, 15 * BASE_CENT / 100 };

// (0.015 per byte)
const int64_t COMMENT_FEE_PER_CHAR[N_COLORS] = { 0, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000,
                                (BASE_CENT * 15) / 1000, (BASE_CENT * 15) / 1000 };


// op returns can be big, but they are expensive (0.015 per byte)
const int64_t OP_RET_FEE_PER_CHAR[N_COLORS] = { 0, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000,
                                (BASE_CENT * 23) / 1000, (BASE_CENT * 23) / 1000 };


// Can the currency be transferred?
// Some currencies should not be transferrable because their use is restricted
// or depends on tokenized privileges. For example DPT can only be interconverted
// with UNIH, and relies on PREM for conversion from UNIH to DPT.
const bool TRANSFERABLE[N_COLORS] = { 0, 1, 1, 1, 1, 1, 1, 1, 1,
                                      0, 1, 1, 1, 1, 1, 1, 1, 1 };

// Can the currency be recovered by fee scavenging?
// DPT is not transferrable and is not a fee delegate. Therefore, there is no
// opportunity to lose it such that it could then be scavenged.
// Withdrawal fees may not be claimed, but these are taken in UHIH, which
// is scavengable.
const bool SCAVENGABLE[N_COLORS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                     0, 0, 0, 0, 0, 0, 0, 0, 0 };



// only used in GUI
const int64_t MIN_TXOUT_AMOUNT[N_COLORS] = {0, BASE_CENT / BASE_CENT,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100 };


// user configurable
const int64_t MIN_INPUT_VALUE[N_COLORS] = {0,  BASE_CENT / BASE_CENT,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100,
                                      BASE_CENT / 100, BASE_CENT / 100 };


// combine threshold for creating coinstake
// if a currency can't stake, then this value is not relevant
// IMPORTANT: make sure these values are in smallest divisible units
const int64_t STAKE_COMBINE_THRESHOLD[N_COLORS] = { 0, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000,
                                        BASE_COIN * 1000, BASE_COIN * 1000 };


// what does a given currency mint (see GetProofOfStakeReward)
// these are in order of PSN_COLOR
const int MINT_COLOR[N_COLORS] = { (int) PSN_COLOR_P00, (int) PSN_COLOR_P00,
                                   (int) PSN_COLOR_P02, (int) PSN_COLOR_P03,
                                   (int) PSN_COLOR_P04, (int) PSN_COLOR_P00,
                                   (int) PSN_COLOR_P06, (int) PSN_COLOR_P07,
                                   (int) PSN_COLOR_P08, (int) PSN_COLOR_P00,
                                   (int) PSN_COLOR_P10, (int) PSN_COLOR_P11,
                                   (int) PSN_COLOR_P12, (int) PSN_COLOR_P13,
                                   (int) PSN_COLOR_P14, (int) PSN_COLOR_P15,
                                   (int) PSN_COLOR_P16, (int) PSN_COLOR_P17 };

const char *COLOR_TICKER[N_COLORS] = { "<none>", "PRM", "PSN", "P03", "P04", "XUP",
                                          "P06", "P07", "P08", "DPT", "P10", "P11",
                                          "P12", "P13", "P14", "P15", "P16", "P17" };
                   
const char *COLOR_NAME[N_COLORS] = { "<none>", "PoS-01", "PoS-N",
                                     "PoS-03", "PoS-04", "PoS-05",
                                     "PoS-06", "PoS-07", "PoS-08",
                                     "PoS-09", "PoS-10", "PoS-11",
                                     "PoS-12", "PoS-13", "PoS-14",
                                     "PoS-15", "PoS-16", "PoS-17" };


// these must be unique, except color none (first) is 0 or 0, 0 etc.
// for thousands of currencies, initialize with a loop

// first dimension is indexed by ADDESS_VERSION_INDEX enum
// IMPORTANT: don't use aColorID directly, it gets copied to vector COLOR_ID
const unsigned char aColorID[N_VERSIONS][N_COLORS][N_COLOR_BYTES] = {
        /* Main Net PUBKEY */              { {  0}, {  1}, {  2}, {  3}, {  4}, {  5},
                                             {  6}, {  7}, {  8}, {  9}, { 10}, { 11},
                                             { 12}, { 13}, { 14}, { 15}, { 16}, { 17} },
        /* Main Net SCRIPT */              { {118}, {119}, {120}, {121}, {122}, {123},
                                             {124}, {125}, {126}, {127}, {128}, {129},
                                             {130}, {131}, {132}, {133}, {134}, {135} },
        /* Test Net PUBKEY */              { { 36}, { 37}, { 38}, { 39}, {140}, { 41},
                                             { 42}, { 43}, { 44}, { 45}, { 46}, { 47},
                                             { 48}, { 49}, { 50}, { 51}, { 52}, { 53} },
        /* Test Net SCRIPT */              { {154}, {155}, {156}, {157}, {158}, {159},
                                             {160}, {161}, {162}, {163}, {164}, {165},
                                             {166}, {167}, {168}, {169}, {170}, {171} } };



// COLOR_ID (vector version of 3D aColorID)
std::vector<std::vector<std::vector<unsigned char> > > COLOR_ID(
                       N_VERSIONS, std::vector<std::vector<unsigned char> >(
                            N_COLORS, std::vector<unsigned char>(N_COLOR_BYTES)));

// Think of PRIORITY_MULTIPLIER this way:
//              priority ~ multiplier[color] * value_in * confs
// where value_in is in smallest divisible units (e.g. bitcoin -> satoshi).
// Miners will have to adjust these or have them set dynamically
// from the exchange values.

// PRIORITY_MULTIPLIER also adjusts fee-based prioritization.
// PRIORITY_MULTIPLIER should take into account these differences:
//   - total coin counts of the currencies
//   - differences in exchange values
//   - differences in how COIN is defined for each currency
// TODO: make this adjustable by RPC and configurable in the init.
const int64_t PRIORITY_MULTIPLIER[N_COLORS] = { 0, BASE_COIN, BASE_COIN,
                                            BASE_COIN, BASE_COIN, BASE_COIN,
                                            BASE_COIN, BASE_COIN, BASE_COIN,
                                            BASE_COIN, BASE_COIN, BASE_COIN,
                                            BASE_COIN, BASE_COIN, BASE_COIN,
                                            BASE_COIN, BASE_COIN, BASE_COIN };


// their weights determine how readily they stake
// these are in order of PSN_COLOR
// IMPORTANT: make sure to take money supply into account right here, these are per coin
const int64_t WEIGHT_MULTIPLIER[N_COLORS] = { 0, 1, 1, 1, 1, 1, 1, 1,
                                              0, 1, 1, 1, 1, 1, 1, 1 };


// These are for the premine blocks.
// IMPORTANT: make sure these values are in smallest divisible units
//      the smallest divisble unit of PSN is the integer 1, which is the same as an posntoshi
// There is no premine for this coin.
const int64_t POW_SUBSIDY[N_COLORS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                        0, 0, 0, 0, 0, 0, 0, 0, 0 };


// MAPS_COLOR_ID is to look up values in case there are many currencies
// has this structure: 
//      [ {version1_bytes_color1 : color1, version1_bytes_color2, ...},
//        {version2_bytes_color1 : color1, version2_bytes_color2, ...}, ... ]
std::vector<std::map <std::vector <unsigned char>, int > > MAPS_COLOR_ID;


//////////////////////////////////////////////////////////////////////
///
/// GUI Constants
///
//////////////////////////////////////////////////////////////////////

// The default currency for the gui-less client is NONE.
// These are default currencies for the gui client, where the user
//    will need to have an operational client without any configuration.
const int DEFAULT_COLOR = (int) PSN_COLOR_P02;
const int DEFAULT_STAKE_COLOR = (int) PSN_COLOR_P02;

// For the gui, how divisible is the currency?
// For example, BTC is 3 (BTC, mBTC, uBTC)
const int COLOR_UNITS[N_COLORS] = {0, 3, 3, 3, 3, 3, 3, 3, 3,
                                   0, 3, 3, 3, 3, 3, 3, 3, 3 };


// The GUI can create overview stats for only a few (e.g. 3) currencies.
// These are the default ordering
const int aGuiOverviewColors[N_GUI_OVERVIEW_COLORS] = { PSN_COLOR_P02 };

// A vector is used so that number of currencies may be dynamic.
std::vector<int> GUI_OVERVIEW_COLORS;


//////////////////////////////////////////////////////////////////////
///
/// Currency Methods
///
//////////////////////////////////////////////////////////////////////


bool GetColorFromTicker(const std::string &ticker, int &nColorIn)
{
    nColorIn = (int) PSN_COLOR_P00;
    for (int nColor = 1; nColor < N_COLORS; ++nColor)
    {
           if (std::string(COLOR_TICKER[nColor]) == ticker)
           {
                 nColorIn = nColor;
                 return true;
           }
    }
    return false;
}

bool GetTickerFromColor(int nColor, std::string &ticker)
{
     if (nColor < 1 || nColor > N_COLORS)
     {
              ticker = COLOR_TICKER[PSN_COLOR_P00];
              return false;
     }
     ticker = COLOR_TICKER[nColor];
     return true;
}

bool CheckColor(int nColor)
{
    return (nColor >= 1 && nColor < N_COLORS);
}

int64_t GetCoin(int nColor)
{
   return (nColor - 1 ? COIN[nColor] : 1);
}

int64_t GetCent(int nColor)
{
   return (nColor - 1 ? CENT[nColor] : 1);
}


bool CanStake(int nStakeColor)
{
    if (!CheckColor(nStakeColor))
    {
        return false;
    }
    if (MINT_COLOR[nStakeColor] == PSN_COLOR_P00)
    {
        return false;
    }
    return true;
}



int GetStakeMinConfirmations(int nColor)
{
    if (!CanStake(nColor))
    {
       return std::numeric_limits<int>::max();
    }
    if (fTestNet)
    {
       return nStakeMinConfirmationsTestnet;
    }
    return nStakeMinConfirmations;
}

int IsTransferrable(int nColor)
{
    if (!CheckColor(nColor))
    {
        return false;
    }
    return TRANSFERABLE[nColor];
}

bool SplitQualifiedAddress(const std::string &qualAddress,
                              std::string &address, int &nColor, bool fDebug)
{

    // find the delimeter
    size_t x = qualAddress.find(ADDRESS_DELIMETER);
    if (x == std::string::npos)
    {
          if (fDebug)
          {
              printf("Unable to find ticker suffix for %s\n", qualAddress.c_str());
          }
          return false;
    }

    // make the ticker, check, set nColor
    std::string ticker = qualAddress.substr(x + ADDRESS_DELIMETER.size(),
                                                          qualAddress.size());
    if (!GetColorFromTicker(ticker, nColor))
    {
          if (fDebug)
          {
              printf("Ticker is not valid for %s\n", qualAddress.c_str());
          }
          return false;
    }

    // make the address
    address = qualAddress.substr(0, x);

    return true;
}

// add b58 compatible bytes of n to end of vch, little byte first
bool AppendColorBytes(int n, std::vector<unsigned char> &vch)
{
        if(!CheckColor(n))
        {
               return false;
        }
        while (n >= 256)
        {
            vch.push_back(n & 255);   //  fast % 256
            n = n >> 8;               //  fast / 256
        }
        vch.push_back(n);
        return true;
}



//////////////////////////////////////////////////////////////////////
///
/// Data Structures
///
//////////////////////////////////////////////////////////////////////

bool ValueMapAllPositive(const std::map<int, int64_t> &mapNet)
{
    if (mapNet.empty())
    {
       return false;
    }
    std::map<int, int64_t>::const_iterator itnet;
    for (itnet = mapNet.begin(); itnet != mapNet.end(); ++itnet)
    {
        if (itnet->second <= 0)
        {
            return false;
        }
    }
    return true;
}

bool ValueMapAllZero(const std::map<int, int64_t> &mapNet)
{
    std::map<int, int64_t>::const_iterator itnet;
    for (itnet = mapNet.begin(); itnet != mapNet.end(); ++itnet)
    {
        if (itnet->second != 0)
        {
            return false;
        }
    }
    return true;
}


// effectively mapCredit - mapDebit
void FillNets(const std::map<int, int64_t> &mapDebit,
              const std::map<int, int64_t> &mapCredit,
              std::map<int, int64_t> &mapNet)
{
    mapNet = mapCredit;
    std::map<int, int64_t>::iterator itnet;
    std::map<int, int64_t>::const_iterator itdeb;
    for (itnet = mapNet.begin(); itnet != mapNet.end(); ++itnet)
    {
        for (itdeb = mapDebit.begin(); itdeb != mapDebit.end(); ++itdeb)
        {
            if (itnet->first == itdeb->first)
            {
                itnet->second -= itdeb->second;
                break;
            }
        }
    }
    for (itdeb = mapDebit.begin(); itdeb != mapDebit.end(); ++itdeb)
    {
        itnet = mapNet.find(itdeb->first);
        if (itnet == mapNet.end())
        {
            mapNet[itdeb->first] = -itdeb->second;
        }
    }
}





//////////////////////////////////////////////////////////////////////
///
/// Minting
///
//////////////////////////////////////////////////////////////////////
int64_t GetTargetSpacing(int nHeight, bool fProofOfStake)
{
    int64_t nTargetSpacing = nTargetSpacingPoS;
    if (fProofOfStake)
    {
        if (fTestNet)
        {
            nTargetSpacing = nTargetSpacingPoSTestNet;
        }
    }
    else
    {
        nTargetSpacing = fTestNet ? nTargetSpacingPoWTestNet : nTargetSpacingPoW;
    }
    return nTargetSpacing;
}


int GetLastFairLaunchBlock()
{
    return fTestNet ? LAST_FAIR_LAUNCH_BLOCK_TESTNET : LAST_FAIR_LAUNCH_BLOCK;
}

int64_t GetMaxPoWMint(int nColor)
{
    if (nColor == PSN_COLOR_P00)
    {
       return 0;
    }
    else if (nColor == PSN_COLOR_DPST)
    {
       return 0;
    }
    else if (nColor == PSN_COLOR_PREM)
    {
       return 15;
    }
    return 63000000 * GetCoin(nColor);
}

int64_t GetMaxPoWSubsidy(int nColor, const int64_t nReward)
{
    if (nColor == PSN_COLOR_P00)
    {
        return 0;
    }
    else if (nColor == PSN_COLOR_DPST)
    {
        return 0;
    }
    else if (nColor == PSN_COLOR_PREM)
    {
        if (nReward <= GetCoin(nColor))
        {
            return 0;
        }
        return 1;
    }
    else if (nColor == PSN_COLOR_UNIH)
    {
        if (nReward <= GetCoin(nColor))
        {
            return 0;
        }
        return nReward << 7;
    }
    return nReward;
}

int GetLastPoWBlock()
{
    return fTestNet ? LAST_POW_BLOCK_TESTNET : LAST_POW_BLOCK;
}

int GetFirstPoSBlock()
{
    return fTestNet ? FIRST_POS_BLOCK_TESTNET : FIRST_POS_BLOCK;
}

