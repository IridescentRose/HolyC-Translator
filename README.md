<h1 align="center">HolyC-Translator</h1>
<p align="center">Translates the language of our lord to commoner C for daily use</p>

---

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

One other such limitation is re-interpretting the ASM specification.

## To-Do

- [x] Tokenize Keywords in a single pass
- [ ] Tokenize Joint Symbols (+=) as a secondary pass
- [x] Tokenize all valid ASCII-based symbols
- [x] Token Metadata for error reporting (line position & cursor position)
- [x] Reorganize Tokenizer
- [x] Parser error handling improvements
- [ ] Reorganize Parser
- [ ] Parser can parse conditionals (if, else if, else, switch, while)
- [ ] Code generator can emit conditionals
- [ ] Parser can parse struct (class?)
- [ ] Code generator can emit structs/classes
- [ ] Parser can parse uniforms
- [ ] Code generator can emit uniforms
- [ ] Code generator can re-order function parameters (defaults)
- [ ] Parser type checking
- [ ] Parse sizeof & U0 0-length type semantics

