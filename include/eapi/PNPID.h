/*
 *<KHeader>
 *+=========================================================================
 *I  Project Name: EApiDK Embedded Application Development Kit
 *+=========================================================================
 *I  $HeadURL: https://sourceforge.net/p/eapidk/code/HEAD/tree/trunk/include/PNPID.h $
 *+=========================================================================
 *I   Copyright: Copyright (c) 2002-2009, Kontron Embedded Modules GmbH
 *I      Author: John Kearney,                  John.Kearney@kontron.com
 *I
 *I     License: All rights reserved. This program and the accompanying 
 *I              materials are licensed and made available under the 
 *I              terms and conditions of the BSD License which 
 *I              accompanies this distribution. The full text of the 
 *I              license may be found at 
 *I              http://opensource.org/licenses/bsd-license.php
 *I              
 *I              THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "
 *I              AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS OF 
 *I              ANY KIND, EITHER EXPRESS OR IMPLIED.
 *I
 *I Description: Auto Created for PNPID.h
 *I
 *+-------------------------------------------------------------------------
 *I
 *I  File Name            : PNPID.h
 *I  File Location        : include
 *I
 *+=========================================================================
 *</KHeader>
 */

typedef struct PNPIDLookupTable_s{
        uint16_t PNPID;
        TCHAR   *pVName;
}PNPIDLookupTable_t;

#define PNPID_END_OF_LIST_MARKER 0xFFFF
PNPIDLookupTable_t PNPIDLookupTable[]={
  {
    0x2504,   /* AAE */
    TEXT("Anatek Electronics Inc.")
  },
  {
    0x3404,   /* AAT */
    TEXT("Ann Arbor Technologies")
  },
  {
    0x4104,   /* ABA */
    TEXT("ABBAHOME   INC.")
  },
  {
    0x4304,   /* ABC */
    TEXT("AboCom System Inc")
  },
  {
    0x4404,   /* ABD */
    TEXT("Allen Bradley Company")
  },
  {
    0x4504,   /* ABE */
    TEXT("Alcatel Bell")
  },
  {
    0x4F04,   /* ABO */
    TEXT("D-Link Systems Inc")
  },
  {
    0x5404,   /* ABT */
    TEXT("Anchor Bay Technologies, Inc.")
  },
  {
    0x5604,   /* ABV */
    TEXT("Advanced Research Technology")
  },
  {
    0x6104,   /* ACA */
    TEXT("Ariel Corporation")
  },
  {
    0x6204,   /* ACB */
    TEXT("Aculab Ltd")
  },
  {
    0x6304,   /* ACC */
    TEXT("Accton Technology Corporation")
  },
  {
    0x6404,   /* ACD */
    TEXT("AWETA BV")
  },
  {
    0x6504,   /* ACE */
    TEXT("Actek Engineering Pty Ltd")
  },
  {
    0x6704,   /* ACG */
    TEXT("A&R Cambridge Ltd")
  },
  {
    0x6804,   /* ACH */
    TEXT("Archtek Telecom Corporation")
  },
  {
    0x6904,   /* ACI */
    TEXT("Ancor Communications Inc")
  },
  {
    0x6B04,   /* ACK */
    TEXT("Acksys")
  },
  {
    0x6C04,   /* ACL */
    TEXT("Apricot Computers")
  },
  {
    0x6D04,   /* ACM */
    TEXT("Acroloop Motion Control Systems Inc")
  },
  {
    0x6F04,   /* ACO */
    TEXT("Allion Computer Inc.")
  },
  {
    0x7004,   /* ACP */
    TEXT("Aspen Tech Inc")
  },
  {
    0x7204,   /* ACR */
    TEXT("Acer Technologies")
  },
  {
    0x7304,   /* ACS */
    TEXT("Altos Computer Systems")
  },
  {
    0x7404,   /* ACT */
    TEXT("Applied Creative Technology")
  },
  {
    0x7504,   /* ACU */
    TEXT("Acculogic")
  },
  {
    0x7604,   /* ACV */
    TEXT("ActivCard S.A")
  },
  {
    0x8104,   /* ADA */
    TEXT("Addi-Data GmbH")
  },
  {
    0x8204,   /* ADB */
    TEXT("Aldebbaron")
  },
  {
    0x8304,   /* ADC */
    TEXT("Acnhor Datacomm")
  },
  {
    0x8404,   /* ADD */
    TEXT("Advanced Peripheral Devices Inc")
  },
  {
    0x8504,   /* ADE */
    TEXT("Arithmos, Inc.")
  },
  {
    0x8804,   /* ADH */
    TEXT("Aerodata Holdings Ltd")
  },
  {
    0x8904,   /* ADI */
    TEXT("ADI Systems Inc")
  },
  {
    0x8B04,   /* ADK */
    TEXT("Adtek System Science Company Ltd")
  },
  {
    0x8C04,   /* ADL */
    TEXT("ASTRA Security Products Ltd")
  },
  {
    0x8D04,   /* ADM */
    TEXT("Ad Lib MultiMedia Inc")
  },
  {
    0x8E04,   /* ADN */
    TEXT("Analog & Digital Devices Tel. Inc")
  },
  {
    0x9004,   /* ADP */
    TEXT("Adaptec Inc")
  },
  {
    0x9204,   /* ADR */
    TEXT("Nasa Ames Research Center")
  },
  {
    0x9304,   /* ADS */
    TEXT("Analog Devices Inc")
  },
  {
    0x9404,   /* ADT */
    TEXT("Adtek")
  },
  {
    0x9404,   /* ADT */
    TEXT("Adtek")
  },
  {
    0x9604,   /* ADV */
    TEXT("Advanced Micro Devices Inc")
  },
  {
    0x9804,   /* ADX */
    TEXT("Adax Inc")
  },
  {
    0xA304,   /* AEC */
    TEXT("Antex Electronics Corporation")
  },
  {
    0xA404,   /* AED */
    TEXT("Advanced Electronic Designs, Inc.")
  },
  {
    0xA904,   /* AEI */
    TEXT("Actiontec Electric Inc")
  },
  {
    0xAA04,   /* AEJ */
    TEXT("Alpha Electronics Company")
  },
  {
    0xAD04,   /* AEM */
    TEXT("ASEM S.p.A.")
  },
  {
    0xB004,   /* AEP */
    TEXT("Aetas Peripheral International")
  },
  {
    0xB404,   /* AET */
    TEXT("Aethra Telecomunicazioni S.r.l.")
  },
  {
    0xC104,   /* AFA */
    TEXT("Alfa Inc")
  },
  {
    0xE304,   /* AGC */
    TEXT("Beijing Aerospace Golden Card Electronic Engineering Co.,Ltd.")
  },
  {
    0xE904,   /* AGI */
    TEXT("Artish Graphics Inc")
  },
  {
    0xEC04,   /* AGL */
    TEXT("Argolis")
  },
  {
    0xED04,   /* AGM */
    TEXT("Advan Int'l Corporation")
  },
  {
    0xF404,   /* AGT */
    TEXT("Agilent Technologies")
  },
  {
    0x0305,   /* AHC */
    TEXT("Advantech Co., Ltd.")
  },
  {
    0x2305,   /* AIC */
    TEXT("Arnos Insturments & Computer Systems")
  },
  {
    0x2505,   /* AIE */
    TEXT("Altmann Industrieelektronik")
  },
  {
    0x2905,   /* AII */
    TEXT("Amptron International Inc.")
  },
  {
    0x2C05,   /* AIL */
    TEXT("Altos India Ltd")
  },
  {
    0x2D05,   /* AIM */
    TEXT("AIMS Lab Inc")
  },
  {
    0x3205,   /* AIR */
    TEXT("Advanced Integ. Research Inc")
  },
  {
    0x3305,   /* AIS */
    TEXT("Alien Internet Services")
  },
  {
    0x3705,   /* AIW */
    TEXT("Aiwa Company Ltd")
  },
  {
    0x3805,   /* AIX */
    TEXT("ALTINEX, INC.")
  },
  {
    0x4105,   /* AJA */
    TEXT("AJA Video Systems, Inc.")
  },
  {
    0x6205,   /* AKB */
    TEXT("Akebia Ltd")
  },
  {
    0x6905,   /* AKI */
    TEXT("AKIA Corporation")
  },
  {
    0x6C05,   /* AKL */
    TEXT("AMiT Ltd")
  },
  {
    0x6D05,   /* AKM */
    TEXT("Asahi Kasei Microsystems Company Ltd")
  },
  {
    0x7005,   /* AKP */
    TEXT("Atom Komplex Prylad")
  },
  {
    0x7905,   /* AKY */
    TEXT("Askey Computer Corporation")
  },
  {
    0x8105,   /* ALA */
    TEXT("Alacron Inc")
  },
  {
    0x8305,   /* ALC */
    TEXT("Altec Corporation")
  },
  {
    0x8405,   /* ALD */
    TEXT("In4S Inc")
  },
  {
    0x8705,   /* ALG */
    TEXT("Realtek Semiconductor Corp.")
  },
  {
    0x8805,   /* ALH */
    TEXT("AL Systems")
  },
  {
    0x8905,   /* ALI */
    TEXT("Acer Labs")
  },
  {
    0x8A05,   /* ALJ */
    TEXT("Altec Lansing")
  },
  {
    0x8B05,   /* ALK */
    TEXT("Acrolink Inc")
  },
  {
    0x8C05,   /* ALL */
    TEXT("Alliance Semiconductor Corporation")
  },
  {
    0x8D05,   /* ALM */
    TEXT("Acutec Ltd.")
  },
  {
    0x8E05,   /* ALN */
    TEXT("Alana Technologies")
  },
  {
    0x8F05,   /* ALO */
    TEXT("Algolith Inc.")
  },
  {
    0x9005,   /* ALP */
    TEXT("Alps Electric Company Ltd")
  },
  {
    0x9205,   /* ALR */
    TEXT("Advanced Logic")
  },
  {
    0x9305,   /* ALS */
    TEXT("Avance Logic Inc")
  },
  {
    0x9405,   /* ALT */
    TEXT("Altra")
  },
  {
    0x9605,   /* ALV */
    TEXT("AlphaView LCD")
  },
  {
    0x9805,   /* ALX */
    TEXT("ALEXON Co.,Ltd.")
  },
  {
    0xA105,   /* AMA */
    TEXT("Asia Microelectronic Development Inc")
  },
  {
    0xA205,   /* AMB */
    TEXT("Ambient Technologies, Inc.")
  },
  {
    0xA305,   /* AMC */
    TEXT("Attachmate Corporation")
  },
  {
    0xA405,   /* AMD */
    TEXT("Amdek Corporation")
  },
  {
    0xA905,   /* AMI */
    TEXT("American Megatrends Inc")
  },
  {
    0xAC05,   /* AML */
    TEXT("Anderson Multimedia Communications (HK) Limited")
  },
  {
    0xAE05,   /* AMN */
    TEXT("Amimon LTD.")
  },
  {
    0xB005,   /* AMP */
    TEXT("AMP Inc")
  },
  {
    0xB405,   /* AMT */
    TEXT("AMT International Industry")
  },
  {
    0xB805,   /* AMX */
    TEXT("AMX LLC")
  },
  {
    0xC105,   /* ANA */
    TEXT("Anakron")
  },
  {
    0xC305,   /* ANC */
    TEXT("Ancot")
  },
  {
    0xC405,   /* AND */
    TEXT("Adtran Inc")
  },
  {
    0xC905,   /* ANI */
    TEXT("Anigma Inc")
  },
  {
    0xCB05,   /* ANK */
    TEXT("Anko Electronic Company Ltd")
  },
  {
    0xCC05,   /* ANL */
    TEXT("Analogix Semiconductor, Inc")
  },
  {
    0xCF05,   /* ANO */
    TEXT("Anorad Corporation")
  },
  {
    0xD005,   /* ANP */
    TEXT("Andrew Network Production")
  },
  {
    0xD205,   /* ANR */
    TEXT("ANR Ltd")
  },
  {
    0xD305,   /* ANS */
    TEXT("Ansel Communication Company")
  },
  {
    0xD405,   /* ANT */
    TEXT("Ace CAD Enterprise Company Ltd")
  },
  {
    0xD805,   /* ANX */
    TEXT("Acer Netxus Inc")
  },
  {
    0xE105,   /* AOA */
    TEXT("AOpen Inc.")
  },
  {
    0xE505,   /* AOE */
    TEXT("Advanced Optics Electronics, Inc.")
  },
  {
    0xEC05,   /* AOL */
    TEXT("America OnLine")
  },
  {
    0xF405,   /* AOT */
    TEXT("Alcatel")
  },
  {
    0x0306,   /* APC */
    TEXT("American Power Conversion")
  },
  {
    0x0406,   /* APD */
    TEXT("AppliAdata")
  },
  {
    0x0706,   /* APG */
    TEXT("Horner Electric Inc")
  },
  {
    0x0906,   /* API */
    TEXT("A Plus Info Corporation")
  },
  {
    0x0C06,   /* APL */
    TEXT("Aplicom Oy")
  },
  {
    0x0D06,   /* APM */
    TEXT("Applied Memory Tech")
  },
  {
    0x0E06,   /* APN */
    TEXT("Appian Tech Inc")
  },
  {
    0x1006,   /* APP */
    TEXT("Apple Computer Inc")
  },
  {
    0x1206,   /* APR */
    TEXT("Aprilia s.p.a.")
  },
  {
    0x1306,   /* APS */
    TEXT("Autologic Inc")
  },
  {
    0x1406,   /* APT */
    TEXT("Audio Processing Technology  Ltd")
  },
  {
    0x1806,   /* APX */
    TEXT("AP Designs Ltd")
  },
  {
    0x4306,   /* ARC */
    TEXT("Alta Research Corporation")
  },
  {
    0x4506,   /* ARE */
    TEXT("ICET S.p.A.")
  },
  {
    0x4706,   /* ARG */
    TEXT("Argus Electronics Co., LTD")
  },
  {
    0x4906,   /* ARI */
    TEXT("Argosy Research Inc")
  },
  {
    0x4B06,   /* ARK */
    TEXT("Ark Logic Inc")
  },
  {
    0x4C06,   /* ARL */
    TEXT("Arlotto Comnet Inc")
  },
  {
    0x4D06,   /* ARM */
    TEXT("Arima")
  },
  {
    0x4F06,   /* ARO */
    TEXT("Poso International B.V.")
  },
  {
    0x5306,   /* ARS */
    TEXT("Arescom Inc")
  },
  {
    0x5406,   /* ART */
    TEXT("Corion Industrial Corporation")
  },
  {
    0x6306,   /* ASC */
    TEXT("Ascom Strategic Technology Unit")
  },
  {
    0x6406,   /* ASD */
    TEXT("USC Information Sciences Institute")
  },
  {
    0x6506,   /* ASE */
    TEXT("AseV Display Labs")
  },
  {
    0x6906,   /* ASI */
    TEXT("Ahead Systems")
  },
  {
    0x6B06,   /* ASK */
    TEXT("Ask A/S")
  },
  {
    0x6C06,   /* ASL */
    TEXT("AccuScene Corporation Ltd")
  },
  {
    0x6D06,   /* ASM */
    TEXT("ASEM S.p.A.")
  },
  {
    0x6E06,   /* ASN */
    TEXT("Asante Tech Inc")
  },
  {
    0x7006,   /* ASP */
    TEXT("ASP Microelectronics Ltd")
  },
  {
    0x7406,   /* AST */
    TEXT("AST Research Inc")
  },
  {
    0x7506,   /* ASU */
    TEXT("Asuscom Network Inc")
  },
  {
    0x7806,   /* ASX */
    TEXT("AudioScience")
  },
  {
    0x7906,   /* ASY */
    TEXT("Rockwell Collins / Airshow Systems")
  },
  {
    0x8106,   /* ATA */
    TEXT("Allied Telesyn International (Asia) Pte Ltd")
  },
  {
    0x8306,   /* ATC */
    TEXT("Ably-Tech Corporation")
  },
  {
    0x8406,   /* ATD */
    TEXT("Alpha Telecom Inc")
  },
  {
    0x8506,   /* ATE */
    TEXT("Innovate Ltd")
  },
  {
    0x8806,   /* ATH */
    TEXT("Athena Informatica S.R.L.")
  },
  {
    0x8906,   /* ATI */
    TEXT("Allied Telesis KK")
  },
  {
    0x8B06,   /* ATK */
    TEXT("Allied Telesyn Int'l")
  },
  {
    0x8C06,   /* ATL */
    TEXT("Arcus Technology Ltd")
  },
  {
    0x8D06,   /* ATM */
    TEXT("ATM Ltd")
  },
  {
    0x8E06,   /* ATN */
    TEXT("Athena Smartcard Solutions Ltd.")
  },
  {
    0x8F06,   /* ATO */
    TEXT("ASTRO DESIGN, INC.")
  },
  {
    0x9006,   /* ATP */
    TEXT("Alpha-Top Corporation")
  },
  {
    0x9406,   /* ATT */
    TEXT("AT&T")
  },
  {
    0x9606,   /* ATV */
    TEXT("Office Depot, Inc.")
  },
  {
    0x9806,   /* ATX */
    TEXT("Athenix Corporation")
  },
  {
    0xA906,   /* AUI */
    TEXT("Alps Electric Inc")
  },
  {
    0xAF06,   /* AUO */
    TEXT("DO NOT USE - AUO")
  },
  {
    0xB206,   /* AUR */
    TEXT("Aureal Semiconductor")
  },
  {
    0xB406,   /* AUT */
    TEXT("Autotime Corporation")
  },
  {
    0xC106,   /* AVA */
    TEXT("Avaya Communication")
  },
  {
    0xC306,   /* AVC */
    TEXT("Auravision Corporation")
  },
  {
    0xC406,   /* AVD */
    TEXT("Avid Electronics Corporation")
  },
  {
    0xC506,   /* AVE */
    TEXT("Add Value Enterpises (Asia) Pte Ltd")
  },
  {
    0xC906,   /* AVI */
    TEXT("Nippon Avionics Co.,Ltd")
  },
  {
    0xCD06,   /* AVM */
    TEXT("AVM GmbH")
  },
  {
    0xCF06,   /* AVO */
    TEXT("Avocent Corporation")
  },
  {
    0xD406,   /* AVT */
    TEXT("Avtek (Electronics) Pty Ltd")
  },
  {
    0xD606,   /* AVV */
    TEXT("SBS Technologies (Canada), Inc. (was Avvida Systems, Inc.)")
  },
  {
    0xE306,   /* AWC */
    TEXT("Access Works Comm Inc")
  },
  {
    0xEC06,   /* AWL */
    TEXT("Aironet Wireless Communications, Inc")
  },
  {
    0xF306,   /* AWS */
    TEXT("Wave Systems")
  },
  {
    0x0207,   /* AXB */
    TEXT("Adrienne Electronics Corporation")
  },
  {
    0x0307,   /* AXC */
    TEXT("AXIOMTEK CO., LTD.")
  },
  {
    0x0507,   /* AXE */
    TEXT("D-Link Systems Inc (used as 2nd pnpid)")
  },
  {
    0x0907,   /* AXI */
    TEXT("American Magnetics")
  },
  {
    0x0C07,   /* AXL */
    TEXT("Axel")
  },
  {
    0x1007,   /* AXP */
    TEXT("American Express")
  },
  {
    0x1407,   /* AXT */
    TEXT("Axtend Technologies Inc")
  },
  {
    0x1807,   /* AXX */
    TEXT("Axxon Computer Corporation")
  },
  {
    0x1907,   /* AXY */
    TEXT("AXYZ Automation Services, Inc")
  },
  {
    0x2407,   /* AYD */
    TEXT("Aydin Displays")
  },
  {
    0x3207,   /* AYR */
    TEXT("Airlib, Inc")
  },
  {
    0x4D07,   /* AZM */
    TEXT("AZ Middelheim - Radiotherapy")
  },
  {
    0x5407,   /* AZT */
    TEXT("Aztech Systems Ltd")
  },
  {
    0x2308,   /* BAC */
    TEXT("Biometric Access Corporation")
  },
  {
    0x2E08,   /* BAN */
    TEXT("Banyan")
  },
  {
    0x4208,   /* BBB */
    TEXT("an-najah university")
  },
  {
    0x4808,   /* BBH */
    TEXT("B&Bh")
  },
  {
    0x4C08,   /* BBL */
    TEXT("Brain Boxes Limited")
  },
  {
    0x6308,   /* BCC */
    TEXT("Beaver Computer Corporaton")
  },
  {
    0x6408,   /* BCD */
    TEXT("Dr. Seufert GmbH")
  },
  {
    0x6D08,   /* BCM */
    TEXT("Broadcom")
  },
  {
    0x7108,   /* BCQ */
    TEXT("Deutsche Telekom Berkom GmbH")
  },
  {
    0x7308,   /* BCS */
    TEXT("Booria CAD/CAM systems")
  },
  {
    0x8F08,   /* BDO */
    TEXT("Brahler ICS")
  },
  {
    0x9208,   /* BDR */
    TEXT("Blonder Tongue Labs, Inc.")
  },
  {
    0x9308,   /* BDS */
    TEXT("Barco Display Systems")
  },
  {
    0xA308,   /* BEC */
    TEXT("Elektro Beckhoff GmbH")
  },
  {
    0xA908,   /* BEI */
    TEXT("Beckworth Enterprises Inc")
  },
  {
    0xAB08,   /* BEK */
    TEXT("Beko Elektronik A.S.")
  },
  {
    0xAC08,   /* BEL */
    TEXT("Beltronic Industrieelektronik GmbH")
  },
  {
    0xAF08,   /* BEO */
    TEXT("Baug & Olufsen")
  },
  {
    0xC508,   /* BFE */
    TEXT("B.F. Engineering Corporation")
  },
  {
    0xE208,   /* BGB */
    TEXT("Barco Graphics N.V")
  },
  {
    0xF408,   /* BGT */
    TEXT("Budzetron Inc")
  },
  {
    0x1A09,   /* BHZ */
    TEXT("BitHeadz, Inc.")
  },
  {
    0x2309,   /* BIC */
    TEXT("Big Island Communications")
  },
  {
    0x2909,   /* BII */
    TEXT("Boeckeler Instruments Inc")
  },
  {
    0x2C09,   /* BIL */
    TEXT("Billion Electric Company Ltd")
  },
  {
    0x2F09,   /* BIO */
    TEXT("BioLink Technologies International, Inc.")
  },
  {
    0x3409,   /* BIT */
    TEXT("Bit 3 Computer")
  },
  {
    0x8909,   /* BLI */
    TEXT("Busicom")
  },
  {
    0x8E09,   /* BLN */
    TEXT("BioLink Technologies")
  },
  {
    0x9009,   /* BLP */
    TEXT("Bloomberg L.P.")
  },
  {
    0xA909,   /* BMI */
    TEXT("Benson Medical Instruments Company")
  },
  {
    0xAC09,   /* BML */
    TEXT("BIOMED Lab")
  },
  {
    0xB309,   /* BMS */
    TEXT("BIOMEDISYS")
  },
  {
    0xC509,   /* BNE */
    TEXT("Bull AB")
  },
  {
    0xCB09,   /* BNK */
    TEXT("Banksia Tech Pty Ltd")
  },
  {
    0xCF09,   /* BNO */
    TEXT("Bang & Olufsen")
  },
  {
    0xD309,   /* BNS */
    TEXT("Boulder Nonlinear Systems")
  },
  {
    0xE209,   /* BOB */
    TEXT("Rainy Orchard")
  },
  {
    0xE509,   /* BOE */
    TEXT("BOE")
  },
  {
    0xF309,   /* BOS */
    TEXT("BOS")
  },
  {
    0x040A,   /* BPD */
    TEXT("Micro Solutions, Inc.")
  },
  {
    0x150A,   /* BPU */
    TEXT("Best Power")
  },
  {
    0x430A,   /* BRC */
    TEXT("BARC")
  },
  {
    0x470A,   /* BRG */
    TEXT("Bridge Information Co., Ltd")
  },
  {
    0x490A,   /* BRI */
    TEXT("Boca Research Inc")
  },
  {
    0x4D0A,   /* BRM */
    TEXT("Braemar Inc")
  },
  {
    0x4F0A,   /* BRO */
    TEXT("BROTHER INDUSTRIES,LTD.")
  },
  {
    0x650A,   /* BSE */
    TEXT("Bose Corporation")
  },
  {
    0x6C0A,   /* BSL */
    TEXT("Biomedical Systems Laboratory")
  },
  {
    0x740A,   /* BST */
    TEXT("BodySound Technologies, Inc.")
  },
  {
    0x830A,   /* BTC */
    TEXT("Bit 3 Computer")
  },
  {
    0x850A,   /* BTE */
    TEXT("Brilliant Technology")
  },
  {
    0x860A,   /* BTF */
    TEXT("Bitfield Oy")
  },
  {
    0x890A,   /* BTI */
    TEXT("BusTech Inc")
  },
  {
    0xA60A,   /* BUF */
    TEXT("Yasuhiko Shirai Melco Inc")
  },
  {
    0xAA0A,   /* BUJ */
    TEXT("ATI Tech Inc")
  },
  {
    0xAC0A,   /* BUL */
    TEXT("Bull")
  },
  {
    0xB20A,   /* BUR */
    TEXT("Bernecker & Rainer Ind-Eletronik GmbH")
  },
  {
    0xB30A,   /* BUS */
    TEXT("BusTek")
  },
  {
    0xB40A,   /* BUT */
    TEXT("21ST CENTURY ENTERTAINMENT")
  },
  {
    0xEB0A,   /* BWK */
    TEXT("Bitworks Inc.")
  },
  {
    0x050B,   /* BXE */
    TEXT("Buxco Electronics")
  },
  {
    0x240B,   /* BYD */
    TEXT("byd:sign corporation")
  },
  {
    0x210C,   /* CAA */
    TEXT("Castles Automation Co., Ltd")
  },
  {
    0x230C,   /* CAC */
    TEXT("CA & F Elettronica")
  },
  {
    0x270C,   /* CAG */
    TEXT("CalComp")
  },
  {
    0x290C,   /* CAI */
    TEXT("Canon Inc.")
  },
  {
    0x2C0C,   /* CAL */
    TEXT("Acon")
  },
  {
    0x2D0C,   /* CAM */
    TEXT("Cambridge Audio")
  },
  {
    0x2E0C,   /* CAN */
    TEXT("Canopus Company Ltd")
  },
  {
    0x2E0C,   /* CAN */
    TEXT("Canopus Company Ltd")
  },
  {
    0x2E0C,   /* CAN */
    TEXT("Canopus Company Ltd")
  },
  {
    0x320C,   /* CAR */
    TEXT("Cardinal Company Ltd")
  },
  {
    0x330C,   /* CAS */
    TEXT("CASIO COMPUTER CO.,LTD")
  },
  {
    0x340C,   /* CAT */
    TEXT("Consultancy in Advanced Technology")
  },
  {
    0x490C,   /* CBI */
    TEXT("ComputerBoards Inc")
  },
  {
    0x520C,   /* CBR */
    TEXT("Cebra Tech A/S")
  },
  {
    0x580C,   /* CBX */
    TEXT("Cybex Computer Products Corporation")
  },
  {
    0x630C,   /* CCC */
    TEXT("C-Cube Microsystems")
  },
  {
    0x690C,   /* CCI */
    TEXT("Cache")
  },
  {
    0x6A0C,   /* CCJ */
    TEXT("CONTEC CO.,LTD.")
  },
  {
    0x6C0C,   /* CCL */
    TEXT("CCL/ITRI")
  },
  {
    0x700C,   /* CCP */
    TEXT("Capetronic USA Inc")
  },
  {
    0x830C,   /* CDC */
    TEXT("Core Dynamics Corporation")
  },
  {
    0x840C,   /* CDD */
    TEXT("Convergent Data Devices")
  },
  {
    0x850C,   /* CDE */
    TEXT("Colin.de")
  },
  {
    0x870C,   /* CDG */
    TEXT("Christie Digital Systems Inc")
  },
  {
    0x890C,   /* CDI */
    TEXT("Concept Development Inc")
  },
  {
    0x8B0C,   /* CDK */
    TEXT("Cray Communications")
  },
  {
    0x8E0C,   /* CDN */
    TEXT("Codenoll Technical Corporation")
  },
  {
    0x900C,   /* CDP */
    TEXT("CalComp")
  },
  {
    0x930C,   /* CDS */
    TEXT("Computer Diagnostic Systems")
  },
  {
    0x940C,   /* CDT */
    TEXT("IBM Corporation")
  },
  {
    0x960C,   /* CDV */
    TEXT("Convergent Design Inc.")
  },
  {
    0xA10C,   /* CEA */
    TEXT("Consumer Electronics Association")
  },
  {
    0xA30C,   /* CEC */
    TEXT("Chicony Electronics Company Ltd")
  },
  {
    0xA40C,   /* CED */
    TEXT("Cambridge Electronic Design Ltd")
  },
  {
    0xA60C,   /* CEF */
    TEXT("Cefar Digital Vision")
  },
  {
    0xA90C,   /* CEI */
    TEXT("Crestron Electronics, Inc.")
  },
  {
    0xAD0C,   /* CEM */
    TEXT("MEC Electronics GmbH")
  },
  {
    0xAE0C,   /* CEN */
    TEXT("Centurion Technologies P/L")
  },
  {
    0xB00C,   /* CEP */
    TEXT("C-DAC")
  },
  {
    0xB20C,   /* CER */
    TEXT("Ceronix")
  },
  {
    0xB40C,   /* CET */
    TEXT("TEC CORPORATION")
  },
  {
    0xC70C,   /* CFG */
    TEXT("Atlantis")
  },
  {
    0xE10C,   /* CGA */
    TEXT("Chunghwa Picture Tubes, LTD")
  },
  {
    0xF30C,   /* CGS */
    TEXT("Chyron Corp")
  },
  {
    0x010D,   /* CHA */
    TEXT("Chase Research PLC")
  },
  {
    0x030D,   /* CHC */
    TEXT("Chic Technology Corp.")
  },
  {
    0x040D,   /* CHD */
    TEXT("ChangHong Electric Co.,Ltd")
  },
  {
    0x050D,   /* CHE */
    TEXT("Acer Inc")
  },
  {
    0x070D,   /* CHG */
    TEXT("Sichuan Changhong Electric CO, LTD.")
  },
  {
    0x090D,   /* CHI */
    TEXT("Chrontel Inc")
  },
  {
    0x0C0D,   /* CHL */
    TEXT("Chloride-R&D")
  },
  {
    0x0D0D,   /* CHM */
    TEXT("CHIC TECHNOLOGY CORP.")
  },
  {
    0x0F0D,   /* CHO */
    TEXT("Sichuang Changhong Corporation")
  },
  {
    0x100D,   /* CHP */
    TEXT("CH Products")
  },
  {
    0x130D,   /* CHS */
    TEXT("Agentur Chairos")
  },
  {
    0x140D,   /* CHT */
    TEXT("Chunghwa Picture Tubes,LTD.")
  },
  {
    0x190D,   /* CHY */
    TEXT("Cherry GmbH")
  },
  {
    0x230D,   /* CIC */
    TEXT("Comm. Intelligence Corporation")
  },
  {
    0x290D,   /* CII */
    TEXT("Cromack Industries Inc")
  },
  {
    0x2C0D,   /* CIL */
    TEXT("Citicom Infotech Private Limited")
  },
  {
    0x2E0D,   /* CIN */
    TEXT("Citron GmbH")
  },
  {
    0x300D,   /* CIP */
    TEXT("Ciprico Inc")
  },
  {
    0x320D,   /* CIR */
    TEXT("Cirrus Logic Inc")
  },
  {
    0x330D,   /* CIS */
    TEXT("Cisco Systems Inc")
  },
  {
    0x340D,   /* CIT */
    TEXT("Citifax Limited")
  },
  {
    0x630D,   /* CKC */
    TEXT("The Concept Keyboard Company Ltd")
  },
  {
    0x810D,   /* CLA */
    TEXT("Clarion Company Ltd")
  },
  {
    0x840D,   /* CLD */
    TEXT("COMMAT L.t.d.")
  },
  {
    0x850D,   /* CLE */
    TEXT("Classe Audio")
  },
  {
    0x870D,   /* CLG */
    TEXT("CoreLogic")
  },
  {
    0x890D,   /* CLI */
    TEXT("Cirrus Logic Inc")
  },
  {
    0x8D0D,   /* CLM */
    TEXT("CrystaLake Multimedia")
  },
  {
    0x8F0D,   /* CLO */
    TEXT("Clone Computers")
  },
  {
    0x940D,   /* CLT */
    TEXT("automated computer control systems")
  },
  {
    0x960D,   /* CLV */
    TEXT("Clevo Company")
  },
  {
    0x980D,   /* CLX */
    TEXT("CardLogix")
  },
  {
    0xA30D,   /* CMC */
    TEXT("CMC Ltd")
  },
  {
    0xA40D,   /* CMD */
    TEXT("Colorado MicroDisplay, Inc.")
  },
  {
    0xA70D,   /* CMG */
    TEXT("Chenming Mold Ind. Corp.")
  },
  {
    0xA90D,   /* CMI */
    TEXT("C-Media Electronics")
  },
  {
    0xAD0D,   /* CMM */
    TEXT("Comtime GmbH")
  },
  {
    0xAF0D,   /* CMO */
    TEXT("Chi Mei Optoelectronics corp.")
  },
  {
    0xB20D,   /* CMR */
    TEXT("Cambridge Research Systems Ltd")
  },
  {
    0xB30D,   /* CMS */
    TEXT("CompuMaster Srl")
  },
  {
    0xB80D,   /* CMX */
    TEXT("Comex Electronics AB")
  },
  {
    0xC20D,   /* CNB */
    TEXT("American Power Conversion")
  },
  {
    0xC30D,   /* CNC */
    TEXT("Alvedon Computers Ltd")
  },
  {
    0xC50D,   /* CNE */
    TEXT("Cine-tal")
  },
  {
    0xC90D,   /* CNI */
    TEXT("Connect Int'l A/S")
  },
  {
    0xCE0D,   /* CNN */
    TEXT("Canon Inc")
  },
  {
    0xD40D,   /* CNT */
    TEXT("COINT Multimedia Systems")
  },
  {
    0xE20D,   /* COB */
    TEXT("COBY Electronics Co., Ltd")
  },
  {
    0xE40D,   /* COD */
    TEXT("CODAN Pty. Ltd.")
  },
  {
    0xE90D,   /* COI */
    TEXT("Codec Inc.")
  },
  {
    0xEC0D,   /* COL */
    TEXT("Rockwell Collins, Inc.")
  },
  {
    0xED0D,   /* COM */
    TEXT("Comtrol Corporation")
  },
  {
    0xEE0D,   /* CON */
    TEXT("Contec Company Ltd")
  },
  {
    0xF20D,   /* COR */
    TEXT("Corollary Inc")
  },
  {
    0xF30D,   /* COS */
    TEXT("CoStar Corporation")
  },
  {
    0xF40D,   /* COT */
    TEXT("Core Technology Inc")
  },
  {
    0xF70D,   /* COW */
    TEXT("Polycow Productions")
  },
  {
    0x030E,   /* CPC */
    TEXT("Ciprico Inc")
  },
  {
    0x040E,   /* CPD */
    TEXT("CompuAdd")
  },
  {
    0x090E,   /* CPI */
    TEXT("Computer Peripherals Inc")
  },
  {
    0x0C0E,   /* CPL */
    TEXT("Compal Electronics Inc")
  },
  {
    0x110E,   /* CPQ */
    TEXT("Compaq Computer Company")
  },
  {
    0x140E,   /* CPT */
    TEXT("cPATH")
  },
  {
    0x180E,   /* CPX */
    TEXT("Powermatic Data Systems")
  },
  {
    0x430E,   /* CRC */
    TEXT("CONRAC GmbH")
  },
  {
    0x440E,   /* CRD */
    TEXT("Cardinal Technical Inc")
  },
  {
    0x450E,   /* CRE */
    TEXT("Creative Labs Inc")
  },
  {
    0x490E,   /* CRI */
    TEXT("Crio Inc.")
  },
  {
    0x4C0E,   /* CRL */
    TEXT("Creative Logic")
  },
  {
    0x4E0E,   /* CRN */
    TEXT("Cornerstone Imaging")
  },
  {
    0x4F0E,   /* CRO */
    TEXT("Extraordinary Technologies PTY Limited")
  },
  {
    0x510E,   /* CRQ */
    TEXT("Cirque Corporation")
  },
  {
    0x530E,   /* CRS */
    TEXT("Crescendo Communication Inc")
  },
  {
    0x580E,   /* CRX */
    TEXT("Cyrix Corporation")
  },
  {
    0x620E,   /* CSB */
    TEXT("Transtex SA")
  },
  {
    0x630E,   /* CSC */
    TEXT("Crystal Semiconductor")
  },
  {
    0x640E,   /* CSD */
    TEXT("Cresta Systems Inc")
  },
  {
    0x650E,   /* CSE */
    TEXT("Concept Solutions & Engineering")
  },
  {
    0x690E,   /* CSI */
    TEXT("Cabletron System Inc")
  },
  {
    0x6F0E,   /* CSO */
    TEXT("California Institute of Technology")
  },
  {
    0x730E,   /* CSS */
    TEXT("CSS Laboratories")
  },
  {
    0x740E,   /* CST */
    TEXT("CSTI Inc")
  },
  {
    0x810E,   /* CTA */
    TEXT("CoSystems Inc")
  },
  {
    0x830E,   /* CTC */
    TEXT("CTC Communication Development Company Ltd")
  },
  {
    0x850E,   /* CTE */
    TEXT("Chunghwa Telecom Co., Ltd.")
  },
  {
    0x8C0E,   /* CTL */
    TEXT("Creative Technology Ltd")
  },
  {
    0x8D0E,   /* CTM */
    TEXT("Computerm Corporation")
  },
  {
    0x8E0E,   /* CTN */
    TEXT("Computone Products")
  },
  {
    0x900E,   /* CTP */
    TEXT("Computer Technology Corporation")
  },
  {
    0x930E,   /* CTS */
    TEXT("Comtec Systems Co., Ltd.")
  },
  {
    0x980E,   /* CTX */
    TEXT("Creatix Polymedia GmbH")
  },
  {
    0xA20E,   /* CUB */
    TEXT("Cubix Corporation")
  },
  {
    0xAB0E,   /* CUK */
    TEXT("Calibre UK Ltd")
  },
  {
    0xD30E,   /* CVS */
    TEXT("Clarity Visual Systems")
  },
  {
    0xF20E,   /* CWR */
    TEXT("Connectware Inc")
  },
  {
    0x140F,   /* CXT */
    TEXT("Conexant Systems")
  },
  {
    0x220F,   /* CYB */
    TEXT("CyberVision")
  },
  {
    0x230F,   /* CYC */
    TEXT("Cylink Corporation")
  },
  {
    0x240F,   /* CYD */
    TEXT("Cyclades Corporation")
  },
  {
    0x2C0F,   /* CYL */
    TEXT("Cyberlabs")
  },
  {
    0x340F,   /* CYT */
    TEXT("Cytechinfo Inc")
  },
  {
    0x360F,   /* CYV */
    TEXT("Cyviz AS")
  },
  {
    0x370F,   /* CYW */
    TEXT("Cyberware")
  },
  {
    0x380F,   /* CYX */
    TEXT("Cyrix Corporation")
  },
  {
    0x2310,   /* DAC */
    TEXT("Digital Acoustics Corporation")
  },
  {
    0x2510,   /* DAE */
    TEXT("Digatron Industrie Elektronik GmbH")
  },
  {
    0x2910,   /* DAI */
    TEXT("DAIS SET Ltd.")
  },
  {
    0x2B10,   /* DAK */
    TEXT("Daktronics")
  },
  {
    0x2C10,   /* DAL */
    TEXT("Digital Audio Labs Inc")
  },
  {
    0x3310,   /* DAS */
    TEXT("DAVIS AS")
  },
  {
    0x3410,   /* DAT */
    TEXT("Datel Inc")
  },
  {
    0x3510,   /* DAU */
    TEXT("Daou Tech Inc")
  },
  {
    0x3610,   /* DAV */
    TEXT("Davicom Semiconductor Inc")
  },
  {
    0x3710,   /* DAW */
    TEXT("DA2 Technologies Inc")
  },
  {
    0x3810,   /* DAX */
    TEXT("Data Apex Ltd")
  },
  {
    0x4410,   /* DBD */
    TEXT("Diebold Inc.")
  },
  {
    0x4910,   /* DBI */
    TEXT("DigiBoard Inc")
  },
  {
    0x4B10,   /* DBK */
    TEXT("Databook Inc")
  },
  {
    0x4C10,   /* DBL */
    TEXT("Doble Engineering Company")
  },
  {
    0x4E10,   /* DBN */
    TEXT("DB Networks Inc")
  },
  {
    0x6110,   /* DCA */
    TEXT("Digital Communications Association")
  },
  {
    0x6310,   /* DCC */
    TEXT("Dale Computer Corporation")
  },
  {
    0x6410,   /* DCD */
    TEXT("Datacast LLC")
  },
  {
    0x6510,   /* DCE */
    TEXT("dSPACE GmbH")
  },
  {
    0x6910,   /* DCI */
    TEXT("Concepts Inc")
  },
  {
    0x6C10,   /* DCL */
    TEXT("Dynamic Controls Ltd")
  },
  {
    0x6D10,   /* DCM */
    TEXT("DCM Data Products")
  },
  {
    0x6F10,   /* DCO */
    TEXT("Dialogue Technology Corporation")
  },
  {
    0x7210,   /* DCR */
    TEXT("Decros Ltd")
  },
  {
    0x7310,   /* DCS */
    TEXT("Diamond Computer Systems Inc")
  },
  {
    0x7410,   /* DCT */
    TEXT("Dancall Telecom A/S")
  },
  {
    0x7610,   /* DCV */
    TEXT("Datatronics Technology Inc")
  },
  {
    0x8110,   /* DDA */
    TEXT("DA2 Technologies Corporation")
  },
  {
    0x8410,   /* DDD */
    TEXT("Danka Data Devices")
  },
  {
    0x8910,   /* DDI */
    TEXT("Data Display AG")
  },
  {
    0x9310,   /* DDS */
    TEXT("Barco, n.v.")
  },
  {
    0x9410,   /* DDT */
    TEXT("Datadesk Technologies Inc")
  },
  {
    0xA310,   /* DEC */
    TEXT("Digital Equipment Corporation")
  },
  {
    0xA910,   /* DEI */
    TEXT("Deico Electronics")
  },
  {
    0xAC10,   /* DEL */
    TEXT("Deltec Corporation")
  },
  {
    0xAE10,   /* DEN */
    TEXT("Densitron Computers Ltd")
  },
  {
    0xB810,   /* DEX */
    TEXT("idex displays")
  },
  {
    0xC910,   /* DFI */
    TEXT("DFI")
  },
  {
    0xCB10,   /* DFK */
    TEXT("SharkTec A/S")
  },
  {
    0xE110,   /* DGA */
    TEXT("Digiital Arts Inc")
  },
  {
    0xE310,   /* DGC */
    TEXT("Data General Corporation")
  },
  {
    0xE910,   /* DGI */
    TEXT("DIGI International")
  },
  {
    0xEB10,   /* DGK */
    TEXT("DugoTech Co., LTD")
  },
  {
    0xF010,   /* DGP */
    TEXT("Digicorp European sales S.A.")
  },
  {
    0xF310,   /* DGS */
    TEXT("Diagsoft Inc")
  },
  {
    0xF410,   /* DGT */
    TEXT("The Dearborn Group")
  },
  {
    0xF410,   /* DGT */
    TEXT("The Dearborn Group")
  },
  {
    0x1011,   /* DHP */
    TEXT("DH Print")
  },
  {
    0x1111,   /* DHQ */
    TEXT("Quadram")
  },
  {
    0x1411,   /* DHT */
    TEXT("Projectavision Inc")
  },
  {
    0x2111,   /* DIA */
    TEXT("Diadem")
  },
  {
    0x2711,   /* DIG */
    TEXT("Digicom S.p.A.")
  },
  {
    0x2911,   /* DII */
    TEXT("Dataq Instruments Inc")
  },
  {
    0x2D11,   /* DIM */
    TEXT("dPict Imaging, Inc.")
  },
  {
    0x2E11,   /* DIN */
    TEXT("Daintelecom Co., Ltd")
  },
  {
    0x3311,   /* DIS */
    TEXT("Diseda S.A.")
  },
  {
    0x3411,   /* DIT */
    TEXT("Dragon Information Technology")
  },
  {
    0x4511,   /* DJE */
    TEXT("Capstone Visua lProduct Development ")
  },
  {
    0x5011,   /* DJP */
    TEXT("Maygay Machines, Ltd")
  },
  {
    0x7911,   /* DKY */
    TEXT("Datakey Inc")
  },
  {
    0x8311,   /* DLC */
    TEXT("Diamond Lane Comm. Corporation")
  },
  {
    0x8711,   /* DLG */
    TEXT("Digital-Logic GmbH")
  },
  {
    0x8B11,   /* DLK */
    TEXT("D-Link Systems Inc")
  },
  {
    0x9411,   /* DLT */
    TEXT("Digitelec Informatique Park Cadera")
  },
  {
    0xA211,   /* DMB */
    TEXT("Digicom Systems Inc")
  },
  {
    0xA311,   /* DMC */
    TEXT("Dune Microsystems Corporation")
  },
  {
    0xAD11,   /* DMM */
    TEXT("Dimond Multimedia Systems Inc")
  },
  {
    0xB011,   /* DMP */
    TEXT("D&M Holdings Inc, Professional Business Company")
  },
  {
    0xB311,   /* DMS */
    TEXT("DOME imaging systems")
  },
  {
    0xB611,   /* DMV */
    TEXT("NDS Ltd")
  },
  {
    0xC111,   /* DNA */
    TEXT("DNA Enterprises, Inc.")
  },
  {
    0xC711,   /* DNG */
    TEXT("Apache Micro Peripherals Inc")
  },
  {
    0xC911,   /* DNI */
    TEXT("Deterministic Networks Inc.")
  },
  {
    0xD411,   /* DNT */
    TEXT("Dr. Neuhous Telekommunikation GmbH")
  },
  {
    0xD611,   /* DNV */
    TEXT("DiCon")
  },
  {
    0xEC11,   /* DOL */
    TEXT("Dolman Technologies Group Inc")
  },
  {
    0xED11,   /* DOM */
    TEXT("Dome Imaging Systems")
  },
  {
    0xEE11,   /* DON */
    TEXT("DENON, Ltd.")
  },
  {
    0xF411,   /* DOT */
    TEXT("Dotronic Mikroelektronik GmbH")
  },
  {
    0x0112,   /* DPA */
    TEXT("DigiTalk Pro AV")
  },
  {
    0x0312,   /* DPC */
    TEXT("Delta Electronics Inc")
  },
  {
    0x0912,   /* DPI */
    TEXT("DocuPoint")
  },
  {
    0x0C12,   /* DPL */
    TEXT("Digital Projection Limited")
  },
  {
    0x0D12,   /* DPM */
    TEXT("ADPM Synthesis sas")
  },
  {
    0x1312,   /* DPS */
    TEXT("Digital Processing Systems")
  },
  {
    0x1412,   /* DPT */
    TEXT("DPT")
  },
  {
    0x1812,   /* DPX */
    TEXT("DpiX, Inc.")
  },
  {
    0x2212,   /* DQB */
    TEXT("Datacube Inc")
  },
  {
    0x4212,   /* DRB */
    TEXT("Dr. Bott KG")
  },
  {
    0x4312,   /* DRC */
    TEXT("Data Ray Corp.")
  },
  {
    0x4412,   /* DRD */
    TEXT("DIGITAL REFLECTION INC.")
  },
  {
    0x4912,   /* DRI */
    TEXT("Data Race Inc")
  },
  {
    0x6412,   /* DSD */
    TEXT("DS Multimedia Pte Ltd")
  },
  {
    0x6912,   /* DSI */
    TEXT("Digitan Systems Inc")
  },
  {
    0x6D12,   /* DSM */
    TEXT("DSM Digital Services GmbH")
  },
  {
    0x7012,   /* DSP */
    TEXT("Domain Technology Inc")
  },
  {
    0x8112,   /* DTA */
    TEXT("DELTATEC ")
  },
  {
    0x8312,   /* DTC */
    TEXT("DTC Tech Corporation")
  },
  {
    0x8912,   /* DTI */
    TEXT("Diversified Technology, Inc.")
  },
  {
    0x8B12,   /* DTK */
    TEXT("Dynax Electronics (HK) Ltd")
  },
  {
    0x8C12,   /* DTL */
    TEXT("e-Net Inc")
  },
  {
    0x8E12,   /* DTN */
    TEXT("Datang  Telephone Co")
  },
  {
    0x8F12,   /* DTO */
    TEXT("Deutsche Thomson OHG")
  },
  {
    0x9812,   /* DTX */
    TEXT("Data Translation")
  },
  {
    0xA112,   /* DUA */
    TEXT("Dosch & Amand GmbH & Company KG")
  },
  {
    0xAE12,   /* DUN */
    TEXT("NCR Corporation")
  },
  {
    0xC412,   /* DVD */
    TEXT("Dictaphone Corporation")
  },
  {
    0xCC12,   /* DVL */
    TEXT("Devolo AG")
  },
  {
    0xD312,   /* DVS */
    TEXT("Digital Video System")
  },
  {
    0xD412,   /* DVT */
    TEXT("Data Video")
  },
  {
    0xE512,   /* DWE */
    TEXT("Daewoo Electronics Company Ltd")
  },
  {
    0x0313,   /* DXC */
    TEXT("Digipronix Control Systems")
  },
  {
    0x1013,   /* DXP */
    TEXT("Data Expert Corporation")
  },
  {
    0x1313,   /* DXS */
    TEXT("Signet")
  },
  {
    0x2313,   /* DYC */
    TEXT("Dycam Inc")
  },
  {
    0x2D13,   /* DYM */
    TEXT("Dymo-CoStar Corporation")
  },
  {
    0x2E13,   /* DYN */
    TEXT("Askey Computer Corporation")
  },
  {
    0x3813,   /* DYX */
    TEXT("Dynax Electronics (HK) Ltd")
  },
  {
    0x3314,   /* EAS */
    TEXT("Evans and Sutherland Computer")
  },
  {
    0x4814,   /* EBH */
    TEXT("Data Price Informatica")
  },
  {
    0x5414,   /* EBT */
    TEXT("HUALONG TECHNOLOGY CO., LTD")
  },
  {
    0x6114,   /* ECA */
    TEXT("Electro Cam Corp.")
  },
  {
    0x6314,   /* ECC */
    TEXT("ESSential Comm. Corporation")
  },
  {
    0x6914,   /* ECI */
    TEXT("Enciris Technologies")
  },
  {
    0x6B14,   /* ECK */
    TEXT("Eugene Chukhlomin Sole Proprietorship, d.b.a. ")
  },
  {
    0x6C14,   /* ECL */
    TEXT("Excel Company Ltd")
  },
  {
    0x6D14,   /* ECM */
    TEXT("E-Cmos Tech Corporation")
  },
  {
    0x6F14,   /* ECO */
    TEXT("Echo Speech Corporation")
  },
  {
    0x7014,   /* ECP */
    TEXT("Elecom Company Ltd")
  },
  {
    0x7314,   /* ECS */
    TEXT("Elitegroup Computer Systems Company Ltd")
  },
  {
    0x7414,   /* ECT */
    TEXT("Enciris Technologies ")
  },
  {
    0x8314,   /* EDC */
    TEXT("e.Digital Corporation")
  },
  {
    0x8714,   /* EDG */
    TEXT("Electronic-Design GmbH")
  },
  {
    0x8914,   /* EDI */
    TEXT("Edimax Tech. Company Ltd")
  },
  {
    0x8D14,   /* EDM */
    TEXT("EDMI")
  },
  {
    0xA514,   /* EEE */
    TEXT("ET&T Technology Company Ltd")
  },
  {
    0xA814,   /* EEH */
    TEXT("EEH Datalink GmbH")
  },
  {
    0xB014,   /* EEP */
    TEXT("E.E.P.D. GmbH")
  },
  {
    0xB314,   /* EES */
    TEXT("EE Solutions, Inc.")
  },
  {
    0xE414,   /* EGD */
    TEXT("EIZO GmbH Display Technologies ")
  },
  {
    0xEC14,   /* EGL */
    TEXT("Eagle Technology")
  },
  {
    0xEE14,   /* EGN */
    TEXT("Egenera, Inc.")
  },
  {
    0xEF14,   /* EGO */
    TEXT("Ergo Electronics")
  },
  {
    0x0A15,   /* EHJ */
    TEXT("Epson Research")
  },
  {
    0x2315,   /* EIC */
    TEXT("Eicon Technology Corporation")
  },
  {
    0x6115,   /* EKA */
    TEXT("MagTek Inc.")
  },
  {
    0x6315,   /* EKC */
    TEXT("Eastman Kodak Company")
  },
  {
    0x7315,   /* EKS */
    TEXT("EKSEN YAZILIM")
  },
  {
    0x8115,   /* ELA */
    TEXT("ELAD srl")
  },
  {
    0x8315,   /* ELC */
    TEXT("Electro Scientific Ind")
  },
  {
    0x8515,   /* ELE */
    TEXT("Elecom Company Ltd")
  },
  {
    0x8715,   /* ELG */
    TEXT("Elmeg GmbH Kommunikationstechnik")
  },
  {
    0x8915,   /* ELI */
    TEXT("Edsun Laboratories")
  },
  {
    0x8C15,   /* ELL */
    TEXT("Electrosonic Ltd")
  },
  {
    0x8D15,   /* ELM */
    TEXT("Elmic Systems Inc")
  },
  {
    0x8F15,   /* ELO */
    TEXT("Elo TouchSystems Inc")
  },
  {
    0x8F15,   /* ELO */
    TEXT("Elo TouchSystems Inc")
  },
  {
    0x9315,   /* ELS */
    TEXT("ELSA GmbH")
  },
  {
    0x9415,   /* ELT */
    TEXT("Element Labs, Inc.")
  },
  {
    0x9815,   /* ELX */
    TEXT("Elonex PLC")
  },
  {
    0xA215,   /* EMB */
    TEXT("Embedded computing inc ltd")
  },
  {
    0xA315,   /* EMC */
    TEXT("eMicro Corporation")
  },
  {
    0xA515,   /* EME */
    TEXT("EMiNE TECHNOLOGY COMPANY, LTD.")
  },
  {
    0xA715,   /* EMG */
    TEXT("EMG Consultants Inc")
  },
  {
    0xA915,   /* EMI */
    TEXT("Ex Machina Inc")
  },
  {
    0xB515,   /* EMU */
    TEXT("Emulex Corporation")
  },
  {
    0xC315,   /* ENC */
    TEXT("Eizo Nanao Corporation")
  },
  {
    0xC415,   /* END */
    TEXT("ENIDAN Technologies Ltd")
  },
  {
    0xC515,   /* ENE */
    TEXT("ENE Technology Inc.")
  },
  {
    0xC915,   /* ENI */
    TEXT("Efficient Networks")
  },
  {
    0xD315,   /* ENS */
    TEXT("Ensoniq Corporation")
  },
  {
    0xD415,   /* ENT */
    TEXT("Enterprise Comm. & Computing Inc")
  },
  {
    0x0316,   /* EPC */
    TEXT("Empac")
  },
  {
    0x0916,   /* EPI */
    TEXT("Envision Peripherals, Inc")
  },
  {
    0x0E16,   /* EPN */
    TEXT("EPiCON Inc.")
  },
  {
    0x1316,   /* EPS */
    TEXT("KEPS")
  },
  {
    0x3016,   /* EQP */
    TEXT("Equipe Electronics Ltd.")
  },
  {
    0x3816,   /* EQX */
    TEXT("Equinox Systems Inc")
  },
  {
    0x4716,   /* ERG */
    TEXT("Ergo System")
  },
  {
    0x4916,   /* ERI */
    TEXT("Ericsson Mobile Communications AB")
  },
  {
    0x4E16,   /* ERN */
    TEXT("Ericsson, Inc.")
  },
  {
    0x5016,   /* ERP */
    TEXT("Euraplan GmbH")
  },
  {
    0x5416,   /* ERT */
    TEXT("Escort Insturments Corporation")
  },
  {
    0x6316,   /* ESC */
    TEXT("Eden Sistemas de Computacao S/A")
  },
  {
    0x6716,   /* ESG */
    TEXT("ELCON Systemtechnik GmbH")
  },
  {
    0x6916,   /* ESI */
    TEXT("Extended Systems, Inc.")
  },
  {
    0x6B16,   /* ESK */
    TEXT("ES&S")
  },
  {
    0x7316,   /* ESS */
    TEXT("ESS Technology Inc")
  },
  {
    0x7416,   /* EST */
    TEXT("Embedded Solution Technology")
  },
  {
    0x7916,   /* ESY */
    TEXT("E-Systems Inc")
  },
  {
    0x8316,   /* ETC */
    TEXT("Everton Technology Company Ltd")
  },
  {
    0x8916,   /* ETI */
    TEXT("Eclipse Tech Inc")
  },
  {
    0x8B16,   /* ETK */
    TEXT("eTEK Labs Inc.")
  },
  {
    0x8C16,   /* ETL */
    TEXT("Evertz Microsystems Ltd.")
  },
  {
    0x9316,   /* ETS */
    TEXT("Electronic Trade Solutions Ltd")
  },
  {
    0x9416,   /* ETT */
    TEXT("E-Tech Inc")
  },
  {
    0xB416,   /* EUT */
    TEXT("Ericsson Mobile Networks B.V.")
  },
  {
    0xC916,   /* EVI */
    TEXT("eviateg GmbH")
  },
  {
    0xD816,   /* EVX */
    TEXT("Everex")
  },
  {
    0x0117,   /* EXA */
    TEXT("Exabyte")
  },
  {
    0x0317,   /* EXC */
    TEXT("Excession Audio")
  },
  {
    0x0917,   /* EXI */
    TEXT("Exide Electronics")
  },
  {
    0x0E17,   /* EXN */
    TEXT("RGB Systems, Inc. dba Extron Electronics")
  },
  {
    0x1017,   /* EXP */
    TEXT("Data Export Corporation")
  },
  {
    0x1417,   /* EXT */
    TEXT("Exatech Computadores & Servicos Ltda")
  },
  {
    0x1817,   /* EXX */
    TEXT("Exxact GmbH")
  },
  {
    0x1917,   /* EXY */
    TEXT("Exterity Ltd")
  },
  {
    0x4517,   /* EZE */
    TEXT("EzE Technologies")
  },
  {
    0x5017,   /* EZP */
    TEXT("Storm Technology")
  },
  {
    0x3218,   /* FAR */
    TEXT("Farallon Computing")
  },
  {
    0x4918,   /* FBI */
    TEXT("Interface Corporation")
  },
  {
    0x6218,   /* FCB */
    TEXT("Furukawa Electric Company Ltd")
  },
  {
    0x6718,   /* FCG */
    TEXT("First International Computer Ltd")
  },
  {
    0x7318,   /* FCS */
    TEXT("Focus Enhancements, Inc.")
  },
  {
    0x8318,   /* FDC */
    TEXT("Future Domain")
  },
  {
    0x9418,   /* FDT */
    TEXT("Fujitsu Display Technologies Corp.")
  },
  {
    0xA318,   /* FEC */
    TEXT("FURUNO ELECTRIC CO., LTD.")
  },
  {
    0xAC18,   /* FEL */
    TEXT("Fellowes & Questec")
  },
  {
    0xB218,   /* FER */
    TEXT("Ferranti Int'L")
  },
  {
    0xC918,   /* FFI */
    TEXT("Fairfield Industries")
  },
  {
    0xE418,   /* FGD */
    TEXT("Lisa Draexlmaier GmbH")
  },
  {
    0xEC18,   /* FGL */
    TEXT("Fujitsu General Limited.")
  },
  {
    0x0C19,   /* FHL */
    TEXT("FHLP")
  },
  {
    0x2319,   /* FIC */
    TEXT("Formosa Industrial Computing Inc")
  },
  {
    0x2C19,   /* FIL */
    TEXT("Forefront Int'l Ltd")
  },
  {
    0x2E19,   /* FIN */
    TEXT("Finecom Co., Ltd.")
  },
  {
    0x3219,   /* FIR */
    TEXT("Chaplet Systems Inc")
  },
  {
    0x3319,   /* FIS */
    TEXT("FLY-IT Simulators")
  },
  {
    0x4319,   /* FJC */
    TEXT("Fujitsu Takamisawa Component Limited")
  },
  {
    0x5319,   /* FJS */
    TEXT("Fujitsu Spain")
  },
  {
    0x5419,   /* FJT */
    TEXT("F.J. Tieman BV")
  },
  {
    0x8919,   /* FLI */
    TEXT("Faroudja Laboratories")
  },
  {
    0x9919,   /* FLY */
    TEXT("Butterfly Communications")
  },
  {
    0xA119,   /* FMA */
    TEXT("Fast Multimedia AG")
  },
  {
    0xA319,   /* FMC */
    TEXT("Ford Microelectronics Inc")
  },
  {
    0xA919,   /* FMI */
    TEXT("Fujitsu Microelect Inc")
  },
  {
    0xA919,   /* FMI */
    TEXT("Fujitsu Microelect Inc")
  },
  {
    0xAC19,   /* FML */
    TEXT("Fujitsu Microelect Ltd")
  },
  {
    0xBA19,   /* FMZ */
    TEXT("Formoza-Altair")
  },
  {
    0xC319,   /* FNC */
    TEXT("Fanuc LTD")
  },
  {
    0xC919,   /* FNI */
    TEXT("Funai Electric Co., Ltd.")
  },
  {
    0xE119,   /* FOA */
    TEXT("FOR-A Company Limited")
  },
  {
    0xF319,   /* FOS */
    TEXT("Foss Tecator")
  },
  {
    0x051A,   /* FPE */
    TEXT("Fujitsu Peripherals Ltd")
  },
  {
    0x131A,   /* FPS */
    TEXT("Deltec Corporation")
  },
  {
    0x181A,   /* FPX */
    TEXT("Cirel Systemes")
  },
  {
    0x431A,   /* FRC */
    TEXT("Force Computers")
  },
  {
    0x441A,   /* FRD */
    TEXT("Freedom Scientific BLV")
  },
  {
    0x451A,   /* FRE */
    TEXT("Forvus Research Inc")
  },
  {
    0x491A,   /* FRI */
    TEXT("Fibernet Research Inc")
  },
  {
    0x531A,   /* FRS */
    TEXT("South Mountain Technologies, LTD")
  },
  {
    0x631A,   /* FSC */
    TEXT("Future Systems Consulting KK")
  },
  {
    0x691A,   /* FSI */
    TEXT("Fore Systems Inc")
  },
  {
    0x741A,   /* FST */
    TEXT("Modesto PC Inc")
  },
  {
    0x831A,   /* FTC */
    TEXT("Futuretouch Corporation")
  },
  {
    0x851A,   /* FTE */
    TEXT("Frontline Test Equipment Inc.")
  },
  {
    0x871A,   /* FTG */
    TEXT("FTG Data Systems")
  },
  {
    0x891A,   /* FTI */
    TEXT("FastPoint Technologies, Inc.")
  },
  {
    0x8E1A,   /* FTN */
    TEXT("Fountain Technologies Inc")
  },
  {
    0x921A,   /* FTR */
    TEXT("Mediasonic")
  },
  {
    0xAA1A,   /* FUJ */
    TEXT("Fujitsu Ltd")
  },
  {
    0xAE1A,   /* FUN */
    TEXT("sisel muhendislik")
  },
  {
    0xB31A,   /* FUS */
    TEXT("Fujitsu Siemens Computers GmbH")
  },
  {
    0xC31A,   /* FVC */
    TEXT("First Virtual Corporation")
  },
  {
    0xD81A,   /* FVX */
    TEXT("C-C-C Group Plc")
  },
  {
    0xF21A,   /* FWR */
    TEXT("Flat Connections Inc")
  },
  {
    0x181B,   /* FXX */
    TEXT("Fuji Xerox")
  },
  {
    0x431B,   /* FZC */
    TEXT("Founder Group Shenzhen Co.")
  },
  {
    0x491B,   /* FZI */
    TEXT("FZI Forschungszentrum Informatik")
  },
  {
    0x271C,   /* GAG */
    TEXT("Gage Applied Sciences Inc")
  },
  {
    0x2C1C,   /* GAL */
    TEXT("Galil Motion Control")
  },
  {
    0x351C,   /* GAU */
    TEXT("Gaudi Co., Ltd.")
  },
  {
    0x631C,   /* GCC */
    TEXT("GCC Technologies Inc")
  },
  {
    0x691C,   /* GCI */
    TEXT("Gateway Comm. Inc")
  },
  {
    0x731C,   /* GCS */
    TEXT("Grey Cell Systems Ltd")
  },
  {
    0x831C,   /* GDC */
    TEXT("General Datacom")
  },
  {
    0x891C,   /* GDI */
    TEXT("G. Diehl ISDN GmbH")
  },
  {
    0x931C,   /* GDS */
    TEXT("GDS")
  },
  {
    0x941C,   /* GDT */
    TEXT("Vortex Computersysteme GmbH")
  },
  {
    0xA61C,   /* GEF */
    TEXT("GE Fanuc Embedded Systems")
  },
  {
    0xAD1C,   /* GEM */
    TEXT("Gem Plus")
  },
  {
    0xAE1C,   /* GEN */
    TEXT("Genesys ATE Inc")
  },
  {
    0xAF1C,   /* GEO */
    TEXT("GEO Sense")
  },
  {
    0xB31C,   /* GES */
    TEXT("GES Singapore Pte Ltd")
  },
  {
    0xCD1C,   /* GFM */
    TEXT("GFMesstechnik GmbH")
  },
  {
    0xCE1C,   /* GFN */
    TEXT("Gefen Inc.")
  },
  {
    0x231D,   /* GIC */
    TEXT("General Inst. Corporation")
  },
  {
    0x2D1D,   /* GIM */
    TEXT("Guillemont International")
  },
  {
    0x331D,   /* GIS */
    TEXT("AT&T Global Info Solutions")
  },
  {
    0x4E1D,   /* GJN */
    TEXT("Grand Junction Networks")
  },
  {
    0x851D,   /* GLE */
    TEXT("AD electronics")
  },
  {
    0x8D1D,   /* GLM */
    TEXT("Genesys Logic")
  },
  {
    0x931D,   /* GLS */
    TEXT("Gadget Labs LLC")
  },
  {
    0xAB1D,   /* GMK */
    TEXT("GMK Electronic Design GmbH")
  },
  {
    0xAC1D,   /* GML */
    TEXT("General Information Systems")
  },
  {
    0xAD1D,   /* GMM */
    TEXT("GMM Research Inc")
  },
  {
    0xAE1D,   /* GMN */
    TEXT("GEMINI 2000 Ltd")
  },
  {
    0xB81D,   /* GMX */
    TEXT("GMX Inc")
  },
  {
    0xC41D,   /* GND */
    TEXT("Gennum Corporation")
  },
  {
    0xCE1D,   /* GNN */
    TEXT("GN Nettest Inc")
  },
  {
    0xDA1D,   /* GNZ */
    TEXT("Gunze Ltd")
  },
  {
    0x411E,   /* GRA */
    TEXT("Graphica Computer")
  },
  {
    0x451E,   /* GRE */
    TEXT("GOLD RAIN ENTERPRISES CORP.")
  },
  {
    0x481E,   /* GRH */
    TEXT("Granch Ltd")
  },
  {
    0x561E,   /* GRV */
    TEXT("Advanced Gravis")
  },
  {
    0x591E,   /* GRY */
    TEXT("Robert Gray Company")
  },
  {
    0x621E,   /* GSB */
    TEXT("NIPPONDENCHI CO,.LTD")
  },
  {
    0x631E,   /* GSC */
    TEXT("General Standards Corporation")
  },
  {
    0x6D1E,   /* GSM */
    TEXT("Goldstar Company Ltd")
  },
  {
    0x741E,   /* GST */
    TEXT("Graphic SystemTechnology")
  },
  {
    0x791E,   /* GSY */
    TEXT("Grossenbacher Systeme AG")
  },
  {
    0x831E,   /* GTC */
    TEXT("Graphtec Corporation")
  },
  {
    0x891E,   /* GTI */
    TEXT("Goldtouch")
  },
  {
    0x8B1E,   /* GTK */
    TEXT("G-Tech Corporation")
  },
  {
    0x8D1E,   /* GTM */
    TEXT("Garnet System Company Ltd")
  },
  {
    0x931E,   /* GTS */
    TEXT("Geotest Marvin Test Systems Inc")
  },
  {
    0x941E,   /* GTT */
    TEXT("General Touch Technology Co., Ltd.")
  },
  {
    0xA41E,   /* GUD */
    TEXT("Guntermann & Drunck GmbH")
  },
  {
    0xBA1E,   /* GUZ */
    TEXT("Guzik Technical Enterprises")
  },
  {
    0xC31E,   /* GVC */
    TEXT("GVC Corporation")
  },
  {
    0xCC1E,   /* GVL */
    TEXT("Global Village Communication")
  },
  {
    0xE91E,   /* GWI */
    TEXT("GW Instruments")
  },
  {
    0xF91E,   /* GWY */
    TEXT("Gateway 2000")
  },
  {
    0x451F,   /* GZE */
    TEXT("GUNZE Limited")
  },
  {
    0x2520,   /* HAE */
    TEXT("Haider electronics")
  },
  {
    0x2920,   /* HAI */
    TEXT("Haivision Systems Inc.")
  },
  {
    0x2C20,   /* HAL */
    TEXT("Halberthal")
  },
  {
    0x2E20,   /* HAN */
    TEXT("Hanchang System Corporation")
  },
  {
    0x3920,   /* HAY */
    TEXT("Hayes Microcomputer Products Inc")
  },
  {
    0x6120,   /* HCA */
    TEXT("DAT")
  },
  {
    0x6C20,   /* HCL */
    TEXT("HCL America Inc")
  },
  {
    0x6D20,   /* HCM */
    TEXT("HCL Peripherals")
  },
  {
    0x7020,   /* HCP */
    TEXT("Hitachi Computer Products Inc")
  },
  {
    0x7720,   /* HCW */
    TEXT("Hauppauge Computer Works Inc")
  },
  {
    0x8320,   /* HDC */
    TEXT("HardCom Elektronik & Datateknik")
  },
  {
    0x8920,   /* HDI */
    TEXT("HD-INFO d.o.o.")
  },
  {
    0x9620,   /* HDV */
    TEXT("Holografika kft.")
  },
  {
    0xA320,   /* HEC */
    TEXT("Hitachi Engineering Company Ltd")
  },
  {
    0xA320,   /* HEC */
    TEXT("Hitachi Engineering Company Ltd")
  },
  {
    0xAC20,   /* HEL */
    TEXT("Hitachi Micro Systems Europe Ltd")
  },
  {
    0xB220,   /* HER */
    TEXT("Ascom Business Systems")
  },
  {
    0xB420,   /* HET */
    TEXT("HETEC Datensysteme GmbH")
  },
  {
    0x0321,   /* HHC */
    TEXT("HIRAKAWA HEWTECH CORP. ")
  },
  {
    0x2221,   /* HIB */
    TEXT("Hibino Corporation")
  },
  {
    0x2321,   /* HIC */
    TEXT("Hitachi Information Technology Co., Ltd.")
  },
  {
    0x2B21,   /* HIK */
    TEXT("Hikom Co., Ltd.")
  },
  {
    0x2C21,   /* HIL */
    TEXT("Hilevel Technology")
  },
  {
    0x3421,   /* HIT */
    TEXT("Hitachi America Ltd")
  },
  {
    0x4921,   /* HJI */
    TEXT("Harris & Jeffries Inc")
  },
  {
    0x6121,   /* HKA */
    TEXT("HONKO MFG. CO., LTD.")
  },
  {
    0x6721,   /* HKG */
    TEXT("Josef Heim KG")
  },
  {
    0xA321,   /* HMC */
    TEXT("Hualon Microelectric Corporation")
  },
  {
    0xAB21,   /* HMK */
    TEXT("hmk Daten-System-Technik BmbH")
  },
  {
    0xB821,   /* HMX */
    TEXT("HUMAX Co., Ltd.")
  },
  {
    0xD321,   /* HNS */
    TEXT("Hughes Network Systems")
  },
  {
    0xE221,   /* HOB */
    TEXT("HOB Electronic GmbH")
  },
  {
    0xE521,   /* HOE */
    TEXT("Hosiden Corporation")
  },
  {
    0xEC21,   /* HOL */
    TEXT("Holoeye Photonics AG")
  },
  {
    0x0322,   /* HPC */
    TEXT("Hewlett Packard Co.")
  },
  {
    0x0422,   /* HPD */
    TEXT("Hewlett Packard")
  },
  {
    0x0922,   /* HPI */
    TEXT("Headplay, Inc.")
  },
  {
    0x0B22,   /* HPK */
    TEXT("HAMAMATSU PHOTONICS K.K.")
  },
  {
    0x1122,   /* HPQ */
    TEXT("HP")
  },
  {
    0x1222,   /* HPR */
    TEXT("H.P.R. Electronics GmbH")
  },
  {
    0x4322,   /* HRC */
    TEXT("Hercules")
  },
  {
    0x4522,   /* HRE */
    TEXT("Qingdao Haier Electronics Co., Ltd.")
  },
  {
    0x4C22,   /* HRL */
    TEXT("Herolab GmbH")
  },
  {
    0x5322,   /* HRS */
    TEXT("Harris Semiconductor")
  },
  {
    0x5422,   /* HRT */
    TEXT("HERCULES")
  },
  {
    0x6322,   /* HSC */
    TEXT("Hagiwara Sys-Com Company Ltd")
  },
  {
    0x6D22,   /* HSM */
    TEXT("AT&T Microelectronics")
  },
  {
    0x8322,   /* HTC */
    TEXT("Hitachi Ltd")
  },
  {
    0x8922,   /* HTI */
    TEXT("Hampshire Company, Inc.")
  },
  {
    0x8B22,   /* HTK */
    TEXT("Holtek Microelectronics Inc")
  },
  {
    0x9822,   /* HTX */
    TEXT("Hitex Systementwicklung GmbH")
  },
  {
    0xA222,   /* HUB */
    TEXT("GAI-Tronics, A Hubbell Company")
  },
  {
    0xAD22,   /* HUM */
    TEXT("IMP Electronics Ltd.")
  },
  {
    0xE122,   /* HWA */
    TEXT("Harris Canada Inc")
  },
  {
    0xE322,   /* HWC */
    TEXT("DBA Hans Wedemeyer")
  },
  {
    0xE422,   /* HWD */
    TEXT("Highwater Designs Ltd")
  },
  {
    0xF022,   /* HWP */
    TEXT("Hewlett Packard")
  },
  {
    0x0D23,   /* HXM */
    TEXT("Hexium Ltd.")
  },
  {
    0x2323,   /* HYC */
    TEXT("Hypercope Gmbh Aachen")
  },
  {
    0x2F23,   /* HYO */
    TEXT("HYC CO., LTD.")
  },
  {
    0x3023,   /* HYP */
    TEXT("Hyphen Ltd")
  },
  {
    0x3223,   /* HYR */
    TEXT("Hypertec Pty Ltd")
  },
  {
    0x3423,   /* HYT */
    TEXT("Heng Yu Technology (HK) Limited")
  },
  {
    0x3623,   /* HYV */
    TEXT("Hynix Semiconductor")
  },
  {
    0x2624,   /* IAF */
    TEXT("Institut f r angewandte Funksystemtechnik GmbH")
  },
  {
    0x2924,   /* IAI */
    TEXT("Integration Associates, Inc.")
  },
  {
    0x3424,   /* IAT */
    TEXT("IAT Germany GmbH")
  },
  {
    0x4324,   /* IBC */
    TEXT("Integrated Business Systems")
  },
  {
    0x4924,   /* IBI */
    TEXT("INBINE.CO.LTD")
  },
  {
    0x4D24,   /* IBM */
    TEXT("IBM Brasil")
  },
  {
    0x4D24,   /* IBM */
    TEXT("IBM Brasil")
  },
  {
    0x5024,   /* IBP */
    TEXT("IBP Instruments GmbH")
  },
  {
    0x5224,   /* IBR */
    TEXT("IBR GmbH")
  },
  {
    0x6124,   /* ICA */
    TEXT("ICA Inc")
  },
  {
    0x6324,   /* ICC */
    TEXT("BICC Data Networks Ltd")
  },
  {
    0x6424,   /* ICD */
    TEXT("ICD Inc")
  },
  {
    0x6524,   /* ICE */
    TEXT("IC Ensemble")
  },
  {
    0x6924,   /* ICI */
    TEXT("Infotek Communication Inc")
  },
  {
    0x6D24,   /* ICM */
    TEXT("Intracom SA")
  },
  {
    0x6E24,   /* ICN */
    TEXT("Sanyo Icon")
  },
  {
    0x6F24,   /* ICO */
    TEXT("Intel Corp")
  },
  {
    0x7324,   /* ICS */
    TEXT("Integrated Circuit Systems")
  },
  {
    0x7824,   /* ICX */
    TEXT("ICCC A/S")
  },
  {
    0x8324,   /* IDC */
    TEXT("International Datacasting Corporation")
  },
  {
    0x8524,   /* IDE */
    TEXT("IDE Associates")
  },
  {
    0x8B24,   /* IDK */
    TEXT("IDK Corporation")
  },
  {
    0x8F24,   /* IDO */
    TEXT("IDEO Product Development")
  },
  {
    0x9324,   /* IDS */
    TEXT("Interdigital Sistemas de Informacao")
  },
  {
    0x9424,   /* IDT */
    TEXT("International Display Technology")
  },
  {
    0x9824,   /* IDX */
    TEXT("IDEXX Labs")
  },
  {
    0xA324,   /* IEC */
    TEXT("Interlace Engineering Corporation")
  },
  {
    0xA524,   /* IEE */
    TEXT("IEE")
  },
  {
    0xA924,   /* IEI */
    TEXT("Interlink Electronics")
  },
  {
    0xD324,   /* IFS */
    TEXT("In Focus Systems Inc")
  },
  {
    0xD424,   /* IFT */
    TEXT("Informtech")
  },
  {
    0xD824,   /* IFX */
    TEXT("Infineon Technologies AG")
  },
  {
    0xE324,   /* IGC */
    TEXT("Intergate Pty Ltd")
  },
  {
    0xED24,   /* IGM */
    TEXT("IGM Communi")
  },
  {
    0x2325,   /* IIC */
    TEXT("ISIC Innoscan Industrial Computers A/S")
  },
  {
    0x2925,   /* III */
    TEXT("Intelligent Instrumentation")
  },
  {
    0x2E25,   /* IIN */
    TEXT("IINFRA Co., Ltd")
  },
  {
    0x7325,   /* IKS */
    TEXT("Ikos Systems Inc")
  },
  {
    0x8325,   /* ILC */
    TEXT("Image Logic Corporation")
  },
  {
    0x9325,   /* ILS */
    TEXT("Innotech Corporation")
  },
  {
    0xA125,   /* IMA */
    TEXT("Imagraph")
  },
  {
    0xA325,   /* IMC */
    TEXT("IMC Networks")
  },
  {
    0xA425,   /* IMD */
    TEXT("ImasDe Canarias S.A.")
  },
  {
    0xA525,   /* IME */
    TEXT("Imagraph")
  },
  {
    0xA725,   /* IMG */
    TEXT("IMAGENICS Co., Ltd.")
  },
  {
    0xA925,   /* IMI */
    TEXT("International Microsystems Inc")
  },
  {
    0xAD25,   /* IMM */
    TEXT("Immersion Corporation")
  },
  {
    0xAE25,   /* IMN */
    TEXT("Impossible Production")
  },
  {
    0xB025,   /* IMP */
    TEXT("Impression Products Incorporated")
  },
  {
    0xB425,   /* IMT */
    TEXT("Inmax Technology Corporation")
  },
  {
    0xC325,   /* INC */
    TEXT("Home Row Inc")
  },
  {
    0xC425,   /* IND */
    TEXT("ILC")
  },
  {
    0xC525,   /* INE */
    TEXT("Inventec Electronics (M) Sdn. Bhd.")
  },
  {
    0xC625,   /* INF */
    TEXT("Inframetrics Inc")
  },
  {
    0xC725,   /* ING */
    TEXT("Integraph Corporation")
  },
  {
    0xC925,   /* INI */
    TEXT("Initio Corporation")
  },
  {
    0xCB25,   /* INK */
    TEXT("Indtek Co., Ltd.")
  },
  {
    0xCC25,   /* INL */
    TEXT("InnoLux Display Corporation")
  },
  {
    0xCD25,   /* INM */
    TEXT("InnoMedia Inc")
  },
  {
    0xCE25,   /* INN */
    TEXT("Innovent Systems, Inc.")
  },
  {
    0xCF25,   /* INO */
    TEXT("Innolab Pte Ltd")
  },
  {
    0xD025,   /* INP */
    TEXT("Interphase Corporation")
  },
  {
    0xD325,   /* INS */
    TEXT("Ines GmbH")
  },
  {
    0xD425,   /* INT */
    TEXT("Interphase Corporation")
  },
  {
    0xD525,   /* INU */
    TEXT("Inovatec S.p.A.")
  },
  {
    0xD625,   /* INV */
    TEXT("Inviso, Inc.")
  },
  {
    0xDA25,   /* INZ */
    TEXT("Best Buy")
  },
  {
    0xE125,   /* IOA */
    TEXT("CRE Technology Corporation")
  },
  {
    0xE425,   /* IOD */
    TEXT("I-O Data Device Inc")
  },
  {
    0xED25,   /* IOM */
    TEXT("Iomega")
  },
  {
    0xEE25,   /* ION */
    TEXT("Inside Out Networks")
  },
  {
    0xF325,   /* IOS */
    TEXT("i-O Display System")
  },
  {
    0xF425,   /* IOT */
    TEXT("I/OTech Inc")
  },
  {
    0x0326,   /* IPC */
    TEXT("IPC Corporation")
  },
  {
    0x0426,   /* IPD */
    TEXT("Industrial Products Design, Inc.")
  },
  {
    0x0926,   /* IPI */
    TEXT("Intelligent Platform Management Interface (IPMI) forum (Intel, HP, NEC, Dell)")
  },
  {
    0x0D26,   /* IPM */
    TEXT("IPM Industria Politecnica Meridionale SpA")
  },
  {
    0x0E26,   /* IPN */
    TEXT("Performance Technologies")
  },
  {
    0x1226,   /* IPR */
    TEXT("Ithaca Peripherals")
  },
  {
    0x1326,   /* IPS */
    TEXT("IPS, Inc. (Intellectual Property Solutions, Inc.)")
  },
  {
    0x1426,   /* IPT */
    TEXT("International Power Technologies")
  },
  {
    0x1726,   /* IPW */
    TEXT("IPWireless, Inc")
  },
  {
    0x3426,   /* IQT */
    TEXT("IMAGEQUEST Co., Ltd")
  },
  {
    0x4426,   /* IRD */
    TEXT("IRdata")
  },
  {
    0x6126,   /* ISA */
    TEXT("Symbol Technologies")
  },
  {
    0x6326,   /* ISC */
    TEXT("Id3 Semiconductors")
  },
  {
    0x6726,   /* ISG */
    TEXT("Insignia Solutions Inc")
  },
  {
    0x6926,   /* ISI */
    TEXT("Interface Solutions")
  },
  {
    0x6C26,   /* ISL */
    TEXT("Isolation Systems")
  },
  {
    0x7026,   /* ISP */
    TEXT("IntreSource Systems Pte Ltd")
  },
  {
    0x7226,   /* ISR */
    TEXT("INSIS Co., LTD.")
  },
  {
    0x7326,   /* ISS */
    TEXT("ISS Inc")
  },
  {
    0x7426,   /* IST */
    TEXT("Intersolve Technologies")
  },
  {
    0x7926,   /* ISY */
    TEXT("International Integrated Systems,Inc.(IISI)")
  },
  {
    0x8126,   /* ITA */
    TEXT("Itausa Export North America")
  },
  {
    0x8326,   /* ITC */
    TEXT("Intercom Inc")
  },
  {
    0x8426,   /* ITD */
    TEXT("Internet Technology Corporation")
  },
  {
    0x8526,   /* ITE */
    TEXT("Integrated Tech Express Inc")
  },
  {
    0x8B26,   /* ITK */
    TEXT("ITK Telekommunikation AG")
  },
  {
    0x8C26,   /* ITL */
    TEXT("Inter-Tel")
  },
  {
    0x8D26,   /* ITM */
    TEXT("ITM inc.")
  },
  {
    0x8E26,   /* ITN */
    TEXT("The NTI Group")
  },
  {
    0x9026,   /* ITP */
    TEXT("IT-PRO Consulting und Systemhaus GmbH")
  },
  {
    0x9226,   /* ITR */
    TEXT("Infotronic America, Inc.")
  },
  {
    0x9326,   /* ITS */
    TEXT("IDTECH")
  },
  {
    0x9426,   /* ITT */
    TEXT("I&T Telecom.")
  },
  {
    0x9826,   /* ITX */
    TEXT("integrated Technology Express Inc")
  },
  {
    0xA326,   /* IUC */
    TEXT("ICSL")
  },
  {
    0xC926,   /* IVI */
    TEXT("Intervoice Inc")
  },
  {
    0xCD26,   /* IVM */
    TEXT("Liyama North America")
  },
  {
    0xF226,   /* IWR */
    TEXT("Icuiti Corporation")
  },
  {
    0xF826,   /* IWX */
    TEXT("Intelliworxx, Inc.")
  },
  {
    0x0427,   /* IXD */
    TEXT("Intertex Data AB")
  },
  {
    0x2328,   /* JAC */
    TEXT("Astec Inc")
  },
  {
    0x2528,   /* JAE */
    TEXT("Japan Aviation Electronics Industry, Limited")
  },
  {
    0x3428,   /* JAT */
    TEXT("Jaton Corporation")
  },
  {
    0x3A28,   /* JAZ */
    TEXT("Carrera Computer Inc (used as second pnpid)")
  },
  {
    0x6528,   /* JCE */
    TEXT("Jace Tech Inc")
  },
  {
    0x8C28,   /* JDL */
    TEXT("Japan Digital Laboratory Co.,Ltd.")
  },
  {
    0xAE28,   /* JEN */
    TEXT("N-Vision")
  },
  {
    0xB428,   /* JET */
    TEXT("JET POWER TECHNOLOGY CO., LTD.")
  },
  {
    0xD828,   /* JFX */
    TEXT("Jones Futurex Inc")
  },
  {
    0xE428,   /* JGD */
    TEXT("University College")
  },
  {
    0x2329,   /* JIC */
    TEXT("Jaeik Information & Communication Co., Ltd.")
  },
  {
    0xB429,   /* JMT */
    TEXT("Micro Technical Company Ltd")
  },
  {
    0x032A,   /* JPC */
    TEXT("JPC Technology Limited")
  },
  {
    0x172A,   /* JPW */
    TEXT("Wallis Hamilton Industries")
  },
  {
    0x252A,   /* JQE */
    TEXT("CNet Technical Inc")
  },
  {
    0x642A,   /* JSD */
    TEXT("JS DigiTech, Inc")
  },
  {
    0x692A,   /* JSI */
    TEXT("Jupiter Systems, Inc.")
  },
  {
    0x6B2A,   /* JSK */
    TEXT("SANKEN ELECTRIC CO., LTD")
  },
  {
    0x932A,   /* JTS */
    TEXT("JS Motorsports")
  },
  {
    0xAB2A,   /* JUK */
    TEXT("Janich & Klass Computertechnik GmbH")
  },
  {
    0xB02A,   /* JUP */
    TEXT("Jupiter Systems")
  },
  {
    0xC32A,   /* JVC */
    TEXT("JVC")
  },
  {
    0xE42A,   /* JWD */
    TEXT("Video International Inc.")
  },
  {
    0xEC2A,   /* JWL */
    TEXT("Jewell Instruments, LLC")
  },
  {
    0xF32A,   /* JWS */
    TEXT("JWSpencer & Co.")
  },
  {
    0xF92A,   /* JWY */
    TEXT("Jetway Information Co., Ltd")
  },
  {
    0x322C,   /* KAR */
    TEXT("Karna")
  },
  {
    0x492C,   /* KBI */
    TEXT("Kidboard Inc")
  },
  {
    0x4C2C,   /* KBL */
    TEXT("Kobil Systems GmbH")
  },
  {
    0x6C2C,   /* KCL */
    TEXT("Keycorp Ltd")
  },
  {
    0x852C,   /* KDE */
    TEXT("KDE")
  },
  {
    0x8B2C,   /* KDK */
    TEXT("Kodiak Tech")
  },
  {
    0x8D2C,   /* KDM */
    TEXT("Korea Data Systems Co., Ltd.")
  },
  {
    0x932C,   /* KDS */
    TEXT("KDS USA")
  },
  {
    0xA32C,   /* KEC */
    TEXT("Kyushu Electronics Systems Inc")
  },
  {
    0xAD2C,   /* KEM */
    TEXT("Kontron Embedded Modules GmbH")
  },
  {
    0xB32C,   /* KES */
    TEXT("Kesa Corporation")
  },
  {
    0xB92C,   /* KEY */
    TEXT("Key Tech Inc")
  },
  {
    0xC32C,   /* KFC */
    TEXT("SCD Tech")
  },
  {
    0xD82C,   /* KFX */
    TEXT("Kofax Image Products")
  },
  {
    0x332D,   /* KIS */
    TEXT("KiSS Technology A/S")
  },
  {
    0xA32D,   /* KMC */
    TEXT("Mitsumi Company Ltd")
  },
  {
    0xAC2D,   /* KML */
    TEXT("Kensington Microware Ltd")
  },
  {
    0xC32D,   /* KNC */
    TEXT("Konica corporation")
  },
  {
    0xD82D,   /* KNX */
    TEXT("Nutech Marketing PTL")
  },
  {
    0xE22D,   /* KOB */
    TEXT("Kobil Systems GmbH")
  },
  {
    0xE42D,   /* KOD */
    TEXT("Eastman Kodak Company")
  },
  {
    0xE52D,   /* KOE */
    TEXT("KOLTER ELECTRONIC")
  },
  {
    0xEC2D,   /* KOL */
    TEXT("Kollmorgen Motion Technologies Group")
  },
  {
    0xF72D,   /* KOW */
    TEXT("KOWA Company,LTD. ")
  },
  {
    0x032E,   /* KPC */
    TEXT("King Phoenix Company")
  },
  {
    0x4C2E,   /* KRL */
    TEXT("Krell Industries Inc.")
  },
  {
    0x592E,   /* KRY */
    TEXT("Kroy LLC")
  },
  {
    0x632E,   /* KSC */
    TEXT("Kinetic Systems Corporation")
  },
  {
    0x6C2E,   /* KSL */
    TEXT("Karn Solutions Ltd.")
  },
  {
    0x782E,   /* KSX */
    TEXT("King Tester Corporation")
  },
  {
    0x832E,   /* KTC */
    TEXT("Kingston Tech Corporation")
  },
  {
    0x852E,   /* KTE */
    TEXT("K-Tech")
  },
  {
    0x872E,   /* KTG */
    TEXT("Kayser-Threde GmbH")
  },
  {
    0x892E,   /* KTI */
    TEXT("Konica Technical Inc")
  },
  {
    0x8B2E,   /* KTK */
    TEXT("Key Tronic Corporation")
  },
  {
    0x8E2E,   /* KTN */
    TEXT("Katron Tech Inc")
  },
  {
    0xB22E,   /* KUR */
    TEXT("Kurta Corporation")
  },
  {
    0xC12E,   /* KVA */
    TEXT("Kvaser AB")
  },
  {
    0xE42E,   /* KWD */
    TEXT("Kenwood Corporation")
  },
  {
    0x232F,   /* KYC */
    TEXT("Kyocera Corporation")
  },
  {
    0x252F,   /* KYE */
    TEXT("KYE Syst Corporation")
  },
  {
    0x2B2F,   /* KYK */
    TEXT("Samsung Electronics America Inc")
  },
  {
    0x492F,   /* KZI */
    TEXT("K-Zone International co. Ltd.")
  },
  {
    0x4E2F,   /* KZN */
    TEXT("K-Zone International")
  },
  {
    0x2230,   /* LAB */
    TEXT("ACT Labs Ltd")
  },
  {
    0x2330,   /* LAC */
    TEXT("LaCie")
  },
  {
    0x2630,   /* LAF */
    TEXT("Microline")
  },
  {
    0x2730,   /* LAG */
    TEXT("Laguna Systems")
  },
  {
    0x2E30,   /* LAN */
    TEXT("Sodeman Lancom Inc")
  },
  {
    0x3330,   /* LAS */
    TEXT("LASAT Comm. A/S")
  },
  {
    0x3630,   /* LAV */
    TEXT("Lava Computer MFG Inc")
  },
  {
    0x4F30,   /* LBO */
    TEXT("Lubosoft")
  },
  {
    0x6330,   /* LCC */
    TEXT("LCI")
  },
  {
    0x6430,   /* LCD */
    TEXT("Toshiba Matsushita Display Technology Co., Ltd")
  },
  {
    0x6530,   /* LCE */
    TEXT("La Commande Electronique")
  },
  {
    0x6930,   /* LCI */
    TEXT("Lite-On Communication Inc")
  },
  {
    0x6D30,   /* LCM */
    TEXT("Latitude Comm.")
  },
  {
    0x6E30,   /* LCN */
    TEXT("LEXICON")
  },
  {
    0x7330,   /* LCS */
    TEXT("Longshine Electronics Company")
  },
  {
    0x7430,   /* LCT */
    TEXT("Labcal Technologies")
  },
  {
    0x9430,   /* LDT */
    TEXT("LogiDataTech Electronic GmbH")
  },
  {
    0xA330,   /* LEC */
    TEXT("Lectron Company Ltd")
  },
  {
    0xA430,   /* LED */
    TEXT("Long Engineering Design Inc")
  },
  {
    0xA730,   /* LEG */
    TEXT("Legerity, Inc")
  },
  {
    0xAE30,   /* LEN */
    TEXT("Lenovo Group Limited")
  },
  {
    0xAF30,   /* LEO */
    TEXT("First International Computer Inc")
  },
  {
    0xB830,   /* LEX */
    TEXT("Lexical Ltd")
  },
  {
    0xE330,   /* LGC */
    TEXT("Logic Ltd")
  },
  {
    0xE930,   /* LGI */
    TEXT("Logitech Inc")
  },
  {
    0xF330,   /* LGS */
    TEXT("LG Semicom Company Ltd")
  },
  {
    0xF830,   /* LGX */
    TEXT("Lasergraphics, Inc.")
  },
  {
    0x0131,   /* LHA */
    TEXT("Lars Haagh ApS")
  },
  {
    0x0531,   /* LHE */
    TEXT("Lung Hwa Electronics Company Ltd")
  },
  {
    0x3431,   /* LIT */
    TEXT("Lithics Silicon Technology")
  },
  {
    0x5831,   /* LJX */
    TEXT("Datalogic Corporation")
  },
  {
    0x6D31,   /* LKM */
    TEXT("Likom Technology Sdn. Bhd.")
  },
  {
    0xA731,   /* LMG */
    TEXT("Lucent Technologies")
  },
  {
    0xA931,   /* LMI */
    TEXT("Lexmark Int'l Inc")
  },
  {
    0xB031,   /* LMP */
    TEXT("Leda Media Products")
  },
  {
    0xB431,   /* LMT */
    TEXT("Laser Master")
  },
  {
    0xC431,   /* LND */
    TEXT("Land Computer Company Ltd")
  },
  {
    0xCB31,   /* LNK */
    TEXT("Link Tech Inc")
  },
  {
    0xD231,   /* LNR */
    TEXT("Linear Systems Ltd.")
  },
  {
    0xD431,   /* LNT */
    TEXT("LANETCO International")
  },
  {
    0xD631,   /* LNV */
    TEXT("Lenovo")
  },
  {
    0xE331,   /* LOC */
    TEXT("Locamation B.V.")
  },
  {
    0xE531,   /* LOE */
    TEXT("Loewe Opta GmbH")
  },
  {
    0xE731,   /* LOG */
    TEXT("Logicode Technology Inc")
  },
  {
    0x0532,   /* LPE */
    TEXT("El-PUSK Co., Ltd.")
  },
  {
    0x0932,   /* LPI */
    TEXT("Design Technology")
  },
  {
    0x0C32,   /* LPL */
    TEXT("DO NOT USE - LPL")
  },
  {
    0x6332,   /* LSC */
    TEXT("LifeSize Communications")
  },
  {
    0x6932,   /* LSI */
    TEXT("Loughborough Sound Images")
  },
  {
    0x6A32,   /* LSJ */
    TEXT("LSI Japan Company Ltd")
  },
  {
    0x6C32,   /* LSL */
    TEXT("Logical Solutions")
  },
  {
    0x7932,   /* LSY */
    TEXT("LSI Systems Inc")
  },
  {
    0x8332,   /* LTC */
    TEXT("Labtec Inc")
  },
  {
    0x8932,   /* LTI */
    TEXT("Jongshine Tech Inc")
  },
  {
    0x8B32,   /* LTK */
    TEXT("Lucidity Technology Company Ltd")
  },
  {
    0x8E32,   /* LTN */
    TEXT("Litronic Inc")
  },
  {
    0x9332,   /* LTS */
    TEXT("LTS Scale LLC")
  },
  {
    0x9632,   /* LTV */
    TEXT("Leitch Technology International Inc.")
  },
  {
    0x9732,   /* LTW */
    TEXT("Lightware, Inc")
  },
  {
    0xA332,   /* LUC */
    TEXT("Lucent Technologies")
  },
  {
    0xAD32,   /* LUM */
    TEXT("Lumagen, Inc.")
  },
  {
    0xB832,   /* LUX */
    TEXT("Luxxell Research Inc")
  },
  {
    0xE332,   /* LWC */
    TEXT("Labway Corporation")
  },
  {
    0xF232,   /* LWR */
    TEXT("Lightware Visual Engineering")
  },
  {
    0xF732,   /* LWW */
    TEXT("Lanier Worldwide")
  },
  {
    0x0E33,   /* LXN */
    TEXT("Luxeon")
  },
  {
    0x1333,   /* LXS */
    TEXT("ELEA CardWare")
  },
  {
    0x5833,   /* LZX */
    TEXT("Lightwell Company Ltd")
  },
  {
    0x2334,   /* MAC */
    TEXT("MAC System Company Ltd")
  },
  {
    0x2434,   /* MAD */
    TEXT("Xedia Corporation")
  },
  {
    0x2534,   /* MAE */
    TEXT("Maestro Pty Ltd")
  },
  {
    0x2734,   /* MAG */
    TEXT("MAG InnoVision")
  },
  {
    0x2934,   /* MAI */
    TEXT("Mutoh America Inc")
  },
  {
    0x2C34,   /* MAL */
    TEXT("Meridian Audio Ltd")
  },
  {
    0x2E34,   /* MAN */
    TEXT("LGIC")
  },
  {
    0x3334,   /* MAS */
    TEXT("Mass Inc.")
  },
  {
    0x3434,   /* MAT */
    TEXT("Matsushita Electric Ind. Company Ltd")
  },
  {
    0x3834,   /* MAX */
    TEXT("Rogen Tech Distribution Inc")
  },
  {
    0x3934,   /* MAY */
    TEXT("Maynard Electronics")
  },
  {
    0x3A34,   /* MAZ */
    TEXT("MAZeT GmbH")
  },
  {
    0x4334,   /* MBC */
    TEXT("MBC")
  },
  {
    0x4434,   /* MBD */
    TEXT("Microbus PLC")
  },
  {
    0x4D34,   /* MBM */
    TEXT("Marshall Electronics")
  },
  {
    0x5634,   /* MBV */
    TEXT("Moreton Bay")
  },
  {
    0x6134,   /* MCA */
    TEXT("American Nuclear Systems Inc")
  },
  {
    0x6334,   /* MCC */
    TEXT("Micro Industries")
  },
  {
    0x6434,   /* MCD */
    TEXT("McDATA Corporation")
  },
  {
    0x6534,   /* MCE */
    TEXT("Metz-Werke GmbH & Co KG")
  },
  {
    0x6734,   /* MCG */
    TEXT("Motorola Computer Group")
  },
  {
    0x6934,   /* MCI */
    TEXT("Micronics Computers")
  },
  {
    0x6C34,   /* MCL */
    TEXT("Motorola Communications Israel")
  },
  {
    0x6D34,   /* MCM */
    TEXT("Metricom Inc")
  },
  {
    0x6E34,   /* MCN */
    TEXT("Micron Electronics Inc")
  },
  {
    0x6F34,   /* MCO */
    TEXT("Motion Computing Inc.")
  },
  {
    0x7034,   /* MCP */
    TEXT("Magni Systems Inc")
  },
  {
    0x7134,   /* MCQ */
    TEXT("Mat's Computers")
  },
  {
    0x7234,   /* MCR */
    TEXT("Marina Communicaitons")
  },
  {
    0x7334,   /* MCS */
    TEXT("Micro Computer Systems")
  },
  {
    0x7434,   /* MCT */
    TEXT("Microtec")
  },
  {
    0x8134,   /* MDA */
    TEXT("Media4 Inc")
  },
  {
    0x8334,   /* MDC */
    TEXT("Midori Electronics")
  },
  {
    0x8434,   /* MDD */
    TEXT("MODIS")
  },
  {
    0x8734,   /* MDG */
    TEXT("Madge Networks")
  },
  {
    0x8934,   /* MDI */
    TEXT("Micro Design Inc")
  },
  {
    0x8B34,   /* MDK */
    TEXT("Mediatek Corporation")
  },
  {
    0x8F34,   /* MDO */
    TEXT("Panasonic")
  },
  {
    0x9234,   /* MDR */
    TEXT("Medar Inc")
  },
  {
    0x9334,   /* MDS */
    TEXT("Micro Display Systems Inc")
  },
  {
    0x9434,   /* MDT */
    TEXT("Magus Data Tech")
  },
  {
    0x9634,   /* MDV */
    TEXT("MET Development Inc")
  },
  {
    0x9834,   /* MDX */
    TEXT("MicroDatec GmbH")
  },
  {
    0x9934,   /* MDY */
    TEXT("Microdyne Inc")
  },
  {
    0xA334,   /* MEC */
    TEXT("Mega System Technologies Inc")
  },
  {
    0xA434,   /* MED */
    TEXT("Messeltronik Dresden GmbH")
  },
  {
    0xA534,   /* MEE */
    TEXT("Mitsubishi Electric Engineering Co., Ltd.")
  },
  {
    0xA734,   /* MEG */
    TEXT("Abeam Tech Ltd")
  },
  {
    0xA934,   /* MEI */
    TEXT("Panasonic Industry Company")
  },
  {
    0xAC34,   /* MEL */
    TEXT("Mitsubishi Electric Corporation")
  },
  {
    0xAE34,   /* MEN */
    TEXT("MEN Mikroelectronik Nueruberg GmbH")
  },
  {
    0xB134,   /* MEQ */
    TEXT("Matelect Ltd.")
  },
  {
    0xB434,   /* MET */
    TEXT("Metheus Corporation")
  },
  {
    0xC734,   /* MFG */
    TEXT("MicroField Graphics Inc")
  },
  {
    0xC934,   /* MFI */
    TEXT("Micro Firmware")
  },
  {
    0xD234,   /* MFR */
    TEXT("MediaFire Corp.")
  },
  {
    0xE134,   /* MGA */
    TEXT("Mega System Technologies, Inc.")
  },
  {
    0xE534,   /* MGE */
    TEXT("Schneider Electric S.A.")
  },
  {
    0xEC34,   /* MGL */
    TEXT("M-G Technology Ltd")
  },
  {
    0xF434,   /* MGT */
    TEXT("Megatech R & D Company")
  },
  {
    0x2335,   /* MIC */
    TEXT("Micom Communications Inc")
  },
  {
    0x2435,   /* MID */
    TEXT("miro Displays")
  },
  {
    0x2935,   /* MII */
    TEXT("Mitec Inc")
  },
  {
    0x2C35,   /* MIL */
    TEXT("Marconi Instruments Ltd")
  },
  {
    0x3035,   /* MIP */
    TEXT("micronpc.com")
  },
  {
    0x3235,   /* MIR */
    TEXT("Miro Computer Prod.")
  },
  {
    0x3335,   /* MIS */
    TEXT("Modular Industrial Solutions Inc")
  },
  {
    0x3435,   /* MIT */
    TEXT("MCM Industrial Technology GmbH")
  },
  {
    0x4935,   /* MJI */
    TEXT("MARANTZ JAPAN, INC.")
  },
  {
    0x5335,   /* MJS */
    TEXT("MJS Designs")
  },
  {
    0x6335,   /* MKC */
    TEXT("Media Tek Inc.")
  },
  {
    0x7435,   /* MKT */
    TEXT("MICROTEK Inc.")
  },
  {
    0x7635,   /* MKV */
    TEXT("Trtheim Technology")
  },
  {
    0x8435,   /* MLD */
    TEXT("Deep Video Imaging Ltd")
  },
  {
    0x8735,   /* MLG */
    TEXT("Micrologica AG")
  },
  {
    0x8935,   /* MLI */
    TEXT("McIntosh Laboratory Inc.")
  },
  {
    0x8D35,   /* MLM */
    TEXT("Millennium Engineering Inc")
  },
  {
    0x8E35,   /* MLN */
    TEXT("Mark Levinson")
  },
  {
    0x9335,   /* MLS */
    TEXT("Milestone EPE")
  },
  {
    0x9835,   /* MLX */
    TEXT("Mylex Corporation")
  },
  {
    0xA135,   /* MMA */
    TEXT("Micromedia AG")
  },
  {
    0xA435,   /* MMD */
    TEXT("Micromed Biotecnologia Ltd")
  },
  {
    0xA635,   /* MMF */
    TEXT("Minnesota Mining and Manufacturing")
  },
  {
    0xA935,   /* MMI */
    TEXT("Multimax")
  },
  {
    0xAD35,   /* MMM */
    TEXT("Electronic Measurements")
  },
  {
    0xAE35,   /* MMN */
    TEXT("MiniMan Inc")
  },
  {
    0xB335,   /* MMS */
    TEXT("MMS Electronics")
  },
  {
    0xC335,   /* MNC */
    TEXT("Mini Micro Methods Ltd")
  },
  {
    0xCC35,   /* MNL */
    TEXT("Monorail Inc")
  },
  {
    0xD035,   /* MNP */
    TEXT("Microcom")
  },
  {
    0xE435,   /* MOD */
    TEXT("Modular Technology")
  },
  {
    0xED35,   /* MOM */
    TEXT("Momentum Data Systems")
  },
  {
    0xF335,   /* MOS */
    TEXT("Moses Corporation")
  },
  {
    0xF435,   /* MOT */
    TEXT("Motorola UDS")
  },
  {
    0x0336,   /* MPC */
    TEXT("M-Pact Inc")
  },
  {
    0x0936,   /* MPI */
    TEXT("Mediatrix Peripherals Inc")
  },
  {
    0x0A36,   /* MPJ */
    TEXT("Microlab")
  },
  {
    0x0C36,   /* MPL */
    TEXT("Maple Research Inst. Company Ltd")
  },
  {
    0x0E36,   /* MPN */
    TEXT("Mainpine Limited")
  },
  {
    0x1336,   /* MPS */
    TEXT("mps Software GmbH")
  },
  {
    0x1836,   /* MPX */
    TEXT("Micropix Technologies, Ltd.")
  },
  {
    0x3036,   /* MQP */
    TEXT("MultiQ Products AB")
  },
  {
    0x4136,   /* MRA */
    TEXT("Miranda Technologies Inc")
  },
  {
    0x4336,   /* MRC */
    TEXT("Marconi Simulation & Ty-Coch Way Training")
  },
  {
    0x4436,   /* MRD */
    TEXT("MicroDisplay Corporation")
  },
  {
    0x4B36,   /* MRK */
    TEXT("Maruko & Company Ltd")
  },
  {
    0x4C36,   /* MRL */
    TEXT("Miratel")
  },
  {
    0x4F36,   /* MRO */
    TEXT("Medikro Oy")
  },
  {
    0x5436,   /* MRT */
    TEXT("Merging Technologies")
  },
  {
    0x6136,   /* MSA */
    TEXT("Micro Systemation AB")
  },
  {
    0x6336,   /* MSC */
    TEXT("Mouse Systems Corporation")
  },
  {
    0x6436,   /* MSD */
    TEXT("Datenerfassungs- und Informationssysteme")
  },
  {
    0x6636,   /* MSF */
    TEXT("M-Systems Flash Disk Pioneers")
  },
  {
    0x6736,   /* MSG */
    TEXT("MSI GmbH")
  },
  {
    0x6836,   /* MSH */
    TEXT("Microsoft")
  },
  {
    0x6936,   /* MSI */
    TEXT("Microstep")
  },
  {
    0x6B36,   /* MSK */
    TEXT("Megasoft Inc")
  },
  {
    0x6C36,   /* MSL */
    TEXT("MicroSlate Inc.")
  },
  {
    0x6D36,   /* MSM */
    TEXT("Advanced Digital Systems")
  },
  {
    0x7036,   /* MSP */
    TEXT("Mistral Solutions [P] Ltd.")
  },
  {
    0x7436,   /* MST */
    TEXT("MS Telematica")
  },
  {
    0x7536,   /* MSU */
    TEXT("motorola")
  },
  {
    0x7636,   /* MSV */
    TEXT("Mosgi Corporation")
  },
  {
    0x7836,   /* MSX */
    TEXT("Micomsoft Co., Ltd.")
  },
  {
    0x7936,   /* MSY */
    TEXT("MicroTouch Systems Inc")
  },
  {
    0x8236,   /* MTB */
    TEXT("Media Technologies Ltd.")
  },
  {
    0x8336,   /* MTC */
    TEXT("Mars-Tech Corporation")
  },
  {
    0x8436,   /* MTD */
    TEXT("MindTech Display Co. Ltd")
  },
  {
    0x8536,   /* MTE */
    TEXT("MediaTec GmbH")
  },
  {
    0x8836,   /* MTH */
    TEXT("Micro-Tech Hearing Instruments")
  },
  {
    0x8936,   /* MTI */
    TEXT("MaxCom Technical Inc")
  },
  {
    0x8936,   /* MTI */
    TEXT("MaxCom Technical Inc")
  },
  {
    0x8B36,   /* MTK */
    TEXT("Microtek International Inc.")
  },
  {
    0x8C36,   /* MTL */
    TEXT("Mitel Corporation")
  },
  {
    0x8E36,   /* MTN */
    TEXT("Mtron Storage Technology Co., Ltd.")
  },
  {
    0x9236,   /* MTR */
    TEXT("Mitron computer Inc")
  },
  {
    0x9336,   /* MTS */
    TEXT("Multi-Tech Systems")
  },
  {
    0x9536,   /* MTU */
    TEXT("Mark of the Unicorn Inc")
  },
  {
    0x9836,   /* MTX */
    TEXT("Matrox")
  },
  {
    0xA436,   /* MUD */
    TEXT("Multi-Dimension Institute")
  },
  {
    0xAB36,   /* MUK */
    TEXT("mainpine limited")
  },
  {
    0xC436,   /* MVD */
    TEXT("Microvitec PLC")
  },
  {
    0xC936,   /* MVI */
    TEXT("Media Vision Inc")
  },
  {
    0xCD36,   /* MVM */
    TEXT("SOBO VISION")
  },
  {
    0xD336,   /* MVS */
    TEXT("Microvision")
  },
  {
    0xD836,   /* MVX */
    TEXT("COM 1")
  },
  {
    0xE936,   /* MWI */
    TEXT("Multiwave Innovation Pte Ltd")
  },
  {
    0xF236,   /* MWR */
    TEXT("mware")
  },
  {
    0xF936,   /* MWY */
    TEXT("Microway Inc")
  },
  {
    0x0437,   /* MXD */
    TEXT("MaxData Computer GmbH & Co.KG")
  },
  {
    0x0937,   /* MXI */
    TEXT("Macronix Inc")
  },
  {
    0x0C37,   /* MXL */
    TEXT("Hitachi Maxell, Ltd.")
  },
  {
    0x1037,   /* MXP */
    TEXT("Maxpeed Corporation")
  },
  {
    0x1437,   /* MXT */
    TEXT("Maxtech Corporation")
  },
  {
    0x1637,   /* MXV */
    TEXT("MaxVision Corporation")
  },
  {
    0x2137,   /* MYA */
    TEXT("Monydata")
  },
  {
    0x3237,   /* MYR */
    TEXT("Myriad Solutions Ltd")
  },
  {
    0x3837,   /* MYX */
    TEXT("Micronyx Inc")
  },
  {
    0x2338,   /* NAC */
    TEXT("Ncast Corporation")
  },
  {
    0x2438,   /* NAD */
    TEXT("NAD Electronics")
  },
  {
    0x2C38,   /* NAL */
    TEXT("Network Alchemy")
  },
  {
    0x3638,   /* NAV */
    TEXT("Navigation Corporation")
  },
  {
    0x3838,   /* NAX */
    TEXT("Naxos Tecnologia")
  },
  {
    0x4C38,   /* NBL */
    TEXT("N*Able Technologies Inc")
  },
  {
    0x5338,   /* NBS */
    TEXT("National Key Lab. on ISN")
  },
  {
    0x5438,   /* NBT */
    TEXT("NingBo Bestwinning Technology CO., Ltd")
  },
  {
    0x6138,   /* NCA */
    TEXT("Nixdorf Company")
  },
  {
    0x6338,   /* NCC */
    TEXT("NCR Corporation")
  },
  {
    0x6538,   /* NCE */
    TEXT("Norcent Technology, Inc.")
  },
  {
    0x6938,   /* NCI */
    TEXT("NewCom Inc")
  },
  {
    0x6C38,   /* NCL */
    TEXT("NetComm Ltd")
  },
  {
    0x7238,   /* NCR */
    TEXT("NCR Electronics")
  },
  {
    0x7338,   /* NCS */
    TEXT("Northgate Computer Systems")
  },
  {
    0x7438,   /* NCT */
    TEXT("NEC CustomTechnica, Ltd.")
  },
  {
    0x8338,   /* NDC */
    TEXT("National DataComm Corporaiton")
  },
  {
    0x8938,   /* NDI */
    TEXT("National Display Systems")
  },
  {
    0x8B38,   /* NDK */
    TEXT("Naitoh Densei CO., LTD.")
  },
  {
    0x8C38,   /* NDL */
    TEXT("Network Designers")
  },
  {
    0x9338,   /* NDS */
    TEXT("Nokia Data")
  },
  {
    0xA338,   /* NEC */
    TEXT("NEC Corporation")
  },
  {
    0xAF38,   /* NEO */
    TEXT("NEO TELECOM CO.,LTD.")
  },
  {
    0xB438,   /* NET */
    TEXT("Mettler Toledo")
  },
  {
    0xB538,   /* NEU */
    TEXT("NEUROTEC - EMPRESA DE PESQUISA E DESENVOLVIMENTO EM BIOMEDICINA")
  },
  {
    0xB838,   /* NEX */
    TEXT("Nexgen Mediatech Inc.,")
  },
  {
    0xC338,   /* NFC */
    TEXT("BTC Korea Co., Ltd")
  },
  {
    0xD338,   /* NFS */
    TEXT("Number Five Software")
  },
  {
    0xE338,   /* NGC */
    TEXT("Network General")
  },
  {
    0xF338,   /* NGS */
    TEXT("A D S Exports")
  },
  {
    0x1439,   /* NHT */
    TEXT("Vinci Labs")
  },
  {
    0x2339,   /* NIC */
    TEXT("National Instruments Corporation")
  },
  {
    0x3339,   /* NIS */
    TEXT("Nissei Electric Company")
  },
  {
    0x3439,   /* NIT */
    TEXT("Network Info Technology")
  },
  {
    0x3839,   /* NIX */
    TEXT("Seanix Technology Inc")
  },
  {
    0x8339,   /* NLC */
    TEXT("Next Level Communications")
  },
  {
    0xB039,   /* NMP */
    TEXT("Nokia Mobile Phones")
  },
  {
    0xB339,   /* NMS */
    TEXT("Natural Micro System")
  },
  {
    0xB639,   /* NMV */
    TEXT("NEC-Mitsubishi Electric Visual Systems Corporation")
  },
  {
    0xB839,   /* NMX */
    TEXT("Neomagic")
  },
  {
    0xC339,   /* NNC */
    TEXT("NNC")
  },
  {
    0xEB39,   /* NOK */
    TEXT("Nokia Display Products")
  },
  {
    0xF239,   /* NOR */
    TEXT("Norand Corporation")
  },
  {
    0xF439,   /* NOT */
    TEXT("Not Limited Inc")
  },
  {
    0x093A,   /* NPI */
    TEXT("Network Peripherals Inc")
  },
  {
    0x4C3A,   /* NRL */
    TEXT("U.S. Naval Research Lab")
  },
  {
    0x543A,   /* NRT */
    TEXT("Beijing Northern Radiantelecom Co.")
  },
  {
    0x563A,   /* NRV */
    TEXT("Taugagreining hf")
  },
  {
    0x633A,   /* NSC */
    TEXT("National Semiconductor Corporation")
  },
  {
    0x693A,   /* NSI */
    TEXT("NISSEI ELECTRIC CO.,LTD")
  },
  {
    0x703A,   /* NSP */
    TEXT("Nspire System Inc.")
  },
  {
    0x733A,   /* NSS */
    TEXT("Newport Systems Solutions")
  },
  {
    0x743A,   /* NST */
    TEXT("Network Security Technology Co")
  },
  {
    0x833A,   /* NTC */
    TEXT("NeoTech S.R.L")
  },
  {
    0x893A,   /* NTI */
    TEXT("New Tech Int'l Company")
  },
  {
    0x8C3A,   /* NTL */
    TEXT("National Transcomm. Ltd")
  },
  {
    0x8E3A,   /* NTN */
    TEXT("Nuvoton Technology Corporation ")
  },
  {
    0x923A,   /* NTR */
    TEXT("N-trig Innovative Technologies, Inc.")
  },
  {
    0x933A,   /* NTS */
    TEXT("Nits Technology Inc.")
  },
  {
    0x943A,   /* NTT */
    TEXT("NTT Advanced Technology Corporation")
  },
  {
    0x973A,   /* NTW */
    TEXT("Networth Inc")
  },
  {
    0x983A,   /* NTX */
    TEXT("Netaccess Inc")
  },
  {
    0xA73A,   /* NUG */
    TEXT("NU Technology, Inc.")
  },
  {
    0xA93A,   /* NUI */
    TEXT("NU Inc.")
  },
  {
    0xC33A,   /* NVC */
    TEXT("NetVision Corporation")
  },
  {
    0xC43A,   /* NVD */
    TEXT("Nvidia")
  },
  {
    0xC93A,   /* NVI */
    TEXT("NuVision US, Inc.")
  },
  {
    0xCC3A,   /* NVL */
    TEXT("Novell Inc")
  },
  {
    0xD43A,   /* NVT */
    TEXT("Navatek Engineering Corporation")
  },
  {
    0xE33A,   /* NWC */
    TEXT("NW Computer Engineering")
  },
  {
    0xF03A,   /* NWP */
    TEXT("NovaWeb Technologies Inc")
  },
  {
    0xF33A,   /* NWS */
    TEXT("Newisys, Inc.")
  },
  {
    0x033B,   /* NXC */
    TEXT("NextCom K.K.")
  },
  {
    0x073B,   /* NXG */
    TEXT("Nexgen")
  },
  {
    0x103B,   /* NXP */
    TEXT("NXP Semiconductors bv.")
  },
  {
    0x113B,   /* NXQ */
    TEXT("Nexiq Technologies, Inc.")
  },
  {
    0x133B,   /* NXS */
    TEXT("Technology Nexus Secure Open Systems AB")
  },
  {
    0x233B,   /* NYC */
    TEXT("nakayo telecommunications,inc.")
  },
  {
    0x2B3C,   /* OAK */
    TEXT("Oak Tech Inc")
  },
  {
    0x333C,   /* OAS */
    TEXT("Oasys Technology Company")
  },
  {
    0x643C,   /* OCD */
    TEXT("Macraigor Systems Inc")
  },
  {
    0x6E3C,   /* OCN */
    TEXT("Olfan")
  },
  {
    0x733C,   /* OCS */
    TEXT("Open Connect Solutions")
  },
  {
    0x8D3C,   /* ODM */
    TEXT("ODME Inc.")
  },
  {
    0x923C,   /* ODR */
    TEXT("Odrac")
  },
  {
    0xA33C,   /* OEC */
    TEXT("ORION ELECTRIC CO.,LTD")
  },
  {
    0x233D,   /* OIC */
    TEXT("Option Industrial Computers")
  },
  {
    0x2D3D,   /* OIM */
    TEXT("Option International")
  },
  {
    0x2E3D,   /* OIN */
    TEXT("Option International")
  },
  {
    0x693D,   /* OKI */
    TEXT("OKI Electric Industrial Company Ltd")
  },
  {
    0x833D,   /* OLC */
    TEXT("Olicom A/S")
  },
  {
    0x843D,   /* OLD */
    TEXT("Olidata S.p.A.")
  },
  {
    0x893D,   /* OLI */
    TEXT("Olivetti")
  },
  {
    0x943D,   /* OLT */
    TEXT("Olitec S.A.")
  },
  {
    0x963D,   /* OLV */
    TEXT("Olitec S.A.")
  },
  {
    0x993D,   /* OLY */
    TEXT("OLYMPUS CORPORATION")
  },
  {
    0xA33D,   /* OMC */
    TEXT("OBJIX Multimedia Corporation")
  },
  {
    0xAE3D,   /* OMN */
    TEXT("Omnitel")
  },
  {
    0xB23D,   /* OMR */
    TEXT("Omron Corporation")
  },
  {
    0xC53D,   /* ONE */
    TEXT("Oneac Corporation")
  },
  {
    0xCB3D,   /* ONK */
    TEXT("ONKYO Corporation")
  },
  {
    0xD33D,   /* ONS */
    TEXT("On Systems Inc")
  },
  {
    0xD73D,   /* ONW */
    TEXT("OPEN Networks Ltd")
  },
  {
    0xD83D,   /* ONX */
    TEXT("SOMELEC Z.I. Du Vert Galanta")
  },
  {
    0xF33D,   /* OOS */
    TEXT("OSRAM")
  },
  {
    0x033E,   /* OPC */
    TEXT("Opcode Inc")
  },
  {
    0x093E,   /* OPI */
    TEXT("D.N.S. Corporation")
  },
  {
    0x143E,   /* OPT */
    TEXT("OPTi Inc")
  },
  {
    0x163E,   /* OPV */
    TEXT("Optivision Inc")
  },
  {
    0x293E,   /* OQI */
    TEXT("Oksori Company Ltd")
  },
  {
    0x473E,   /* ORG */
    TEXT("ORGA Kartensysteme GmbH")
  },
  {
    0x493E,   /* ORI */
    TEXT("OSR Open Systems Resources, Inc.")
  },
  {
    0x4E3E,   /* ORN */
    TEXT("ORION ELECTRIC CO., LTD.")
  },
  {
    0x613E,   /* OSA */
    TEXT("OSAKA Micro Computer, Inc.")
  },
  {
    0x703E,   /* OSP */
    TEXT("OPTI-UPS Corporation")
  },
  {
    0x723E,   /* OSR */
    TEXT("Oksori Company Ltd")
  },
  {
    0x893E,   /* OTI */
    TEXT("Orchid Technology")
  },
  {
    0x943E,   /* OTT */
    TEXT("OPTO22, Inc.")
  },
  {
    0xAB3E,   /* OUK */
    TEXT("OUK Company Ltd")
  },
  {
    0xEC3E,   /* OWL */
    TEXT("Mediacom Technologies Pte Ltd")
  },
  {
    0x153F,   /* OXU */
    TEXT("Oxus Research S.A.")
  },
  {
    0x2F3F,   /* OYO */
    TEXT("Shadow Systems")
  },
  {
    0x4F3F,   /* OZO */
    TEXT("Tribe Computer Works Inc")
  },
  {
    0x2340,   /* PAC */
    TEXT("Pacific Avionics Corporation")
  },
  {
    0x2440,   /* PAD */
    TEXT("Promotion and Display Technology Ltd.")
  },
  {
    0x2B40,   /* PAK */
    TEXT("Many CNC System Co., Ltd.")
  },
  {
    0x2D40,   /* PAM */
    TEXT("Peter Antesberger Messtechnik")
  },
  {
    0x2E40,   /* PAN */
    TEXT("The Panda Project")
  },
  {
    0x3240,   /* PAR */
    TEXT("Parallan Comp Inc")
  },
  {
    0x4940,   /* PBI */
    TEXT("Pitney Bowes")
  },
  {
    0x4C40,   /* PBL */
    TEXT("Packard Bell Electronics")
  },
  {
    0x4E40,   /* PBN */
    TEXT("Packard Bell NEC")
  },
  {
    0x5640,   /* PBV */
    TEXT("Pitney Bowes")
  },
  {
    0x6140,   /* PCA */
    TEXT("Philips BU Add On Card")
  },
  {
    0x6240,   /* PCB */
    TEXT("OCTAL S.A.")
  },
  {
    0x6340,   /* PCC */
    TEXT("PowerCom Technology Company Ltd")
  },
  {
    0x6740,   /* PCG */
    TEXT("First Industrial Computer Inc")
  },
  {
    0x6940,   /* PCI */
    TEXT("Pioneer Computer Inc")
  },
  {
    0x6B40,   /* PCK */
    TEXT("PCBANK21")
  },
  {
    0x6C40,   /* PCL */
    TEXT("pentel.co.,ltd")
  },
  {
    0x6D40,   /* PCM */
    TEXT("PCM Systems Corporation")
  },
  {
    0x6F40,   /* PCO */
    TEXT("Performance Concepts Inc.,")
  },
  {
    0x7040,   /* PCP */
    TEXT("Procomp USA Inc")
  },
  {
    0x7440,   /* PCT */
    TEXT("PC-Tel Inc")
  },
  {
    0x7740,   /* PCW */
    TEXT("Pacific CommWare Inc")
  },
  {
    0x7840,   /* PCX */
    TEXT("PC Xperten")
  },
  {
    0x8D40,   /* PDM */
    TEXT("Psion Dacom Plc.")
  },
  {
    0x8E40,   /* PDN */
    TEXT("AT&T Paradyne")
  },
  {
    0x9240,   /* PDR */
    TEXT("Pure Data Inc")
  },
  {
    0x9340,   /* PDS */
    TEXT("PD Systems International Ltd")
  },
  {
    0x9440,   /* PDT */
    TEXT("PDTS - Prozessdatentechnik und Systeme")
  },
  {
    0x9640,   /* PDV */
    TEXT("Prodrive B.V.")
  },
  {
    0xA340,   /* PEC */
    TEXT("POTRANS Electrical Corp.")
  },
  {
    0xA940,   /* PEI */
    TEXT("PEI Electronics Inc")
  },
  {
    0xAC40,   /* PEL */
    TEXT("Primax Electric Ltd")
  },
  {
    0xAE40,   /* PEN */
    TEXT("Interactive Computer Products Inc")
  },
  {
    0xB040,   /* PEP */
    TEXT("Peppercon AG")
  },
  {
    0xB240,   /* PER */
    TEXT("Perceptive Signal Technologies")
  },
  {
    0xB440,   /* PET */
    TEXT("Practical Electronic Tools")
  },
  {
    0xD440,   /* PFT */
    TEXT("Telia ProSoft AB")
  },
  {
    0xED40,   /* PGM */
    TEXT("Paradigm Advanced Research Centre")
  },
  {
    0xF040,   /* PGP */
    TEXT("propagamma kommunikation")
  },
  {
    0xF340,   /* PGS */
    TEXT("Princeton Graphic Systems")
  },
  {
    0x0341,   /* PHC */
    TEXT("Pijnenburg Beheer N.V.")
  },
  {
    0x0941,   /* PHI */
    TEXT("DO NOT USE - PHI")
  },
  {
    0x0C41,   /* PHL */
    TEXT("Philips Consumer Electronics Company")
  },
  {
    0x0F41,   /* PHO */
    TEXT("Photonics Systems Inc.")
  },
  {
    0x1341,   /* PHS */
    TEXT("Philips Communication Systems")
  },
  {
    0x1941,   /* PHY */
    TEXT("Phylon Communications")
  },
  {
    0x2541,   /* PIE */
    TEXT("Pacific Image Electronics Company Ltd")
  },
  {
    0x2D41,   /* PIM */
    TEXT("Prism, LLC")
  },
  {
    0x2F41,   /* PIO */
    TEXT("Pioneer Electronic Corporation")
  },
  {
    0x3841,   /* PIX */
    TEXT("Pixie Tech Inc")
  },
  {
    0x4141,   /* PJA */
    TEXT("Projecta")
  },
  {
    0x4441,   /* PJD */
    TEXT("Projectiondesign AS")
  },
  {
    0x5441,   /* PJT */
    TEXT("Pan Jit International Inc.")
  },
  {
    0x6141,   /* PKA */
    TEXT("Acco UK ltd. ")
  },
  {
    0x8341,   /* PLC */
    TEXT("Pro-Log Corporation")
  },
  {
    0x8D41,   /* PLM */
    TEXT("PROLINK Microsystems Corp.")
  },
  {
    0x9641,   /* PLV */
    TEXT("PLUS Vision Corp.")
  },
  {
    0x9841,   /* PLX */
    TEXT("Parallax Graphics")
  },
  {
    0x9941,   /* PLY */
    TEXT("Polycom Inc.")
  },
  {
    0xA341,   /* PMC */
    TEXT("PMC Consumer Electronics Ltd")
  },
  {
    0xA441,   /* PMD */
    TEXT("TDK USA Corporation")
  },
  {
    0xAD41,   /* PMM */
    TEXT("Point Multimedia System")
  },
  {
    0xB441,   /* PMT */
    TEXT("Promate Electronic Co., Ltd.")
  },
  {
    0xB841,   /* PMX */
    TEXT("Photomatrix")
  },
  {
    0xC741,   /* PNG */
    TEXT("Microsoft")
  },
  {
    0xC741,   /* PNG */
    TEXT("Microsoft")
  },
  {
    0xCC41,   /* PNL */
    TEXT("Panelview, Inc.")
  },
  {
    0xD041,   /* PNP */
    TEXT("Microsoft")
  },
  {
    0xD241,   /* PNR */
    TEXT("Planar Systems, Inc.")
  },
  {
    0xD341,   /* PNS */
    TEXT("PanaScope")
  },
  {
    0xD841,   /* PNX */
    TEXT("Phoenix Technologies, Ltd.")
  },
  {
    0xEC41,   /* POL */
    TEXT("PolyComp (PTY) Ltd.")
  },
  {
    0xEE41,   /* PON */
    TEXT("Perpetual Technologies, LLC")
  },
  {
    0xF241,   /* POR */
    TEXT("Portalis LC")
  },
  {
    0x0342,   /* PPC */
    TEXT("Phoenixtec Power Company Ltd")
  },
  {
    0x0442,   /* PPD */
    TEXT("MEPhI")
  },
  {
    0x0942,   /* PPI */
    TEXT("Practical Peripherals")
  },
  {
    0x0D42,   /* PPM */
    TEXT("Clinton Electronics Corp.")
  },
  {
    0x1042,   /* PPP */
    TEXT("Purup Prepress AS")
  },
  {
    0x1242,   /* PPR */
    TEXT("PicPro")
  },
  {
    0x4142,   /* PRA */
    TEXT("PRO/AUTOMATION")
  },
  {
    0x4342,   /* PRC */
    TEXT("PerComm")
  },
  {
    0x4442,   /* PRD */
    TEXT("Praim S.R.L.")
  },
  {
    0x4642,   /* PRF */
    TEXT("Digital Electronics Corporation")
  },
  {
    0x4742,   /* PRG */
    TEXT("The Phoenix Research Group Inc")
  },
  {
    0x4942,   /* PRI */
    TEXT("Priva Hortimation BV")
  },
  {
    0x4D42,   /* PRM */
    TEXT("Prometheus")
  },
  {
    0x4F42,   /* PRO */
    TEXT("Proteon")
  },
  {
    0x5342,   /* PRS */
    TEXT("Leutron Vision")
  },
  {
    0x5842,   /* PRX */
    TEXT("Proxima Corporation")
  },
  {
    0x6142,   /* PSA */
    TEXT("Advanced Signal Processing Technologies")
  },
  {
    0x6342,   /* PSC */
    TEXT("Philips Semiconductors")
  },
  {
    0x6442,   /* PSD */
    TEXT("Peus-Systems GmbH")
  },
  {
    0x6542,   /* PSE */
    TEXT("Practical Solutions Pte., Ltd.")
  },
  {
    0x6942,   /* PSI */
    TEXT("PSI-Perceptive Solutions Inc")
  },
  {
    0x6C42,   /* PSL */
    TEXT("Perle Systems Limited")
  },
  {
    0x6D42,   /* PSM */
    TEXT("Prosum")
  },
  {
    0x7442,   /* PST */
    TEXT("Global Data SA")
  },
  {
    0x8342,   /* PTC */
    TEXT("PS Technology Corporation")
  },
  {
    0x8742,   /* PTG */
    TEXT("Cipher Systems Inc")
  },
  {
    0x8842,   /* PTH */
    TEXT("Pathlight Technology Inc")
  },
  {
    0x8942,   /* PTI */
    TEXT("Promise Technology Inc")
  },
  {
    0x8C42,   /* PTL */
    TEXT("Pantel Inc")
  },
  {
    0x9342,   /* PTS */
    TEXT("Plain Tree Systems Inc")
  },
  {
    0xC742,   /* PVG */
    TEXT("Proview Global Co., Ltd")
  },
  {
    0xCE42,   /* PVN */
    TEXT("Pixel Vision")
  },
  {
    0xD042,   /* PVP */
    TEXT("Klos Technologies, Inc.")
  },
  {
    0x0343,   /* PXC */
    TEXT("Phoenix Contact")
  },
  {
    0x0543,   /* PXE */
    TEXT("PIXELA CORPORATION")
  },
  {
    0x0C43,   /* PXL */
    TEXT("The Moving Pixel Company")
  },
  {
    0x0D43,   /* PXM */
    TEXT("Proxim Inc")
  },
  {
    0x6344,   /* QCC */
    TEXT("QuakeCom Company Ltd")
  },
  {
    0x6844,   /* QCH */
    TEXT("Metronics Inc")
  },
  {
    0x6944,   /* QCI */
    TEXT("Quanta Computer Inc")
  },
  {
    0x6B44,   /* QCK */
    TEXT("Quick Corporation")
  },
  {
    0x6C44,   /* QCL */
    TEXT("Quadrant Components Inc")
  },
  {
    0x7044,   /* QCP */
    TEXT("Qualcomm Inc")
  },
  {
    0x8944,   /* QDI */
    TEXT("Quantum Data Incorporated")
  },
  {
    0x8D44,   /* QDM */
    TEXT("Quadram")
  },
  {
    0x9344,   /* QDS */
    TEXT("Quanta Display Inc.")
  },
  {
    0xC644,   /* QFF */
    TEXT("Padix Co., Inc.")
  },
  {
    0xC944,   /* QFI */
    TEXT("Quickflex, Inc")
  },
  {
    0x8345,   /* QLC */
    TEXT("Q-Logic")
  },
  {
    0x3146,   /* QQQ */
    TEXT("Chuomusen Co., Ltd. ")
  },
  {
    0x6946,   /* QSI */
    TEXT("Quantum Solutions, Inc.")
  },
  {
    0x8446,   /* QTD */
    TEXT("Quantum 3D Inc")
  },
  {
    0x8846,   /* QTH */
    TEXT("Questech Ltd")
  },
  {
    0x8946,   /* QTI */
    TEXT("Quicknet Technologies Inc")
  },
  {
    0x8D46,   /* QTM */
    TEXT("Quantum")
  },
  {
    0x9246,   /* QTR */
    TEXT("Qtronix Corporation")
  },
  {
    0xA146,   /* QUA */
    TEXT("Quatographic AG")
  },
  {
    0xA546,   /* QUE */
    TEXT("Questra Consulting")
  },
  {
    0x2348,   /* RAC */
    TEXT("Racore Computer Products Inc")
  },
  {
    0x2448,   /* RAD */
    TEXT("Radisys Corporation")
  },
  {
    0x2948,   /* RAI */
    TEXT("Rockwell Automation/Intecolor")
  },
  {
    0x2E48,   /* RAN */
    TEXT("Rancho Tech Inc")
  },
  {
    0x3248,   /* RAR */
    TEXT("Raritan, Inc.")
  },
  {
    0x3348,   /* RAS */
    TEXT("RAScom Inc")
  },
  {
    0x3448,   /* RAT */
    TEXT("Rent-A-Tech")
  },
  {
    0x3948,   /* RAY */
    TEXT("Raylar Design, Inc.")
  },
  {
    0x6548,   /* RCE */
    TEXT("Parc d'Activite des Bellevues")
  },
  {
    0x6848,   /* RCH */
    TEXT("Reach Technology Inc")
  },
  {
    0x6948,   /* RCI */
    TEXT("RC International")
  },
  {
    0x6E48,   /* RCN */
    TEXT("Radio Consult SRL")
  },
  {
    0x8948,   /* RDI */
    TEXT("Rainbow Displays, Inc.")
  },
  {
    0x8D48,   /* RDM */
    TEXT("Tremon Enterprises Company Ltd")
  },
  {
    0x9348,   /* RDS */
    TEXT("Radius Inc")
  },
  {
    0xA148,   /* REA */
    TEXT("Real D")
  },
  {
    0xA348,   /* REC */
    TEXT("ReCom")
  },
  {
    0xA448,   /* RED */
    TEXT("Research Electronics Development Inc")
  },
  {
    0xA648,   /* REF */
    TEXT("Reflectivity, Inc.")
  },
  {
    0xAC48,   /* REL */
    TEXT("Reliance Electric Ind Corporation")
  },
  {
    0xAD48,   /* REM */
    TEXT("SCI Systems Inc.")
  },
  {
    0xAE48,   /* REN */
    TEXT("Renesas Technology Corp.")
  },
  {
    0xB348,   /* RES */
    TEXT("ResMed Pty Ltd")
  },
  {
    0xEC48,   /* RGL */
    TEXT("Robertson Geologging Ltd")
  },
  {
    0x0D49,   /* RHM */
    TEXT("Rohm Company Ltd")
  },
  {
    0x2949,   /* RII */
    TEXT("Racal Interlan Inc")
  },
  {
    0x2F49,   /* RIO */
    TEXT("Rios Systems Company Ltd")
  },
  {
    0x3449,   /* RIT */
    TEXT("Ritech Inc")
  },
  {
    0x3649,   /* RIV */
    TEXT("Rivulet Communications")
  },
  {
    0x4149,   /* RJA */
    TEXT("Roland Corporation")
  },
  {
    0x5349,   /* RJS */
    TEXT("Advanced Engineering")
  },
  {
    0x6349,   /* RKC */
    TEXT("Reakin Technolohy Corporation")
  },
  {
    0x8449,   /* RLD */
    TEXT("MEPCO")
  },
  {
    0x8E49,   /* RLN */
    TEXT("RadioLAN Inc")
  },
  {
    0xA349,   /* RMC */
    TEXT("Raritan Computer, Inc")
  },
  {
    0xB049,   /* RMP */
    TEXT("Research Machines")
  },
  {
    0xC249,   /* RNB */
    TEXT("Rainbow Technologies")
  },
  {
    0xE249,   /* ROB */
    TEXT("Robust Electronics GmbH ")
  },
  {
    0xE849,   /* ROH */
    TEXT("Rohm Co., Ltd.")
  },
  {
    0xEB49,   /* ROK */
    TEXT("Rockwell International")
  },
  {
    0xF049,   /* ROP */
    TEXT("Roper International Ltd")
  },
  {
    0x144A,   /* RPT */
    TEXT("R.P.T.Intergroups")
  },
  {
    0x494A,   /* RRI */
    TEXT("Radicom Research Inc")
  },
  {
    0x634A,   /* RSC */
    TEXT("PhotoTelesis")
  },
  {
    0x684A,   /* RSH */
    TEXT("ADC-Centre")
  },
  {
    0x694A,   /* RSI */
    TEXT("Rampage Systems Inc")
  },
  {
    0x6E4A,   /* RSN */
    TEXT("Radiospire Networks, Inc.")
  },
  {
    0x714A,   /* RSQ */
    TEXT("R Squared")
  },
  {
    0x734A,   /* RSS */
    TEXT("Rockwell Semiconductor Systems")
  },
  {
    0x784A,   /* RSX */
    TEXT("Rapid Tech Corporation")
  },
  {
    0x834A,   /* RTC */
    TEXT("Relia Technologies")
  },
  {
    0x894A,   /* RTI */
    TEXT("Rancho Tech Inc")
  },
  {
    0x8C4A,   /* RTL */
    TEXT("Realtek Semiconductor Company Ltd")
  },
  {
    0x934A,   /* RTS */
    TEXT("Raintree Systems")
  },
  {
    0xAE4A,   /* RUN */
    TEXT("RUNCO International")
  },
  {
    0xB04A,   /* RUP */
    TEXT("Ups Manufactoring s.r.l.")
  },
  {
    0xC34A,   /* RVC */
    TEXT("RSI Systems Inc")
  },
  {
    0xC94A,   /* RVI */
    TEXT("Realvision Inc")
  },
  {
    0xCC4A,   /* RVL */
    TEXT("Reveal Computer Prod")
  },
  {
    0xE34A,   /* RWC */
    TEXT("Red Wing Corporation")
  },
  {
    0x144B,   /* RXT */
    TEXT("Tectona SoftSolutions (P) Ltd.,")
  },
  {
    0x214C,   /* SAA */
    TEXT("Sanritz Automation Co.,Ltd.")
  },
  {
    0x254C,   /* SAE */
    TEXT("Saab Aerotech")
  },
  {
    0x274C,   /* SAG */
    TEXT("Sedlbauer")
  },
  {
    0x294C,   /* SAI */
    TEXT("Sage Inc")
  },
  {
    0x2B4C,   /* SAK */
    TEXT("Saitek Ltd")
  },
  {
    0x2D4C,   /* SAM */
    TEXT("Samsung Electric Company")
  },
  {
    0x2E4C,   /* SAN */
    TEXT("Sanyo Electric Co.,Ltd.")
  },
  {
    0x334C,   /* SAS */
    TEXT("Stores Automated Systems Inc")
  },
  {
    0x344C,   /* SAT */
    TEXT("Shuttle Tech")
  },
  {
    0x434C,   /* SBC */
    TEXT("Shanghai Bell Telephone Equip Mfg Co")
  },
  {
    0x444C,   /* SBD */
    TEXT("Softbed - Consulting & Development Ltd")
  },
  {
    0x494C,   /* SBI */
    TEXT("SMART Technologies Inc.")
  },
  {
    0x534C,   /* SBS */
    TEXT("SBS-or Industrial Computers GmbH")
  },
  {
    0x544C,   /* SBT */
    TEXT("Senseboard Technologies AB")
  },
  {
    0x634C,   /* SCC */
    TEXT("SORD Computer Corporation")
  },
  {
    0x644C,   /* SCD */
    TEXT("Sanyo Electric Company Ltd")
  },
  {
    0x654C,   /* SCE */
    TEXT("Sun Corporation")
  },
  {
    0x684C,   /* SCH */
    TEXT("Schlumberger Cards")
  },
  {
    0x694C,   /* SCI */
    TEXT("System Craft")
  },
  {
    0x6C4C,   /* SCL */
    TEXT("Sigmacom Co., Ltd.")
  },
  {
    0x6D4C,   /* SCM */
    TEXT("SCM Microsystems Inc")
  },
  {
    0x6E4C,   /* SCN */
    TEXT("Scanport, Inc.")
  },
  {
    0x6F4C,   /* SCO */
    TEXT("SORCUS Computer GmbH")
  },
  {
    0x704C,   /* SCP */
    TEXT("Scriptel Corporation")
  },
  {
    0x724C,   /* SCR */
    TEXT("Systran Corporation")
  },
  {
    0x734C,   /* SCS */
    TEXT("Nanomach Anstalt")
  },
  {
    0x744C,   /* SCT */
    TEXT("Smart Card Technology")
  },
  {
    0x814C,   /* SDA */
    TEXT("SAT (Societe Anonyme)")
  },
  {
    0x844C,   /* SDD */
    TEXT("Intrada-SDD Ltd")
  },
  {
    0x854C,   /* SDE */
    TEXT("Sherwood Digital Electronics Corporation")
  },
  {
    0x864C,   /* SDF */
    TEXT("SODIFF E&T CO., Ltd.")
  },
  {
    0x884C,   /* SDH */
    TEXT("Communications Specialies, Inc.")
  },
  {
    0x894C,   /* SDI */
    TEXT("Samtron Displays Inc")
  },
  {
    0x8B4C,   /* SDK */
    TEXT("SAIT-Devlonics")
  },
  {
    0x924C,   /* SDR */
    TEXT("SDR Systems")
  },
  {
    0x934C,   /* SDS */
    TEXT("SunRiver Data System")
  },
  {
    0x944C,   /* SDT */
    TEXT("Siemens AG")
  },
  {
    0x984C,   /* SDX */
    TEXT("SDX Business Systems Ltd")
  },
  {
    0xA14C,   /* SEA */
    TEXT("Seanix Technology Inc.")
  },
  {
    0xA24C,   /* SEB */
    TEXT("system elektronik GmbH")
  },
  {
    0xA34C,   /* SEC */
    TEXT("Seiko Epson Corporation")
  },
  {
    0xA54C,   /* SEE */
    TEXT("SeeColor Corporation")
  },
  {
    0xA94C,   /* SEI */
    TEXT("Seitz & Associates Inc")
  },
  {
    0xAC4C,   /* SEL */
    TEXT("Way2Call Communications")
  },
  {
    0xAD4C,   /* SEM */
    TEXT("Samsung Electronics Company Ltd")
  },
  {
    0xAE4C,   /* SEN */
    TEXT("Sencore")
  },
  {
    0xAF4C,   /* SEO */
    TEXT("SEOS Ltd")
  },
  {
    0xB04C,   /* SEP */
    TEXT("SEP Eletronica Ltda.")
  },
  {
    0xB24C,   /* SER */
    TEXT("Sony Ericsson Mobile Communications Inc.")
  },
  {
    0xB44C,   /* SET */
    TEXT("SendTek Corporation")
  },
  {
    0xCD4C,   /* SFM */
    TEXT("TORNADO Company")
  },
  {
    0xD44C,   /* SFT */
    TEXT("Mikroforum Ring 3")
  },
  {
    0xE34C,   /* SGC */
    TEXT("Spectragraphics Corporation")
  },
  {
    0xE44C,   /* SGD */
    TEXT("Sigma Designs, Inc.")
  },
  {
    0xE54C,   /* SGE */
    TEXT("Kansai Electric Company Ltd")
  },
  {
    0xE94C,   /* SGI */
    TEXT("Scan Group Ltd")
  },
  {
    0xEC4C,   /* SGL */
    TEXT("Super Gate Technology Company Ltd")
  },
  {
    0xED4C,   /* SGM */
    TEXT("SAGEM")
  },
  {
    0xEF4C,   /* SGO */
    TEXT("Logos Design A/S")
  },
  {
    0xF44C,   /* SGT */
    TEXT("Stargate Technology")
  },
  {
    0xF84C,   /* SGX */
    TEXT("Silicon Graphics Inc")
  },
  {
    0xFA4C,   /* SGZ */
    TEXT("Systec Computer GmbH")
  },
  {
    0x034D,   /* SHC */
    TEXT("ShibaSoku Co., Ltd.")
  },
  {
    0x074D,   /* SHG */
    TEXT("Soft & Hardware development Goldammer GmbH")
  },
  {
    0x094D,   /* SHI */
    TEXT("Jiangsu Shinco Electronic Group Co., Ltd")
  },
  {
    0x104D,   /* SHP */
    TEXT("Sharp Corporation")
  },
  {
    0x124D,   /* SHR */
    TEXT("Digital Discovery")
  },
  {
    0x144D,   /* SHT */
    TEXT("Shin Ho Tech")
  },
  {
    0x214D,   /* SIA */
    TEXT("SIEMENS AG")
  },
  {
    0x224D,   /* SIB */
    TEXT("Sanyo Electric Company Ltd")
  },
  {
    0x234D,   /* SIC */
    TEXT("Sysmate Corporation")
  },
  {
    0x244D,   /* SID */
    TEXT("Seiko Instruments Information Devices Inc")
  },
  {
    0x254D,   /* SIE */
    TEXT("Siemens")
  },
  {
    0x274D,   /* SIG */
    TEXT("Sigma Designs Inc")
  },
  {
    0x294D,   /* SII */
    TEXT("Silicon Image, Inc.")
  },
  {
    0x2C4D,   /* SIL */
    TEXT("Silicon Laboratories, Inc")
  },
  {
    0x2D4D,   /* SIM */
    TEXT("S3 Inc")
  },
  {
    0x2E4D,   /* SIN */
    TEXT("Singular Technology Co., Ltd.")
  },
  {
    0x324D,   /* SIR */
    TEXT("Sirius Technologies Pty Ltd")
  },
  {
    0x334D,   /* SIS */
    TEXT("Silicon Integrated Systems Corporation")
  },
  {
    0x344D,   /* SIT */
    TEXT("Sitintel")
  },
  {
    0x354D,   /* SIU */
    TEXT("Seiko Instruments USA Inc")
  },
  {
    0x384D,   /* SIX */
    TEXT("Zuniq Data Corporation")
  },
  {
    0x454D,   /* SJE */
    TEXT("Sejin Electron Inc")
  },
  {
    0x644D,   /* SKD */
    TEXT("Schneider & Koch")
  },
  {
    0x744D,   /* SKT */
    TEXT("Samsung Electro-Mechanics Company Ltd")
  },
  {
    0x794D,   /* SKY */
    TEXT("SKYDATA S.P.A.")
  },
  {
    0x814D,   /* SLA */
    TEXT("Systeme Lauer GmbH&Co KG")
  },
  {
    0x824D,   /* SLB */
    TEXT("Shlumberger Ltd")
  },
  {
    0x834D,   /* SLC */
    TEXT("Syslogic Datentechnik AG")
  },
  {
    0x884D,   /* SLH */
    TEXT("Silicon Library Inc.")
  },
  {
    0x894D,   /* SLI */
    TEXT("Symbios Logic Inc")
  },
  {
    0x8B4D,   /* SLK */
    TEXT("Silitek Corporation")
  },
  {
    0x8D4D,   /* SLM */
    TEXT("Solomon Technology Corporation")
  },
  {
    0x924D,   /* SLR */
    TEXT("Schlumberger Technology Corporate")
  },
  {
    0x944D,   /* SLT */
    TEXT("Salt Internatioinal Corp.")
  },
  {
    0x984D,   /* SLX */
    TEXT("Specialix")
  },
  {
    0xA14D,   /* SMA */
    TEXT("SMART Modular Technologies")
  },
  {
    0xA24D,   /* SMB */
    TEXT("Schlumberger")
  },
  {
    0xA34D,   /* SMC */
    TEXT("Standard Microsystems Corporation")
  },
  {
    0xA54D,   /* SME */
    TEXT("Sysmate Company")
  },
  {
    0xA94D,   /* SMI */
    TEXT("SpaceLabs Medical Inc")
  },
  {
    0xAB4D,   /* SMK */
    TEXT("SMK CORPORATION")
  },
  {
    0xAC4D,   /* SML */
    TEXT("Sumitomo Metal Industries, Ltd.")
  },
  {
    0xAD4D,   /* SMM */
    TEXT("Shark Multimedia Inc")
  },
  {
    0xAF4D,   /* SMO */
    TEXT("STMicroelectronics")
  },
  {
    0xB04D,   /* SMP */
    TEXT("Simple Computing")
  },
  {
    0xB24D,   /* SMR */
    TEXT("B.& V. s.r.l.")
  },
  {
    0xB34D,   /* SMS */
    TEXT("Silicom Multimedia Systems Inc")
  },
  {
    0xB44D,   /* SMT */
    TEXT("Silcom Manufacturing Tech Inc")
  },
  {
    0xC34D,   /* SNC */
    TEXT("Sentronic International Corp.")
  },
  {
    0xC94D,   /* SNI */
    TEXT("Siemens Microdesign GmbH")
  },
  {
    0xCB4D,   /* SNK */
    TEXT("S&K Electronics")
  },
  {
    0xCF4D,   /* SNO */
    TEXT("SINOSUN TECHNOLOGY CO., LTD")
  },
  {
    0xD04D,   /* SNP */
    TEXT("Siemens Nixdorf Info Systems")
  },
  {
    0xD34D,   /* SNS */
    TEXT("Cirtech (UK) Ltd")
  },
  {
    0xD44D,   /* SNT */
    TEXT("SuperNet Inc")
  },
  {
    0xD74D,   /* SNW */
    TEXT("Snell & Wilcox")
  },
  {
    0xD84D,   /* SNX */
    TEXT("Sonix Comm. Ltd")
  },
  {
    0xD94D,   /* SNY */
    TEXT("Sony")
  },
  {
    0xE94D,   /* SOI */
    TEXT("Silicon Optix Corporation")
  },
  {
    0xEC4D,   /* SOL */
    TEXT("Solitron Technologies Inc")
  },
  {
    0xEE4D,   /* SON */
    TEXT("Sony")
  },
  {
    0xF24D,   /* SOR */
    TEXT("Sorcus Computer GmbH")
  },
  {
    0xF44D,   /* SOT */
    TEXT("Sotec Company Ltd")
  },
  {
    0xF94D,   /* SOY */
    TEXT("SOYO Group, Inc")
  },
  {
    0x034E,   /* SPC */
    TEXT("SpinCore Technologies, Inc")
  },
  {
    0x054E,   /* SPE */
    TEXT("SPEA Software AG")
  },
  {
    0x084E,   /* SPH */
    TEXT("G&W Instruments GmbH")
  },
  {
    0x094E,   /* SPI */
    TEXT("SPACE-I Co., Ltd.")
  },
  {
    0x0C4E,   /* SPL */
    TEXT("Smart Silicon Systems Pty Ltd")
  },
  {
    0x0E4E,   /* SPN */
    TEXT("Sapience Corporation")
  },
  {
    0x124E,   /* SPR */
    TEXT("pmns GmbH")
  },
  {
    0x134E,   /* SPS */
    TEXT("Synopsys Inc")
  },
  {
    0x144E,   /* SPT */
    TEXT("Sceptre Tech Inc")
  },
  {
    0x154E,   /* SPU */
    TEXT("SIM2 Multimedia S.P.A.")
  },
  {
    0x184E,   /* SPX */
    TEXT("Simplex Time Recorder Co.")
  },
  {
    0x344E,   /* SQT */
    TEXT("Sequent Computer Systems Inc")
  },
  {
    0x434E,   /* SRC */
    TEXT("Integrated Tech Express Inc")
  },
  {
    0x444E,   /* SRD */
    TEXT("Setred")
  },
  {
    0x464E,   /* SRF */
    TEXT("Surf Communication Solutions Ltd")
  },
  {
    0x474E,   /* SRG */
    TEXT("Intuitive Surgical, Inc.")
  },
  {
    0x544E,   /* SRT */
    TEXT("SeeReal Technologies GmbH")
  },
  {
    0x634E,   /* SSC */
    TEXT("Sierra Semiconductor Inc")
  },
  {
    0x644E,   /* SSD */
    TEXT("FlightSafety International")
  },
  {
    0x654E,   /* SSE */
    TEXT("Samsung Electronic Co.")
  },
  {
    0x694E,   /* SSI */
    TEXT("S-S Technology Inc")
  },
  {
    0x6A4E,   /* SSJ */
    TEXT("Sankyo Seiki Mfg.co., Ltd")
  },
  {
    0x704E,   /* SSP */
    TEXT("Spectrum Signal Proecessing Inc")
  },
  {
    0x734E,   /* SSS */
    TEXT("S3 Inc")
  },
  {
    0x744E,   /* SST */
    TEXT("SystemSoft Corporation")
  },
  {
    0x814E,   /* STA */
    TEXT("ST Electronics Systems Assembly Pte Ltd")
  },
  {
    0x824E,   /* STB */
    TEXT("STB Systems Inc")
  },
  {
    0x834E,   /* STC */
    TEXT("STAC Electronics")
  },
  {
    0x844E,   /* STD */
    TEXT("STD Computer Inc")
  },
  {
    0x854E,   /* STE */
    TEXT("SII Ido-Tsushin Inc")
  },
  {
    0x864E,   /* STF */
    TEXT("Starflight Electronics")
  },
  {
    0x874E,   /* STG */
    TEXT("StereoGraphics Corp.")
  },
  {
    0x884E,   /* STH */
    TEXT("Semtech Corporation")
  },
  {
    0x894E,   /* STI */
    TEXT("Smart Tech Inc")
  },
  {
    0x8B4E,   /* STK */
    TEXT("SANTAK CORP.")
  },
  {
    0x8C4E,   /* STL */
    TEXT("SigmaTel Inc")
  },
  {
    0x8D4E,   /* STM */
    TEXT("SGS Thomson Microelectronics")
  },
  {
    0x8E4E,   /* STN */
    TEXT("Samsung Electronics America")
  },
  {
    0x8F4E,   /* STO */
    TEXT("Stollmann E+V GmbH")
  },
  {
    0x904E,   /* STP */
    TEXT("StreamPlay Ltd")
  },
  {
    0x924E,   /* STR */
    TEXT("Starlight Networks Inc")
  },
  {
    0x934E,   /* STS */
    TEXT("SITECSYSTEM CO., LTD.")
  },
  {
    0x944E,   /* STT */
    TEXT("Star Paging Telecom Tech (Shenzhen) Co. Ltd.")
  },
  {
    0x974E,   /* STW */
    TEXT("Starwin Inc.")
  },
  {
    0x994E,   /* STY */
    TEXT("SDS Technologies")
  },
  {
    0xA24E,   /* SUB */
    TEXT("Subspace Comm. Inc")
  },
  {
    0xAD4E,   /* SUM */
    TEXT("Summagraphics Corporation")
  },
  {
    0xAE4E,   /* SUN */
    TEXT("Sun Electronics Corporation")
  },
  {
    0xB04E,   /* SUP */
    TEXT("Supra Corporation")
  },
  {
    0xB24E,   /* SUR */
    TEXT("Surenam Computer Corporation")
  },
  {
    0xC14E,   /* SVA */
    TEXT("SGEG")
  },
  {
    0xC34E,   /* SVC */
    TEXT("Intellix Corp.")
  },
  {
    0xC44E,   /* SVD */
    TEXT("SVD Computer")
  },
  {
    0xC94E,   /* SVI */
    TEXT("Sun Microsystems")
  },
  {
    0xD34E,   /* SVS */
    TEXT("SVSI")
  },
  {
    0xD44E,   /* SVT */
    TEXT("SEVIT Co., Ltd.")
  },
  {
    0xE34E,   /* SWC */
    TEXT("Software Cafe")
  },
  {
    0xE94E,   /* SWI */
    TEXT("Sierra Wireless Inc.")
  },
  {
    0xEC4E,   /* SWL */
    TEXT("Sharedware Ltd")
  },
  {
    0xF34E,   /* SWS */
    TEXT("Static")
  },
  {
    0xF44E,   /* SWT */
    TEXT("Software Technologies Group,Inc.")
  },
  {
    0x024F,   /* SXB */
    TEXT("Syntax-Brillian")
  },
  {
    0x044F,   /* SXD */
    TEXT("Silex technology, Inc.")
  },
  {
    0x0C4F,   /* SXL */
    TEXT("SolutionInside")
  },
  {
    0x234F,   /* SYC */
    TEXT("Sysmic")
  },
  {
    0x2B4F,   /* SYK */
    TEXT("Stryker Communications")
  },
  {
    0x2C4F,   /* SYL */
    TEXT("Sylvania Computer Products")
  },
  {
    0x2D4F,   /* SYM */
    TEXT("Symicron Computer Communications Ltd.")
  },
  {
    0x2E4F,   /* SYN */
    TEXT("Synaptics Inc")
  },
  {
    0x304F,   /* SYP */
    TEXT("SYPRO Co Ltd")
  },
  {
    0x334F,   /* SYS */
    TEXT("Sysgration Ltd")
  },
  {
    0x344F,   /* SYT */
    TEXT("Seyeon Tech Company Ltd")
  },
  {
    0x364F,   /* SYV */
    TEXT("SYVAX Inc")
  },
  {
    0x384F,   /* SYX */
    TEXT("Prime Systems, Inc.")
  },
  {
    0x2150,   /* TAA */
    TEXT("Tandberg")
  },
  {
    0x2250,   /* TAB */
    TEXT("Todos Data System AB")
  },
  {
    0x2750,   /* TAG */
    TEXT("Teles AG")
  },
  {
    0x2950,   /* TAI */
    TEXT("Toshiba America Info Systems Inc")
  },
  {
    0x2D50,   /* TAM */
    TEXT("Tamura Seisakusyo Ltd")
  },
  {
    0x3350,   /* TAS */
    TEXT("Taskit Rechnertechnik GmbH")
  },
  {
    0x3450,   /* TAT */
    TEXT("Teleliaison Inc")
  },
  {
    0x3850,   /* TAX */
    TEXT("Taxan (Europe) Ltd")
  },
  {
    0x4250,   /* TBB */
    TEXT("Triple S Engineering Inc")
  },
  {
    0x4350,   /* TBC */
    TEXT("Turbo Communication, Inc")
  },
  {
    0x5350,   /* TBS */
    TEXT("Turtle Beach System")
  },
  {
    0x6350,   /* TCC */
    TEXT("Tandon Corporation")
  },
  {
    0x6450,   /* TCD */
    TEXT("Taicom Data Systems Co., Ltd.")
  },
  {
    0x6550,   /* TCE */
    TEXT("Century Corporation")
  },
  {
    0x6850,   /* TCH */
    TEXT("Interaction Systems, Inc")
  },
  {
    0x6950,   /* TCI */
    TEXT("Tulip Computers Int'l B.V.")
  },
  {
    0x6A50,   /* TCJ */
    TEXT("TEAC America Inc")
  },
  {
    0x6C50,   /* TCL */
    TEXT("Technical Concepts Ltd")
  },
  {
    0x6D50,   /* TCM */
    TEXT("3Com Corporation")
  },
  {
    0x6E50,   /* TCN */
    TEXT("Tecnetics (PTY) Ltd")
  },
  {
    0x6F50,   /* TCO */
    TEXT("Thomas-Conrad Corporation")
  },
  {
    0x7250,   /* TCR */
    TEXT("Thomson Consumer Electronics")
  },
  {
    0x7350,   /* TCS */
    TEXT("Tatung Company of America Inc")
  },
  {
    0x7450,   /* TCT */
    TEXT("Telecom Technology Centre Co. Ltd.")
  },
  {
    0x7850,   /* TCX */
    TEXT("FREEMARS Heavy Industries")
  },
  {
    0x8350,   /* TDC */
    TEXT("Teradici")
  },
  {
    0x8450,   /* TDD */
    TEXT("Tandberg Data Display AS")
  },
  {
    0x8B50,   /* TDK */
    TEXT("TDK USA Corporation")
  },
  {
    0x8D50,   /* TDM */
    TEXT("Tandem Computer Europe Inc")
  },
  {
    0x9050,   /* TDP */
    TEXT("3D Perception")
  },
  {
    0x9350,   /* TDS */
    TEXT("Tri-Data Systems Inc")
  },
  {
    0x9450,   /* TDT */
    TEXT("TDT")
  },
  {
    0x9650,   /* TDV */
    TEXT("TDVision Systems, Inc.")
  },
  {
    0x9950,   /* TDY */
    TEXT("Tandy Electronics")
  },
  {
    0xA150,   /* TEA */
    TEXT("TEAC System Corporation")
  },
  {
    0xA350,   /* TEC */
    TEXT("Tecmar Inc")
  },
  {
    0xAB50,   /* TEK */
    TEXT("Tektronix Inc")
  },
  {
    0xAC50,   /* TEL */
    TEXT("Promotion and Display Technology Ltd.")
  },
  {
    0xB250,   /* TER */
    TEXT("TerraTec Electronic GmbH")
  },
  {
    0xE950,   /* TGI */
    TEXT("TriGem Computer Inc")
  },
  {
    0xED50,   /* TGM */
    TEXT("TriGem Computer,Inc.")
  },
  {
    0xF350,   /* TGS */
    TEXT("Torus Systems Ltd")
  },
  {
    0xF650,   /* TGV */
    TEXT("Grass Valley Germany GmbH")
  },
  {
    0x0E51,   /* THN */
    TEXT("Thundercom Holdings Sdn. Bhd.")
  },
  {
    0x2351,   /* TIC */
    TEXT("Trigem KinfoComm")
  },
  {
    0x3051,   /* TIP */
    TEXT("TIPTEL AG")
  },
  {
    0x3651,   /* TIV */
    TEXT("OOO Technoinvest")
  },
  {
    0x3851,   /* TIX */
    TEXT("Tixi.Com GmbH")
  },
  {
    0x6351,   /* TKC */
    TEXT("Taiko Electric Works.LTD")
  },
  {
    0x6E51,   /* TKN */
    TEXT("Teknor Microsystem Inc")
  },
  {
    0x6F51,   /* TKO */
    TEXT("TouchKo, Inc.")
  },
  {
    0x7351,   /* TKS */
    TEXT("TimeKeeping Systems, Inc.")
  },
  {
    0x8151,   /* TLA */
    TEXT("Ferrari Electronic GmbH")
  },
  {
    0x8451,   /* TLD */
    TEXT("Telindus")
  },
  {
    0x8951,   /* TLI */
    TEXT("TOSHIBA TELI CORPORATION")
  },
  {
    0x8B51,   /* TLK */
    TEXT("Telelink AG")
  },
  {
    0x9351,   /* TLS */
    TEXT("Teleste Educational OY")
  },
  {
    0x9451,   /* TLT */
    TEXT("Dai Telecom S.p.A.")
  },
  {
    0x9651,   /* TLV */
    TEXT("S3 Inc")
  },
  {
    0x9851,   /* TLX */
    TEXT("Telxon Corporation")
  },
  {
    0xA351,   /* TMC */
    TEXT("Techmedia Computer Systems Corporation")
  },
  {
    0xA551,   /* TME */
    TEXT("AT&T Microelectronics")
  },
  {
    0xA951,   /* TMI */
    TEXT("Texas Microsystem")
  },
  {
    0xAD51,   /* TMM */
    TEXT("Time Management, Inc.")
  },
  {
    0xB251,   /* TMR */
    TEXT("Taicom International Inc")
  },
  {
    0xB351,   /* TMS */
    TEXT("Trident Microsystems Ltd")
  },
  {
    0xB451,   /* TMT */
    TEXT("T-Metrics Inc.")
  },
  {
    0xB851,   /* TMX */
    TEXT("Thermotrex Corporation")
  },
  {
    0xC351,   /* TNC */
    TEXT("TNC Industrial Company Ltd")
  },
  {
    0xCD51,   /* TNM */
    TEXT("TECNIMAGEN SA")
  },
  {
    0xD951,   /* TNY */
    TEXT("Tennyson Tech Pty Ltd")
  },
  {
    0xE551,   /* TOE */
    TEXT("TOEI Electronics Co., Ltd.")
  },
  {
    0xE751,   /* TOG */
    TEXT("The OPEN Group")
  },
  {
    0xF051,   /* TOP */
    TEXT("Orion Communications Co., Ltd.")
  },
  {
    0xF351,   /* TOS */
    TEXT("Toshiba Corporation")
  },
  {
    0xF551,   /* TOU */
    TEXT("Touchstone Technology")
  },
  {
    0x0352,   /* TPC */
    TEXT("Touch Panel Systems Corporation")
  },
  {
    0x0552,   /* TPE */
    TEXT("Technology Power Enterprises Inc")
  },
  {
    0x0A52,   /* TPJ */
    TEXT("(none)")
  },
  {
    0x0B52,   /* TPK */
    TEXT("TOPRE CORPORATION")
  },
  {
    0x1252,   /* TPR */
    TEXT("Topro Technology Inc")
  },
  {
    0x1352,   /* TPS */
    TEXT("Teleprocessing Systeme GmbH")
  },
  {
    0x1652,   /* TPV */
    TEXT("Top Victory Electronics ( Fujian ) Company Ltd")
  },
  {
    0x1A52,   /* TPZ */
    TEXT("Ypoaz Systems Inc")
  },
  {
    0x4152,   /* TRA */
    TEXT("TriTech Microelectronics International")
  },
  {
    0x4352,   /* TRC */
    TEXT("Trioc AB")
  },
  {
    0x4452,   /* TRD */
    TEXT("Trident Microsystem Inc")
  },
  {
    0x4552,   /* TRE */
    TEXT("Tremetrics")
  },
  {
    0x4952,   /* TRI */
    TEXT("Tricord Systems")
  },
  {
    0x4C52,   /* TRL */
    TEXT("Royal Information")
  },
  {
    0x4D52,   /* TRM */
    TEXT("Tekram Technology Company Ltd")
  },
  {
    0x4E52,   /* TRN */
    TEXT("Datacommunicatie Tron B.V.")
  },
  {
    0x5352,   /* TRS */
    TEXT("Torus Systems Ltd")
  },
  {
    0x5552,   /* TRU */
    TEXT("Aashima Technology B.V.")
  },
  {
    0x5852,   /* TRX */
    TEXT("Trex Enterprises")
  },
  {
    0x6252,   /* TSB */
    TEXT("Toshiba America Info Systems Inc")
  },
  {
    0x6352,   /* TSC */
    TEXT("Sanyo Electric Company Ltd")
  },
  {
    0x6452,   /* TSD */
    TEXT("TechniSat Digital GmbH")
  },
  {
    0x6552,   /* TSE */
    TEXT("Tottori Sanyo Electric")
  },
  {
    0x6652,   /* TSF */
    TEXT("Racal-Airtech Software Forge Ltd")
  },
  {
    0x6752,   /* TSG */
    TEXT("The Software Group Ltd")
  },
  {
    0x6952,   /* TSI */
    TEXT("TeleVideo Systems")
  },
  {
    0x6C52,   /* TSL */
    TEXT("Tottori SANYO Electric Co., Ltd.")
  },
  {
    0x7052,   /* TSP */
    TEXT("U.S. Navy")
  },
  {
    0x7452,   /* TST */
    TEXT("Transtream Inc")
  },
  {
    0x7952,   /* TSY */
    TEXT("TouchSystems")
  },
  {
    0x8152,   /* TTA */
    TEXT("Topson Technology Co., Ltd.")
  },
  {
    0x8252,   /* TTB */
    TEXT("National Semiconductor Japan Ltd")
  },
  {
    0x8352,   /* TTC */
    TEXT("Telecommunications Techniques Corporation")
  },
  {
    0x8552,   /* TTE */
    TEXT("TTE, Inc.")
  },
  {
    0x8952,   /* TTI */
    TEXT("Trenton Terminals Inc")
  },
  {
    0x8B52,   /* TTK */
    TEXT("Totoku Electric Company Ltd")
  },
  {
    0x8C52,   /* TTL */
    TEXT("2-Tel B.V.")
  },
  {
    0x9352,   /* TTS */
    TEXT("TechnoTrend Systemtechnik GmbH")
  },
  {
    0xB452,   /* TUT */
    TEXT("Tut Systems")
  },
  {
    0xC452,   /* TVD */
    TEXT("Tecnovision")
  },
  {
    0xC952,   /* TVI */
    TEXT("Truevision")
  },
  {
    0xCD52,   /* TVM */
    TEXT("Taiwan Video & Monitor Corporation")
  },
  {
    0xCF52,   /* TVO */
    TEXT("TV One Ltd")
  },
  {
    0xD252,   /* TVR */
    TEXT("TV Interactive Corporation")
  },
  {
    0xD352,   /* TVS */
    TEXT("TVS Electronics Limited")
  },
  {
    0xE152,   /* TWA */
    TEXT("Tidewater Association")
  },
  {
    0xE552,   /* TWE */
    TEXT("Kontron Electronik")
  },
  {
    0xE852,   /* TWH */
    TEXT("Twinhead International Corporation")
  },
  {
    0xE952,   /* TWI */
    TEXT("Easytel oy")
  },
  {
    0xEB52,   /* TWK */
    TEXT("TOWITOKO electronics GmbH")
  },
  {
    0x0C53,   /* TXL */
    TEXT("Trixel Ltd")
  },
  {
    0x0E53,   /* TXN */
    TEXT("Texas Insturments")
  },
  {
    0x1453,   /* TXT */
    TEXT("Textron Defense System")
  },
  {
    0x2E53,   /* TYN */
    TEXT("Tyan Computer Corporation")
  },
  {
    0x3354,   /* UAS */
    TEXT("Ultima Associates Pte Ltd")
  },
  {
    0x4954,   /* UBI */
    TEXT("Ungermann-Bass Inc")
  },
  {
    0x4C54,   /* UBL */
    TEXT("Ubinetics Ltd.")
  },
  {
    0x8E54,   /* UDN */
    TEXT("Uniden Corporation")
  },
  {
    0xA354,   /* UEC */
    TEXT("Ultima Electronics Corporation")
  },
  {
    0xA754,   /* UEG */
    TEXT("Elitegroup Computer Systems Company Ltd")
  },
  {
    0xA954,   /* UEI */
    TEXT("Universal Electronics Inc")
  },
  {
    0xB454,   /* UET */
    TEXT("Universal Empowering Technologies")
  },
  {
    0xC754,   /* UFG */
    TEXT("UNIGRAF-USA")
  },
  {
    0xCF54,   /* UFO */
    TEXT("UFO Systems Inc")
  },
  {
    0x0255,   /* UHB */
    TEXT("XOCECO")
  },
  {
    0x2355,   /* UIC */
    TEXT("Uniform Industrial Corporation")
  },
  {
    0x5255,   /* UJR */
    TEXT("Ueda Japan Radio Co., Ltd.")
  },
  {
    0x9455,   /* ULT */
    TEXT("Ultra Network Tech")
  },
  {
    0xA355,   /* UMC */
    TEXT("United Microelectr Corporation")
  },
  {
    0xA755,   /* UMG */
    TEXT("Umezawa Giken Co.,Ltd")
  },
  {
    0xAD55,   /* UMM */
    TEXT("Universal Multimedia")
  },
  {
    0xC155,   /* UNA */
    TEXT("Unisys DSD")
  },
  {
    0xC255,   /* UNB */
    TEXT("Unisys Corporation")
  },
  {
    0xC355,   /* UNC */
    TEXT("Unisys Corporation")
  },
  {
    0xC455,   /* UND */
    TEXT("DO NOT USE - UND")
  },
  {
    0xC555,   /* UNE */
    TEXT("DO NOT USE - UNE")
  },
  {
    0xC655,   /* UNF */
    TEXT("DO NOT USE - UNF")
  },
  {
    0xC955,   /* UNI */
    TEXT("Unisys Corporation")
  },
  {
    0xC955,   /* UNI */
    TEXT("Unisys Corporation")
  },
  {
    0xCD55,   /* UNM */
    TEXT("Unisys Corporation")
  },
  {
    0xCF55,   /* UNO */
    TEXT("Unisys Corporation")
  },
  {
    0xD055,   /* UNP */
    TEXT("Unitop")
  },
  {
    0xD355,   /* UNS */
    TEXT("Unisys Corporation")
  },
  {
    0xD455,   /* UNT */
    TEXT("Unisys Corporation")
  },
  {
    0xD955,   /* UNY */
    TEXT("Unicate")
  },
  {
    0x1056,   /* UPP */
    TEXT("UPPI")
  },
  {
    0x1356,   /* UPS */
    TEXT("Systems Enhancement")
  },
  {
    0x4456,   /* URD */
    TEXT("Video Computer S.p.A.")
  },
  {
    0x6156,   /* USA */
    TEXT("Utimaco Safeware AG")
  },
  {
    0x6456,   /* USD */
    TEXT("U.S. Digital Corporation")
  },
  {
    0x6956,   /* USI */
    TEXT("Universal Scientific Industrial Co., Ltd.")
  },
  {
    0x7256,   /* USR */
    TEXT("U.S. Robotics Inc")
  },
  {
    0x8456,   /* UTD */
    TEXT("Up to Date Tech")
  },
  {
    0xE356,   /* UWC */
    TEXT("Uniwill Computer Corp.")
  },
  {
    0x2C58,   /* VAL */
    TEXT("Valence Computing Corporation")
  },
  {
    0x3258,   /* VAR */
    TEXT("Varian Australia Pty Ltd")
  },
  {
    0x5458,   /* VBT */
    TEXT("Valley Board Ltda")
  },
  {
    0x6358,   /* VCC */
    TEXT("Virtual Computer Corporation")
  },
  {
    0x6958,   /* VCI */
    TEXT("VistaCom Inc")
  },
  {
    0x6A58,   /* VCJ */
    TEXT("Victor Company of Japan, Limited")
  },
  {
    0x6D58,   /* VCM */
    TEXT("Vector Magnetics, LLC")
  },
  {
    0x7858,   /* VCX */
    TEXT("VCONEX")
  },
  {
    0x8158,   /* VDA */
    TEXT("Victor Data Systems")
  },
  {
    0x8D58,   /* VDM */
    TEXT("Vadem")
  },
  {
    0x8F58,   /* VDO */
    TEXT("Video & Display Oriented Corporation")
  },
  {
    0x9358,   /* VDS */
    TEXT("Vidisys GmbH & Company")
  },
  {
    0x9458,   /* VDT */
    TEXT("Viditec, Inc.")
  },
  {
    0xA358,   /* VEC */
    TEXT("Vector Informatik GmbH")
  },
  {
    0xAB58,   /* VEK */
    TEXT("Vektrex")
  },
  {
    0xB358,   /* VES */
    TEXT("Vestel Elektronik Sanayi ve Ticaret A. S.")
  },
  {
    0xC958,   /* VFI */
    TEXT("VeriFone Inc")
  },
  {
    0x0959,   /* VHI */
    TEXT("Macrocad Development Inc.")
  },
  {
    0x2159,   /* VIA */
    TEXT("VIA Tech Inc")
  },
  {
    0x2259,   /* VIB */
    TEXT("Tatung UK Ltd")
  },
  {
    0x2359,   /* VIC */
    TEXT("Victron B.V.")
  },
  {
    0x2459,   /* VID */
    TEXT("Ingram Macrotron Germany")
  },
  {
    0x2B59,   /* VIK */
    TEXT("Viking Connectors")
  },
  {
    0x2E59,   /* VIN */
    TEXT("Vine Micros Ltd")
  },
  {
    0x3259,   /* VIR */
    TEXT("Visual Interface, Inc")
  },
  {
    0x3359,   /* VIS */
    TEXT("Visioneer")
  },
  {
    0x3459,   /* VIT */
    TEXT("Visitech AS")
  },
  {
    0x8259,   /* VLB */
    TEXT("ValleyBoard Ltda.")
  },
  {
    0x9459,   /* VLT */
    TEXT("VideoLan Technologies")
  },
  {
    0xA959,   /* VMI */
    TEXT("Vermont Microsystems")
  },
  {
    0xAC59,   /* VML */
    TEXT("Vine Micros Limited")
  },
  {
    0xC359,   /* VNC */
    TEXT("Vinca Corporation")
  },
  {
    0xE259,   /* VOB */
    TEXT("MaxData Computer AG")
  },
  {
    0x125A,   /* VPR */
    TEXT("Best Buy")
  },
  {
    0x435A,   /* VRC */
    TEXT("Virtual Resources Corporation")
  },
  {
    0x635A,   /* VSC */
    TEXT("ViewSonic Corporation")
  },
  {
    0x645A,   /* VSD */
    TEXT("3M")
  },
  {
    0x695A,   /* VSI */
    TEXT("VideoServer")
  },
  {
    0x6E5A,   /* VSN */
    TEXT("Ingram Macrotron")
  },
  {
    0x705A,   /* VSP */
    TEXT("Vision Systems GmbH")
  },
  {
    0x725A,   /* VSR */
    TEXT("V-Star Electronics Inc.")
  },
  {
    0x835A,   /* VTC */
    TEXT("VTel Corporation")
  },
  {
    0x875A,   /* VTG */
    TEXT("Voice Technologies Group Inc")
  },
  {
    0x895A,   /* VTI */
    TEXT("VLSI Tech Inc")
  },
  {
    0x8B5A,   /* VTK */
    TEXT("Viewteck Co., Ltd.")
  },
  {
    0x8C5A,   /* VTL */
    TEXT("Vivid Technology Pte Ltd")
  },
  {
    0x935A,   /* VTS */
    TEXT("VTech Computers Ltd")
  },
  {
    0x965A,   /* VTV */
    TEXT("VATIV Technologies")
  },
  {
    0xB45A,   /* VUT */
    TEXT("Vutrix (UK) Ltd")
  },
  {
    0xE25A,   /* VWB */
    TEXT("Vweb Corp.")
  },
  {
    0x235C,   /* WAC */
    TEXT("Wacom Tech")
  },
  {
    0x2C5C,   /* WAL */
    TEXT("Wave Access")
  },
  {
    0x365C,   /* WAV */
    TEXT("Wavephore")
  },
  {
    0x4E5C,   /* WBN */
    TEXT("MicroSoftWare")
  },
  {
    0x535C,   /* WBS */
    TEXT("WB Systemtechnik GmbH")
  },
  {
    0x695C,   /* WCI */
    TEXT("Wisecom Inc")
  },
  {
    0x735C,   /* WCS */
    TEXT("Woodwind Communications Systems Inc")
  },
  {
    0x835C,   /* WDC */
    TEXT("Western Digital")
  },
  {
    0x855C,   /* WDE */
    TEXT("Westinghouse Digital Electronics")
  },
  {
    0xA25C,   /* WEB */
    TEXT("WebGear Inc")
  },
  {
    0xA35C,   /* WEC */
    TEXT("Winbond Electronics Corporation")
  },
  {
    0xB95C,   /* WEY */
    TEXT("WEY Design AG")
  },
  {
    0x095D,   /* WHI */
    TEXT("Whistle Communications")
  },
  {
    0x295D,   /* WII */
    TEXT("Innoware Inc")
  },
  {
    0x2C5D,   /* WIL */
    TEXT("WIPRO Information Technology Ltd")
  },
  {
    0x2E5D,   /* WIN */
    TEXT("Wintop Technology Inc")
  },
  {
    0x305D,   /* WIP */
    TEXT("Wipro Infotech")
  },
  {
    0x685D,   /* WKH */
    TEXT("Uni-Take Int'l Inc.")
  },
  {
    0x845D,   /* WLD */
    TEXT("Wildfire Communications Inc")
  },
  {
    0xAC5D,   /* WML */
    TEXT("Wolfson Microelectronics Ltd")
  },
  {
    0xAF5D,   /* WMO */
    TEXT("Westermo Teleindustri AB")
  },
  {
    0xB45D,   /* WMT */
    TEXT("Winmate Communication Inc")
  },
  {
    0xC95D,   /* WNI */
    TEXT("WillNet Inc.")
  },
  {
    0xD65D,   /* WNV */
    TEXT("Winnov L.P.")
  },
  {
    0xD85D,   /* WNX */
    TEXT("Wincor Nixdorf International GmbH")
  },
  {
    0x015E,   /* WPA */
    TEXT("Matsushita Communication Industrial Co., Ltd.")
  },
  {
    0x095E,   /* WPI */
    TEXT("Wearnes Peripherals International (Pte) Ltd")
  },
  {
    0x435E,   /* WRC */
    TEXT("WiNRADiO Communications")
  },
  {
    0x635E,   /* WSC */
    TEXT("CIS Technology Inc")
  },
  {
    0x705E,   /* WSP */
    TEXT("Wireless And Smart Products Inc.")
  },
  {
    0x835E,   /* WTC */
    TEXT("ACC Microelectronics")
  },
  {
    0x895E,   /* WTI */
    TEXT("WorkStation Tech")
  },
  {
    0x8B5E,   /* WTK */
    TEXT("Wearnes Thakral Pte")
  },
  {
    0x935E,   /* WTS */
    TEXT("Restek Electric Company Ltd")
  },
  {
    0xCD5E,   /* WVM */
    TEXT("Wave Systems Corporation")
  },
  {
    0xF65E,   /* WWV */
    TEXT("World Wide Video, Inc.")
  },
  {
    0x335F,   /* WYS */
    TEXT("Myse Technology")
  },
  {
    0x345F,   /* WYT */
    TEXT("Wooyoung Image & Information Co.,Ltd.")
  },
  {
    0x2360,   /* XAC */
    TEXT("XAC Automation Corp")
  },
  {
    0xC760,   /* XFG */
    TEXT("Jan Strapko - FOTO")
  },
  {
    0xCF60,   /* XFO */
    TEXT("EXFO Electro Optical Engineering")
  },
  {
    0x2E61,   /* XIN */
    TEXT("Xinex Networks Inc")
  },
  {
    0x2F61,   /* XIO */
    TEXT("Xiotech Corporation")
  },
  {
    0x3261,   /* XIR */
    TEXT("Xirocm Inc")
  },
  {
    0x3461,   /* XIT */
    TEXT("Xitel Pty ltd")
  },
  {
    0x9861,   /* XLX */
    TEXT("Xilinx, Inc.")
  },
  {
    0xAD61,   /* XMM */
    TEXT("C3PO S.L.")
  },
  {
    0xD461,   /* XNT */
    TEXT("XN Technologies, Inc.")
  },
  {
    0x3562,   /* XQU */
    TEXT("SHANGHAI SVA-DAV ELECTRONICS CO., LTD")
  },
  {
    0x4362,   /* XRC */
    TEXT("Xircom Inc")
  },
  {
    0x4F62,   /* XRO */
    TEXT("XORO ELECTRONICS (CHENGDU) LIMITED")
  },
  {
    0x6E62,   /* XSN */
    TEXT("Xscreen AS")
  },
  {
    0x7462,   /* XST */
    TEXT("XS Technologies Inc")
  },
  {
    0x7962,   /* XSY */
    TEXT("XSYS")
  },
  {
    0x8462,   /* XTD */
    TEXT("Icuiti Corporation")
  },
  {
    0x8C62,   /* XTL */
    TEXT("Crystal Computer")
  },
  {
    0x8E62,   /* XTN */
    TEXT("X-10 (USA) Inc")
  },
  {
    0x2363,   /* XYC */
    TEXT("Xycotec Computer GmbH")
  },
  {
    0xA464,   /* YED */
    TEXT("Y-E Data Inc")
  },
  {
    0x1165,   /* YHQ */
    TEXT("Yokogawa Electric Corporation")
  },
  {
    0x1765,   /* YHW */
    TEXT("Exacom SA")
  },
  {
    0xA865,   /* YMH */
    TEXT("Yamaha Corporation")
  },
  {
    0xF765,   /* YOW */
    TEXT("American Biometric Company")
  },
  {
    0x2E68,   /* ZAN */
    TEXT("Zandar Technologies plc")
  },
  {
    0x3868,   /* ZAX */
    TEXT("Zefiro Acoustics")
  },
  {
    0x3A68,   /* ZAZ */
    TEXT("Zazzle Technologies")
  },
  {
    0x5268,   /* ZBR */
    TEXT("Zebra Technologies International, LLC")
  },
  {
    0x7468,   /* ZCT */
    TEXT("ZeitControl cardsystems GmbH")
  },
  {
    0x9368,   /* ZDS */
    TEXT("Zenith Data Systems")
  },
  {
    0xF468,   /* ZGT */
    TEXT("Zenith Data Systems")
  },
  {
    0x2369,   /* ZIC */
    TEXT("ZTEIC DESIGN CO., LTD. ")
  },
  {
    0xB469,   /* ZMT */
    TEXT("Zalman Tech Co., Ltd.")
  },
  {
    0xBA69,   /* ZMZ */
    TEXT("Z Microsystems")
  },
  {
    0xC969,   /* ZNI */
    TEXT("Zetinet Inc")
  },
  {
    0xD869,   /* ZNX */
    TEXT("Znyx Adv. Systems")
  },
  {
    0xF769,   /* ZOW */
    TEXT("Zowie Intertainment, Inc")
  },
  {
    0x4E6A,   /* ZRN */
    TEXT("Zoran Corporation")
  },
  {
    0x656A,   /* ZSE */
    TEXT("Zenith Data Systems")
  },
  {
    0x836A,   /* ZTC */
    TEXT("ZyDAS Technology Corporation")
  },
  {
    0x896A,   /* ZTI */
    TEXT("Zoom Telephonics Inc")
  },
  {
    0x8D6A,   /* ZTM */
    TEXT("ZT Group Int'l Inc.")
  },
  {
    0x246B,   /* ZYD */
    TEXT("Zydacron Inc")
  },
  {
    0x306B,   /* ZYP */
    TEXT("Zypcom Inc")
  },
  {
    0x346B,   /* ZYT */
    TEXT("Zytex Computers")
  },
  {
    0x386B,   /* ZYX */
    TEXT("Zyxel")
  },
  {
    0x5A6B,   /* ZZZ */
    TEXT("Boca Research Inc")
  },
  {
    0x8358,   /* VDC */
    TEXT("VDC Display Systems")
  },
  {
    0x934D,   /* SLS */
    TEXT("Schnick-Schnack-Systems GmbH")
  },
  {
    0x6520,   /* HCE */
    TEXT("Hitachi Consumer Electronics Co., Ltd")
  },
  {
    0x450F,   /* CZE */
    TEXT("Carl Zeiss AG")
  },
  {
    0x6116,   /* ESA */
    TEXT("Elbit Systems of America")
  },
  {
    0x2942,   /* PQI */
    TEXT("Pixel Qi")
  },
  {
    0xC942,   /* PVI */
    TEXT("Prime view international Co., Ltd")
  },
  {
    0x842E,   /* KTD */
    TEXT("Takahata Electronics Co.,Ltd.")
  },
  {
    0x2B38,   /* NAK */
    TEXT("Nakano Engineering Co.,Ltd.")
  },
  {
    0xE334,   /* MGC */
    TEXT("Mentor Graphics Corporation")
  },
  {
    0x3419,   /* FIT */
    TEXT("Feature Integration Technology Inc.")
  },
  {
    0x6422,   /* HSD */
    TEXT("HannStar Display Corp")
  },
  {
    0x7022,   /* HSP */
    TEXT("HannStar Display Corp")
  },
  {
    0x9414,   /* EDT */
    TEXT("Emerging Display Technologies Corp")
  },
  {
    0x8C11,   /* DLL */
    TEXT("Dell Inc")
  },
  {
    0xB411,   /* DMT */
    TEXT("Distributed Management Task Force, Inc. (DMTF) ")
  },
  {
    0x9742,   /* PTW */
    TEXT("DO NOT USE")
  },
  {
    0xC342,   /* PVC */
    TEXT("DO NOT USE")
  },
  {
    0x8B4A,   /* RTK */
    TEXT("DO NOT USE")
  },
  {
    0xA74C,   /* SEG */
    TEXT("DO NOT USE")
  },
  {
    0xCA51,   /* TNJ */
    TEXT("DO NOT USE")
  },
  {
    0x2E5C,   /* WAN */
    TEXT("DO NOT USE")
  },
  {
    0xB260,   /* XER */
    TEXT("DO NOT USE")
  },
  {
    0xE361,   /* XOC */
    TEXT("DO NOT USE")
  },
  {
    0x8519,   /* FLE */
    TEXT("ADTI Media, Inc")
  },
  {PNPID_END_OF_LIST_MARKER, NULL},  /* End of list marker */
};
