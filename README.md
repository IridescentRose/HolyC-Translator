<h1 align="center">HolyC-Translator</h1>
<p align="center">Translates the language of our lord to commoner C for daily use</p>

---

## How to Use

The translator can be simply used as follows:
```
> hc2c myFile.hc
```

## Why?

Holy C is obviously the best programming language, meant to follow the lord's will. The language was created by the (in)famous Terry A. Davis who recently passed away in 2018. As a tribute to Terry, it would be great to attempt to honor him with a more portable HolyC so that his glorious creation can find some more use.

## Specification and Limitation

The translator attempts to imitate or reproduce the behavior defined by the specifications of [HolyC](https://web.archive.org/web/20170325000321/http://www.templeos.org/Wb/Doc/HolyC.html) as documented here. However there are some critical limitations that a translator cannot do compared to a full on compiler. One of which is JIT / AOT mode as described by the original HolyC specification. The `#ifaot` and `#ifjit` preprocessor directives will be automatically excluded from the program. The #exe directive executes and records the results of commands executed in the shell. Similarly `#help_index` and `#help_file` will not be available. Another limitation is the reinterpreting of HolyC's ASM specification due to time and complexity restraints.
