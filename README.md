<head>
  <meta name="google-site-verification" content="l4gzIHopgDDt57xRYeRvJZ5DYgg4lLb-qPciUxhNxkY" />
</head>

# ChMD5Cracker

`Ch4120N MD5 Hash Cracker` is a specialized tool for <u>hackers</u> designed to crack MD5 hashes using brute force. This tool is specifically created for penetration testing and security activities, helping hackers recover passwords from MD5 hashes.


## Project Porgrammer
> Ch4120N - Ch4120N@Proton.me

## Made For
> This script is designed for ethical hackers and security professionals to perform brute force attacks on MD5 hashes, helping them recover plaintext passwords during penetration testing and security assessments.

## 📑 Usage/Example
```
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
```

## 💻 Supported Operating Systems
- [X] Ubuntu/Debian
- [X] Kali Linux
- [X] Arch Linux/Red Hat Linux
- [X] Windows 10/8.1/8/7


## ✨ Features

* Good Command Line Design
* Easy to use
* Very High Speed
* Advanced Error Handling

## 📝️ How it Works?
The `Ch4120N MD5 Hash Cracker` is a <u>brute force</u> tool designed to crack MD5 hashes by systematically generating potential plaintexts and comparing their MD5 hash to the target hash. Here’s a breakdown of how the tool works:

1. Input Parameters:

    * The tool accepts four main arguments:

      - setChar: Specifies the character set to use, where a includes lowercase letters, A includes uppercase letters, n includes numbers, and s includes symbols.
  
      - minChar: The minimum length of the strings to be generated.
      - maxChar: The maximum length of the strings to be generated.
      - MD5Hash: The MD5 hash that needs to be cracked.

2. Character Set Selection:

    * Based on the setChar input, the tool constructs a character set that will be used to generate possible plaintexts. This set can include any combination of lowercase letters, uppercase letters, digits, and symbols.
  
3. Hash Cracking Process:

    * The tool iterates through all possible combinations of characters within the specified length range (minChar to maxChar).
  For each combination, it generates a string and calculates its MD5 hash using Python's hashlib library.
  The generated hash is then compared to the target MD5 hash.

4. Success and Failure:

    * If a match is found, the tool prints the cracked password and exits.
    * If no match is found after testing all combinations, the tool informs the user that the bruteforce attack was completed without success.

5. Output:

    * The tool provides real-time updates on the progress of the cracking attempt, showing the current string being tested and its corresponding MD5 hash.

This process continues until either the correct password is found or all possible combinations have been exhausted.

## ⚠️ Legal disclaimer ⚠️
> Usage of `Ch4120N MD5 Hash Cracker` for attacking targets without prior mutual consent is illegal. It's the end user's responsibility to obey all applicable local, state and federal laws. Developers assume no liability and are not responsible for any misuse or damage caused by this program.

## ❤️ Support the Project
If you find this tool useful, consider donating:

| Cryptocurrency | Address                                      |
| :------------- | :------------------------------------------- |
| **BTC**        | `bc1ql4syps7qpa3djqrxwht3g66tldyh4j7qsyjkq0` |
| **ETH**        | `0xfddbd535a4ad28792cbebceee3d6982d774e6d13` |
| **USDT**       | `3Cq6HRQsiwZFmPEQfG9eJkZE2QGChvf2VN`         |
> 💖 Your support helps keep the project alive and improving!

## ☠️ Reporting Issues

If you are facing a configuration issue or something is not working as you expected to be, please use the **Ch4120N@Proton.me**


<div align="center">

**⭐ If you like this project, don’t forget to give it a star!**

</div> 
