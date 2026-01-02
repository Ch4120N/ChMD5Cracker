#!/usr/bin/python3
#  ____  _____  _    _  ____  ____  ____  ____     ____  _  _     ___  _   _  __  __  ___   ___  _  _ 
# (  _ \(  _  )( \/\/ )( ___)(  _ \( ___)(  _ \   (  _ \( \/ )   / __)( )_( )/. |/  )(__ \ / _ \( \( )
#  )___/ )(_)(  )    (  )__)  )   / )__)  )(_) )   ) _ < \  /   ( (__  ) _ ((_  _))(  / _/( (_) ))  ( 
# (__)  (_____)(__/\__)(____)(_)\_)(____)(____/   (____/ (__)    \___)(_) (_) (_)(__)(____)\___/(_)\_)
#
###############################################################################

#########################################
#### Ch4120N MD5 Hash Cracker ######
#### Powered By Ch4120N ####
#########################################

import sys
import hashlib
import time
import string
import argparse
import itertools
import signal
from colorama import Fore, init
init()

alpha = ''
print(Fore.LIGHTGREEN_EX)
def exit_on_sigint(frm, func):
    sys.exit(f"\n{Fore.LIGHTBLUE_EX}[{Fore.LIGHTRED_EX}-{Fore.LIGHTBLUE_EX}] {Fore.LIGHTRED_EX} Program Interrupted")

signal.signal(signal.SIGINT, exit_on_sigint)
def usage():
    return """
 ##############################################################################
 # Ch4120N MD5 Hash Cracker                                                   #
 #                                                                            #
 # Usage: python chMd5Cracker.py <setChar> <minChar> <maxChar> <hashMD5>      #
 # Character options: a - small letters # a,b,c                               #
 #                    A - big letters   # A,B,C                               #
 #                    n - numbers       # 1,2,3                               #
 #                    s - symbols       # !,#,@                               #
 # Example: python chMd5Cracker.py aAns 1 4 9bd4f3afae4f32050d2b0e467c9fb8ef  #
 ##############################################################################
    """

def check_hash(text:str):
    return hashlib.md5(text.encode()).hexdigest()

parse = argparse.ArgumentParser(add_help=False, usage=usage())
parse.add_argument("setChar")
parse.add_argument("minChar", type=int)
parse.add_argument("maxChar", type=int)
parse.add_argument("MD5Hash")

parse.error = lambda message: print(usage()) or sys.exit(2)
args = parse.parse_args()
setChar = args.setChar
minChar = args.minChar
maxChar = args.maxChar
MD5Hash = args.MD5Hash

if not all(char in {'a', 'A', 'n', 's'} for char in setChar):
    sys.exit(f"{Fore.LIGHTBLUE_EX}[{Fore.LIGHTRED_EX}-{Fore.LIGHTBLUE_EX}] {Fore.LIGHTRED_EX}Invalid characters in argument '{setChar}'. Allowed characters are: 'a', 'A', 'n', 's'")
else:

    if "a" in setChar:
        alpha += string.ascii_lowercase
    if "A" in setChar:
        alpha += string.ascii_uppercase
    if "n" in setChar:
        alpha += string.digits
    if "s" in setChar:
        alpha += r"!\"$%&/()=?-.:\\*'-_:.;,"


if (len(MD5Hash) != 32): sys.exit(f"{Fore.LIGHTBLUE_EX}[{Fore.LIGHTRED_EX}-{Fore.LIGHTBLUE_EX}] {Fore.LIGHTRED_EX}Sorry but it seems that the MD5 is not valid!")



print(f"{Fore.LIGHTBLUE_EX}[{Fore.LIGHTGREEN_EX}+{Fore.LIGHTBLUE_EX}] {Fore.LIGHTGREEN_EX} Selected charset for attack: '{alpha}'")
print(f"{Fore.LIGHTBLUE_EX}[{Fore.LIGHTGREEN_EX}+{Fore.LIGHTBLUE_EX}] {Fore.LIGHTGREEN_EX} Going to Crack '{MD5Hash}'...")
time.sleep(2)


for len_strings in range(minChar, maxChar + 1):
    for atmp in itertools.product(alpha, repeat=len_strings):
        ret = ''.join(atmp)
        res = check_hash(ret)
        if res == MD5Hash:
            print("\n", f'{Fore.LIGHTCYAN_EX}-'*10, Fore.LIGHTGREEN_EX+"Everybody Needs A Hacker", f'{Fore.LIGHTCYAN_EX}-'*10)
            print(f"   {Fore.LIGHTGREEN_EX}Powered By {Fore.LIGHTRED_EX}Ch4120N")
            print(f'{Fore.LIGHTCYAN_EX}-'*46)
            sys.exit(f"{Fore.LIGHTBLUE_EX}[{Fore.LIGHTGREEN_EX}+{Fore.LIGHTBLUE_EX}] {Fore.LIGHTGREEN_EX} Password Cracked! => {Fore.LIGHTRED_EX}{ret}")
        
        print(MD5Hash, "=>", res, "=>", ret)


print("\n",f'{Fore.LIGHTCYAN_EX}-'*10, Fore.LIGHTGREEN_EX+"Everybody Needs A Hacker", f'{Fore.LIGHTCYAN_EX}-'*10)
print(f"   {Fore.LIGHTGREEN_EX}Powered By {Fore.LIGHTRED_EX}Ch4120N")
print(f'{Fore.LIGHTCYAN_EX}-'*46)
print(f"{Fore.LIGHTBLUE_EX}[{Fore.LIGHTRED_EX}-{Fore.LIGHTBLUE_EX}] {Fore.LIGHTGREEN_EX}Bruteforcing done with {alpha} Chars. {Fore.LIGHTRED_EX}No Results.")


