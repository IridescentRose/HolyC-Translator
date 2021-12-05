<h1 align="center">HolyC-Translator</h1>
<p align="center">Translates the language of our lord to commoner C for daily use</p>

![example workflow](https://github.com/IridescentRose/HolyC-Translator/actions/workflows/c-cpp.yml/badge.svg)

## How to Use

The translator can be simply used as follows:
```
> hc2c myFile.hc output.c
```

## Why?

Holy C is obviously the best programming language, meant to follow the lord's will. The language was created by the (in)famous Terry A. Davis who recently passed away in 2018. As a tribute to Terry, it would be great to attempt to honor him with a more portable HolyC so that his glorious creation can find some more use.

## Specification and Limitation

The translator attempts to imitate or reproduce the behavior defined by the specifications of [HolyC](https://web.archive.org/web/20170325000321/http://www.templeos.org/Wb/Doc/HolyC.html) as documented. However there are some critical limitations that a translator cannot do compared to a full on compiler. 

One of these limitations is JIT / AOT mode as described by the original HolyC specification. The `#ifaot` and `#ifjit` preprocessor directives will be changed to `#if 0`. 

The #exe directive executes and records the results of commands executed in the shell and encapsulates the output in quotations - which may not be preferred for some data.

Similarly `#help_index` and `#help_file` will not be available. These statements are completely deleted.

Sub-switch statements, multiple reserve/pad variable, noreg, and function flags are ignored.

Defines will technically allow you to create macros because preprocessor statements are not evaluated

Locks and memory functions like MSize or the ability to free NULL pointers will not be supported. Furthermore the semantics of bit extension for all calculations will likely not be supported.

One other such limitation is re-interpretting the ASM specification.
